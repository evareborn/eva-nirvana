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

#include "evalogintoken.h"
#include "evadefines.h"
#include "evacrypt.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

RequestLoginTokenPacket::RequestLoginTokenPacket()
	: OutPacket(QQ_CMD_REQUEST_LOGIN_TOKEN, true)
{
}

RequestLoginTokenPacket::RequestLoginTokenPacket( const RequestLoginTokenPacket & rhs )
	: OutPacket(rhs)
{
}

RequestLoginTokenPacket & RequestLoginTokenPacket::operator =( const RequestLoginTokenPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	return *this;
}

int RequestLoginTokenPacket::putBody( unsigned char * buf )
{
	buf[0]=0x00;
	return 1;
}





/**  *************************************************************************************************************/

RequestLoginTokenReplyPacket::RequestLoginTokenReplyPacket()
	: InPacket(), replyCode(0x01), length(0), 
	token(NULL)
{
}

RequestLoginTokenReplyPacket::RequestLoginTokenReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len), replyCode(0x01), length(0), 
	token(NULL)
{
}

RequestLoginTokenReplyPacket::RequestLoginTokenReplyPacket( const RequestLoginTokenReplyPacket & rhs )
	: InPacket(rhs), replyCode(0x01), length(0), 
	token(NULL)
{
	replyCode = rhs.getReplyCode();
	length = rhs.getTokenLength();
	if(length){
		if(!token)
			token = (unsigned char *)malloc(length * sizeof(unsigned char));
		if(!token){
			length = 0;
			fprintf(stderr, "RequestLoginTokenReplyPacket( const RequestLoginTokenReplyPacket & rhs ): malloc() failed\n");
			return;
		}
		memcpy(token, rhs.getToken(), length);
	}
}

RequestLoginTokenReplyPacket::~RequestLoginTokenReplyPacket()
{
	if(token)
		free(token);
}

RequestLoginTokenReplyPacket & RequestLoginTokenReplyPacket::operator =( const RequestLoginTokenReplyPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	replyCode = rhs.getReplyCode();
	length = rhs.getTokenLength();
	if(length){
		if(!token)
			token = (unsigned char *)malloc(length * sizeof(unsigned char));
		if(!token){
			length = 0;
			fprintf(stderr, "RequestLoginTokenReplyPacket( const RequestLoginTokenReplyPacket & rhs ): malloc() failed\n");
			return *this;
		}
		memcpy(token, rhs.getToken(), length);
	}
	return *this;
}

const bool RequestLoginTokenReplyPacket::isReplyOk( )  const
{
	return replyCode == QQ_REQUEST_LOGIN_TOKEN_REPLY_OK;
}

void RequestLoginTokenReplyPacket::parseBody( )
{
	replyCode = decryptedBuf[0];
	if(replyCode == QQ_REQUEST_LOGIN_TOKEN_REPLY_OK){
		length = decryptedBuf[1];
		if(token)
			free(token);
		token = (unsigned char *)malloc(length * sizeof(unsigned char));		
		if(!token){
			length = 0;
			fprintf(stderr, "RequestLoginTokenReplyPacket( const RequestLoginTokenReplyPacket & rhs ): malloc() failed\n");
			return;
		}
		memcpy(token, decryptedBuf + 2, length);
		setLoginToken(token, length); // we set the loginToken now
	}
}

/**  *************************************************************************************************************/



RequestLoginTokenExPacket::RequestLoginTokenExPacket(const unsigned char type)
	: OutPacket(QQ_CMD_REQUEST_LOGIN_TOKEN_EX, true),
	m_Type( type),
	m_Code( ""),
	m_Token( NULL ),
	m_TokenLen ( 0 )
{
}

RequestLoginTokenExPacket::RequestLoginTokenExPacket( const RequestLoginTokenExPacket & rhs )
	: OutPacket(rhs),
	m_Code( ""),
	m_Token( NULL ),
	m_TokenLen ( 0 )
{
	*this = rhs;
}

RequestLoginTokenExPacket::~RequestLoginTokenExPacket()
{
	if(m_Token)
		delete [] m_Token;
}

RequestLoginTokenExPacket & RequestLoginTokenExPacket::operator =( const RequestLoginTokenExPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	m_Type = rhs.getType();
	m_Code = rhs.getCode();
	setToken( rhs.getToken(), rhs.getTokenLength());
	return *this;
}

void RequestLoginTokenExPacket::setToken(const unsigned char *token, unsigned short len)
{
	if(!(token && (len>0))) return;

	if(m_Token) delete [] m_Token;
	m_TokenLen = len;
	m_Token = new unsigned char[m_TokenLen];
	memcpy(m_Token, token, m_TokenLen);
}

int RequestLoginTokenExPacket::putBody( unsigned char * buf )
{
	int pos = 0;
	buf[pos++] = m_Type;
	*((unsigned short *)(buf+pos)) = htons(0x0005);
	pos+=2;

	memset(buf+pos, 0, 4); pos+=4;
	
	if(m_Type == QQ_LOGIN_TOKEN_VERIFY) {
		if(!m_Token) {
			fprintf(stderr, "RequestLoginTokenExPacket: No token available, error!\n");
			return 0;
		}
		*((unsigned short*)(buf+pos)) = htons(m_Code.length());
		pos+=2;
		memcpy( buf + pos, m_Code.c_str(), m_Code.length());
		pos += m_Code.length();
		*((unsigned short*)(buf+pos)) = htons(m_TokenLen);
		pos+= 2;
		memcpy(buf + pos, m_Token, m_TokenLen);
		pos += m_TokenLen;
	}

	return pos;
}

/** ========================================================================    */


RequestLoginTokenExReplyPacket::RequestLoginTokenExReplyPacket()
	: InPacket(),
	m_Type(QQ_LOGIN_TOKEN_VERIFY),
	m_ReplyCode(QQ_LOGIN_TOKEN_NEED_VERI),
	m_Token(NULL),
	m_Data(NULL),
	m_TokenLen(0L),
	m_DataLen(0L)
{
}

RequestLoginTokenExReplyPacket::RequestLoginTokenExReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len),
	m_Type(QQ_LOGIN_TOKEN_VERIFY),
	m_ReplyCode(QQ_LOGIN_TOKEN_NEED_VERI),
	m_Token(NULL),
	m_Data(NULL),
	m_TokenLen(0L),
	m_DataLen(0L)
{
}

RequestLoginTokenExReplyPacket::RequestLoginTokenExReplyPacket( const RequestLoginTokenExReplyPacket & rhs )
	: InPacket(rhs),
	m_Type(QQ_LOGIN_TOKEN_VERIFY),
	m_ReplyCode(QQ_LOGIN_TOKEN_NEED_VERI),
	m_Token(NULL),
	m_Data(NULL),
	m_TokenLen(0L),
	m_DataLen(0L)
{
	*this = rhs;
}

RequestLoginTokenExReplyPacket::~RequestLoginTokenExReplyPacket()
{
	if(m_Token) delete [] m_Token;
	if(m_Data) delete [] m_Data;
}

RequestLoginTokenExReplyPacket & RequestLoginTokenExReplyPacket::operator =( const RequestLoginTokenExReplyPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	m_Type = rhs.getType();
	m_ReplyCode = rhs.getReplyCode();

	m_TokenLen = rhs.getTokenLength();
	if(m_Token) delete [] m_Token;
	m_Token = new unsigned char [m_TokenLen];
	if(m_TokenLen && rhs.getToken())
		memcpy(m_Token, rhs.getToken(), m_TokenLen);

	m_DataLen = rhs.getDataLength();
	if(m_Data) delete [] m_Data;
	m_Data = new unsigned char [m_DataLen];
	if(m_DataLen > 0 && rhs.getData())
		memcpy(m_Data, rhs.getData(), m_DataLen);
	
	return *this;
}

void RequestLoginTokenExReplyPacket::parseBody( )
{
	int offset = 0;
	m_Type = decryptedBuf[offset++];
	
	offset+=2; // these 2 bytes should be 00 05

	m_ReplyCode = decryptedBuf[offset++];
	if(m_ReplyCode == QQ_LOGIN_TOKEN_OK){
		m_TokenLen = ntohs(*(unsigned short *)(decryptedBuf + offset));
		offset+=2;
		setLoginToken(decryptedBuf+offset, m_TokenLen); // we set the loginToken now
		return;
	}

	if(m_ReplyCode == QQ_LOGIN_TOKEN_NEED_VERI){
		m_TokenLen = ntohs(*(unsigned short *)(decryptedBuf + offset));
		offset+=2;
		if(m_Token) delete [] m_Token;
		m_Token = new unsigned char [m_TokenLen];
		memcpy(m_Token, decryptedBuf + offset, m_TokenLen);
		offset += m_TokenLen;
		
		m_DataLen = ntohs(*(unsigned short *)(decryptedBuf + offset));
		offset+=2;
		if(m_Data) delete [] m_Data;
		m_Data = new unsigned char [m_DataLen];
		memcpy(m_Data, decryptedBuf + offset, m_DataLen);  //PNG image data
	}
}


/** ========================================================================    */


unsigned char ServerDetectorPacket::m_Step = 0x00;
unsigned int ServerDetectorPacket::m_FromIP = 0x00;

ServerDetectorPacket::ServerDetectorPacket( )
	: OutPacket(QQ_CMD_SERVER_DETECT, true)
{
}

ServerDetectorPacket::ServerDetectorPacket( const ServerDetectorPacket & rhs )
	: OutPacket(rhs)
{
	*this = rhs;
}

ServerDetectorPacket::~ ServerDetectorPacket( )
{
}


ServerDetectorPacket & ServerDetectorPacket::operator =( const ServerDetectorPacket & rhs )
{
	m_FromIP = rhs.getFromIP();
	m_Step = rhs.getStep();
	return *this;
}


int ServerDetectorPacket::putBody( unsigned char * buf )
{
	// this should be a random key and used for decrypting the reply packets
	// but, we simply use QQ_Client_Key anyway
	memcpy(buf, QQ_Client_Key, 16);

	unsigned char tmp[15];
	int offset = 0;
	tmp[offset++] = 0x00;
	tmp[offset++] = m_Step; // start at 0, when redirect needed, step increase one
	tmp[offset++] = m_Step?0x01:0x00; // always 0x01, might be the sub counter for step
	*((unsigned int *)(tmp+offset)) = m_Step?htonl(0x00000001):0x00;
	offset+=4;
	*((unsigned int *)(tmp+offset)) = 0x00;
	offset+=4;	
	*((unsigned int *)(tmp+offset)) = htonl(m_FromIP);
	offset+=4;

	for(int i = 0; i< offset; i++){
		if(! (i%8)) printf("\n");
		printf("%2x ", tmp[i]);
	}
	printf("\n");

	int pos =256;
	EvaCrypt::encrypt(tmp, offset, (unsigned char *)QQ_Client_Key, buf + 16, &pos);
	return 16 + pos;
}


/** ========================================================================    */

ServerDetectorReplyPacket::ServerDetectorReplyPacket( )
	: m_ReplyCode ( 0),
	m_IP(0)
{
}

ServerDetectorReplyPacket::ServerDetectorReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len),
	m_ReplyCode ( 0),
	m_IP(0)
{
}

ServerDetectorReplyPacket::ServerDetectorReplyPacket( const ServerDetectorReplyPacket & rhs )
	: InPacket(rhs)
{
	*this = rhs;
}

ServerDetectorReplyPacket & ServerDetectorReplyPacket::operator =( const ServerDetectorReplyPacket & rhs )
{
	m_ReplyCode = rhs.getReplyCode();
	m_IP = rhs.getRedirectIP();
	return *this;
}

void ServerDetectorReplyPacket::parseBody( )
{

	for(int i = 0; i< bodyLength; i++){
		if(! (i%8)) printf("\n");
		printf("%2x ", decryptedBuf[i]);
	}
	printf("\n");

	int offset = 0;
	m_ReplyCode = ntohs( *((unsigned short *)(decryptedBuf)) ); offset+=2;
	if(m_ReplyCode == QQ_CMD_SERVER_DETECT_REPLY_OK) 
		return;

	// we need redirect to some server here
	offset+=9; // unknown 9 bytes( 1 + 8)
	m_IP = ntohl( *((unsigned int *)(decryptedBuf + offset)) );
}

