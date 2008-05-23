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
#include "evaaddfriend.h"
#include "evadefines.h"
#include "evautil.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring>

AddFriendPacket::AddFriendPacket( )
	: OutPacket(),
	qqNum(0)
{
}

AddFriendPacket::AddFriendPacket( const int id )
	: OutPacket(QQ_CMD_ADD_FRIEND, true),
	qqNum(id)
{
}

AddFriendPacket::AddFriendPacket(const AddFriendPacket &rhs)
	: OutPacket(rhs)
{
	qqNum = rhs.getAddQQ();
}


AddFriendPacket & AddFriendPacket::operator =( const AddFriendPacket & rhs )
{
	*( (OutPacket *)this) = (OutPacket)rhs;
	qqNum = rhs.getAddQQ();
	return *this;
}

int AddFriendPacket::putBody( unsigned char * buf )
{
	if(qqNum==0) return 0;
	char strID[20];
	sprintf(strID,"%d", qqNum);
	int len = strlen(strID);
	memcpy(buf, strID, len);
	return len;
}

/* =========================================================== */

AddFriendReplyPacket::AddFriendReplyPacket(unsigned char* buf, int len)
	: InPacket(buf, len)
{
}

AddFriendReplyPacket::AddFriendReplyPacket( const AddFriendReplyPacket &rhs)
	: InPacket(rhs)
{
	replyCode = rhs.getReplyCode();
	myQQ = rhs.getMyQQ();
}

AddFriendReplyPacket &AddFriendReplyPacket::operator=(const AddFriendReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	replyCode = rhs.getReplyCode();
	myQQ = rhs.getMyQQ();
	return *this;
}

void AddFriendReplyPacket::parseBody()
{
	int index=0;			
	while(decryptedBuf[index++] != 0x1f);
#ifdef WIN32
	char* qqstr; qqstr=(char*)_alloca(index+1);
#else
	char qqstr[index+1];
#endif
	memcpy(qqstr, decryptedBuf, index);
	qqstr[index]=0x00;		
	myQQ = atoi(qqstr);
	replyCode = decryptedBuf[index] - 0x30;
}

/* =========================================================== */

AddFriendAuthPacket::AddFriendAuthPacket( )
	: OutPacket( QQ_CMD_ADD_FRIEND_AUTH, true),
	message("")
{
}

AddFriendAuthPacket::AddFriendAuthPacket( const int id , const uint8_t type )
	: OutPacket( QQ_CMD_ADD_FRIEND_AUTH, true),
	type(type),
	buddyQQNum(id),
	message("")
{
}

AddFriendAuthPacket::AddFriendAuthPacket( AddFriendAuthPacket & rhs )
	: OutPacket(rhs)
{
	type = rhs.getType();
	buddyQQNum = rhs.getBuddyQQ();
	message = rhs.getMessage();
}

AddFriendAuthPacket & AddFriendAuthPacket::operator =( const AddFriendAuthPacket & rhs )
{
	*((OutPacket *)this) = (OutPacket)rhs;
	type = rhs.getType();
	buddyQQNum = rhs.getBuddyQQ();
	message = rhs.getMessage();
	return *this;
}

int AddFriendAuthPacket::putBody( unsigned char * buf )
{	
	char qq[20];
	sprintf(qq,"%d", buddyQQNum);
	int offset = strlen(qq);
	memcpy(buf, qq, offset);
	buf[offset++] = DIVIDER;
	buf[offset++] = type;
	buf[offset++] = DIVIDER;
	memcpy(buf + offset, message.c_str(), message.length());	
	return offset + message.length();
}

/* =========================================================== */

AddFriendAuthReplyPacket::AddFriendAuthReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len)
{
}

AddFriendAuthReplyPacket::AddFriendAuthReplyPacket( const AddFriendAuthReplyPacket & rhs )
	: InPacket(rhs)
{
}

AddFriendAuthReplyPacket & AddFriendAuthReplyPacket::operator =( const AddFriendAuthReplyPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	replyCode = rhs.getReplyCode();
	return *this;
}

const bool AddFriendAuthReplyPacket::isSentOK( ) const
{
	return replyCode == QQ_ADD_FRIEND_AUTH_REPLY_OK;
}

void AddFriendAuthReplyPacket::parseBody( )
{
	replyCode = decryptedBuf[0];
}

/* =========================================================== */

DeleteFriendPacket::DeleteFriendPacket( )
	: OutPacket(QQ_CMD_DELETE_FRIEND, true),
	buddyQQNum(0)
{
}

DeleteFriendPacket::DeleteFriendPacket( const int id )
	: OutPacket(QQ_CMD_DELETE_FRIEND, true),
	buddyQQNum(id)
{
}

DeleteFriendPacket::DeleteFriendPacket( const DeleteFriendPacket & rhs )
	: OutPacket(rhs)
{
	buddyQQNum = rhs.getBuddyQQ();
}

DeleteFriendPacket & DeleteFriendPacket::operator =( const DeleteFriendPacket & rhs )
{
	*((OutPacket *)this) = (OutPacket)rhs;
	buddyQQNum = rhs.getBuddyQQ();
	return *this;
}

int DeleteFriendPacket::putBody( unsigned char * buf )
{
	if(buddyQQNum==0) return 0;
	char qq[20];
	sprintf(qq,"%d", buddyQQNum);
	memcpy(buf, qq, strlen(qq));
	return strlen(qq);
}

/* =========================================================== */

DeleteFriendReplyPacket::DeleteFriendReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len)
{
}

DeleteFriendReplyPacket::DeleteFriendReplyPacket( const DeleteFriendReplyPacket & rhs )
	: InPacket(rhs)
{
	replyCode = rhs.getReplyCode();
}

DeleteFriendReplyPacket & DeleteFriendReplyPacket::operator =( const DeleteFriendReplyPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	replyCode = rhs.getReplyCode();
	return *this;
}

const bool DeleteFriendReplyPacket::isDeleted( ) const
{
	return replyCode == QQ_DELETE_FRIEND_REPLY_OK;
}

void DeleteFriendReplyPacket::parseBody( )
{
	replyCode = decryptedBuf[0];
}

/* =========================================================== */


DeleteMePacket::DeleteMePacket( )
	: OutPacket(QQ_CMD_DELETE_ME, true),
	buddyQQNum(0)
{
}

DeleteMePacket::DeleteMePacket( const int id )
	: OutPacket(QQ_CMD_DELETE_ME, true),
	buddyQQNum(id)
{
}

DeleteMePacket::DeleteMePacket( const DeleteMePacket & rhs )
	: OutPacket(rhs)
{
	buddyQQNum = rhs.getBuddyQQ();
}

DeleteMePacket & DeleteMePacket::operator =( const DeleteMePacket & rhs )
{
	*((OutPacket *)this) = (OutPacket)rhs;
	buddyQQNum = rhs.getBuddyQQ();
	return *this;
}

int DeleteMePacket::putBody( unsigned char * buf )
{
	if(buddyQQNum==0) return 0;
	int qq = htonl(buddyQQNum);
	memcpy(buf, &qq, 4);
	return 4;
}


/* =========================================================== */

DeleteMeReplyPacket::DeleteMeReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len)
{
}

DeleteMeReplyPacket::DeleteMeReplyPacket( const DeleteMeReplyPacket & rhs )
	: InPacket(rhs)
{
	replyCode = rhs.getReplyCode();
}

DeleteMeReplyPacket & DeleteMeReplyPacket::operator =( const DeleteMeReplyPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	replyCode = rhs.getReplyCode();
	return *this;
}

const bool DeleteMeReplyPacket::isDeleted( ) const
{
	return replyCode == QQ_DELETE_ME_REPLY_OK;
}

void DeleteMeReplyPacket::parseBody( )
{
	replyCode = decryptedBuf[0];
}


/* =========================================================== */

SystemNotificationPacket::SystemNotificationPacket( )
	: InPacket(),
	m_AllowReverse(false),
	m_Code(0),
	m_CodeLen(0)
{
}

SystemNotificationPacket::SystemNotificationPacket( unsigned char * buf, int len )
	: InPacket(buf, len),
	m_AllowReverse(false),
	m_Code(0),
	m_CodeLen(0)
{
}

SystemNotificationPacket::SystemNotificationPacket( const SystemNotificationPacket & rhs )
	: InPacket(rhs),
	m_AllowReverse(false),
	m_Code(0),
	m_CodeLen(0)
{
	*this = rhs;
}

SystemNotificationPacket::~SystemNotificationPacket( )
{
	if(m_Code)
		delete [] m_Code;
}

SystemNotificationPacket & SystemNotificationPacket::operator =( const SystemNotificationPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	type = rhs.getType();
	myQQ = rhs.getMyQQ();
	fromQQ = rhs.getFromQQ();
	message = rhs.getMessage();
	setCode(rhs.getCode(), rhs.getCodeLength());
	m_AllowReverse = rhs.allowReverse();
	return *this;	
}

void SystemNotificationPacket::parseBody( )
{
	printf("\nSystemNotificationPacket\n");
	for(int i=0; i< bodyLength; i++){
		if(!(i%8)) printf("\n");
		printf("%2x ", decryptedBuf[i]);
	}
	printf("\n===\n");
	int start=0 , offset=0;
#ifdef WIN32
	char* buf; buf=(char*)_alloca(bodyLength);
#else
	char buf[bodyLength];
#endif
	
	while(decryptedBuf[offset]!=DIVIDER) offset++;	
	memcpy(buf, decryptedBuf + start, offset-start);		
	buf[offset-start]=0x00;
	start = ++offset;
	type = (uint8_t)atoi(buf);		
	
	
	while(decryptedBuf[offset]!=DIVIDER) offset++;	
	memcpy(buf, decryptedBuf +start, offset-start);		
	buf[offset-start]=0x00;
	start = ++offset;
	fromQQ = atoi(buf);
	
	
	while(decryptedBuf[offset]!=DIVIDER) offset++;	
	memcpy(buf, decryptedBuf+start, offset-start);
	buf[offset-start]=0x00;
	start = ++offset;
	myQQ = atoi(buf);

	if(bodyLength > offset ) {
		while(decryptedBuf[offset]!=DIVIDER && (offset <= bodyLength) ) offset++;
	}

	switch(type){
		case QQ_MSG_SYS_ADD_FRIEND_APPROVED:{
			/// should be  0x30 then the last part diveded by 0x1f
			message = "";
			start += 2; /// ignore 0x30 and 0x1f
			m_CodeLen = ntohs(*((unsigned short *)(decryptedBuf + start)));
			start += 2;
			setCode(decryptedBuf + start, m_CodeLen);
			start += m_CodeLen;
			}
			break;
		case QQ_MSG_SYS_ADD_FRIEND_REJECTED:{
			int len = offset - start;
			memcpy(buf, decryptedBuf+start, len);
			buf[len]=0x00;
			message = buf;
			start += len;
			start++; /// ignore the last 0x1f
			m_CodeLen = ntohs(*((unsigned short *)(decryptedBuf + start)));
			start += 2;
			setCode(decryptedBuf + start, m_CodeLen);
			start += m_CodeLen;
			}
			break;
		case QQ_MSG_SYS_ADD_FRIEND_REQUEST_EX:{
			int len = decryptedBuf[start++] & 0xff;
			memcpy(buf, decryptedBuf+start, len);
			buf[len]=0x00;printf("buf: %s\n", buf);
			message = buf;
			start += len;

			m_AllowReverse = (decryptedBuf[start++] == QQ_MSG_SYS_ADDING_ALLOW_REVERSE);
			printf("start : %d\n", start);
			m_CodeLen = ntohs(*((unsigned short *)(decryptedBuf + start)));
			start+=2;
			setCode(decryptedBuf + start, m_CodeLen);
			start += m_CodeLen;
			}
			break;
		case QQ_MSG_SYS_ADD_FRIEND_APPROVED_AND_ADD:{
			int len = decryptedBuf[start++] & 0xff;
			memcpy(buf, decryptedBuf+start, len);
			buf[len]=0x00;
			message = buf; // actually, no message here, only a 0x00 here
			start += len;

			m_CodeLen = ntohs(*((unsigned short *)(decryptedBuf + start)));
			start += 2;
			setCode(decryptedBuf + start, m_CodeLen);
			start += m_CodeLen;
			}
			break;
		case QQ_MSG_SYS_ADDED_BY_CORRECT_ANSWER:
		case QQ_MSG_SYS_BEING_ADDED_EX:{
			m_AllowReverse = (decryptedBuf[start++] == QQ_MSG_SYS_ADDING_ALLOW_REVERSE);
			m_CodeLen = ntohs(*((unsigned short *)(decryptedBuf + start)));
			start+=2;
			setCode(decryptedBuf + start, m_CodeLen);
			start += m_CodeLen;
			}
			break;
		default:{
			int len = offset - start;
			memcpy(buf, decryptedBuf+start, len);
			buf[len]=0x00;
			message = buf;
			start += len;
			}
			break;
	}
}

void SystemNotificationPacket::setCode( const unsigned char * code, const unsigned int len )
{
	if(!len || !code) return;
	if(m_Code ) delete [] m_Code;
	m_CodeLen = len;
	m_Code = new unsigned char [m_CodeLen];
	memcpy(m_Code, code, m_CodeLen);
}




/* =========================================================== */


VerifyAddingMsgPacket::VerifyAddingMsgPacket( )
	: OutPacket(QQ_CMD_VERIFY_ADDING_MSG, true),
	m_BuddyQQNum(0),
	m_Code(0),
	m_CodeLen(0)
{
}

VerifyAddingMsgPacket::VerifyAddingMsgPacket( const unsigned int id )
	: OutPacket(QQ_CMD_VERIFY_ADDING_MSG, true),
	m_BuddyQQNum(id),
	m_Code(0),
	m_CodeLen(0)
{
}

VerifyAddingMsgPacket::VerifyAddingMsgPacket( const VerifyAddingMsgPacket & rhs )
	: OutPacket(rhs)
{
	*this = rhs;
}

VerifyAddingMsgPacket::~ VerifyAddingMsgPacket( )
{
	if(m_Code)
		delete [] m_Code;
}

VerifyAddingMsgPacket & VerifyAddingMsgPacket::operator =( const VerifyAddingMsgPacket & rhs )
{
	*((OutPacket *)this) = (OutPacket)rhs;
	m_BuddyQQNum = rhs.getBuddyQQ();
	setCode(rhs.getCode(), rhs.getCodeLength());
	return *this;
}

void VerifyAddingMsgPacket::setCode( const unsigned char * code, const unsigned int len )
{
	if(!len || !code) return;
	if(m_Code ) delete [] m_Code;
	m_CodeLen = len;
	m_Code = new unsigned char [m_CodeLen];
	memcpy(m_Code, code, len);
}

int VerifyAddingMsgPacket::putBody( unsigned char * buf )
{
	int pos=0;
	buf[pos++] = 0x03; /// might be sub-command
	buf[pos++] = 0x01; /// might be sub of subcommand,

	
	//*((unsigned int *)(buf+pos)) = htonl(m_BuddyQQNum);
	WRITE32(buf+pos, m_BuddyQQNum);
	pos+=4;
	
	if(!m_CodeLen || !m_Code){
		fprintf(stderr, "VerifyAddingMsgPacket: no verification code set!\n");
		return 0;
	}

	WRITE16(buf+pos, m_CodeLen);
	pos+=2;

	memcpy(buf+pos, m_Code, m_CodeLen);
	pos += m_CodeLen;

	return pos;
	
}



/* =========================================================== */


VerifyAddingMsgReplyPacket::VerifyAddingMsgReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len),
	m_BuddyQQNum(0),
	m_Flag1(0),
	m_Flag2(0)
{
}

VerifyAddingMsgReplyPacket::VerifyAddingMsgReplyPacket( const VerifyAddingMsgReplyPacket & rhs )
	: InPacket(rhs),
	m_BuddyQQNum(0),
	m_Flag1(0),
	m_Flag2(0)
{
	*this = rhs;
}

VerifyAddingMsgReplyPacket & VerifyAddingMsgReplyPacket::operator =( const VerifyAddingMsgReplyPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	m_BuddyQQNum = rhs.getBuddyQQ();
	m_Flag1 = rhs.getFlag1();
	m_Flag2 = rhs.getFlag2();
	m_ReplyCode = rhs.getReplyCode();
	return *this;
}

void VerifyAddingMsgReplyPacket::parseBody( )
{
	int offset=0; /// ignore 0x03, 0x01
	offset++; // 0x03
	m_ReplyCode = decryptedBuf[offset++]; // reply code: 0x00 failed, 0x01 success

	if(m_ReplyCode == 0x00) return;
	
	m_BuddyQQNum = READ32(decryptedBuf + offset);
	offset += 4;
	m_Flag1 = decryptedBuf[offset++];
	m_Flag2 = decryptedBuf[offset++];
}

