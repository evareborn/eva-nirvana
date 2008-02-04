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
#include "evalogin.h"
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "evacrypt.h"
#include "evautil.h"

LoginPacket::LoginPacket()
	: OutPacket(),
	m_LoginMode(QQ_LOGIN_MODE_NORMAL),
	m_NumProcess(1),
	m_UUID(NULL)
{
}

LoginPacket::LoginPacket(unsigned char loginMode)
	: OutPacket(QQ_CMD_LOGIN, true),
	m_LoginMode(loginMode),
	m_NumProcess(1),
	m_UUID(NULL)
{
}

LoginPacket::LoginPacket(const LoginPacket &rhs)
	:OutPacket(rhs)
{
	*this = rhs;
}

LoginPacket::~LoginPacket()
{
}

LoginPacket &LoginPacket::operator=(const LoginPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	m_LoginMode = rhs.getLoginMode();
	m_NumProcess = rhs.getNumProcess();
	m_UUID = rhs.getComputerUUID();
        return *this;
}

int LoginPacket::putBody(unsigned char *buf) 
{
	memcpy(buf, iniKey, QQ_KEY_LENGTH);

	int pos = 16;
	
	if(!getPasswordKey()){
		fprintf(stderr, "LoginPacket->putBody: no password key avalible\n");
		return 0;
	}
	
	unsigned char *login = new unsigned char[QQ_LOGIN_DATA_LENGTH];
	EvaCrypt::encrypt(NULL, 0, getPasswordKey(), login, &pos);

	memset(login + pos, 0, QQ_LOGIN_RESERVED_LENGTH);
	pos += QQ_LOGIN_RESERVED_LENGTH;

	memcpy(login+pos, QQ_Hash, 16); // md5 hash for qq.exe
	pos += 16;

	// a little trick for the number of processes of qq.exe
	unsigned char *tmp = new unsigned char[32];
	memcpy(tmp, login, 16);
	memcpy(tmp+16, QQ_Hash, 16);
	char no_process = m_NumProcess;
	for(int i=0; i<32;)
		no_process ^= tmp[i++];

	delete [] tmp;
	login[pos++] = no_process;

	login[pos++] = m_LoginMode;

	*((unsigned int *)(login+pos)) = 0; pos += 4;   /// unknown 0s, 4 bytes
	*((unsigned short *)(login+pos)) = htons(0x01); pos += 2;   /// unknown 0x0001, 2 bytes
	*((unsigned int *)(login+pos)) = htonl(0x01); pos += 4;   /// unknown 0x00 00 00 01, 4 bytes	
	memset(login + pos, 0, 14); pos += 14;  /// 14 bytes 0x00s
	
	memcpy(login+pos, m_UUID, 16);
	pos+=16;

	*((unsigned short *)(login + pos)) = htons(getLoginTokenLength()); pos+=2;
	memcpy(login+pos, getLoginToken(), getLoginTokenLength());
	pos += getLoginTokenLength();

	*((unsigned short *)(login + pos)) = htons(QQ_LOGIN_APPEND_LENGTH);
	pos+=2;

	memcpy(login+pos, login_appended_2_72, 71);
	pos+=71;


	
	memset(login+pos, 0, QQ_LOGIN_DATA_LENGTH - pos);
// 				printf("login data:\n");
// 				for(int i=0; i<QQ_LOGIN_DATA_LENGTH; i++){
// 					if(!(i%8)) printf("\n%d: ",i);
// 					char t = login[i];
// 					printf("%2x ", (unsigned char)t);
// 				}
// 				printf("\n");	

	unsigned char *encrypted = new unsigned char[MAX_PACKET_SIZE];
	int enLen=200;
	EvaCrypt::encrypt(login, QQ_LOGIN_DATA_LENGTH, iniKey, encrypted, &enLen);
	
	memcpy(buf+QQ_KEY_LENGTH, encrypted, enLen);
	
	delete login;
	delete encrypted;
	return QQ_KEY_LENGTH + enLen;
}

/* =========================================================== */

LoginReplyPacket::LoginReplyPacket(unsigned char *buf, int len)
	: InPacket(buf, len)
{ 
}   

LoginReplyPacket::LoginReplyPacket(const LoginReplyPacket &rhs)
	: InPacket(rhs)
{ 	
	replyCode = rhs.getReplyCode();
	redirectedIP = rhs.getRedirectedIP();
	redirectedPort = rhs.getRedirectedPort();
	
	qqNum = rhs.getQQ();
	IP = rhs.getMyIP();
	port = rhs.getMyPort();
	loginTime = rhs.getLoginTime();
	serverIP = rhs.getServerIP();
	serverPort = rhs.getServerPort();
	lastLoginIP = rhs.getLastLoginIP();
	lastLoginTime = rhs.getLastLoginTime();
	replyMessage = rhs.getReplyMessage();
} 

LoginReplyPacket &LoginReplyPacket::operator=(const LoginReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	replyCode = rhs.getReplyCode();
	redirectedIP = rhs.getRedirectedIP();
	redirectedPort = rhs.getRedirectedPort();
	
	qqNum = rhs.getQQ();
	IP = rhs.getMyIP();
	port = rhs.getMyPort();
	loginTime = rhs.getLoginTime();
	serverIP = rhs.getServerIP();
	serverPort = rhs.getServerPort();
	lastLoginIP = rhs.getLastLoginIP();
	lastLoginTime = rhs.getLastLoginTime();
	replyMessage = rhs.getReplyMessage();
        return *this;
}

void LoginReplyPacket::parseBody() 
{
	replyCode = decryptedBuf[0];
	switch(replyCode) {
	case QQ_LOGIN_REPLY_OK:
		{
			// length of 16 bytes: 001-016, session key
			setSessionKey(decryptedBuf+1);

			// 4 bytes: 017-020,  user id(QQ number)
			qqNum = ntohl( *((int *)(decryptedBuf + QQ_KEY_LENGTH + 1) ) );

			// now we can set the file session key
			char *fsbuf = new char[QQ_KEY_LENGTH + 4];
			// copy the big endian qqNum directly
			memcpy(fsbuf, decryptedBuf + QQ_KEY_LENGTH + 1 , 4); // qqNum in network bytes order
			// then, the session key
			memcpy(fsbuf+4, getSessionKey(), QQ_KEY_LENGTH);
			// save it as the file session key
			setFileSessionKey((unsigned char *)EvaUtil::doMd5(fsbuf, 4 + QQ_KEY_LENGTH));
			delete []fsbuf;

			// 021-024  user IP
			IP = ntohl(* ((int *)(decryptedBuf + 21)) );

			// 025-026 user port
			port = ntohs(* ((short *)(decryptedBuf + 25)) );

			// 027-030 server IP ( always 127.0.0.1 in QQ2006 standard)
			serverIP = ntohl(* ((int *)(decryptedBuf + 27)) );

			// 031-032 server port ( should be 0x1f40(8000) )
			serverPort = ntohs(* ((short *)(decryptedBuf + 31)) );

			// 033-036  login time
			loginTime = ntohl(* ((int *)(decryptedBuf + 33)) );

			/// 037-038 2 unknown bytes
			
			// 039-062 file share token used in Qun disk share file access
			setFileShareToken(decryptedBuf+39);
			
			/// 063 -> 082: unknown bytes

			// 83-114  client key for QQ Home
			setClientKey(decryptedBuf+83, 32); // always 32 bytes long

			/// 115 - 118, unknown 4 bytes ( 0x00 00 00 01)
			/// 119 - 126, unknown 8 bytes, all 0x0s
			
			// 127-130  user IP when last login
			lastLoginIP = ntohl(* ((int *)(decryptedBuf + 127)) );

			// 131-134  last login time, not sure
			lastLoginTime = ntohl(* ((int *)(decryptedBuf + 131)) );
			
			// 135 - 138 might be IP, 4 bytes
			// 139 - 142 might be IP, 4 bytes
			// 143 -183 unknow bytes
			break;
		}
	case QQ_LOGIN_REPLY_REDIRECT:
		// redirect user to another server for balance reason
		{
			// 001-004 user qq number
			qqNum = ntohl(* ((int *)(decryptedBuf + 1)) );
			// 005-008  the new server IP address
			redirectedIP = ntohl(* ((int *)(decryptedBuf + 5)) );
			// 009-010  the new server's Port number
			redirectedPort = ntohs(* ((short *)(decryptedBuf + 9)) );
			break;
		}
	case QQ_LOGIN_REPLY_PWD_ERROR:
	case QQ_LOGIN_REPLY_NEED_REACTIVATE:
		// message from server when password is wrong. the messege is encoded by "GB18030"
		{
			char *tmpMsg = new char[bodyLength];
			memcpy(tmpMsg, decryptedBuf+1, bodyLength-1);
			tmpMsg[bodyLength-1]=0; 
			replyMessage.assign(tmpMsg);
			delete []tmpMsg;
			break;
		}
	case QQ_LOGIN_REPLY_REDIRECT_EX:
		// redirect user to another server for balance reason
		{
			// 001-004 user qq number
			qqNum = ntohl(* ((int *)(decryptedBuf + 1)) );
			/// 5 - 14, 10 unknown bytes
			// 005-008  the new server IP address
			redirectedIP = ntohl(* ((int *)(decryptedBuf + 15)) );
			redirectedPort = -1;
			printf("login reply(first byte is reply code)\n");
			for(int i=0; i<bodyLength; i++){
				if(!(i%8)) printf("\n%d: ",i);
				char t = decryptedBuf[i];
				printf("%2x ", (uint8_t)t);
			}
			printf("\n");
			break;
		}
	case QQ_LOGIN_REPLY_PWD_ERROR_EX:
		{
			char *tmpMsg = new char[bodyLength];
			memcpy(tmpMsg, decryptedBuf+1, bodyLength-1);
			tmpMsg[bodyLength-1]=0; 
			replyMessage.assign(tmpMsg);
			delete []tmpMsg;
		}
	default:
		replyCode = QQ_LOGIN_REPLY_MISC_ERROR;
		{
			printf("login unknown reply(first byte is reply code)\n");
			for(int i=0; i<bodyLength; i++){
				if(!(i%8)) printf("\n%d: ",i);
				char t = decryptedBuf[i];
				printf("%2x ", (uint8_t)t);
			}
			printf("\n");
		}
		break;
	}
}

/* =========================================================== */

LogoutPacket::LogoutPacket()
	: OutPacket(QQ_CMD_LOGOUT, false)
{
 	sequence = 0xFFFF;
}

LogoutPacket::LogoutPacket(const LogoutPacket &rhs)
	: OutPacket(rhs)
{
}

LogoutPacket &LogoutPacket::operator=(const LogoutPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
        return *this;
}

int LogoutPacket::putBody(unsigned char *buf) 
{    
	unsigned char *pwKey = Packet::getPasswordKey();
	if(pwKey==NULL){
		fprintf(stderr, "LogoutPacket->putBody: no password key avalible\n");
		return 0;
	}
	memcpy(buf, pwKey, QQ_KEY_LENGTH);
	return QQ_KEY_LENGTH;
}

/* =========================================================== */

KeepAlivePacket::KeepAlivePacket()
	: OutPacket(QQ_CMD_KEEP_ALIVE, true) 
{
}

KeepAlivePacket::KeepAlivePacket(const KeepAlivePacket &rhs)
	: OutPacket(rhs) 
{
}

KeepAlivePacket &KeepAlivePacket::operator=(const KeepAlivePacket &rhs )
{
	*((OutPacket *)this) = (OutPacket)rhs;
        return *this;
}

int KeepAlivePacket::putBody(unsigned char *buf) 
{
	char tmp[20];
	sprintf(tmp, "%d", getQQ());
	memcpy(buf, tmp, strlen(tmp));
        return 4; 
}

/* =========================================================== */

int KeepAliveReplyPacket::onlineUsers=0;

KeepAliveReplyPacket::KeepAliveReplyPacket(unsigned char *buf, int len) 
	: InPacket(buf, len) 
{
}   

KeepAliveReplyPacket::KeepAliveReplyPacket(const KeepAliveReplyPacket &rhs)
	: InPacket(rhs) 
{
	onlineUsers = rhs.numOnlineUsers();
	myIP = rhs.getMyIP();
	myPort = rhs.getMyPort();
	m_Time = rhs.getTime();
}

KeepAliveReplyPacket &KeepAliveReplyPacket::operator=(const KeepAliveReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	onlineUsers = rhs.numOnlineUsers();
	myIP = rhs.getMyIP();
	myPort = rhs.getMyPort();
	m_Time = rhs.getTime();
        return *this;	
}
 
void KeepAliveReplyPacket::parseBody()
{
	int offset = 0;
	offset++; // first byte: 0x00 
	onlineUsers = ntohl(*((unsigned int *)(decryptedBuf + offset))); offset+=4;
	myIP = ntohl(*((unsigned int *)(decryptedBuf + offset)));offset+=4;
	myPort = ntohl(*((unsigned short *)(decryptedBuf + offset)));offset+=2;
	offset+=2; // 2 bytes unknown
	m_Time = ntohl(*((unsigned int *)(decryptedBuf + offset)));offset+=4;
	// 5 bytes unknown left: 0x00 00 0
}


