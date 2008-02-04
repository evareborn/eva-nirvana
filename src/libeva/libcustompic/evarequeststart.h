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
 
#ifndef LIBCUSTOMPIC_EVAREQUESTSTART_H
#define LIBCUSTOMPIC_EVAREQUESTSTART_H 

#include "evapicpacket.h"

class EvaRequestStartPacket : public EvaPicOutPacket {
public:
	EvaRequestStartPacket();
	EvaRequestStartPacket(const EvaRequestStartPacket &rhs);
	virtual ~EvaRequestStartPacket(){};
	
	EvaRequestStartPacket &operator=(const EvaRequestStartPacket &rhs);
	
	void setSessionID(const unsigned int id) { sessionID= id;}
	void setMd5(const unsigned char *value);
	void setRequestSend( const bool ok) { requestSend = ok; }
	
	const unsigned int getSessionID() const { return sessionID; }
	const unsigned char *getMd5() const { return md5; }
	const bool isRequestSend() const { return requestSend; }
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned int sessionID;
	unsigned char md5[16];
	bool requestSend;
};

class EvaRequestStartReplyPacket: public EvaPicInPacket {
public:
	EvaRequestStartReplyPacket(){};
	EvaRequestStartReplyPacket(unsigned char *buf, int len);
	EvaRequestStartReplyPacket(const EvaRequestStartReplyPacket &rhs);
	virtual ~EvaRequestStartReplyPacket(){};
	
	EvaRequestStartReplyPacket &operator=(const EvaRequestStartReplyPacket &rhs);
	
	const unsigned int getSessionID() const { return sessionID; }
protected:
	virtual void parseBody();
private:
	unsigned int sessionID;
};

#endif

 
