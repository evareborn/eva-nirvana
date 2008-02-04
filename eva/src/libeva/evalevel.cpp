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

#include "evalevel.h"
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

EvaGetLevelPacket::EvaGetLevelPacket(const std::list<unsigned int> &l)
	: OutPacket(QQ_CMD_GET_LEVEL, true), 
	friends(l)
{
}

EvaGetLevelPacket::EvaGetLevelPacket(const EvaGetLevelPacket &rhs)
	: OutPacket(rhs)
{
	friends = rhs.getList();
}

EvaGetLevelPacket &EvaGetLevelPacket::operator=(const EvaGetLevelPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	friends = rhs.getList();
	return *this;
}

int EvaGetLevelPacket::putBody(unsigned char *buf)
{
	int pos = 0;
	buf[pos++] = 0x00;
	
	int tmp4;
	std::list<unsigned int>::iterator iter;
	for(iter = friends.begin(); iter!=friends.end(); ++iter){
		tmp4 = htonl(*iter);
		memcpy(buf+pos, &tmp4, 4);
		pos+=4;
	}
	return pos;
}


/*** ============================================================== */

EvaGetLevelReplyPacket::EvaGetLevelReplyPacket(unsigned char *buf, int len)
	: InPacket(buf, len)
{ 
} 

EvaGetLevelReplyPacket::EvaGetLevelReplyPacket(const EvaGetLevelReplyPacket &rhs)
	: InPacket(rhs)
{
	replyCode = rhs.getReplyCode();
	friends = rhs.getLevelList();
}

EvaGetLevelReplyPacket &EvaGetLevelReplyPacket::operator=(const EvaGetLevelReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	replyCode = rhs.getReplyCode();
	friends = rhs.getLevelList();
	return *this;
}

void EvaGetLevelReplyPacket::parseBody()
{
	replyCode = decryptedBuf[0];
	
	int pos = 1;
	int tmp4;
	short tmp2;
	
	friends.clear();
	LevelUserItem item;
	while( pos < bodyLength ){
		memcpy(&tmp4, decryptedBuf + pos, 4);
		item.qqNum = ntohl(tmp4);
		pos+=4;
		
		memcpy(&tmp4, decryptedBuf + pos, 4);
		item.onlineTime = ntohl(tmp4);
		pos+=4;
		
		memcpy(&tmp2, decryptedBuf + pos, 2);
		item.level = ntohs(tmp2);
		pos+=2;
		
		memcpy(&tmp2, decryptedBuf + pos, 2);
		item.timeRemainder = ntohs(tmp2);
		pos+=2;
		
		friends.push_back(item);
	}
}

