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
 #include "evafriendlist.h"
 #include <stdio.h>
#include <cstring>
#include <cstdlib>

QQFriend::sortItem QQFriend::field2Sort = Sort_Nick;

QQFriend::QQFriend()
{
	initalizeBuddy();
} 

QQFriend::QQFriend(const unsigned int qqid, const unsigned short qqFace)
{
	initalizeBuddy( qqid, qqFace);
}

QQFriend::QQFriend(const QQFriend &rhs)
{
	*this = rhs;
}

void QQFriend::setFriendItem( const FriendItem &item)
{
	qqNum = item.getQQ();
	if(face != item.getFace()){
		face = item.getFace() ;
		m_evaUpdateFlag |= FaceChanged;
	}
		
	age = item.getAge();
	gender = item.getGender();
	if(nick != item.getNick()){
		nick = item.getNick();
		m_evaUpdateFlag |= NickChanged;
	}
		
	extFlag = item.getExtFlag();
	commonFlag = item.getCommonFlag();
	loginTime = item.getLoginTime();
	idleTime = item.getIdleTime();
	lastRefreshTime = item.getLastRefreshTime();
	
}

void QQFriend::setFriendOnlineEntry( const FriendOnlineEntry &entry)
{
	if(qqNum != entry.getQQ()) return;
	unknown4 = entry.getUnknown1_4();
	IP = entry.getIP();  // 4 bytes
	port = entry.getPort();
	unknown11 = entry.getUnknown2_11();
	
	if(status != entry.getStatus()){
		status = entry.getStatus();
		m_evaUpdateFlag |= StatusChanged;
	}
		
	unknown13_14 = entry.getUnknown3_13_14();
	memcpy(unknownKey, entry.getUnknownKey(), QQ_KEY_LENGTH);
	
	unknown31_32 = entry.getUnknown4_31_32();
	onlineExtFlag = entry.getExtFlag();
	onlineCommonFlag = entry.getCommFlag();
	unknown35_36 = entry.getUnknown5_35_36();
	
}

void QQFriend::setUserInformation( const ContactInfo &info)
{
	userInfo = info;
	if(info.count()<5) {
		return;
	}
	face = (unsigned short)(atoi(info.at(ContactInfo::Info_face).c_str()));
	age = (char)(atoi(info.at(ContactInfo::Info_age).c_str()));
	gender = (char)(atoi(info.at(ContactInfo::Info_gender).c_str()));
	nick  = info.at(ContactInfo::Info_nick);
}

QQFriend &QQFriend::operator=(const QQFriend &rhs)
{
	qqNum = rhs.getQQ();
	face = rhs.getFace();
	age = rhs.getAge();
	gender = rhs.getGender();
	nick = rhs.getNick();	
	extFlag = rhs.getExtFlag();    
	commonFlag = rhs.getCommonFlag();  

	loginTime = rhs.getLoginTime();
	idleTime = rhs.getIdleTime();
	lastRefreshTime = rhs.getLastRefreshTime();
	
	//int qqNum;
	unknown4 = rhs.getUnknown1_4();
	IP = rhs.getIP();  // 4 bytes
	port = rhs.getPort();
	unknown11 = rhs.getUnknown2_11();
	status = rhs.getStatus();
	unknown13_14 = rhs.getUnknown3_13_14();
	memcpy(unknownKey, rhs.getUnknownKey(), QQ_KEY_LENGTH);
	unknown31_32 = rhs.getUnknown4_31_32();
	onlineExtFlag = rhs.getOnlineExtFlag();
	onlineCommonFlag = rhs.getOnlineCommFlag();
	unknown35_36 = rhs.getUnknown5_35_36();
	
	timeOnline = rhs.getOnlineTime();
	level = rhs.getLevel();
	hoursToLevelUp = rhs.getHoursToLevelUp();
	
	clientVersion = rhs.getVersion();
	
	mExtraInfo = rhs.getExtraInfo();
	mSignature = rhs.getSignature();
	mSignatureModifyTime = rhs.getSignatureModifyTime();

	m_Memo = rhs.getMemo();

	memcpy(fileSessionKey, rhs.getFileSessionKey(), QQ_KEY_LENGTH);
	
	userInfo = rhs.getUserInformation();
	
	field2Sort = rhs.getSortField();
	
	groupIndex = rhs.getGroupIndex();
	
	m_Sequence = rhs.getSequence();

	m_ChatFontSize = rhs.getChatFontSize();
	m_ChatFontColor = rhs.getChatFontColor();
	
	m_evaUpdateFlag = rhs.getEvaUpdateFlag();
	return *this;
}

int QQFriend::operator==(const QQFriend &rhs) const 
{
	if( qqNum != rhs.getQQ()) return 0;
	return 1;
}

// for this part, we compare the field by field2Sort
int QQFriend::operator<(const QQFriend &rhs) const 
{
	switch(field2Sort){
		case Sort_QQ:
			if(qqNum< rhs.getQQ()) return 1;
			break;
		case Sort_Nick:
			if(nick < rhs.getNick()) return 1;
			break;
		case Sort_Status:
			if(status == rhs.getStatus())  return 1;
			if(status == QQ_FRIEND_STATUS_ONLINE) return 1;
			if(rhs.getStatus() == QQ_FRIEND_STATUS_ONLINE) return 0;
			if(status == QQ_FRIEND_STATUS_LEAVE)  return 1;
			if(rhs.getStatus() == QQ_FRIEND_STATUS_LEAVE) return 0;
			return 1;
	}
	return 0;
}

void QQFriend::initalizeBuddy(const unsigned int qqid, const unsigned short qqFace)
{
	qqNum = qqid;
	face = qqFace;
	age = BUDDY_INVALID_FIELD;
	gender = BUDDY_INVALID_FIELD;
	char *temp = (char*)malloc(20 * sizeof(char));
	if(temp==NULL) {
		fprintf(stderr, "Memomry not enough, exit.\n");
		exit(0);
	}
	sprintf(temp,"%d",qqid);
	nick.assign(temp);
	free(temp);
	extFlag = BUDDY_INVALID_FIELD;
	commonFlag = BUDDY_INVALID_FIELD;
	loginTime = BUDDY_INVALID_FIELD;
	idleTime = BUDDY_INVALID_FIELD;
	lastRefreshTime = BUDDY_INVALID_FIELD;
	
	unknown4 = BUDDY_INVALID_FIELD;
	IP = 0;
	port = BUDDY_INVALID_FIELD;
	unknown11 = BUDDY_INVALID_FIELD;
	status = QQ_FRIEND_STATUS_OFFLINE;
	unknown13_14 = BUDDY_INVALID_FIELD;	
	memset(unknownKey, 0, QQ_KEY_LENGTH);
	unknown31_32 = BUDDY_INVALID_FIELD;
	onlineExtFlag = BUDDY_INVALID_FIELD;
	onlineCommonFlag = BUDDY_INVALID_FIELD;
	unknown35_36 = BUDDY_INVALID_FIELD;
	
	mExtraInfo = 0L;
	mSignature = "";
	mSignatureModifyTime = 0;

	m_Memo.name = "";
	m_Memo.telephone = "";
	m_Memo.mobile = "";
	m_Memo.address = "";
	m_Memo.email = "";
	m_Memo.zipcode = "";
	m_Memo.note = "";
	
	clientVersion = BUDDY_INVALID_FIELD;
	memset(fileSessionKey, 0, QQ_KEY_LENGTH);
	
	groupIndex = 0;  // set default to zero, "My Buddies"
	
	timeOnline = 0;
	level = 0;
	hoursToLevelUp = 0;
	m_Sequence = 0;

	m_ChatFontSize = 14;
	m_ChatFontColor = 255;
	
	m_evaUpdateFlag = 0xffffffff;
}

void QQFriend::setOnlineTime(const unsigned int time ) 
{
	timeOnline = time;
}
/*  ======================================================= */

const bool FriendList::hasFriend(const unsigned int id) 
{
	std::map<unsigned int, QQFriend>::iterator iter;
	iter = getFriendPosition(id);
	if( iter == privateList.end()) return false;
	else
		return true;
}

QQFriend *FriendList::getFriend(const unsigned int id)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return NULL;
	
	return &privateList[id];
}

bool FriendList::deleteFriend(const unsigned int id)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if( iter == privateList.end()) return false;
	privateList.erase(iter);
	return true;
}

void FriendList::addFriend(const QQFriend &frd)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(frd.getQQ());
	if( iter != privateList.end()) {
		privateList[frd.getQQ()].setGroupIndex(frd.getGroupIndex());
		//updateFriend(frd);
		return;
	}
	privateList[frd.getQQ()] = frd;
}

void FriendList::updateFriend( const QQFriend &frd)
{
	deleteFriend(frd.getQQ());      // delete it first
	addFriend(frd);                   // add new one;
}

bool FriendList::updateFriendIP(const unsigned int id, const unsigned int ip)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setIP(ip);
	return true;
}

bool FriendList::updateFriendFace(const unsigned int id, const unsigned short face)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setFace(face);
	return true;
}

bool FriendList::updateFriendPort(const unsigned int id, const unsigned short p)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setPort(p);
	return true;
}

bool FriendList::updateFriendStatus(const unsigned int id, const char status)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	privateList[id].setStatus(status);
	return true;
}

bool FriendList::updateFriendFileSessionKey(const unsigned int id, const unsigned char *key)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setFileSessionKey(key);
	return true;
}

bool FriendList::updateFriendGroupIndex(const unsigned int id, const int index)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setGroupIndex(index);
	return true;
}

bool FriendList::updateFriendLevel(const unsigned int id, const unsigned int online, const unsigned short level, const unsigned short hours)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setOnlineTime(online);
	privateList[id].setLevel(level);
	privateList[id].setHoursToLevelUp(hours);
	
	return true;
}

bool FriendList::addFriendItemTo( const unsigned int id, const FriendItem &item)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setFriendItem(item);
	return true;
}

bool FriendList::addOnlineFriendEntryTo( const unsigned int id, const FriendOnlineEntry & entry)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setFriendOnlineEntry(entry);
	return true;
}

bool FriendList::addContactInfoTo( const unsigned int id, const ContactInfo &info)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	privateList[id].setUserInformation(info);
	return true;
}

const int FriendList::numberOfOnlines()
{
	int onlines = 0;
        std::map<unsigned int, QQFriend>::iterator iter;
	for(iter =privateList.begin(); iter != privateList.end(); ++iter){
		switch(iter->second.getStatus()){
		case QQ_FRIEND_STATUS_ONLINE:
			onlines++;
			break;
		case QQ_FRIEND_STATUS_OFFLINE:
			break;
		case QQ_FRIEND_STATUS_LEAVE:
			onlines++;
			break;
		case QQ_FRIEND_STATUS_INVISIBLE:
			break;
		}
	}
	return onlines;
}

std::list<QQFriend> FriendList::getFriendsInGroupIndexOf( const int index)
{
	std::list<QQFriend> groupFriends;
        std::map<unsigned int, QQFriend>::iterator iter;
	for(iter =privateList.begin(); iter != privateList.end(); ++iter){
		if(index == (iter->second).getGroupIndex())
			groupFriends.push_back(iter->second);
	}
	return groupFriends;
}

std::list<QQFriend> FriendList::getAllFriends( const unsigned int myId )
{
	std::list<QQFriend> allFriends;
        std::map<unsigned int, QQFriend>::iterator iter;
	for(iter =privateList.begin(); iter != privateList.end(); ++iter){
		allFriends.push_back(iter->second);
	}
	if(myId)
		allFriends.push_back(QQFriend(myId, 0));
	QQFriend::sortItem order = QQFriend::getSortField();
	QQFriend::setSortField(QQFriend::Sort_QQ);
	allFriends.sort();
	QQFriend::setSortField(order);
	return allFriends;
}

std::map<unsigned int, QQFriend>::iterator FriendList::getFriendPosition(const unsigned int id)
{
	return privateList.find(id);
}

bool FriendList::setExtraInfo(const unsigned int id, const short info)
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setExtraInfo(info);
	return true;
}

const std::map< unsigned int, unsigned int > FriendList::getMemberSignatureList(const unsigned int start, const unsigned int myId, const bool hasSignature)
{
	std::map< unsigned int, unsigned int > list;
	int count =0 ;
	std::list<QQFriend> qqList = getAllFriends(myId);
	std::list<QQFriend>::iterator qqIter;
	
	for(qqIter = qqList.begin(); qqIter != qqList.end(); ++qqIter){
		//printf(" qq: %d\n", qqIter->getQQ());
		if(qqIter->getQQ()>=start && (qqIter->hasSignature()|| (qqIter->getQQ() == myId && hasSignature))){
			list[qqIter->getQQ()] = qqIter->getSignatureModifyTime();
			count++;
			if(count>99) break;
		}
	}
	// printf("member signature size: %d(%d)\t friend size: %d\n", list.size(), count, privateList.size());
 	return list;
}

bool FriendList::setSignature( const unsigned int id, const std::string sig, const unsigned int time )
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setSignature(sig, time);
	return true; 
}

const std::list< unsigned int > FriendList::getUserHeadList(const unsigned int myId, const bool hasUserHead )
{
 std::list<unsigned int> results;
 std::list<QQFriend> qqList = getAllFriends(myId);
 std::list<QQFriend>::iterator qqIter;
 
 for(qqIter = qqList.begin(); qqIter != qqList.end(); ++qqIter){
		if( qqIter->hasUserHead() || (qqIter->getQQ() == myId && hasUserHead))
			results.push_back(qqIter->getQQ());
	}
	return results;
}

bool FriendList::setMemo( const unsigned int id, const MemoItem& memo )
{
	std::map<unsigned int, QQFriend>::iterator iter = getFriendPosition(id);
	if(iter == privateList.end()) return false;
	
	privateList[id].setMemo(memo);
	return true;	
}

QQFriend *FriendList::firstFriend()
{
    m_iter = privateList.begin();
    if(m_iter == privateList.end())
        return NULL;
    else
        return (&(m_iter->second));
}

QQFriend *FriendList::nextFriend()
{
    ++m_iter;
    if(m_iter == privateList.end())
        return NULL;
    else
        return (&(m_iter->second));
}
