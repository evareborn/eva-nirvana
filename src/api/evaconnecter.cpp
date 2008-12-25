/***************************************************************************
 *   Copyright (C) 2004 by yunfan                                          *
 *   yunfan_zg@163.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "evaconnecter.h"
#include "evaresource.h"
#include "evanetwork.h"
#include "evaservers.h"
#include "evaqtutil.h"
#include "evahtmlparser.h"
#include "evaimsend.h"
#include "evaqun.h"
#include "evalogintoken.h"
#include <qtimer.h> 
#include <qmutex.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <stdlib.h>
#include <cstring>
#include <QApplication>
//X #include <kdebug.h>

#define POOL_CHECK_INTERVAL  2000    /// every 2 second check two pools

		
EvaConnecter::EvaConnecter(const EvaNetworkPolicy& policy)
    : policy( policy ), network( NULL )
{

    fetchQQServer();

    memset(buffer, 0, 65535);
    packetLength = 0;
    bufLength = 0;
    connectionReady = false;
    isClientSetup = false;

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(packetMonitor()));

}

EvaConnecter::~EvaConnecter()
{
    printf( "EvaConnecter:: free!!!!!\n" );
	delete network;
	if(timer->isActive())
		timer->stop();
	delete timer;
        while ( inPool.size() ) {
            InPacket* packet = inPool.last();
            inPool.pop_back();
            delete packet;
        }
        while ( outPool.size() ) {
            OutPacket* packet = outPool.last();
            outPool.pop_back();
            delete packet;
        }

}

void EvaConnecter::fetchQQServer()
{
	EvaServers *server = EvaGlobal::getInstance()->getEvaServers();
//X         slotGotServer( server->getDefaultAddress( ) );
//X 	QObject::disconnect(server, 0, 0, 0);
	QObject::connect(server, SIGNAL(isReady(QHostAddress)), this, SLOT(slotGotServer(QHostAddress)));
	server->fetchAddress(policy.getConnectionType() == CONN_UDP ? true: false);
}

void EvaConnecter::slotGotServer(QHostAddress addr)
{
    printf( "server ip: %s\n", addr.toString().latin1() );
    printf( "type: %d\n", policy.getConnectionType( ) );
    if ( network ) return;
    EvaGlobal::getInstance()->getEvaServers()->stopDns();

    switch(policy.getConnectionType()){
        case CONN_UDP:
            network = new EvaNetwork(addr, 8000, policy);
            break;
        case CONN_TCP:
            network = new EvaNetwork(addr, 80, policy);
            break;
        case HTTP_Proxy:
            network = new EvaNetwork(addr, 443, policy);
    }

    printf( "connected to server: %s:%d\n", network->getHostAddress().toString().latin1(), network->getHostPort() );
    QObject::connect(network, SIGNAL(isReady()), this, SLOT(isReadySlot()));
    QObject::connect(network, SIGNAL(dataComming(int)), this, SLOT(dataCommingSlot(int)));
    QObject::connect(network, SIGNAL(exceptionEvent(int)), this, SLOT(slotNetworkException(int)));

    emit isNetworkReady();

    //X         outPool.setAutoDelete(true);
    //X         inPool.setAutoDelete(false);

}
void EvaConnecter::append(OutPacket *out)
{
	sendOut(out);   // force to send
	if(out->needAck()){
		outPool.append(out);
		if(!timer->isActive())     // start out/in pool checking timer
			timer->start(POOL_CHECK_INTERVAL, false);
	}else
		delete out;
}

InPacket *EvaConnecter::getInPacket()
{
	if(inPool.count() <= 0 ){ 
		emit packetException(-1);
		return NULL;
	}
        InPacket *packet = inPool.last();
        inPool.pop_back();
	return packet;
}

void EvaConnecter::redirectTo(const int ip, const short port)
{
    if ( ! network ) {
        return;
    }
    m_IsDetecting = true;  // set the detecting flag
    inPool.clear();
    outPool.clear();
    if(policy.getConnectionType()!= HTTP_Proxy){
        connectionReady = false;
        network->setServer(QHostAddress(ip), port==-1?network->getHostPort():port);
    }else{
        //X 		network->setServer(network->getHostAddress(), network->getHostPort());
        network->redirectTo(QHostAddress(ip).toString(), 443); // always 443 for http proxy
    }

    //X 	kdDebug() << "[EvaConnecter->redirectTo] "<< QHostAddress(ip).toString() << " : " << (port==-1?network->getHostPort():port) <<endl;
    connect();
    printf( "redirected to server: %s:%d\n", network->getHostAddress().toString().latin1(), network->getHostPort() );
}

void EvaConnecter::connect()
{
    if ( !network ) {
        return;
    }
    memset(buffer, 0, 65535);
    packetLength = 0;
    bufLength = 0;
    network->connect();
}

void EvaConnecter::stop()
{
	if(timer->isActive())
		timer->stop();
	//QTimer::singleShot(200, this, SLOT(clearAllPools()));
	clearAllPools();
	memset(buffer, 0, 65535);
	packetLength = 0;
	bufLength = 0;
	connectionReady = false;
	isClientSetup = false;

//X 	kdDebug() << "[EvaConnecter] stoped" << endl;
}

void EvaConnecter::clearAllPools()
{
	inPool.clear();
	outPool.clear();
}

void EvaConnecter::sendOut( OutPacket *out)
{
	if(timer->isActive())
		timer->stop();
	if(!connectionReady) return;
	unsigned char *buf = (unsigned char *)malloc(MAX_PACKET_SIZE * sizeof(unsigned char));
	int len;
	
	out->fill(buf, &len);
	network->write((char *)buf, len);
	free(buf);
	if(!timer->isActive())
		timer->start(POOL_CHECK_INTERVAL, false);
}

void EvaConnecter::removePacket(const int hashCode)
{
	QMutex mutex;
	mutex.lock();
	for( int i = 0; i < outPool.count(); i++){
            OutPacket* packet = outPool.at( i );
		if(packet->hashCode() == hashCode){
			outPool.removeAt(i);
                        delete packet;
			--i;
		}
	}
	mutex.unlock();
}

void EvaConnecter::removeOutRequests(const short cmd)
{
	for( int i = 0; i < outPool.count(); i++){
            OutPacket* packet = outPool.at( i );
		if(packet->getCommand() == cmd){
			outPool.removeAt(i);
                        delete packet;
			--i;
		}
	}
}

void EvaConnecter::isReadySlot()
{
    printf( "ready server: %s:%d\n", network->getHostAddress().toString().latin1(), network->getHostPort() );
    connectionReady = true;

    if(m_IsDetecting)
        startDetecting();
    else{
        emit isReady();
    }
}

void EvaConnecter::startDetecting( )
{
	append( new ServerDetectorPacket);
}

void EvaConnecter::processDetectReply( InPacket * in )
{
	ServerDetectorReplyPacket *packet = new ServerDetectorReplyPacket();
	packet->setInPacket(in);
	if( !packet->parse()) {
//X 		kdDebug() << "[EvaConnecter] server detect reply parse error!" << endl;
		delete packet;
		return;
	}

	if(packet->isServerReady()){
		m_IsDetecting = false;
        	removePacket(packet->hashCode());
//X 		kdDebug() << "[EvaConnecter] server " << network->getHostAddress().toString() << " is ready." << endl;
		delete packet;
		emit isReady();
		return;
	}else if(packet->needRedirect()){
        		removePacket(packet->hashCode());
			ServerDetectorPacket::nextStep();
			ServerDetectorPacket::setFromIP(network->getHostAddress().toIPv4Address());
			redirectTo( packet->getRedirectIP(), -1);
		}else{
//X 			kdDebug() << "[EvaConnecter] unkown server detect reply ( reply code: " << packet->getReplyCode() << ")" << endl;
		}

	delete packet;	
}


void EvaConnecter::dataCommingSlot(int len)
{
	char *rawData = new char[len+1];
	if(!network->read(rawData, len)){
		fprintf(stderr, "--Eva Connecter: Bytes read wrong!\n");
		return;
	}
	if(policy.getConnectionType() != CONN_UDP){
		memcpy(buffer+bufLength, rawData, len);
		bufLength += len;
		delete []rawData;
		unsigned short tmp;
		memcpy(&tmp, buffer, 2);
		packetLength = ntohs(tmp);
		while(bufLength >= packetLength){
			rawData = new char[packetLength];
			memcpy(rawData, buffer, packetLength);
			memcpy(buffer, buffer + packetLength, bufLength - packetLength);
			len = packetLength;
			bufLength -=packetLength;
			processPacket(rawData, len);
			delete []rawData;
			if(!bufLength)	break;
			memcpy(&tmp, buffer, 2);
			packetLength = ntohs(tmp);
		}
		
		
	}else{
		processPacket(rawData, len);
		delete []rawData;
	}
}

void EvaConnecter::processPacket( char * data, int len )
{
	InPacket *packet = new InPacket((unsigned char *)data, len);
	if(!packet->getLength()){
//X 		kdDebug() << "[EvaConnecter] Bad packet (cmd: " << packet->getCommand() << "), ignore it" << endl;
		delete packet;
		return;
	}

	if(packet->getCommand() == QQ_CMD_SERVER_DETECT){
		processDetectReply(packet);
		delete packet;
		return;
	}

        removePacket(packet->hashCode());
	
	// for the case of keep alive, once we got one, we could ignore all keep alive
	// packets in the outPool
	if(packet->getCommand() == QQ_CMD_KEEP_ALIVE)
		removeOutRequests(QQ_CMD_KEEP_ALIVE);

	// same reason as above
	if(packet->getCommand() == QQ_CMD_GET_FRIEND_ONLINE)
		removeOutRequests(QQ_CMD_GET_FRIEND_ONLINE);

        inPool.append(packet);

	//NOTE: the if condition needs more consideration
//X 	if(isClientSetup || (!isClientSetup && packet->getCommand()!= QQ_CMD_RECV_IM && 
//X 				packet->getCommand() != QQ_CMD_RECV_MSG_SYS          &&
//X 				packet->getCommand() != QQ_CMD_RECV_MSG_FRIEND_CHANGE_STATUS))
        emit newPacket();
//X 	else if(!isClientSetup)
//X 		emit clientNotReady();
}

void EvaConnecter::packetMonitor()
{
	if(!connectionReady)  return;
	for ( int i=0;  i < outPool.count(); i++ ){
		if(outPool.at(i)->needResend()){
			sendOut(outPool.at(i));
		}else{
			short cmd = outPool.at(i)->getCommand();
			if(cmd == QQ_CMD_SEND_IM){
				SendIM *im = dynamic_cast<SendIM *>(outPool.at(i));
				if(im)
					emit sendMessage(im->getReceiver(), false);
				else
					emit packetException( cmd);
			} else if( cmd == QQ_CMD_QUN_CMD ){
					QunPacket *qun = dynamic_cast<QunPacket *>(outPool.at(i));
					if(qun){
						char qunCmd = qun->getQunCommand();
						if(qunCmd == QQ_QUN_CMD_SEND_IM || qunCmd == QQ_QUN_CMD_SEND_IM_EX)
							emit sendQunMessage(qun->getQunID(), false, QString::null);
						else
							emit packetException(cmd);
					} else
						emit packetException(cmd);
				} else
					emit packetException(cmd);
			if(!connectionReady)  return;
			removePacket(outPool.at(i)->hashCode());
			if(!outPool.count() && !inPool.count() && timer)	timer->stop();
			return;
		}
	}
	if(isClientSetup && inPool.count()>0){
		emit newPacket();
	}
	if(!outPool.count() && !inPool.count() && timer)	timer->stop();
}

void EvaConnecter::slotClientReady( )
{
	isClientSetup = true;
	while(inPool.count()){
		emit newPacket();
		qApp->processEvents();
	}
}

QHostAddress EvaConnecter::getSocketIp( ) const
{
    /**
     * We will reesolve this name ambigious later.
     */

	if(network) return network->getHostAddress();
	return QHostAddress();
}

short EvaConnecter::getSocketPort( ) const
{
	if(network) return network->getHostPort();
	return 0;
}

void EvaConnecter::slotNetworkException( int num )
{
	printf("EvaConnecter::slotNetworkException: %d\n", num);
	if(num == EvaNetwork::Failed || 
		num == EvaNetwork::Proxy_Read_Error ||
		num == EvaNetwork::Proxy_Error){
		fprintf(stderr, "Received a network error, stop EvaConnecter now\n");
		stop();
		emit networkException(num);
	}
}

