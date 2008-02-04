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
 
#ifndef LIBCUSTOMPIC_EVAREQUESTAGENT_H
#define LIBCUSTOMPIC_EVAREQUESTAGENT_H 

#include "evapicpacket.h" 
#include <string>

class EvaRequestAgentPacket : public EvaPicOutPacket{
public:
	EvaRequestAgentPacket(const unsigned char *fileAgentToken, const unsigned short tokenLength);
	EvaRequestAgentPacket(const EvaRequestAgentPacket &rhs);
	virtual ~EvaRequestAgentPacket();
	
	EvaRequestAgentPacket &operator=(const EvaRequestAgentPacket &rhs);
	
	const unsigned int getQunID() const {return qunID; }
	const unsigned char *getMD5() const { return md5;}
	const std::string &getFileName() const { return fileName; }
	const unsigned int getImageLength() const { return imageLength; }

	
	void setQunID(const unsigned int id) { qunID=id;}
	void setMd5(const unsigned char *value);
	void setImageLength(const unsigned short len) { imageLength = len; }
	void setFileName( const std::string &name) { fileName = name ;}
	
	const unsigned char *getFileAgentToken() const { return fileAgentKey; }
	const unsigned short getTokenLength() const { return tokenLength; }
protected:
	int putBody(unsigned char *buf);
private:
	unsigned int qunID;
	
	// for send file info
	unsigned char md5[16]; // always 16 bytes long
	unsigned int imageLength;
	std::string fileName;
	
	unsigned char *fileAgentToken;
	unsigned short tokenLength;
};


class RequestAgentReplyPacket : public EvaPicInPacket {
public:
	RequestAgentReplyPacket() {}
	RequestAgentReplyPacket(unsigned char *buf, int len);
	RequestAgentReplyPacket(const RequestAgentReplyPacket &rhs);
	virtual ~RequestAgentReplyPacket() {};
	
	RequestAgentReplyPacket &operator=(const RequestAgentReplyPacket &rhs);
	
	const unsigned short getReplyCode() const { return replyCode; }
	const unsigned int getSessionID() const { return sessionID; }
	const unsigned int getRedirectIP() const { return redirectIP; }
	const unsigned short getRedirectPort() const { return redirectPort; }
	const std::string &getMessage() const { return message; }
protected:
	void parseBody();
private:
	unsigned short replyCode;
	unsigned int sessionID;
	
	unsigned int redirectIP;
	unsigned short redirectPort;
	std::string message;
};

#endif

