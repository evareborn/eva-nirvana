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
#ifndef LIBEVALOGINTOKEN_H
#define LIBEVALOGINTOKEN_H

#include "evapacket.h"
#include <string>

class RequestLoginTokenPacket : public OutPacket{
public:
	RequestLoginTokenPacket();
	RequestLoginTokenPacket(const RequestLoginTokenPacket &);
	virtual ~RequestLoginTokenPacket(){};
	
	OutPacket * copy() { return new RequestLoginTokenPacket(*this);}
	RequestLoginTokenPacket &operator=(const RequestLoginTokenPacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);
};

class RequestLoginTokenReplyPacket : public InPacket {
public:
	RequestLoginTokenReplyPacket();
	RequestLoginTokenReplyPacket(unsigned char *buf, int len) ;  
	RequestLoginTokenReplyPacket(const RequestLoginTokenReplyPacket &rhs);
	virtual ~RequestLoginTokenReplyPacket();
	
	InPacket * copy() { return new RequestLoginTokenReplyPacket(*this);}
	RequestLoginTokenReplyPacket &operator=(const RequestLoginTokenReplyPacket &rhs);
	
	const int getTokenLength() const { return length; }
	const unsigned char *getToken() const { return token; }
	
	const bool isReplyOk() const ;
	const char getReplyCode() const { return replyCode; }
protected:
	virtual void parseBody();
private:
	char replyCode;
	int length;
	unsigned char *token;
};

/**
 *  \brief New login token requirement
 *  this packet was firstly seen in the QQ 2006 beta 3.
 *  It send fixed 7 bytes to server, "01 00 05 00 00 00 00"
 *  I didn't dig that deep to see the details of each byte.
 *  as we could satisfy out needs already :)
 *  Also, this packet takes a different encryption mechanism
 *
 *  yunfan, 20/10/2006
 *
 */
class RequestLoginTokenExPacket : public OutPacket{
public:
	RequestLoginTokenExPacket(const unsigned char type = QQ_LOGIN_TOKEN_REQUEST);
	RequestLoginTokenExPacket(const RequestLoginTokenExPacket & rhs);
	virtual ~RequestLoginTokenExPacket();

	RequestLoginTokenExPacket &operator=(const RequestLoginTokenExPacket &rhs);
	void setCode(const std::string &code) {  m_Code = code; }
	void setToken(const unsigned char *token, unsigned short len);

	const unsigned char getType() const { return m_Type; }
	const std::string &getCode() const { return m_Code; }
	const unsigned char *getToken() const { return m_Token; }
	const unsigned short getTokenLength() const { return m_TokenLen; }
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned char m_Type;
	std::string m_Code;
	unsigned char *m_Token;
	unsigned short m_TokenLen;
};

class RequestLoginTokenExReplyPacket : public InPacket {
public:
	RequestLoginTokenExReplyPacket();
	RequestLoginTokenExReplyPacket(unsigned char *buf, int len) ;
	RequestLoginTokenExReplyPacket(const RequestLoginTokenExReplyPacket &rhs);
	virtual ~RequestLoginTokenExReplyPacket();
	
	RequestLoginTokenExReplyPacket &operator=(const RequestLoginTokenExReplyPacket &rhs);

	const unsigned char getType() const { return m_Type; }
	const unsigned char getReplyCode() const { return m_ReplyCode; }

	const unsigned short getTokenLength() const { return m_TokenLen; }
	const unsigned char *getToken() const { return m_Token; }

	const unsigned char *getData() const { return m_Data; }
	const unsigned int getDataLength() const { return m_DataLen; }
protected:
	virtual void parseBody();
private:
	unsigned char m_Type, m_ReplyCode, *m_Token, *m_Data;
	unsigned short m_TokenLen, m_DataLen;
};

class ServerDetectorPacket : public OutPacket {
public:
	ServerDetectorPacket();
	ServerDetectorPacket(const ServerDetectorPacket &rhs);
	virtual ~ServerDetectorPacket();

	ServerDetectorPacket &operator=(const ServerDetectorPacket &rhs);
	
	const unsigned int getFromIP() const { return m_FromIP; }
	const unsigned char getStep() const { return m_Step; }

	static void setFromIP(const unsigned int ip) { m_FromIP = ip; }
	static void setStep( const unsigned char step) { m_Step = step; }
	static void nextStep() { m_Step++; }
protected:
	virtual int putBody(unsigned char *buf);
private:
	static unsigned int m_FromIP;
	static unsigned char m_Step;
};

class ServerDetectorReplyPacket : public InPacket {
public:
	ServerDetectorReplyPacket();
	ServerDetectorReplyPacket(unsigned char *buf, int len) ;
	ServerDetectorReplyPacket(const ServerDetectorReplyPacket &rhs);
	ServerDetectorReplyPacket &operator=(const ServerDetectorReplyPacket &rhs);

	const unsigned int getRedirectIP() const { return m_IP; }
	const unsigned short getReplyCode() const { return m_ReplyCode; }
	const bool isServerReady() const { return m_ReplyCode == QQ_CMD_SERVER_DETECT_REPLY_OK; }
	const bool needRedirect() const { return m_ReplyCode == QQ_CMD_SERVER_DETECT_REPLY_REDIRECT; }
protected:
	virtual void parseBody();
private:
	unsigned short m_ReplyCode;
	unsigned int m_IP;
};

#endif
