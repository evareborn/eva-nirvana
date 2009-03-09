/***************************************************************************
 *   Copyright (C) 2005 by casper                                          *
 *   tlmcasper@163.com                                                     *
 *                                                                         *
 *   Updated by yunfan, 15th, March, 2007                                  *
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
 #include "evaaddfriendex.h"
 #include "evautil.h"
 #include "evadefines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring> 

EvaAddFriendExPacket::EvaAddFriendExPacket()
 	:OutPacket(),
	m_AddQQNum(0)
{
}

EvaAddFriendExPacket::EvaAddFriendExPacket(const unsigned int id)
 	:OutPacket(QQ_CMD_ADD_FRIEND_EX, true),
	m_AddQQNum(id)
{
}

EvaAddFriendExPacket::EvaAddFriendExPacket(const EvaAddFriendExPacket &rhs)
 	:OutPacket(rhs)
{
	m_AddQQNum = rhs.getAddQQ();
}

EvaAddFriendExPacket &EvaAddFriendExPacket::operator=(const EvaAddFriendExPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	m_AddQQNum = rhs.getAddQQ();
	return *this;
}

int EvaAddFriendExPacket::putBody(unsigned char* buf)
{
	if(m_AddQQNum==0) return 0;
	int pos = 0;
	pos += EvaUtil::write32(buf+pos, m_AddQQNum);//qq number of the added friend
	return pos;
}

/**  ======================================================= */

EvaAddFriendExReplyPacket::EvaAddFriendExReplyPacket(unsigned char* buf, int len)
 	:InPacket(buf, len)
{
}

EvaAddFriendExReplyPacket::EvaAddFriendExReplyPacket(const EvaAddFriendExReplyPacket &rhs)
 	:InPacket(rhs)
{
	m_AddQQNum = rhs.getAddQQ();
	m_AuthStatus = rhs.getAuthStatus();
	m_ReplyCode = rhs.getReplyCode();
	
}

EvaAddFriendExReplyPacket &EvaAddFriendExReplyPacket::operator=( const EvaAddFriendExReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	m_AddQQNum = rhs.getAddQQ();
	m_AuthStatus = rhs.getAuthStatus();
	m_ReplyCode = rhs.getReplyCode();
	return *this;
}

void EvaAddFriendExReplyPacket::parseBody()
{
	int pos = 0;
	m_AddQQNum= EvaUtil::read32(decryptedBuf+pos);
	pos += 4;
	m_ReplyCode = decryptedBuf[pos++];
	switch(m_ReplyCode){
	case QQ_ADD_FRIEND_EX_ADDING_POSSIBLE://the user can be added to your buddy list
		m_AuthStatus = decryptedBuf[pos];
		break;
	case QQ_ADD_FRIEND_EX_ALREADY_IN_LIST://the user is already in your buddy list
		break;
	default:
		break;
	}
	
}

/**  ======================================================= */

EvaAddFriendAuthExPacket::EvaAddFriendAuthExPacket()
	:OutPacket(QQ_CMD_ADD_FRIEND_AUTH_EX, true),
	m_AuthStatus(0x02),
	m_DestGroup(0),
	m_AllowAddReverse(true),
	m_AddQQNum(0),
	m_AuthCodeLen(0),
	m_AuthCode(0),
	m_QuestionCodeLen(0),
	m_QuestionCode(0),
	m_Message("")
{
}

EvaAddFriendAuthExPacket::EvaAddFriendAuthExPacket(const unsigned int id, const unsigned char authStatus)
	:OutPacket(QQ_CMD_ADD_FRIEND_AUTH_EX, true),
	m_AuthStatus(authStatus),
	m_DestGroup(0),
	m_AllowAddReverse(true),
	m_AddQQNum(id),
	m_AuthCodeLen(0),
	m_AuthCode(0),
	m_QuestionCodeLen(0),
	m_QuestionCode(0),
	m_Message("")
{
}

EvaAddFriendAuthExPacket::EvaAddFriendAuthExPacket( EvaAddFriendAuthExPacket& rhs)
	:OutPacket(rhs)
{
	*this = rhs;
}

EvaAddFriendAuthExPacket::~EvaAddFriendAuthExPacket()
{
	if(m_AuthCodeLen && m_AuthCode){
		delete []m_AuthCode;
		m_AuthCodeLen = 0;
	}
	if(m_QuestionCodeLen && m_QuestionCode){
		delete []m_QuestionCode;
		m_QuestionCodeLen = 0;
	}	
}

EvaAddFriendAuthExPacket &EvaAddFriendAuthExPacket::operator=(const EvaAddFriendAuthExPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	m_AddQQNum = rhs.getAddQQ();
	m_AuthStatus = rhs.getAuthStatus();
	m_DestGroup = rhs.getDestGroup();
	m_AllowAddReverse = rhs.getAllowAddReverse();
	setAuthCode(rhs.getAuthCode(), rhs.getAuthCodeLength());
	setQuestionCode( rhs.getQuestionCode(), rhs.getQuestionCodeLength());
	return *this;
}

void EvaAddFriendAuthExPacket::setAuthCode( const unsigned char *code, const unsigned short len)
{
	if(!code) return;

	m_AuthCodeLen = len;
	if(m_AuthCodeLen && code){
		if(m_AuthCode) delete [] m_AuthCode;
		m_AuthCode = new unsigned char[m_AuthCodeLen];
		memcpy(m_AuthCode, code, m_AuthCodeLen);
	}
}

void EvaAddFriendAuthExPacket::setQuestionCode( const unsigned char *code, const unsigned short len)
{
	if(!code) return;

	m_QuestionCodeLen = len;
	if(m_QuestionCodeLen && code){
		if(m_QuestionCode) delete [] m_QuestionCode;
		m_QuestionCode = new unsigned char[m_QuestionCodeLen];
		memcpy(m_QuestionCode, code, m_QuestionCodeLen);
	}
}

int EvaAddFriendAuthExPacket::putBody( unsigned char * buf )
{
	int pos = 0;
	int len = 0;
	buf[pos++] = m_AuthStatus;

	pos += EvaUtil::write32(buf+pos, m_AddQQNum);

	// 2 bytes unknown, 0x0000
	memset(buf + pos, 0, 2);
	pos += 2;

	if(m_AuthStatus == ADDING_AUTH_TYPE_ACCEPT)
		return pos;

	if(m_AuthStatus == ADDING_AUTH_TYPE_ACCEPT_ADD){
		buf[pos++] = 0;
		return pos;
	}

	if( m_AuthStatus == ADDING_AUTH_TYPE_REJECT){
		len = m_Message.length();
		buf[pos++] = len & 0xff;
		if(len){
			memcpy(buf+pos, m_Message.c_str(), len);
			pos += len;
		}
		return pos;
	}
	// 2 bytes, length of veri code
	*( (unsigned short *)(buf+pos)) = htons(m_AuthCodeLen);
	pos+=2;

	// veri code here
	memcpy(buf+pos, m_AuthCode, m_AuthCodeLen);
	pos+= m_AuthCodeLen;
	
	if( m_AuthStatus == ADDING_AUTH_TYPE_COMPOUND){
		// 2 bytes, length of veri code
		*( (unsigned short *)(buf+pos)) = htons(m_QuestionCodeLen);
		pos+=2;
		// veri code here
		memcpy(buf+pos, m_QuestionCode, m_QuestionCodeLen);
		pos += m_QuestionCodeLen;
	}

	buf[pos++] = m_AllowAddReverse ? 0x01 : 0x02;
	buf[pos++] = m_DestGroup & 0xff;
	if( m_AuthStatus == ADDING_AUTH_TYPE_AUTH){
		// only happen when auth is 0x02
		len = m_Message.length();
		buf[pos++] = len & 0xff;
		memcpy(buf+pos, m_Message.c_str(), len);
		pos += len;
	}
	
	return pos;
	
}

/*  ======================================================= */
EvaAddFriendAuthExReplyPacket::EvaAddFriendAuthExReplyPacket(unsigned char* buf, int len)
	:InPacket(buf, len)
{
}

EvaAddFriendAuthExReplyPacket::EvaAddFriendAuthExReplyPacket( const EvaAddFriendAuthExReplyPacket& rhs)
	:InPacket(rhs)
{
	m_AddQQNum = rhs.getAddQQ();
	m_AuthStatus = rhs.getAuthStatus();
	m_ReplyCode = rhs.getReplyCode();
	
}

EvaAddFriendAuthExReplyPacket &EvaAddFriendAuthExReplyPacket::operator=(const EvaAddFriendAuthExReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	m_AddQQNum = rhs.getAddQQ();
	m_AuthStatus = rhs.getAuthStatus();
	m_ReplyCode = rhs.getReplyCode();
	
	return *this;
}

void EvaAddFriendAuthExReplyPacket::parseBody()
{
	int pos = 0;
	m_AuthStatus = decryptedBuf[pos++];
	m_AddQQNum = EvaUtil::read32(decryptedBuf+pos);
	pos += 4;
	m_ReplyCode = decryptedBuf[pos++]; // 0x00: success
}




/**  ======================================================= */

EvaAddFriendGetAuthInfoPacket::EvaAddFriendGetAuthInfoPacket( )
	:OutPacket(QQ_CMD_ADD_FRIEND_AUTH_INFO, true),
	m_IsQun(false),
	m_AddID(0),
	m_Cmd(AUTH_INFO_CMD_INFO),
	m_SubCmd(AUTH_INFO_SUB_CMD_USER)
{
}

EvaAddFriendGetAuthInfoPacket::EvaAddFriendGetAuthInfoPacket( const unsigned int id, const unsigned char cmd, const bool isQun )
	:OutPacket(QQ_CMD_ADD_FRIEND_AUTH_INFO, true),
	m_IsQun(isQun),
	m_AddID(id),
	m_Cmd(cmd),
	m_SubCmd(AUTH_INFO_SUB_CMD_USER)
{
}

EvaAddFriendGetAuthInfoPacket::EvaAddFriendGetAuthInfoPacket( const EvaAddFriendGetAuthInfoPacket & rhs )
	: OutPacket(rhs)
{
	*this = rhs;
}

EvaAddFriendGetAuthInfoPacket::~ EvaAddFriendGetAuthInfoPacket( )
{
}

EvaAddFriendGetAuthInfoPacket & EvaAddFriendGetAuthInfoPacket::operator =( const EvaAddFriendGetAuthInfoPacket & rhs )
{
	*((OutPacket *)this) = (OutPacket)rhs;
	m_AddID = rhs.getAddID();
	m_Cmd = rhs.getSubCommand();
	m_Verification = rhs.getVerificationStr();
	m_Session = rhs.getSessionStr();
	m_IsQun = rhs.isQun();
	m_SubCmd = rhs.getSubSubCommand();
	return *this;
}

int EvaAddFriendGetAuthInfoPacket::putBody( unsigned char * buf )
{
	int pos = 0;

	buf[pos++] = m_Cmd;
	
	// unknown 2 bytes, always 0x00 0x01
	buf[pos++] = 0x00;
	if(m_IsQun){
		buf[pos++] = AUTH_INFO_SUB_CMD_QUN;
	} else {
		buf[pos++] = m_SubCmd;
	}

	pos += EvaUtil::write32(buf+pos, m_AddID);

	if(m_Cmd == AUTH_INFO_CMD_INFO) return pos;

	// now, should be AUTH_INFO_CMD_CODE(0x02)

	// length of verification string
	*((unsigned short*)(buf+pos)) = htons(m_Verification.length());
	pos += 2;

	// verification string
	memcpy(	buf+pos, m_Verification.c_str(), m_Verification.length());
	pos += m_Verification.length();

	// length of session string
	*((unsigned short*)(buf+pos)) = htons(m_Session.length());
	pos += 2;
	
	// session string
	memcpy(	buf+pos, m_Session.c_str(), m_Session.length());
	pos += m_Session.length();
printf("EvaAddFriendGetAuthInfoPacket\n");
for(int i=0;i<pos;i++){
	if(!(i%8)) printf("\n");
	printf("%2x ", buf[i]);
}
printf("\n\n");
	return pos;
}


/**  ======================================================= */

EvaAddFriendGetAuthInfoReplyPacket::EvaAddFriendGetAuthInfoReplyPacket( )
	: m_IsQun(false),
	m_Cmd(AUTH_INFO_CMD_INFO),
	m_ReplyCode(AUTH_INFO_TYPE_CODE),
	m_CodeLen(0),
	m_Code(0)
{
}


EvaAddFriendGetAuthInfoReplyPacket::EvaAddFriendGetAuthInfoReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len),
	m_IsQun(false),
	m_Cmd(AUTH_INFO_CMD_INFO),
	m_ReplyCode(AUTH_INFO_TYPE_CODE),
	m_CodeLen(0),
	m_Code(0)
{
}

EvaAddFriendGetAuthInfoReplyPacket::EvaAddFriendGetAuthInfoReplyPacket( const EvaAddFriendGetAuthInfoReplyPacket & rhs )
	: InPacket(rhs),
	m_IsQun(false),
	m_Cmd(AUTH_INFO_CMD_INFO),
	m_ReplyCode(AUTH_INFO_TYPE_CODE),
	m_CodeLen(0),
	m_Code(0)
{
	*this = rhs;
}

EvaAddFriendGetAuthInfoReplyPacket::~ EvaAddFriendGetAuthInfoReplyPacket( )
{
	if(m_CodeLen && m_Code){
		delete []m_Code;
		m_CodeLen = 0;
	}
}

EvaAddFriendGetAuthInfoReplyPacket & EvaAddFriendGetAuthInfoReplyPacket::operator =( const EvaAddFriendGetAuthInfoReplyPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	m_Cmd = rhs.getSubCommand();
	m_ReplyCode = rhs.getReplyCode();
	m_CodeLen = rhs.getCodeLength();
	m_IsQun = rhs.isQun();
	if(m_CodeLen){
		if(m_Code) delete [] m_Code;
		m_Code = new unsigned char[m_CodeLen+1]; // allocate 1 more,just in case of url string situation
		memcpy(m_Code, rhs.getCode(), m_CodeLen);
	}
	return *this;
}

void EvaAddFriendGetAuthInfoReplyPacket::parseBody( )
{
printf("EvaAddFriendGetAuthInfoReplyPacket\n");
for(int i=0;i<bodyLength;i++){
	if(!(i%8)) printf("\n");
	printf("%2x ", decryptedBuf[i]);
}
printf("\n\n");
	int offset = 0;

	m_Cmd = decryptedBuf[offset++];

	offset++; // unknown 0x00

	m_SubCmd = decryptedBuf[offset++];
	if(m_SubCmd == AUTH_INFO_SUB_CMD_USER)
		m_IsQun = false;
	else if(m_SubCmd == AUTH_INFO_SUB_CMD_QUN)
			m_IsQun = true;

	m_ReplyCode = decryptedBuf[offset++];
	
	m_CodeLen = ntohs( *( (unsigned short *)(decryptedBuf + offset)) );
	offset+=2;

	if(m_CodeLen){
		if(m_Code) delete [] m_Code;
		m_Code = new unsigned char[m_CodeLen+1];	
		memcpy(m_Code, decryptedBuf + offset, m_CodeLen);
	}

	// just in case the url, give it a terminate char
	if(m_ReplyCode == AUTH_INFO_TYPE_GRAPHIC){
		m_Code[m_CodeLen] = 0x00;
		m_CodeLen++;// as it is the url string, so we append a 0x00 at the end
	}
}


/**  ======================================================= */



EvaAddFriendAuthQuestionPacket::EvaAddFriendAuthQuestionPacket( )
	: OutPacket(QQ_CMD_ADD_FRIEND_AUTH_QUESTION, true),
	m_AddQQNum(0),
	m_AuthStatus(AUTH_TYPE_QUESTION_REQUEST),
	m_Question(""),
	m_Answer("")
{
}

EvaAddFriendAuthQuestionPacket::EvaAddFriendAuthQuestionPacket( const unsigned int id, const unsigned char auth )
	: OutPacket(QQ_CMD_ADD_FRIEND_AUTH_QUESTION, true),
	m_AddQQNum(id),
	m_AuthStatus(auth),
	m_Question(""),
	m_Answer("")
{
}

EvaAddFriendAuthQuestionPacket::EvaAddFriendAuthQuestionPacket( const EvaAddFriendAuthQuestionPacket & rhs )
	: OutPacket(rhs)
{
	*this = rhs;
}

EvaAddFriendAuthQuestionPacket::~EvaAddFriendAuthQuestionPacket( )
{
}

EvaAddFriendAuthQuestionPacket & EvaAddFriendAuthQuestionPacket::operator =( const EvaAddFriendAuthQuestionPacket & rhs )
{
	*( (OutPacket*)(this) ) = (OutPacket)(rhs);
	m_AddQQNum = rhs.getAddQQ();
	m_AuthStatus = rhs.getAuthStatus();
	m_Question = rhs.getQuestion();
	m_Answer = rhs.getAnswer();
	return *this;
}

int EvaAddFriendAuthQuestionPacket::putBody( unsigned char * buf )
{
	int pos = 0;
	buf[pos++] = m_AuthStatus; // 0x01, 0x02, 0x03 or 0x04

	if(m_AuthStatus == AUHT_TYPE_QUESTION_GET) {
		buf[pos++] = 0x00;
		return pos;
	}

	if(m_AuthStatus == AUTH_TYPE_QUESTION_SET){
		printf("question(%d):%s\n", m_Question.length(), m_Question.c_str());
		buf[pos++] = (unsigned char)(m_Question.length() & 0xff);
		memcpy(buf + pos, m_Question.c_str(), m_Question.length());
		pos += m_Question.length();

		buf[pos++] = (unsigned char)(m_Answer.length() & 0xff);
		printf("answer(%d):%s\n", m_Answer.length(), m_Answer.c_str());
		memcpy(buf + pos, m_Answer.c_str(), m_Answer.length());
		pos += m_Answer.length();
		
		buf[pos++] = 0x00;
printf("EvaAddFriendAuthQuestionPacket\n");
for(int i=0;i<pos;i++){
	if(!(i%8)) printf("\n");
	printf("%2x ", buf[i]);
}
printf("\n\n");
		return pos;
	}	
	// unknown 2 bytes, always 0x0001
	buf[pos++] = 0x00;
	buf[pos++] = 0x01;

	*((unsigned int *)(buf + pos)) = htonl(m_AddQQNum);
	pos+= 4;

	if(m_AuthStatus == AUTH_TYPE_QUESTION_REQUEST) return pos;

	buf[pos++] = (unsigned char)(m_Answer.length() & 0xff);
	memcpy(buf + pos, m_Answer.c_str(), m_Answer.length());
	pos += m_Answer.length();

	return pos;
}



/**  ======================================================= */

EvaAddFriendAuthQuestionReplyPacket::EvaAddFriendAuthQuestionReplyPacket( )
	: m_AuthStatus(AUTH_TYPE_QUESTION_REQUEST),
	m_ReplyCode(01),
	m_CodeLen(0),
	m_Code(0),
	m_Answer("")
{
}

EvaAddFriendAuthQuestionReplyPacket::EvaAddFriendAuthQuestionReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len),
	m_AuthStatus(AUTH_TYPE_QUESTION_REQUEST),
	m_ReplyCode(01),
	m_CodeLen(0),
	m_Code(0),
	m_Answer("")
{
}

EvaAddFriendAuthQuestionReplyPacket::EvaAddFriendAuthQuestionReplyPacket( const EvaAddFriendAuthQuestionReplyPacket & rhs )
	: InPacket(rhs),
	m_AuthStatus(AUTH_TYPE_QUESTION_REQUEST),
	m_ReplyCode(01),
	m_CodeLen(0),
	m_Code(0),
	m_Answer("")
{
	*this = rhs;
}

EvaAddFriendAuthQuestionReplyPacket::~ EvaAddFriendAuthQuestionReplyPacket( )
{
	if(m_CodeLen && m_Code){
		delete []m_Code;
		m_CodeLen = 0;
	}
}

EvaAddFriendAuthQuestionReplyPacket & EvaAddFriendAuthQuestionReplyPacket::operator =( const EvaAddFriendAuthQuestionReplyPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	m_AuthStatus = rhs.getAuthStatus();
	m_ReplyCode = rhs.getReplyCode();
	m_CodeLen = rhs.getCodeLength();
	if(m_CodeLen){
		if(m_Code) delete [] m_Code;
		m_Code = new unsigned char[m_CodeLen+1]; // allocate 1 more,just in case of url string situation
		memcpy(m_Code, rhs.getCode(), m_CodeLen);
	}
	m_Answer = rhs.getAnswer();
	return *this;
}

void EvaAddFriendAuthQuestionReplyPacket::parseBody( )
{
printf("EvaAddFriendAuthQuestionReplyPacket\n");
for(int i=0;i<bodyLength;i++){
	if(!(i%8)) printf("\n");
	printf("%2x ", decryptedBuf[i]);
}
printf("\n\n");
	int offset = 0;

	m_AuthStatus = decryptedBuf[offset++];

	if(m_AuthStatus == AUHT_TYPE_QUESTION_GET){
		m_CodeLen = decryptedBuf[offset++];
		if(m_CodeLen){
			if(m_Code) delete [] m_Code;
			m_Code = new unsigned char[m_CodeLen+1];
			memcpy(m_Code, decryptedBuf + offset, m_CodeLen);
			m_Code[m_CodeLen] = 0x00;
			offset += m_CodeLen;
		}
		
		unsigned int len = decryptedBuf[offset++];
		if(len){
			char *tmp = new char [len+1];
			memcpy(tmp, decryptedBuf + offset, len);
			tmp[len] = 0x00;
			m_Answer = tmp;
			delete []tmp;
			offset += len;
		}
		return;
	}

	if(m_AuthStatus == AUTH_TYPE_QUESTION_SET){
		m_ReplyCode = decryptedBuf[offset++]; // 0x00: success, (? -- 0x01: error)
		return;
	}

	offset+=2; // unknown 2 bytes, always be 0x0001

	m_ReplyCode = decryptedBuf[offset++];
	
	if(m_ReplyCode == 0x01){
		// should be error happened
		return;
	}

	switch(m_AuthStatus){
	case AUTH_TYPE_QUESTION_REQUEST:
		m_CodeLen = decryptedBuf[offset++];
		break;
	case AUTH_TYPE_QUESTION_ANSWER:{
		m_CodeLen = ntohs( *( (unsigned short *)(decryptedBuf + offset)) );
		offset+=2;
		}
		break;
	default:
		fprintf(stderr, "Unknown auth status code: 0x%2x\n", m_AuthStatus);
		return;
	}
	if(m_CodeLen){
		if(m_Code) delete [] m_Code;
		m_Code = new unsigned char[m_CodeLen+1];
		memcpy(m_Code, decryptedBuf + offset, m_CodeLen);
		offset += m_CodeLen;
	}

	// just in case the url, give it a terminate char
	if(m_AuthStatus == AUTH_TYPE_QUESTION_REQUEST || 
		m_AuthStatus == AUHT_TYPE_QUESTION_GET){
		m_Code[m_CodeLen] = 0x00;
		m_CodeLen++;
	}
}





