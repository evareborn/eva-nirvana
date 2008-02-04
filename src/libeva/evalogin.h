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
#ifndef LIBEVALOGIN_H
#define LIBEVALOGIN_H

#include "evapacket.h"
#include <string>

class LoginPacket : public OutPacket {    
public:
	LoginPacket();
	LoginPacket(unsigned char loginMode = QQ_LOGIN_MODE_NORMAL);
	LoginPacket(const LoginPacket &rhs);
	virtual ~LoginPacket();
	
	OutPacket * copy() { return new LoginPacket(*this);}
	LoginPacket &operator=(const LoginPacket &rhs);
	const unsigned char getLoginMode() const { return m_LoginMode; }
	unsigned char getNumProcess() const { return m_NumProcess; }
	const unsigned char *getComputerUUID() const { return m_UUID; }

	void setNumProcess(const unsigned char n) { m_NumProcess = n;} // how many qq.exes are running now
	void setComputerUUID(const unsigned char *id) { m_UUID = id;}
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned char m_LoginMode;
	unsigned char m_NumProcess;
	const unsigned char *m_UUID; // 16 bytes
};

class LoginReplyPacket : public InPacket {
public:
	LoginReplyPacket() {}
	LoginReplyPacket(unsigned char *sbuf, int len) ;  
	LoginReplyPacket(const LoginReplyPacket &rhs);
	virtual ~LoginReplyPacket() {};
	
	InPacket * copy() { return new LoginReplyPacket(*this);}
	LoginReplyPacket &operator=(const LoginReplyPacket &rhs);
	const unsigned char getReplyCode() const { return replyCode; };
	const unsigned int getRedirectedIP() const { return redirectedIP; }
	const short getRedirectedPort() const { return redirectedPort; }
	const std::string &getReplyMessage() const { return replyMessage; }
	
	const int getQQ() const { return qqNum; }
	const unsigned int getMyIP() const { return IP; }
	const unsigned short getMyPort() const { return port; }
	const int getLoginTime() const { return loginTime; };
	const unsigned int getServerIP() const { return serverIP; }
	const short getServerPort() const { return serverPort; }
	const unsigned int getLastLoginIP() const { return lastLoginIP; }
	const int getLastLoginTime() const { return lastLoginTime; }
	
protected:
	virtual void parseBody();
private:	
	int qqNum;
	unsigned int IP;
	unsigned short port;
	int loginTime;
	unsigned int serverIP;
	short serverPort;	
	unsigned int lastLoginIP;
	int lastLoginTime;
	
	// only the part below is useful to us
	unsigned char clientKey[32];
	unsigned int redirectedIP;
	short redirectedPort;
		
	unsigned char replyCode;
	
	// note that reply messege is encoded by "GB18030"
	std::string replyMessage;
 
};

class LogoutPacket : public OutPacket {
public:
	LogoutPacket();
	LogoutPacket(const LogoutPacket &rhs);
	virtual ~LogoutPacket() {};
	OutPacket * copy() { return new LogoutPacket(*this);}
	LogoutPacket &operator=(const LogoutPacket &rhs);
	
protected:
	virtual int putBody(unsigned char *buf);
};

class KeepAlivePacket : public OutPacket {    
public:
	KeepAlivePacket( );
	KeepAlivePacket( const KeepAlivePacket &rhs );
	virtual ~KeepAlivePacket( ) {};    
    
	OutPacket * copy() { return new KeepAlivePacket(*this);}
	KeepAlivePacket &operator=(const KeepAlivePacket &rhs );
protected:
	virtual int putBody(unsigned char *buf) ;
};

class KeepAliveReplyPacket: public InPacket 
{
public: 
	KeepAliveReplyPacket() {}
	KeepAliveReplyPacket(unsigned char *buf, int len);
	KeepAliveReplyPacket(const KeepAliveReplyPacket &rhs);
	virtual ~KeepAliveReplyPacket(){};
 
	const int numOnlineUsers() const { return onlineUsers; }
	const unsigned int getMyIP() const { return myIP; }
	const unsigned short getMyPort() const { return myPort; }
	const unsigned int getTime() const { return m_Time; }
	
	KeepAliveReplyPacket &operator=(const KeepAliveReplyPacket &rhs);
protected:
	virtual void parseBody() ;
private:
	static const unsigned char KA_DIVIDER = 0x1f;
	static int onlineUsers;
	unsigned int myIP;
	unsigned short myPort; 
	unsigned int m_Time;
};

#endif
