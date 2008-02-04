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
 
#ifndef LIBCUSTOMPIC_EVAPICPACKET_H
#define LIBCUSTOMPIC_EVAPICPACKET_H
#include "../evadefines.h"

const unsigned char FAMILY_05_TAG   =  0x05;
const unsigned char FAMILY_05_TAIL  =  0x03;
const int NO_CRYPTOGRAPH = -1;

static const unsigned short QQ_05_CMD_REQUEST_AGENT = 0x0021;
static const unsigned short QQ_05_CMD_REQUEST_FACE  = 0x0022;
static const unsigned short QQ_05_CMD_TRANSFER      = 0x0023;
static const unsigned short QQ_05_CMD_REQUEST_START = 0x0026;

/** apply transfer server success */
static const unsigned short QQ_REQUEST_AGENT_REPLY_OK       = 0x0000;
/** apply for transfer server */
static const unsigned short QQ_REQUEST_AGENT_REPLY_REDIRECT = 0x0001;
/** sending picture is too big */
static const unsigned short QQ_REQUEST_AGENT_REPLY_TOO_LONG = 0x0003;

static const char GROUP_FILE_AGENT[] = "219.133.40.128"; // GroupFile.tencent.com


class EvaPicPacket {
public:
	EvaPicPacket(){};
	EvaPicPacket( unsigned char *buf, int len);
	// this is for outcoming packets
	EvaPicPacket(const unsigned short source, const unsigned short command, const unsigned short sequence) ;
	// this is for incoming packets
	EvaPicPacket( unsigned char *buf, int *len, const int cryptStart);
	
	EvaPicPacket(const EvaPicPacket &rhs);
	~EvaPicPacket();
	
	EvaPicPacket &operator=(const EvaPicPacket &rhs);
	
	const int hashCode();
	static void setQQ(const unsigned int id) { myQQ = id; }
	static unsigned int getQQ() { return myQQ;}
	
	const unsigned char getHeader() const { return header; }
	const unsigned char getTail() const { return tail; }
	const unsigned short getSource() const { return source; }
	const unsigned short getCommand() const { return command; }
	const unsigned short getSequence() const { return sequence; }
	
	const int getCryptPosition() const { return cryptPosition; }
	
	static void setFileAgentKey(const unsigned char *key);
	static void clearKey();
	void setKey(const unsigned char *key);
	
	const bool isValid() const { return header==FAMILY_05_TAG && tail==FAMILY_05_TAIL ;}
	const unsigned short getPacketLength() const { return packetLength; }
protected:
	void setCryptPosition(const int position) {cryptPosition = position; }
	const int parseHeader(unsigned char *buf);
	static unsigned char *fileAgentKey;
	static unsigned int myQQ;

	
	unsigned char header;
	unsigned short source;
	unsigned short command;
	unsigned short sequence;
	unsigned char *key;
	unsigned char tail;
	int cryptPosition;
	unsigned short packetLength;
};

class EvaPicOutPacket : public EvaPicPacket{
public:
	EvaPicOutPacket() {};
	EvaPicOutPacket(const unsigned short command, const bool ack);
	EvaPicOutPacket(const EvaPicOutPacket &rhs);
	
	virtual ~EvaPicOutPacket() {}
	
	EvaPicOutPacket &operator=(const EvaPicOutPacket &rhs);
	
	const int getResendCount() const { return resendCount; }
	const bool fill(unsigned char *buf, int *len);
	const bool needAck() const { return mNeedAck; };
	const bool needResend() { return (--resendCount) != 0; }
protected:
	virtual int putBody(unsigned char *buf);
private:
	static short sequenceStart;
	bool mNeedAck;
	int resendCount;
	int  putHead(unsigned char *buf);
	void encryptBody(unsigned char *b, int length, 
			unsigned char *decryptedBody, int *bodyLen);
};

class EvaPicInPacket : public EvaPicPacket{
public:
	EvaPicInPacket() {}
	EvaPicInPacket( unsigned char *buf, int len);
	EvaPicInPacket( unsigned char *buf, int len, const int cryptStart);
	EvaPicInPacket(const EvaPicInPacket &rhs);
	virtual ~EvaPicInPacket();
	
        void parse() { parseBody(); }
	const int getLength() const { return bodyLength; };
	unsigned char * getBody() const { return decryptedBuf; };
        void setInPacket(const EvaPicInPacket *packet);
	
	void cutOffPacketData();
	
	EvaPicInPacket &operator=( const EvaPicInPacket &rhs);
	
	unsigned char *getRawBody() const { return rawBody; }
	unsigned int getRawBodyLength() const { return rawLength; }
	void printRawData();
protected:
	int bodyLength;
	unsigned char *decryptedBuf;
	
	virtual void parseBody() {};  
private: 
	void  decryptBody(unsigned char * buf, int len); 
	unsigned char *rawBody;
	unsigned int rawLength;
};

#endif 
