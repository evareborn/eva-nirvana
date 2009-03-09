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
 
#include "evapacket.h"
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "evacrypt.h"
#include <cstring>

unsigned char Packet::iniKey[16] = {
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
			};
			
unsigned int Packet::qqNum = 0;
bool Packet::mIsUDP = true;
unsigned char *Packet::sessionKey = NULL;
unsigned char *Packet::passwordKey = NULL;
unsigned char *Packet::fileSessionKey = NULL;
unsigned char *Packet::loginToken= NULL;
int Packet::loginTokenLength = 0;
unsigned char *Packet::clientKey= NULL;
int Packet::clientKeyLength = 0;

unsigned char *Packet::fileAgentKey = NULL;
unsigned char *Packet::fileAgentToken = NULL;
int Packet::fileAgentTokenLength = 0;

unsigned char *Packet::fileShareToken = NULL;

Packet::Packet(const short version, const short command, const short sequence)
{
	this->version = version;
	this->command = command;
	this->sequence = sequence;
}

Packet::Packet(unsigned char *buf, int *len)
{
	int pos;
	// if it's not UDP, must be TCP
	if(!mIsUDP)	
		pos=3;
	else
		pos = 1;
	
	short tmp;
	
	memcpy(&tmp, buf+pos, 2);
	version = ntohs(tmp);
	pos+=2;
	
	memcpy(&tmp, buf+pos, 2);
	command = ntohs(tmp);
	pos+=2;
	
	memcpy(&tmp, buf+pos, 2);
	sequence = ntohs(tmp);
	pos+=2;
	
	(*len)=(*len)-pos;
	memcpy(buf, buf+pos, *len);
}

Packet::Packet(const Packet &rhs)
{
	*this = rhs;
}

Packet::~Packet()
{
}

bool Packet::operator ==( const Packet &rhs) const
{
	return (command == rhs.getCommand()) && (sequence == rhs.getSequence());
}

Packet &Packet::operator=(const Packet &rhs)
{
	this->version = rhs.getVersion();
	this->command = rhs.getCommand();
	this->sequence = rhs.getSequence();
	this->mIsUDP = rhs.isUDP();
        return *this;	
}

const int Packet::hashCode() 
{
	return ((int)sequence << 16) | command;
}

void Packet::setSessionKey(const unsigned char *skey)
{
	if(!sessionKey)
		sessionKey = new unsigned char[QQ_KEY_LENGTH];
	memcpy(sessionKey, skey, QQ_KEY_LENGTH);
	//fprintf(stderr,"session key set!\n");
}

void Packet::setPasswordKey(const unsigned char* pkey)
{
	if(!passwordKey) 
		passwordKey = new unsigned char[QQ_KEY_LENGTH];
	memcpy(passwordKey, pkey, QQ_KEY_LENGTH);
	//fprintf(stderr, "password key set!\n");
}

void Packet::setFileSessionKey(const unsigned char* fskey)
{
	if(!fileSessionKey) 
		fileSessionKey = new unsigned char[QQ_KEY_LENGTH];
	memcpy(fileSessionKey, fskey, QQ_KEY_LENGTH);
	//fprintf(stderr, "file session key set!\n");
}

void Packet::setLoginToken(const unsigned char *token, const int length)
{
	if(!loginToken)
		loginToken = new unsigned char[length];
	memcpy(loginToken, token, length);
	loginTokenLength = length;
	//fprintf(stderr, "login token set!\n");	
}

void Packet::setClientKey(const unsigned char *ckey, const int length)
{
	if(!clientKey)
		clientKey = new unsigned char[length];
	memcpy(clientKey, ckey, length);
	clientKeyLength = length;
	//fprintf(stderr, "client key set!\n");
}

void Packet::setFileAgentKey(const unsigned char *key)
{
	if(!fileAgentKey)
		fileAgentKey = new unsigned char[QQ_KEY_LENGTH];
	memcpy(fileAgentKey, key, 16);
//	fprintf(stderr, "file agent key set!\n");
// 	for(int i=0; i<16; i++){
// 		char t = fileAgentKey[i];
// 		printf("%2x", (unsigned char)t);
// 	}
// 	printf("\n");
}

void Packet::setFileAgentToken(const unsigned char *token, const int length)
{
	if(!fileAgentToken)
		fileAgentToken = new unsigned char[length];
	memcpy(fileAgentToken, token, length);
	fileAgentTokenLength = length;
//	fprintf(stderr, "file agent token set!\n");	
// 	for(int i=0; i<length; i++){
// 		char t = fileAgentToken[i];
// 		printf("%2x", (unsigned char)t);
// 	}
// 	printf("\n");
}

void Packet::setFileShareToken(const unsigned char *token)
{
	if(!fileShareToken)
		fileShareToken = new unsigned char[24];
	memcpy(fileShareToken, token, 24);
	//fprintf(stderr, "file share token set!\n");
}

// this will be called just right before sending in OutPacket::fill method
void Packet::clearAllKeys()
{
	if(sessionKey) delete [] sessionKey;
	if(passwordKey) delete [] passwordKey;
	if(fileSessionKey) delete [] fileSessionKey;
	if(loginToken) delete [] loginToken;
	if(clientKey) delete [] clientKey;
	if(fileAgentKey) delete [] fileAgentKey;
	if(fileAgentToken) delete [] fileAgentToken;
	if(fileShareToken) delete [] fileShareToken;
	
	// force them to NULL now
	sessionKey = NULL;
	passwordKey = NULL;
	fileSessionKey = NULL;
	loginToken = NULL;
	loginTokenLength = 0;
	clientKey = NULL;
	clientKeyLength = 0;
	fileAgentKey = NULL;
	fileAgentToken = NULL;
	fileAgentTokenLength = 0;
	fileShareToken = NULL;
}

/* =========================================================== */

// OutPacket starts here
short OutPacket::startSequenceNum = 5;   // could be any random number

OutPacket::OutPacket(const short command, const bool needAck) :
	Packet(QQ_CLIENT_VERSION, command, ((startSequenceNum++)%0xffff))
{
	this->mNeedAck = needAck;
	resendCount = 10;
}

OutPacket::OutPacket(const OutPacket &rhs) 
	: Packet(rhs)
{
	mNeedAck = rhs.needAck();
	resendCount = rhs.getResendCount();
}

bool OutPacket::fill(unsigned char *buf, int *len) 
{
	int headerLen = 0, bodyLen=0, encryptedLen = 0;
	unsigned char *bodyBuf;
	unsigned char *encrypted;
	bodyBuf   = (unsigned char *)malloc(MAX_PACKET_SIZE* sizeof(unsigned char));
	encrypted = (unsigned char *)malloc(MAX_PACKET_SIZE * sizeof(unsigned char));
	
	headerLen = putHead(buf);	
	bodyLen   = putBody(bodyBuf);
	encryptedLen = bodyLen;
	encryptBody(bodyBuf, bodyLen, encrypted, &encryptedLen);
	
	memcpy(buf+headerLen, encrypted, encryptedLen);
	
	buf[ headerLen + encryptedLen ] = QQ_PACKET_TAIL;
	(*len) = headerLen + encryptedLen + 1;
	
	if(!isUDP()) { 
		short tmp2 = htons(*len);
		memcpy(buf, &tmp2, 2);
	}
	free(bodyBuf);
	free(encrypted);
	return true;
}

OutPacket &OutPacket::operator=( const OutPacket &rhs)
{
	*((Packet*)this) = (Packet)rhs;
	mNeedAck = rhs.needAck();
	resendCount = rhs.getResendCount();
        return *this;
}

int OutPacket::putBody(unsigned char *buf)
{
	fprintf(stderr, "In OutPacket\n"); 
	buf[0]=0; 
	return 0;
}

int OutPacket::putHead(unsigned char *buf) 
{
	int pos=0;
	if(!isUDP()){
		buf[0]=buf[1]=0;
		pos+=2;
	}
	buf[pos++]=(unsigned char)QQ_PACKET_TAG;
	
	short tmp = htons(version);
	memcpy(buf+pos, &tmp, 2);
	pos+=2; 
	
	tmp = htons(command);
	memcpy(buf+pos, &tmp, 2);
	pos+=2; 

	tmp = htons(sequence);
	memcpy(buf+pos, &tmp, 2);
	pos+=2; 
	
	int id = htonl(getQQ());
	memcpy(buf+pos, &id, 4);
	pos+=4;
		
	return pos;
}

void OutPacket::encryptBody(unsigned char *b, int length, 
                            unsigned char *body, int *bodyLen) 
{
	switch(command){
	case QQ_CMD_SERVER_DETECT:
	case QQ_CMD_REQUEST_LOGIN_TOKEN:
	case QQ_CMD_LOGIN:
		*bodyLen = length;
		memcpy(body, b, length);
		break;
	case QQ_CMD_REQUEST_LOGIN_TOKEN_EX:{
		memcpy(body, QQ_Client_Key, 16);
		EvaCrypt::encrypt(b, length, (unsigned char*)(QQ_Client_Key), body+16, bodyLen);
		(*bodyLen) +=16;
		}
		break;
	default:{
		unsigned char * sessionKey = getSessionKey();
		if(sessionKey == NULL) {     
			fprintf(stderr, "OutPacket->encryptBody: sessionkey not set yet, set encrypted length to 0\n");
			*bodyLen = 0;
			return;
		} 
		EvaCrypt::encrypt(b, length, sessionKey, body, bodyLen); 
		}
	}
}

/* =========================================================== */

// InPacket Class starts here

InPacket::InPacket( )
	: decryptedBuf(NULL)
{
}

InPacket::InPacket( unsigned char *buf, int len)
	: Packet(buf, &len),
	bodyLength(0)
{
	decryptedBuf = (unsigned char*)malloc(MAX_PACKET_SIZE * sizeof(unsigned char));
	decryptBody(buf, len - QQ_TAIL_LENGTH);
}

InPacket::InPacket(const InPacket &rhs)
	: Packet(rhs)
{
	bodyLength = rhs.getLength();
	decryptedBuf = (unsigned char*)malloc(bodyLength * sizeof(unsigned char));
	memcpy(decryptedBuf, rhs.getBody(), bodyLength);
}

InPacket::~InPacket()
{
	delete decryptedBuf;
}

InPacket &InPacket::operator=( const InPacket &rhs)
{
	*((Packet*)this) = (Packet)rhs;
	bodyLength = rhs.getLength();
	if(!decryptedBuf){
		delete decryptedBuf;
		decryptedBuf = new unsigned char[bodyLength];
	}
	memcpy(decryptedBuf, rhs.getBody(), bodyLength);
        return *this;
}

void InPacket::decryptBody(unsigned char * buf, int len)
{
	int ret = 0;
	bodyLength = MAX_PACKET_SIZE;
	
	unsigned char * passwordKey = getPasswordKey();
	unsigned char * sessionKey = getSessionKey();
	
/*	if(command == QQ_CMD_REQUEST_LOGIN_TOKEN ){
		memcpy(decryptedBuf, buf, len);
		bodyLength = len;
		return;
	}
	
	if(passwordKey == NULL) {     
		fprintf(stderr, "InPacket->decryptBody: password key not set yet, set decrypted length to 0\n");
		bodyLength = 0;
		return;
	}
	if((command != QQ_CMD_LOGIN) && (sessionKey == NULL)) {     
		fprintf(stderr, "InPacket->decryptBody: session key not set yet, set decrypted length to 0\n");
		command = -1;
		bodyLength = 0;
		return;
	} 
	if(command == QQ_CMD_LOGIN) {
		ret = EvaCrypt::decrypt(buf, len, passwordKey, decryptedBuf, &bodyLength);
		if(ret == 0){
			bodyLength = MAX_PACKET_SIZE;
			ret = EvaCrypt::decrypt(buf, len, iniKey, decryptedBuf, &bodyLength); 
			if(ret == 0){
				printf("CANNOT DECRYPT! ----- login reply data(len:%d):\n", len);
				for(int i=0; i<len; i++){
					if(!(i%8)) printf("\n%d: ",i);
					char t = buf[i];
					printf("%2x ", (unsigned char)t);
				}
				printf("\n");
			}
		}
	} else {
		ret = EvaCrypt::decrypt(buf, len, sessionKey, decryptedBuf, &bodyLength);
		if(ret == 0){
			bodyLength = MAX_PACKET_SIZE;
			ret = EvaCrypt::decrypt(buf, len, passwordKey, decryptedBuf, &bodyLength);
		}
	}
	if(ret == 0)
		bodyLength = 0;
*/

	switch(command){		
	case QQ_CMD_REQUEST_LOGIN_TOKEN:
		memcpy(decryptedBuf, buf, len);
		bodyLength = len;
		ret = 1;
		break;
	case QQ_CMD_SERVER_DETECT:
	case QQ_CMD_REQUEST_LOGIN_TOKEN_EX:
		ret = EvaCrypt::decrypt(buf, len, (unsigned char *)(QQ_Client_Key), decryptedBuf, &bodyLength);
		if(ret == 0){
			fprintf(stderr, "InPacket->decryptBody: cannot decryt reply packet( command: 0x%2x\n", command);
		}
		break;
	case QQ_CMD_LOGIN:
		if(passwordKey == NULL) {
			fprintf(stderr, "InPacket->decryptBody: password key not set yet, set decrypted length to 0\n");
			command = -1;
			bodyLength = 0;
			return;
		} 
		ret = EvaCrypt::decrypt(buf, len, passwordKey, decryptedBuf, &bodyLength);
		if(ret == 0){
			bodyLength = MAX_PACKET_SIZE;
			ret = EvaCrypt::decrypt(buf, len, iniKey, decryptedBuf, &bodyLength); 
			if(ret == 0){
				printf("CANNOT DECRYPT! ----- login reply data(len:%d):\n", len);
				for(int i=0; i<len; i++){
					if(!(i%8)) printf("\n%d: ",i);
					char t = buf[i];
					printf("%2x ", (unsigned char)t);
				}
				printf("\n");
			}
		}
		break;
	default:
		if(sessionKey == NULL) {
			fprintf(stderr, "InPacket->decryptBody: session key not set yet, set decrypted length to 0\n");
			command = -1;
			bodyLength = 0;
			break;
		}
		ret = EvaCrypt::decrypt(buf, len, sessionKey, decryptedBuf, &bodyLength);
		if(ret == 0){
			if(passwordKey == NULL) {
				fprintf(stderr, "InPacket->decryptBody: password key not set yet, set decrypted length to 0\n");
				bodyLength = 0;
				return;
			}
			bodyLength = MAX_PACKET_SIZE;
			ret = EvaCrypt::decrypt(buf, len, passwordKey, decryptedBuf, &bodyLength);
		}
		break;
	}

	if(ret == 0){
		bodyLength = 0;
	}
}

void InPacket::setInPacket( const InPacket * packet )
{
	version = packet->getVersion();
	command = packet->getCommand();
        sequence = packet->getSequence();
	bodyLength = packet->getLength();
	if(!decryptedBuf){
		delete decryptedBuf;
		decryptedBuf = new unsigned char[bodyLength];
	}
	memcpy(decryptedBuf, packet->getBody(), bodyLength);        
}

const bool InPacket::parse( )
{
	if(bodyLength){
		parseBody();
		return true;
	}else
		return false;
}

