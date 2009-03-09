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

#include "evaimsend.h"
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
#include <cstdlib>

SendIM::SendIM(const unsigned short type)
	: OutPacket(QQ_CMD_SEND_IM, true),
	receiver(0),
	face(0),
	contentType(type)
{
	m_MsgSequence = 0;
	m_SendTime = time(NULL);
}

SendIM::SendIM(const SendIM &rhs)
	: OutPacket(rhs)
{
	*this = rhs;
}

SendIM &SendIM::operator=(const SendIM &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	receiver = rhs.getReceiver();
	face = rhs.getFaceCode();
	contentType = rhs.getContentType();
	m_SendTime = rhs.getSendTime();
	m_MsgSequence = rhs.getMsgSequence();
        return *this;
}

int SendIM::putBody(unsigned char *buf) 
{
	int pos=0;
	// bit 0-3 sender
	pos += EvaUtil::write32(buf+pos, getQQ());
	// bit 4-7 receiver
	pos += EvaUtil::write32(buf+pos, receiver);
	
	//  bit 8-9 qq client version
	pos += EvaUtil::write16(buf+pos, version);
	
	// bit 10-13 sender again
	pos += EvaUtil::write32(buf+pos, getQQ());
	
	// bit 14-17 receiver
	pos += EvaUtil::write32(buf+pos, receiver);
	
	// bit 18-33 file transfer session key
	memcpy( buf+pos, getFileSessionKey(), QQ_KEY_LENGTH);
	pos+=QQ_KEY_LENGTH;
	
	// bit 34-35 contents type ( is it normal text message or file transfer...)
	pos += EvaUtil::write16(buf+pos, contentType);
	
	// sequence number
//	short tmpSeq = htons(sequence);
//	memcpy(buf+pos, &tmpSeq,2);
//	pos+=2;	
	if(m_MsgSequence)
		pos += EvaUtil::write16(buf+pos, m_MsgSequence);
	else
		pos+= EvaUtil::write16(buf+pos, sequence);
		
	// sending time
	pos += EvaUtil::write32(buf+pos, m_SendTime);

	pos += EvaUtil::write16(buf+pos, face);
	
	memset(buf+pos, 0, 3);
	pos+=3;
	
	// font information, set as 1
	buf[pos++] = 0x01;

	if(contentType == QQ_IM_NORMAL_TEXT){
		// note that, for QQ_IM_NORMAL_TEXT the contents should 
		// be 4 bytes(multi-fragment information) more than others
		pos+= putContents(buf+pos);  // put all contents in	
	} else {
		// unknown 4 bytes
		memset(buf + pos, 0, 4);
		pos+=4;

		pos+= putContents(buf+pos);  // put all contents in
	}
	return pos;
} 

int SendIM::putContents(unsigned char *buf)
{
	fprintf(stderr, "In SendIM\n");
	buf[0]=0;
	return 0;
}

/*  ======================================================= */

short SendTextIMPacket::messageID = 0;

SendTextIMPacket::SendTextIMPacket()
	: SendIM(QQ_IM_NORMAL_TEXT),
	encoding(QQ_IM_ENCODING_GB),
	red(0),green(0),blue(0),
	bold(false),italic(false),underline(false),
	fontSize(0x09),
	fontFlag(0x09),
	message("Hello, I am using Eva."),
	replyType(QQ_IM_NORMAL_REPLY)
{
	char *fname = (char *)malloc(5);
        fname[0] = 0xcb;
	fname[1] = 0xce;
	fname[2] = 0xcc;
        fname[3] = 0xe5;
	fname[4] = 0x00;
	fontName = fname; // just set font name to "Song Ti" in Chinese
	free(fname);
	
	numFragments = 0;
	seqFragments = 0;
}

SendTextIMPacket::SendTextIMPacket(const SendTextIMPacket &rhs)
	: SendIM(rhs)
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
	replyType = rhs.getReplyType();
}

SendTextIMPacket &SendTextIMPacket::operator=(const SendTextIMPacket &rhs)
{
	*((SendIM *)this) = (SendIM)rhs;
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
	replyType = rhs.getReplyType();
        return *this;
}

void SendTextIMPacket::setBold(bool bold) 
{
	this->bold = bold;
	fontFlag &= 0xDF;
	fontFlag |= bold ? 0x20 : 0x00;
}
 
void SendTextIMPacket::setItalic(bool italic) 
{
	this->italic = italic;
	fontFlag &= 0xBF;
	fontFlag |= italic ? 0x40 : 0x00;
}
	 
void SendTextIMPacket::setUnderline(bool underline) 
{
	this->underline = underline;
	fontFlag &= 0x7F;
	fontFlag |= underline ? 0x80 : 0x00;
}
    
void SendTextIMPacket::setFontSize(char fontSize) 
{
	this->fontSize = fontSize;
	fontSize &= 0x1F;
	fontFlag &= 0xE0;
	fontFlag |= fontSize;
}

void SendTextIMPacket::setAutoReply(const bool isNormal)
{
	if(isNormal)
		replyType = QQ_IM_NORMAL_REPLY;
	else
		replyType = QQ_IM_AUTO_REPLY;
}

int SendTextIMPacket::putContents(unsigned char *buf) 
{
	int pos=0;

	//multi-fragment information
	buf[pos++] = numFragments;
	buf[pos++] = seqFragments;
	memcpy(buf+pos, &messageID, 2);
	pos+=2;
	
	bool hasImage = false;
	std::string str2send = EvaUtil::convertToSend(message, &hasImage);

	buf[pos++] = hasImage?QQ_IM_IMAGE_REPLY:replyType; // auto-reply or not

	memcpy(buf+pos, str2send.c_str(), str2send.length());
	pos += str2send.length();

	if (getNumFragments() == (getSeqOfFragments() + 1))
	{
		buf[pos++] = 0x20; //  a space, witch is needed in the last fragment
		setMessageID(getMessageID() + 1);
	}
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
/*
std::string SendTextIMPacket::convertToSend(const std::string &text)
{
	std::string converted = "";
	int offset=0;
	char smileyTag = 0x14;
	for(uint i=0; i< text.length(); i++){
		if(text[i] == '/'){
			offset = i;
			while(text[offset] != 0x00 && text[++offset]!=' ');
			if((offset - i)<8){
				char code = util.textToSmiley(text.substr(i,offset-i));
				if(code){
					converted += smileyTag;
					converted += code;
					i=offset;
					continue;
				}
			}
			converted+=text[i];
			continue;
		}
		converted+=text[i];
	}
	return converted;
}*/

/* =========================================================== */

SendIMReplyPacket::SendIMReplyPacket(unsigned char *buf, int len)
	: InPacket(buf, len),
	replyCode(0x01)
{
}

SendIMReplyPacket::SendIMReplyPacket(const SendIMReplyPacket &rhs)
	: InPacket(rhs)
{
	replyCode = rhs.getReplyCode();
}

SendIMReplyPacket &SendIMReplyPacket::operator=(const SendIMReplyPacket &rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	replyCode = rhs.getReplyCode();
        return *this;
}

const bool SendIMReplyPacket::isSentOK() const
{
	 return replyCode == QQ_SEND_IM_REPLY_OK;
}

void SendIMReplyPacket::parseBody()
{
	replyCode = decryptedBuf[0];
}


/*  ======================================================= */


SendFileRequestPacket::SendFileRequestPacket( )
	: SendIM(QQ_IM_UDP_REQUEST)
	, m_FileName("")
	, m_FileSize(0)
	, m_DirectPort(0)
	, m_WanPort(0)
	, m_WanIp(0)
	, m_TransferType(QQ_TRANSFER_FACE)
{
}

SendFileRequestPacket::SendFileRequestPacket( const SendFileRequestPacket & rhs )
	: SendIM(rhs)
{
	*this = rhs;
}

SendFileRequestPacket & SendFileRequestPacket::operator =( const SendFileRequestPacket & rhs )
{
	(SendIM)(*this) = (SendIM)rhs;
	m_FileName = rhs.getFileName();
	m_FileSize = rhs.getFileSize();
	m_DirectPort = rhs.getDirectPort();
	m_WanPort = rhs.getWanPort();
	m_WanIp = rhs.getWanIp();
	m_TransferType = rhs.getTransferType();
	return *this;
}

int SendFileRequestPacket::putContents( unsigned char * buf )
{
	int pos = 0;
	memset(buf, 0, 11);pos+=11;	// 11 bytes 0x00
	
	buf[pos++] = m_TransferType;	// transfer File or custom-picture
	buf[pos++] = 0x00;
	
	pos+=EvaUtil::write32(buf+pos, m_WanIp);
	pos+=EvaUtil::write16(buf+pos, m_WanPort);
	
	pos+=EvaUtil::write16(buf+pos, m_DirectPort);
	memset(buf+pos, 6, 0); pos+=6;

	buf[pos++] = 0x20;
	buf[pos++] = 0x1f;
	
	int len = strlen(m_FileName.c_str());
	memcpy(buf+pos, m_FileName.c_str(), len); pos+=len;
	buf[pos++] = 0x1f;

#ifdef WIN32
	char* strSize; strSize=(char*)_alloca(100);
#else
	char strSize[100];
#endif
	memset(strSize, 100, 0);
	sprintf(strSize, "%d", m_FileSize);
	len = strlen(strSize);
	memcpy(buf+pos, strSize, len); pos+=len;
	buf[pos++] = 0x20;

	// chinese for "byte"(zijie) encoded in GB18030
	buf[pos++] = 0xd7;
	buf[pos++] = 0xd6;
	buf[pos++] = 0xbd;
	buf[pos++] = 0xda;

	return pos;
}



/*  ======================================================= */


SendFileExRequestPacket::SendFileExRequestPacket( const short cmd )
	: SendIM(cmd)
	, m_FileName("")
	, m_FileSize(0)
	, m_WanPort(0)
	, m_WanIp(0)
	, m_TransferType(QQ_TRANSFER_FILE)
{
}

SendFileExRequestPacket::SendFileExRequestPacket( const SendFileExRequestPacket & rhs )
	: SendIM(rhs)
{
	*this = rhs;
}

SendFileExRequestPacket & SendFileExRequestPacket::operator =( const SendFileExRequestPacket & rhs )
{
	(SendIM)(*this) = (SendIM)rhs;
	m_FileName = rhs.getFileName();
	m_FileSize = rhs.getFileSize();
	m_WanPort = rhs.getWanPort();
	m_WanIp = rhs.getWanIp();
	m_TransferType = rhs.getTransferType();
	m_SessionId = rhs.getSessionId();
	return *this;
}

int SendFileExRequestPacket::putContents( unsigned char * buf )
{
	int pos = 0;
	memset(buf, 0, 11);pos+=11;	// 11 bytes 0x00
	
	buf[pos++] = m_TransferType;	// transfer File or custom-picture
	buf[pos++] = 0x00;

	buf[pos++] = 0x66;  // unknown byte, 0x66, but sometimes is 0x67

	// 2 bytes, 0x0000 ( if is Cancellation Request,this part still here? 
	// no, if not QQ_IM_EX_REQUEST_CANCELED command, this part will be here.:)
	if(getContentType() != QQ_IM_EX_REQUEST_CANCELLED) pos+=EvaUtil::write16(buf+pos, 0x00);

	pos+=EvaUtil::write32(buf+pos, 1); // 4 bytes, 0x00000001
	pos+=EvaUtil::write16(buf+pos, 0x00); // 2 bytes, 0x0000
	pos+=EvaUtil::write16(buf+pos, m_SessionId); // can it be random number ?

	if(getContentType() != QQ_IM_EX_REQUEST_CANCELLED){
		if(getContentType() == QQ_IM_EX_REQUEST_ACCEPTED){
			pos+=EvaUtil::write32(buf+pos, m_WanIp); // big endian
			memset(buf+pos, 0, 8); pos+=8;
		}else {
			// this may include ip(4 bytes) and port(4 bytes) and 0x00000000(4 bytes)
			memset(buf+pos, 0, 12); pos+=12;
		}
	
		buf[pos++] = 0x02; // unknown, 0x02 or 0x04
		if(getContentType() == QQ_IM_TCP_REQUEST)
			EvaUtil::write16(buf+pos, 0x0102);
		else
			memset(buf+pos, 0, 2);
		pos+=2;
	}

	pos+=EvaUtil::write32(buf+pos, 1); // unknown 4 bytes, 0x00000000 or  0x00000001

	memset(buf+pos, 0, 2); pos+=2;

	if(getContentType() == QQ_IM_EX_REQUEST_ACCEPTED){ // QQ_IM_EX_REQUEST_ACCEPTED finished here
		memset(buf+pos, 0, 2); pos+=2;
		return pos;
	}

	if(getContentType() == QQ_IM_EX_REQUEST_CANCELLED){ // QQ_IM_EX_REQUEST_CANCELED finished here
		memset(buf+pos, 0, 6); pos+=6;
		return pos;
	}
		
	int offset = 0;
	if(getContentType() == QQ_IM_EX_UDP_REQUEST){
		offset = pos;
		pos+=2;
		buf[pos++] = 0x01; // unknown
		pos+=2;  // this part this the following part length again! so offset will be offset+3
	}


	buf[pos++] = 0x20;
	buf[pos++] = 0x1f;
	
	int len = strlen(m_FileName.c_str());
	memcpy(buf+pos, m_FileName.c_str(), len); pos+=len;
	buf[pos++] = 0x1f;

#ifdef WIN32
	char* strSize; strSize=(char*)_alloca(100);
#else
	char strSize[100];
#endif
	memset(strSize, 100, 0);
	sprintf(strSize, "%d", m_FileSize);
	len = strlen(strSize);
	memcpy(buf+pos, strSize, len); pos+=len;
	buf[pos++] = 0x20;

	// chinese for "byte"(zijie) encoded in GB18030
	buf[pos++] = 0xd7;
	buf[pos++] = 0xd6;
	buf[pos++] = 0xbd;
	buf[pos++] = 0xda;

	// now we set the 2 lengths
	EvaUtil::write16(buf+offset, pos-offset-2);
	EvaUtil::write16(buf+offset+3, pos-offset-2-3-2);

// 	printf("\n----====== SendFileExRequestPacket::putContents ======----");
// 	for(int i=0; i<pos; i++){
// 		if(!(i%8)) printf("\n%d: ",i);
// 		char t = buf[i];
// 		printf("%2x ", (uint8_t)t);
// 	}
// 	printf("\n        ----====== UDP EX Request Sent ======----\n\n");
	return pos;
}


/*  ======================================================= */


SendIpExNotifyPacket::SendIpExNotifyPacket( const bool isSender )
	: SendIM(QQ_IM_EX_NOTIFY_IP)
	, m_IsSender(isSender)
	, m_TransferType(QQ_TRANSFER_FILE)
	, m_ConnectMode(QQ_TRANSFER_FILE_UDP)
	, m_SessionId(0)
	, m_WanIp1(0), m_WanPort1(0)
	, m_WanIp2(0), m_WanPort2(0)
	, m_WanIp3(0), m_WanPort3(0)
	, m_LanIp1(0), m_LanPort1(0)
	, m_LanIp2(0), m_LanPort2(0)
	, m_LanIp3(0), m_LanPort3(0)
	, m_SyncIp(0), m_SyncPort(0)
	, m_SyncSession(0)
{
}


SendIpExNotifyPacket::SendIpExNotifyPacket( const SendIpExNotifyPacket & rhs )
	: SendIM(rhs)
{
	*this = rhs;
}

SendIpExNotifyPacket & SendIpExNotifyPacket::operator =( const SendIpExNotifyPacket & rhs )
{
	(SendIM)(*this) = (SendIM)rhs;
	m_IsSender = rhs.isSender();
	m_TransferType = rhs.getTransferType();
	m_ConnectMode = rhs.getConnectMode();
	m_SessionId = rhs.getSessionId();

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

	m_SyncIp = rhs.getSyncIp();
	m_SyncPort = rhs.getSyncPort();
	m_SyncSession = rhs.getSyncSession();

	return *this;
}

int SendIpExNotifyPacket::putContents( unsigned char * buf )
{
	int pos = 0;
	memset(buf, 0, 11);pos+=11;	// 11 bytes 0x00
	
	buf[pos++] = m_TransferType;	// transfer File or custom-picture
	buf[pos++] = m_ConnectMode;

	buf[pos++] = 0x66;  // unknown byte, 0x66, but sometimes is 0x67

	// 2 bytes, 0x0000 ( if is Cancellation Request,this part still here?
	//pos+=EvaUtil::write16(buf+pos, 0x00);

	pos+=EvaUtil::write32(buf+pos, 1); // 4 bytes, 0x00000001
	pos+=EvaUtil::write32(buf+pos, m_SessionId);

	// following 2 bytes maybe a unsigned short sequence id for this session
	buf[pos++] = 0x00;
	buf[pos++] = m_IsSender?0x01:0x02;

	int offset = pos;
	pos+=2;  // length of following;

	memcpy(buf+pos, &m_WanIp1, 4); pos+=4; // little endian
	pos+=EvaUtil::write16(buf+pos, m_WanPort1); // 2 bytes

	memcpy(buf+pos, &m_WanIp2, 4); pos+=4; // little endian
	pos+=EvaUtil::write16(buf+pos, m_WanPort2); // 2 bytes

	memcpy(buf+pos, &m_LanIp1, 4); pos+=4; // little endian
	pos+=EvaUtil::write16(buf+pos, m_LanPort1); // 2 bytes

	// this part looks always 6 0x00s
	memcpy(buf+pos, &m_LanIp2, 4); pos+=4; // little endian
	pos+=EvaUtil::write16(buf+pos, m_LanPort2); // 2 bytes


	memcpy(buf+pos, &m_LanIp3, 4); pos+=4; // little endian
	pos+=EvaUtil::write16(buf+pos, m_LanPort3); // 2 bytes

	if(m_IsSender){
		//unsigned int agent= 0xdb503185;
		memcpy(buf+pos, &m_SyncIp, 4); pos+=4; // little endian
		//unsigned short aport = 0x1f40;
		pos+=EvaUtil::write16(buf+pos, m_SyncPort); // 2 bytes	
		pos+=EvaUtil::write32(buf+pos, m_SyncSession);
	}
	memcpy(buf+pos, &m_WanIp3, 4); pos+=4; // little endian
	pos+=EvaUtil::write16(buf+pos, m_WanPort3); // 2 bytes	

	memset(buf+pos, 0, 10); pos+=10;
	if(m_IsSender){
		memset(buf+pos, 0, 2); pos+=2;
	}
// 	memset(buf+pos, 0, 2); pos+=2;
// 	buf[pos++] = 0x14; // unknown byte
// 	buf[pos++] = 0x00;
// 	pos+=EvaUtil::write16(buf+pos, 0x40d5); // unknown 2 bytes
// 
// 	memset(buf+pos, 0, 4); pos+=4; // maybe a another ip? we try all 0
	buf[pos++] = 0x12;
	buf[pos++] = 0x00;

	// write the length
	EvaUtil::write16(buf+offset, pos-offset-2);

	printf("\n----====== SendIpExNotifyPacket::putContents ======----");
	for(int i=0; i<pos; i++){
		if(!(i%8)) printf("\n%d: ",i);
		char t = buf[i];
		printf("%2x ", (uint8_t)t);
	}
	printf("\n----====== IP Notification Built ======----\n\n");
	return pos;
}


/** ====================================================================== */


SendFileNotifyAgentPacket::SendFileNotifyAgentPacket()
	: SendIM(QQ_IM_NOTIFY_FILE_AGENT_INFO)
	, m_TransferType(QQ_TRANSFER_FILE)
	, m_ConnectMode(QQ_TRANSFER_FILE_TCP)
{
}

SendFileNotifyAgentPacket::SendFileNotifyAgentPacket(const SendFileAcceptPacket &rhs)
	: SendIM(rhs)
{
	*this = rhs;
}

SendFileNotifyAgentPacket &SendFileNotifyAgentPacket::operator=(const SendFileNotifyAgentPacket &rhs)
{
	(SendIM)(*this) = (SendIM)rhs;
	m_Ip = rhs.getAgentIp();
	m_Port = rhs.getAgentPort();
	m_Session = rhs.getAgentSession();
	return *this;
}

int SendFileNotifyAgentPacket::putContents(unsigned char *buf)
{
	int pos = 0;
	memset(buf, 0, 11);pos+=11;	// 11 bytes 0x00
	
	buf[pos++] = m_TransferType;	// transfer File or custom-picture
	buf[pos++] = m_ConnectMode;

	memset(buf+pos, 0, 12); pos+=12;
	pos += EvaUtil::write32(buf+pos, m_Ip);
	pos += EvaUtil::write16(buf+pos, m_Port);
	pos += EvaUtil::write32(buf+pos, m_Session);

	memcpy(buf+pos, getFileAgentKey(), 16); pos+=16;
	buf[pos++] = 0x01; // unknown byte
	printf("SendFileNotifyAgentPacket::putContents -- ip:%8x\tport:%d\n", m_Ip, m_Port);
	for(int i=0; i<pos; i++){
		if(!(i%8)) printf("\n%d: ",i);
		char t = buf[i];
		printf("%2x ", (uint8_t)t);
	}
	printf("\n --------------------------------------\n\n");
	return pos;	
}


/** ====================================================================== */


SendTempSessionTextIMPacket::SendTempSessionTextIMPacket()
: OutPacket(QQ_CMD_TEMP_SESSION_OP,true),
encoding(QQ_IM_ENCODING_GB),
red(0),green(0),blue(0),
bold(false),italic(false),underline(false),
fontSize(0x09),
fontFlag(0x09),
message("Hello, I am using Eva."),
subcommand(QQ_SUB_CMD_SEND_TEMP_SESSION_IM),
authInfo(NULL)
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

SendTempSessionTextIMPacket::SendTempSessionTextIMPacket(const SendTempSessionTextIMPacket &rhs)
: OutPacket(rhs)
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
	subcommand = rhs.getSubcommand();

	site = rhs.getSite();
	nick = rhs.getNick();
}

SendTempSessionTextIMPacket &SendTempSessionTextIMPacket::operator=(const SendTempSessionTextIMPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
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
	subcommand = rhs.getSubcommand();

	site = rhs.getSite();
	nick = rhs.getNick();

	setAuthInfo(rhs.getAuthInfo(),rhs.getAuthInfoSize());
	return *this;
}

SendTempSessionTextIMPacket::~SendTempSessionTextIMPacket() {
	if (authInfo) delete[] authInfo;
}

void SendTempSessionTextIMPacket::setAuthInfo(const unsigned char* authInfo, int len) {
	if (this->authInfo) delete authInfo;
	this->authInfo = new unsigned char[len];
	this->authInfoSize=len;
	memcpy(this->authInfo,authInfo,len);
}

void SendTempSessionTextIMPacket::setBold(bool bold) 
{
	this->bold = bold;
	fontFlag &= 0xDF;
	fontFlag |= bold ? 0x20 : 0x00;
}

void SendTempSessionTextIMPacket::setItalic(bool italic) 
{
	this->italic = italic;
	fontFlag &= 0xBF;
	fontFlag |= italic ? 0x40 : 0x00;
}

void SendTempSessionTextIMPacket::setUnderline(bool underline) 
{
	this->underline = underline;
	fontFlag &= 0x7F;
	fontFlag |= underline ? 0x80 : 0x00;
}

void SendTempSessionTextIMPacket::setFontSize(char fontSize) 
{
	this->fontSize = fontSize;
	fontSize &= 0x1F;
	fontFlag &= 0xE0;
	fontFlag |= fontSize;
}

int SendTempSessionTextIMPacket::putBody(unsigned char *buf) 
{
	int pos=0;
	buf[pos++]=subcommand;
	char* pszPos;
	int rev;
	unsigned short sht;

	switch(subcommand) {
			case QQ_SUB_CMD_SEND_TEMP_SESSION_IM:
				// ±µ¶¨™Ã
				rev=htonl(getReceiver());
				memmove(buf+pos,&rev,4);
				pos+=4;
				// authinfo
				sht=htons(authInfoSize);
				memmove(buf+pos,&sht,2);
				pos+=2;
				memmove(buf+pos,authInfo,authInfoSize);
				pos+=authInfoSize;
				// •º™æ
				memset(buf+pos,0,4);
				pos+=4;
				// nick
				buf[pos++]=(unsigned char) strlen(nick.c_str());
				memmove(buf+pos,nick.c_str(),strlen(nick.c_str()));
				pos+=strlen(nick.c_str());
				// site name
				buf[pos++]=(unsigned char) strlen(site.c_str());
				memmove(buf+pos,site.c_str(),strlen(site.c_str()));
				pos+=strlen(site.c_str());
				// •º™æ
				buf[pos++]=2;
				// •º™æ
				memset(buf+pos,0,4);
				pos+=4;
				// Éc´◊°A≥Ã¶Z¶A∂ÅE
				pszPos=(char*)buf+pos;
				pos+=2;
				// Æ¯ÆßÅXÆe
				memmove(buf+pos,message.c_str(),strlen(message.c_str()));
				pos+=strlen(message.c_str());
				buf[pos++]=0x20;
				// ¶r ^åp© 
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
				// ¶^∂ÒÉc´◊
				int finalLength=htons((char*)buf+pos-pszPos-2);
				memmove(pszPos,&finalLength,2);
				break;
	}
	return pos;
}
