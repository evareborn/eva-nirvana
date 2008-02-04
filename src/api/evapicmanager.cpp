/***************************************************************************
 *   Copyright (C) 2005 by yunfan                                          *
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
 
#include "evapicmanager.h"

#include "evauser.h"
#include "evausersetting.h"
#include "evanetwork.h"
#include "evaqtutil.h"
#include "libeva.h"

#include <stdio.h>
#include <unistd.h>
#include <qevent.h>
#include <qhostaddress.h>
#include <qtextcodec.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qmutex.h>
#include <qtimer.h>

#define QUEUE_INTERVAL 10000
EvaPicManager::EvaPicManager(EvaUser *u, bool useProxy)
{
	user = u;
	usingProxy = useProxy;
	isBusy = false;
	connecter = NULL;
	currentIndex = 0;
	codec = QTextCodec::codecForName("GB18030");
	EvaPicPacket::setQQ(user->getQQ());
	bufLength = 0;
	currentFile.buf = NULL;
	expectedSequence = 0;
	isSend = false;
	isAppending = false;
	isRemoving = false;
	
	outPool.setAutoDelete(true);
	
	timer = new QTimer(this, "sendOutQueue");
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(packetMonitor()));
}

EvaPicManager::~EvaPicManager()
{
	if(timer->isActive())
		timer->stop();
	delete timer;
	clearManager();
}

void EvaPicManager::setProxyServer( const QString ip, const short port, QCString proxyParam )
{
	proxyIP = ip;
	proxyPort = port;
	proxyAuthParam = proxyParam;
}

void EvaPicManager::customEvent( QCustomEvent * e )
{
	if(e->type() == EvaRequestCustomizedPicEvent){
		EvaAskForCustomizedPicEvent *se = (EvaAskForCustomizedPicEvent *)e;
		Session session;
		session.qunID = se->getQunID();
		session.list = se->getPicList();
		downloadList.push_back(session);
		//if(isBusy) printf("EvaPicManager::customEvent -- isBusy \n");
		if(!isBusy) doProcessEvent();
	}else if(e->type() == EvaSendPictureReadyEvent){
		EvaSendCustomizedPicEvent *se = (EvaSendCustomizedPicEvent *) e;
		OutSession session;
		session.qunID = se->getQunID();
		session.list = se->getPicList();
		sendList.push_back(session);
		if(!isBusy) doProcessOutEvent();
	}
}

void EvaPicManager::doProcessEvent()
{
	if(!downloadList.size()) {
		isBusy = false;
		if(sendList.size()) doProcessOutEvent();
		clearManager();
		printf("EvaPicManager::doProcessEvent -- downloadList size is ZERO, return\n");
		return;
	}
	isBusy = true;
	isSend = false;
	Session session = downloadList.front();
	downloadList.pop_front();
	qunID = session.qunID;
	picList = session.list;
	//printf("EvaPicManager::doProcessEvent \n");
	if(!picList.size()){
		isBusy = false;
		doProcessEvent();
		printf("EvaPicManager::doProcessEvent -- picList size is ZERO, return\n");
		return;
	}
	currentIndex = -1;
	if(!Packet::getFileAgentKey()) {
		clearManager();
		return;
	}
	EvaPicPacket::setFileAgentKey(Packet::getFileAgentKey());
	currentPic = *(picList.begin());
	initConnection(currentPic.ip, currentPic.port);
}

void EvaPicManager::initConnection(const int ip, const short port)
{
	//printf("EvaPicManager::initConnection ip:%s, port:%d\n", QHostAddress(ip).toString().ascii(), port);
	QHostAddress host;
	if(ip==-1)
		host.setAddress(QString(GROUP_FILE_AGENT));
	else
		host.setAddress(ip);
	sendIP = host.toIPv4Address();
	sendPort = port;
	if(connecter){
		delete connecter;
	}
	if(usingProxy){
		connecter = new EvaNetwork(QHostAddress(proxyIP), proxyPort, EvaNetwork::HTTP_Proxy);
		connecter->setDestinationServer(host.toString(), port);
		connecter->setAuthParameter(proxyAuthParam);
	}else
		connecter = new EvaNetwork(host, port, EvaNetwork::TCP);

	QObject::connect(connecter, SIGNAL(isReady()), SLOT(slotReady()));
	QObject::connect(connecter, SIGNAL(dataComming(int)), SLOT(slotDataComming(int)));
	QObject::connect(connecter, SIGNAL(exceptionEvent(int)), SLOT(slotException(int)));
	//printf("EvaPicManager::initConnection -- connecting....\n");
	connecter->connect();
}

void EvaPicManager::slotReady()
{	
	//printf("EvaPicManager::slotReady -- connection ready\n");
	if(!isSend)
		doRequestNextPic();
	else
		doRequestAgent();
}

void EvaPicManager::sendPacket(EvaPicOutPacket *packet)
{
	if(!connecter) {
		printf("EvaPicManager::sendPacket -- connecter NULL\n");
		return;
	}
	QMutex mutex;
	mutex.lock();	
	unsigned char *buf = new unsigned char[MAX_PACKET_SIZE];
	int len;
	packet->fill(buf, &len);
	bool result = connecter->write((char *)buf, len);
	delete buf;
	mutex.unlock();
	if(!result) slotException(-2);
}

void EvaPicManager::append(EvaPicOutPacket *packet)
{
	isAppending = true;
	sendPacket(packet);   // force to send
	if(packet->needAck())
		outPool.append(packet);
	else
		delete packet;
	isAppending = false;
	if(!timer->isActive())
		timer->start(QUEUE_INTERVAL, false);
}

void EvaPicManager::slotDataComming(int len)
{
	QMutex mutex;
	mutex.lock();
	char * rawData = new char [len+1];
	if(!connecter->read(rawData, len)){
		delete rawData;
		mutex.unlock();
		slotException(-1);
		return;
	}
	
	memcpy(buf + bufLength, rawData, len);
	bufLength += len;
	delete rawData;
	slotProcessBuffer();
	mutex.unlock();
}

void EvaPicManager::slotProcessBuffer( )
{
	if(buf[0] == FAMILY_05_TAG && buf[bufLength-1] == FAMILY_05_TAIL){
		EvaPicInPacket *packet = new EvaPicInPacket(buf, bufLength);
		unsigned int pLen = packet->getPacketLength();
		while(pLen <= bufLength){
			packet->cutOffPacketData();
			memcpy(buf, buf+pLen, bufLength-pLen);
			bufLength -= pLen;
			removePacket(packet->hashCode());
			parseInData(packet);
			delete packet;
			packet = new EvaPicInPacket(buf, bufLength);
			pLen = packet->getPacketLength();
		}
		delete packet;
	}
}

void EvaPicManager::parseInData(const EvaPicInPacket *in)
{
	if(in->isValid()){
		switch(in->getCommand()){
		case QQ_05_CMD_REQUEST_AGENT:
			processRequestAgentReply(in);
			break;
		case QQ_05_CMD_REQUEST_FACE:
			processRequestFaceReply(in);
			break;
		case QQ_05_CMD_TRANSFER:
			processTransferReply(in);
			break;
		case QQ_05_CMD_REQUEST_START:
			processRequestStartReply(in);
			break;
		}
	} else
		printf("EvaPicManager::parseInData -- invalid packet!\n");
}

void EvaPicManager::doRequestPic(CustomizedPic pic)
{
	if( pic.fileName.isEmpty() ){
		doRequestNextPic();
		return;
	}
	if(QFile::exists(user->getSetting()->getPictureCacheDir() + "/" + pic.fileName)){
		emit pictureReady(qunID, user->getSetting()->getPictureCacheDir() + "/" + pic.fileName, currentPic.tmpFileName);
		doRequestNextPic();
		return;
	}
	EvaRequestFacePacket *packet = new EvaRequestFacePacket();
	packet->setQunID(qunID);
	packet->setKey(pic.fileAgentKey);
	packet->setFileAgentToken(Packet::getFileAgentToken(), Packet::getFileAgentTokenLength());
	packet->setSessionID(pic.sessionID);
	sessionID = pic.sessionID;
	append(packet);
}

void EvaPicManager::doRequestData(CustomizedPic pic, const bool isReply)
{
	EvaPicTransferPacket *packet = new EvaPicTransferPacket();
	packet->setSessionID(pic.sessionID);
	packet->setDataReply(isReply);
	append(packet);
}

void EvaPicManager::slotException(int /*type*/)
{
	isRemoving = true;
	outPool.first();
	outPool.remove();
	isRemoving = false;
	//printf("EvaPicManager::slotException -- type %d\n", type);
	bufLength = 0;
	doRequestNextPic();
}

void EvaPicManager::processRequestAgentReply(const EvaPicInPacket *in)
{
	RequestAgentReplyPacket *packet = new RequestAgentReplyPacket(in->getRawBody(), in->getRawBodyLength());
	packet->parse();
	if(expectedSequence != packet->getSequence()){
		delete packet;
		return;
	}
	switch(packet->getReplyCode()){
	case QQ_REQUEST_AGENT_REPLY_OK:{
		sessionID = packet->getSessionID();
		//printf("EvaPicManager::processRequestAgentReply -- \n\tmessage:%s\n", packet->getMessage().c_str());
		if(currentIndex == -1)
			doRequestStart();
		else
			doSendFileInfo();
		}
		break;
	case QQ_REQUEST_AGENT_REPLY_REDIRECT:
		printf("EvaPicManager::processRequestAgentReply -- redirect\n");
		initConnection(packet->getRedirectIP(), packet->getRedirectPort());
		break;
	case QQ_REQUEST_AGENT_REPLY_TOO_LONG:
	default:
		clearManager();
		std::string str = packet->getMessage();
		printf("EvaPicManager::processRequestAgentReply -- agent error:%s\n", packet->getMessage().c_str());
		emit sendErrorMessage(qunID, codec->toUnicode(str.c_str()));
		break;
	}
	delete packet;
}

// We are not interested in this, so just ignore it :)
void EvaPicManager::processRequestFaceReply(const EvaPicInPacket * /*in*/)
{
	//printf("EvaPicManager::processRequestFaceReply -- got request face reply\n");
}

void EvaPicManager::processTransferReply(const EvaPicInPacket *in)
{
	EvaPicTransferReplyPacket *packet = new EvaPicTransferReplyPacket(in->getRawBody(), in->getRawBodyLength());
	packet->parse();
	if(packet->getLength() == 0) {
		fprintf(stderr, "EvaPicManager::processTransferReply -- bodyLength is Zero!!!\n");
		delete packet;
		return;
	}
	if(packet->getData()){
		if(!isSend){
			if(currentFile.lastPacketSeq!=0xffff && currentFile.lastPacketSeq >= packet->getSequence()){
				delete packet;
				return;
			}
			if(!currentFile.buf){
				delete packet;
				doRequestNextPic();
				return;
			}
			currentFile.lastPacketSeq = packet->getSequence();
			memcpy(currentFile.buf + currentFile.offset, packet->getData(), packet->getDataLength());
			currentFile.offset += packet->getDataLength();
			if(currentFile.offset >= currentFile.length){
				doSaveFile();
				doRequestNextPic();
				delete packet;
				return;
			}
			doRequestData(currentPic, true);
		} else {
			doSendNextFragment();
		}
	} else{
		currentFile.filename = codec->toUnicode(packet->getFileName().c_str());
		currentFile.length = packet->getImageLength();
		currentFile.buf = new unsigned char [ currentFile.length ];
		memset(currentFile.buf, 0, currentFile.length);
		currentFile.offset = 0;
		currentFile.lastPacketSeq = 0xffff;
		doRequestData(currentPic, false);
	}
	delete packet;
}

void EvaPicManager::processRequestStartReply(const EvaPicInPacket *in)
{
	EvaRequestStartReplyPacket *packet = new EvaRequestStartReplyPacket(in->getRawBody(), in->getRawBodyLength());
	packet->parse();
	if(sessionID != packet->getSessionID()){
		delete packet;
		return;
	}
	if(isSend){
		currentIndex++;// now we start
		doSendFileInfo();
	}
	delete packet;
}

void EvaPicManager::doRequestNextPic()
{
	currentIndex++;
	if(currentIndex >= (int)picList.size()){
		picList.clear();
		doProcessEvent();
		return;
	}
	
	currentFile.offset = 0;
	if(currentFile.buf) 
		delete currentFile.buf; 
	currentFile.buf = NULL;
	currentFile.lastPacketSeq = 0xffff;
	int index = 0;
	std::list<CustomizedPic>::iterator iter;
	for(iter = picList.begin(); iter!=picList.end(); ++iter){
		if(index == currentIndex){
			break;
		}
		index++;
	}
	if(iter != picList.end()){
		currentPic = *iter;
		doRequestPic(currentPic);
	}
}

void EvaPicManager::doSaveFile()
{
	QString fileName = user->getSetting()->getPictureCacheDir() + "/" + currentFile.filename;
	QFile file(fileName);
	if(!file.open(IO_WriteOnly | IO_Raw )){
		printf("EvaPicManager::doSaveFile -- cannot open file \'%s\'!\n", fileName.ascii());
		return;
	}
	
	QDataStream stream(&file);
	stream.writeRawBytes((char *)currentFile.buf, currentFile.offset);
	file.close();
	if(currentFile.buf)
		delete currentFile.buf;
	currentFile.buf = NULL;
	currentFile.offset = 0;
	currentFile.lastPacketSeq = 0xffff;
	emit pictureReady(qunID, fileName, currentPic.tmpFileName);
}

void EvaPicManager::doRequestAgent()
{
	EvaRequestAgentPacket *packet = new EvaRequestAgentPacket(Packet::getFileAgentToken(), Packet::getFileAgentTokenLength());
	packet->setQunID(qunID);
	packet->setMd5(currentOutPic.md5);
	packet->setImageLength(currentOutPic.imageLength);
	packet->setFileName(std::string((codec->fromUnicode(currentOutPic.fileName)).data()));
	expectedSequence = packet->getSequence();
	append(packet);
}

void EvaPicManager::doRequestStart( )
{
	EvaRequestStartPacket *packet = new EvaRequestStartPacket();
	packet->setSessionID(sessionID);
	packet->setMd5(currentOutPic.md5);
	append(packet);
}

void EvaPicManager::doSendFileInfo( )
{	
	currentFile.filename = currentOutPic.fileName.right(currentOutPic.fileName.length() - currentOutPic.fileName.findRev("/") -1);
	currentFile.length = currentOutPic.imageLength;
	currentFile.offset = 0;
	currentFile.buf = new unsigned char[currentFile.length];
	QFile file(currentOutPic.fileName);
	if(!file.open(IO_ReadOnly | IO_Raw )){
		printf("EvaPicManager::doSendFileInfo -- cannot open file \'%s\' !\n", currentOutPic.fileName.ascii());
		return;
	}
	
	QDataStream stream(&file);
	stream.readRawBytes((char *)currentFile.buf, currentFile.length);
	file.close();
	
	EvaPicTransferPacket *packet = new EvaPicTransferPacket(false, false);
	packet->setSessionID(sessionID);
	packet->setMd5(currentOutPic.md5);
	packet->setImageLength(currentFile.length);
	packet->setFileName(std::string((codec->fromUnicode(currentFile.filename)).data()));
	expectedSequence = packet->getSequence();
	append(packet);
}

void EvaPicManager::doProcessOutEvent( )
{
	if(!sendList.size()) {
		isBusy = false;
		if(downloadList.size()) doProcessEvent();
		return;
	}
	isBusy = true;
	isSend = true;
	OutSession session = sendList.front();
	sendList.pop_front();
	qunID = session.qunID;
	outList = session.list;
	if(!outList.size()){
		isBusy = false;
		doProcessOutEvent( );
		return;
	}
	currentIndex = -1;
	if(!Packet::getFileAgentKey()) {
		clearManager();
		return;
	}
	EvaPicPacket::setFileAgentKey(Packet::getFileAgentKey());
	currentOutPic = *(outList.begin());
	initConnection(-1, 443);
}

void EvaPicManager::doSendNextFragment()
{
	if(currentFile.length <= currentFile.offset){
		currentIndex++;
		if(currentIndex >= (int)(outList.size())){
			doProcessOutEvent();
			return;
		}
		currentFile.offset = 0;
		if(currentFile.buf) 
			delete currentFile.buf; 
		currentFile.buf = NULL;
		int index = 0;
		std::list<OutCustomizedPic>::iterator iter;
		for(iter = outList.begin(); iter!=outList.end(); ++iter){
			if(index == currentIndex){
				break;
			}
			index++;
		}
		if(iter != outList.end()){
			currentOutPic = *iter;
			doSendFileInfo();
		}
	}
	for(int i=0; i<10; i++){
		bool isLast = ( (currentFile.length - currentFile.offset) <= 1024 );
		unsigned int len = isLast?((currentFile.length - currentFile.offset)):1024;
		EvaPicTransferPacket *packet = new EvaPicTransferPacket(true, isLast);
		packet->setSessionID(sessionID);
		packet->setFragment(currentFile.buf + currentFile.offset, len);
		currentFile.offset += len;
		append(packet);
		if(isLast) {
			if((currentIndex+1) == (int)(outList.size())){
				outList.clear();
				emit pictureSent(qunID, sessionID, sendIP, sendPort);
				return;
			}
			break;
		}
	}
}

void EvaPicManager::clearManager()
{
	outPool.clear();
	outList.clear();
	currentIndex = -1;
	bufLength = 0;
	isSend = false;
	isBusy = false;
	if(connecter){
		connecter->close();
		delete connecter;
		connecter = NULL;
	}
}

void EvaPicManager::removePacket( const int hashCode )
{
	isRemoving = true;
	QMutex mutex;
	mutex.lock();
	EvaPicOutPacket *packet;
	for(packet=outPool.first(); packet; packet = outPool.next()){
		if(packet->hashCode() == hashCode)
			outPool.remove();
	}
	mutex.unlock();
	isRemoving = false;
}

void EvaPicManager::packetMonitor( )
{
	if(isAppending || isRemoving ) return;
	for ( uint i=0;  i < outPool.count(); i++ ){
		if(outPool.at(i)->needResend()){
			sendPacket(outPool.at(i));
		}else{
			removePacket(outPool.at(i)->hashCode());
			isBusy = false;
			fprintf(stderr, "EvaPicManager::packetMonitor -- time out\n");
			return;
		}
		if(isAppending || isRemoving ) break;
	}
}

void EvaPicManager::stop( )
{
	if(timer->isActive())
		timer->stop();
	clearManager();
}
