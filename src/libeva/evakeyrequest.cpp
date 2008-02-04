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

#include "evakeyrequest.h"
#include <string.h>

EvaRequestKeyPacket::EvaRequestKeyPacket()
      : OutPacket(QQ_CMD_REQUEST_KEY, true)
{
}

EvaRequestKeyPacket::EvaRequestKeyPacket(const unsigned char requestType)
      : OutPacket(QQ_CMD_REQUEST_KEY, true)
{
	request = requestType;
}

EvaRequestKeyPacket::EvaRequestKeyPacket( const EvaRequestKeyPacket &rhs )
	: OutPacket(rhs)
{
	*this = rhs;
}

EvaRequestKeyPacket::~EvaRequestKeyPacket()
{
}

EvaRequestKeyPacket &EvaRequestKeyPacket::operator=(const EvaRequestKeyPacket &rhs)
{
	*( (OutPacket *)this) = (OutPacket)rhs;
	request = rhs.getRequest();
	return *this;
}

int EvaRequestKeyPacket::putBody(unsigned char* buf)
{
	buf[0]=request;
	return 1;
}



/**********************************************************************************************************************/

EvaRequestKeyReplyPacket::EvaRequestKeyReplyPacket(unsigned char* buf, int len)
	: InPacket(buf, len)
{
}

EvaRequestKeyReplyPacket::EvaRequestKeyReplyPacket(const EvaRequestKeyReplyPacket &rhs)
	: InPacket(rhs)
{
	*this = rhs;
}

EvaRequestKeyReplyPacket &EvaRequestKeyReplyPacket::operator=(const EvaRequestKeyReplyPacket &rhs)
{
	*( (InPacket *)this) = (InPacket)rhs;
	keyType = rhs.getKeyType();
	replyCode = rhs.getReplyCode();
	memcpy(key, rhs.getKey(), 16);
	tokenLength = rhs.getTokenLength();
	memcpy(token, rhs.getToken(), tokenLength);
	return *this;
}

void EvaRequestKeyReplyPacket::parseBody()
{
	keyType = decryptedBuf[0];
	replyCode = decryptedBuf[1];
	if(replyCode == QQ_REQUEST_KEY_REPLY_OK) {            
		memcpy(key, decryptedBuf+2, QQ_KEY_LENGTH);
		
		if(keyType == QQ_REQUEST_FILE_AGENT_KEY) 
			setFileAgentKey(key);                   // now we set file agent key
		
		tokenLength = (int)(decryptedBuf[2+QQ_KEY_LENGTH+12]) & 0xFF;            
		memcpy(token, decryptedBuf+2+QQ_KEY_LENGTH+12+1, tokenLength);
		
		if(keyType == QQ_REQUEST_FILE_AGENT_KEY) 
			setFileAgentToken(token, tokenLength);          // now we can set file agent token
	}
}
