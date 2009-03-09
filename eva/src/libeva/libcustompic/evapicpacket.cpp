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
#include "evapicpacket.h"
#include "../evacrypt.h"
#include <string.h>
#include <stdio.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring>

unsigned int EvaPicPacket::myQQ = 0;
unsigned char *EvaPicPacket::fileAgentKey = NULL;

EvaPicPacket::EvaPicPacket( unsigned char *buf, int len)
	: key(NULL)
{
	parseHeader(buf);
	tail = buf[len-1];
}

EvaPicPacket::EvaPicPacket( const unsigned short src, const unsigned short cmd, const unsigned short seq )
	: source(src), command(cmd), sequence(seq), key(NULL), cryptPosition(NO_CRYPTOGRAPH)
{
}

EvaPicPacket::EvaPicPacket( unsigned char *buf, int *len, const int cryptStart)
	: key(NULL), cryptPosition(cryptStart)
{
	int pos = parseHeader(buf);
	tail = buf[(*len)-1];
	(*len)=(*len) - pos - ((tail==FAMILY_05_TAIL)?1:0);
	memcpy(buf, buf+pos, *len);

}

EvaPicPacket::EvaPicPacket(const EvaPicPacket &rhs)
{
	*this = rhs;
}

EvaPicPacket::~EvaPicPacket()
{
	if(key)
		delete key;
}

const int EvaPicPacket::hashCode()
{
	return ((int)sequence << 16) | command;
}

const int EvaPicPacket::parseHeader(unsigned char *buf)
{
	int pos=0;
	
	unsigned short tmp;
	
	header = buf[pos++];  // should be always 0x05
	
	memcpy(&tmp, buf+pos, 2);
	source = ntohs(tmp);
	pos+=2;
	
	memcpy(&tmp, buf+pos, 2);
	packetLength = ntohs(tmp);
	pos+=2; 
	
	memcpy(&tmp, buf+pos, 2);
	command = ntohs(tmp);
	pos+=2;
	
	memcpy(&tmp, buf+pos, 2);
	sequence = ntohs(tmp);
	pos+=2;
	
	unsigned int tmp4;
	memcpy(&tmp4, buf+pos, 4);
	myQQ = ntohl(tmp4);
	pos+=4;
	return pos;
}

EvaPicPacket &EvaPicPacket::operator=(const EvaPicPacket &rhs)
{
	header = rhs.getHeader();
	tail = rhs.getTail();
	source = rhs.getSource();
	command = rhs.getCommand();
	sequence = rhs.getSequence();
	myQQ = rhs.getQQ();
	cryptPosition = rhs.getCryptPosition();
	return *this;
}

void EvaPicPacket::setFileAgentKey(const unsigned char *key)
{
	if(!fileAgentKey)
		fileAgentKey = new unsigned char[16];
	memcpy(fileAgentKey, key, 16);
}

void EvaPicPacket::setKey(const unsigned char *k)
{
	key = new unsigned char[16];
	memcpy(key, k, 16);
}

void EvaPicPacket::clearKey()
{
	if(fileAgentKey)
		delete fileAgentKey;
	fileAgentKey = NULL;
}

/* =========================================================== */

// EvaPicOutPacket starts here
short EvaPicOutPacket::sequenceStart = 5;   // could be any random number

EvaPicOutPacket::EvaPicOutPacket(const unsigned short command, const bool ack)
	: EvaPicPacket(QQ_CLIENT_VERSION, command, ((sequenceStart++)%0xffff))
{
	mNeedAck = ack;
	resendCount = 5;
}

EvaPicOutPacket::EvaPicOutPacket(const EvaPicOutPacket &rhs) 
	: EvaPicPacket(rhs)
{
	*this = rhs;
}

const bool EvaPicOutPacket::fill(unsigned char *buf, int *len) 
{
	int headerLen = 0, bodyLen=0, encryptedLen = 0;
	unsigned char *bodyBuf = new unsigned char[MAX_PACKET_SIZE];
	unsigned char *encrypted = new unsigned char[MAX_PACKET_SIZE];
	
	headerLen = putHead(buf);
	bodyLen   = putBody(bodyBuf);
	encryptedLen = bodyLen;
	
	int start = getCryptPosition();
	if(start == -1){
		memcpy(buf + headerLen, bodyBuf, bodyLen);
		buf[headerLen + bodyLen] = FAMILY_05_TAIL;
		*len = headerLen + bodyLen + 1;
		short tmp2 = htons(*len);
		memcpy(buf+3, &tmp2, 2);
		delete bodyBuf;
		delete encrypted;
		return true;
	}
	
	encryptBody(bodyBuf + start, bodyLen - start, encrypted, &encryptedLen); // encrypt
	
	memcpy(buf + headerLen, bodyBuf, start);                               // copy un-encrypted body
	memcpy(buf + headerLen + start, encrypted, encryptedLen);             // copy encrypted body
	buf[headerLen + start + encryptedLen] = FAMILY_05_TAIL;                // the last byte
	*len = headerLen + start + encryptedLen + 1;
	
	short tmp2 = htons(*len);
	memcpy(buf+3, &tmp2, 2);
	
	delete bodyBuf;
	delete encrypted;
	return true;
}

EvaPicOutPacket &EvaPicOutPacket::operator=( const EvaPicOutPacket &rhs)
{
	*((EvaPicPacket*)this) = (EvaPicPacket)rhs;
	mNeedAck = rhs.needAck();
	resendCount = rhs.getResendCount();
        return *this;
}

int EvaPicOutPacket::putBody(unsigned char *buf)
{
	fprintf(stderr, "In EvaPicOutPacket\n"); 
	buf[0]=0; 
	return 0;
}

int EvaPicOutPacket::putHead(unsigned char *buf) 
{
	int pos=0;
	
	buf[pos++] = FAMILY_05_TAG;
	
	short tmp = htons(getSource());
	memcpy(buf+pos, &tmp, 2);
	pos+=2; 

	memset(buf+pos, 0, 2); // 2 unknown bytes, all 0
	pos+=2;
		
	tmp = htons(getCommand());
	memcpy(buf+pos, &tmp, 2);
	pos+=2; 
	
	tmp = htons(getSequence());
	memcpy(buf+pos, &tmp, 2);
	pos+=2; 
	
	int id = htonl(getQQ());
	memcpy(buf+pos, &id, 4);
	pos+=4;
		
	return pos;
}

void EvaPicOutPacket::encryptBody(unsigned char *b, int length, 
                            unsigned char *body, int *bodyLen) 
{
	if(fileAgentKey == NULL) {     
		fprintf(stderr, "EvaPicOutPacket->encryptBody: file agent key not set yet, set encrypted length to 0\n");
		*bodyLen = 0;
		return;
	} 
	EvaCrypt::encrypt(b, length, key?key:fileAgentKey, body, bodyLen); 
		
}



/* =========================================================== */


EvaPicInPacket::EvaPicInPacket( unsigned char *buf, int len)
	: EvaPicPacket(buf, len),decryptedBuf(NULL)
{
	rawBody = new unsigned char [len];
	rawLength = len;
	memcpy(rawBody, buf, rawLength);
}

EvaPicInPacket::EvaPicInPacket( unsigned char *buf, int len, const int cryptStart)
	: EvaPicPacket(buf, &len, cryptStart),
	bodyLength(0), rawBody(NULL)
{
	decryptedBuf = new unsigned char [MAX_PACKET_SIZE];
	decryptBody(buf, len);
}

EvaPicInPacket::EvaPicInPacket(const EvaPicInPacket &rhs)
	: EvaPicPacket(rhs)
{
	*this = rhs;
}

EvaPicInPacket::~EvaPicInPacket()
{
	if(decryptedBuf) delete decryptedBuf;
	decryptedBuf = NULL;
	if(rawBody)
		delete rawBody;
	rawBody = NULL;
}

void EvaPicInPacket::cutOffPacketData()
{
	rawLength = getPacketLength();
}

EvaPicInPacket &EvaPicInPacket::operator=( const EvaPicInPacket &rhs)
{
	*((EvaPicPacket*)this) = (EvaPicPacket)rhs;
	bodyLength = rhs.getLength();
	if(decryptedBuf) delete decryptedBuf;
	decryptedBuf = new unsigned char [bodyLength];
	memcpy(decryptedBuf, rhs.getBody(), bodyLength);
        return *this;
}

void EvaPicInPacket::decryptBody(unsigned char * buf, int len)
{
	bodyLength = 0;
	int start = getCryptPosition();
	if(start == -1){
		bodyLength = len;
		memcpy(decryptedBuf, buf, bodyLength);
		return;
	}
	
	if(fileAgentKey == NULL) {     
		fprintf(stderr, "EvaPicOutPacket->decryptBody: file agent key not set yet, set decrypted length to 0\n");
		bodyLength = 0;
		return;
	}
	printf(" \n\nEvaPicInPacket::decryptBody\n");
	for(int i=0; i<len; i++){
 		if(!(i%8)) printf("\n%d: ",i);
 		char t = buf[i];
 		printf("%2x ", (uint8_t)t);
	}
	printf("\n---------=======================---------\n\n");

	unsigned char *bodyBuf = new unsigned char[MAX_PACKET_SIZE];
	int bodyLen=MAX_PACKET_SIZE;	
	int ret = EvaCrypt::decrypt(buf + start, len - start, fileAgentKey, bodyBuf, &bodyLen); 
	if(ret == 0) {
		bodyLength = 0;
		fprintf(stderr, "EvaPicInPacket->decryptBody: failed\n");
		delete bodyBuf;
		return;
	}
	
	memcpy(decryptedBuf, buf, start);                               // copy un-encrypted body
	memcpy(decryptedBuf + start, bodyBuf, bodyLen);                // copy encrypted body
	bodyLength = start + bodyLen;
	
	delete bodyBuf;
}

void EvaPicInPacket::setInPacket( const EvaPicInPacket * packet )
{
	header = packet->getHeader();
	source = packet->getSource();
	command = packet->getCommand();
        sequence = packet->getSequence();
	bodyLength = packet->getLength();
	if(decryptedBuf) delete decryptedBuf;
	decryptedBuf = new  unsigned char[bodyLength];
	memcpy(decryptedBuf, packet->getBody(), bodyLength);        
}

void EvaPicInPacket::printRawData()
{
	if(!decryptedBuf) return;
	printf("packet length:%d\n", bodyLength);
	for(int i=0; i<bodyLength; i++){
		if(!(i%8)) printf("\n%d: ",i);
		unsigned char t = decryptedBuf[i];
		printf("%2x ", (unsigned char)t);
	}
	printf("\n");
}
