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
#ifndef EVAREQUESTKEYPACKET_H
#define EVAREQUESTKEYPACKET_H

#include "evapacket.h"

class EvaRequestKeyPacket : public OutPacket
{
public:
	EvaRequestKeyPacket();
	EvaRequestKeyPacket(const unsigned char requestType);
	EvaRequestKeyPacket(const EvaRequestKeyPacket &rhs);
	virtual ~EvaRequestKeyPacket();
	
	EvaRequestKeyPacket &operator=(const EvaRequestKeyPacket &rhs);
	
	void setRequest(const unsigned char request){ this->request = request; }
	const unsigned char getRequest() const { return request; }

protected:
	virtual int putBody(unsigned char* buf);

private:
	unsigned char request;    
};

class EvaRequestKeyReplyPacket : public InPacket
{
public:
	EvaRequestKeyReplyPacket(){}
	EvaRequestKeyReplyPacket(unsigned char* buf, int len);
	EvaRequestKeyReplyPacket(const EvaRequestKeyReplyPacket &rhs);
	virtual ~EvaRequestKeyReplyPacket() {};
	
	const unsigned char getKeyType() const { return keyType; }
	const unsigned char *getKey() const { return key; }
	const unsigned char *getToken() const { return token; }
	const int getTokenLength() const { return tokenLength; }
	const unsigned char getReplyCode() const { return replyCode; }
	
	const bool isReplyOK() const { return replyCode == QQ_REQUEST_KEY_REPLY_OK; }

        EvaRequestKeyReplyPacket &operator=(const EvaRequestKeyReplyPacket &rhs);
protected:
	virtual void parseBody();
private:
	unsigned char replyCode;
	unsigned char keyType;
	unsigned char key[16];
	unsigned char token[256];
	int tokenLength;
};

#endif
