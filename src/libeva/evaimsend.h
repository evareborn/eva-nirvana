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

#ifndef LIBEVAIMSEND_H
#define LIBEVAIMSEND_H

#include "evapacket.h"
#include "evautil.h"
#include <string>
#include <time.h>

// note that all text in SendIMPacket class is assumed as
//  encoded in locale "GB18030"
class SendIM : public OutPacket
{
public:
	SendIM() {}
	SendIM(const unsigned short type);
	SendIM(const SendIM &rhs) ;
	virtual ~SendIM() {}
		
	const int getReceiver() const { return receiver; };                       // your friend's qq number
	void setReceiver(int receiver) { this->receiver = receiver; }
	
	const short getFaceCode() const { return face; }
	void setFaceCode(short face) { this->face = face; }                      // set my face code	
	
	const unsigned short getContentType() const  { return contentType; }
	const time_t getSendTime() const { return m_SendTime; }

	const unsigned short getMsgSequence() const  { return m_MsgSequence; }
	void setMsgSequence(const unsigned short seq) { m_MsgSequence = seq; }

	SendIM &operator=(const SendIM &rhs);

protected:
	virtual int putBody(unsigned char *buf);
	virtual int putContents(unsigned char *buf);
private:	
	int receiver; 
	short face;
	unsigned short contentType;
	unsigned short m_MsgSequence;
	time_t m_SendTime;
};


class SendTextIMPacket : public SendIM
{
public:
	SendTextIMPacket();
	SendTextIMPacket(const SendTextIMPacket &rhs) ;
	virtual ~SendTextIMPacket() {}

	
	OutPacket * copy() { return new SendTextIMPacket(*this);}
	SendTextIMPacket &operator=(const SendTextIMPacket &rhs);
	
	const std::string &getFontName() const { return fontName; }               // font setting
	void setFontName(std::string &fontName) { this->fontName = fontName; }
	const char getFontSize() const { return fontSize; }
	void setFontSize(char fontSize); 
	const short getEncoding() const { return encoding; }
	void setEncoding(short encoding) { this->encoding = encoding; }

	const bool isUnderline() const { return underline;}                      // U B I setting
	void setUnderline(bool underline) ;
	const bool isItalic() const { return italic; }
	void setItalic(bool italic);
	const bool isBold() const { return bold; };
	void setBold(bool bold);
	
	const char getBlue() const { return blue; }                         // color setting
	void setBlue(char blue) { this->blue = blue; }
	const char getGreen() const { return green; }
	void setGreen(char green)  {this->green = green; }
	const char getRed() const { return red; }
	void setRed(char red) { this->red = red; }
	
	/*
	      the message is plain text, which means the smiley code is already
	      replaced with plain text.  
	*/
	const std::string &getMessage() const { return message; }              // message you want to send
	void setMessage(std::string message) { this->message = message; }   // and just put your plain text message here
	const char getReplyType() const { return replyType; }
	void setReplyType(char replyType) { this->replyType = replyType; }
	void setAutoReply(const bool isNormal);
		
	
	const unsigned char getNumFragments() const { return numFragments; }
	const unsigned char getSeqOfFragments() const { return seqFragments; }
	const short getMessageID() const { return messageID; }
	
	void setNumFragments(const unsigned char num) { numFragments = num; }
	void setSeqOfFragments( const unsigned char seq) { seqFragments = seq; }
	void setMessageID(const short id) { messageID = id; }
protected:
	virtual int putContents(unsigned char *buf);
private:
	//std::string convertToSend(const std::string &text);
	short encoding;
	std::string fontName;
	char red, green, blue;
	bool bold, italic, underline;
	unsigned char fontSize;
	unsigned char fontFlag; 
	   
	std::string message; 
	char replyType;
	
	unsigned char numFragments;
	unsigned char seqFragments;
	static short messageID;
	
};

class SendIMReplyPacket : public InPacket
{
public:
	SendIMReplyPacket() {};
	SendIMReplyPacket(unsigned char *buf, int len);
	SendIMReplyPacket(const SendIMReplyPacket &rhs );
	virtual ~SendIMReplyPacket() {}
	
	const char getReplyCode() const { return replyCode; };
	const bool isSentOK() const ;
	
	SendIMReplyPacket &operator=(const SendIMReplyPacket &rhs );

protected:
	virtual void parseBody();
private: 
	char replyCode;
};

class SendFileRequestPacket : public SendIM
{
public:
	SendFileRequestPacket();
	SendFileRequestPacket(const SendFileRequestPacket &rhs) ;
	virtual ~SendFileRequestPacket() {}

	
	OutPacket * copy() { return new SendFileRequestPacket(*this);}
	SendFileRequestPacket &operator=(const SendFileRequestPacket &rhs);

	inline void setFileName(const std::string &filename) { m_FileName = filename; }
	inline void setFileSize(const unsigned int size) { m_FileSize = size; }
	inline void setDirectPort(const unsigned short port) { m_DirectPort = port; }
	inline void setWanPort(const unsigned short port) { m_WanPort = port; }
	inline void setWanIp( const unsigned int ip) { m_WanIp = ip; }
	inline void setTransferType(const unsigned char type) { m_TransferType = type; }

	inline const std::string getFileName() const { return m_FileName; }
	inline const unsigned int getFileSize() const { return m_FileSize; }
	inline const unsigned short getDirectPort() const { return m_DirectPort; }
	inline const unsigned short getWanPort() const { return m_WanPort; }
	inline const unsigned int getWanIp() const { return m_WanIp; }
	inline const unsigned char getTransferType() const { return m_TransferType; }

protected:
	virtual  int putContents(unsigned char *buf);
private:
	std::string m_FileName;
	unsigned int m_FileSize;
	unsigned short m_DirectPort;
	unsigned short m_WanPort;
	unsigned int m_WanIp;
	unsigned char m_TransferType;
};


// QQ_IM_EX_UDP_REQUEST, QQ_IM_EX_REQUEST_ACCEPTED or QQ_IM_EX_REQUEST_CANCELED
class SendFileExRequestPacket : public SendIM
{
public:
	SendFileExRequestPacket( const short cmd );
	SendFileExRequestPacket(const SendFileExRequestPacket &rhs) ;
	virtual ~SendFileExRequestPacket() {}

	
	OutPacket * copy() { return new SendFileExRequestPacket(*this);}
	SendFileExRequestPacket &operator=(const SendFileExRequestPacket &rhs);

	inline void setFileName(const std::string &filename) { m_FileName = filename; }
	inline void setFileSize(const unsigned int size) { m_FileSize = size; }
	inline void setWanPort(const unsigned short port) { m_WanPort = port; }
	inline void setWanIp( const unsigned int ip) { m_WanIp = ip; }
	inline void setTransferType(const unsigned char type) { m_TransferType = type; }
	inline void setSessionId(const unsigned short session) { m_SessionId = session; }

	inline const std::string getFileName() const { return m_FileName; }
	inline const unsigned int getFileSize() const { return m_FileSize; }
	inline const unsigned short getWanPort() const { return m_WanPort; }
	inline const unsigned int getWanIp() const { return m_WanIp; }
	inline const unsigned char getTransferType() const { return m_TransferType; }
	inline const unsigned short getSessionId() const { return m_SessionId; }

protected:
	virtual  int putContents(unsigned char *buf);
private:
	unsigned short m_SessionId;
	std::string m_FileName;
	unsigned int m_FileSize;
	unsigned short m_WanPort;
	unsigned int m_WanIp;
	unsigned char m_TransferType;
};

class SendFileAcceptPacket : public SendIM
{
public:
	SendFileAcceptPacket();
	SendFileAcceptPacket(const SendFileAcceptPacket &rhs) ;
	virtual ~SendFileAcceptPacket() {}

	
	OutPacket * copy() { return new SendFileAcceptPacket(*this);}
	SendFileAcceptPacket &operator=(const SendFileAcceptPacket &rhs);

	inline void setTransferType(const unsigned char type) { m_TransferType = type; }
	inline void setConnectMode(const unsigned char mode) { m_ConnectMode = mode; }
	inline void setSessionId(const unsigned short session) { m_SessionId = session; }

	inline const unsigned char getTransferType() const { return m_TransferType; }
	inline const unsigned char getConnectMode() const { return m_ConnectMode; }
	inline const unsigned short getSessionId() const { return m_SessionId; }

protected:
	virtual  int putContents(unsigned char *buf);
private:
	unsigned short m_SessionId;
	unsigned char m_TransferType;
	unsigned char m_ConnectMode;
};

class SendIpExNotifyPacket : public SendIM
{
public:
	SendIpExNotifyPacket(const bool isSender = true);
	SendIpExNotifyPacket(const SendIpExNotifyPacket &rhs) ;
	virtual ~SendIpExNotifyPacket() {}

	
	OutPacket * copy() { return new SendIpExNotifyPacket(*this);}
	SendIpExNotifyPacket &operator=(const SendIpExNotifyPacket &rhs);

	inline void setTransferType(const unsigned char type) { m_TransferType = type; }
	inline void setConnectMode(const unsigned char mode) { m_ConnectMode = mode; }
	inline void setSessionId(const unsigned int session) { m_SessionId = session; }

	inline void setWanIp1( const unsigned int ip) { m_WanIp1 = ip; }
	inline void setWanPort1(const unsigned short port) { m_WanPort1 = port; }
	inline void setWanIp2( const unsigned int ip) { m_WanIp2 = ip; }
	inline void setWanPort2(const unsigned short port) { m_WanPort2 = port; }
	inline void setWanIp3( const unsigned int ip) { m_WanIp3 = ip; }
	inline void setWanPort3(const unsigned short port) { m_WanPort3 = port; }

	inline void setLanIp1( const unsigned int ip) { m_LanIp1 = ip; }
	inline void setLanPort1(const unsigned short port) { m_LanPort1 = port; }
	inline void setLanIp2( const unsigned int ip) { m_LanIp2 = ip; }
	inline void setLanPort2(const unsigned short port) { m_LanPort2 = port; }
	inline void setLanIp3( const unsigned int ip) { m_LanIp3 = ip; }
	inline void setLanPort3(const unsigned short port) { m_LanPort3 = port; }

	inline void setSender(const bool sender) { m_IsSender = sender; }
	inline const bool isSender() const { return m_IsSender; }

	inline const unsigned char getTransferType() const { return m_TransferType; }
	inline const unsigned char getConnectMode() const { return m_ConnectMode; }
	inline const unsigned int getSessionId() const { return m_SessionId; }

	inline const unsigned int getWanIp1() const { return m_WanIp1; }
	inline const unsigned short getWanPort1() const { return m_WanPort1; }
	inline const unsigned int getWanIp2() const { return m_WanIp2; }
	inline const unsigned short getWanPort2() const { return m_WanPort2; }
	inline const unsigned int getWanIp3() const { return m_WanIp3; }
	inline const unsigned short getWanPort3() const { return m_WanPort3; }

	inline const unsigned int getLanIp1() const { return m_LanIp1; }
	inline const unsigned short getLanPort1() const { return m_LanPort1; }
	// in most cases, just leave lan ip 2 & port 2 as default ( 0.0.0.0:0)
	inline const unsigned int getLanIp2() const { return m_LanIp2; }
	inline const unsigned short getLanPort2() const { return m_LanPort2; }
	inline const unsigned int getLanIp3() const { return m_LanIp3; }
	inline const unsigned short getLanPort3() const { return m_LanPort3; }

	inline void setSyncSession(const unsigned int session) { m_SyncSession = session; }
	inline const unsigned int getSyncSession() const { return m_SyncSession; }

	inline void setSyncIp( const unsigned int ip) { m_SyncIp = ip; }
	inline void setSyncPort(const unsigned short port) { m_SyncPort = port; }
	inline const unsigned int getSyncIp() const { return m_SyncIp; }
	inline const unsigned short getSyncPort() const { return m_SyncPort; }

protected:
	virtual  int putContents(unsigned char *buf);
private:
	bool m_IsSender;
	unsigned char m_TransferType;
	unsigned char m_ConnectMode;
	unsigned int m_SessionId;

	unsigned int m_WanIp1;
	unsigned short m_WanPort1;
	unsigned int m_WanIp2;
	unsigned short m_WanPort2;
	unsigned int m_WanIp3;
	unsigned short m_WanPort3;

	unsigned int m_LanIp1;
	unsigned short m_LanPort1;
	unsigned int m_LanIp2;
	unsigned short m_LanPort2;
	unsigned int m_LanIp3;
	unsigned short m_LanPort3;


	unsigned int m_SyncIp;
	unsigned short m_SyncPort;
	unsigned int m_SyncSession;
};

class SendFileNotifyAgentPacket : public SendIM
{
public:
	SendFileNotifyAgentPacket();
	SendFileNotifyAgentPacket(const SendFileAcceptPacket &rhs) ;
	virtual ~SendFileNotifyAgentPacket() {}

	
	OutPacket * copy() { return new SendFileNotifyAgentPacket(*this);}
	SendFileNotifyAgentPacket &operator=(const SendFileNotifyAgentPacket &rhs);

	inline void setTransferType(const unsigned char type) { m_TransferType = type; }
	inline void setConnectMode(const unsigned char mode) { m_ConnectMode = mode; }
	inline void setAgentServer(const unsigned int ip, const unsigned short port) 
				{ m_Ip = ip;  m_Port = port; printf("========== ip:%8x, port:%4x\n", ip, 0xffff&port);}
	inline void setAgentSession(const unsigned int session) { m_Session = session; }

	inline const unsigned char getTransferType() const { return m_TransferType; }
	inline const unsigned char getConnectMode() const { return m_ConnectMode; }
	inline const unsigned int getAgentIp() const { return m_Ip; }
	inline const unsigned short getAgentPort() const { return m_Port; }
	inline const unsigned int getAgentSession() const { return m_Session; }

protected:
	virtual  int putContents(unsigned char *buf);
private:
	unsigned int m_Ip;
	unsigned short m_Port;
	unsigned int m_Session;
	unsigned char m_TransferType;
	unsigned char m_ConnectMode;
};


#endif

