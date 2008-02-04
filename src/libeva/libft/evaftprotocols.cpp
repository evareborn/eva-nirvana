/***************************************************************************
 *   Copyright (C) 2004-2005 by yunfan                                     *
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

#include "evaftprotocols.h"
#include "../evautil.h"
#include "../evacrypt.h"
#include <stdio.h>
#include <string.h>

EvaFTAgentCreate::EvaFTAgentCreate()
	: EvaFTAgentPacket(QQ_FILE_AGENT_CMD_CREATE),
	m_Token(NULL), m_TokenLength(0), m_Id(0), m_Ip(0)
{
}

EvaFTAgentCreate::~EvaFTAgentCreate()
{
	if(m_Token)
		delete []m_Token;
}

void EvaFTAgentCreate::setFileAgentToken(const unsigned char *token, const int len)
{
	if(!token) return;
	if(m_Token) delete [] m_Token;
	m_Token = new unsigned char[len];
	memcpy(m_Token, token, len);
	m_TokenLength = len;
}

const int EvaFTAgentCreate::fillBody(unsigned char *buf)
{
	if(!m_Token) return -1;
	int pos=0;
	pos += EvaUtil::write16(buf, m_TokenLength);
	memcpy(buf+pos, m_Token, m_TokenLength); pos+=m_TokenLength;
	
	int rawPos=0;
	unsigned char *raw = new unsigned char [20];
	rawPos += EvaUtil::write16(raw, 0x07d0);
	rawPos += EvaUtil::write32(raw + rawPos, m_Id);
	memcpy(raw+rawPos, &m_Ip, 4); rawPos+=4; // in little endian format
	rawPos += EvaUtil::write16(raw + rawPos, 0x0575);

	int outLen = 1000;
	EvaCrypt::encrypt(raw, rawPos, m_FileAgentKey, buf+pos, &outLen);
	pos += outLen;
	delete []raw;
	return pos;
}


/** ========================================================================================== */

EvaFTAgentCreateReply::EvaFTAgentCreateReply(const unsigned char *buf, const int len)
	: EvaFTAgentPacket(buf, len),
	m_ReplyCode(QQ_FILE_AGENT_CREATE_REDIRECT),
	m_Ip(0), m_Port(0)
{
}

const bool EvaFTAgentCreateReply::parseBody( unsigned char *buf, const int len)
{
	int rawLen = 256;
	unsigned char *raw = new unsigned char[256];
	
	if(!EvaCrypt::decrypt(buf, len, m_FileAgentKey, raw, &rawLen)){
		fprintf(stderr, "EvaFTAgentCreateReply::parseBody -- decrypt failed\n");
		delete []raw;
		return false;
	}
	
	int pos = 0;
	m_ReplyCode = EvaUtil::read16(raw); pos+=2;
	switch(m_ReplyCode){
	case QQ_FILE_AGENT_CREATE_OK:
		memcpy(&m_Ip, raw+pos, 4); pos+=4;   // ip in little endian format
		m_Port = EvaUtil::read16(raw+pos); pos+=2;
		m_Session = EvaUtil::read32(raw+pos); pos+=4;
		// all following are 0x00s, 8 more bytes
		break;
	case QQ_FILE_AGENT_CREATE_REDIRECT:
		pos+=10; // 10 bytes 0x000
		memcpy(&m_Ip, raw+pos, 4); pos+=4;   // ip in little endian format
		m_Port = EvaUtil::read16(raw+pos); pos+=2;
		// actually, there still 2 more bytes(0x0000)
		break;
	case QQ_FILE_AGENT_CREATE_ERROR:{
		pos+=2; // unknown 2 bytes
		memcpy(&m_Ip, raw+pos, 4); pos+=4;   // ip in little endian format, unknown ip
		pos+=10; // 10 bytes 0x000
		short msgLen = EvaUtil::read16(raw+pos); pos+=2;
		char *tmp = new char[msgLen+1];
		memcpy(tmp, raw+pos, msgLen); pos+=msgLen;
		tmp[msgLen]=0x00;
		m_ErrMessage.assign(tmp);
		delete []tmp;
		}
		break;
	default:
		fprintf(stderr, "EvaFTAgentCreateReply::parseBody -- un-supported reply code: 0x%4x\n", 0xffff&m_ReplyCode);
		delete []raw;
		return false;
	}
	delete []raw;
	return true;
}



/** ========================================================================================== */

EvaFTAgentLogin::EvaFTAgentLogin()
	: EvaFTAgentPacket(QQ_FILE_AGENT_CMD_LOGIN),
	m_Token(NULL), m_TokenLength(0)
{
}

EvaFTAgentLogin::~EvaFTAgentLogin()
{
	if(m_Token)
		delete []m_Token;
}

void EvaFTAgentLogin::setFileAgentToken(const unsigned char *token, const int len)
{
	if(!token) return;
	if(m_Token) delete [] m_Token;
	m_Token = new unsigned char[len];
	memcpy(m_Token, token, len);
	m_TokenLength = len;
}

const int EvaFTAgentLogin::fillBody(unsigned char *buf)
{
	if(!m_Token) return -1;
	int pos=0;
	
	unsigned char *raw = new unsigned char [256];
	pos += EvaUtil::write16(raw, m_TokenLength);
	memcpy(raw+pos, m_Token, m_TokenLength); pos+=m_TokenLength;
	
	
	int outLen = 1000;
	EvaCrypt::encrypt(raw, pos, m_FileAgentKey, buf, &outLen);
	delete []raw;
	return outLen;
}



/** ========================================================================================== */


EvaFTAgentLoginReply::EvaFTAgentLoginReply(const unsigned char *buf, const int len)
	: EvaFTAgentPacket(buf, len),
	m_ReplyCode(1)
{
}

const bool EvaFTAgentLoginReply::parseBody(unsigned char *buf, const int len)
{
	int rawLen = 256;
	unsigned char *raw = new unsigned char[256];
	
	if(!EvaCrypt::decrypt(buf, len, m_FileAgentKey, raw, &rawLen)){
		fprintf(stderr, "EvaFTAgentLoginReply::parseBody -- decrypt failed\n");
		delete []raw;
		return false;
	}
	
	int pos = 0;
	m_ReplyCode = EvaUtil::read32(raw); pos+=4;
	delete []raw;
	return true;
}



/** ========================================================================================== */

EvaFTAgentTransfer::EvaFTAgentTransfer(const int type)
	: EvaFTAgentPacket(QQ_FILE_AGENT_CMD_TRANSFER),
	m_Type(type), m_FileName(""), m_FMd5(NULL),
	m_FileSize(0), m_Data(NULL),
	m_DataLength(0), m_StartOffset(0)
{
}

EvaFTAgentTransfer::~EvaFTAgentTransfer()
{	
	if(m_FMd5)
		delete []m_FMd5;
	if(m_Data)
		delete []m_Data;
}

void EvaFTAgentTransfer::setInfo(const std::string &file, const unsigned char *fileMd5, 
			const unsigned int size)
{
	if(!fileMd5)  return;

	m_FileName = file;
	if(!m_FMd5)   m_FMd5 = new unsigned char[16];
	memcpy(m_FMd5, fileMd5, 16);

	m_FileSize = size;
}

void EvaFTAgentTransfer::setData(const unsigned char *data, const unsigned int len)
{
	m_DataLength = len;

	if(m_Data) delete []m_Data;
	m_Data = new unsigned char[m_DataLength];
	memcpy(m_Data, data, m_DataLength);
}

const int EvaFTAgentTransfer::fillBody(unsigned char *buf)
{
	int pos=4; // ignore unknown 4 bytes
	int start = pos;
	pos+=2; // the following data length, set it at last
	
	switch(m_Type){
	case QQ_FILE_AGENT_TRANSFER_INFO:
		pos+=2; // the whole part length including these 2 bytes(save as 2 bytes ahead)
		memcpy(buf+pos, m_FMd5, 16); pos+=16;
		memcpy(buf+pos, EvaUtil::doMd5((char *)m_FileName.c_str(), m_FileName.length()), 16); pos+=16;
		pos+=EvaUtil::write32(buf+pos, m_FileSize);
		pos+=EvaUtil::write16(buf+pos, 0xffff & m_FileName.length());
		memcpy(buf+pos, m_FileName.c_str(), m_FileName.length());
		pos += m_FileName.length();
		pos+=8; // unknown 8 bytes;
		EvaUtil::write16(buf+start+2, pos-start-2); // write back length of this part
		break;
	case QQ_FILE_AGENT_TRANSFER_DATA:
		memcpy(buf+pos, m_Data, m_DataLength); pos+=m_DataLength;
		break;
	case QQ_FILE_AGENT_TRANSFER_REPLY:
		buf[pos++] = 0x02; // always 0x02
		break;
	case QQ_FILE_AGENT_TRANSFER_START:
		pos+=EvaUtil::write32(buf+pos, m_StartOffset);
		break;
	default:
		fprintf(stderr, "EvaFTAgentTransfer::fillBody -- Un-supported type: %d\n", m_Type);
		return -1;
	}
	EvaUtil::write16(buf+start, pos - start -2);
	return pos;
}



/** ========================================================================================== */

EvaFTAgentTransferReply::EvaFTAgentTransferReply(const int type, const unsigned char *buf, const int len)
	: EvaFTAgentPacket(buf, len), m_Type(type),
	m_ReplyCode(1), m_NextReplyCode(0xff),
	m_FileMd5(NULL), m_FileNameMd5(NULL),
	m_FileName(""), m_FileSize(0),
	m_Data(NULL), m_DataLength(0)
{
}

EvaFTAgentTransferReply::~EvaFTAgentTransferReply()
{
	if(m_FileMd5) delete []m_FileMd5;
	if(m_FileNameMd5) delete []m_FileNameMd5;
	if(m_Data) delete []m_Data;
}

const unsigned char * EvaFTAgentTransferReply::getFileMd5()
{
	return m_FileMd5;
}

const unsigned char * EvaFTAgentTransferReply::getFileNameMd5()
{
	return m_FileNameMd5;
}


const bool EvaFTAgentTransferReply::parseBody( unsigned char *buf, const int len)
{
	int pos=4; // 4 unknown bytes
	int contentsLen = EvaUtil::read16(buf+pos); pos += 2;
	if(contentsLen != (len - 6)) return false;

	switch(m_Type){
	case QQ_FILE_AGENT_TRANSFER_INFO:{
		if(EvaUtil::read16(buf+pos) != contentsLen) return false;
		pos+=2;
		if(!m_FileMd5) m_FileMd5 = new unsigned char[16];
		memcpy(m_FileMd5, buf+pos, 16); pos+=16;

		if(!m_FileNameMd5) m_FileNameMd5 = new unsigned char[16];
		memcpy(m_FileNameMd5, buf+pos, 16); pos+=16;

		m_FileSize = EvaUtil::read32(buf+pos); pos+=4;
		
		unsigned short strLen = EvaUtil::read16(buf+pos); pos+=2;
		char *strFile = new char[strLen + 1];
		memcpy(strFile, buf+pos, strLen);
		strFile[strLen] = 0x00;
		m_FileName.assign(strFile);
		delete strFile;
		}
		break;
	case QQ_FILE_AGENT_TRANSFER_DATA:
		m_DataLength = contentsLen;
		m_Data = new unsigned char[m_DataLength];
		memcpy(m_Data, buf+pos, m_DataLength); pos+=m_DataLength;
		break;
	case QQ_FILE_AGENT_TRANSFER_REPLY:
		m_NextReplyCode = buf[pos++];
		printf("EvaFTAgentTransferReply::parseBody -- m_NextReplyCode: 0x%2x\n", m_NextReplyCode);
		break;
	case QQ_FILE_AGENT_TRANSFER_START:
		m_ReplyCode = EvaUtil::read32(buf+pos); pos += 4;
		break;
	default:
		fprintf(stderr, "EvaFTAgentTransferReply::parseBody -- Un-supported type: %d\n", m_Type);
		return false;
	}

	return true;
}


/** ========================================================================================== */

EvaFTAgentAckReady::EvaFTAgentAckReady()
	: EvaFTAgentPacket(QQ_FILE_AGENT_CMD_READY)
{
}

EvaFTAgentAckReady::~EvaFTAgentAckReady()
{
}

const int EvaFTAgentAckReady::fillBody(unsigned char *buf)
{
	int pos=0;
	unsigned char *raw = new unsigned char [4];
	memset(raw, 0, 4); pos+=4;
	int outLen = 64;
	EvaCrypt::encrypt(raw, pos, m_FileAgentKey, buf, &outLen);
	pos = outLen;
	delete []raw;
	return pos;
}



/** ========================================================================================== */


EvaFTAgentAskReady::EvaFTAgentAskReady(const unsigned char *buf, const int len)
	: EvaFTAgentPacket(buf, len),
	m_ReplyCode(1)
{
}

const bool EvaFTAgentAskReady::parseBody(unsigned char *buf, const int len)
{
	int rawLen = 256;
	unsigned char *raw = new unsigned char[256];
	
	if(!EvaCrypt::decrypt(buf, len, m_FileAgentKey, raw, &rawLen)){
		fprintf(stderr, "EvaFTAgentAskReady::parseBody -- decrypt failed\n");
		delete []raw;
		return false;
	}
	
	int pos = 0;
	m_ReplyCode = EvaUtil::read32(raw); pos+=4;
	delete []raw;
	return true;
}



/** ========================================================================================== */

EvaFTAgentStart::EvaFTAgentStart()
	: EvaFTAgentPacket(QQ_FILE_AGENT_CMD_START)
{
}

EvaFTAgentStart::~EvaFTAgentStart()
{
}

const int EvaFTAgentStart::fillBody(unsigned char *buf)
{
	int pos=0;
	unsigned char *raw = new unsigned char [2];
	
	// unknown fixed bytes
	raw[pos++] = 0x07;
	raw[pos++] = 0xd0;

	int outLen = 64;
	EvaCrypt::encrypt(raw, pos, m_FileAgentKey, buf, &outLen);
	pos = outLen;
	delete []raw;
	return pos;
}



/** ========================================================================================== */


EvaFTAgentStartReply::EvaFTAgentStartReply(const unsigned char *buf, const int len)
	: EvaFTAgentPacket(buf, len)
{
}

const bool EvaFTAgentStartReply::parseBody(unsigned char *buf, const int len)
{
	int rawLen = 256;
	unsigned char *raw = new unsigned char[256];
	
	if(!EvaCrypt::decrypt(buf, len, m_FileAgentKey, raw, &rawLen)){
		fprintf(stderr, "EvaFTAgentStartReply::parseBody -- decrypt failed\n");
		delete []raw;
		return false;
	}
	
	// looks like all unknown contents are fixed values
	int pos = 0;	
	unsigned int part1 = EvaUtil::read32(raw); pos+=4; // this should be 1
	if(part1 != 1)
		fprintf(stderr, "EvaFTAgentStartReply::parseBody -- part1 is 0x%8x\n", part1);
	unsigned char part2 = raw[pos++];  // should be 0x90
	if(part2 != 0x90)
		fprintf(stderr, "EvaFTAgentStartReply::parseBody -- part2 is 0x%2x\n", 0xff&part2);
	pos+=2; // 2 0x00s
	unsigned char part3 = raw[pos++]; // should be 0x2d
	if(part3 != 0x2d)
		fprintf(stderr, "EvaFTAgentStartReply::parseBody -- part3 is 0x%2x\n", 0xff&part3);
	pos+=4; // all 0x00s
	
	delete []raw;
	return true;
}


/** =================================================================================================== */

EvaFTSynCreate::EvaFTSynCreate()
	: EvaFTSynPacket(QQ_FILE_SYN_CMD_CREATE),
	m_Token(NULL), m_TokenLength(0), m_BuddyId(0)
{
}

EvaFTSynCreate::~EvaFTSynCreate()
{
	if(m_Token)
		delete []m_Token;
}

void EvaFTSynCreate::setFileAgentToken(const unsigned char *token, const int len)
{
	if(!token) return;
	if(m_Token) delete [] m_Token;
	m_Token = new unsigned char[len];
	memcpy(m_Token, token, len);
	m_TokenLength = len;
}

const int EvaFTSynCreate::fillBody(unsigned char *buf)
{
	if(!m_Token) return -1;
	int pos=0;
	pos += EvaUtil::write16(buf, m_TokenLength);
	memcpy(buf+pos, m_Token, m_TokenLength); pos+=m_TokenLength;
	
	int rawPos=0;
	unsigned char *raw = new unsigned char [20];
	memset(raw, 0, 4); rawPos+=4;
	rawPos += EvaUtil::write32(raw + rawPos, m_BuddyId);

	int outLen = 1000;
	EvaCrypt::encrypt(raw, rawPos, m_FileAgentKey, buf+pos, &outLen);
	pos += outLen;
	delete []raw;
	return pos;
}



/** ========================================================================================== */

EvaFTSynCreateReply::EvaFTSynCreateReply(const unsigned char *buf, const int len)
	: EvaFTSynPacket(buf, len),
	m_ReplyCode(QQ_FILE_AGENT_CREATE_REDIRECT),
	m_Ip(0), m_Port(0)
{
}

const bool EvaFTSynCreateReply::parseBody( unsigned char *buf, const int len)
{
	int rawLen = 256;
	unsigned char *raw = new unsigned char[256];
	
	if(!EvaCrypt::decrypt(buf, len, m_FileAgentKey, raw, &rawLen)){
		fprintf(stderr, "EvaFTSynCreateReply::parseBody -- decrypt failed\n");
		delete []raw;
		return false;
	}
	
	int pos = 0;
	m_ReplyCode = EvaUtil::read16(raw); pos+=2;
	switch(m_ReplyCode){
	case QQ_FILE_AGENT_CREATE_OK:
		m_Session = EvaUtil::read32(raw+pos); pos+=4;
		m_Ip  = EvaUtil::read32(raw+pos); pos+=4;
		m_Port = EvaUtil::read16(raw+pos); pos+=2;
		// all following are 0x00s, 6 more bytes
		break;
	default:
		fprintf(stderr, "EvaFTSynCreateReply::parseBody -- un-supported reply code: 0x%4x\n", 0xffff&m_ReplyCode);
		delete []raw;
		return false;
	}
	delete []raw;
	return true;
}











