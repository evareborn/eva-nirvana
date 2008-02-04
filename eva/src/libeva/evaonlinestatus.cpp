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
 
#include "evaonlinestatus.h"
#include "evadefines.h"
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

// class FriendStatus for private use
class FriendStatus {
public:
	FriendStatus() : numOfBytes(0) {};
	FriendStatus(unsigned char * buf);
	~FriendStatus() {};
	
	unsigned int qqNum;
	char unknown4;
	unsigned int ip;  // 4 bytes
	unsigned short port;
	char unknown11;
	char status;
	short unknown13_14;
	unsigned char unknownKey[QQ_KEY_LENGTH];
	
	int numOfBytes;
	
	FriendStatus & operator=(const FriendStatus& other);
	
	int readData(unsigned char * buf);
}; 

FriendStatus::FriendStatus(unsigned char * buf)
{
	numOfBytes = readData(buf);
}
	
FriendStatus &FriendStatus::operator= (const FriendStatus& other)
{
	qqNum = other.qqNum;
	unknown4 = other.unknown4;
	ip = other.ip;  // 4 bytes
	port = other.port;
	unknown11 = other.unknown11;
	status = other.status;
	unknown13_14 = other.unknown13_14;
	memcpy(unknownKey, other.unknownKey, QQ_KEY_LENGTH);
	
	numOfBytes = other.numOfBytes;
	return *this;
}

int FriendStatus::readData(unsigned char * buf)
{
	int tmp4;
	memcpy(&tmp4,buf,4);
	// 000-003:  qq id 
	qqNum = ntohl(tmp4);
	unknown4 = buf[4];
	// 005-008: IP	
	memcpy(&tmp4, buf+5, 4);
	ip = ntohl(tmp4);
	// 009-010: port
	short tmp2;
	memcpy(&tmp2, buf+9, 2);
	port = ntohs(tmp2);
	unknown11 = buf[11];
	// 012: status
	status = buf[12];
	memcpy(&tmp2, buf+13, 2);
	unknown13_14 = ntohs(tmp2);
	memcpy(unknownKey, buf+15, QQ_KEY_LENGTH);
	numOfBytes = 31; // always 31 bytes31
	return numOfBytes;
}

/*  ======================================================= */

FriendOnlineEntry::FriendOnlineEntry()
	:numOfBytes(0)
{
}

FriendOnlineEntry::FriendOnlineEntry(const FriendOnlineEntry &rhs)
{
	status = new FriendStatus();
	status->qqNum = rhs.getQQ();
	status->unknown4 = rhs.getUnknown1_4();
	status->ip = rhs.getIP();  // 4 bytes
	status->port = rhs.getPort();
	status->unknown11 = rhs.getUnknown2_11();
	status->status = rhs.getStatus();
	status->unknown13_14 = rhs.getUnknown3_13_14();
	memcpy(status->unknownKey, rhs.getUnknownKey(), QQ_KEY_LENGTH);
	
	unknown31_32 = rhs.getUnknown4_31_32();
	extFlag = rhs.getExtFlag();
	commFlag = rhs.getCommFlag();
	unknown35_36 = rhs.getUnknown5_35_36();
	ending = rhs.getEnd();
		
	numOfBytes = rhs.numOfBytes;
}

FriendOnlineEntry::~FriendOnlineEntry()
{ 
	delete status; 
}

const unsigned int FriendOnlineEntry::getQQ() const  { return status->qqNum;}
const unsigned int FriendOnlineEntry::getIP() const  { return status->ip;}
const unsigned short FriendOnlineEntry::getPort() const  { return status->port;}
const char FriendOnlineEntry::getStatus() const  { return status->status;}
const char FriendOnlineEntry::getUnknown1_4() const  { return status->unknown4;}
const char FriendOnlineEntry::getUnknown2_11() const  { return status->unknown11;}
const short FriendOnlineEntry::getUnknown3_13_14() const  { return status->unknown13_14;}
const unsigned char * FriendOnlineEntry::getUnknownKey() const  { return status->unknownKey;}


int FriendOnlineEntry::readData(unsigned char * buf)
{
	status = new FriendStatus();
	status->readData(buf);
	unknown31_32 = (short)(buf[31])<<8 + buf[32];
	extFlag = buf[33];
	commFlag = buf[34];
	unknown35_36 = (short)(buf[35])<<8 + buf[36];
	ending = buf[37];
	numOfBytes = 38;
	return numOfBytes;
}

FriendOnlineEntry & FriendOnlineEntry::operator=(const FriendOnlineEntry &rhs)
{
	status = new FriendStatus();
	status->qqNum = rhs.getQQ();
	status->unknown4 = rhs.getUnknown1_4();
	status->ip = rhs.getIP();  // 4 bytes
	status->port = rhs.getPort();
	status->unknown11 = rhs.getUnknown2_11();
	status->status = rhs.getStatus();
	status->unknown13_14 = rhs.getUnknown3_13_14();
	memcpy(status->unknownKey, rhs.getUnknownKey(), QQ_KEY_LENGTH);
	
	unknown31_32 = rhs.getUnknown4_31_32();
	extFlag = rhs.getExtFlag();
	commFlag = rhs.getCommFlag();
	unknown35_36 = rhs.getUnknown5_35_36();
	ending = rhs.getEnd();
		
	numOfBytes = rhs.numOfBytes;
	return *this;
}

/*  ======================================================= */

GetOnlineFriendsPacket::GetOnlineFriendsPacket()
	: OutPacket(QQ_CMD_GET_FRIEND_ONLINE, true),
	  startPosition (QQ_FRIEND_ONLINE_LIST_POSITION_START)
{
}

GetOnlineFriendsPacket::GetOnlineFriendsPacket(const unsigned char position)
	: OutPacket(QQ_CMD_GET_FRIEND_ONLINE, true),
	  startPosition (position)
{
}
	 
GetOnlineFriendsPacket::GetOnlineFriendsPacket(const GetOnlineFriendsPacket &rhs)
	: OutPacket(rhs)
{
	startPosition = rhs.getStartPosition();
}

GetOnlineFriendsPacket &GetOnlineFriendsPacket::operator=(const GetOnlineFriendsPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	startPosition = rhs.getStartPosition(); 
        return *this;
}

int GetOnlineFriendsPacket::putBody(unsigned char *buf) 
{
	buf[0] = 0x02;
	buf[1]=startPosition;
	memset(buf+2, 0, 3);
	return 5;
}

/*  ======================================================= */

GetOnlineFriendReplyPacket::GetOnlineFriendReplyPacket(unsigned char *buf, int len) 
	: InPacket(buf, len)
{
}

GetOnlineFriendReplyPacket::GetOnlineFriendReplyPacket(const GetOnlineFriendReplyPacket &rhs)
	: InPacket(rhs)
{
	position = rhs.getPosition();
	onlineFriends = rhs.getOnlineFriendList();
}

GetOnlineFriendReplyPacket &GetOnlineFriendReplyPacket::operator=(const GetOnlineFriendReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	position = rhs.getPosition();
	onlineFriends = rhs.getOnlineFriendList();
        return *this;
}

void GetOnlineFriendReplyPacket::parseBody()
{
	position = decryptedBuf[0];
	int offset = 1;
	while(bodyLength>offset) {
		FriendOnlineEntry entry;
		offset+= entry.readData(decryptedBuf+offset);    
		onlineFriends.push_back(entry);
	}
}

/*  ======================================================= */

FriendChangeStatusPacket::FriendChangeStatusPacket(unsigned char* buf, int len)
	: InPacket(buf, len), status(NULL)
{
}

FriendChangeStatusPacket::FriendChangeStatusPacket(const FriendChangeStatusPacket &rhs)
	: InPacket(rhs), status(NULL)
{
	status = new FriendStatus();
	status->qqNum = rhs.getQQ();
	status->unknown4 = rhs.getUnknown1_4();
	status->ip = rhs.getIP();  // 4 bytes
	status->port = rhs.getPort();
	status->unknown11 = rhs.getUnknown2_11();
	status->status = rhs.getStatus();
	status->unknown13_14 = rhs.getUnknown3_13_14();
	memcpy(status->unknownKey, rhs.getUnknownKey(), QQ_KEY_LENGTH);
	
	myQQNum = rhs.getMyQQ();
}

FriendChangeStatusPacket::~FriendChangeStatusPacket()
{
	if(status)
		delete status;
}

FriendChangeStatusPacket &FriendChangeStatusPacket::operator=(const FriendChangeStatusPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	status = new FriendStatus();
	status->qqNum = rhs.getQQ();
	status->unknown4 = rhs.getUnknown1_4();
	status->ip = rhs.getIP();  // 4 bytes
	status->port = rhs.getPort();
	status->unknown11 = rhs.getUnknown2_11();
	status->status = rhs.getStatus();
	status->unknown13_14 = rhs.getUnknown3_13_14();
	memcpy(status->unknownKey, rhs.getUnknownKey(), QQ_KEY_LENGTH);
	
	myQQNum = rhs.getMyQQ();
        return *this;
}

void FriendChangeStatusPacket::parseBody()
{
	status = new FriendStatus();
	status->readData(decryptedBuf);
	
	// friendStatus read 31 bytes, but there still 4 bytes unknown, ignore them
	memcpy(&myQQNum,decryptedBuf + 4 + 31, 4);
	// this is not that useful, just means the message's receiver should hold this account
	myQQNum = ntohl(myQQNum);
}

const unsigned int FriendChangeStatusPacket::getQQ() const  { return status->qqNum;}
const unsigned int FriendChangeStatusPacket::getIP() const  { return status->ip;}
const unsigned short FriendChangeStatusPacket::getPort() const  { return status->port;}
const char FriendChangeStatusPacket::getStatus() const  { return status->status;}
const char FriendChangeStatusPacket::getUnknown1_4() const  { return status->unknown4;}
const char FriendChangeStatusPacket::getUnknown2_11() const  { return status->unknown11;}
const short FriendChangeStatusPacket::getUnknown3_13_14() const  { return status->unknown13_14;}
const unsigned char * FriendChangeStatusPacket::getUnknownKey() const  { return status->unknownKey;}


/*  ======================================================= */

ChangeStatusPacket::ChangeStatusPacket() 
	: OutPacket(QQ_CMD_CHANGE_STATUS, true) 
{
	myStatus = QQ_FRIEND_STATUS_INVISIBLE;
	miscStatus=0;
}

ChangeStatusPacket::ChangeStatusPacket(char status) 
	: OutPacket(QQ_CMD_CHANGE_STATUS, true) 
{
	myStatus = status;
}

ChangeStatusPacket::ChangeStatusPacket(const ChangeStatusPacket &rhs)
	: OutPacket(rhs)
{
	myStatus = rhs.getOnlineStatus();
}

ChangeStatusPacket &ChangeStatusPacket::operator=(const ChangeStatusPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	myStatus = rhs.getOnlineStatus();
        return *this;
}

int ChangeStatusPacket::putBody(unsigned char *buf) 
{
	buf[0]=myStatus;
	// show fake video
	//memset(buf+1, 0, 4);
	*((unsigned int*)buf+1)=miscStatus;
	//memmove(buf+1,&miscStatus,sizeof(unsigned int));
	
	return 5;
}

/*  ======================================================= */

ChangeStatusReplyPacket::ChangeStatusReplyPacket(unsigned char *buf, int len) 
	: InPacket(buf, len)
{ 
}

ChangeStatusReplyPacket::ChangeStatusReplyPacket(const ChangeStatusReplyPacket &rhs)
	: InPacket(rhs)
{
	replyCode = rhs.getReplyCode(); 
}        
    
void ChangeStatusReplyPacket::parseBody()
{
    replyCode = decryptedBuf[0];
}

const bool ChangeStatusReplyPacket::isAccepted() const
{
	return ( replyCode == QQ_CHANGE_STATUS_REPLY_OK);
}

ChangeStatusReplyPacket &ChangeStatusReplyPacket::operator=(const ChangeStatusReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	replyCode = rhs.getReplyCode();
        return *this;
}
