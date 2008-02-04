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
 
#ifndef LIBEVAGROUP_H
#define LIBEVAGROUP_H

#include "evapacket.h"
#ifndef WIN32
#include <inttypes.h>
#endif
#include <list>
#include <string>
#include <map>

// This file is for all group related operations

// can be used to upload or download group names, default is download 
class GroupNameOpPacket : public OutPacket {
public:
	GroupNameOpPacket();
	GroupNameOpPacket(const char cmdType);
	GroupNameOpPacket(const GroupNameOpPacket &rhs);
	virtual ~GroupNameOpPacket() {};
	
	virtual OutPacket* copy() { return new GroupNameOpPacket(*this); }
	GroupNameOpPacket &operator=(const GroupNameOpPacket &rhs);
	
	const std::list<std::string> getGroups() const { return groups; }
	const char getType() const { return type; }
	
	void setGroups(const std::list<std::string> &l) { groups = l; }	
	void setType(const char type);
protected:
	virtual int putBody(unsigned char* buf);
private:
	std::list<std::string> groups;
	char type;
};

class GroupNameOpReplyPacket : public InPacket
{
public:
	GroupNameOpReplyPacket() {};
	GroupNameOpReplyPacket(unsigned char* buf, int len);
	GroupNameOpReplyPacket(const GroupNameOpReplyPacket &rhs);
	virtual ~GroupNameOpReplyPacket() {};
	
	InPacket *copy() { return new GroupNameOpReplyPacket(*this); }
	GroupNameOpReplyPacket &operator=( const GroupNameOpReplyPacket &rhs );
	
	const std::list<std::string> getGroupNames() const { return groupNames; }
	const char getType() const { return type; }
	const bool isDownloadReply() const ;// otherwise, upload reply
protected:	
	virtual void parseBody();
private:
	std::list<std::string> groupNames;
	char type;
};

class DownloadFriendEntry
{
public:
	DownloadFriendEntry() {};
	DownloadFriendEntry(const char *buf);	
	DownloadFriendEntry(const DownloadFriendEntry &rhs);
	~DownloadFriendEntry() {};
		
	DownloadFriendEntry &operator=(const DownloadFriendEntry &rhs);
	
	const int readData(const char *buf);
	const int numberOfBytesRead() const { return 6; }
	
	const unsigned int getQQ() const { return qqNum; }
	const char getType() const { return type; }
	const uint8_t getGroupID() const { return groupID; }
	
	const bool isQun() const ;
private:	
	unsigned int qqNum;
	char type;
	uint8_t groupID;	
};

class DownloadGroupFriendPacket : public OutPacket
{
public:
	DownloadGroupFriendPacket();
	DownloadGroupFriendPacket(const int start);
	DownloadGroupFriendPacket( const DownloadGroupFriendPacket &rhs);
	
	virtual ~DownloadGroupFriendPacket() {};
	
	DownloadGroupFriendPacket &operator=(const DownloadGroupFriendPacket &rhs);
	virtual OutPacket * copy() { return new DownloadGroupFriendPacket(*this); }

	void setStartID(const unsigned int start) { startID = start; }
	const unsigned int getStartID() const { return startID; }
protected:
	virtual int putBody(unsigned char* buf);
private:
	unsigned int startID;
};

class DownloadGroupFriendReplyPacket : public InPacket
{
public:
	DownloadGroupFriendReplyPacket() {}
	DownloadGroupFriendReplyPacket(unsigned char* buf, int len);
	DownloadGroupFriendReplyPacket( const DownloadGroupFriendReplyPacket &rhs);
	
	virtual ~DownloadGroupFriendReplyPacket() {};
	
	InPacket *copy() { return new DownloadGroupFriendReplyPacket( *this); }
	DownloadGroupFriendReplyPacket &operator=( const DownloadGroupFriendReplyPacket &rhs);
	virtual void parseBody();
	
	const unsigned int getNextStartID() const { return nextStartID; }
	const std::list<DownloadFriendEntry> &getGroupedFriends() const { return friends; }
private:
	unsigned int nextStartID;
	std::list<DownloadFriendEntry> friends;
  
};

class UploadGroupFriendPacket : public OutPacket
{
public:
	UploadGroupFriendPacket();
	UploadGroupFriendPacket(const UploadGroupFriendPacket &rhs);
	
	virtual ~UploadGroupFriendPacket();
	
	OutPacket *copy() { return new UploadGroupFriendPacket(*this); }
	UploadGroupFriendPacket &operator=( const UploadGroupFriendPacket &rhs);
	
	// note that the groups do not include Anonymous and Black List groups.
	void setGroupedFriends(std::map<unsigned int, unsigned int> &list) { privateList = list; } 
	const std::map<unsigned int, unsigned int> &getGroupedFriends() const { return privateList; }
protected:
	virtual int putBody(unsigned char* buf);
private:
	std::map<unsigned int, unsigned int> privateList;  // qq number, gourp index
};

class UploadGroupFriendReplyPacket : public InPacket
{
public:
	UploadGroupFriendReplyPacket() {}
	UploadGroupFriendReplyPacket(unsigned char* buf, int len);
	UploadGroupFriendReplyPacket(const UploadGroupFriendReplyPacket &rhs);
	
	virtual ~UploadGroupFriendReplyPacket();
	
	InPacket *copy() { return new UploadGroupFriendReplyPacket(*this); }
	UploadGroupFriendReplyPacket &operator=(const UploadGroupFriendReplyPacket &rhs);
	
	virtual void parseBody();
	
	const bool uploadOk() { return replyCode == 0; }
	const uint8_t getReplyCode() const { return replyCode; }
private:
	uint8_t replyCode;
};

#endif
 
