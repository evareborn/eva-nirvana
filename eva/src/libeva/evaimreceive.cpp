/***************************************************************************
 *   Copyright (C) 2004- 2006 by yunfan                                          *
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
#include "evaimreceive.h"
#include "evadefines.h"
#include "evautil.h"
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif


ReceiveIMPacket::ReceiveIMPacket(unsigned char *buf, const int len)
	: InPacket(buf, len),
	bodyOffset(0)
{
}   

ReceiveIMPacket::ReceiveIMPacket(const ReceiveIMPacket &rhs)
	: InPacket(rhs)
{
	memcpy(replyKey, rhs.getReplyKey(), REPLY_KEY_LENGTH);
	sender = rhs.getSender();
	receiver = rhs.getReceiver();
	sequence = rhs.getSequence();
	senderIP = rhs.getSenderIP();
	senderPort = rhs.getSenderPort();
	type = rhs.getIMType();
	
	bodyOffset = rhs.getLength() - rhs.getBodyLength();
}

std::string ReceiveIMPacket::convertToShow(const std::string &src, const unsigned char type)
{
	std::string converted = "";
	unsigned int start = 0;
	char *uuid = NULL; // uuid format: {12345678-1234-1234-1234-123456789ABC}
	if(type == QQ_IM_IMAGE_REPLY){
		uuid = new char[39]; // 38 + 1
		memcpy(uuid, src.c_str(), 38);
		uuid[38] = 0x00; // just make it as a string
		start+=38;
		start+=2; // 2 unknown bytes: 0x13 0x4c (always)
	}
	for(uint i=start; i<src.length(); i++){
		if(src[i]==0x14){
			converted+=EvaUtil::smileyToText(src[++i]);
			converted+=' ';
		}else if(src[i]==0x15){
				int t=0;
				char *tmp = new char[src.length()+1];
				strcpy(tmp, src.c_str());
				converted+=EvaUtil::customSmileyToText(tmp + i, &t, uuid);
				i+=(t-1); // get rid of 0x15
				delete tmp;
			} else
				converted+=src[i];
	}
	if(uuid) delete []uuid;
	return converted;
}

ReceiveIMPacket &ReceiveIMPacket::operator=(const ReceiveIMPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	memcpy(replyKey, rhs.getReplyKey(), REPLY_KEY_LENGTH);
	sender = rhs.getSender();
	receiver = rhs.getReceiver();
	sequence = rhs.getSequence();
	senderIP = rhs.getSenderIP();
	senderPort = rhs.getSenderPort();
	type = rhs.getIMType();
	
	bodyOffset = rhs.getLength() - rhs.getBodyLength();
        return *this;
}

void ReceiveIMPacket::parseBody()
{
	if(bodyLength < 16){
		printf("message too short, just chuck it\n");  
		bodyOffset = -1; // means error occurred
		return;
	}
	memcpy(replyKey, decryptedBuf, REPLY_KEY_LENGTH);
	bodyOffset = readHeader(decryptedBuf);
}

int ReceiveIMPacket::readHeader(const unsigned char *buf)
{
	int pos=0;
	sender = READ32(buf);
	pos+=4;

	receiver = READ32(buf+pos);
	pos+=4;

	intSequence = READ32(buf+pos);
	pos+=4;

	senderIP = READ32(buf+pos);
	pos+=4;

	senderPort = READ16(buf+pos);
	pos+=2;

	type = READ16(buf+pos);
	pos+=2;	
	return pos;
}

/* =========================================================== */

NormalIMBase::NormalIMBase(const unsigned char *buf, const int len)
{
	bodyBuf = (unsigned char *)malloc(len * sizeof(unsigned char));
	bodyLength = len;
	memcpy(bodyBuf, buf, len);
}

NormalIMBase::NormalIMBase(const NormalIMBase &rhs)
{
	*this = rhs;
}

NormalIMBase::~NormalIMBase()
{
	free(bodyBuf);
}

void NormalIMBase::setNormalIMBase(const NormalIMBase *base)
{
	*this = *base;	
}

void NormalIMBase::parseData()
{
	int len = readHeader(bodyBuf);
	parseContents(bodyBuf + len, bodyLength - len);
}

NormalIMBase &NormalIMBase::operator=(const NormalIMBase &rhs)
{
	bodyLength = rhs.getBodyLength();
	bodyBuf = (unsigned char *)malloc(bodyLength * sizeof(unsigned char));
	memcpy(bodyBuf, rhs.getBodyData(), bodyLength);
	
	senderVersion = rhs.getSenderVersion();
	sender = rhs.getSender();
	receiver = rhs.getReceiver();
	memcpy(fileSessionKey, rhs.getBuddyFileSessionKey(), 16);
	type = rhs.getNormalIMType();
	sequence = rhs.getSequence();
	sendTime = rhs.getSendTime();
	unknown1 = rhs.getUnknown1();
	senderFace = rhs.getSenderFace();
        return *this;
}

void NormalIMBase::parseContents(const unsigned char *, const int )
{
	//fprintf(stderr, "In NormalIMBase: buf[0]=0x%2x, len=%d\n",buf[0], len); 
}

int NormalIMBase::readHeader(const unsigned char *buf)
{
	int pos=0;

	senderVersion = READ16(buf+pos);
	pos+=2;

	sender = READ32(buf+pos);
	pos+=4;

	receiver = READ32(buf+pos);
	pos+=4;
	
	memcpy(fileSessionKey, buf+pos, 16);
	pos+=16;

	type = READ16(buf+pos);
	pos+=2;

	sequence = READ16(buf+pos);
	pos+=2;
	
	sendTime = READ32(buf+pos);
	pos+=4;

	senderFace = READ16(buf+pos);
	pos+=2;
	
	return pos;
}

/* =========================================================== */

ReceivedNormalIM::ReceivedNormalIM(const unsigned char *buf, const int len)
	: NormalIMBase(buf, len)
{
}

ReceivedNormalIM::ReceivedNormalIM(const ReceivedNormalIM &rhs)
	: NormalIMBase(rhs)
{
	memcpy(unknown2, rhs.getUnknown2(), 3);
	numFragments = rhs.getNumFragments();
	seqFragments = rhs.getSeqOfFragments();
	messageID = rhs.getMessageID();
	replyType = rhs.getReplyType();
	message = rhs.getMessage();
	mHasFontAttribute = rhs.hasFontAttribute();
	encoding = rhs.getEncoding();
	red = rhs.getRed();
	green = rhs.getGreen();
	blue = rhs.getBlue();
	fontSize = rhs.getFontSize();
	fontName = rhs.getFontName();
	bold = rhs.isBold();
	italic = rhs.isItalic();
	underline = rhs.isUnderline();
}

const bool ReceivedNormalIM::isNormalReply() const
{
	return replyType == QQ_IM_NORMAL_REPLY;
}

ReceivedNormalIM &ReceivedNormalIM::operator=(const ReceivedNormalIM &rhs)
{
	*((NormalIMBase *)this) = (NormalIMBase)rhs;
	memcpy(unknown2, rhs.getUnknown2(), 3);
	numFragments = rhs.getNumFragments();
	seqFragments = rhs.getSeqOfFragments();
	messageID = rhs.getMessageID();
	replyType = rhs.getReplyType();
	message = rhs.getMessage();
	mHasFontAttribute = rhs.hasFontAttribute();
	encoding = rhs.getEncoding();
	red = rhs.getRed();
	green = rhs.getGreen();
	blue = rhs.getBlue();
	fontSize = rhs.getFontSize();
	fontName = rhs.getFontName();
	bold = rhs.isBold();
	italic = rhs.isItalic();
	underline = rhs.isUnderline();
        return *this;
}

void ReceivedNormalIM::parseContents(const unsigned char *buf, const int len)
{
	int pos=0;
	memcpy(unknown2, buf, 3);
        pos+=3;

        mHasFontAttribute = (buf[pos++] != 0);
	pos+=8; /// unknown 8 bytes, all 0x00s
	numFragments = buf[pos++];
	seqFragments = buf[pos++];
	memcpy(&messageID, buf+pos, 2);
	pos+=2;

        replyType = buf[pos++];
			
	int i=0;
	while( (buf[pos++]) != 0x00){
		i++;
	}
#ifdef WIN32
	char* str; str=(char*)_alloca(i+2);
#else
	char str[i+2];
#endif
	memcpy(str, buf+(pos-i-1), i+1);
	str[i+1]= 0x00;
	message = ReceiveIMPacket::convertToShow(str, replyType);
	
        if(mHasFontAttribute) {
		if(pos<len) {
#ifdef WIN32
			char* fa; fa=(char*)_alloca(len-pos);
#else
			char fa[len-pos];
#endif
			memcpy(fa, buf+pos, len-pos);
			fontSize = fa[0] & 0x1F;
			bold = ((fa[0] & 0x20) != 0);
			italic = ((fa[0] & 0x40) != 0);
			underline = ((fa[0] & 0x80) != 0);
			red = fa[1] & 0xFF;
			green = fa[2] & 0xFF;
			blue = fa[3] & 0xFF;
			short tmp2;
			memcpy(&tmp2, fa+5, 2);
			encoding = ntohs(tmp2);
#ifdef WIN32
			char* fn; fn=(char*)_alloca(len-pos-7);
#else
			char fn[len-pos-7];
#endif
			memcpy(fn, fa+7, len-pos-7-1);
			fn[len-pos-7-1]=0x00;
			//fn[len-pos-7]=0x00;
			fontName.assign(fn);
		} else
			mHasFontAttribute = false;
        }
}

/* =========================================================== */

ReceivedSystemIM::ReceivedSystemIM(const unsigned char *buf, const int len)
{
	parseData(buf, len);
}

ReceivedSystemIM::ReceivedSystemIM( const ReceivedSystemIM &rhs)
{
	*this = rhs;
}

ReceivedSystemIM &ReceivedSystemIM::operator=(const ReceivedSystemIM &rhs)
{
	systemIMType = rhs.getSystemIMType();
	message = rhs.getMessage();
        return *this;
}

void ReceivedSystemIM::parseData(const unsigned char *buf, const int /*len*/)
{
	int pos = 0;
	systemIMType = buf[pos++];
	
	char msgLen = buf[pos++];
#ifdef WIN32
	char* msg; msg=(char*)_alloca(msgLen+1);
#else
	char msg[msgLen+1];
#endif
	memcpy(msg, buf+pos, msgLen);
	msg[0xff & msgLen]=0x00;
	message.assign(msg);
}

/* =========================================================== */

ReceiveIMReplyPacket::ReceiveIMReplyPacket(const char *key) 
	: OutPacket(QQ_CMD_RECV_IM, false)
{
        memcpy(replyKey, key, 16);
}

ReceiveIMReplyPacket::ReceiveIMReplyPacket(const ReceiveIMReplyPacket &rhs)
	: OutPacket(rhs)
{
	memcpy(replyKey, rhs.getReplyKey(), REPLY_KEY_LENGTH);
}

ReceiveIMReplyPacket &ReceiveIMReplyPacket::operator=(const ReceiveIMReplyPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	memcpy(replyKey, rhs.getReplyKey(), REPLY_KEY_LENGTH);
        return *this;
}

int ReceiveIMReplyPacket::putBody(unsigned char *buf) {

        memcpy(buf, replyKey, REPLY_KEY_LENGTH);
	return REPLY_KEY_LENGTH;
}

/* =========================================================== */

ReceivedQunIM::ReceivedQunIM(const unsigned short src,  const unsigned char * buf, const int len )
	: source(src)
{
	parseData(buf, len);
}

ReceivedQunIM::ReceivedQunIM( const ReceivedQunIM & rhs )
{
	(*this) = rhs;
}

ReceivedQunIM & ReceivedQunIM::operator =( const ReceivedQunIM & rhs )
{
	source = rhs.getSource(); // this is to mention,  permenent, temporary qun 
	externalID = rhs.getExtID();
	qunID = rhs.getQunID();
	type = rhs.getType();
	sender = rhs.getSenderQQ();
	unknown1 = rhs.getUnknown1();
	numFragments = rhs.getNumFragments();
	seqFragments = rhs.getSeqOfFragments();
	messageID = rhs.getMessageID();
	sequence = rhs.getSequence();
	sentTime = rhs.getSentTime();
	versionID = rhs.getVersionID();
	message = rhs.getMessage();
	
	mHasFontAttribute = rhs.hasFontAttribute();
	encoding = rhs.getEncoding();
	red = rhs.getRed();
	green = rhs.getGreen();
	blue = rhs.getBlue();
	fontSize = rhs.getFontSize();
	fontName = rhs.getFontName();
	bold = rhs.isBold();
	italic = rhs.isItalic();
	underline = rhs.isUnderline();
	return *this;
}

void ReceivedQunIM::parseData( const unsigned char * buf, const int len )
{
// 			printf("ReceivedQunIM::parseData\n");
// 			for(int i=0; i<len; i++){
// 				if(!(i%8)) printf("\n%d: ",i);
// 				char t = buf[i];
// 				printf("%2x ", (uint8_t)t);
// 			}
// 			printf("\n");
	int pos=0;

	externalID = ntohl(*(int*)buf); // the external Qun ID
	pos+=4;
	
	type = buf[pos++];  // type of the Qun

	if(source == QQ_RECV_IM_TEMP_QUN_IM){ // internal ID of temporary Qun
		qunID = ntohl( *(int*)(buf+pos) );
		pos+=4;
	}
	
	sender = ntohl(*(int*)(buf+pos) );       // the sender
	pos+=4;
	
	memcpy(&unknown1, buf+pos, 2);
	pos+=2;

	sequence = ntohs(*(short*)(buf+pos) );     // message sequence number
	pos+=2;
	
	sentTime = ntohl(*(int*)(buf+pos) );    // the sending time
	pos+=4;
	
	versionID = ntohl(*(int*)(buf+pos) );   // version ID is to tell if something like memebers, or rules of this Qun were changed
        pos+=4;

        pos+=2; // ignore 2 bytes;  Luma QQ says it's the length for the following data.

	// 10 unknown bytes
	if(source != QQ_RECV_IM_UNKNOWN_QUN_IM) {
		pos+=2;
		
		numFragments = buf[pos++];
		seqFragments = buf[pos++];
		memcpy(&messageID, buf+pos, 2);
		
		pos+=6;
	}
	
	int i=0;

	//       not scan \0 but to extract the length from the packet
	if (numFragments != (seqFragments + 1))
	{
		i = len - pos - 1;
		pos = len;
	} else {
		i = buf[len-1];
		i = len - i - pos;
		pos = pos + i + 1;
	}
#ifdef WIN32
	char* str; str=(char*)_alloca(i+2);
#else
	char str[i+2];
#endif
	memcpy(str, buf+(pos-i-1), i+1);
	str[i+1]= 0x00;
	message = ReceiveIMPacket::convertToShow(str, QQ_IM_NORMAL_REPLY);
	
	mHasFontAttribute = (len > pos);
        if(mHasFontAttribute) {
#ifdef WIN32
		char* fa; fa=(char*)_alloca(len-pos);
#else
		char fa[len-pos];
#endif
		memcpy(fa, buf+pos, len-pos);
		fontSize = fa[0] & 0x1F;
		bold = ((fa[0] & 0x20) != 0);
		italic = ((fa[0] & 0x40) != 0);
		underline = ((fa[0] & 0x80) != 0);
		red = fa[1] & 0xFF;
		green = fa[2] & 0xFF;
		blue = fa[3] & 0xFF;
		encoding = ntohs(*(short*)(fa+5) );
#ifdef WIN32
		char* fn; fn=(char*)_alloca(len-pos-7);
#else
		char fn[len-pos-7];
#endif
		memcpy(fn, fa+7, len-pos-7-1);
		fn[len-pos-7-1]=0x00;
		fontName.assign(fn);
        } 
}



/* =========================================================== */

ReceivedQunIMJoinRequest::ReceivedQunIMJoinRequest()
	: externalID(0),
	sender(0),
	type(0),
	message(""),
	commander(0),
	m_Code(0),
	m_CodeLen(0),
	m_Token(0),
	m_TokenLen(0)
{
}

// many thanks to starkwong. He found the change of this bit in QQ2006
ReceivedQunIMJoinRequest::ReceivedQunIMJoinRequest(const unsigned short imType, const unsigned char * buf, const int len)
	: externalID(0),
	sender(0),
	type(0),
	message(""),
	commander(0),
	m_Code(0),
	m_CodeLen(0),
	m_Token(0),
	m_TokenLen(0)
{
	printf("ReceivedQunIMJoinRequest\n");
	for(int i=0; i<len; i++){
		if(!(i%8)) printf("\n%d: ", i);
		printf("%2x ", (uint8_t)buf[i]);
	}
	printf("\n\n");

	int pos=0;
	unsigned char exttype=0;
	externalID = ntohl(*(unsigned int*)buf);
	pos+=4;
	
	type = buf[pos++];


	switch(imType){
		case QQ_RECV_IM_ADDED_TO_QUN:
		case QQ_RECV_IM_DELETED_FROM_QUN:
			sender = READ32(buf+pos);
			pos+=4;
			exttype=buf[pos++];
			pos++;
			if(exttype == 3) {
				commander = READ32(buf+pos);
				pos+=4;
			}
			break;
		case QQ_RECV_IM_REQUEST_JOIN_QUN:
			sender = READ32(buf+pos);
			pos+=4;
			break;
		case QQ_RECV_IM_APPROVE_JOIN_QUN:
		case QQ_RECV_IM_REJECT_JOIN_QUN:
			commander = READ32(buf+pos);
			pos+=4;
			break;
		case QQ_RECV_IM_SET_QUN_ADMIN:
			commander = buf[pos++]; // just use the commander variable to save the action. 0:unset, 1:set
			sender = READ32(buf + pos); // sender is the persion which the action performed on
			pos += 4;
			pos++;// unknonw, probably the admin value of the above member
			break;
		case QQ_RECV_IM_CREATE_QUN:
		default:
			sender = READ32(buf+pos);
			pos+=4;
			break;
	}

	if(imType != QQ_RECV_IM_DELETED_FROM_QUN || exttype == 3){
		if(len == pos) {
			message = "";
			return;
		}
		unsigned length = (unsigned int)buf[pos++];
		char *msg = (char *)malloc((length+1) * sizeof(char));
		memcpy(msg, buf + pos, length);
		msg[length] = 0x00;
		message.assign(msg);
		free(msg);
		pos += length;
	}

	m_CodeLen = READ16(buf + pos);
	pos +=2;

	setCode(buf+pos, m_CodeLen);
	pos += m_CodeLen;

	if(imType == QQ_RECV_IM_REQUEST_JOIN_QUN){
		m_TokenLen = READ16(buf + pos);
		pos +=2;
	
		setToken(buf+pos, m_TokenLen);
		pos += m_TokenLen;
	}
}

ReceivedQunIMJoinRequest::ReceivedQunIMJoinRequest( const ReceivedQunIMJoinRequest & rhs )
{
	*this = rhs;
}

ReceivedQunIMJoinRequest::~ReceivedQunIMJoinRequest()
{
	if(m_Code)
		delete []m_Code;
	if(m_Token)
		delete []m_Token;
}

ReceivedQunIMJoinRequest & ReceivedQunIMJoinRequest::operator =( const ReceivedQunIMJoinRequest & rhs )
{
	externalID = rhs.getExtID();
	sender = rhs.getSender();
	type = rhs.getType();
	message = rhs.getMessage();
	commander = rhs.getCommander();
	setCode(rhs.getCode(), rhs.getCodeLength());
	setToken(rhs.getToken(), rhs.getTokenLength());
	return *this;
}

void ReceivedQunIMJoinRequest::setCode(const unsigned char *code, const unsigned short len)
{
	if(code && len){
		if(m_Code) delete []m_Code;
		m_CodeLen = len;
		m_Code = new unsigned char [m_CodeLen];
		memcpy(m_Code, code, m_CodeLen);
	}
}

void ReceivedQunIMJoinRequest::setToken(const unsigned char *token, const unsigned short len)
{
	if(token && len){
		if(m_Token) delete []m_Token;
		m_TokenLen = len;
		m_Token = new unsigned char [m_TokenLen];
		memcpy(m_Token, token, m_TokenLen);
	}
}

/* =========================================================== */

SignatureChangedPacket::SignatureChangedPacket( const unsigned char * buf, const int len )
{
	parseData(buf, len);
}

SignatureChangedPacket::SignatureChangedPacket( const SignatureChangedPacket & rhs )
{
	*this = rhs;
}

SignatureChangedPacket & SignatureChangedPacket::operator =( const SignatureChangedPacket & rhs )
{
	qq = rhs.getQQ();
	time = rhs.getTime();
	signature = rhs.getSignature();
	return *this;
}

void SignatureChangedPacket::parseData( const unsigned char * buf, const int /*len*/ )
{
	int pos=0;
	qq = READ32(buf); pos+=4; // your buddy qq number
	

	time = READ32(buf+pos);  pos+=4; // the time he/she changed his/her signature
	
	unsigned char tmp = buf[pos++]; // the length of signature
	
	char *str = new char[tmp+1];
	memcpy(str, buf+pos, tmp);   // we get the new signature
	str[tmp] = 0x00;
	signature.assign(str);
	delete []str;
}



/* =========================================================== */


ReceivedFileIM::ReceivedFileIM( const unsigned char * buf, const int len )
	: NormalIMBase(buf, len)
	, m_FileName("")
	, m_FileSize(0)
{
}

ReceivedFileIM::ReceivedFileIM( const ReceivedFileIM & rhs )
	: NormalIMBase(rhs)
{
	*this = rhs;
}

ReceivedFileIM & ReceivedFileIM::operator =( const ReceivedFileIM & rhs )
{
	*((NormalIMBase *)this) = (NormalIMBase)rhs;
	m_TransferType = rhs.getTransferType();
	m_ConnectMode = rhs.getConnectMode();
	m_SessionId = getSessionId();
	m_WanIp = rhs.getWanIp();
	m_WanPort = rhs.getWanPort();
	//m_MajorPort = rhs.getMajorPort();
	//m_LanIp = rhs.getLanIp();
	//m_LanPort = rhs.getLanPort();

	memcpy(m_AgentServerKey, rhs.getAgentServerKey(), 16);
	m_FileName = rhs.getFileName();
	m_FileSize = rhs.getFileSize();
	return *this;
}

void ReceivedFileIM::parseContents( const unsigned char * buf, const int len )
{
	int pos = 27; // ignore 27 bytes
	m_TransferType = buf[pos++];
	if(m_TransferType != QQ_TRANSFER_FILE && m_TransferType != QQ_TRANSFER_IMAGE) return;
	if(getNormalIMType() == QQ_IM_REQUEST_CANCELED) return;

	m_ConnectMode = buf[pos++];
	if(getNormalIMType() != QQ_IM_TCP_REQUEST){  
		// this part actually is for UDP only(maybe Request cancellation as well)
		pos++; // unknown bytes, 0x66 or 0x67
		if(getNormalIMType() != QQ_IM_EX_REQUEST_CANCELLED)
			pos+=2; // unknown 2 bytes, 0x0000
		pos+=4; // unknown 4 bytes, 0x00000001, or all 0x0s

//		pos+=2; // unknown 2 bytes, 0x0000
//		m_SessionId = EvaUtil::read16(buf+pos); pos+=2;
		m_SessionId = EvaUtil::read32(buf+pos); pos+=4;

		if(getNormalIMType() == QQ_IM_NOTIFY_FILE_AGENT_INFO)
			pos++; // 0x00, and all above after m_ConnectionMode are 0x00s
	}


	m_WanIp = EvaUtil::read32(buf+pos); pos+=4; // if is cancellation, 0x00000001
	m_WanPort = EvaUtil::read16(buf+pos); pos+=2; // if is UDP_EX request, 0x00000000
	
	if(getNormalIMType() == QQ_IM_NOTIFY_FILE_AGENT_INFO){
		m_SessionId = EvaUtil::read32(buf+pos); pos+=4;
		memcpy(m_AgentServerKey, buf+pos, 16);
		pos++; // last byte always be 0x01
		return;
	}
	
	pos+=2;
	// request cancellation finished, actually, there are still 2 bytes(0x0000) afterwards
	// we just ignore them :)
	if(getNormalIMType() == QQ_IM_EX_REQUEST_CANCELLED) return; 

	// request acceptation finished, actually, there are still 16 bytes afterwards
	// they are all unknown, we just ignore them :)
	if(getNormalIMType() == QQ_IM_EX_REQUEST_ACCEPTED) return;

	if(getNormalIMType() != QQ_IM_TCP_REQUEST){
		pos+=4; // unknown, 0x00000000
		pos++; // unknown, 0x02 or 0x04
	}

	pos+=2; // if UDP request: 0x0000, if TCP request: 0x0102
	if(getNormalIMType() != QQ_IM_TCP_REQUEST)
		pos+=4; // unknown, 0x00000000 or 0x00000001

	pos+=2; // unknown 2 bytes, 0x0000

	if(getNormalIMType() != QQ_IM_TCP_REQUEST){
		pos+=2; //FIXME the length of the following part, we should check this
		pos++; // unknown, 0x01
		pos+=2; //FIXME the length of the following part, we should check this, again!
	}

	if(getNormalIMType() == QQ_IM_ACCEPT_UDP_REQUEST ||
		getNormalIMType() == QQ_IM_NOTIFY_IP)
		return;

	// now type should be QQ_IM_UDP_REQUEST or QQ_IM_TCP_REQUEST
	pos+=2; //FIXME we should check if they space and 0x1F, they should be 0x201f

	int strLen = 0;
	while(buf[pos+strLen]!=0x1f)
		strLen++;
#ifdef WIN32
	char* strFile; strFile=(char*)_alloca(strLen + 1);
#else
	char strFile[strLen + 1];
#endif
	memcpy(strFile, buf+pos, strLen);
	strFile[strLen] = 0x00;
	m_FileName.assign(strFile);
	pos+=strLen;
	pos++; // the 0x1f
	
	strLen = len - pos - 5; // ignore last 5 bytes " zijie" in Chinese
#ifdef WIN32
	char* strSize; strSize=(char*)_alloca(strLen + 1);
#else
	char strSize[strLen + 1];
#endif
	memcpy(strSize, buf+pos, strLen);
	strSize[strLen] = 0x00;
	m_FileSize = atoi(strSize);
}


/** ===========================================================    **/


ReceivedFileExIpIM::ReceivedFileExIpIM( const unsigned char * buf, const int len )
	: NormalIMBase(buf, len),
	m_WanIp1(0), m_WanPort1(0),
	m_WanIp2(0), m_WanPort2(0),
	m_WanIp3(0), m_WanPort3(0),
	m_LanIp1(0), m_LanPort1(0),
	m_LanIp2(0), m_LanPort2(0),
	m_LanIp3(0), m_LanPort3(0),
	m_SyncIp(0), m_SyncPort(0), m_SyncSession(0)
{
}

ReceivedFileExIpIM::ReceivedFileExIpIM( const ReceivedFileExIpIM & rhs )
	: NormalIMBase(rhs)
{
	*this = rhs;
}

ReceivedFileExIpIM & ReceivedFileExIpIM::operator =( const ReceivedFileExIpIM & rhs )
{
	*((NormalIMBase *)this) = (NormalIMBase)rhs;
	m_TransferType = rhs.getTransferType();
	m_ConnectMode = rhs.getConnectMode();
	m_SessionId = getSessionId();

	m_WanIp1 = rhs.getWanIp1();
	m_WanPort1 = rhs.getWanPort1();
	m_WanIp2 = rhs.getWanIp2();
	m_WanPort2 = rhs.getWanPort2();
	m_WanIp3 = rhs.getWanIp3();
	m_WanPort3 = rhs.getWanPort3();

	m_LanIp1 = rhs.getLanIp1();
	m_LanPort1 = rhs.getLanPort1();
	m_LanIp2 = rhs.getLanIp2();
	m_LanPort2 = rhs.getLanPort2();
	m_LanIp3 = rhs.getLanIp3();
	m_LanPort3 = rhs.getLanPort3();

	return *this;
}

void ReceivedFileExIpIM::parseContents( const unsigned char * buf, const int len )
{
	int pos = 27; // ignore 19 + 8 bytes
	m_TransferType = buf[pos++];

	m_ConnectMode = buf[pos++];

	// this part actually is for UDP only(maybe Request cancellation as well)
	pos++; // unknown bytes, 0x66 or 0x67
	pos+=4; // unknown 4 bytes, 0x00000001

	m_SessionId = EvaUtil::read32(buf+pos); pos+=4;

	if(EvaUtil::read16(buf+pos) == 0x0001)
		m_IsSender = true;
	else
		m_IsSender = false;
	pos+=2;

	int l = EvaUtil::read16(buf+pos); pos+=2;  // following length
	if(l!= (len - pos)) return;

	// all ip is in little endian format, whereas port is in big endian
	memcpy(&m_WanIp1, buf+pos, 4); pos+=4;
	m_WanPort1 = EvaUtil::read16(buf+pos); pos+=2;
	memcpy(&m_WanIp2, buf+pos, 4); pos+=4;
	m_WanPort2 = EvaUtil::read16(buf+pos); pos+=2;

	memcpy(&m_LanIp1, buf+pos, 4); pos+=4;
	m_LanPort1 = EvaUtil::read16(buf+pos); pos+=2;
	memcpy(&m_LanIp2, buf+pos, 4); pos+=4;               // always 0?
	m_LanPort2 = EvaUtil::read16(buf+pos); pos+=2;       // always 0?
	memcpy(&m_LanIp3, buf+pos, 4); pos+=4;
	m_LanPort3 = EvaUtil::read16(buf+pos); pos+=2;

	if(m_IsSender){
		memcpy(&m_SyncIp, buf+pos, 4); pos+=4;
		m_SyncPort = EvaUtil::read16(buf+pos); pos+=2;
		m_SyncSession = EvaUtil::read32(buf+pos); pos+=4;
	}

	memcpy(&m_WanIp3, buf+pos, 4); pos+=4;
	m_WanPort3 = EvaUtil::read16(buf+pos); pos+=2;
	
	// following are unknown 12 bytes, starting with 0x00 00 14 00 ....
	pos+=12;
}




/** ===========================================================    **/



ReceivedQQNews::ReceivedQQNews( const unsigned char * buf, const int len )
	: m_Title(""),
	m_Brief(""),
	m_URL("")
{
	parseData(buf, len);
}

ReceivedQQNews::ReceivedQQNews( const ReceivedQQNews & rhs )
	: m_Title(""),
	m_Brief(""),
	m_URL("")
{
	*this = rhs;
}

ReceivedQQNews & ReceivedQQNews::operator =( const ReceivedQQNews & rhs )
{
	m_Title = rhs.getTitle();
	m_Brief = rhs.getBrief();
	m_URL = rhs.getURL();
	return *this;
}

void ReceivedQQNews::parseData( const unsigned char * buf, const int /*len*/ )
{
	int pos = 4; // ignore unknown 4 bytes
	unsigned char slen = buf[pos++];
	char *str = new char[slen+1];
	memcpy(str, buf+pos, slen);
	str[slen] = 0x00;
	m_Title = str;
	delete []str;
	pos += slen;

	slen = buf[pos++];
	str = new char[slen+1];
	memcpy(str, buf+pos, slen);
	str[slen] = 0x00;
	m_Brief = str;
	delete []str;	
	pos += slen;

	slen = buf[pos++];
	str = new char[slen+1];
	memcpy(str, buf+pos, slen);
	str[slen] = 0x00;
	m_URL = str;
	delete []str;	
	pos += slen;

	// still 5 unknown 0x00s
}









