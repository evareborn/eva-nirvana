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
#ifndef LIBEVAPACKET_H
#define LIBEVAPACKET_H

#include "evadefines.h"

// this is the base class for all QQ protocol classes
class Packet{
public:	
	Packet() {};
	// this is for outcoming packets
	Packet(const short version, const short command, const short sequence) ;
	// this is for incoming packets
	Packet( unsigned char *buf, int *len);
	Packet(const Packet &rhs);
	~Packet();
	
	bool operator==(const Packet &rhs)  const;
	Packet &operator=(const Packet &rhs);
	const int hashCode() ;
	
	const short getVersion() const {   return version;};
	const short getCommand() const {   return command;};
	const short getSequence()  const {   return sequence;};
	
	
	void setVersion(const short version) { this->version = version; };
	void setCommand(const short command) { this->command = command; };
	void setSequence(const short sequence) { this->sequence = sequence; };
	

	static const unsigned int getQQ() { return qqNum; }
	static void setQQ(const unsigned int id) { qqNum = id; }

	static void setUDP(bool isUDP) { mIsUDP = isUDP; };
	static bool isUDP() { return mIsUDP; };
	static void setPasswordKey(const unsigned char* pkey);
	static bool isLoginTokenSet() { return loginToken != 0; }
	static bool isClientKeySet() { return clientKeyLength != 0; }
	static bool isFileAgentKeySet() { return fileAgentKey != 0; }
	static bool isFileAgentTokenSet() { return fileAgentToken != 0; }
	
	static unsigned char * getFileAgentKey() { return fileAgentKey; }
	static unsigned char * getFileAgentToken() { return fileAgentToken; }
	static unsigned int getFileAgentTokenLength() { return fileAgentTokenLength; }
	static unsigned char *getFileShareToken() { return fileShareToken; }
	static unsigned char *getClientKey() { return clientKey; }
	static const int getClientKeyLength() { return clientKeyLength; }
	
	static void clearAllKeys();      // called this after logged out to release memery
protected:
	short version;
	short command;
	short sequence; 
	
	static unsigned char iniKey[16];
	
	// any key is 16 bytes long defined in evadefines as QQ_KEY_LENGTH
	static unsigned char *getSessionKey()  { return sessionKey; }
	static unsigned char *getPasswordKey()  { return passwordKey; }
	static unsigned char *getFileSessionKey()  { return fileSessionKey; }
	static unsigned char *getLoginToken() { return loginToken; }
	static const int getLoginTokenLength() { return loginTokenLength; }
	
	static void setSessionKey(const unsigned char *skey); 
	static void setFileSessionKey(const unsigned char *fskey);
	static void setLoginToken(const unsigned char *token, const int length);
	static void setClientKey(const unsigned char *ckey, const int length);
	static void setFileAgentKey(const unsigned char *key);
	static void setFileAgentToken(const unsigned char *token, const int length);
	static void setFileShareToken(const unsigned char *token); // always 24 bytes long. used for Qun share disk access
private: 
	static unsigned int qqNum;
	static bool mIsUDP;
	static unsigned char *sessionKey;
	static unsigned char *passwordKey;
	static unsigned char *fileSessionKey;  
	static unsigned char *loginToken;
	static int loginTokenLength;
	
	static unsigned char *fileAgentKey;
	static unsigned char *fileAgentToken;
	static int fileAgentTokenLength;
	
	static unsigned char *clientKey;
	static int clientKeyLength;
	
	static unsigned char *fileShareToken; // note: always 24 bytes long
};

// this is the parent class for all sent-out packets
class OutPacket : public Packet {
public:
	OutPacket() {}
	OutPacket(const short command, const bool needAck);  
	OutPacket(const OutPacket &rhs);
	virtual ~OutPacket() {}

	const int getResendCount() const { return resendCount; }
	
	bool fill(unsigned char *buf, int *len);
	bool needAck() const { return mNeedAck; };
	const bool needResend() { return (--resendCount) != 0; }
	OutPacket &operator=( const OutPacket &rhs);
	
protected:
	virtual int putBody(unsigned char *buf);

private:
	static short startSequenceNum;
	bool mNeedAck;
	int resendCount;
	int  putHead(unsigned char *buf);
	void encryptBody(unsigned char *b, int length, 
			unsigned char *decryptedBody, int *bodyLen);
};

// this class is inherited by all received-in packet
class InPacket : public Packet {
public:
	InPacket();
	InPacket( unsigned char *buf, int len);
	InPacket(const InPacket &rhs);
	virtual ~InPacket();
	
        const bool parse();
	const int getLength() const { return bodyLength; };
	unsigned char * getBody() const { return decryptedBuf; };
        void setInPacket(const InPacket *packet);
	InPacket &operator=( const InPacket &rhs);
protected:	
	int bodyLength;
	unsigned char *decryptedBuf;
	
	virtual void parseBody() {};  
private: 
	void  decryptBody(unsigned char * buf, int len);    
};

#endif
