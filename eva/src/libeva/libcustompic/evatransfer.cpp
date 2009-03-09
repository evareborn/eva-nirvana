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
 
#include "evatransfer.h"
#include "../evautil.h"
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring>
EvaPicTransferPacket::EvaPicTransferPacket() 
	: EvaPicOutPacket(QQ_05_CMD_TRANSFER, false), fragment(NULL)
{
	cryptPosition = NO_CRYPTOGRAPH;
        requestSend = false;
        dataReply = false;
}

EvaPicTransferPacket::EvaPicTransferPacket(const bool isDataPacket, const bool isLastPacket)
	: EvaPicOutPacket(QQ_05_CMD_TRANSFER, isDataPacket?(isLastPacket?true:false):true)
	, fragment(NULL)
{
	cryptPosition = NO_CRYPTOGRAPH;
        isData = isDataPacket;
        isLast = isLastPacket;
        requestSend = true;
}

EvaPicTransferPacket::EvaPicTransferPacket(const EvaPicTransferPacket &rhs)
	: EvaPicOutPacket(rhs)
{
	*this = rhs;
}

EvaPicTransferPacket::~EvaPicTransferPacket()
{
	if(fragment)
		delete fragment;
}

EvaPicTransferPacket &EvaPicTransferPacket::operator=(const EvaPicTransferPacket &rhs)
{
	*((EvaPicOutPacket*)this) = (EvaPicOutPacket)rhs;
	sessionID = rhs.getSessionID();
	
	memcpy(md5, rhs.getMD5(), 16); // always 16 bytes long
	imageLength = rhs.getImageLength();
	fileName = rhs.getFileName();
	
	fragLength = rhs.getFragmentLength();
	memcpy(fragment, rhs.getFragment(), fragLength);
	isData = rhs.isDataPacket();
	isLast = rhs.isLastPacket();
	
	requestSend = rhs.isRequestSend();
	dataReply = rhs.isDataReply();
	return *this;
}

void EvaPicTransferPacket::setMd5(const unsigned char *value)
{
	memcpy(md5, value, 16);
}

void EvaPicTransferPacket::setFragment(const unsigned char *data, const unsigned int len)
{
	if(!fragment)
		fragment = new unsigned char[len];
	fragLength = len;
	memcpy(fragment, data, fragLength);
}

int EvaPicTransferPacket::putBody(unsigned char *buf)
{
	int pos=0;
	unsigned int tmp4_1 = htonl(0x01000000), tmp4_2 = 0;
	if(!requestSend){
		memcpy(buf+pos, &tmp4_1, 4);
		pos+=4;
		memcpy(buf+pos, &tmp4_2, 4);
		pos+=4;                    // ignore 8 bytes
		
		tmp4_1 = htonl(sessionID);
		memcpy(buf+pos, &tmp4_1, 4);
		pos+=4;
		
		memset(buf+pos, 0, 4);
		pos+=4;  // unknown 4 bytes, all 0s
		
		unsigned short tmp2;
		if(dataReply){
			tmp2 = (unsigned short)htons(1);
			memcpy(buf+pos, &tmp2, 2);  //  the following length
			pos+=2;
			
			buf[pos++] = 0x02;   // the contents
		}else{
			tmp2 = (unsigned short)htons(4);
			memcpy(buf+pos, &tmp2, 2);     //  the following length
			pos+=2;
			
			memset(buf+pos, 0, 4);     // the contents
			pos+=4;
		}
	}else{
		if(isData){
			if(isLast){ // if is last , should be random 8 bytes
				tmp4_1 = 1;
				tmp4_2 = 0;
				memcpy(buf+pos, &tmp4_1, 4); pos+=4;
				memcpy(buf+pos, &tmp4_2, 4); pos+=4;
			}else{
				buf[pos++] = 0x01;
				memset(buf+pos, 0, 6); pos+=6;
				buf[pos++] = 0x01;
			}
			tmp4_1 = htonl(sessionID);
			memcpy(buf+pos, &tmp4_1, 4); pos+=4;
			
			memset(buf+pos, 0, 4); pos+=4;
			
			unsigned short tmp2 = htons(fragLength);
			memcpy(buf+pos, &tmp2, 2); pos+=2;
			
			memcpy(buf+pos, fragment, fragLength); pos+=fragLength;
		}else{
			tmp4_1 = 1;
			tmp4_2 = 0;
			memcpy(buf+pos, &tmp4_1, 4); pos+=4;
			memcpy(buf+pos, &tmp4_2, 4); pos+=4; // unknown 8 bytes
			
			tmp4_1 = htonl(sessionID);
			memcpy(buf+pos, &tmp4_1, 4); pos+=4;
			
			memset(buf+pos, 0, 4); pos+=4;
			
			pos+=2;
			int start = pos;
			pos+=2;
			memcpy(buf+pos, md5, 16); pos+=16;
			
			memcpy(buf+pos, EvaUtil::doMd5((char *)fileName.c_str(), fileName.length()), 16); pos+=16;
			
			tmp4_1 = htonl(imageLength);
			memcpy(buf+pos, &tmp4_1, 4); pos+=4;
			
			short tmp2 = htons(fileName.length());
			memcpy(buf+pos, &tmp2, 2); pos+=2;
			
			memcpy(buf+pos, fileName.c_str(), fileName.length()); pos+=fileName.length();
			
			memset(buf+pos, 0, 8); pos+=8;
			
			tmp2 = htons((short)(pos - start));
			memcpy(buf+start-2, &tmp2, 2);
			memcpy(buf+start, &tmp2, 2);
		}
	}
	return pos;
}


/************************************************************************************************************************************************/


EvaPicTransferReplyPacket::EvaPicTransferReplyPacket(unsigned char *buf, int len)
	: EvaPicInPacket(buf, len, NO_CRYPTOGRAPH), data(NULL)
{
}

EvaPicTransferReplyPacket::EvaPicTransferReplyPacket(const EvaPicTransferReplyPacket &rhs)
	: EvaPicInPacket(rhs)
{
	*this = rhs;
}

EvaPicTransferReplyPacket::~EvaPicTransferReplyPacket()
{
	if(data)
		delete data;
}

EvaPicTransferReplyPacket &EvaPicTransferReplyPacket::operator=(const EvaPicTransferReplyPacket &rhs)
{
	*((EvaPicInPacket *)this) = (EvaPicInPacket)rhs;
	sessionID = rhs.getSessionID();
	memcpy(md5, rhs.getMd5(), 16);
	memcpy(fileNameMd5, rhs.getFileNameMd5(), 16);
	fileName = rhs.getFileName();
	imageLength = rhs.getImageLength();
	
	dataLength = rhs.getDataLength();
	memcpy(data, rhs.getData(), dataLength);
	return *this;
}

void EvaPicTransferReplyPacket::parseBody()
{
// 	printf("\n\nEvaPicTransferReplyPacket::parseBody\n");
// 	for(int i=0; i<bodyLength; i++){
//  		if(!(i%8)) printf("\n%d: ",i);
//  		char t = decryptedBuf[i];
//  		printf("%2x ", (uint8_t)t);
// 	}
// 	printf("\n---------=======================---------\n\n");

	int pos=0;
	pos+=8; // ignore unknown 8 bytes
	
	unsigned int tmp4;
	memcpy(&tmp4, decryptedBuf+pos, 4); pos+=4;
	sessionID = ntohl(tmp4);
	
	pos+=4;  // unknown 4 bytes
	
	unsigned short tmp2;
	memcpy(&tmp2, decryptedBuf+pos, 2); pos+=2;
	unsigned short len = ntohs(tmp2);
	
	if(source != QQ_CLIENT_VERSION && !sequence){
		pos+=2;
		memcpy(md5, decryptedBuf+pos, 16); pos+=16;
		
		memcpy(fileNameMd5, decryptedBuf+pos, 16); pos+=16;
		
		memcpy(&tmp4, decryptedBuf+pos, 4); pos+=4;
		imageLength = ntohl(tmp4);
		
		unsigned short fileNameLength;
		memcpy(&tmp2, decryptedBuf+pos, 2); pos+=2;
		fileNameLength = ntohs(tmp2);
		
		char *str = new char[fileNameLength + 1];
		memcpy(str, decryptedBuf+pos, fileNameLength); pos+= fileNameLength;
		str[fileNameLength] = 0x00;
		fileName.assign(str);
		delete str;
	}else{
		data = new unsigned char[len];
		dataLength = len;
		
		memcpy(data, decryptedBuf+pos, len); pos+= len;
	}
}




