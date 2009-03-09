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
 
#include "evaextrainfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring>

RequestExtraInfoPacket::RequestExtraInfoPacket()
	: OutPacket(QQ_CMD_REQUEST_EXTRA_INFORMATION, true), 
	mOffset(0)
{
}

RequestExtraInfoPacket::RequestExtraInfoPacket(const unsigned short offset)
	: OutPacket(QQ_CMD_REQUEST_EXTRA_INFORMATION, true), 
	mOffset(offset)
{
}

RequestExtraInfoPacket::RequestExtraInfoPacket(const RequestExtraInfoPacket &rhs)
	: OutPacket(rhs)
{
	(*this) = rhs;
}
 
RequestExtraInfoPacket &RequestExtraInfoPacket::operator=(const RequestExtraInfoPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	mOffset = rhs.getOffset();
	return *this;
}

int RequestExtraInfoPacket::putBody(unsigned char *buf)
{
	int pos=0;
	buf[pos++]=0x01; // at the moment ,  I only find this sub-command, so just hard-code here
	unsigned short tmp;
	tmp = htons(mOffset);
	memcpy(buf + pos, &tmp, 2); pos+=2;
	return pos;
}


/* =========================================================== */

RequestExtraInfoReplyPacket::RequestExtraInfoReplyPacket(unsigned char *buf, int len)
	: InPacket(buf, len)
{
}

RequestExtraInfoReplyPacket::RequestExtraInfoReplyPacket(const RequestExtraInfoReplyPacket &rhs)
	: InPacket(rhs)
{
	*this = rhs;
}

RequestExtraInfoReplyPacket &RequestExtraInfoReplyPacket::operator=(const RequestExtraInfoReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	mReplyCode = rhs.getReplyCode();
	mOffset = rhs.getOffset();
	mMembers = rhs.getMembers();
	return *this;
}

void RequestExtraInfoReplyPacket::parseBody()
{
	int pos = 0;
	mReplyCode = decryptedBuf[pos++];
	if(mReplyCode != 1){
		fprintf(stderr, "RequestExtraInfoReplyPacket::parseBody -- Unknow reply code :%4x\n", 0xffff&mReplyCode);
		return;
	}
	mOffset = ntohs(*(unsigned short *)(decryptedBuf + pos)); pos+=2;

	unsigned char bitWidth = decryptedBuf[pos++];
	if(bitWidth > sizeof(long long)){
		fprintf(stderr, "Warnning[RequestExtraInfoReplyPacket]: bit width flag is %d, out of range for long long type(%d chars)\n", bitWidth, sizeof(long long));
	}
	int id;
	long long tmp;
	mMembers.clear();
	while(pos<bodyLength){
		id = ntohl(*(int *)(decryptedBuf + pos)); pos+=4;// QQ number
		tmp = 0;
		memcpy(&tmp, decryptedBuf + pos, (sizeof(long long) > bitWidth)?bitWidth:(sizeof(long long))); // we get the first 8 bytes, ignore the rest if it is greater than "long long"
		pos += bitWidth;
		mMembers[id] = tmp;// information flags( signature, QQ tang, QQ album, QQ firend making ), the length should
				// be same as declared before ( 0x04 now)
	}
}


/* =========================================================== */

SignaturePacket::SignaturePacket( const unsigned char type )
	: OutPacket(QQ_CMD_SIGNATURE_OP, true), mType(type)
{
}

SignaturePacket::SignaturePacket( const SignaturePacket & rhs )
	: OutPacket(rhs)
{
	(*this) = rhs;
}

SignaturePacket & SignaturePacket::operator =( const SignaturePacket & rhs )
{
	*((OutPacket *)this) = (OutPacket)rhs;
	mType = rhs.getType();
	mSignature = rhs.getSignature();
	mMembers = rhs.getMembers();
	return *this;
}

int SignaturePacket::putBody( unsigned char * buf )
{
	int pos=0;
	buf[pos++] = mType;
	
	switch(mType){
	case QQ_SIGNATURE_MODIFY:{
		buf[pos++] = 0x00;
		int len = mSignature.length();
		buf[pos++] = 0xff & len;
		memcpy(buf + pos, mSignature.c_str(), len); pos+=len;
		}
		break;
	case QQ_SIGNATURE_DELETE:
		break;
	case QQ_SIGNATURE_REQUEST:{
		unsigned short tmp2;
		tmp2 = htons(mMembers.size());
		memcpy(buf + pos, &tmp2, 2); pos+=2;
		
		unsigned int tmp4;
		std::map<unsigned int, unsigned int>::iterator iter;
		for(iter = mMembers.begin(); iter!=mMembers.end(); ++iter){
			tmp4 = htonl(iter->first);   // QQ number
			memcpy(buf + pos, &tmp4, 4); pos+=4;
			
			tmp4 = htonl(iter->second);   // time or 0x00 00 00 01 / 0x00 00 00 02
			memcpy(buf + pos, &tmp4, 4); pos+=4;
		}
		}
	}
	return pos;
}


/* =========================================================== */

SignatureReplyPacket::SignatureReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len)
{
}

SignatureReplyPacket::SignatureReplyPacket( const SignatureReplyPacket & rhs )
	: InPacket(rhs)
{
	*this = rhs;
}

SignatureReplyPacket & SignatureReplyPacket::operator =( const SignatureReplyPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	mType = rhs.getType();
	mReplyCode = rhs.getReplyCode();
	mMembers = rhs.getMembers();
	return *this;
}

void SignatureReplyPacket::parseBody( )
{
	int pos=0;
	mType = decryptedBuf[pos++];
	mReplyCode = decryptedBuf[pos++];
	if(mReplyCode != 0x00) return;
	
	switch(mType){
	case QQ_SIGNATURE_MODIFY:
	case QQ_SIGNATURE_DELETE:
		break;
	case QQ_SIGNATURE_REQUEST:{
		uint tmp4;
		memcpy(&tmp4, decryptedBuf+pos, 4); pos+=4; // next start QQ number must equal or greater than this number
		mNextStartId = ntohl(tmp4);
		int id, sigLen;
		while(pos<bodyLength){
			memcpy(&tmp4, decryptedBuf+pos, 4); pos+=4; // QQ
			id = ntohl(tmp4);
			
			SignatureElement element;
			
			memcpy(&tmp4, decryptedBuf+pos, 4); pos+=4; // the last modify time for the QQ above
			element.lastModifyTime = ntohl(tmp4);
			
			sigLen = decryptedBuf[pos++];        // the length of the  signature
			// MEMORY LEAK!
			char *str = new char[sigLen+1];
			memcpy(str, decryptedBuf + pos, sigLen); pos+=sigLen;  // the signature
			str[sigLen] = 0x00;
			element.signature.assign(str);
			
			mMembers[id] = element;
			delete[] str;
		}
		}
		break;
	}
}

