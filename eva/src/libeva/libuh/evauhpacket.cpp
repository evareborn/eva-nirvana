/***************************************************************************
 *   Copyright (C) 2004-2006 by yunfan                                     *
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

#include "evauhpacket.h"
#include "../evadefines.h"
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <string.h>
#include <stdio.h>
#include <cstring>

unsigned short EvaUHPacket::seq_random    = 0;
unsigned short EvaUHPacket::seq_info      = 0;
unsigned short EvaUHPacket::seq_transfer = 0;

EvaUHPacket::EvaUHPacket(const unsigned char *buf, const int len)
	: buffer(NULL), mBufferLen(len), mVersion(QQ_CLIENT_VERSION),
	 mIsData(false), mNumPackets(1), mPacketNum(0)
{
	if(!buf) {
		mBodyStart = -1; // means error occurred
		return;
	}
	buffer = new unsigned char[mBufferLen];
	memcpy(buffer, buf, mBufferLen);
}

EvaUHPacket::EvaUHPacket(const bool isInfo)
	: buffer(NULL), mVersion(QQ_CLIENT_VERSION), mIsData(false), 
	mNumPackets(1), mPacketNum(0)
{
	mCommand = isInfo?UH_CMD_INFO:UH_CMD_TRANSFER;
}

EvaUHPacket::~EvaUHPacket()
{
	if(buffer) delete buffer;
}

const bool EvaUHPacket::parse()
{
	if(mBodyStart == -1) return false;
	mBodyStart = parseHead();
	if(!mBodyStart) return false;
	return parseBody();
}

const bool EvaUHPacket::fill(unsigned char *buf, int *len)
{
	if(!fillHead(buf)) return false;
	int tmp;
	bool result = fillBody(buf + mBodyStart,&tmp);
	(*len) = mBodyStart + tmp;
	return result;
}

const bool EvaUHPacket::parseBody()
{
	fprintf(stderr, "EvaUHPacket::parseBody\n");
	return false;
}

const bool EvaUHPacket::fillBody(unsigned char */*buf*/, int */*len*/)
{
	fprintf(stderr, "EvaUHPacket::fillBody\n");
	return false;
}

const int EvaUHPacket::parseHead()
{
	int pos = 0;
	if(buffer[pos++] != UH_HEAD_TAG) return false;
	mCommand = buffer[pos++];
	pos+=2; // random sequence
	pos+=2; // 2 bytes 0x00
	pos+=2; // packet sequence which depends on the mCommand, don't care, ignore
	pos+=6; // 6 bytes 0x00

	unsigned short tmp2;
	memcpy(&tmp2, buffer+pos, 2);
	mIsData = ntohs(tmp2)?true:false;
	pos+=2; 

	pos+=24; // 24 bytes 0x00
	mNumPackets = buffer[pos++];
	mPacketNum = buffer[pos++];
	
	pos++; // 0x00, 1 byte
	pos+=2; // version, also always 0x0000 for server
	pos++; // 0x00, 1 byte

	return pos;
}

const bool EvaUHPacket::fillHead(unsigned char *buf)
{
	if(!buf) return false;
	int pos = 0;

	buf[pos++]=UH_HEAD_TAG;
	buf[pos++]=mCommand;

	unsigned short tmp2;
	tmp2 = htons(seq_random++);
	memcpy(buf+pos, &tmp2, 2); pos+=2;
	
	memset(buf+pos, 0, 2); pos+=2;

	if(mCommand==UH_CMD_INFO){
		tmp2 = htons(seq_info++);
	} else if(mCommand==UH_CMD_TRANSFER){
			tmp2=htons(seq_transfer++);
		} else {
			return false;
		}
	memcpy(buf+pos, &tmp2, 2); pos+=2;

	memset(buf+pos, 0, 6); pos+=6;
	memset(buf+pos, 0, 2); pos+=2; // mIsData always be 0x0000 in client side
	memset(buf+pos, 0, 24); pos+=24;	
	
	if(mPacketNum >= mNumPackets)
		return false;
	buf[pos++] = mNumPackets;
	buf[pos++] = mPacketNum;

	buf[pos++] = 0x00;

	tmp2 = htons(mVersion);
	memcpy(buf+pos, &tmp2, 2); pos+=2;

	buf[pos++] = 0x00;
	mBodyStart = pos;
	return true;
}
