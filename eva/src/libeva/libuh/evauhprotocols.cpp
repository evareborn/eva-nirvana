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

#include "evauhprotocols.h"
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <string.h>
#include <stdio.h>
#include <cstring>

EvaUHInfoRequest::EvaUHInfoRequest() 
	:EvaUHPacket(true)
{	
}

const bool EvaUHInfoRequest::fillBody(unsigned char *buf, int *len)
{
	if(!mList.size()) return false;
	
	int pos=0, sizeOffset;
	memset(buf, 0, 8); pos+=8;

	sizeOffset = pos;  // we will set the length of this packet last
	pos+=2;
	
	buf[pos++] = 0x01;
	buf[pos++] = 0x00;

	unsigned short tmp2 = htons((mList.size()>20)?20:mList.size());
	memcpy(buf+pos, &tmp2, 2); pos+=2;

	unsigned int tmp4;
	int count=0;
	std::list<unsigned int>::iterator iter;
	for(iter=mList.begin(); iter!=mList.end(); ++iter){
		tmp4 = htonl(*iter);
		memcpy(buf+pos, &tmp4, 4); pos+=4;
		count++;
		if(count>=20) break;
	}
	
	*len = pos;

	// now we set the packet size
	tmp2 = htons(pos - sizeOffset - 2);
	memcpy(buf+sizeOffset, &tmp2, 2);

	return true;
}


/****************************************************************************/


EvaUHInfoReply::EvaUHInfoReply(const unsigned char *buf, const int len) 
	:EvaUHPacket(buf, len)
{	
}

const bool EvaUHInfoReply::parseBody()
{
	int pos= mBodyStart;
	pos+=8;
	unsigned short tmp2;
	memcpy(&tmp2, buffer+pos, 2); pos+=2;
	if( (pos + ntohs(tmp2)) != mBufferLen ){
		fprintf(stderr, "EvaUHInfoReply::parseBody -- Bad Packet\n");
		return false;
	}

	// 0x00: ok, there some pictures, 0x01: error, probably no picture exists
	replyCode = buffer[pos++];
	// if thing's going wrong, we ignore all following stuff
	// actually, only 3 bytes after this, 0x02 0x00 0x00
	if(replyCode == 0x01)
		return false;
	pos++; // 0x02

	mList.clear();
		
	int num = 0;
	memcpy(&tmp2, buffer+pos, 2); pos+=2;
	num = ntohs(tmp2);
	
	unsigned int tmp4;
	while(pos<mBufferLen){
		if( (mBufferLen - pos) < 24) {
			fprintf(stderr, "EvaUHInfoReply::parseBody -- Bad Packet\n");
			return false;			
		}
		UHInfoItem item;
		memcpy(&tmp4, buffer+pos, 4); pos+=4;
		item.id = ntohl(tmp4);
		memcpy(&tmp4, buffer+pos, 4); pos+=4;
		item.sessionId = tmp4;  // don't need ntohl, we can use it directly :)
		memcpy(item.md5, buffer+pos, 16); pos+=16;
		mList.push_back(item);
	}
	return true;
}


/****************************************************************************/



EvaUHTransferRequest::EvaUHTransferRequest() 
	:EvaUHPacket(false), mStart(0xffffffff), mEnd(0)
{	
}

const bool EvaUHTransferRequest::fillBody(unsigned char *buf, int *len)
{
	int pos=0, sizeOffset;
	memset(buf, 0, 8); pos+=8;

	sizeOffset = pos;  // we will set the length of this packet last
	pos+=2;
	
	unsigned int tmp4;
	tmp4 = htonl(mId);
	memcpy(buf+pos, &tmp4, 4); pos+=4;
	
	buf[pos++] = 0x01; // unknown 1 byte, always 1
	
	memcpy(buf+pos, &mSessionId, 4); pos+=4; // don't need convert

	tmp4 = htonl(mStart);
	memcpy(buf+pos, &tmp4, 4); pos+=4;
	
	tmp4 = htonl(mEnd);
	memcpy(buf+pos, &tmp4, 4); pos+=4;
	
	*len = pos;
	
	// now we set the packet size
	unsigned short tmp2 = htons(pos - sizeOffset - 2);
	memcpy(buf+sizeOffset, &tmp2, 2);

	return true;
}



/****************************************************************************/



EvaUHTransferReply::EvaUHTransferReply(const unsigned char *buf, const int len) 
	:EvaUHPacket(buf, len)
{	
}

const bool EvaUHTransferReply::parseBody()
{
	// if not data packet, it's a transfer start notification, empty body
	if(!mIsData)       
		return true; 

	int pos= mBodyStart;
	pos+=8;
	unsigned short tmp2;
	memcpy(&tmp2, buffer+pos, 2);  pos+=2;// the length of following contents
	if( (pos + ntohs(tmp2)) != mBufferLen ){
		fprintf(stderr, "EvaUHInfoReply::parseBody -- Bad Packet\n");
		return false;
	}
	
	pos++; // byte 0x00
	
	unsigned int tmp4;
	memcpy(&tmp4, buffer+pos, 4); pos+=4;
	mId = ntohl(tmp4);
	
	memcpy(&mSessionId, buffer+pos, 4); pos+=4;
	
	memcpy(&tmp4, buffer+pos, 4); pos+=4;   // total size of this file
	mFileSize = ntohl(tmp4);
	
	memcpy(&tmp4, buffer+pos, 4); pos+=4;   // the start position of this packet
	mStart = ntohl(tmp4);
	
	memcpy(&tmp4, buffer+pos, 4); pos+=4;   // size of this data packet
	mPartSize = ntohl(tmp4);
	
	mPartData = buffer+pos;                 // the actual data
	
	return true;
}

