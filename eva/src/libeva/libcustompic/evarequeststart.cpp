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

#include "evarequeststart.h"
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring>

EvaRequestStartPacket::EvaRequestStartPacket()
	: EvaPicOutPacket(QQ_05_CMD_REQUEST_START, true), requestSend(true)
{
	cryptPosition = 12;
}

EvaRequestStartPacket::EvaRequestStartPacket(const EvaRequestStartPacket &rhs)
	: EvaPicOutPacket(rhs)
{
	*this = rhs;
}

EvaRequestStartPacket &EvaRequestStartPacket::operator=(const EvaRequestStartPacket &rhs)
{
	*((EvaPicOutPacket*)this) = (EvaPicOutPacket)rhs;
	sessionID = rhs.getSessionID();
	memcpy(md5, rhs.getMd5(), 16);
	requestSend = rhs.isRequestSend();
	return *this;
}

void EvaRequestStartPacket::setMd5(const unsigned char *value)
{
	memcpy(md5, value, 16);
}

int EvaRequestStartPacket::putBody(unsigned char *buf)
{
	int pos=0;
	unsigned int tmp4_1=1, tmp4_2=0;
	if(requestSend){
		buf[pos++] = 0x01;
		memcpy(buf+pos, md5+4, 7), pos+=7;
	}else{
		memcpy(buf+pos, &tmp4_1, 4); pos+=4;
		memcpy(buf+pos, &tmp4_2, 4); pos+=4;
	}
	
	tmp4_1 = htonl(sessionID);
	memcpy(buf+pos, &tmp4_1, 4); pos+=4;
	
	if(requestSend){
		buf[pos++] = 0x04;
		buf[pos++] = 0x4c;
	}else{
		memset(buf+pos, 0, 2);pos+=2;
	}
	
	return pos;
}


/* =========================================================== */


EvaRequestStartReplyPacket::EvaRequestStartReplyPacket(unsigned char *buf, int len)
	: EvaPicInPacket(buf, len, 12)
{
}

EvaRequestStartReplyPacket::EvaRequestStartReplyPacket(const EvaRequestStartReplyPacket &rhs)
	: EvaPicInPacket(rhs)
{
	*this = rhs;
}

EvaRequestStartReplyPacket &EvaRequestStartReplyPacket::operator=(const EvaRequestStartReplyPacket &rhs)
{
	*((EvaPicInPacket*)this) = (EvaPicInPacket)rhs;
	sessionID = rhs.getSessionID();
	return *this;
}

void EvaRequestStartReplyPacket::parseBody()
{
	int pos = 0;
	pos+=8;
	unsigned int tmp4;
	memcpy(&tmp4, decryptedBuf+pos, 4);
	sessionID = ntohl(tmp4);
}

