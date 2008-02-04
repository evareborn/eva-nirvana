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

#ifndef LIBEVAONLINESTATUS_H
#define LIBEVAONLINESTATUS_H

#include "evapacket.h"
#include <list>

class FriendStatus;  // this class is for private use only

class FriendOnlineEntry {
public:
	FriendOnlineEntry();
	FriendOnlineEntry(const FriendOnlineEntry &rhs);
	~FriendOnlineEntry();
	
	FriendOnlineEntry *copy() { return new FriendOnlineEntry(*this);}
	
	const unsigned int getQQ() const;
	const unsigned int getIP() const;
	const unsigned short getPort() const;
	const char getStatus() const;
	const char getUnknown1_4() const;    // bit 4
	const char getUnknown2_11() const;   // bit 11
	const short getUnknown3_13_14() const;  // bit 13-14
	const unsigned char * getUnknownKey() const;
	
	
	const short getUnknown4_31_32() const { return unknown31_32; }
	const char getExtFlag() const { return extFlag; }
	const char getCommFlag() const { return commFlag; }
	const short getUnknown5_35_36() const { return unknown35_36; }
	const char getEnd() const { return ending; }
	
	int numOfBytes;
	int readData(unsigned char * buf);
	FriendOnlineEntry &operator=(const FriendOnlineEntry &rhs);
private:
	FriendStatus *status;
	short unknown31_32;
	char extFlag;
	char commFlag;
	short unknown35_36;
	char ending;  // always be 0
};

typedef std::list<FriendOnlineEntry> onlineList;

class GetOnlineFriendsPacket : public OutPacket 
{
public:
	GetOnlineFriendsPacket();
	GetOnlineFriendsPacket(const unsigned char position);
	GetOnlineFriendsPacket(const GetOnlineFriendsPacket &rhs);
	virtual ~GetOnlineFriendsPacket() {};
	
	OutPacket * copy() { return new GetOnlineFriendsPacket(*this);}
	GetOnlineFriendsPacket &operator=(const GetOnlineFriendsPacket &rhs);
	
	const unsigned char getStartPosition() const { return startPosition; };
	
	void setStartPosition(const unsigned char position) { startPosition = position; };

protected:
	int putBody(unsigned char *buf);
private:
	unsigned char startPosition;
};

class GetOnlineFriendReplyPacket : public InPacket
{
public:
	GetOnlineFriendReplyPacket() {}
	GetOnlineFriendReplyPacket(unsigned char *buf, int len);
	GetOnlineFriendReplyPacket(const GetOnlineFriendReplyPacket &rhs);
	virtual ~GetOnlineFriendReplyPacket() {} 
	
	const unsigned char getPosition() const { return position; }
	const onlineList &getOnlineFriendList() const { return onlineFriends; }
	GetOnlineFriendReplyPacket &operator=(const GetOnlineFriendReplyPacket &rhs);
protected:
	void parseBody();
private:
	unsigned char position;
	onlineList onlineFriends;
};

class FriendChangeStatusPacket : public InPacket
{
public:
	FriendChangeStatusPacket() {}
	FriendChangeStatusPacket(unsigned char* buf, int len);
	FriendChangeStatusPacket(const FriendChangeStatusPacket &rhs);
	virtual ~FriendChangeStatusPacket();

	const unsigned int getQQ() const;
	const unsigned int getIP() const;
	const unsigned short getPort() const;
	const char getStatus() const;
	const char getUnknown1_4() const;    // bit 4
	const char getUnknown2_11() const;   // bit 11
	const short getUnknown3_13_14() const;  // bit 13-14
	const unsigned char *getUnknownKey() const;
	
	// this is not that useful, just means the message's receiver should hold this account
	const unsigned int getMyQQ() const { return myQQNum;}
	FriendChangeStatusPacket &operator=(const FriendChangeStatusPacket &rhs);
protected:
	void parseBody();
private:
	FriendStatus *status;
	unsigned int myQQNum;

};

class ChangeStatusPacket : public OutPacket 
{
public:
	ChangeStatusPacket();
	ChangeStatusPacket(char status);
	ChangeStatusPacket(const ChangeStatusPacket &rhs);
	virtual ~ChangeStatusPacket() {};
	OutPacket * copy() { return new ChangeStatusPacket(*this);}
	ChangeStatusPacket &operator=(const ChangeStatusPacket &rhs);

	const char getOnlineStatus() const { return myStatus; }
	void setOnlineStatus( const char status) { myStatus = status; }
	void setMiscStatus( const unsigned int status ) { miscStatus = status; }
protected:
	virtual int putBody(unsigned char *buf);
private:
	char myStatus;
	unsigned int miscStatus;
};

class ChangeStatusReplyPacket : public InPacket 
{
public: 
	ChangeStatusReplyPacket() {}   
	ChangeStatusReplyPacket(unsigned char *buf, int len);
	ChangeStatusReplyPacket(const ChangeStatusReplyPacket &rhs);
	virtual ~ChangeStatusReplyPacket() {}
	
	const char getReplyCode() const { return replyCode; };
	const bool isAccepted() const;
	ChangeStatusReplyPacket &operator=(const ChangeStatusReplyPacket &rhs);
protected:
	void parseBody();
private:
	char replyCode;
};

#endif

