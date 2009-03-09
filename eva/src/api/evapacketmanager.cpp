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

#include "evapacketmanager.h"
#include "evausersetting.h"
#include "evaconnecter.h"
#include "evaloginmanager.h"
#include "evacontactmanager.h"
#include "evamain.h"
#include "evaapi.h"
#include <qapplication.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <qtextcodec.h>
#include <kdebug.h>
#include <stdlib.h>
#include <map>

EvaPacketManager::EvaPacketManager(EvaUser *user, EvaConnecter *connecter)
{
	codec = QTextCodec::codecForName("GB18030");
	this->user = user;
	addingBuddyQQ = 0;
	deletingBuddyQQ = 0;
	processQunID = 0;
	qunMemberInfoFinished = false;
	OutPacket::setQQ(user->getQQ());
	Packet::setPasswordKey((unsigned char *)(user->getMd5Password()));
	this->connecter = connecter;
	Packet::setUDP(connecter->getConnectionType() == EvaNetwork::UDP);	
	// connet the signal of connecter so that I will know the network is ready
	QObject::connect(connecter, SIGNAL(isReady()), SLOT(slotConnectReady()));
	QObject::connect(connecter, SIGNAL(newPacket()), SLOT(newPacketSlot()));
	QObject::connect(connecter, SIGNAL(networkException(int)), SLOT(networkExceptionSlot(int)));
	QObject::connect(connecter, SIGNAL(packetException(int)), SLOT(packetExceptionSlot(int)));
	QObject::connect(connecter, SIGNAL(clientNotReady()), SLOT(slotClientNotReady()));

	timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), SLOT(timerSlot()) );
}

EvaPacketManager::~EvaPacketManager()
{
	if(timer->isActive()){
		timer->stop();
	}
	delete timer;
	if(user) delete user;
	if(connecter) delete connecter;
}

void EvaPacketManager::newPacketSlot()
{
	InPacket *packet = connecter->getInPacket();
	if(packet==NULL){
		fprintf(stderr, "grab packet error: NULL pointer!\n");
		return;
	}
	parsePacket(packet);
}

void EvaPacketManager::networkExceptionSlot(int exp)
{
	printf("network exception: %d\n", exp);
	emit networkException(exp);
}

void EvaPacketManager::packetExceptionSlot(int cmd)
{
	printf("packet exception: (cmd)0x%4x\n",cmd);
	emit packetException(cmd);
}

void EvaPacketManager::timerSlot()
{
	connecter->append(new KeepAlivePacket());
	receivedPacketList.clear();
	receivedCacheList.clear();
}

void EvaPacketManager::redirectTo(const unsigned int ip, const short port)
{
	connecter->redirectTo(ip, port);
}

void EvaPacketManager::saveBuddyQQ(const unsigned int id, const short seq)
{
	std::map<short, int>::iterator iter = sendRemoveCacheList.find(seq);
	if( iter != sendRemoveCacheList.end()) return ;
	
	sendRemoveCacheList[seq] = id;
}

void EvaPacketManager::removeSequence(const short seq)
{
	std::map<short, int>::iterator iter = sendRemoveCacheList.find(seq);
	if( iter == sendRemoveCacheList.end()) return;
	sendRemoveCacheList.erase(iter);
}

const int EvaPacketManager::getSavedBuddyQQ(const short seq)
{
	std::map<short, int>::iterator iter = sendRemoveCacheList.find(seq);
	if(iter == sendRemoveCacheList.end()) return -1;
	
	return sendRemoveCacheList[seq];
}

void EvaPacketManager::parsePacket(InPacket *packet)
{
	int index = receivedPacketList.findIndex(packet->hashCode());
	if(index != -1 && packet->getCommand() != QQ_CMD_RECV_IM) {
		delete packet;
		return;
	}
	receivedPacketList.append(packet->hashCode());
	
	switch(packet->getCommand()){
	case QQ_CMD_LOGIN:
		processLoginReply(packet);
		break;
	case QQ_CMD_KEEP_ALIVE:
		processKeepAliveReply(packet);
		break;
	case QQ_CMD_GET_USER_INFO:
		printf("user info\n"); 
		processGetUserInfoReply(packet);
		break;
	case QQ_CMD_CHANGE_STATUS:
		printf("change status reply\n");
		processChangeStatusReply(packet);
		break;
	case QQ_CMD_GET_FRIEND_LIST:
		printf("friend list\n");
		processGetFriendListReply(packet);
		break;
	case QQ_CMD_GET_FRIEND_ONLINE:
		//printf("online-friend list\n");
		processGetOnlineFriendReply(packet);
		break;
	case QQ_CMD_SEND_IM:
		printf("send im reply\n");
		processSendIMReply(packet);
		break;
	case QQ_CMD_RECV_IM:
		printf("received new message\n");
		processReceiveIM(packet);
		break;
	case QQ_CMD_RECV_MSG_FRIEND_CHANGE_STATUS:
		printf("friend change status\n");
		processFriendChangeStatus(packet);
		break;
	case QQ_CMD_GROUP_NAME_OP:
		printf("upload/download group names reply\n");
		processGroupNameOp(packet);
		break;
	case QQ_CMD_DOWNLOAD_GROUP_FRIEND:
		printf("grouped friends reply\n");
		processDownloadGroupFriendReply(packet);
		break;	
	case QQ_CMD_UPLOAD_GROUP_FRIEND:
		printf("upload grouped friend reply\n");
		processUploadGroupFriendReply(packet);
		break;
	case QQ_CMD_DELETE_FRIEND:
		printf("delete friend reply\n");
		processDeleteFriendReply(packet);
		break;
	case QQ_CMD_ADD_FRIEND:
		printf("add friend reply\n");
		processAddFriendReply(packet);
		break;
	case QQ_CMD_ADD_FRIEND_AUTH:
		printf("add friend auth reply\n");
		processAddFriendAuthReply(packet);
		break;				
	case QQ_CMD_RECV_MSG_SYS:
		printf("system message\n");
		processSystemNotification(packet);
		break;
	case QQ_CMD_MODIFY_INFO:
		printf("received modify info\n");
		processModifyInfoReply(packet);
		break;
	case QQ_CMD_SEARCH_USER:
		printf("search reply\n");
		processSearchUserReply(packet);
		break;
	case QQ_CMD_DELETE_ME:
		printf("delete me reply\n");
		processDeleteMeReply(packet);
		break;
	case QQ_CMD_REQUEST_LOGIN_TOKEN:
		printf("got login token\n");
		processRequestLoginTokenReply(packet);
		break;
	case QQ_CMD_REQUEST_LOGIN_TOKEN_EX:
		printf("got login token ex\n");
		processRequestLoginTokenExReply(packet);
		break;
	case QQ_CMD_QUN_CMD:
		printf("got Qun operation reply\n");
		processQunReply(packet);
		break;
	case QQ_CMD_GET_LEVEL:
		printf("got level reply\n");
		processGetLevelReply(packet);
		break;
	case QQ_CMD_REQUEST_KEY:
		printf("got keys reply\n");
		processRequestKeyReply(packet);
		break;
	case QQ_CMD_REQUEST_EXTRA_INFORMATION:
		printf("got extra info reply\n");
		processRequestExtraInfoReply(packet);
		break;
	case QQ_CMD_SIGNATURE_OP:
		printf("got signature op reply\n");
		processSignatureReply(packet);
		break;
	case QQ_CMD_MEMO_OP:
		printf("got memo op reply\n");
		processMemoReply(packet);
		break;
	case QQ_CMD_ADVANCED_SEARCH:
		printf("advanced search reply\n");
		processAdvancedSearchReply(packet);
		break;
	case QQ_CMD_ADD_FRIEND_EX:
		printf("add friend ex reply\n");
		processAddFriendExReply(packet);
		break;
	case QQ_CMD_ADD_FRIEND_AUTH_EX:
		printf("add friend auth ex reply\n");
		processAddFriendAuthExReply(packet);
		break;
	case QQ_CMD_ADD_FRIEND_AUTH_INFO:
		printf("auth info reply\n");
		processAddFrendAuthInfoReply(packet);
		break;
	case QQ_CMD_ADD_FRIEND_AUTH_QUESTION:
		printf("auth question reply\n");
		processAddFriendAuthQuestionReply(packet);
		break;
	case QQ_CMD_VERIFY_ADDING_MSG:
		printf("verify adding message reply\n");
		processVerifyAddingMsgReply(packet);
		break;
	case QQ_CMD_WEATHER:
		printf("weather request reply\n");
		processWeatherOpReply(packet);
	}
        delete packet;
}

void EvaPacketManager::doRequestLoginToken()
{
	connecter->append(new RequestLoginTokenPacket());
}

void EvaPacketManager::doLogin()
{
	if(! Packet::isLoginTokenSet()){
		//doRequestLoginToken();
		doRequestLoginTokenEx();
		return;
	}
	unsigned char mode = QQ_LOGIN_MODE_INVISIBLE;
	if(user->getStatus() == EvaUser::Eva_Online)
		mode = QQ_LOGIN_MODE_NORMAL;
	Packet::setPasswordKey((unsigned char *)(user->getMd5Password()));

	LoginPacket *packet = new LoginPacket(mode);
	packet->setNumProcess(1); // we simple set it to 1 for now
	packet->setComputerUUID(Computer_UUID); // use hard-coded UUID, might change it to follow QQ's logic
	connecter->append(packet);
}

//Due to the protocol update, this method will never be called any more
void EvaPacketManager::processRequestLoginTokenReply(const InPacket *in)
{
	if(Packet::isLoginTokenSet()) return;
	RequestLoginTokenReplyPacket *packet = new RequestLoginTokenReplyPacket();
	packet->setInPacket(in);
	bool isWrong = packet->parse();
	delete packet;
	if(!isWrong){
		emit serverBusy();
		return;
	}
	//user->loginManager()->finishedCommand(QQ_CMD_REQUEST_LOGIN_TOKEN);
	// here, we've got login token, therefore, try to log in
	doLogin();
}

void EvaPacketManager::processLoginReply(const InPacket *in)
{
	// if client key is set, the user has logged in.
	if(Packet::isClientKeySet()) 
		return;
	//if(user->loginManager()->isCommandFinished(QQ_CMD_LOGIN))
	//	return;

	LoginReplyPacket *packet = new LoginReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()) { 
		printf("parse error\n"); 
		delete packet;
		//emit serverBusy(); // if parse error, we just emit a serverBusy signal so that we can have another try
		return;
	}
	switch(packet->getReplyCode())
	{
	case QQ_LOGIN_REPLY_OK:
		{
		kdDebug() << "[EvaPacketManager] user \"" << user->getQQ()
				<< "\" logged in from "
				<< QHostAddress(packet->getMyIP()).toString()
				<< ":" << packet->getMyPort()<< endl;
// 		printf("EvaPacketManager::processLoginReply -- \n");
// 		for(int i=0; i<packet->getLength(); i++){
// 			if(!(i%8)) printf("\n%d: ",i);
// 			char t = packet->getBody()[i];
// 			printf("%02x ", (uint8_t)t);
// 		}
// 		printf("\n----====== Login Successfully ======----\n\n");
		user->setLoginWanIp(packet->getMyIP());
		user->setLoginWanPort(packet->getMyPort());
		user->setLastLoginIp(packet->getLastLoginIP());
		user->setLastLoginTime(packet->getLastLoginTime());
		user->setLoginLanIp(connecter->getSocketIp().toIPv4Address());
		user->setLoginLanPort(connecter->getSocketPort());
		
		//user->loginManager()->finishedCommand(QQ_CMD_LOGIN);
		///connecter->append(new ChangeStatusPacket(EvaUser::getStatusCode(user->getStatus())));
		///connecter->append(new EvaRequestKeyPacket(QQ_REQUEST_FILE_AGENT_KEY));
		//connecter->append(new EvaRequestKeyPacket(QQ_REQUEST_UNKNOWN_KEY));
		if(user->isBuddiesLoaded())
			connecter->append(new GetOnlineFriendsPacket());
		if(!timer->isActive()){
			timer->start(60000, false);
		}else
			printf("impossible! \n");
		//emit loginOK();
		GetLoginManager()->loginOK();
		}
		break;
	case QQ_LOGIN_REPLY_REDIRECT:
	case QQ_LOGIN_REPLY_REDIRECT_EX:
		printf("EVA redirect to: %8x, %d\n", packet->getRedirectedIP(), packet->getRedirectedPort());
		if(packet->getRedirectedIP() == 0) {
			emit serverBusy();
		}else{
			//ServerDetectorPacket::nextStep();
			//ServerDetectorPacket::setFromIP(connecter->getHostAddress().toIPv4Address());
			//redirectTo(packet->getRedirectedIP(),packet->getRedirectedPort());
			GetLoginManager()->loginNeedRedirect(connecter->getHostAddress().toIPv4Address(), 
								   packet->getRedirectedIP(),
								   packet->getRedirectedPort());
// 			emit loginNeedRedirect(connecter->getHostAddress().toIPv4Address(), 
// 								   packet->getRedirectedIP(),
// 								   packet->getRedirectedPort());
		}
		break;
	case QQ_LOGIN_REPLY_PWD_ERROR:
	case QQ_LOGIN_REPLY_NEED_REACTIVATE:
		printf("something wrong:%s\n",codec->toUnicode(packet->getReplyMessage().c_str()).local8Bit().data());
		GetLoginManager()->wrongPassword( codec->toUnicode(packet->getReplyMessage().c_str()));
		//emit wrongPassword(codec->toUnicode(packet->getReplyMessage().c_str()));
		break;
	case QQ_LOGIN_REPLY_PWD_ERROR_EX:
		GetLoginManager()->wrongPassword( codec->toUnicode(packet->getReplyMessage().c_str()));
		break;
	case QQ_LOGIN_REPLY_MISC_ERROR:
		printf("some unknown error:%s\nhaving another try ...\n",packet->getReplyMessage().c_str());
		emit serverBusy();
		break;
        default:
                printf("unknown reply code\n");
	}
	delete packet;
}

void EvaPacketManager::doRequestFileAgentKey()
{
	connecter->append(new EvaRequestKeyPacket(QQ_REQUEST_FILE_AGENT_KEY));
}

void EvaPacketManager::doLogout( )
{
	if(user->getStatus() != EvaUser::Eva_Offline){
		if(connecter->isConnectionReady()){
			for(int i=0; i<4; i++) {
				connecter->append(new LogoutPacket());
			}
		}
		user->setStatus(EvaUser::Eva_Offline);
	}
	if(timer->isActive())
		timer->stop();
	connecter->stop();
	
	longMsgPacketQueue.clear();
	Packet::clearAllKeys();
	emit logoutOK();
}

void EvaPacketManager::doGetUserInfo( const unsigned int id )
{
	connecter->append(new GetUserInfoPacket(id));
}

void EvaPacketManager::processGetUserInfoReply( const InPacket * in )
{
	GetUserInfoReplyPacket *packet = new GetUserInfoReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	ContactInfo info = packet->contactInfo();
	std::string strID = info.at(ContactInfo::Info_qqID);
	unsigned int id = atoi(strID.c_str());
	if(GetLoginManager()->isLoggedIn()){
		GetContactManager()->processGetUserInfoReply( packet);
	} else {
		if(id == user->getQQ())
			GetLoginManager()->myInfoReady(info);
	}
	 

// 	if( id == user->getQQ()){
// 		user->setDetails(info);
// 		user->loginManager()->finishedCommand(QQ_CMD_GET_USER_INFO);
// 		emit myInfoReady();
// 	}else{
// 		user->getFriendList().addContactInfoTo(id, info);
// 	}
// 	delete packet;
// 	user->saveGroupedBuddyList();
	emit userInfoReady(convertDetails(info));
	delete packet;
}

void EvaPacketManager::doChangeStatus( EvaUser::UserStatus newStatus )
{
	user->setStatus( newStatus );
	connecter->append(new ChangeStatusPacket(EvaUser::getStatusCode(newStatus)));
}

void EvaPacketManager::processChangeStatusReply( const InPacket * in )
{
	ChangeStatusReplyPacket *packet = new ChangeStatusReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	if(packet->isAccepted()){  // if change status failed,  in most cases it's because of connection problem, 
		//user->loginManager()->finishedCommand(QQ_CMD_CHANGE_STATUS);    // so we simply emit a offline signal
		switch(user->getStatus()){
		case EvaUser::Eva_Online:
			emit onlineReady();
			break;
		case EvaUser::Eva_Offline:
			if(timer->isActive())
				timer->stop();
			connecter->stop();
			Packet::clearAllKeys();
			emit offlineReady();
			break;	
		case EvaUser::Eva_Invisible:
			emit invisibleReady();
			break;
		case EvaUser::Eva_Leave:
			emit leaveReady();
			break; 
		}
	}else{
		if(timer->isActive())
			timer->stop();
		connecter->stop();
		emit offlineReady();    // if we meet any problem, just emit offline signal
	}
	delete packet;
}

void EvaPacketManager::doGetOnlineFriends()
{
	connecter->append(new GetOnlineFriendsPacket());
}

void EvaPacketManager::doGetAllFriends( )
{
	connecter->append(new GetFriendListPacket());
}

void EvaPacketManager::processGetFriendListReply( const InPacket * in )
{
	GetFriendListReplyPacket *packet = new GetFriendListReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	GetContactManager()->processGetFriendListReply( packet);
// 	friendItemList gotList = packet->getFriendList();
// 	friendItemList::iterator iter;
// 	for(iter = gotList.begin(); iter!= gotList.end(); ++iter){
// 		QQFriend frd;
// 		frd.setFriendItem(*iter);
// 		user->getFriendList().addFriend(frd);
// 	}
// 	if(packet->getPosition()!=QQ_FRIEND_LIST_POSITION_END){
// 		connecter->append(new GetFriendListPacket(packet->getPosition()));
// 	}else{
// 		emit friendListReady(); // EvaMain class will set the loginManager
// 	}
	delete packet;
}

void EvaPacketManager::doSendMessage( const unsigned int receiver, const bool isNormal, QString & message, const char fontSize, 
					const bool u, const bool i, const bool b, 
					const char blue, const char green, const char red )
{	
// 	if(!user->loginManager()->isReady()) {
// 		printf("Client Not ready, ignore sending request!\n");
// 		emit sentMessageResult(receiver, false);
// 		return;
// 	}
	if(!GetLoginManager()->isLoggedIn()){
		printf("Client Not ready, ignore sending request!\n");
		emit sentMessageResult(receiver, false);
		return;
	}
	
	QQFriend * frd = user->getFriendList().getFriend(receiver);
	if(!frd){
		printf("Friend \"%d\" not in list, sending ignored\n", receiver);
		emit sentMessageResult(receiver, false);
		return;
	}
	QString rNick = codec->toUnicode( frd->getNick().c_str());
	QString sNick = codec->toUnicode(user->getDetails().at(ContactInfo::Info_nick).c_str());
	QDateTime time = QDateTime::currentDateTime(Qt::LocalTime);	
	user->getSetting()->saveMessage(receiver,  user->getQQ(), sNick, receiver, rNick,
				isNormal, message, time, fontSize, u, i, b, blue, green, red);
	
	QCString str = codec->fromUnicode(message);
	unsigned short pcs = strlen(str.data()) / 700;
	SendTextIMPacket *firstPacket = 0;
	int lastSeq = -1;
	for (unsigned short pcsn=0; pcsn<=pcs; pcsn++)
	{
		SendTextIMPacket *packet = new SendTextIMPacket();
		packet->setNumFragments(pcs + 1);
		packet->setSeqOfFragments(pcsn);	
		
		packet->setReceiver(receiver);
		//packet->setMsgSequence(frd->getNextSequence());
		packet->setFontSize(fontSize);
		packet->setUnderline(u);
		packet->setItalic(i);
		packet->setBold(b);
		packet->setBlue(blue);
		packet->setGreen(green);
		packet->setRed(red);
		packet->setAutoReply(isNormal);
		packet->setMessage(str.mid(pcsn * 700, 700).data());
		if(!pcsn) {
			firstPacket = packet;
			lastSeq = firstPacket->getSequence();
		} else {
			longMsgPacketQueue[lastSeq] = packet; // record the next packet
			lastSeq = packet->getSequence();
			if(pcsn == (pcs - 1))  // the last sent packet points to NULL
				longMsgPacketQueue[lastSeq] = NULL;
		}
		//qApp->processEvents();
	}
	saveBuddyQQ(receiver, firstPacket->getSequence());
	connecter->append(firstPacket);
}

void EvaPacketManager::processSendIMReply( const InPacket * in )
{
	SendIMReplyPacket *packet = new SendIMReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	int id = getSavedBuddyQQ(packet->getSequence());
	if(id==-1) return;
	removeSequence(packet->getSequence());

	// check if there are more sub packets to send
	std::map<int, SendTextIMPacket*>::iterator iter = longMsgPacketQueue.find(packet->getSequence());
	if(iter == longMsgPacketQueue.end()){
		// should be one-shot message
		emit sentMessageResult(id, packet->isSentOK());
	} else {
		// got it in the long message queue
		SendTextIMPacket *next = iter->second;  // next packet
		
		// erase the last sent sequence entry
		longMsgPacketQueue.erase(packet->getSequence());

		if(packet->isSentOK()){
			// if sent ok, try to send next
			int receiver = next->getReceiver();
			saveBuddyQQ(receiver, next->getSequence());
			connecter->append(next);
			printf("next fragment sent!\n");
		}else{
			// if one of the long message got error back, chuck all fragments
			int nextSeq = next->getSequence();
			while(next){
				longMsgPacketQueue.erase(next->getSequence());
				next = longMsgPacketQueue[nextSeq];
				nextSeq = next->getSequence();
			}
			emit sentMessageResult(id, false);
		}
	}

	delete packet;
}

void EvaPacketManager::processKeepAliveReply( const InPacket * in )
{
	KeepAliveReplyPacket *packet = new KeepAliveReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	numOnlineUsers = packet->numOnlineUsers();
	delete packet;
	connecter->append(new GetOnlineFriendsPacket());
}

void EvaPacketManager::processGetOnlineFriendReply( const InPacket * in )
{
	GetOnlineFriendReplyPacket *packet = new GetOnlineFriendReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	onlineList list = packet->getOnlineFriendList();
	onlineList::iterator iter;
	for(iter = list.begin(); iter!=list.end(); ++iter){
		user->getFriendList().addOnlineFriendEntryTo(iter->getQQ(), *iter);
	}
	if(packet->getPosition() != QQ_FRIEND_ONLINE_LIST_POSITION_END){
		connecter->append(new GetOnlineFriendsPacket(packet->getPosition()));
	} else
		emit friendListReady();
	delete packet;
}

void EvaPacketManager::processFriendChangeStatus( const InPacket * in )
{
	FriendChangeStatusPacket *packet = new FriendChangeStatusPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	
	//user->getFriendList().updateFriendIP(packet->getQQ(), packet->getIP());
	//user->getFriendList().updateFriendPort(packet->getQQ(), packet->getPort());

	const QQFriend *frd = (user->getFriendList()).getFriend(packet->getQQ());
	
	if( frd && (frd->getStatus() != packet->getStatus()) ){
		user->getFriendList().updateFriendStatus(packet->getQQ(), packet->getStatus());
		emit friendStatusChanged(packet->getQQ());
	}
	delete packet;
}

void EvaPacketManager::processReceiveIM( const InPacket * in )
{
	ReceiveIMPacket *packet = new ReceiveIMPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		printf(" ----- parse error!\n");
		delete packet;
		return;
	}
	printf("IM: seq: %d ----- msg seq: %d\n", packet->getIntSequence(), packet->getSequence());
	ReceiveIMReplyPacket *packetReply = new ReceiveIMReplyPacket(packet->getReplyKey());
	packetReply->setSequence(packet->getSequence());
	connecter->append(packetReply);
	
	int index = receivedCacheList.findIndex(packet->hashCode());
	if(index != -1 ){
		printf("Duplicated message, ignore\n");
		delete packet;
		return;
	}
	
	receivedCacheList.append(packet->hashCode());

	switch(packet->getIMType()){
	case 0x0010:{
		int len = packet->getBodyLength();
		const unsigned char *buf = packet->getBodyData();
		printf("COMMAND: 0X0010s\n");
		for(int i=0; i<len; i++){
			if(!(i%8)) printf("\n");
			char t = buf[i];
			printf("%2x ", (uint8_t)t);
		}
		printf("\n");
		}
		break;
	case QQ_RECV_IM_NEWS:{
		ReceivedQQNews *news = new ReceivedQQNews(packet->getBodyData(), packet->getBodyLength());
		emit receivedQQNews(codec->toUnicode(news->getTitle().c_str()),
					codec->toUnicode(news->getBrief().c_str()),
					codec->toUnicode(news->getURL().c_str()));
		}
		break;
	case QQ_RECV_IM_FROM_BUDDY_2006:
	case QQ_RECV_IM_FROM_UNKNOWN_2006:
	case QQ_RECV_IM_TO_BUDDY:
	case QQ_RECV_IM_TO_UNKNOWN:{
		NormalIMBase *base = new NormalIMBase(packet->getBodyData(), packet->getBodyLength());	
		base->parseData();
		printf("sender version: 0x%4x\n", base->getSenderVersion());
		switch(base->getNormalIMType())
		{
		case QQ_IM_NORMAL_TEXT:{
			for(int i=0; i<packet->getLength(); i++){
				if(!(i%8)) printf("\n%d: ",i);
				char t = packet->getBody()[i];
				printf("%2x ", (uint8_t)t);
			}
			printf("\n----====== Normal Text Finished ======----\n\n");
			ReceivedNormalIM * received = new ReceivedNormalIM();
			received->setNormalIMBase(base);
			received->parseData();
			const QQFriend *frd = user->getFriendList().getFriend(received->getSender());
			if(frd==NULL){
				QQFriend f(received->getSender(),received->getSenderFace());
				f.setGroupIndex(EvaUser::getAnonymousIndex());
				user->getFriendList().addFriend(f);
				user->saveGroupedBuddyList();
				emit addAnonymous(received->getSender(),received->getSenderFace());
			}else	if(frd->getGroupIndex() == EvaUser::getBlackIndex()){
					delete received;
					delete packet;
					return;
				}
			
			//check fragments
			if (received->getNumFragments()>1)
			{
				unsigned short ropID = received->getMessageID();
				
				//push fragment inormation into map
				unsigned short total = received->getNumFragments();
				pcMsgCache[ropID].total = total;
				pcMsgCache[ropID].content[received->getSeqOfFragments()] = received->getMessage().c_str();
				
				if (total == pcMsgCache[ropID].content.size())
				{
					//all fragments received
					QString allText = "";
					for (int idx=0; idx<total; idx++) 
						allText += pcMsgCache[ropID].content[idx];
					pcMsgCache.erase(ropID);
					received->setMessage(allText);
				} else
				{
					delete received;
					delete packet;
					return;
				}
				
			}
			//user->getFriendList().updateFriendIP(packet->getSender(), packet->getSenderIP());
			//user->getFriendList().updateFriendPort(packet->getSender(), packet->getSenderPort());
			user->getFriendList().updateFriendFileSessionKey(packet->getSender(),received->getBuddyFileSessionKey());
			user->getFriendList().getFriend(received->getSender())->setSequence(base->getSequence());
			QString msg = codec->toUnicode(received->getMessage().c_str());
			QDateTime time;
			time.setTime_t(received->getSendTime());
			
			int sender = received->getSender();
			bool isNormal = received->isNormalReply();
			char fontSize = received->getFontSize();
			bool u = received->isUnderline();
			bool i = received->isItalic();
			bool b = received->isBold();
			char blue = received->getBlue();
			char green = received->getGreen();
			char red = received->getRed();
			if(received->getReplyType() == QQ_IM_IMAGE_REPLY) isNormal = true;
			QString sNick = codec->toUnicode(user->getFriendList().getFriend(sender)->getNick().c_str());
			QString rNick = codec->toUnicode(user->getDetails().at(ContactInfo::Info_nick).c_str());
			user->getSetting()->saveMessage(sender, sender, sNick, user->getQQ(), rNick, 
						isNormal, msg, time, fontSize, u, i, b, blue, green, red);
			printf("From Buddy -- %s (%d): <BEGIN>%s<END>\n", sNick.local8Bit().data(), sender, msg.local8Bit().data());
			emit txtMessage(sender, isNormal, msg, time, fontSize, 
					u, i, b, blue, green, red);
			delete received;
			}
			break;
		case QQ_IM_UDP_REQUEST:
			printf("EvaPacketManager::processReceiveIM -- QQ_IM_UDP_REQUEST <- old version used\n");
			for(int i=0; i<packet->getLength(); i++){
				if(!(i%8)) printf("\n%d: ",i);
				char t = packet->getBody()[i];
				printf("%2x ", (uint8_t)t);
			}
			printf("\n----====== QQ_IM_UDP_REQUEST Finished ======----\n\n");
			break;
		case QQ_IM_TCP_REQUEST:
		case QQ_IM_ACCEPT_UDP_REQUEST:
		case QQ_IM_NOTIFY_IP:
		case QQ_IM_REQUEST_CANCELED:
			fprintf(stderr, "Received file command from \"%d\"(v:%d). Command not supported, Buddy might use a client earlier than QQ2005 beta1!\n",
					packet->getSender(), 0xffff&packet->getVersion());
			break;
		case QQ_IM_NOTIFY_FILE_AGENT_INFO:
		case QQ_IM_EX_UDP_REQUEST:
		case QQ_IM_EX_REQUEST_CANCELLED:
		case QQ_IM_EX_REQUEST_ACCEPTED:{
			printf("EvaPacketManager::processReceiveIM -- Here we got\n");
			for(int i=0; i<packet->getLength(); i++){
				if(!(i%8)) printf("\n%d: ",i);
				char t = packet->getBody()[i];
				printf("%2x ", (uint8_t)t);
			}
			printf("\n----====== Before parseData ======----\n\n");

			ReceivedFileIM * received = new ReceivedFileIM();
			received->setNormalIMBase(base);
			received->parseData();
			if(received->getTransferType() != QQ_TRANSFER_FILE && 
				received->getTransferType() != QQ_TRANSFER_IMAGE ){
				fprintf(stderr, "EvaPacketManager::processReceiveIM -- unknown transfer type: %2x\n", 0xff & received->getTransferType());
				delete received;
				break;
			}
			printf("EvaPacketManager::processReceiveIM -- TransferType : 0x%2x\n", 
								received->getTransferType());
			printf("EvaPacketManager::processReceiveIM -- ConnectMode : 0x%2x\n", 
								received->getConnectMode());
			printf("EvaPacketManager::processReceiveIM -- WanIp : 0x%8x\n", 
								received->getWanIp());
			printf("EvaPacketManager::processReceiveIM -- WanPort : 0x%8x\n", 
								received->getWanPort());
			printf("EvaPacketManager::processReceiveIM -- file name : %s\n", 
								received->getFileName().c_str());
			printf("EvaPacketManager::processReceiveIM -- file size : %d\n", 
								received->getFileSize());
			printf("EvaPacketManager::processReceiveIM -- file seq : %d\n", 
								0xffff & received->getSequence());
			//printf("EvaPacketManager::processReceiveIM -- MajorPort : 0x%4x\n", 
			//					received->getMajorPort());
			//printf("EvaPacketManager::processReceiveIM -- LanIp : 0x%8x\n", 
			//					received->getLanIp());
			//printf("EvaPacketManager::processReceiveIM -- LanPort : 0x%4x\n", 
			//					received->getLanPort());
			user->getFriendList().updateFriendIP(packet->getSender(), received->getWanIp());
			user->getFriendList().updateFriendPort(packet->getSender(), received->getWanPort());
			for(int i=0; i<packet->getLength(); i++){
				if(!(i%8)) printf("\n%d: ",i);
				char t = packet->getBody()[i];
				printf("%2x ", (uint8_t)t);
			}
			printf("\n----====== Lots Msg Finished ======----\n\n");
			switch(base->getNormalIMType()){
			case QQ_IM_EX_UDP_REQUEST:
				emit receivedFileRequest(packet->getSender(), received->getSessionId(), 
						codec->toUnicode(received->getFileName().c_str()),
						received->getFileSize(), received->getTransferType());
				break;
			case QQ_IM_EX_REQUEST_ACCEPTED:
				emit receivedFileAccepted(packet->getSender(), received->getSessionId(), 
							received->getWanIp(), true, received->getTransferType());
				break;
			case QQ_IM_EX_REQUEST_CANCELLED:
				printf("EvaPacketManager::processReceiveIM -- cmd canceled\n");
				emit receivedFileAccepted(packet->getSender(), received->getSessionId(), 
							received->getWanIp(), false, received->getTransferType());
				break;
			case QQ_IM_NOTIFY_FILE_AGENT_INFO:{
				emit receivedFileAgentInfo(packet->getSender(), received->getSequence(), received->getSessionId(), 
							received->getWanIp(), received->getWanPort(), received->getAgentServerKey());
				}
				break;
			}
			delete received;
			}
			break;
		case QQ_IM_EX_NOTIFY_IP:{
			printf("EvaPacketManager::processReceiveIM -- QQ_IM_EX_NOTIFY_IP -- \n");
			for(int i=0; i<packet->getLength(); i++){
				if(!(i%8)) printf("\n%d: ",i);
				char t = packet->getBody()[i];
				printf("%2x ", (uint8_t)t);
			}
			printf("\n----====== QQ_IM_EX_NOTIFY_IP Before parseData ======----\n\n");
			ReceivedFileExIpIM * received = new ReceivedFileExIpIM();
			received->setNormalIMBase(base);
			received->parseData();
			printf("EvaPacketManager::processReceiveIM -- TransferType : 0x%2x\n", 
								received->getTransferType());
			printf("EvaPacketManager::processReceiveIM -- ConnectMode : 0x%2x\n", 
								received->getConnectMode());
			printf("EvaPacketManager::processReceiveIM -- WanIp1 : 0x%8x\n", 
								received->getWanIp1());
			printf("EvaPacketManager::processReceiveIM -- WanPort1 : 0x%8x\n", 
								received->getWanPort1());
			printf("EvaPacketManager::processReceiveIM -- syn ip : 0x%8x\n", 
								received->getSyncIp());
			printf("EvaPacketManager::processReceiveIM -- syn port : 0x%8x\n", 
								received->getSyncPort());
			printf("EvaPacketManager::processReceiveIM -- session : 0x%8x\n", 
								received->getSyncSession());
			if(received->isSender()) printf("isSender -- true\n");
			else printf("isSender -- false\n");
			emit receivedFileNotifyIpEx(packet->getSender(), received->isSender(),
						received->getSessionId(), received->getTransferType(),
						received->getWanIp1(), received->getWanPort1(),
						received->getWanIp2(), received->getWanPort2(),
						received->getWanIp3(), received->getWanPort3(),
						received->getLanIp1(), received->getLanPort1(),
						received->getLanIp2(), received->getLanPort2(),
						received->getLanIp3(), received->getLanPort3(),
						received->getSyncIp(), received->getSyncPort(), received->getSyncSession()
						);
			delete received;
			}
			break;
		default:
			printf("Got a non-text msg, can't process it, ignore it\n");
			for(int i=0; i<packet->getLength(); i++){
				if(!(i%8)) printf("\n%d: ",i);
				char t = packet->getBody()[i];
				printf("%2x ", (uint8_t)t);
			}
			printf("\n----====== Unknown Normal IM Finished ======----\n\n");
		}
		delete base;
		}
		break;
	case QQ_RECV_IM_SYS_MESSAGE:{
		ReceivedSystemIM *sys = new ReceivedSystemIM(packet->getBodyData(), packet->getBodyLength());		
		switch(sys->getSystemIMType())
		{
		case QQ_RECV_IM_KICK_OUT:
			printf("this account has logged in somewhere, you are kicked out!\n");
			if(timer->isActive())
				timer->stop();
			connecter->stop();
			emit kickedOut(codec->toUnicode(sys->getMessage().c_str()));
			break;
		default:
			printf("Got a system msg: %s\n", sys->getMessage().c_str());
		}
		delete sys;
		}
		break;
	case QQ_RECV_IM_QUN_IM:
	case QQ_RECV_IM_TEMP_QUN_IM:
	case QQ_RECV_IM_UNKNOWN_QUN_IM:{
		ReceivedQunIM *im = new ReceivedQunIM(packet->getIMType(), packet->getBodyData(), packet->getBodyLength());
		Qun *qun = user->getQunList()->getQun(packet->getSender());
		if(!qun){
			Qun q(packet->getSender());
			user->getQunList()->add(q);
			doRequestQunInfo(packet->getSender());
			qun = user->getQunList()->getQun(packet->getSender());
			emit qunRequestUpdateDisplay();
		}else if( im->getVersionID() != qun->getDetails().getVersionID()){
				printf("Qun version changed to: %d\n", im->getVersionID());
				//doRequestQunInfo(packet->getSender());
				GetContactManager()->fetchQunDetails(packet->getSender());
			}
		if(qun->getMessageType() == Qun::Reject){
			delete im;
			return;
		}
		if(im->getSenderQQ() == user->getQQ()){
			printf("Qun -- my words, ignore\n");
			delete im;
			return;
		}
		
		
		
		//check fragments
		if (im->getNumFragments()>1)
		{
			unsigned short ropID = im->getMessageID();
			
			//push fragment inormation into map
			unsigned short total = im->getNumFragments();
			pcMsgCache[ropID].total = total;
			pcMsgCache[ropID].content[im->getSeqOfFragments()] = im->getMessage().c_str();
			
			if (total == pcMsgCache[ropID].content.size())
			{
				//all fragments received
				QString allText = "";
				for (int idx=0; idx<total; idx++) 
					allText += pcMsgCache[ropID].content[idx];
				pcMsgCache.erase(ropID);
				im->setMessage(allText);
			} else
			{
				delete im;
				delete packet;
				return;
			}
			
		}
		QString msg = codec->toUnicode(im->getMessage().c_str());
		QDateTime time;
		time.setTime_t(im->getSentTime());
		// we cannot use  im->getQunID() to get qun id, because it's only for tempory qun's id
		int qunId = packet->getSender();
		int sender = im->getSenderQQ();
		char fontSize = im->getFontSize();
		bool u = im->isUnderline();
		bool i = im->isItalic();
		bool b = im->isBold();
		char blue = im->getBlue();
		char green = im->getGreen();
		char red = im->getRed();
		
		qun = user->getQunList()->getQun(qunId);
		QString qName = "";
		QString sNick = QString::number(sender);
		if(qun){
			QunInfo info = qun->getDetails();
			qName = codec->toUnicode(info.getName().c_str());
			const FriendItem *item = qun->getMemberDetails(sender);
			if(item){
				sNick = codec->toUnicode(item->getQunRealName().c_str());
				if(sNick.isEmpty())
					sNick = codec->toUnicode(item->getNick().c_str());
			}
		}
		QString rNick = codec->toUnicode(user->getDetails().at(ContactInfo::Info_nick).c_str());
		user->getSetting()->saveMessage(qunId,  sender, sNick, user->getQQ(), rNick,
					true, msg, time, fontSize, u, i, b, blue, green, red, true);
		QString qunName = qun ? (codec->toUnicode(qun->getDetails().getName().c_str())) : ("Not In List");
		printf("Qun -- %s--%s(%d) : %s\n", qunName.local8Bit().data(), sNick.local8Bit().data(), sender, msg.local8Bit().data());
		if(qun->getMessageType() != Qun::RecordOnly)
			emit qunTxtMessage(qunId, sender, msg, time, fontSize, u, i, b, blue, green, red);

		delete im;
		}
		break;
	case QQ_RECV_IM_CREATE_QUN:
	case QQ_RECV_IM_ADDED_TO_QUN:
	case QQ_RECV_IM_DELETED_FROM_QUN:// these three type of notifications has no messages
	case QQ_RECV_IM_REQUEST_JOIN_QUN:
	case QQ_RECV_IM_APPROVE_JOIN_QUN:
	case QQ_RECV_IM_REJECT_JOIN_QUN:
	case QQ_RECV_IM_SET_QUN_ADMIN:{
		ReceivedQunIMJoinRequest *join = new ReceivedQunIMJoinRequest(packet->getIMType(),
									packet->getBodyData(),
									packet->getBodyLength());
		QString  msg = codec->toUnicode(join->getMessage().c_str());
		if(packet->getIMType() == QQ_RECV_IM_REQUEST_JOIN_QUN){
			if(user->getSetting()->isInQunRejectForever( packet->getSender(), join->getSender())){
				delete join;
				break;
			}
		}

		emit qunSysMessage(packet->getSender(), packet->getIMType(), join->getExtID(), join->getType(),
			join->getSender(), join->getCommander(), msg, join->getCode(), join->getCodeLength(),
			join->getToken(), join->getTokenLength());

		printf(" imtype: 0x%2x, type: %d --- external id: %d, sender: %d,commander: %d, msg:%s\n",
										packet->getIMType(),
										join->getType(),
										join->getExtID(),
										join->getSender(),
										join->getCommander(),
										join->getMessage().c_str());

		delete join;
		break;
		/////////////TODO: remove following later on =====================
		if( packet->getIMType()== QQ_RECV_IM_APPROVE_JOIN_QUN || packet->getIMType()== QQ_RECV_IM_ADDED_TO_QUN){
			if(join->getSender() == user->getQQ()){
				user->getQunList()->add(Qun(packet->getSender()));
				user->saveQunList();
				emit qunSystemMessageRequest(join->getSender(), msg);
				emit qunRequestUpdateDisplay();
			}else{
				// show information in the Qun chat window
				emit qunJoinEvent(packet->getSender(), packet->getIMType(), join->getSender(), join->getCommander());
			}
			doRequestQunInfo(packet->getSender());
		} else if( packet->getIMType()== QQ_RECV_IM_DELETED_FROM_QUN){
				if( join->getSender() == user->getQQ()) {
					user->getQunList()->remove(packet->getSender());
					user->saveQunList();
					emit qunSystemMessageRequest(join->getSender(), msg);
					emit qunRequestUpdateDisplay();
				}else {
					doRequestQunInfo(packet->getSender());
					// show information in the Qun chat window
					emit qunJoinEvent(packet->getSender(), packet->getIMType(), join->getSender(), join->getCommander());
				}
			} else {
					emit qunSystemMessageRequest(join->getSender(), msg);
				}
		delete join;
		///////// ===============================
		}
		break;
	case QQ_RECV_IM_SIGNATURE_CHANGED:{
		SignatureChangedPacket *sig = new SignatureChangedPacket(packet->getBodyData(), packet->getBodyLength());
		user->getFriendList().setSignature(sig->getQQ(), sig->getSignature(), sig->getTime());
		user->saveGroupedBuddyList();
		QString sigMsg = codec->toUnicode(sig->getSignature().c_str());
		QDateTime time;
		time.setTime_t(sig->getTime());
		emit friendSignatureChanged(sig->getQQ(), time, sigMsg);
		break;
		}
	default:{
		printf("Unknow IM Type: 0x%2x\n", packet->getIMType());
		printf("sender: %d, receiver: %d, sender IP: %s, sender port: %d\n", packet->getSender(), packet->getReceiver(),
								QHostAddress(packet->getSenderIP()).toString().ascii(), packet->getSenderPort());
		short version = packet->getVersion();
		short command = packet->getCommand();
		short sequence = packet->getSequence();
		short bodyLength = packet->getLength();
		printf("version:0x%4x, command: 0x%4x, sequence: 0x%4x\n", version, command, sequence);
		printf("after InPacket headr(7(UDP) or 9(TCP) bytes) plus ReceiveIMPacket header(20 bytes):\n");
		for(int i=0; i<bodyLength; i++){
			if(!(i%8)) printf("\n%d: ",i);
			char t = packet->getBody()[i];
			printf("%2x ", (uint8_t)t);
		}
		printf("\n============================\n");  
		}
	}
	delete packet;
}

void EvaPacketManager::doGetGroups( )
{
	printf("EvaPacketManager::doGetGroups\n");
	connecter->append(new GroupNameOpPacket(QQ_DOWNLOAD_GROUP_NAME));
}

void EvaPacketManager::doUploadGroups()
{
	GroupNameOpPacket *packet = new GroupNameOpPacket(QQ_UPLOAD_GROUP_NAME);
	packet->setGroups(user->getGroupNames());
	connecter->append(packet);
}

void EvaPacketManager::processGroupNameOp( const InPacket * in )
{
	GroupNameOpReplyPacket *packet = new GroupNameOpReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	if(packet->isDownloadReply()){
		GetContactManager()->processDownloadGroupName(packet);
// 		std::list<std::string>::iterator iter;
// 		std::list<std::string> names = packet->getGroupNames();
// 		user->clearGroupNames(); // note that the first group won't be deleted
// 		for(iter = names.begin(); iter != names.end(); ++iter){
// 			user->newGroup(*iter);
// 		}
// 		doGetGroupedFriends();
	}else{
		std::map<unsigned int, QQFriend> srcList = user->getFriendList().getFriendList();
		std::map<unsigned int, QQFriend>::iterator iter;
		std::map<unsigned int, unsigned int> destList;
		for(iter = srcList.begin(); iter!= srcList.end(); ++iter){
			if((iter->second).getGroupIndex() != user->getAnonymousIndex() &&
				(iter->second).getGroupIndex() != user->getBlackIndex()){
				destList[(iter->second).getQQ()] = (iter->second).getGroupIndex();
			}
		}
		UploadGroupFriendPacket *packet = new UploadGroupFriendPacket();
		packet->setGroupedFriends(destList);
		connecter->append(packet);
	}
	delete packet;
}

void EvaPacketManager::doGetGroupedFriends(int start )
{
	printf("EvaPacketManager::doGetGroupedFriends \n");
	connecter->append(new DownloadGroupFriendPacket(start));
}

void EvaPacketManager::processDownloadGroupFriendReply( const InPacket * in )
{
	DownloadGroupFriendReplyPacket *packet = new DownloadGroupFriendReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}

	GetContactManager()->processDownloadGroupFriendReply(packet);
		
// 	std::list<DownloadFriendEntry> friends = packet->getGroupedFriends();
// 	std::list<DownloadFriendEntry>::iterator iter;
// 	for(iter = friends.begin(); iter!= friends.end(); ++iter){
// 		if(!iter->isQun()){
// 			user->getFriendList().updateFriendGroupIndex(iter->getQQ(), iter->getGroupID());
// 		}else{
// 			Qun q(iter->getQQ());
// 			//printf("------------- Qun : %d\n", iter->getQQ());
// 			user->getQunList()->add(q);
// 		}
// 	}
// 	
// 	int nextID = packet->getNextStartID();
// 	if(nextID != 0x0000){
// 		connecter->append(new DownloadGroupFriendPacket(nextID));
// 	}else{
// 		emit friendGroupsReady();
// 		QunList *qunList = user->getQunList();
// 		if(qunList && qunList->numQuns()){
// 			doRequestQunInfo(qunList->getQunList().begin()->getQunID());
// 		}
// 	}
	delete packet;
}

void EvaPacketManager::processUploadGroupFriendReply(const InPacket *in)
{
	UploadGroupFriendReplyPacket *packet = new UploadGroupFriendReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	if(packet->uploadOk()) 
		emit friendGroupsUploadReady(true);
	else
		emit friendGroupsUploadReady(false);
	
	delete packet;
}

void EvaPacketManager::doDeleteBuddy( const unsigned int id )
{
	if(deletingBuddyQQ)
		return;
	deletingBuddyQQ = id;
	DeleteFriendPacket *packet = new DeleteFriendPacket(id);
	saveBuddyQQ(id, packet->getSequence());
	connecter->append(packet);
}

void EvaPacketManager::processDeleteFriendReply( const InPacket * in )
{
	DeleteFriendReplyPacket *packet = new DeleteFriendReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	if(packet->isDeleted()){
		user->getFriendList().deleteFriend(deletingBuddyQQ);
		user->saveGroupedBuddyList();
	}
	deletingBuddyQQ = 0;
	int id = getSavedBuddyQQ(packet->getSequence());
	if(id==-1) {
		delete packet;
		return;
	}
	removeSequence(packet->getSequence());
	emit deleteBuddyReady(id, packet->isDeleted());	
	delete packet;
}

void EvaPacketManager::doAddBuddy( const unsigned int id )
{
	if(addingBuddyQQ) {
		emit addingBuddy(addingBuddyQQ);
		return;
	}
	addingBuddyQQ = id;
	connecter->append(new AddFriendPacket(id));
}

void EvaPacketManager::processAddFriendReply( const InPacket * in )
{
	AddFriendReplyPacket *packet = new AddFriendReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	switch(packet->getReplyCode()){
	case QQ_AUTH_NO_AUTH:{
		QQFriend frd(addingBuddyQQ, 0);
		user->getFriendList().addFriend(frd);
		addingBuddyQQ = 0;
		emit addBuddyReady();
		}
		break;
	case QQ_AUTH_NEED_AUTH:
		addingBuddyQQ = 0;
		emit addBuddyNeedAuth();
		break;
	case QQ_AUTH_NO_ADD:
		addingBuddyQQ = 0;
		emit addBuddyRejected();
		break;
	}
	delete packet;	
}

void EvaPacketManager::doAddAuthBuddy( const unsigned int id, const QString &message)
{
	AddFriendAuthPacket *packet = new AddFriendAuthPacket(id, QQ_MY_AUTH_REQUEST);
	QCString msg = codec->fromUnicode(message);
	packet->setMessage(msg.data());
	connecter->append(packet);
}

void EvaPacketManager::doApproveBuddy( const unsigned int id)
{
	AddFriendAuthPacket *packet = new AddFriendAuthPacket(id, QQ_MY_AUTH_APPROVE);
	connecter->append(packet);
}

void EvaPacketManager::doRejectBuddy( const unsigned int id , const QString &message)
{
	AddFriendAuthPacket *packet = new AddFriendAuthPacket(id, QQ_MY_AUTH_REJECT);
	QCString msg = codec->fromUnicode(message);
	packet->setMessage(msg.data());
	connecter->append(packet);
}

void EvaPacketManager::processAddFriendAuthReply( const InPacket * in )
{
	AddFriendAuthReplyPacket *packet = new AddFriendAuthReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	emit addBuddySentToServer(packet->isSentOK());
	delete packet;
}

void EvaPacketManager::processSystemNotification( const InPacket * in )
{
	SystemNotificationPacket *packet = new SystemNotificationPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	if(packet->getType() == QQ_MSG_SYS_ADD_FRIEND_REQUEST_EX){
		if(user->getSetting()->isInRejectForever( packet->getFromQQ())){
			delete packet;
			return;
		}
	}

	QString msg = codec->toUnicode(packet->getMessage().c_str());

	emit receivedSysMessage(packet->getType(), packet->getMyQQ(), packet->getFromQQ(),
				 msg, packet->allowReverse(), packet->getCode(), packet->getCodeLength());

	delete packet;
}

void EvaPacketManager::doModifyDetails(QStringList info, QString oldPwd, QString newPwd)
{
	std::vector<std::string> result;
	for(uint i=0; i< info.count(); i++){
		printf("changing my details[%d]:%s\n", i, info[i].local8Bit().data());
		result.push_back((codec->fromUnicode(info[i])).data());
	}
	changingDetails.setDetails(result);
	ModifyInfoPacket *packet = new ModifyInfoPacket(changingDetails);
	if(oldPwd != QString::null && newPwd != QString::null){
		std::string o, n;
		o = codec->fromUnicode(oldPwd).data();
		n = codec->fromUnicode(newPwd).data();
		packet->setPassword(o);
		packet->setNewPassword(n);
	}
	connecter->append(packet);
}

void EvaPacketManager::processModifyInfoReply(const InPacket *in)
{
	ModifyInfoReplyPacket *packet = new ModifyInfoReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	if(packet->isAccepted()){
		user->setDetails(changingDetails);
		user->saveGroupedBuddyList();
		//emit myInfoReady();
		QApplication::postEvent(g_eva, new EvaNotifyEvent(E_MyInfo));
		emit modifyInfo(true);
	}else
		emit modifyInfo(false);
	delete packet;
}

QStringList EvaPacketManager::convertDetails(const ContactInfo &info)
{
	QStringList result;
	for(uint i=0; i< info.count(); i++){
		result.append(codec->toUnicode(info.at(i).c_str()));
	}
	return result;
}

void EvaPacketManager::doSearchUsers( const bool isSearchAll, const QString page, const QString id, 
				const QString nick, const QString email, const bool match )
{
	SearchUserPacket *packet = new SearchUserPacket();
	if(!isSearchAll){
		packet->setSearchType(nick.isEmpty()?QQ_SEARCH_QQ:QQ_SEARCH_NICK);
		packet->setPage(codec->fromUnicode(page).data());
		packet->setQQ(codec->fromUnicode(id).data());
		packet->setNick(codec->fromUnicode(nick).data());
		packet->setEmail(codec->fromUnicode(email).data());
		packet->setMatchEntireString(match);
	}
        connecter->append(packet);
}

void EvaPacketManager::processSearchUserReply( const InPacket * in )
{
	SearchUserReplyPacket *packet = new SearchUserReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}

	emit searchUsersReady(packet->isFinished(), packet->getUsers());
	delete packet;
}

void EvaPacketManager::doDeleteMeFrom( const unsigned int id )
{
	DeleteMePacket *packet = new DeleteMePacket(id);
	connecter->append(packet);
}

void EvaPacketManager::processDeleteMeReply( const InPacket * in )
{
	DeleteMeReplyPacket *packet = new DeleteMeReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	emit deleteMeReady(packet->isDeleted());
	delete packet;
}

void EvaPacketManager::processQunReply( const InPacket * in )
{
	QunReplyPacket *packet = new QunReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}

	switch(packet->getQunCommand()){
	case QQ_QUN_CMD_SEND_IM:
	case QQ_QUN_CMD_SEND_IM_EX:{
		// check if there are more sub packets to send
		std::map<int, QunSendIMExPacket*>::iterator iter = longMsgQunPacketQueue.find(packet->getSequence());
		if(iter == longMsgQunPacketQueue.end()){
			// last packet should be a one-shot message
			emit qunSentMessageResult(packet->getQunID() ,
								packet->isReplyOK(), 
								codec->toUnicode(packet->getErrorMessage().c_str()));
		} else {
			// got it in the long message queue
			QunSendIMExPacket *next = iter->second;  // next packet
			
			// erase the last sent sequence entry
			longMsgQunPacketQueue.erase(packet->getSequence());
	
			if(packet->isReplyOK()){
				// if sent ok, try to send next
				connecter->append(next);
				printf("Qun msg next fragment sent!\n");
			}else{
				// if any one fragment of the long message got error back, chuck all fragments
				int nextSeq = next->getSequence();
				while(next){
					longMsgQunPacketQueue.erase(next->getSequence());
					next = longMsgQunPacketQueue[nextSeq];
					nextSeq = next->getSequence();
				}
				emit qunSentMessageResult(packet->getQunID() ,
									packet->isReplyOK(), 
									codec->toUnicode(packet->getErrorMessage().c_str()));
			}
		}
		}
		break;
	case QQ_QUN_CMD_SEND_TEMP_QUN_IM:
		break;
	case QQ_QUN_CMD_CREATE_QUN:
		if(packet->isReplyOK()){
			Qun q(packet->getQunID());
			user->getQunList()->add(q);
			doRequestQunInfo(packet->getQunID());
			emit qunCreateDone(packet->getQunID());
			connecter->append(new QunActivePacket(packet->getQunID()));
			emit qunRequestUpdateDisplay();
		}else
			emit qunCreateFailed(codec->toUnicode(packet->getErrorMessage().c_str()));
		break;
	case QQ_QUN_CMD_CREATE_TEMP_QUN:
		break;
	case QQ_QUN_CMD_ACTIVATE_QUN:
		if(!packet->isReplyOK())
			emit qunActiveQunFailed(codec->toUnicode(packet->getErrorMessage().c_str()));
		break;
	case QQ_QUN_CMD_MODIFY_MEMBER:
		if(packet->isReplyOK())
			doRequestQunInfo(packet->getQunID());
		emit qunModifyQunMembersReply(packet->getQunID(), packet->isReplyOK(), codec->toUnicode(packet->getErrorMessage().c_str()));
		break;
	case QQ_QUN_CMD_GET_QUN_INFO:{
		if(!packet->isReplyOK()){
			// note that if comes here, the packet->getQunID() won't get the right id, only error message is ok
			emit qunInfomationReady(processQunID, packet->isReplyOK(), codec->toUnicode(packet->getErrorMessage().c_str()));
			printf("QQ_QUN_CMD_GET_QUN_INFO --  reply code: %d\n",packet->getReplyCode());
			switch(packet->getReplyCode()){
			case 3: // qun has been dismissed
				user->getQunList()->remove(processQunID);
				user->saveQunList();
				emit qunRequestUpdateDisplay();
				break;
			default:break;
			}
			processQunID = 0;
			return;
		}
		GetContactManager()->processQunInfoReply(packet);
// 		QunInfo info = packet->getQunInfo();
// 		user->getQunList()->setDetails(info);
// 		user->getQunList()->setMemberArgs(info.getQunID(), packet->getMemberList());
// 		emit qunInfomationReady(info.getQunID(), packet->isReplyOK(), codec->toUnicode(packet->getErrorMessage().c_str()));
// 		if( ! (user->getQunList()->getQun( info.getQunID() ))) return;

// 		std::map<unsigned int, QunMember>::iterator iter;
// 		std::map<unsigned int, QunMember> lst = packet->getMemberList();
// 		std::list<unsigned int> toSend;
// 		int i=0;
// 		qunMemberInfoFinished = false;
// 
// 		for(iter= lst.begin(); iter != lst.end(); ++iter){
// 			if(i==30){
// 				doRequestQunMemberInfo(info.getQunID(), toSend);
// 				toSend.clear();
// 				i = 0;
// 			}
// 			toSend.push_back(iter->second.qqNum);
// 			i++;
// 		}
// 		if(toSend.size())
// 			doRequestQunMemberInfo(info.getQunID(), toSend);
// 
// 		qunMemberInfoFinished = true;
// 		processQunID = 0;
// 		std::list<Qun>::iterator qunIter;
// 		std::list<Qun> list = user->getQunList()->getQunList();
// 		for(qunIter = list.begin(); qunIter != list.end(); ++qunIter){
// 			QunInfo info = qunIter->getDetails();
// 			if(info.getExtID() == 0){
// 				doRequestQunInfo(qunIter->getQunID());
// 				break;
// 			}
// 		}
  		}
		break;
	case QQ_QUN_CMD_EXIT_QUN:
		emit qunExitReply(packet->getQunID(), packet->isReplyOK(), codec->toUnicode(packet->getErrorMessage().c_str()));
		if(packet->isReplyOK()){
			user->getQunList()->remove(packet->getQunID());
			user->saveQunList();
			emit qunRequestUpdateDisplay();
		}
		break;
	case QQ_QUN_CMD_GET_MEMBER_INFO:{
		GetContactManager()->processQunMemberReply(packet);
/*		user->getQunList()->setMembers(packet->getQunID(), packet->getMemberInfoList());
		if(!qunMemberInfoFinished) break;
		user->saveQunList();*/
		emit qunMemberInfoReady(packet->getQunID());
		}
		break;
	case QQ_QUN_CMD_GET_ONLINE_MEMBER:{
		if(packet->isReplyOK()){
			user->getQunList()->setOnlineMembers(packet->getQunID(), packet->getQQNumberList());
			emit qunOnlineListReady(packet->getQunID());
		}else{
			// most likely, you are not a member of this Qun any mare
			QString msg = codec->toUnicode(packet->getErrorMessage().c_str());
			printf("QQ_QUN_CMD_GET_ONLINE_MEMBER -- reply code: %d, error msg: %s\n", packet->getReplyCode(), msg.local8Bit().data());
			user->getQunList()->remove(packet->getQunID());
			user->saveQunList();
			emit qunRequestUpdateDisplay();
			//emit qunSentMessageResult(packet->getQunID(), packet->isReplyOK(), codec->toUnicode(packet->getErrorMessage().c_str()));
		}
		}
		break;
	case QQ_QUN_CMD_JOIN_QUN:
		emit joinQunReply(packet->getQunID(), packet->getJoinReply(), codec->toUnicode( packet->getErrorMessage().c_str()));
		break;
	case QQ_QUN_CMD_JOIN_QUN_AUTH:
		emit joinQunAuthReply(packet->getQunID(), packet->getReplyCode());
		break;
	case QQ_QUN_CMD_MODIFY_QUN_INFO:
		if(packet->isReplyOK())
			doRequestQunInfo(packet->getQunID());
		emit qunModifyInfoReply(packet->getQunID(), packet->isReplyOK(), codec->toUnicode(packet->getErrorMessage().c_str()));
		break;
	case QQ_QUN_CMD_SEARCH_QUN:
		emit qunSearchReady(packet->getQunInfoList(), codec->toUnicode(packet->getErrorMessage().c_str()));
		break;
	case QQ_QUN_CMD_GET_TEMP_QUN_INFO:
		break;
	case QQ_QUN_CMD_EXIT_TEMP_QUN:
		break;
	case QQ_QUN_CMD_GET_TEMP_QUN_MEMBERS:
		break;
	case QQ_QUN_CMD_MODIFY_CARD:
		emit qunModifyQunCardReply(packet->getQunID(), packet->isReplyOK(), packet->getTargetQQ(),
							 codec->toUnicode(packet->getErrorMessage().c_str()));
		break;
	case QQ_QUN_CMD_REQUEST_ALL_REALNAMES:
		if(packet->isReplyOK()){
			QunList *qunList = user->getQunList();
			if(!qunList) break;
			Qun *qun = qunList->getQun( packet->getQunID());
			if(!qun) break;
			if(qun->getRealNamesVersion() == packet->getCardVersion())
				break;

			std::map<int, std::string> nameList = packet->getRealNames();
			std::map<int, std::string>::iterator it;
			FriendItem *member = 0;
			FriendItem *item = 0;
			for(it = nameList.begin(); it != nameList.end(); ++it){
				member = qun->memberDetails( it->first);
				if(!member) { // new member not existed in our list
					printf("New Qun member found when getting member real names!\n");
					printf("\t -- id: %d, real name(Qun card): %s\n", it->first, codec->toUnicode(it->second.c_str()).local8Bit().data());
					item = new FriendItem(it->first);
				} else { // we already have this member
					item = new FriendItem(*member);
				}
				item->setQunRealName( it->second);
				qun->setMember( *item);
				delete item;
			}
			if(nameList.size()) {
			}
			if(packet->getNextStartIndex()){
				QunRequestAllRealNames *all = new QunRequestAllRealNames(packet->getQunID());
				all->setStartIndex(packet->getNextStartIndex());
				connecter->append(all);
			} else { // we got all members' real names
				qun->setRealNamesVersion(packet->getCardVersion());
				user->saveQunList();
				emit qunMemberInfoReady(packet->getQunID());
			}
		}
		break;
	case QQ_QUN_CMD_REQUEST_CARD:
		if(packet->isReplyOK() && (packet->getTargetQQ() == user->getQQ()) )
			user->getQunList()->setMyQunCardInfo(packet->getQunID(), packet->getRealName(), packet->getGender(),
						packet->getPhone(), packet->getEmail(), packet->getMemo());
		emit qunRequestQunCardReply(packet->getQunID(), packet->isReplyOK(), packet->getTargetQQ(),
					codec->toUnicode(packet->getRealName().c_str()), packet->getGender(),
					codec->toUnicode(packet->getPhone().c_str()),codec->toUnicode(packet->getEmail().c_str()),
					codec->toUnicode(packet->getMemo().c_str()),codec->toUnicode(packet->getErrorMessage().c_str()));
		break;
	case QQ_QUN_CMD_ADMIN:
		if(packet->isReplyOK())
			doRequestQunInfo(packet->getQunID());
		emit qunSetAdminReply(packet->getQunID(), packet->isReplyOK(), packet->getTargetQQ(),
					(packet->getOpCode() == QQ_QUN_SET_ADMIN), codec->toUnicode(packet->getErrorMessage().c_str()));
		break;
	case QQ_QUN_CMD_TRANSFER:
		if(packet->isReplyOK())
			doRequestQunInfo(packet->getQunID());
		emit qunTransferReply(packet->getQunID(), packet->isReplyOK(), packet->getTargetQQ(),
						  codec->toUnicode(packet->getErrorMessage().c_str()));
		break;
	default:
		printf("Unknown qun operation reply\n");
	}
	delete packet;
}

void EvaPacketManager::doRequestQunInfo( const unsigned int id )  // note that the id should be the internal Qun ID
{
	//if(processQunID) return;
	processQunID = id;
	connecter->append(new QunGetInfoPacket(id));
}

void EvaPacketManager::doRequestQunMemberInfo( const unsigned int id , const std::list<unsigned int> &l)
{
	QunGetMemberInfoPacket *packet = new QunGetMemberInfoPacket(id);
	packet->setMemberList(l);
	connecter->append(packet);
}

void EvaPacketManager::doRequestQunOnlineMembers( const unsigned int id )
{
	connecter->append(new QunGetOnlineMemberPacket(id));
}

void EvaPacketManager::doSendQunMessage( const unsigned int receiver, QString & message, const char fontSize, 
					const bool u, const bool i, const bool b, 
					const char blue, const char green, const char red )
{
	QString rNick = QString::number(receiver);
	QString sNick = codec->toUnicode(user->getDetails().at(ContactInfo::Info_nick).c_str());
	Qun *qun = user->getQunList()->getQun(receiver);
	if(qun){
		QunInfo info = qun->getDetails();
		rNick = codec->toUnicode(info.getName().c_str());

		const FriendItem *item = qun->getMemberDetails(user->getQQ());
		if(item)
			sNick  = codec->toUnicode(item->getQunRealName().c_str());
	}

	QDateTime time = QDateTime::currentDateTime(Qt::LocalTime);	
	user->getSetting()->saveMessage(receiver,  user->getQQ(), sNick, receiver, rNick,
				true, message, time, fontSize, u, i, b, blue, green, red, true);
	

	QCString str = codec->fromUnicode(message);
	unsigned short pcs = strlen(str.data()) / 700;
	QunSendIMExPacket *firstPacket = 0;
	int lastSeq = 0;
	
	for (unsigned short pcsn=0; pcsn<=pcs; pcsn++)
	{
		QunSendIMExPacket *packet = new QunSendIMExPacket(receiver);
		packet->setNumFragments(pcs + 1);
		packet->setSeqOfFragments(pcsn);
		//memcpy(&packet->pieceInfo[2], &opID, 2);
		
		packet->setFontSize(fontSize);
		packet->setUnderline(u); 
		packet->setItalic(i);
		packet->setBold(b);
		packet->setBlue(blue);
		packet->setGreen(green);
		packet->setRed(red);
		//QCString str = codec->fromUnicode(message);
		//packet->setMessage(str.data());
		packet->setMessage(str.mid(pcsn * 700, 700).data());
		if(!pcsn) {
			firstPacket = packet;
			lastSeq = firstPacket->getSequence();
		} else {
			longMsgQunPacketQueue[lastSeq] = packet; // record the next packet
			lastSeq = packet->getSequence();
			if(pcsn == (pcs - 1))  // the last sent packet points to NULL
				longMsgQunPacketQueue[lastSeq] = NULL;
		}
	}
	connecter->append(firstPacket);
}

void EvaPacketManager::doSearchQun( const unsigned int extID )
{
	QunSearchPacket *packet = new QunSearchPacket();
	packet->setExtID(extID);
	connecter->append(packet);
}

void EvaPacketManager::doJoinQun( const unsigned int qunID )
{
	QunJoinPacket *packet = new QunJoinPacket(qunID);
	connecter->append(packet);
}

void EvaPacketManager::doJoinQunAuth( const unsigned int qunID , const unsigned char *code, const unsigned short len, const QString &message)
{
	printf("code len: %d\n", len);
	QunAuthPacket *packet = new QunAuthPacket(qunID, QQ_QUN_AUTH_REQUEST);
	std::string msg = (codec->fromUnicode(message)).data();
	packet->setCode( code, len);
	packet->setMessage(msg);
	connecter->append(packet);
}

void EvaPacketManager::doApproveQun( const unsigned int id, const unsigned int qunID,
					const unsigned char *token, const unsigned short len)
{
	QunAuthPacket *packet = new QunAuthPacket(qunID, QQ_QUN_AUTH_APPROVE);
	packet->setReceiver(id);
	packet->setToken( token, len);
	connecter->append(packet);
}

void EvaPacketManager::doRejectQun( const unsigned int id, const unsigned int qunID, const QString & message,
				const unsigned char *token, const unsigned short len)
{	
	QunAuthPacket *packet = new QunAuthPacket(qunID, QQ_QUN_AUTH_REJECT);
	packet->setReceiver(id);
	packet->setToken(token, len);
	std::string msg = (codec->fromUnicode(message)).data();
	packet->setMessage(msg);
	connecter->append(packet);
}

void EvaPacketManager::doGetAllLevels( )
{
	std::list<unsigned int> list;
	list.push_back(user->getQQ());
	
	std::list<QQFriend> friends = user->getFriendList().getAllFriends();
	std::list<QQFriend>::iterator iter;
	
	int i = 0;
	for(iter = friends.begin(); iter!=friends.end(); ++iter){
		list.push_back(iter->getQQ());
		i++;
		if(i==70){
			EvaGetLevelPacket *packet = new EvaGetLevelPacket(list);
			connecter->append(packet);
			list.clear();
			i=0;
		}
	}

	if(list.size()){
		EvaGetLevelPacket *packet = new EvaGetLevelPacket(list);
		connecter->append(packet);
	}
}

void EvaPacketManager::requestFriendLevel(const unsigned int id)
{
	std::list<unsigned int> list;
	list.push_back(id);
	EvaGetLevelPacket *packet = new EvaGetLevelPacket(list);
	connecter->append(packet);
}

void EvaPacketManager::processGetLevelReply( const InPacket * in )
{
	EvaGetLevelReplyPacket *packet = new EvaGetLevelReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	
	std::list<LevelUserItem> friends = packet->getLevelList();
	std::list<LevelUserItem>::iterator iter;
	for(iter = friends.begin(); iter != friends.end(); ++iter){
		if(iter->qqNum == user->getQQ()){
			user->setOnlineTime(iter->onlineTime);
			user->setLevel(iter->level);
			user->setHoursToLevelUp(iter->timeRemainder);
		}else
			user->getFriendList().updateFriendLevel(iter->qqNum, iter->onlineTime, iter->level, iter->timeRemainder);
		
	}
	delete packet;
}

void EvaPacketManager::doModifyQunInfo( const unsigned int id, unsigned char auth, unsigned short cate, 
					QString name, QString notice, QString description )
{
	user->getSetting()->removeFromQunRejectForever(id); // clear setting to allow join requests
	user->getSetting()->saveSettings();

	std::string qunName = (codec->fromUnicode(name)).data();
	std::string qunNotice = (codec->fromUnicode(notice)).data();
	std::string qunDescription = (codec->fromUnicode(description)).data();
	
	QunInfo info;
	info.setQunID(id);
	info.setAuthType(auth);
	info.setCategory(cate);
	info.setName(qunName);
	info.setNotice(qunNotice);
	info.setDescription(qunDescription);
	connecter->append(new QunModifyPacket(info));
}

void EvaPacketManager::doModifyQunCard(const unsigned int id,  const unsigned int qqID, QString name, unsigned char gender, 
				QString phone, QString email, QString memo )
{
	printf("qun card modifying\n");
	QunModifyCardPacket *packet = new QunModifyCardPacket(id, qqID);
	
	std::string cardName = codec->fromUnicode(name).data(); 
	std::string cardPhone = codec->fromUnicode(phone).data();
	std::string cardEmail = codec->fromUnicode(email).data();
	std::string cardMemo = codec->fromUnicode(memo).data();
	
	packet->setName(cardName);
	packet->setGender(gender);
	packet->setPhone(cardPhone);
	packet->setEmail(cardEmail);
	packet->setMemo(cardMemo);
	connecter->append(packet);
}

void EvaPacketManager::doQunSetAdmin( const unsigned int id, const unsigned int qq, const bool isSetAdmin )
{
	connecter->append(new QunAdminOpPacket(id, qq, isSetAdmin));
}

void EvaPacketManager::doQunTransfer( const unsigned int id, const unsigned int qq )
{
	connecter->append(new QunTransferPacket(id, qq));
}

void EvaPacketManager::doModifyQunMembers( const unsigned int id, const std::list< unsigned int > members, const bool isAdd )
{
	user->getSetting()->removeFromQunRejectForever(id); // clear setting to allow join requests
	user->getSetting()->saveSettings();

	QunModifyMemberPacket *packet = new QunModifyMemberPacket(id, isAdd);
	packet->setMembers(members);
	connecter->append(packet);
}

void EvaPacketManager::processRequestKeyReply( const InPacket * in )
{
	EvaRequestKeyReplyPacket *packet = new EvaRequestKeyReplyPacket();
	packet->setInPacket(in);
	packet->parse();
	delete packet;
// 	if(packet->getKeyType() == QQ_REQUEST_FILE_AGENT_KEY){
// 		user->loginManager()->finishedCommand(QQ_CMD_REQUEST_KEY);
// 	}
	//emit fileAgentInfoReady();
	GetLoginManager()->fileAgentInfoReady();
}

void EvaPacketManager::doRequestQunCard( const unsigned int id, const unsigned int qq )
{
	printf("doRequestQunCard\n");
	connecter->append(new QunRequestCardPacket(id, qq));
}

void EvaPacketManager::doQunExit( const unsigned int id )
{
	connecter->append(new QunExitPacket(id));
}

void EvaPacketManager::doQunCreate( const QString & name, const unsigned short category, const unsigned char auth, 
				const QString & notice, const QString & description, const std::list< unsigned int > & members )
{
	QCString _name = codec->fromUnicode(name);
	QCString _notice = codec->fromUnicode(notice);
	QCString _description = codec->fromUnicode(description);
	
	QunCreatePacket *packet = new QunCreatePacket(_name.data(), category, auth);
	packet->setNotice(_notice.data());
	packet->setDescription(_description.data());
	packet->setMembers(members);
	connecter->append(packet);
}

void EvaPacketManager::doRequestExtraInfo( )
{
	connecter->append(new RequestExtraInfoPacket());
}

void EvaPacketManager::processRequestExtraInfoReply(const InPacket *in)
{
	RequestExtraInfoReplyPacket *packet = new RequestExtraInfoReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	if(packet->getReplyCode() != 1){
		delete packet;
		return;
	}
	std::map<unsigned int, unsigned long long> members = packet->getMembers();
	std::map<unsigned int, unsigned long long>::iterator iter;
	for(iter = members.begin(); iter!= members.end(); ++iter){
		if(iter->first == user->getQQ())
			user->setExtraInfo(iter->second);
		else
			user->getFriendList().setExtraInfo(iter->first, iter->second);
	}
	if(packet->getOffset()!=QQ_FRIEND_LIST_POSITION_END){
		connecter->append(new RequestExtraInfoPacket(packet->getOffset()));
	}else{
		emit extraInfoReady();
		doRequestSignature();
	}
	delete packet;
}

void EvaPacketManager::doModifySignature( const QString signature )
{
	SignaturePacket *packet = new SignaturePacket(QQ_SIGNATURE_MODIFY);
	std::string sig = codec->fromUnicode(signature).data();
	packet->setSignature(sig);
	connecter->append(packet);
}

void EvaPacketManager::doDeleteSignature( )
{
	connecter->append(new SignaturePacket(QQ_SIGNATURE_DELETE));
}

bool EvaPacketManager::doRequestSignature( const unsigned int start, const bool isSingleContact)
{
	SignaturePacket *packet = new SignaturePacket(QQ_SIGNATURE_REQUEST);
	if(isSingleContact){
		QQFriend *frd = user->getFriends()->getFriend(start);
		packet->setSignatureArgs( start, frd->getSignatureModifyTime());
		connecter->append(packet);
		return true;
	} else {
		std::map<unsigned int, unsigned int> list = 
							user->getFriendList().getMemberSignatureList(start, 
												     user->getQQ(), 
												     user->hasSignature());
		if(list.size()){
			packet->setMembers(list);
			connecter->append(packet);
			return true;
		} else
			return false;
	}
}

void EvaPacketManager::processSignatureReply( const InPacket * in )
{
	SignatureReplyPacket *packet = new SignatureReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	
	switch(packet->getType()){
	case QQ_SIGNATURE_MODIFY:
		if(packet->isChangeAccepted())
			doRequestSignature(user->getQQ());
		emit signatureModifyReply(packet->isChangeAccepted());
		break;
	case QQ_SIGNATURE_DELETE:
		emit signatureDeleteReply(packet->isChangeAccepted());
		if(packet->isChangeAccepted()){
			user->setSignature("", 0);
		}
		break;
	case QQ_SIGNATURE_REQUEST:{
		GetContactManager()->processSignatureReply(packet);
// 		std::map<unsigned int, SignatureElement> members = packet->getMembers();
// 		std::map<unsigned int, SignatureElement>::iterator iter;
// 		for(iter = members.begin(); iter!= members.end(); ++iter){
// 			if(iter->first == user->getQQ())
// 				user->setSignature(iter->second.signature, iter->second.lastModifyTime);
// 			else
// 				user->getFriendList().setSignature(iter->first, iter->second.signature, iter->second.lastModifyTime);
// 		}
// 		user->saveGroupedBuddyList();
// 		doRequestSignature(packet->nextStartID());
		}
		break;
	}
	delete packet;
}
/*
void EvaPacketManager::lastLoginStep()
{	
	doGetAllLevels();
	doRequestExtraInfo();
	doGetWeatherForecast(user->getLoginWanIp()); /// get local weather
}*/

void EvaPacketManager::doUploadMemo( const unsigned int id, const MemoItem &memo )
{
	EvaMemoPacket *packet = new EvaMemoPacket(id,QQ_MEMO_UPLOAD);
	packet->setMemo(memo);
	connecter->append(packet);
}

void EvaPacketManager::doRemoveMemo( const unsigned int id )
{
	connecter->append(new EvaMemoPacket(id,QQ_MEMO_REMOVE));
}

void EvaPacketManager::doDownloadMemo( const unsigned int id )
{
	connecter->append(new EvaMemoPacket(id,QQ_MEMO_DOWNLOAD));
}

void EvaPacketManager::processMemoReply( const InPacket * in )
{
	EvaMemoReplyPacket *packet = new EvaMemoReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	switch(packet->getType()){
	case QQ_MEMO_UPLOAD:
		user->getFriendList().setMemo(packet->getQQ(), packet->getMemo());
		user->saveGroupedBuddyList();
		emit memoUploadReply(packet->isChangeAccepted());
		break;
	case QQ_MEMO_REMOVE:break;//not sure the function of memo_remove
	case QQ_MEMO_DOWNLOAD:
		if( packet->getQQ() ){
			user->getFriendList().setMemo(packet->getQQ(), packet->getMemo());
			emit memoDownloadReply(packet->getMemo());
		}
		else
			emit memoNoMemoFound();
		user->saveGroupedBuddyList();
		break;
	}
	delete packet;
	
}

void EvaPacketManager::doSendFileUdpRequest(const unsigned int id, const QString fileName,
					const unsigned int fileSize, const short session,
					const unsigned char transferType)
{
	if( id == user->getQQ()) return;
	SendFileExRequestPacket *udpPacket = new SendFileExRequestPacket(QQ_IM_EX_UDP_REQUEST);
	udpPacket->setReceiver(id);
	udpPacket->setTransferType(transferType);
	QCString strName = codec->fromUnicode(fileName);
	udpPacket->setFileName(strName.data());
	udpPacket->setFileSize(fileSize);
	udpPacket->setSessionId(session);
	udpPacket->setWanIp(user->getLoginWanIp());
	connecter->append(udpPacket);
	printf("EvaPacketManager::doSendFileUdpRequest - Sent.\n");
}

void EvaPacketManager::doAcceptFileRequest( const unsigned int id, const short session, const unsigned int wanIp,
					const unsigned char transferType )
{
	if( id == user->getQQ()) return;
	SendFileExRequestPacket *packet = new SendFileExRequestPacket(QQ_IM_EX_REQUEST_ACCEPTED);
	packet->setReceiver(id);
	packet->setTransferType(transferType);
	packet->setWanIp(wanIp);
	packet->setSessionId(session);
	connecter->append(packet);
	printf("EvaPacketManager::doAcceptFileRequest -- sent\n");
}

void EvaPacketManager::doCancelFileRequest( const unsigned int id, const short session, const unsigned char transferType)
{
	if( id == user->getQQ()) return;
	SendFileExRequestPacket *packet = new SendFileExRequestPacket(QQ_IM_EX_REQUEST_CANCELLED);
	packet->setReceiver(id);
	packet->setTransferType(transferType);
	packet->setSessionId(session);
	connecter->append(packet);
	printf("EvaPacketManager::doCancelFileRequest -- sent\n");
}

void EvaPacketManager::doNotifyAgentTransfer( const unsigned int id, const unsigned int oldSession, const unsigned int agentSession, 
					const unsigned int ip, const unsigned short port, const unsigned char transferType)
{
	if( id == user->getQQ()) return;
	SendFileNotifyAgentPacket *packet = new SendFileNotifyAgentPacket();
	packet->setMsgSequence(oldSession);
	packet->setReceiver(id);
	packet->setTransferType(transferType);
	packet->setAgentSession(agentSession);
	packet->setAgentServer(ip, port);
	printf("EvaPacketManager::doNotifyAgentTransfer Sent. -- id:%d, ip:%8x, port:%d, session:0x%8x\n",id, ip, port, agentSession);
	connecter->append(packet);
	
}

void EvaPacketManager::doNotifyIpEx(const unsigned int id, const bool isSender, 
			const unsigned int session,const unsigned char transferType,
			const unsigned int wanIp1, const unsigned short wanPort1,
			const unsigned int wanIp2, const unsigned short wanPort2,
			const unsigned int wanIp3, const unsigned short wanPort3,
			const unsigned int lanIp1, const unsigned short lanPort1,
			const unsigned int lanIp2, const unsigned short lanPort2,
			const unsigned int lanIp3, const unsigned short lanPort3,
			const unsigned int syncIp, const unsigned short syncPort,
			const unsigned int syncSession )
{
	
	SendIpExNotifyPacket *packet = new SendIpExNotifyPacket(true);
	packet->setReceiver(id);
	packet->setSessionId(session);
	packet->setSender(isSender);
	packet->setTransferType(transferType);

	packet->setWanIp1(wanIp1);
	packet->setWanPort1(wanPort1);
	packet->setWanIp2(wanIp2);
	packet->setWanPort2(wanPort2);
	packet->setWanIp3(wanIp3);
	packet->setWanPort3(wanPort3);

	packet->setLanIp1(lanIp1);
	packet->setLanPort1(lanPort1);
	packet->setLanIp2(lanIp2);
	packet->setLanPort3(lanPort2);
	packet->setLanIp3(lanIp3);
	packet->setLanPort3(lanPort3);

	packet->setSyncIp(syncIp);
	packet->setSyncPort(syncPort);
	packet->setSyncSession(syncSession);

	connecter->append(packet);
	printf("IP Notification packet Sent.\n");
}

void EvaPacketManager::doRequestQunRealNames( const unsigned int id )
{
	connecter->append(new QunRequestAllRealNames(id));
}

void EvaPacketManager::slotClientNotReady( )
{
	//printf("EvaPacketManager::slotClientNotReady, return\n");
	return;
// 	unsigned short cmd = user->loginManager()->nextStep();
// 	switch(cmd){
// 	case QQ_CMD_REQUEST_LOGIN_TOKEN:
// 		printf("EvaPacketManager::slotClientNotReady ----- QQ_CMD_REQUEST_LOGIN_TOKEN\n");
// 		//doRequestLoginToken();
// 		doRequestLoginTokenEx();
// 		break;
// 	case QQ_CMD_LOGIN:
// 		printf("EvaPacketManager::slotClientNotReady ----- QQ_CMD_LOGIN\n");
// 		doLogin();
// 		break;
// 	case QQ_CMD_REQUEST_KEY:
// 		printf("EvaPacketManager::slotClientNotReady ----- QQ_CMD_REQUEST_KEY\n");
// 		connecter->append(new EvaRequestKeyPacket(QQ_REQUEST_FILE_AGENT_KEY));
// 		break;
// 	case QQ_CMD_GET_USER_INFO:
// 		printf("EvaPacketManager::slotClientNotReady ----- QQ_CMD_GET_USER_INFO\n");
// 		doGetUserInfo(user->getQQ());
// 		break;
// 	case QQ_CMD_CHANGE_STATUS:
// 		printf("EvaPacketManager::slotClientNotReady ----- QQ_CMD_CHANGE_STATUS\n");
// 		doChangeStatus(user->getStatus());
// 		break;
// 	case QQ_CMD_GET_FRIEND_LIST:
// 		printf("EvaPacketManager::slotClientNotReady ----- QQ_CMD_GET_FRIEND_LIST\n");
// 		doGetAllFriends();
// 		break;
// 	}
}

void EvaPacketManager::doRequestLoginTokenEx(  const QString &code )
{
	if(!user->loginNeedVerify()){
		// as QQ does send it 3 times, so do we
		connecter->append(new RequestLoginTokenExPacket());
		connecter->append(new RequestLoginTokenExPacket());
		connecter->append(new RequestLoginTokenExPacket());
	} else {
		RequestLoginTokenExPacket *packet = new RequestLoginTokenExPacket(QQ_LOGIN_TOKEN_VERIFY);
		GraphicVerifyCode gcode = user->getLoginVerifyInfo();
		packet->setToken(gcode.m_SessionToken, gcode.m_SessionTokenLen);
		packet->setCode(codec->fromUnicode(code).data());
		connecter->append( packet);
		user->clearAllVerifyCodes(); // remove all codes here.
	}
}

void EvaPacketManager::processRequestLoginTokenExReply( const InPacket * in )
{
	if(Packet::isLoginTokenSet()) return;
	RequestLoginTokenExReplyPacket *packet = new RequestLoginTokenExReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		//emit serverBusy();
		return;
	} else {
		// QQ_LOGIN_TOKEN_REQUEST or QQ_LOGIN_TOKEN_VERIFY
		// as long as getReplyCode is QQ_LOGIN_TOKEN_NEED_VERI,
		// the verification image should be in the packet
		if(packet->getReplyCode() == QQ_LOGIN_TOKEN_NEED_VERI){
			GraphicVerifyCode code;
			code.setSessionToken(packet->getToken(), packet->getTokenLength());
			code.setData(packet->getData(), packet->getDataLength());
			user->addLoginVerifyInfo( code);
			if( user->getNumVerifyCodes()==1 ){
				GetLoginManager()->loginVerification();
				//emit loginNeedVerification();
			}
			delete packet;
			return;
		}
		
		delete packet;
	}
	// notify login verirfy window that the verification got passed
	GetLoginManager()->verifyPassed();
	//emit loginVerifyPassed();

	//user->loginManager()->finishedCommand(QQ_CMD_REQUEST_LOGIN_TOKEN);
	// here, we've got login token, therefore, try to log in
	//doLogin();
}

void EvaPacketManager::doAdvancedSearch( const int page,
					const bool onlineUser,
					const bool hasCam,
					const int province,
					const int city,
					const int age,
					const int gender )
{
	EvaAdvancedSearchPacket *packet = new EvaAdvancedSearchPacket();
	packet->setSearchOnline(onlineUser);
	packet->setHasCam(hasCam);
	packet->setPage(page);
	packet->setProvinceIndex(province);
	packet->setCityIndex(city);
	packet->setAgeIndex(age);
	packet->setGenderIndex(gender);
	
	connecter->append(packet);
}

void EvaPacketManager::processAdvancedSearchReply( const InPacket * in )
{
	EvaAdvancedSearchReplyPacket *packet = new EvaAdvancedSearchReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	emit advancedSearchReady(packet->getPage(), packet->isFinished(), packet->getUsers());
}

void EvaPacketManager::doAddBuddyEx( const unsigned int id )
{
	connecter->append( new EvaAddFriendExPacket(id));
}

void EvaPacketManager::processAddFriendExReply( const InPacket * in )
{
	EvaAddFriendExReplyPacket *packet = new EvaAddFriendExReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}

	emit addBuddyExReply(packet->getAddQQ(), packet->getReplyCode(), packet->getAuthStatus());
}

void EvaPacketManager::doRequestAuthInfo( const unsigned int id , const unsigned char cmd, const QString &veri, const QString &session, const bool isQun)
{
	EvaAddFriendGetAuthInfoPacket *packet = new EvaAddFriendGetAuthInfoPacket(id, cmd, isQun);
	packet->setVerificationStr( std::string(codec->fromUnicode( veri ).data()) );
	packet->setSessionStr( std::string(codec->fromUnicode( session ).data()) );
	connecter->append( packet );
}

void EvaPacketManager::processAddFrendAuthInfoReply( const InPacket * in )
{
	EvaAddFriendGetAuthInfoReplyPacket *packet = new EvaAddFriendGetAuthInfoReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	emit requestAuthInfoReply(packet->getSubCommand(), packet->getReplyCode(), packet->getCode(), packet->getCodeLength());
}

void EvaPacketManager::doRequestAuthQuestion( const unsigned int id, const bool isQuestion, const QString & answer )
{
	EvaAddFriendAuthQuestionPacket *packet = new EvaAddFriendAuthQuestionPacket(id);//default is AUTH_TYPE_QUESTION_REQUEST
	if(!isQuestion){
		packet->setAuthStatus( AUTH_TYPE_QUESTION_ANSWER );
		packet->setAnswer( std::string(codec->fromUnicode( answer).data()) );
	}
	connecter->append( packet);
}

void EvaPacketManager::doRequestMyAuthQuestionSetting( )
{
	EvaAddFriendAuthQuestionPacket *packet = new EvaAddFriendAuthQuestionPacket(user->getQQ());//default is AUTH_TYPE_QUESTION_REQUEST
	packet->setAuthStatus( AUHT_TYPE_QUESTION_GET );
	connecter->append( packet);
	printf("doRequestMyAuthQuestionSetting sent\n");
}

void EvaPacketManager::doSetMyAuthQuestion( const QString & quest, const QString & answer )
{
	EvaAddFriendAuthQuestionPacket *packet = new EvaAddFriendAuthQuestionPacket(user->getQQ());//default is AUTH_TYPE_QUESTION_REQUEST
	packet->setAuthStatus( AUTH_TYPE_QUESTION_SET );
	packet->setQuestion( std::string(codec->fromUnicode( quest).data()) );
	packet->setAnswer(   std::string(codec->fromUnicode( answer).data()) );
	connecter->append( packet);
	printf("doSetMyAuthQuestion sent\n");
}

void EvaPacketManager::processAddFriendAuthQuestionReply( const InPacket * in )
{
	EvaAddFriendAuthQuestionReplyPacket *packet = new EvaAddFriendAuthQuestionReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	switch(packet->getAuthStatus()){
		case AUHT_TYPE_QUESTION_GET:{
			QString question = codec->toUnicode((const char*)(packet->getCode()));
			QString answer = codec->toUnicode(packet->getAnswer().c_str());
			emit receivedMyAuthSettings(packet->getAuthStatus(),
						question,
						answer);
			}
			break;
		case AUTH_TYPE_QUESTION_SET:
			emit updateAuthSettingResult(packet->getAuthStatus(), packet->getReplyCode());
			break;
		case AUTH_TYPE_QUESTION_REQUEST:
		case AUTH_TYPE_QUESTION_ANSWER:
			emit requestQuestionReply(packet->getAuthStatus(),
						packet->getReplyCode(),
						packet->getCode(),
						packet->getCodeLength());
			break;
	}
}

void EvaPacketManager::doAddBuddyAuthEx( const unsigned int id, const unsigned char authStatus, const unsigned int destGroup,
			const unsigned char * code, const unsigned short len,
			const unsigned char * qcode, const unsigned short qlen,
			const QString &msg, const bool allowReverse)
{
	EvaAddFriendAuthExPacket *packet = new EvaAddFriendAuthExPacket(id, authStatus);
	packet->setDestGroup( destGroup);
	packet->setAuthCode( code, len);
	packet->setQuestionCode( qcode, qlen);
	packet->setMessage( std::string( codec->fromUnicode(msg).data()));
	packet->setAllowAddReverse( allowReverse);
	connecter->append(packet);
	
}

void EvaPacketManager::processAddFriendAuthExReply( const InPacket * in )
{
	EvaAddFriendAuthExReplyPacket *packet = new EvaAddFriendAuthExReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	emit addFriendAuthExReply(packet->getAddQQ(), packet->getAuthStatus(), packet->isAddSuccessful());
}

void EvaPacketManager::doVerifyAddingMessage( const unsigned int id, const unsigned char * code, const unsigned int len )
{
	VerifyAddingMsgPacket *packet = new VerifyAddingMsgPacket(id);
	packet->setCode( code, len);
	connecter->append( packet);
}

void EvaPacketManager::processVerifyAddingMsgReply( const InPacket * in )
{
	VerifyAddingMsgReplyPacket *packet = new VerifyAddingMsgReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	emit receivedVerifyAddingMessageReply(packet->getBuddyQQ(), packet->getReplyCode(), packet->getFlag1(), packet->getFlag2());
}

void EvaPacketManager::doGetWeatherForecast( const unsigned int ip )
{
	printf("weather IP: %s\n", QHostAddress(ip).toString().ascii());
	connecter->append(new WeatherOpPacket(ip));
}

void EvaPacketManager::processWeatherOpReply( const InPacket * in )
{
	WeatherOpReplyPacket *packet = new WeatherOpReplyPacket();
	packet->setInPacket(in);
	if(!packet->parse()){
		delete packet;
		return;
	}
	printf("Province:%s, city: %s\n", codec->toUnicode(packet->getProvince().c_str()).local8Bit().data(),
					codec->toUnicode(packet->getCity().c_str()).local8Bit().data());

	std::list<Weather> w = packet->getWeathers();
	std::list<Weather>::iterator iter;
	QDateTime date;
	for(iter = w.begin(); iter != w.end(); ++iter){
		date.setTime_t( iter->getTime());
		printf("\t[%s] %s, %s, %d-%dC, %s\n",date.toString( "yyyy-MM-dd hh:mm:ss").local8Bit().data(),
						codec->toUnicode(iter->getShortDesc().c_str()).local8Bit().data(),
						codec->toUnicode(iter->getWind().c_str()).local8Bit().data(),
						iter->getLowTemperature(), iter->getHighTemperature(),
						codec->toUnicode(iter->getHint().c_str()).local8Bit().data());
	}
}

void EvaPacketManager::slotConnectReady( )
{
	EvaNotifyEvent *event = new EvaNotifyEvent(E_SvrConnected);
	QApplication::postEvent(g_eva, event);

	kdDebug() << "[QQ: " << user->getQQ() << "] start logging in server \"" 
				<< connecter->getHostAddress().toString()  << "\" ..." << endl;
	doLogin();
}

void EvaPacketManager::doGetContacts( int pos )
{
	connecter->append(new GetFriendListPacket(pos));
}
