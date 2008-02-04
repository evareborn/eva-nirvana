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
 
#include "evanetwork.h"
#include "evasocket.h"

EvaNetwork::EvaNetwork(const QHostAddress &host, const short port, const Type type)
 	:socket(NULL)
{
	this->type = type;
	switch(type){
	case UDP:
		socket = new EvaSocket(host, port);  // default is UDP
		QObject::connect(socket, SIGNAL(isReady()), this, SIGNAL(isReady()));
		QObject::connect(socket, SIGNAL(writeReady()), SIGNAL(writeReady()));
		QObject::connect(socket, SIGNAL(receivedData(int)), this, SIGNAL(dataComming(int)));
		QObject::connect(socket, SIGNAL(exceptionEvent(int)), this, SLOT(processProxyEvent(int)));
		break;
	case TCP:
		socket = new EvaSocket(host, port, EvaSocket::TCP);
		QObject::connect(socket, SIGNAL(isReady()), this, SIGNAL(isReady()));
		QObject::connect(socket, SIGNAL(writeReady()), SIGNAL(writeReady()));
		QObject::connect(socket, SIGNAL(receivedData(int)), this, SIGNAL(dataComming(int)));
		QObject::connect(socket, SIGNAL(exceptionEvent(int)), this, SLOT(processProxyEvent(int)));
		break;
	case HTTP_Proxy:
		socket = new EvaHttpProxy(host, port);
		QObject::connect(socket, SIGNAL(proxyWriteReady()), SIGNAL(writeReady()));
		QObject::connect(socket, SIGNAL(proxyEvent(int)), this, SLOT(processProxyEvent(int)));
		QObject::connect(socket, SIGNAL(dataArrived(int)), this, SIGNAL(dataComming(int)));
		QObject::connect(socket, SIGNAL(socketException(int)), this, SIGNAL(exceptionEvent(int)));
		break;
	default:
		socket = new EvaSocket(host, port);  // default is UDP
		break;
	}
}

EvaNetwork::~EvaNetwork()
{
	if(socket) delete socket;
}
 
void EvaNetwork::setServer(const QHostAddress &address, const short port)
{
	socket->setHost(address, port);	
}

const QHostAddress &EvaNetwork::getHostAddress() const
{
	return socket->getHostAddress();
}

const short EvaNetwork::getHostPort() const
{
	return socket->getHostPort();
}

void EvaNetwork::setDestinationServer(const QString &server, const short port) // for Http Proxy only;
{
	if(type != HTTP_Proxy) return;
	
	((EvaHttpProxy*)(socket))->setDestinationServer(server, port);
}

void EvaNetwork::setAuthParameter(const QString &username, const QString &password)
{
 	 ((EvaHttpProxy*)(socket))->setAuthParameter(username, password);
}

void EvaNetwork::setAuthParameter(const QCString &param)
{
	((EvaHttpProxy*)(socket))->setBase64AuthParam(param);
}

void EvaNetwork::newURLRequest()
{
	((EvaHttpProxy*)(socket))->tcpReady();
}

void EvaNetwork::connect()
{
	socket->startConnecting();
}

bool EvaNetwork::read(char *buf, int len)
{
	return socket->read(buf, len);
}

bool EvaNetwork::write(const char *buf, const int len)
{
	return socket->write(buf, len);
}

void EvaNetwork::setWriteNotifierEnabled(bool enabled)
{
	socket->setWriteNotifierEnabled(enabled);
}

void EvaNetwork::processProxyEvent(int num)
{
	if(type != HTTP_Proxy){
		switch(num){
		case EvaSocket::Init:
		case EvaSocket::Connecting:
		case EvaSocket::Ready:
		case EvaSocket::Failed:
		case EvaSocket::None:
		case EvaSocket::BytesReadWrong:
			emit exceptionEvent(num);
			break;
		}
        }else{
        	switch(num){
		case EvaHttpProxy::Proxy_None:
		case EvaHttpProxy::Proxy_TCP_Ready:
		case EvaHttpProxy::Proxy_Connecting:
			break;
		case EvaHttpProxy::Proxy_Ready:
			emit isReady();
			break;
		case EvaHttpProxy::Proxy_Need_Auth:
			emit exceptionEvent(Proxy_Need_Auth);
			break;
		case EvaHttpProxy::Proxy_Read_Error:
			emit exceptionEvent(Proxy_Read_Error);
			break;
		case EvaHttpProxy::Proxy_Error:
			emit exceptionEvent(Proxy_Error);
			break;
		}
        }
}

void EvaNetwork::close( )
{
	socket->closeConnection();
}

const QHostAddress EvaNetwork::getSocketIp( )
{
	if(socket) return socket->getSocketAddress();
	return QHostAddress();
}

const unsigned int EvaNetwork::getSocketPort( )
{
	if(socket) return socket->getSocketPort();
	return 0;
}
