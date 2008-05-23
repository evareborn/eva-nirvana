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
 
#include "evarequestface.h"
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring>

EvaRequestFacePacket::EvaRequestFacePacket()
	: EvaPicOutPacket(QQ_05_CMD_REQUEST_FACE, true), fileAgentToken(NULL)
{
	cryptPosition = 12;
}

EvaRequestFacePacket::EvaRequestFacePacket(const EvaRequestFacePacket &rhs)
	: EvaPicOutPacket(rhs), fileAgentToken(NULL)
{
	*this = rhs;
}

EvaRequestFacePacket::~EvaRequestFacePacket()
{
	if(fileAgentToken)
		delete fileAgentToken;
}

EvaRequestFacePacket &EvaRequestFacePacket::operator=(const EvaRequestFacePacket &rhs)
{
	*((EvaPicOutPacket*)this) = (EvaPicOutPacket)rhs;
	qunID = rhs.getQunID();
	sessionID = rhs.getSessionID();
	
	tokenLength = rhs.getTokenLength();
	setFileAgentToken(rhs.getFileAgentToken(), tokenLength);
	return *this;
}

void EvaRequestFacePacket::setFileAgentToken(const unsigned char *token, const int len)
{
	if(!fileAgentToken)
		fileAgentToken = new unsigned char[len];
	tokenLength = len;
	memcpy(fileAgentToken, token, tokenLength);
}

int EvaRequestFacePacket::putBody(unsigned char *buf)
{
	int pos = 0;
	int tmp4_1, tmp4_2;
	memcpy(buf, &tmp4_1, 4);
	pos+=4;
	memcpy(buf+pos, &tmp4_2, 4); // unknown 8 bytes
	pos+=4;
	
	tmp4_1 = htonl(sessionID);
	memcpy(buf+pos, &tmp4_1, 4);
	pos+=4;
	
	unsigned short tmp2;
	tmp2 = htons(tokenLength) & 0xffff;
	memcpy(buf+pos, &tmp2, 2);
	pos+=2;
	
	memcpy(buf+pos, fileAgentToken, tokenLength);
	pos+=tokenLength;
	
	tmp4_1 = htonl(qunID);
	memcpy(buf+pos, &tmp4_1, 4);
	pos+=4;
	
	return pos;
}


/***********************************************************************************************************************/


EvaRequestFaceReplyPacket::EvaRequestFaceReplyPacket(unsigned char *buf, int len)
	: EvaPicInPacket(buf, len, 12)
{
}

EvaRequestFaceReplyPacket::EvaRequestFaceReplyPacket(const EvaRequestFaceReplyPacket &rhs)
	: EvaPicInPacket(rhs)
{
	*this = rhs;
}

EvaRequestFaceReplyPacket &EvaRequestFaceReplyPacket::operator=(const EvaRequestFaceReplyPacket &rhs)
{
	*((EvaPicInPacket*)this) = (EvaPicInPacket)rhs;
	sessionID = rhs.getSessionID();
	return *this;
}

void EvaRequestFaceReplyPacket::parseBody()
{
	int pos = 0;
	pos+=8;  // we ignore the first 8 bytes of body
	int tmp4;
	memcpy(&tmp4, decryptedBuf+pos, 4);
	sessionID = ntohl(tmp4);
}





