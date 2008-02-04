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
#ifndef LIBEVAFRIEND_H
#define LIBEVAFRIEND_H

#include "evapacket.h"
#include "evadefines.h"
#include <list>
#include <string>

class FriendItem{
public:
	FriendItem(const unsigned int id = 0);
	FriendItem(const unsigned char *buf, int *len);
	FriendItem(const FriendItem &rhs);
	~FriendItem() {}
	
	const unsigned int getQQ() const { return qqNum; }
	const unsigned short getFace() const { return face; }
	const char getAge() const { return age; }
	const char getGender() const { return gender; }
	const std::string &getNick() const { return nick; }  // note: nick is encoded by "GB18030"
	const char getExtFlag() const { return extFlag; }
	const char getCommonFlag() const { return commonFlag; }
	const long getLoginTime() const { return loginTime; }
	const long getIdleTime() const { return idleTime; }
	const long getLastRefreshTime() const { return lastRefreshTime; }

	void setQQ(const unsigned int id) { qqNum = id; }
	void setFace(const unsigned short f) { face = f; }
	void setAge(const char a) { age = a; }
	void setGender(const char g) { gender = g; }
	void setNick(const std::string &n) { nick = n; }
	void setExtFlag(const char ef) { extFlag = ef; }
	void setCommonFlag(const char cf) { commonFlag = cf; }
		
	void setOnline(const bool online) { mIsOnline = online; }
	const bool isOnline() const { return mIsOnline; }
	
	const unsigned short getQunGroupIndex() const { return qunGroupIndex; }
	const unsigned short getQunAdminValue() const { return qunAdminValue; }
	
	void setQunGroupIndex(const unsigned char index) { qunGroupIndex = index;}
	void setQunAdminValue(const unsigned char value) { qunAdminValue = value;}
	const bool isAdmin() const { return qunAdminValue & QUN_TYPE_ADMIN; }
	const bool isShareHolder() const { return qunAdminValue & QUN_TYEP_SHAREHOLDER; }

	// the following 2 methods implemented by henry first, I change the names a bit :)
	void setQunRealName(const std::string &name) {
		m_QunRealName = name; 
	}
	void setQunRealNameVersion( const int version) { m_QunRealNameVersion = version; }
	const std::string &getQunRealName() const { return m_QunRealName; }
	const int getQunRealNameVersion() const { return m_QunRealNameVersion; }

	bool isMember() { return (commonFlag & 0x2) != 0; }
	bool isBoy() { return gender == QQ_FRIEND_GENDER_GG;}
	
	FriendItem *copy() { return new FriendItem(*this);}
	FriendItem &operator=(const FriendItem &rhs);
private:
	void parseData(const unsigned char *buf, int *len);
	 
	unsigned int qqNum;
	unsigned short face;
	char age;
	char gender;
	std::string nick;	
	char extFlag;  // bit1: if has qqshow, the rest are unknown  
	char commonFlag;  //  bit1: memeber,   bit4: TCP mode,   bit5 : mobile QQ, 
			  //  bit6: mobile binding,   bit7: webcam
	long loginTime;
	long idleTime;
	long lastRefreshTime;
	
	unsigned char qunGroupIndex;  // for qun use only, default is 0
	unsigned char qunAdminValue;  // for qun use only, default is 0
	int m_QunRealNameVersion;
	std::string m_QunRealName; // henry: for qun use only
	
	bool mIsOnline;
};

typedef std::list<FriendItem> friendItemList;

class GetFriendListPacket : public OutPacket {
public:
	GetFriendListPacket();
	GetFriendListPacket(const unsigned short startPosition);
	GetFriendListPacket(const GetFriendListPacket &rhs);
	virtual ~GetFriendListPacket() {};
	
	OutPacket * copy() { return new GetFriendListPacket(*this);}
	GetFriendListPacket &operator=( const GetFriendListPacket &rhs);
	
	const unsigned short getStartPosition() const { return startPosition; }
	void setStartPosition(const unsigned short startPosition) { this->startPosition = startPosition; }

protected:
	virtual int putBody(unsigned char *buf) ;

private:
	unsigned short startPosition;
};

class GetFriendListReplyPacket : public InPacket {
public:
	GetFriendListReplyPacket() {}
	GetFriendListReplyPacket(unsigned char *buf, int len);
	GetFriendListReplyPacket(const GetFriendListReplyPacket &rhs);
	virtual ~GetFriendListReplyPacket() {}
	
	const unsigned short getPosition() const { return position; }
	const friendItemList &getFriendList() const { return friends; }
	GetFriendListReplyPacket &operator=( const GetFriendListReplyPacket &rhs);
protected:
	void parseBody();
private:
	unsigned short position;
	friendItemList friends;
};

#endif

