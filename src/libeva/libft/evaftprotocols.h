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

#ifndef EVAFTPROTOCOLS_H
#define EVAFTPROTOCOLS_H

#include "evaftpacket.h"
#include <string>

#define QQ_FILE_AGENT_CREATE_OK           0x0000
#define QQ_FILE_AGENT_CREATE_REDIRECT     0x0001
#define QQ_FILE_AGENT_CREATE_ERROR        0x0002
#define QQ_FILE_AGENT_OPERATION_OK        0x00000000

#define QQ_FILE_AGENT_TRANSFER_INFO       1
#define QQ_FILE_AGENT_TRANSFER_DATA       2
#define QQ_FILE_AGENT_TRANSFER_REPLY      3
#define QQ_FILE_AGENT_TRANSFER_START      4

class EvaFTAgentCreate : public EvaFTAgentPacket
{
public:
	EvaFTAgentCreate();
	virtual ~EvaFTAgentCreate();
	void setFileAgentToken(const unsigned char *token, const int len);
	inline void setBuddyQQ(const int id) { m_Id = id; }
	inline void setIp(const unsigned int ip) { m_Ip = ip; }
protected:
	const int fillBody(unsigned char *buf);
private:
	unsigned char *m_Token;
	int m_TokenLength;
	int m_Id;
	unsigned int m_Ip;
};


class EvaFTAgentCreateReply : public EvaFTAgentPacket
{
public:
	EvaFTAgentCreateReply(const unsigned char *buf, const int len);
	
	inline const bool isConnected() const { return m_ReplyCode==QQ_FILE_AGENT_CREATE_OK; }
	const unsigned short getReplyCode() const { return m_ReplyCode; }

	// if isConnect() returns true, the followings are for the server you are connecting with
	// otherwise, they will be another proper server's ip and port
	// also, getSessionId() will return the session id for this operation
	inline const unsigned int getIp() const { return m_Ip; }
	inline const unsigned short getPort() const { return m_Port; }

	const std::string &getMessage() const { return m_ErrMessage; }
protected:
	const bool parseBody(unsigned char *buf, const int len);
private:
	unsigned short m_ReplyCode;
	unsigned int m_Ip;
	unsigned short m_Port;
	std::string m_ErrMessage;
};


class EvaFTAgentLogin : public EvaFTAgentPacket
{
public:
	EvaFTAgentLogin();
	~EvaFTAgentLogin();
	void setFileAgentToken(const unsigned char *token, const int len);
protected:
	const int fillBody(unsigned char *buf);
private:
	unsigned char *m_Token;
	int m_TokenLength;
};


class EvaFTAgentLoginReply : public EvaFTAgentPacket
{
public:
	EvaFTAgentLoginReply(const unsigned char *buf, const int len);
	~EvaFTAgentLoginReply() {};
	inline const bool isConnected() const { return m_ReplyCode==QQ_FILE_AGENT_OPERATION_OK ;}
protected:
	const bool parseBody(unsigned char *buf, const int len);
private:
	unsigned int m_ReplyCode;
};


// note: whenever we got 50 packets we need to send a reply packet to server
class EvaFTAgentTransfer : public EvaFTAgentPacket
{
public:
	EvaFTAgentTransfer(const int type);
	~EvaFTAgentTransfer();
	
	// for file info
	void setInfo(const std::string &file, const unsigned char *fileMd5, 
			const unsigned int size);

	// for file data
	void setData(const unsigned char *data, const unsigned int len);

	// ask start at offset
	inline void setOffset(const unsigned int offset) { m_StartOffset = offset; }
protected:
	const int fillBody(unsigned char *buf);
private:
	int m_Type;

	// for file info
	std::string m_FileName;
	unsigned char *m_FMd5;
	unsigned int m_FileSize;

	// for file data
	unsigned char *m_Data;
	unsigned int m_DataLength; // should be always 0x0800

	unsigned int m_StartOffset;
};


// we receive this from agent server ( relay server)
class EvaFTAgentTransferReply : public EvaFTAgentPacket
{
public:
	EvaFTAgentTransferReply(const int type, const unsigned char *buf, const int len);
	~EvaFTAgentTransferReply();

	// ask sender to start sending contents at position
	inline unsigned int getStartPosition() const { return m_ReplyCode;}
	//inline const bool isTransferNow() const { return m_ReplyCode == QQ_FILE_AGENT_OPERATION_OK; }

	// ask sender to sending next 50 packets
	inline const bool isReceivedOk() const { return m_NextReplyCode == 0x02; }

	// tell receiver the file information, sequence always be 0x0001
	const unsigned char * getFileMd5();
	const unsigned char * getFileNameMd5();
	inline const std::string &getFileName() const { return m_FileName; }
	inline const unsigned int getFileSize() const { return m_FileSize; }

	// data packet, also you should check the sequence number
	// the file info has the number 1, the data packet starts 2 till
	// the last packet
	inline const unsigned int getDataLength() const { return m_DataLength; }
	//void getData(unsigned char *buf);
	inline const unsigned char *getData() const { return m_Data; }
	 
protected:
	const bool parseBody(unsigned char *buf, const int len);
private:
	int m_Type;

	// ask sender to start sending contents
	unsigned int m_ReplyCode;

	// ask sender to sending next 50 packets
	unsigned char m_NextReplyCode;

	// tell receiver the file information
	unsigned char *m_FileMd5;
	unsigned char *m_FileNameMd5;
	std::string m_FileName;
	unsigned int m_FileSize;

	// data packet
	unsigned char *m_Data;
	unsigned int m_DataLength;
};


class EvaFTAgentAckReady : public EvaFTAgentPacket
{
public:
	EvaFTAgentAckReady();
	~EvaFTAgentAckReady();
protected:
	const int fillBody(unsigned char *buf);
};


class EvaFTAgentAskReady : public EvaFTAgentPacket
{
public:
	EvaFTAgentAskReady(const unsigned char *buf, const int len);
	~EvaFTAgentAskReady() {};
	inline const bool isAskReady() const { return m_ReplyCode==QQ_FILE_AGENT_OPERATION_OK ;}
protected:
	const bool parseBody(unsigned char *buf, const int len);
private:
	unsigned int m_ReplyCode;
};


class EvaFTAgentStart : public EvaFTAgentPacket
{
public:
	EvaFTAgentStart();
	~EvaFTAgentStart();
protected:
	const int fillBody(unsigned char *buf);
};


class EvaFTAgentStartReply : public EvaFTAgentPacket
{
public:
	EvaFTAgentStartReply(const unsigned char *buf, const int len);
	~EvaFTAgentStartReply() {};
protected:
	const bool parseBody(unsigned char *buf, const int len);
};


/** =================================================== */


class EvaFTSynCreate : public EvaFTSynPacket
{
public:
	EvaFTSynCreate();
	~EvaFTSynCreate();
	void setFileAgentToken(const unsigned char *token, const int len);
	inline void setBuddyQQ(const int id) { m_BuddyId = id; }
protected:
	const int fillBody(unsigned char *buf);
private:
	unsigned char *m_Token;
	int m_TokenLength;
	int m_BuddyId;	
};

class EvaFTSynCreateReply : public EvaFTSynPacket
{
public:
	EvaFTSynCreateReply(const unsigned char *buf, const int len);
	
	inline const bool isSuccessful() const { return m_ReplyCode==QQ_FILE_AGENT_CREATE_OK; }
	const unsigned short getReplyCode() const { return m_ReplyCode; }

	inline const unsigned int getIp() const { return m_Ip; }
	inline const unsigned short getPort() const { return m_Port; }
protected:
	const bool parseBody(unsigned char *buf, const int len);
private:
	unsigned short m_ReplyCode;
	unsigned int m_Ip;
	unsigned short m_Port;
};


#endif //#ifndef EVAFTPROTOCOL_H
