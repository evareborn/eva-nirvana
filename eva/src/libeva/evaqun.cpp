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
 
#include "evaqun.h" 
#include "evadefines.h"
#include "evautil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring>
#include <cstdlib>

QunInfo::QunInfo()
	: qunID(0), externalID(0), type(QQ_QUN_TYPE_PERMANENT),
	creator(0), authType(QQ_QUN_NEED_AUTH), category(0),
	versionID(0), name(""), description(""), notice(""),
	m_UnknownTokenLength(0),
	m_UnknownToken(0)
{
}

QunInfo::QunInfo(const QunInfo &rhs)
	: qunID(0), externalID(0), type(QQ_QUN_TYPE_PERMANENT),
	creator(0), authType(QQ_QUN_NEED_AUTH), category(0),
	versionID(0), name(""), description(""), notice(""),
	m_UnknownTokenLength(0),
	m_UnknownToken(0)
{
	*this = rhs;
}

QunInfo::~QunInfo()
{
	if(m_UnknownToken)
		delete [] m_UnknownToken;
}

void QunInfo::clearInfo()
{
	qunID = 0;
	externalID = 0;
	type = QQ_QUN_TYPE_PERMANENT;
	creator = 0;
	authType = QQ_QUN_NEED_AUTH;
	category = 0;
	versionID = 0;
	name = "";
	description = "";
	notice = "";
	m_UnknownTokenLength = 0;
	m_UnknownToken = 0;
}

QunInfo &QunInfo::operator=(const QunInfo &rhs)
{
	qunID = rhs.getQunID();
	externalID = rhs.getExtID();
	type = rhs.getType();
	creator = rhs.getCreator();
	authType = rhs.getAuthType();
	unknown1 = rhs.getUnknown1();
	category = rhs.getCategory();
	versionID = rhs.getVersionID();
	name = rhs.getName();
	unknown2 = rhs.getUnknown2();
	description = rhs.getDescription();
	notice = rhs.getNotice();
	m_UnknownTokenLength = rhs.getUnknownTokenLength();
	if(!m_UnknownTokenLength)
		return *this;

	if(m_UnknownToken)
		delete [] m_UnknownToken;
	m_UnknownToken = new unsigned char [m_UnknownTokenLength];
	memcpy(m_UnknownToken, rhs.getUnknownToken(), m_UnknownTokenLength);
	return *this;
}

const bool QunInfo::operator==(const QunInfo &rhs)
{
	return (qunID == rhs.getQunID())       && (externalID == rhs.getExtID()) &&
		(type == rhs.getType())         && (authType == rhs.getAuthType()) &&
		(category == rhs.getCategory()) && (name == rhs.getName()) &&
		(notice == rhs.getNotice())     && (notice == rhs.getNotice()) ;
}

int QunInfo::readTempQunInfo(unsigned char *buf)
{
	int pos=0;
	type = buf[pos++];
	
	int tmp4;
	memcpy(&tmp4, buf+pos, 4);
	externalID = ntohl(tmp4);
	pos+=4;
	
	memcpy(&tmp4, buf+pos, 4);
	qunID = ntohl(tmp4);
	pos+=4;
	
	memcpy(&tmp4, buf+pos, 4);
	creator = ntohl(tmp4);
	pos+=4;

	authType = buf[pos++];
	unknown1 = buf[pos++];
	
	short tmp2;
	memcpy(&tmp2, buf+pos, 2);
	category = ntohs(tmp2);
	pos+=2;
	
	// length of group name
	int len = buf[pos++];
	char *nameChar = (char *)malloc((len + 1)* sizeof(char));
	memcpy(nameChar, buf + pos, len);
	nameChar[len]=0x00;
	name.assign(nameChar);
	free(nameChar);
	pos+=len;
	
	
	return pos;
}

int QunInfo::readQunInfo(unsigned char *buf)
{
	int pos=0;
	
	int tmp4;
	memcpy(&tmp4, buf+pos, 4);
	qunID = ntohl(tmp4);
	pos+=4;

	memcpy(&tmp4, buf+pos, 4);
	externalID = ntohl(tmp4);
	pos+=4;
	
	type = buf[pos++];
	
	pos+=4; // here we ignore 4 unknown bytes;
	
	memcpy(&tmp4, buf+pos, 4);
	creator = ntohl(tmp4);
	pos+=4;

	authType = buf[pos++];
	
	pos+=6; // here we ignore 6 unknown bytes;
	
 	short tmp2;
	category = EvaUtil::read32( buf + pos); pos+=4;
	
	memcpy(&tmp2, buf+pos, 2);
	unknown1 = ntohs(tmp2);
	pos+=2;

	pos++;

	pos+=4; // unknown 4 bytes

	memcpy(&tmp4, buf+pos, 4);
	versionID = ntohl(tmp4);
	pos+=4;
	
	//pos+=2; // unknown 2 bytes, 0x00 00

	//tmp2 = ntohs(*((unsigned short *)(buf+pos))); // the length of Qun name and Qun notice(max: 0xff ff)
	//pos+=2;

	// length of Qun name
	int len = buf[pos++];   // which means the max length of Qun name is 0xff
	char *nameChar = (char *)malloc((len + 1)* sizeof(char));
	memcpy(nameChar, buf + pos, len);
	nameChar[len]=0x00;
	name.assign(nameChar);
	free(nameChar);
	pos+=len;
	
	pos++; // one byte 0x00, could be the C-stryle string terminator
	pos++; // normally 0x00, 0x02 if super qun?

	// length of Qun notice
	len = buf[pos++];  // starkwong: means the max length of Qun notice is also 0xff, not 0xffff
	char *noticeChar = (char *)malloc((len + 1)* sizeof(char));
	memcpy(noticeChar, buf + pos, len);
	noticeChar[len]=0x00;
	notice.assign(noticeChar);
	free(noticeChar);
	pos+=len;

	// length of Qun description, max length is 0xff
	len = buf[pos++];
	char *descriptionChar = (char *)malloc((len + 1)* sizeof(char));
	memcpy(descriptionChar, buf + pos, len);
	descriptionChar[len]=0x00;
	description.assign(descriptionChar);
	free(descriptionChar);
	pos+=len;
	
	return pos;
}

int QunInfo::readQunInfoFromSearchReply(unsigned char *buf)
{
	int pos=0;
	
	int tmp4;
	memcpy(&tmp4, buf+pos, 4);
	qunID = ntohl(tmp4);
	pos+=4;

	memcpy(&tmp4, buf+pos, 4);
	externalID = ntohl(tmp4);
	pos+=4;
	
	type = buf[pos++];
	
	if (QQ_CLIENT_VERSION>=QQ_CLIENT_0D51)
		pos+=4;

	memcpy(&tmp4, buf+pos, 4);
	creator = ntohl(tmp4);
	pos+=4;

	short tmp2;
	memcpy(&tmp2, buf+pos, 2);
	unknown1 = ntohs(tmp2);
	pos+=4; // qun category for QQ 2004, should be a int
	
	category = EvaUtil::read32( buf+pos); pos+=4;

	pos+=2; // unkonwn 2 bytes
	
	// length of group name
	int len = buf[pos++];
	char *nameChar = (char *)malloc((len + 1)* sizeof(char));
	memcpy(nameChar, buf + pos, len);
	nameChar[len]=0x00;
	name.assign(nameChar);
	free(nameChar);
	pos+=len;
	
	memcpy(&tmp2, buf+pos, 2);
	unknown2 = ntohs(tmp2);
	pos+=2;

	authType = buf[pos++];

	// length of group description
	len = buf[pos++];
	char *descriptionChar = (char *)malloc((len + 1)* sizeof(char));
	memcpy(descriptionChar, buf + pos, len);
	descriptionChar[len]=0x00;
	description.assign(descriptionChar);
	free(descriptionChar);
	pos+=len;

	// new stuff in 2006
	m_UnknownTokenLength = EvaUtil::read16(buf + pos); pos+=2;
	m_UnknownToken = new unsigned char[m_UnknownTokenLength];
	memcpy(m_UnknownToken, buf + pos, m_UnknownTokenLength);
	
	pos+=m_UnknownTokenLength;
	return pos;
}

/**********************************************************************************************************************/

QunPacket::QunPacket(const char qunCmd)
	: OutPacket(QQ_CMD_QUN_CMD, true)
{
	qunCommand = qunCmd;
}

QunPacket::QunPacket(const char qunCmd, const unsigned int id)
	: OutPacket(QQ_CMD_QUN_CMD, true)
{
	qunCommand = qunCmd;
	qunID = id;
}

QunPacket::QunPacket(const QunPacket &rhs)
	: OutPacket(rhs)
{
	*this = rhs;
}

QunPacket::~QunPacket()
{
}

QunPacket &QunPacket::operator=(const QunPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	qunCommand = rhs.getQunCommand();
	qunID = rhs.getQunID();
	return *this;
}




/***************************************************************************************************************/

QunReplyPacket::QunReplyPacket(unsigned char *buf, int len)
	: InPacket(buf, len)
{
}

QunReplyPacket::QunReplyPacket(const QunReplyPacket &rhs)
	: InPacket(rhs)
{
	*this = rhs;
}

QunReplyPacket::~QunReplyPacket()
{
}
	
QunReplyPacket &QunReplyPacket::operator=(const QunReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	qunCommand = rhs.getQunCommand();
	replyCode = rhs.getReplyCode();
	qunID = rhs.getQunID();
	externalID = rhs.getExtID();
	type = rhs.getType();
	parentQunID = rhs.getParentQunID();
	errorMessage = rhs.getErrorMessage();
	
	searchType = rhs.getSearchType();
	info = rhs.getQunInfo();
	qunInfoList = rhs.getQunInfoList();
	qunQQNumberList = rhs.getQQNumberList();
	memberList = rhs.getMemberList();
	memberInfoList = rhs.getMemberInfoList();
	joinReply = rhs.getJoinReply();
	targetQQ = rhs.getTargetQQ();
	opCode = rhs.getOpCode();
	versionID = rhs.getQunVersionID();
	realName = rhs.getRealName();
	gender = rhs.getGender();
	phone = rhs.getPhone();
	email = rhs.getEmail();
	memo = rhs.getMemo();
	return *this;
}

void QunReplyPacket::parseBody()
{
// 			printf("QunReplyPacket::parseBody\n");
// 			for(int i=0; i<bodyLength; i++){
// 				if(!(i%8)) printf("\n%d: ",i);
// 				char t = decryptedBuf[i];
// 				printf("%2x ", (uint8_t)t);
// 			}
// 			printf("\n");
	int pos = 0;
	qunCommand = decryptedBuf[pos++];
	replyCode = decryptedBuf[pos++];
	switch(qunCommand){
	case QQ_QUN_CMD_SEND_IM:
	case QQ_QUN_CMD_SEND_IM_EX:
		pos += parseSendIMReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_SEND_TEMP_QUN_IM:
		pos += parseSendTempQunIMReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_CREATE_QUN:
		pos += parseCreateReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_CREATE_TEMP_QUN:
		pos += parseCreateTempQun(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_ACTIVATE_QUN:
		pos += parseActivateReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_MODIFY_MEMBER:
		pos += parseModifyMemberReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_GET_QUN_INFO:
		pos += parseGetInfoReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_EXIT_QUN:
		pos += parseExitReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_GET_MEMBER_INFO:
		pos += parseGetMemberInfoReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_GET_ONLINE_MEMBER:
		pos += parseGetOnlineMemberReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_JOIN_QUN:
		pos += parseJoinReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_JOIN_QUN_AUTH:
		pos += parseJoinAuthReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_MODIFY_QUN_INFO:
		pos += parseModifyInfoReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_SEARCH_QUN:
		pos += parseSearchReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_GET_TEMP_QUN_INFO:
		pos += parseGetTempQunInfoReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_EXIT_TEMP_QUN:
		pos += parseExitTempQunReply(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_GET_TEMP_QUN_MEMBERS:
		pos += parseGetTempQunMembers(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_MODIFY_CARD:
		pos += parseModifyQunCard(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_REQUEST_ALL_REALNAMES:
		pos += parseRequestAllRealNames(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_REQUEST_CARD:
		pos += parseRequestQunCard(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_ADMIN:
		pos += parseQunAdminOperation(decryptedBuf + pos, bodyLength - pos);
		break;
	case QQ_QUN_CMD_TRANSFER:
		pos += parseQunTransfer(decryptedBuf + pos, bodyLength - pos);
		break;
	default:
		printf("unknown Qun command:0x%2x\n", (char)qunCommand);
	}
	
	if(replyCode != QQ_QUN_CMD_REPLY_OK) {
		/* operation failed */
		char * errorChar = (char *)malloc((bodyLength-pos+1) * sizeof(char));
		errorChar[bodyLength-pos] = 0x00;
		memcpy(errorChar, decryptedBuf + pos, bodyLength - pos);
		printf("Qun Reply error(%d):%s\n", qunCommand, errorChar);
		errorMessage.assign(errorChar);
		free(errorChar);
		pos = bodyLength;
	}
}

const bool QunReplyPacket::isReplyOK() const
{
	return replyCode == QQ_QUN_CMD_REPLY_OK;
}

// useful variable: qunID
int QunReplyPacket::parseSendIMReply(unsigned char *buf, int)
{
	int pos = 0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK){
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
	}
	return pos;
}

// useful variables: type, parentQunID and qunID
int QunReplyPacket::parseSendTempQunIMReply(unsigned char *buf, int)
{
	int pos = 0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {    
		type = buf[pos++];    
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		parentQunID = ntohl(tmp4);
		pos+=4;
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
	}
	return pos;
}

// useful variables: qunID and externalID
int QunReplyPacket::parseCreateReply(unsigned char *buf, int)
{
	int pos = 0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK){
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
		
		memcpy(&tmp4, buf + pos, 4);
		externalID = ntohl(tmp4);
		pos+=4;
	}
	return pos;
}

// useful variables:  qunID, parentQunID and type
int QunReplyPacket::parseCreateTempQun(unsigned char *buf, int)
{
	int pos = 0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {		    
		type = buf[pos++];    
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		parentQunID = ntohl(tmp4);
		pos+=4;
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
	}
	return pos;
}

// useful variable: qunID
int QunReplyPacket::parseActivateReply(unsigned char *buf, int)
{
	int pos = 0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK){
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
	}
	return pos;
}

// useful variable: qunID
int QunReplyPacket::parseModifyMemberReply(unsigned char *buf, int )
{
	int pos = 0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK){
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
	}
	return pos;
}

/*
         for this part, the useful variables are:
	 qunID,  externalID and
	 info,  memberList (actually these two contain all information)
*/
int QunReplyPacket::parseGetInfoReply(unsigned char *buf, int len)
{
// 			printf("Qun info reply --- \n");
// 			for(int i=0; i<len; i++){
// 				if(!(i%8)) printf("\n%d: ",i);
// 				char t = buf[i];
// 				printf("%2x ", (uint8_t)t);
// 			}
// 			printf("\n");
	int pos=0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {
		int pos = info.readQunInfo(buf);
		qunID = info.getQunID();
		externalID = info.getExtID();
		
		int tmp4;
		while(pos < len ) {
			QunMember member;
			memcpy(&tmp4, buf+pos, 4);
			member.qqNum = ntohl(tmp4);
			pos+=4;
			
			member.groupIndex = buf[pos++];
			member.admin = buf[pos++];
			memberList[member.qqNum] = member;
			//printf("\t\tqq:%d, group index:%d, admin:%2x\n", member.qqNum, member.groupIndex, member.admin);
		}
		//printf("memberList size: %d\n", memberList.size());
	}
	return pos;
}

// useful variable: qunID
int QunReplyPacket::parseExitReply(unsigned char *buf, int )
{
	int pos = 0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK){
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
	}
	return pos;
}

// useful variables:  qunID and memberInfoList;
int QunReplyPacket::parseGetMemberInfoReply(unsigned char *buf, int len)
{
// 			printf("Qun member info reply --- \n");
// 			for(int i=0; i<len; i++){
// 				if(!(i%8)) printf("\n%d: ",i);
// 				char t = buf[i];
// 				printf("%2x ", (uint8_t)t);
// 			}
// 			printf("\n");
	int pos=0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {
		int tmp4;
		memcpy(&tmp4, buf+pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
		// member information now
		int tmpLen=0;
		while(pos < len) {
			FriendItem *info = new FriendItem(buf + pos, &tmpLen);
			memberInfoList.push_back(*info);
			delete info;
			pos+=tmpLen;
		}
	}
	return pos;
}

// useful variables: qunID and qunQQNumberList
int QunReplyPacket::parseGetOnlineMemberReply(unsigned char *buf, int len)
{
	int pos=0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
		pos++; // ignore one unknow byte, always be 0x3c;
		while(pos < len ) {
			memcpy(&tmp4, buf+pos, 4);
			qunQQNumberList.push_back(ntohl(tmp4));
			pos+=4;
		}
	}
	return pos;
}

// useful variables: qunID and joinReply
int QunReplyPacket::parseJoinReply(unsigned char *buf, int )
{
	int pos = 0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK){
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
		joinReply = buf[pos++];
	}
	return pos;
}

// useful varible: qunID    ** note that this is only a reply from server for your request and dose not mean anything
int QunReplyPacket::parseJoinAuthReply(unsigned char *buf, int )
{
	int pos = 0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK){
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
	}
	return pos;
}

// useful variable: qunID
int QunReplyPacket::parseModifyInfoReply(unsigned char *buf, int )
{
	int pos = 0;
	//if(replyCode == QQ_QUN_CMD_REPLY_OK){
	int tmp4;
	memcpy(&tmp4, buf + pos, 4);
	qunID = ntohl(tmp4);
	pos+=4;
	//}
	return pos;
}

// useful variables: searchType and qunInfoList
int QunReplyPacket::parseSearchReply(unsigned char *buf, int len)
{
			printf("QunReplyPacket::parseSearchReply\n");
			for(int i=0; i<len; i++){
				if(!(i%8)) printf("\n%d: ",i);
				char t = buf[i];
				printf("%2x ", (uint8_t)t);
			}
			printf("\n");
	int pos=0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {
		searchType = buf[pos++]; // actually, In new QQ, all types except QQ_SEARCH_QUN_BY_ID are searched by Web Explorer
		while(pos < len ) {
			QunInfo info;
			pos+=info.readQunInfoFromSearchReply(buf+pos);
			qunInfoList.push_back(info);
		}
	}
	printf("QunReplyPacket:: parseSearchReply ----- finished\n");
	return pos;
}

/*
         for this part, the useful variables are:
	 type, qunID,  parentID and
	 info,  qunQQNumberList (actually these two contain all information)
*/
int QunReplyPacket::parseGetTempQunInfoReply(unsigned char *buf, int len)
{
	int pos=0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {
		pos = info.readTempQunInfo(buf);
		type = info.getType();
		qunID = info.getQunID();
		parentQunID = info.getExtID();
		
		int tmp4;
		while(pos < len ) {
			memcpy(&tmp4, buf+pos, 4);
			qunQQNumberList.push_back(ntohl(tmp4));
			pos+=5; // note that we ignore one unknow byte, maybe just the seperator byte
		}
	}
	return pos;
}

int QunReplyPacket::parseExitTempQunReply(unsigned char *buf, int )
{
	int pos = 0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {
		type = buf[pos++];  
		  
		int tmp4;
		memcpy(&tmp4, buf + pos, 4);
		parentQunID = ntohl(tmp4);
		pos+=4;
		
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
	}
	return pos;
}

// useful variables: type, parentQunID, qunID and qunQQNumberList
int QunReplyPacket::parseGetTempQunMembers(unsigned char *buf, int len)
{
	int pos=0;
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {
		type = buf[pos++];
		
		int tmp4;
		memcpy(&tmp4, buf+pos, 4);
		parentQunID = ntohl(tmp4);
		pos+=4;
		
		memcpy(&tmp4, buf + pos, 4);
		qunID = ntohl(tmp4);
		pos+=4;
		
		while(pos < len ) {
			memcpy(&tmp4, buf+pos, 4);
			qunQQNumberList.push_back(ntohl(tmp4));
			pos+=4; 
		}
	}
	return pos;
}

// useful variables:  qunID,  targetQQ
int QunReplyPacket::parseModifyQunCard(unsigned char *buf, int /*len*/)
{
	int pos=0;
	//if(replyCode == QQ_QUN_CMD_REPLY_OK) {
	int tmp4;
	memcpy(&tmp4, buf+pos, 4);
	qunID = ntohl(tmp4);
	pos+=4;
	
	memcpy(&tmp4, buf + pos, 4);
	targetQQ = ntohl(tmp4);
	pos+=4;
	//}
	return pos;
}

int QunReplyPacket::parseRequestAllRealNames(unsigned char *buf, int len)
{
	if(replyCode != QQ_QUN_CMD_REPLY_OK) {
		printf("Qun real name reply code is NOT ok.\n");
		return 0;
	}
	int pos = 0;
	qunID = EvaUtil::read32( buf + pos); pos+=4;
	m_CardVersion = EvaUtil::read32( buf + pos); pos+=4; 
	m_NextStart = EvaUtil::read32( buf + pos); pos+=4;
	m_RealNameList.clear();
	int tmpId;
	char strName[257];
	unsigned char nameLen = 0;
	while(pos<len){
		tmpId = EvaUtil::read32( buf + pos); pos+=4;
		nameLen = buf[pos++];
		memcpy(strName, buf + pos, nameLen); pos+=nameLen;
		strName[nameLen] = 0x0;
		std::string name;
		name.assign(strName);
		m_RealNameList[tmpId] = name;
	}
	return pos;
}

// useful variables: qunID, targetQQ
int QunReplyPacket::parseRequestQunCard(unsigned char *buf, int /*len*/)
{
	int pos = 0;
	
	int tmp4;
	memcpy(&tmp4, buf + pos, 4); pos+=4;
	qunID = ntohl(tmp4);
	
	memcpy(&tmp4, buf + pos, 4);
	targetQQ = ntohl(tmp4);
	pos+=4;
	
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {
		unsigned char strLen;
		char *str;
		strLen = buf[pos++];
		str = new char[strLen + 1];
		memcpy(str, buf+pos, strLen); pos+=strLen;
		str[strLen] = 0x00;
		realName.assign(str);
		delete str;

		gender = buf[pos++];
		
		strLen = buf[pos++];
		str = new char[strLen + 1];
		memcpy(str, buf+pos, strLen); pos+=strLen;
		str[strLen] = 0x00;
		phone.assign(str);
		delete str;
		
		strLen = buf[pos++];
		str = new char[strLen + 1];
		memcpy(str, buf+pos, strLen); pos+=strLen;
		str[strLen] = 0x00;
		email.assign(str);
		delete str;
		
		strLen = buf[pos++];
		str = new char[strLen + 1];
		memcpy(str, buf+pos, strLen); pos+=strLen;
		str[strLen] = 0x00;
		memo.assign(str);
		delete str;
	}else
		pos++;
	return pos;
}

int QunReplyPacket::parseQunAdminOperation(unsigned char *buf, int /*len*/)
{
	int pos=0;

	int tmp4;
	memcpy(&tmp4, buf+pos, 4);
	qunID = ntohl(tmp4);
	pos+=4;
		
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {
		memcpy(&tmp4, buf + pos, 4);
		versionID = ntohl(tmp4);
		pos+=4;	
		
		memcpy(&tmp4, buf + pos, 4);
		targetQQ = ntohl(tmp4);
		pos+=4;
		
		opCode = buf[pos++];
	}
	
	return pos;
}

int QunReplyPacket::parseQunTransfer(unsigned char *buf, int /*len*/)
{
	int pos=0;
	int tmp4;
	memcpy(&tmp4, buf+pos, 4);
	qunID = ntohl(tmp4);
	pos+=4;
	
	memcpy(&tmp4, buf + pos, 4);
	targetQQ = ntohl(tmp4);
	pos+=4;
	
	if(replyCode == QQ_QUN_CMD_REPLY_OK) {
		memcpy(&tmp4, buf + pos, 4);
		versionID = ntohl(tmp4);
		pos+=4;
	}
	return pos;
}

/***************************************************************************************************************/
 

QunGetInfoPacket::QunGetInfoPacket(const int id)
	: QunPacket(QQ_QUN_CMD_GET_QUN_INFO, id)
{
}

QunGetInfoPacket::QunGetInfoPacket(const QunGetInfoPacket &rhs)
	: QunPacket(rhs)
{
}

QunGetInfoPacket::~QunGetInfoPacket()
{
}

int QunGetInfoPacket::putBody(unsigned char *buf)
{
	buf[0] = qunCommand;
	int tmp4 = htonl(qunID);
	memcpy(buf + 1, &tmp4, 4);
	return 5;
}


/***************************************************************************************************************/
 


QunGetMemberInfoPacket::QunGetMemberInfoPacket(const int id)
	: QunPacket(QQ_QUN_CMD_GET_MEMBER_INFO, id)
{
}

QunGetMemberInfoPacket::QunGetMemberInfoPacket( const QunGetMemberInfoPacket & rhs )
	: QunPacket(rhs)
{
	memberList = rhs.getMemberList();
}

QunGetMemberInfoPacket::~ QunGetMemberInfoPacket( )
{
}

QunGetMemberInfoPacket & QunGetMemberInfoPacket::operator =( const QunGetMemberInfoPacket & rhs )
{
	*(QunPacket*)this = (QunPacket)rhs;
	memberList = rhs.getMemberList();
	return *this;
}

void QunGetMemberInfoPacket::addMember( const unsigned int id )
{
	memberList.push_back(id);
}

int QunGetMemberInfoPacket::putBody( unsigned char * buf )
{
	int pos=0;
	buf[pos++] = qunCommand;
	int tmp4 = htonl(qunID);
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;
	
	std::list<unsigned int>::iterator itr;
	for(itr=memberList.begin(); itr!= memberList.end(); ++itr){
		tmp4 = htonl(*itr);
		memcpy(buf + pos, &tmp4, 4);
		pos+=4;
		
	}
	return pos;
}



/***************************************************************************************************************/
 
QunGetOnlineMemberPacket::QunGetOnlineMemberPacket(const unsigned int id)
	: QunPacket(QQ_QUN_CMD_GET_ONLINE_MEMBER, id)
{
}

QunGetOnlineMemberPacket::QunGetOnlineMemberPacket(const QunGetOnlineMemberPacket &rhs)
	: QunPacket(rhs)
{
}

QunGetOnlineMemberPacket::~QunGetOnlineMemberPacket()
{
}

int QunGetOnlineMemberPacket::putBody(unsigned char *buf)
{
	buf[0] = qunCommand;
	int tmp4 = htonl(qunID);
	memcpy(buf + 1, &tmp4, 4);
	return 5;
}



/***************************************************************************************************************/
 
QunActivatePacket::QunActivatePacket(const unsigned int id)
	: QunPacket(QQ_QUN_CMD_ACTIVATE_QUN, id)
{
}

QunActivatePacket::QunActivatePacket(const QunActivatePacket &rhs)
	: QunPacket(rhs)
{
}

QunActivatePacket::~QunActivatePacket()
{
}

int QunActivatePacket::putBody(unsigned char *buf)
{
	buf[0] = qunCommand;
	int tmp4 = htonl(qunID);
	memcpy(buf + 1, &tmp4, 4);
	return 5;
}



/***************************************************************************************************************/

QunAuthPacket::QunAuthPacket()
	: QunPacket(),
	type(QQ_QUN_AUTH_REQUEST),
	receiver(0),
	m_Code(0),
	m_CodeLen(0),
	message("")
{
}

QunAuthPacket::QunAuthPacket( const unsigned int id, unsigned char authType )
	: QunPacket(QQ_QUN_CMD_JOIN_QUN_AUTH, id),
	type(authType),
	receiver(0),
	m_Code(0),
	m_CodeLen(0),
	message("")
{
}

QunAuthPacket::QunAuthPacket( const QunAuthPacket & rhs )
	: QunPacket(rhs)
{
	*this = rhs;
}

QunAuthPacket::~ QunAuthPacket( )
{
	if(m_Code)
		delete []m_Code;
}

QunAuthPacket & QunAuthPacket::operator =( const QunAuthPacket & rhs )
{
	*(QunPacket *)this = (QunPacket)rhs;
	type = rhs.getAuthType();
	receiver = rhs.getReceiver();
	message = rhs.getMessage();
	setCode(rhs.getCode(), rhs.getCodeLength());
	return *this;
}

void QunAuthPacket::setCode(const unsigned char *code, const unsigned short len)
{
	if(len && code){
		m_CodeLen = len;
		if(m_Code) delete []m_Code;
		m_Code = new unsigned char[m_CodeLen];
		memcpy(m_Code, code, m_CodeLen);
	}
}

void QunAuthPacket::setToken(const unsigned char *code, const unsigned short len)
{
	// so far, code and token never appear in the same sub-command of
	// this packet. so, we just use one pair of variabls
	setCode(code, len);
}

int QunAuthPacket::putBody( unsigned char * buf )
{
	int pos=0;
	buf[pos++] = qunCommand;
	
	int tmp4 = htonl(qunID);
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;
	
	buf[pos++]=type;

	if(type == QQ_QUN_AUTH_REQUEST){
		WRITE16(buf+pos, m_CodeLen);
		pos+=2;
	
		memcpy(buf+pos, m_Code, m_CodeLen);
		pos += m_CodeLen;
	}

	tmp4 = htonl(receiver);
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;
	
	int len = message.length();
	buf[pos++] = (char)(len & 0xFF);
	
	memcpy(buf+pos, message.c_str(), len);
	pos+=len;

	if(type == QQ_QUN_AUTH_APPROVE ||
		type == QQ_QUN_AUTH_REJECT){
		WRITE16(buf+pos, m_CodeLen);
		pos+=2;
	
		memcpy(buf+pos, m_Code, m_CodeLen);
		pos += m_CodeLen;
	}

	printf("QunAuthPacket\n");
	for(int i=0; i<pos; i++){
		if(!(i%8)) printf("\n%d: ", i);
		printf("%2x ", (uint8_t)buf[i]);
	}
	printf("\n\n");
	return pos;
}



/***************************************************************************************************************/

QunSendIMPacket::QunSendIMPacket( const unsigned int id )
	: QunPacket(QQ_QUN_CMD_SEND_IM, id),
	encoding(QQ_IM_ENCODING_GB),
	red(0),green(0),blue(0),
	bold(false),italic(false),underline(false),
	fontSize(0x09),
	fontFlag(0x09),
	message("Hello, I am using Eva.")
{
	char *fname = (char *)malloc(5);
        fname[0] = 0xcb;
	fname[1] = 0xce;
	fname[2] = 0xcc;
        fname[3] = 0xe5;
	fname[4] = 0x00;
	fontName = fname; // just set font name to "Song Ti" in Chinese
	free(fname);
}

QunSendIMPacket::QunSendIMPacket( const QunSendIMPacket & rhs )
	: QunPacket(rhs)
{
	encoding = rhs.getEncoding();  
	fontName = rhs.getFontName();
	
	red = rhs.getRed();
	green = rhs.getGreen();
	blue = rhs.getBlue();
		
	fontFlag = 0x00;
	bold = rhs.isBold(); 
	fontFlag |= bold ? 0x20 : 0x00; 
	
	italic = rhs.isItalic(); 
	fontFlag |= italic ? 0x40 : 0x00;
	
	underline = rhs.isUnderline();
	fontFlag |= underline ? 0x80 : 0x00;
	
	fontSize = rhs.getFontSize();
	fontFlag |= fontSize;
	
	message = rhs.getMessage();
}

QunSendIMPacket::~ QunSendIMPacket( )
{
}

QunSendIMPacket & QunSendIMPacket::operator =( const QunSendIMPacket & rhs )
{
	*((QunPacket *)this) = (QunPacket)rhs;
	encoding = rhs.getEncoding();  
	fontName = rhs.getFontName();
	
	red = rhs.getRed();
	green = rhs.getGreen();
	blue = rhs.getBlue();
		
	fontFlag = 0x00;
	bold = rhs.isBold(); 
	fontFlag |= bold ? 0x20 : 0x00; 
	
	italic = rhs.isItalic(); 
	fontFlag |= italic ? 0x40 : 0x00;
	
	underline = rhs.isUnderline();
	fontFlag |= underline ? 0x80 : 0x00;
	
	fontSize = rhs.getFontSize();
	fontFlag |= fontSize;
	
	message = rhs.getMessage();     
        return *this;
}

void QunSendIMPacket::setFontSize( char fontSize )
{
	this->fontSize = fontSize;
	fontSize &= 0x1F;
	fontFlag &= 0xE0;
	fontFlag |= fontSize;
}

void QunSendIMPacket::setUnderline( bool underline )
{
	this->underline = underline;
	fontFlag &= 0x7F;
	fontFlag |= underline ? 0x80 : 0x00;
}

void QunSendIMPacket::setItalic( bool italic )
{
	this->italic = italic;
	fontFlag &= 0xBF;
	fontFlag |= italic ? 0x40 : 0x00;
}

void QunSendIMPacket::setBold( bool bold )
{
	this->bold = bold;
	fontFlag &= 0xDF;
	fontFlag |= bold ? 0x20 : 0x00;
}

int QunSendIMPacket::putBody( unsigned char * buf )
{
	int pos=0;
	buf[pos++] = qunCommand; // qun command
	
	int tmp4 = htonl(qunID);
	memcpy(buf+pos, &tmp4, 4); // qun internal ID
	pos+=4;
	
	int lenPos = pos;  // we record the index of the length of following contents, it takes 2 bytes
	pos+=2; 
	
	std::string str2send = EvaUtil::convertToSend(message);
	memcpy(buf+pos, str2send.c_str(), str2send.length());
	pos += str2send.length();
	
	//now we can set the length of message and font name;
	short tmp2 = htons((short)(str2send.length() + fontName.length() + 10));
	memcpy(buf + lenPos, &tmp2, 2);
	
	buf[pos++] = 0x20; //  a space
	buf[pos++] = 0x00;    //  C style string terminator
	
	buf[pos++] = fontFlag;
	buf[pos++] = red;
	buf[pos++] = green;
	buf[pos++] = blue;
	
	buf[pos++] = 0;
	
	short tmpEncoding = htons(encoding);  // encoding for text
	memcpy(buf+pos,&tmpEncoding, 2);
	pos+=2;
	
	int len = fontName.length();     // font name
	memcpy(buf+pos, fontName.c_str(), len);
        pos+=len;
        
	buf[pos++] = 0x0D;   // an Enter
	
	return pos;
}




/***************************************************************************************************************/

QunSearchPacket::QunSearchPacket()
	: QunPacket(QQ_QUN_CMD_SEARCH_QUN), 
	type(QQ_SEARCH_QUN_BY_ID)
{
}

QunSearchPacket::QunSearchPacket( int type )
	: QunPacket(QQ_QUN_CMD_SEARCH_QUN)
{
	this->type = type;
}

QunSearchPacket::QunSearchPacket( const QunSearchPacket & rhs )
	: QunPacket(rhs)
{
	*this = rhs;
}

QunSearchPacket & QunSearchPacket::operator =( const QunSearchPacket & rhs )
{
	*((QunPacket *)this) = (QunPacket)rhs;
	type = rhs.getSearchType();
	externalID = rhs.getExtID();
	return *this;
}

int QunSearchPacket::putBody( unsigned char * buf )
{
	int pos=0;
	buf[pos++] = qunCommand; // qun command
	buf[pos++] = type;       // qun type
	
	int tmp4;
	if(type == QQ_SEARCH_DEMO_QUN){
		memset(buf+pos, 0, 4);
	}else{
		tmp4 = htonl(externalID);
		memcpy(buf+pos, &tmp4, 4);
	}
	pos+=4;
	return pos;
	
}



/***************************************************************************************************************/
 
QunJoinPacket::QunJoinPacket(const unsigned int id)
	: QunPacket(QQ_QUN_CMD_JOIN_QUN, id)
{
}

QunJoinPacket::QunJoinPacket(const QunJoinPacket &rhs)
	: QunPacket(rhs)
{
}

QunJoinPacket::~QunJoinPacket()
{
}

int QunJoinPacket::putBody(unsigned char *buf)
{
	buf[0] = qunCommand;
	int tmp4 = htonl(qunID);
	memcpy(buf + 1, &tmp4, 4);
	return 5;
}



/***************************************************************************************************************/
 
QunModifyPacket::QunModifyPacket( const QunInfo & inf )
	: QunPacket( QQ_QUN_CMD_MODIFY_QUN_INFO, inf.getQunID())
{
	info = inf;
}

QunModifyPacket::QunModifyPacket( const QunModifyPacket & rhs )
	: QunPacket(rhs)
{
	info = rhs.getQunInfo();
}

QunModifyPacket & QunModifyPacket::operator =( const QunModifyPacket & rhs )
{
	*( (QunPacket*)this) = (QunPacket)rhs;
	info = rhs.getQunInfo();
	return *this;
}

int QunModifyPacket::putBody( unsigned char * buf )
{
	int pos=0;
	
	buf[pos++] = qunCommand;
	
	int tmp4 = htonl(info.getQunID());
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;
	
	buf[pos++] = 0x01;  // dose this byte means permenant Qun or some thing?
	
	buf[pos++] = info.getAuthType();
	
	memset(buf+pos, 0, 6);  // 6 bytes 0x00 s
	pos+=6;
	
	unsigned short tmp2;
	tmp2 = htons(info.getCategory());
	memcpy(buf+pos, &tmp2, 2);
	pos+=2;
	
	buf[pos++] = info.getName().length() & 0xff;
	
	memcpy(buf+pos, info.getName().c_str(), buf[pos - 1]);
	pos+= buf[pos - 1];
	
	memset(buf+pos, 0, 2);
	pos+=2;
	
	buf[pos++] = info.getNotice().length() & 0xff;
	memcpy(buf+pos, info.getNotice().c_str(), buf[pos - 1]);
	pos+= buf[pos - 1];
	
	buf[pos++] = info.getDescription().length() & 0xff;
	memcpy(buf+pos, info.getDescription().c_str(), buf[pos - 1]);
	pos+= buf[pos - 1];
	
	return pos;
}



/***************************************************************************************************************/
 

QunModifyCardPacket::QunModifyCardPacket( const unsigned int qunID, const unsigned int qqID )
	: QunPacket(QQ_QUN_CMD_MODIFY_CARD, qunID), qqNum(qqID)
{
	m_AllowAdminChange = 0x02;// admin cannot change my Qun card
}

QunModifyCardPacket::QunModifyCardPacket( const QunModifyCardPacket & rhs )
	: QunPacket(rhs)
{
	*this = rhs;
}

QunModifyCardPacket & QunModifyCardPacket::operator =( const QunModifyCardPacket & rhs )
{
	*((QunPacket *)this) = (QunPacket)rhs;
	qqNum = rhs.getQQ();
	m_AllowAdminChange = rhs.getAllowAdminChange();
	name = rhs.getName();
	gender = rhs.getGender();
	phone = rhs.getPhone();
	email = rhs.getEmail();
	memo  = rhs.getMemo();
	return *this;
}

int QunModifyCardPacket::putBody( unsigned char * buf )
{
	int pos=0;
	
	buf[pos++] = qunCommand;
	
	int tmp4 = htonl(qunID);
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;
	
	tmp4 = htonl(qqNum);
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;

	memset(buf+pos, 0, 3);	
	pos+=3;

	buf[pos++] = m_AllowAdminChange;

	buf[pos++] = name.length() & 0xff;
	memcpy(buf+pos, name.c_str(), buf[pos-1]);
	pos+= buf[pos-1];
	
	buf[pos++]=gender;
	
	buf[pos++] = phone.length() & 0xff;
	memcpy(buf+pos, phone.c_str(), buf[pos-1]);
	pos+= buf[pos-1];
	
	buf[pos++] = email.length() & 0xff;
	memcpy(buf+pos, email.c_str(), buf[pos-1]);
	pos+= buf[pos-1];

	buf[pos++] = memo.length() & 0xff;
	memcpy(buf+pos, memo.c_str(), buf[pos-1]);
	pos+= buf[pos-1];
	
	return pos;
}



/***************************************************************************************************************/
 

QunAdminOpPacket::QunAdminOpPacket( const int id, const int qqID, const bool isSetAdmin )
	: QunPacket(QQ_QUN_CMD_ADMIN, id)
{
	qqNum = qqID;
	if(isSetAdmin)
		action = QQ_QUN_SET_ADMIN;
	else
		action = QQ_QUN_UNSET_ADMIN;
}

QunAdminOpPacket::QunAdminOpPacket( const QunAdminOpPacket & rhs )
	: QunPacket(rhs)
{
	*this = rhs;
}

QunAdminOpPacket & QunAdminOpPacket::operator =( const QunAdminOpPacket & rhs )
{
	*((QunPacket *)this) = (QunPacket)rhs;
	qqNum = rhs.getQQ();
	action = rhs.getActionCode();
	return *this;
}

int QunAdminOpPacket::putBody( unsigned char * buf )
{
	int pos=0;
	
	buf[pos++] = qunCommand;
	
	int tmp4 = htonl(qunID);
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;
	
	tmp4 = htonl(qqNum);
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;
	
	buf[pos++] = action;
	
	return pos;
}



/***************************************************************************************************************/


QunTransferPacket::QunTransferPacket( const unsigned int id, const unsigned int qqID )
	: QunPacket(QQ_QUN_CMD_TRANSFER, id), qqNum(qqID)
{
}

QunTransferPacket::QunTransferPacket( const QunTransferPacket & rhs )
	: QunPacket(rhs)
{
	*this = rhs;
}

QunTransferPacket & QunTransferPacket::operator =( const QunTransferPacket & rhs )
{
	*((QunPacket *)this) = (QunPacket)rhs;
	qqNum = rhs.getQQ();
	return *this;
}

int QunTransferPacket::putBody( unsigned char * buf )
{
	int pos=0;
	
	buf[pos++] = qunCommand;
	
	int tmp4 = htonl(qunID);
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;
	
	tmp4 = htonl(qqNum);
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;
	return pos;
}


/***************************************************************************************************************/


QunModifyMemberPacket::QunModifyMemberPacket( const unsigned int id, const bool isAdd )
	: QunPacket(QQ_QUN_CMD_MODIFY_MEMBER, id), mIsAdd(isAdd)
{
}

QunModifyMemberPacket::QunModifyMemberPacket( const QunModifyMemberPacket & rhs )
	: QunPacket(rhs)
{
	*this = rhs;
}

QunModifyMemberPacket &QunModifyMemberPacket::operator=(const QunModifyMemberPacket &rhs)
{
	*((QunPacket *)this) = (QunPacket)rhs;
	mIsAdd = rhs.isAdd();
	members = rhs.getMembers();
	return *this;	
}

int QunModifyMemberPacket::putBody( unsigned char * buf )
{
	int pos=0;
	
	buf[pos++] = qunCommand;
	
	unsigned int tmp4 = htonl(qunID);
	memcpy(buf + pos, &tmp4, 4);
	pos+=4;
	
	if(mIsAdd)
		buf[pos++] = QQ_QUN_MEMBER_ADD;
	else
		buf[pos++] = QQ_QUN_MEMBER_REMOVE;
	
	std::list<unsigned int>::iterator iter;
	for(iter=members.begin(); iter!= members.end(); ++iter){
		tmp4 = htonl(*iter);
		memcpy(buf + pos, &tmp4, 4);
		pos+=4;
		
	}
	return pos;
}


/***************************************************************************************************************/

short QunSendIMExPacket::messageID = 0;

QunSendIMExPacket::QunSendIMExPacket( const unsigned int id )
	: QunSendIMPacket(id)
{
	qunCommand = QQ_QUN_CMD_SEND_IM_EX;
	numFragments = 1;
	seqFragments = 0;
	messageID++;
}

QunSendIMExPacket::QunSendIMExPacket(const QunSendIMExPacket &rhs)
	: QunSendIMPacket(rhs)
{
	*this = rhs;
}

QunSendIMExPacket & QunSendIMExPacket::operator =( const QunSendIMExPacket & rhs )
{
	*((QunSendIMPacket *)this) = (QunSendIMPacket)rhs;
	numFragments = rhs.getNumFragments();
	seqFragments = rhs.getSeqOfFragments();
	messageID = rhs.getMessageID();
	return *this;
}

int QunSendIMExPacket::putBody( unsigned char * buf )
{
	int pos=0;
	buf[pos++] = qunCommand; // qun command
	
	unsigned int tmp4 = htonl(qunID);
	memcpy(buf+pos, &tmp4, 4); // qun internal ID
	pos+=4;
	
	int lenPos = pos;  // we record the index of the length of following contents, it takes 2 bytes
	pos+=2; 
	
	unsigned short tmp2 = htons(2); // luma said 0001 means pure text, 0002 represents text with customized picture
	memcpy(buf+pos, &tmp2, 2); pos+=2;
	
	buf[pos++] = numFragments;
	buf[pos++] = seqFragments;
	
	tmp2 = htons(messageID);
	memcpy(buf+pos, &tmp2, 2); pos+=2;
	
	memset(buf+pos, 0, 4); pos+=4;  // 4 unknown bytes
	
	std::string str2send = EvaUtil::convertToSend(message);
	memcpy(buf+pos, str2send.c_str(), str2send.length());
	pos += str2send.length();
	
	if(numFragments == seqFragments + 1){
		//now we can set the length of message and font name;
		unsigned short tmp2 = htons((short)(pos - lenPos -2 + fontName.length() + 10));
		memcpy(buf + lenPos, &tmp2, 2);
		
		buf[pos++] = 0x20; //  a space
		buf[pos++] = 0x00;    //  C style string terminator
		
		buf[pos++] = fontFlag;
		buf[pos++] = red;
		buf[pos++] = green;
		buf[pos++] = blue;
		
		buf[pos++] = 0;
		
		unsigned short tmpEncoding = htons(encoding);  // encoding for text
		memcpy(buf+pos,&tmpEncoding, 2);
		pos+=2;
		
		int len = fontName.length();     // font name
		memcpy(buf+pos, fontName.c_str(), len);
		pos+=len;
		
		buf[pos++] = (unsigned char)(len + 9); 
	}else{
		short tmp2 = htons((short)(10 + str2send.length()));
		memcpy(buf + lenPos, &tmp2, 2); pos+=2;
	}
// 	printf("before encrypted\n");
// 	for(int i=0; i<pos; i++){
// 		if(!(i%8)) printf("\n%d: ",i);
// 		char t = buf[i];
// 		printf("%2x ", (uint8_t)t);
// 	}
// 	printf("\n");
	return pos;
}



/***************************************************************************************************************/

QunRequestCardPacket::QunRequestCardPacket( const unsigned int qunID, const unsigned int qqID )
	: QunPacket(QQ_QUN_CMD_REQUEST_CARD, qunID), qqNum(qqID)
{
}

QunRequestCardPacket::QunRequestCardPacket( const QunRequestCardPacket & rhs )
	: QunPacket(rhs)
{
	*this = rhs;
}

QunRequestCardPacket & QunRequestCardPacket::operator =( const QunRequestCardPacket & rhs )
{
	*((QunPacket *)this) = (QunPacket)rhs;
	qqNum = rhs.getQQ();
	return *this;
}

int QunRequestCardPacket::putBody( unsigned char * buf )
{
	int pos=0;
	
	buf[pos++] = qunCommand;
	
	unsigned int tmp4 = htonl(qunID);
	memcpy(buf + pos, &tmp4, 4); pos+=4;
	
	tmp4 = htonl(qqNum);
	memcpy(buf + pos, &tmp4, 4); pos+=4;
	
	return pos;
}



/***************************************************************************************************************/

QunExitPacket::QunExitPacket( const unsigned int id )
	: QunPacket(QQ_QUN_CMD_EXIT_QUN, id)
{
}

QunExitPacket::QunExitPacket( const QunExitPacket & rhs )
	: QunPacket(rhs)
{
	*this = rhs;
}

QunExitPacket & QunExitPacket::operator =( const QunExitPacket & rhs )
{
	*((QunPacket *)this) = (QunPacket)rhs;
	return *this;
}

int QunExitPacket::putBody( unsigned char * buf )
{
	int pos=0;
	
	buf[pos++] = qunCommand;
	
	unsigned int tmp4 = htonl(qunID);
	memcpy(buf + pos, &tmp4, 4); pos+=4;
	
	return pos;
}



/***************************************************************************************************************/

QunCreatePacket::QunCreatePacket( const std::string &name, const unsigned short category, 
				const unsigned char auth)
	: QunPacket(QQ_QUN_CMD_CREATE_QUN), qunAuth(auth), qunCategory(category),
	 qunName(name), qunNotice(""), qunDescription("")
{
}

QunCreatePacket::QunCreatePacket( const QunCreatePacket & rhs )
	: QunPacket(rhs)
{
	*this = rhs;	
}

QunCreatePacket & QunCreatePacket::operator =( const QunCreatePacket & rhs )
{
	*((QunPacket *)this) = (QunPacket)rhs;
	qunName = rhs.getName();
	qunCategory = rhs.getCategory();
	qunAuth = rhs.getAuth();
	qunNotice = rhs.getNotice();
	qunDescription = rhs.getDescription();
	qunMembers = rhs.getMembers();
	return *this;
}

int QunCreatePacket::putBody( unsigned char * buf )
{
	int pos=0;
	
	buf[pos++] = qunCommand;
	
	buf[pos++] = QQ_QUN_TYPE_PERMANENT; 
	
	buf[pos++] = qunAuth;
	
	memset(buf+pos, 0, 6);  // 6 bytes 0x00 s
	pos+=6;
	
	unsigned short tmp2;
	tmp2 = htons(qunCategory);
	memcpy(buf+pos, &tmp2, 2);
	pos+=2;
	
	buf[pos++] = qunName.length() & 0xff;
	
	memcpy(buf+pos, qunName.c_str(), buf[pos - 1]);
	pos+= buf[pos - 1];
	
	memset(buf+pos, 0, 2);
	pos+=2;
	
	buf[pos++] = qunNotice.length() & 0xff;
	memcpy(buf+pos, qunNotice.c_str(), buf[pos - 1]);
	pos+= buf[pos - 1];
	
	buf[pos++] = qunDescription.length() & 0xff;
	memcpy(buf+pos, qunDescription.c_str(), buf[pos - 1]);
	pos+= buf[pos - 1];
	
	unsigned int tmp4;
	std::list<unsigned int>::iterator iter;
	for(iter=qunMembers.begin(); iter!= qunMembers.end(); ++iter){
		if(*iter == getQQ()) continue;
		tmp4 = htonl(*iter);
		memcpy(buf + pos, &tmp4, 4);
		pos+=4;
		
	}	
	return pos;
}



/***************************************************************************************************************/

QunActivePacket::QunActivePacket( const unsigned int id )
	: QunPacket(QQ_QUN_CMD_ACTIVATE_QUN, id)
{
}

QunActivePacket::QunActivePacket( const QunActivePacket & rhs )
	: QunPacket(rhs)
{
	*this = rhs;
}

QunActivePacket & QunActivePacket::operator =( const QunActivePacket & rhs )
{
	*((QunPacket *)this) = (QunPacket)rhs;
	return *this;
}

int QunActivePacket::putBody( unsigned char * buf )
{
	int pos=0;
	
	buf[pos++] = qunCommand;
	
	unsigned int tmp4 = htonl(qunID);
	memcpy(buf + pos, &tmp4, 4); pos+=4;
	
	return pos;
}



/***************************************************************************************************************/

QunRequestAllRealNames::QunRequestAllRealNames( const unsigned int id )
	: QunPacket(QQ_QUN_CMD_REQUEST_ALL_REALNAMES, id),
	m_StartIndex(0)
{
}

QunRequestAllRealNames::QunRequestAllRealNames( const QunRequestAllRealNames & rhs )
	: QunPacket(rhs),
	m_StartIndex(0)
{
	*this = rhs;
}

QunRequestAllRealNames & QunRequestAllRealNames::operator =( const QunRequestAllRealNames & rhs )
{
	*((QunPacket *)this) = (QunPacket)rhs;
	m_StartIndex = rhs.getStartIndex();
	return *this;
}

int QunRequestAllRealNames::putBody( unsigned char * buf )
{
	int pos=0;
	
	buf[pos++] = qunCommand;
	
	pos+=EvaUtil::write32( buf + pos, qunID);
	pos+=EvaUtil::write32( buf + pos, 0);
	pos+=EvaUtil::write32( buf + pos, m_StartIndex);
	
	return pos;
}



