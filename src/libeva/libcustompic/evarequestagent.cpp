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

#include "evarequestagent.h" 
#include "../evautil.h"
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring>

EvaRequestAgentPacket::EvaRequestAgentPacket( const unsigned char *token, const unsigned short length)
	: EvaPicOutPacket(QQ_05_CMD_REQUEST_AGENT, false)
{
	tokenLength = length;
	cryptPosition = 14 + tokenLength;
	fileAgentToken = new unsigned char [ tokenLength ];
	memcpy(fileAgentToken, token, length);
}

EvaRequestAgentPacket::EvaRequestAgentPacket( const EvaRequestAgentPacket &rhs )
	: EvaPicOutPacket(rhs)
{
	*this = rhs;
}

EvaRequestAgentPacket::~ EvaRequestAgentPacket( )
{
	delete fileAgentToken;
}

EvaRequestAgentPacket &EvaRequestAgentPacket::operator=(const EvaRequestAgentPacket &rhs)
{
	*((EvaPicOutPacket*)this) = (EvaPicOutPacket)rhs;
	qunID = rhs.getQunID();
	memcpy(md5, rhs.getMD5(), 16);
	imageLength = rhs.getImageLength();
	fileName = rhs.getFileName();
	tokenLength = rhs.getTokenLength();
	memcpy(fileAgentToken, rhs.getFileAgentToken(), tokenLength);
	return *this;
}

void EvaRequestAgentPacket::setMd5(const unsigned char *value)
{
	memcpy(md5, value, 16);
}

int EvaRequestAgentPacket::putBody(unsigned char *buf)
{
	int pos=0;
	unsigned int tmp4_1 = htonl(0x01000000), tmp4_2 = 0;
	memcpy(buf+pos, &tmp4_1, 4);
	pos+=4;
	memcpy(buf+pos, &tmp4_2, 4);
	pos+=4;                    // ignore 8 bytes
	
	memset(buf+pos, 0, 4); pos+=4;
	
	unsigned short tmp2;
	tmp2 = htons(tokenLength);
	memcpy(buf+pos, &tmp2, 2); pos+=2;
	
	memcpy(buf+pos, fileAgentToken, tokenLength); pos+=tokenLength;
	
	buf[pos++] = 0x04;
	buf[pos++] = 0x4c;
	
	tmp4_1 = htonl(qunID);
	memcpy(buf+pos, &tmp4_1, 4); pos+=4;
	
	tmp4_1 = htonl(imageLength);
	memcpy(buf+pos, &tmp4_1, 4); pos+=4;
	
	memcpy(buf+pos, md5, 16); pos+=4;
	
	memcpy(buf+pos, EvaUtil::doMd5((char*)fileName.c_str(), fileName.length()), 16); pos+=16;
	
	memset(buf+pos, 0, 2); pos+=2;
	
	return pos;
}


/****************************************************************************************************************************************************/

RequestAgentReplyPacket::RequestAgentReplyPacket(unsigned char *buf, int len)
	: EvaPicInPacket(buf, len, 8)
{
}

RequestAgentReplyPacket::RequestAgentReplyPacket(const RequestAgentReplyPacket &rhs)
	: EvaPicInPacket(rhs)
{
	*this = rhs;
}

RequestAgentReplyPacket &RequestAgentReplyPacket::operator=(const RequestAgentReplyPacket &rhs)
{
	*((EvaPicInPacket *)this) = (EvaPicInPacket)rhs;
	replyCode = rhs.getReplyCode();
	sessionID = rhs.getSessionID();
	redirectIP = rhs.getRedirectIP();
	redirectPort = rhs.getRedirectPort();
	message = rhs.getMessage();
	return *this;
}

void RequestAgentReplyPacket::parseBody()
{
	int pos = 0;
	
	pos+=8; // ignore 8 bytes;
	
	unsigned short tmp2;
	memcpy(&tmp2, decryptedBuf+pos, 2); pos+=2;
	replyCode = ntohs(tmp2);
	
	pos+=4; // ignore the reply server ip
	pos+=2; // ignore the reply server port
	
	unsigned int tmp4;
	memcpy(&tmp4, decryptedBuf+pos, 4); pos+=4;
	sessionID = ntohl(tmp4);
	
	memcpy(&tmp4, decryptedBuf+pos, 4); pos+=4;
	redirectIP = tmp4;  // note that: the ip is already in little endian format, so we don't need ntohl
	
	memcpy(&tmp2, decryptedBuf+pos, 2); pos+=2;
	redirectPort = ntohs(tmp2);
	
	memcpy(&tmp2, decryptedBuf+pos, 2); pos+=2;
	unsigned short len = ntohs(tmp2);
	
	char *str = new char [len + 1];
	memcpy(str, decryptedBuf+pos, len); pos += len;
	str[len] = 0x00;
	message.assign(str);
	delete str;
}
