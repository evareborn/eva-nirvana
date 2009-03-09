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
#include "evafriend.h"
#include "evadefines.h"
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring>
#include <cstdlib>

FriendItem::FriendItem(const unsigned int id)
	:qqNum(id), face(0), 
	age(0), gender(0),
	nick(""), qunGroupIndex(0),
	qunAdminValue(0), m_QunRealName(""),
	mIsOnline(false)
{
}

FriendItem::FriendItem(const unsigned char *buf, int *len)
	: mIsOnline(false)
{
    parseData(buf, len);
}

FriendItem::FriendItem(const FriendItem &rhs)
{
	*this = rhs;
}

FriendItem &FriendItem::operator=(const FriendItem &rhs)
{
	qqNum = rhs.getQQ();
	face = rhs.getFace() ;
	age = rhs.getAge();
	gender = rhs.getGender();
	nick = rhs.getNick();
	extFlag = rhs.getExtFlag();
	commonFlag = rhs.getCommonFlag();
	loginTime = rhs.getLoginTime();
	idleTime = rhs.getIdleTime();
	lastRefreshTime = rhs.getLastRefreshTime();
	mIsOnline = rhs.isOnline();
	qunGroupIndex = rhs.getQunGroupIndex();
	qunAdminValue = rhs.getQunAdminValue();
	m_QunRealName = rhs.getQunRealName();    // original by henry
        return *this;
}

void FriendItem::parseData(const unsigned char *buf, int *len)
{
	unsigned int qqtemp;
	memcpy(&qqtemp, buf, 4);
	qqNum = ntohl(qqtemp);
	short tmp2;
	memcpy(&tmp2, buf+4, 2);
	face = ntohs(tmp2);
	age = buf[6];
	gender = buf[7];
	int nickLen = (int)(buf[8]);
	char *b = (char *)malloc( (nickLen+1)*sizeof(char)); 
	memcpy(b, buf+9, nickLen);
	b[nickLen]=0x00;
	nick.assign(b);
	free(b);
	// 2 bits unknown, ignore
	extFlag = buf[9+nickLen+2];
	commonFlag = buf[9+nickLen+3];
	*len = 9+nickLen+4;
}

/*  ======================================================= */

GetFriendListPacket::GetFriendListPacket() 
	: OutPacket(QQ_CMD_GET_FRIEND_LIST, true),
	  startPosition(QQ_FRIEND_LIST_POSITION_START)
{ 
}

GetFriendListPacket::GetFriendListPacket(const unsigned short startPosition)
	: OutPacket(QQ_CMD_GET_FRIEND_LIST, true),
	  startPosition(startPosition)
{
}

GetFriendListPacket::GetFriendListPacket(const GetFriendListPacket &rhs)
	: OutPacket(rhs)
{
	startPosition = rhs.getStartPosition();
}

GetFriendListPacket &GetFriendListPacket::operator=( const GetFriendListPacket &rhs)
{
	*((OutPacket*)this) = (OutPacket)rhs;
	startPosition = rhs.getStartPosition();
        return *this;
}

int GetFriendListPacket::putBody(unsigned char *buf)
{
	unsigned short pos = htons(startPosition);
	memcpy(buf, &pos, 2);
	
	buf[2] = QQ_FRIEND_LIST_UNSORTED;
	buf[3] = 0x00;
	buf[4] = 0x00;
	return 5;
}

/*  ======================================================= */

GetFriendListReplyPacket::GetFriendListReplyPacket(unsigned char *buf, int len)
	: InPacket(buf, len)
{    
}

GetFriendListReplyPacket::GetFriendListReplyPacket(const GetFriendListReplyPacket &rhs)
	:InPacket(rhs)
{
	position = rhs.getPosition();
	friends = rhs.getFriendList();
}

GetFriendListReplyPacket &GetFriendListReplyPacket::operator=( const GetFriendListReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	position = rhs.getPosition();
	friends = rhs.getFriendList();
        return *this;
}

void GetFriendListReplyPacket::parseBody()
{
	short pos;
	memcpy(&pos, decryptedBuf, 2);
	position = ntohs(pos);
	int offset = 2;
	while(bodyLength > offset) {
		int tmp=0;
		friends.push_back(FriendItem(decryptedBuf+offset, &tmp));       
		offset+=tmp;
	}
}


 
