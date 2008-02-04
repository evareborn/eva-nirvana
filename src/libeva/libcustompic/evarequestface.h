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
 
#ifndef LIBCUSTOMPIC_EVAREQUESTFACE_H
#define LIBCUSTOMPIC_EVAREQUESTFACE_H 

#include "evapicpacket.h"

class EvaRequestFacePacket : public EvaPicOutPacket {
public:
	EvaRequestFacePacket();
	EvaRequestFacePacket(const EvaRequestFacePacket &rhs);
	virtual ~EvaRequestFacePacket();
	
	EvaRequestFacePacket &operator=(const EvaRequestFacePacket &rhs);
	
	void setQunID(const int id) { qunID = id;}
	void setSessionID(const unsigned int id) { sessionID= id;}
	void setFileAgentToken(const unsigned char *token, const int len);
	
	const int getQunID() const { return qunID; }
	const unsigned int getSessionID() const { return sessionID; }
	const int getTokenLength() const { return tokenLength; }
	const unsigned char *getFileAgentToken() const { return fileAgentToken; }
protected:
	virtual int putBody(unsigned char *buf);
private:
	int qunID;
	unsigned int sessionID;
	unsigned char *fileAgentToken;
	int tokenLength;
};

class EvaRequestFaceReplyPacket: public EvaPicInPacket {
public:
	EvaRequestFaceReplyPacket(){};
	EvaRequestFaceReplyPacket(unsigned char *buf, int len);
	EvaRequestFaceReplyPacket(const EvaRequestFaceReplyPacket &rhs);
	virtual ~EvaRequestFaceReplyPacket(){};
	
	EvaRequestFaceReplyPacket &operator=(const EvaRequestFaceReplyPacket &rhs);
	
	const unsigned int getSessionID() const { return sessionID; }
protected:
	virtual void parseBody();
private:
	unsigned int sessionID;
};

#endif

