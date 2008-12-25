/***************************************************************************
 *   Copyright (C) 2007 by yunfan                                          *
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

#include "evacontactmanager.h"
#include "evaguimain.h"
#include "evaconnecter.h"
#include "evaloginmanager.h"
#include "evasession.h"
#include "evaapi.h"
#include "evapacketmanager.h"
#include "evagroup.h"
#include "evaqun.h"
#include "evauser.h"
#include "evaqunlist.h"
#include "libeva.h"
#include <assert.h>
#include <qapplication.h>

EvaContactManager::EvaContactManager( EvaSession* session, EvaConnecter* connecter, EvaPacketManager* pm)
    : m_contactsReady(false)
    , session( session )  
    , connecter( connecter )
    , m_status(ESCM_NONE)
      , packetManager(pm)
{
}

EvaContactManager::~ EvaContactManager( )
{
}

void EvaContactManager::setPacketManager( EvaPacketManager * pm )
{
	assert(pm);
	packetManager = pm;

	m_status = ESCM_NONE;
			
	//QObject::connect();
	
}

void EvaContactManager::fetchContacts( )
{
	m_status = ESCM_CONTACTLIST;
	
	//we try to load local cache first
	EvaUser *user = EvaMain::session->getUser();
	if(user && user->getNumberOfFriends() ){
		m_downloadAll = false;
                fetchAllLevels();
                fetchAllSignatures();
                packetManager->doRequestExtraInfo();
                packetManager->doGetWeatherForecast(session->getLoginManager()->getLoginWanIp()); /// get local weather
			
                connecter->slotClientReady();
		return;
	} else
		m_downloadAll = true;
	
	//clear private list first
	m_Contacts.clearFriendList();
	//user->getFriendList().clearFriendList();
	packetManager->doGetContacts(QQ_FRIEND_LIST_POSITION_START);	
}

void EvaContactManager::processGetFriendListReply( const GetFriendListReplyPacket * packet )
{
	assert(packet);
	if( m_status != ESCM_CONTACTLIST) return;	
	
	friendItemList gotList = packet->getFriendList();
	friendItemList::iterator iter;
	for(iter = gotList.begin(); iter!= gotList.end(); ++iter){
		QQFriend frd;
		frd.setFriendItem(*iter);
		//user->getFriendList().addFriend(frd);
		m_Contacts.addFriend(frd);
	}
	if(packet->getPosition()!=QQ_FRIEND_LIST_POSITION_END){
		packetManager->doGetContacts( packet->getPosition());
	}else{
// 		EvaUser * user = EvaMain::session->getUser();
// 		if(user){
// 			user->setFriendList(m_Contacts);
// 		}
		m_status = ESCM_NONE;
		if(m_downloadAll){
			fetchGroupNames();
		}
		//emit friendListReady(); // EvaMain class will set the loginManager
	}
}

void EvaContactManager::fetchGroupNames( )
{
	m_status = ESCM_GROUPNAMES;
	m_GroupNames.clear();
	packetManager->doGetGroups();
}

void EvaContactManager::processDownloadGroupName( const GroupNameOpReplyPacket * packet )
{
	assert(packet);
	assert(packet->isDownloadReply());
	if( m_status != ESCM_GROUPNAMES) return;
		
	m_GroupNames = packet->getGroupNames();
// 	EvaUser *user = EvaMain::session->getUser();
// 	if(user){
// 		user->setGroupNames(m_GroupNames);
// 	}
	if(m_downloadAll)
		fetchGroupedFriends();
	//user->clearGroupNames(); // note that the first group won't be deleted
	//for(iter = names.begin(); iter != names.end(); ++iter){
	//	user->newGroup(*iter);
	//}
}

void EvaContactManager::fetchGroupedFriends( )
{
	m_status = ESCM_GROUPEDBUDDIES;
	m_GroupedContacts.clear();
	//m_QunIDs.clear();
	packetManager->doGetGroupedFriends();
}

void EvaContactManager::fetchQunList( )
{
	fetchGroupedFriends();
}

void EvaContactManager::processDownloadGroupFriendReply( const DownloadGroupFriendReplyPacket * packet )
{
	assert(packet);
	if( m_status != ESCM_GROUPEDBUDDIES) return;
	
//X 	kdDebug() << "[EvaContactManager] download group reply" << endl;
	
	std::list<DownloadFriendEntry> friends = packet->getGroupedFriends();
	std::list<DownloadFriendEntry>::iterator iter;
	for(iter = friends.begin(); iter!= friends.end(); ++iter){
		if(!iter->isQun()){
			//user->getFriendList().updateFriendGroupIndex(iter->getQQ(), iter->getGroupID());
			m_GroupedContacts[iter->getQQ()] = iter->getGroupID();
			m_Contacts.updateFriendGroupIndex( iter->getQQ(), iter->getGroupID() );
		}else{
			//Qun q(iter->getQQ());
			//printf("------------- Qun : %d\n", iter->getQQ());
			//user->getQunList()->add(q);
			//m_QunIDs.push_back(iter->getQQ());
			Qun q(iter->getQQ());
			m_QunList.add(q);
		}
	}
//X 	QString msg = QString(i18n("%1")).arg(m_GroupedContacts.size());
	QString msg = QString("%1").arg(m_GroupedContacts.size());
	
	int nextID = packet->getNextStartID();
	if(nextID != 0x0000){
		//connecter->append(new DownloadGroupFriendPacket(nextID));		
		packetManager->doGetGroupedFriends(nextID);
	}else{
//X 		kdDebug() << "[EvaContactManager] Grouped friends done (" << m_QunList.numQuns() << " Quns)" << endl;
		//emit friendGroupsReady();
		//QunList *qunList = user->getQunList();
		//if(qunList && qunList->numQuns()){
		//	doRequestQunInfo(qunList->getQunList().begin()->getQunID());
		//}
		//user->getFriendList().clearFriendList();
		EvaUser *user = EvaMain::session->getUser();
		if(user){
			user->setGroupNames(m_GroupNames);
			user->setFriendList(m_Contacts);
//X 			user->saveGroupedBuddyList();
			
			user->setQunList(m_QunList);
			user->saveQunList();
		}
		m_contactsReady = true;
		if(m_downloadAll){
			printf("fetching qun info\n");
			Qun *q = m_QunList.first();
			if(q){
				fetchQunDetails(q->getQunID());
			}
		}
	}
}

void EvaContactManager::fetchQunDetails( const unsigned int id )
{
//X 	kdDebug()<< "[EvaContactManager] getting Qun details(" << id << ")" << endl;
	m_status = ESCM_QUNDETAILS;
	m_QunInfo.clearInfo();
	m_QunMembers.clear();
	packetManager->doRequestQunInfo(id);
}

void EvaContactManager::processQunInfoReply( const QunReplyPacket * packet )
{
	assert(packet);
	if( m_status != ESCM_QUNDETAILS) return;

	m_QunInfo = packet->getQunInfo();
	m_QunMembers = packet->getMemberList();
	
	m_QunList.setDetails(m_QunInfo);
	m_QunList.setMemberArgs(m_QunInfo.getQunID(), m_QunMembers);

	EvaUser *user = EvaMain::session->getUser();
	if(user){
		user->getQunList()->setDetails(m_QunInfo);
		user->getQunList()->setMemberArgs(m_QunInfo.getQunID(), m_QunMembers);
		QunInfo info = user->getQunList()->getQun(m_QunInfo.getQunID())->getDetails();
// 		kdDebug() << "[EvaContactManager] Qun(" << m_QunInfo.getQunID() << ") has " << m_QunMembers.size() << " members" << endl;		
// 		kdDebug() << "[EvaContactManager] Qun " << GB2Unicode(info.getName().c_str())
// 				<< ", Notice " << GB2Unicode(info.getNotice().c_str()) << endl;
		user->saveQunList();
	}
	
	QString msg = QString("%1").arg(m_QunInfo.getQunID());
	if(m_downloadAll){
		Qun *q = m_QunList.next();
		if(q){
// 			kdDebug() << "[EvaContactManager] -----------fetching next Qun " << q->getQunID() << endl;
			fetchQunDetails(q->getQunID());
		}else{
			m_downloadAll = false;
			m_QunMemberCount = 0;
			// show main display to user
			packetManager->doRequestExtraInfo();
			packetManager->doGetWeatherForecast(session->getLoginManager()->getLoginWanIp()); /// get local weather
			
			fetchAllLevels();
			fetchAllSignatures();
			
// 			q = m_QunList.first();
// 			kdDebug() << "[EvaContactManager] All Quns infos finished. Fetching Qun member info of " << q->getQunID() << endl;
// 			if(q){
// 				m_QunMemberCount = 0;
// 				fetchQunMembersInfo(q->getQunID());
// 			} else {
// 				kdDebug() << "[EvaContactManager] No Qun at all" << endl;
// 			}
		}
	}else{
		m_QunMemberCount = 0;
		fetchQunMembersInfo(m_QunInfo.getQunID());
	}
}

void EvaContactManager::fetchQunMembersInfo( const unsigned int id , bool isFirst/* = true*/)
{
//X 	kdDebug() << "fetchQunMembersInfo" << endl;
	EvaUser *user = EvaMain::session->getUser();
	assert(user);
	Qun *qun = user->getQunList()->getQun(id);
	if(!qun){
//X 		kdDebug() << "[EvaContactManager] Qun " << id << " doesn't exit" << endl;
		return;
	}else {
		if(isFirst)
			qun->resetMemberIterator();
	}

	m_status = ESCM_QUNMEMBERS;
		
	std::map<unsigned int, QunMember>::iterator it;
	std::map<unsigned int, QunMember> members = qun->getMemberArgs();
	std::list<unsigned int> toSend;
	//int i = 0;
	//int start = m_QunMemberCount;
	//kdDebug() << "m_QunMemberCount:"<< m_QunMemberCount << ", member size: " << members.size() << endl;
	
// 	std::list<unsigned int> toSend;
// 	std::list<FriendItem>::iterator it;
// 	std::list<FriendItem> members = qun->getMembers();
// 	int i = 0;
// 	int start = m_QunMemberCount;
// 	kdDebug() << "m_QunMemberCount:"<< m_QunMemberCount << ", member size: " << members.size() << endl;
// 	for(it = members.begin(); it != members.end(); it++){
// 		if((start + 30) == i ){
// 			printf("-----toSend(size:%d)\n", toSend.size());
// 			packetManager->doRequestQunMemberInfo(id, toSend);
// 			toSend.clear();
// 			break;
// 			//i = 0;
// 		}
// 		if( i >= start )
// 			toSend.push_back(it->getQQ());
// 			//toSend.push_back(it->second.qqNum);
// 		i++;
// 	}
	toSend = qun->getNextMembers();
//X 	kdDebug() << "to Send size:"<< toSend.size() << endl;
	if(toSend.size()){
		packetManager->doRequestQunMemberInfo(id, toSend);
	}
}

void EvaContactManager::processQunMemberReply( const QunReplyPacket * packet )
{
//X 	kdDebug() << "processQunMemberReply" << endl;
	EvaUser *user = EvaMain::session->getUser();
	assert(user);
	assert(packet);
	if( m_status != ESCM_QUNMEMBERS) return;
	
	m_QunMemberCount += packet->getMemberInfoList().size();
	
	m_QunList.setMembers(packet->getQunID(), packet->getMemberInfoList());
	user->getQunList()->setMembers(packet->getQunID(), packet->getMemberInfoList());
	user->saveQunList();

	//Qun *q = m_QunList.getQun(packet->getQunID());
	Qun *q = user->getQunList()->getQun(packet->getQunID());
	if(q ){
		//if( q->numMembers() == m_QunMemberCount){ // we got all members(info) for current Qun
		if(q->isNextMembersEnd()){
			//q->resetMemberIterator();
			m_QunMemberCount = 0;
// 			kdDebug() << "[EvaContactManager] 2 q->numMembers() " << q->numMembers() << ", m_QunMemberCount " << m_QunMemberCount << endl;
// 			if(m_downloadAll){
// 				kdDebug() << "[EvaContactManager] 3 q->numMembers() " << q->numMembers() << ", m_QunMemberCount " << m_QunMemberCount << endl;
// 				Qun *q = m_QunList.next();
// 				kdDebug() << "[EvaContactManager] 4 q->numMembers() " << q->numMembers() << ", m_QunMemberCount " << m_QunMemberCount << endl;
// 				if(q){
// 					kdDebug() << "[EvaContactManager] fetching Qun(" << q->getQunID() 
// 							<< ") member info(received:" << m_QunMemberCount 
// 							<< "/totally:" << q->numMembers() << ")" 
// 							<< endl;
// 					m_QunMemberCount = 0;
// 					fetchQunMembersInfo(q->getQunID());
// 				} else{
// 					m_downloadAll = false;
// 				}
// 			}
		} else {  // Still some members needed
//X 			kdDebug() << "[EvaContactManager] Qun(" << packet->getQunID() 
//X 					<< ") got " 
//X 					<< m_QunMemberCount 
//X 					<< " members(totally " 
//X 					<<  q->numMembers() 
//X 					<< ")" 
//X 					<< endl;
			fetchQunMembersInfo(packet->getQunID(), false);
		}
	}
}

void EvaContactManager::fetchDetail( const unsigned int id )
{
	m_status = ESCM_CONTACTDETAIL;
	packetManager->doGetUserInfo(id);
}

void EvaContactManager::processGetUserInfoReply( const GetUserInfoReplyPacket * packet )
{
 	printf( "[EvaContactManager] got user info\n" );
	EvaUser *user = EvaMain::session->getUser();
	assert(user);
	assert(packet);
	//if( m_status != ESCM_CONTACTDETAIL) return;

	m_ContactInfo = packet->contactInfo();
	std::string strID = m_ContactInfo.at(ContactInfo::Info_qqID);
	unsigned int id = atoi(strID.c_str());
	if( id == user->getQQ()){
		user->setDetails(m_ContactInfo);
	}else if(user->getFriendList().hasFriend( id)){
			user->setContactInfo(id, m_ContactInfo);
		}
	
	QString msg = QString("%1").arg(id);
}

void EvaContactManager::fetchSignature( const unsigned int id )
{
	m_status = ESCM_SIGNATURE;
	packetManager->doRequestSignature(id, true);
}

void EvaContactManager::fetchAllSignatures( )
{
	m_status = ESCM_ALLSIGNATURES;
	packetManager->doRequestSignature();
}

void EvaContactManager::processSignatureReply( const SignatureReplyPacket * packet )
{
	EvaUser *user = EvaMain::session->getUser();
	assert(user);
	assert(packet);

	unsigned int id = 0;
	QString sig = "";
	//QTextCodec *codec = QTextCodec::codecForName("GB18030");
	std::map<unsigned int, SignatureElement> members = packet->getMembers();
	std::map<unsigned int, SignatureElement>::iterator iter;
	for(iter = members.begin(); iter!= members.end(); ++iter){
		if(iter == members.begin()){
			id  = iter->first;
			sig = GB2Unicode(iter->second.signature.c_str());
		}
		if(iter->first == user->getQQ())
			user->setSignature(iter->second.signature, iter->second.lastModifyTime);
		else
			user->setContactSignature(iter->first, iter->second.signature, iter->second.lastModifyTime);
	}
	
	if(m_status == ESCM_ALLSIGNATURES){
		packetManager->doRequestSignature(packet->nextStartID());
	} 
}

void EvaContactManager::fetchLevel( const unsigned int id )
{
	m_status = ESCM_LEVEL;
	packetManager->requestFriendLevel(id);
}

void EvaContactManager::processGetLevelReply( const EvaGetLevelReplyPacket * packet )
{
	EvaUser *user = EvaMain::session->getUser();
	assert(user);
	assert(packet);

	unsigned int id = 0;
	std::list<LevelUserItem> friends = packet->getLevelList();
	std::list<LevelUserItem>::iterator iter;
	for(iter = friends.begin(); iter != friends.end(); ++iter){
		if(iter == friends.begin())
			id = iter->qqNum;
		if(iter->qqNum == user->getQQ()){
			user->setOnlineTime(iter->onlineTime);
			user->setLevel(iter->level);
			user->setHoursToLevelUp(iter->timeRemainder);
		}else
			user->updateFriendLevel(iter->qqNum, iter->onlineTime, iter->level, iter->timeRemainder);
	}	
}

void EvaContactManager::fetchAllLevels( )
{
	m_status = ESCM_ALLLEVELS;
	packetManager->doGetAllLevels();
}

void EvaContactManager::fetchAddQuestion( const unsigned int /*id*/ )
{
}

void EvaContactManager::downloadAll( )
{
	m_downloadAll = true;
	fetchContacts();
}

