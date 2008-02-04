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
#ifndef EVA_RECEIVE_IM_PACKET_H
#define EVA_RECEIVE_IM_PACKET_H

#include "evapacket.h"
#include <string>


#define REPLY_KEY_LENGTH 16
class ReceiveIMPacket :public InPacket
{
public: 
	ReceiveIMPacket() {}
	ReceiveIMPacket(unsigned char *buf, const int len);
	ReceiveIMPacket(const ReceiveIMPacket &rhs);
	~ReceiveIMPacket() {};
	
	const unsigned int getSender() const { return sender; }
	const unsigned int getReceiver() const { return receiver; }
	const int getIntSequence() const { return intSequence; }
	const unsigned int getSenderIP() const { return senderIP; }
	const unsigned short getSenderPort() const { return senderPort; }
	const unsigned short getIMType() const { return type; }
	
	const char *getReplyKey() const { return replyKey; }
	const int getBodyLength() const { return (bodyLength - bodyOffset); }
	const unsigned char *getBodyData() const { return (decryptedBuf + bodyOffset); }
	
	static std::string convertToShow(const std::string &src, const unsigned char type = QQ_IM_NORMAL_REPLY);
	ReceiveIMPacket &operator=(const ReceiveIMPacket &rhs);
protected:
	virtual void parseBody(); 

private:
	char replyKey[REPLY_KEY_LENGTH];
	// these are the header information
	unsigned int sender;
	unsigned int receiver;
	int intSequence;
	unsigned int senderIP;  // 4 bytes for 4 parts of an IP address
	unsigned short senderPort;
	unsigned short type;
	
	int bodyOffset;
	int readHeader(const unsigned char * buf);
};

class ReceivedSystemIM{
public:
	ReceivedSystemIM(const unsigned char *buf, const int len);
	ReceivedSystemIM( const ReceivedSystemIM &rhs);
	~ReceivedSystemIM() {};
	
	const unsigned char getSystemIMType() const { return systemIMType; }
	const std::string &getMessage() const { return message; }
	ReceivedSystemIM &operator=(const ReceivedSystemIM &rhs);
private:
	unsigned char systemIMType;
	std::string message;
	void parseData(const unsigned char *buf, const int len);
};

class NormalIMBase{
public:
	NormalIMBase() {}
	NormalIMBase(const unsigned char *buf, const int len);
	NormalIMBase(const NormalIMBase &rhs);
	virtual ~NormalIMBase();

	void setNormalIMBase(const NormalIMBase *base);	
	const unsigned char *getBodyData() const { return bodyBuf; }
	const int getBodyLength() const { return bodyLength; }
	
	const short getSenderVersion() const { return senderVersion; }
	const unsigned int getSender() const { return sender; }
	const unsigned int getReceiver() const { return receiver; }
	const unsigned char *getBuddyFileSessionKey() const { return fileSessionKey; }
	const short getNormalIMType() const { return type; }
	const short getSequence() const { return sequence; }
	const unsigned int getSendTime() const { return sendTime; }
	const char getUnknown1() const { return unknown1; }
	const unsigned short getSenderFace() const { return senderFace; }
	
	void parseData();
	
	NormalIMBase &operator=(const NormalIMBase &rhs);
protected:
	virtual void parseContents(const unsigned char *buf, const int len);
	unsigned char *bodyBuf;
	int bodyLength;
private:
	short senderVersion;
	unsigned int sender;
	unsigned int receiver;
	unsigned char fileSessionKey[16];
	short type;
	short sequence;
	unsigned int sendTime;
	char unknown1;
	unsigned short senderFace;
	int readHeader(const unsigned char * buf);
};

class ReceivedNormalIM : public NormalIMBase {
public:
	ReceivedNormalIM() {}
	ReceivedNormalIM(const unsigned char *buf, const int len);
	ReceivedNormalIM(const ReceivedNormalIM &rhs);
	virtual ~ReceivedNormalIM() {};
	
	const char *getUnknown2() const { return unknown2; }
	const char getReplyType() const { return replyType; }
	const std::string getMessage() const { return message; }
	void setMessage(std::string message) { this->message = message; }
	const bool hasFontAttribute() const { return mHasFontAttribute; }
	const short getEncoding() const { return encoding; }
	const char getRed() const { return red; }
	const char getGreen() const { return green; }
	const char getBlue() const { return blue; }
	const char getFontSize() const { return fontSize; }
	const std::string &getFontName() const { return fontName; }
	const bool isBold() const { return bold; }
	const bool isItalic() const { return italic; }
	const bool isUnderline() const { return underline; }
	
	const unsigned char getNumFragments() const { return numFragments; }
	const unsigned char getSeqOfFragments() const { return seqFragments; }
	const short getMessageID() const { return messageID; }
	
	void setNumFragments(const unsigned char num) { numFragments = num; }
	void setSeqOfFragments( const unsigned char seq) { seqFragments = seq; }
	void setMessageID(const short id) { messageID = id; }
	
	const bool isNormalReply() const; // true, it is normal reply, otherwise autoreply (because only two values for replyType)
	ReceivedNormalIM &operator=(const ReceivedNormalIM &rhs);
protected:
	virtual void parseContents(const unsigned char *buf, const int len);
private:
	char unknown2[3];
	char replyType;
	std::string message;
	
	unsigned char numFragments;
	unsigned char seqFragments;
	short messageID;
	
	bool mHasFontAttribute;
	short encoding;
	char red, green, blue;
	char fontSize;
	std::string fontName;
	bool bold, italic, underline;
};

class ReceiveIMReplyPacket : public OutPacket 
{    
public:
	ReceiveIMReplyPacket() {}
	ReceiveIMReplyPacket(const char * key);
	ReceiveIMReplyPacket(const ReceiveIMReplyPacket &rhs);
	virtual ~ReceiveIMReplyPacket() {};
	
	virtual OutPacket *copy(){ return new ReceiveIMReplyPacket(*this);}
	ReceiveIMReplyPacket &operator=(const ReceiveIMReplyPacket &rhs);
	
	const char *getReplyKey() const { return replyKey; }

protected:
	virtual int putBody(unsigned char *buf);
private:
	char replyKey[REPLY_KEY_LENGTH];
};

class ReceivedQunIM{
public:
	ReceivedQunIM(const unsigned short src, const unsigned char *buf, const int len);
	ReceivedQunIM( const ReceivedQunIM &rhs);
	ReceivedQunIM() {};
	
	const short getUnknown1() const { return unknown1; }
	//const char *getUnknown3() const { return unknown3; }

	const unsigned short getSource() const { return source; }
	const unsigned int getExtID() const { return externalID; }
	const unsigned int getQunID() const { return qunID; } // only for temporary Qun message
	const char getType() const { return type; }
	const unsigned int getSenderQQ() const { return sender; }
	const short getSequence() const { return sequence; }
	const int getSentTime() const { return sentTime; }
	const unsigned int getVersionID() const { return versionID; }
	const std::string getMessage() const { return message; }
	void setMessage(std::string message) { this->message = message; }
	
	const bool hasFontAttribute() const { return mHasFontAttribute; }
	const short getEncoding() const { return encoding; }
	const char getRed() const { return red; }
	const char getGreen() const { return green; }
	const char getBlue() const { return blue; }
	const char getFontSize() const { return fontSize; }
	const std::string &getFontName() const { return fontName; }
	const bool isBold() const { return bold; }
	const bool isItalic() const { return italic; }
	const bool isUnderline() const { return underline; }
	
	ReceivedQunIM &operator=(const ReceivedQunIM &rhs);
	
	const unsigned char getNumFragments() const { return numFragments; }
	const unsigned char getSeqOfFragments() const { return seqFragments; }
	const short getMessageID() const { return messageID; }
	
	void setNumFragments(const unsigned char num) { numFragments = num; }
	void setSeqOfFragments( const unsigned char seq) { seqFragments = seq; }
	void setMessageID(const short id) { messageID = id; }
private:
	short source;  // this is to mention,  permenent, temporary qun 
	unsigned int externalID;
	unsigned int qunID; // for temporary Qun message only
	char type;
	unsigned int sender;
	short unknown1;
	
	unsigned char numFragments;
	unsigned char seqFragments;
	short messageID;
	
	short sequence;
	uint sentTime;
	unsigned int versionID;
	std::string message;
	
	bool mHasFontAttribute;
	short encoding;
	char red, green, blue;
	char fontSize;
	std::string fontName;
	bool bold, italic, underline;
	
	void parseData(const unsigned char *buf, const int len);
};


/*
	this class is used to analyse following notifications:
	
	QQ_RECV_IM_CREATE_QUN
	QQ_RECV_IM_ADDED_TO_QUN
	QQ_RECV_IM_DELETED_FROM_QUN
	QQ_RECV_IM_REQUEST_JOIN_QUN
	QQ_RECV_IM_APPROVE_JOIN_QUN
	 QQ_RECV_IM_REJECT_JOIN_QUN
	QQ_RECV_IM_SET_QUN_ADMIN
	 
	 note that:  
	 	QQ_RECV_IM_CREATE_QUN
		QQ_RECV_IM_ADDED_TO_QUN
		QQ_RECV_IM_DELETED_FROM_QUN
		QQ_RECV_IM_SET_QUN_ADMIN
		have no messages in its packet.
*/
class ReceivedQunIMJoinRequest{
public:
	ReceivedQunIMJoinRequest(const unsigned short type, const unsigned char *buf, const int len);
	ReceivedQunIMJoinRequest( const ReceivedQunIMJoinRequest &rhs);
	ReceivedQunIMJoinRequest();
	~ReceivedQunIMJoinRequest();

	const unsigned int getExtID() const { return externalID; }
	const unsigned int getSender() const { return sender; }
	const std::string &getMessage() const { return message; }
	const char getType() const { return type; }
	// if QQ_RECV_IM_SET_QUN_ADMIN packet, commander will be the 
	// action. 0: unset admin, 1: set admin
	const unsigned int getCommander() const { return commander; }  // from starkwong's patch
	
	const unsigned char *getCode() const { return m_Code; }
	const unsigned short getCodeLength() const { return m_CodeLen; }
	void setCode(const unsigned char *code, const unsigned short len);

	const unsigned char *getToken() const { return m_Token; }
	const unsigned short getTokenLength() const { return m_TokenLen; }
	void setToken(const unsigned char *token, const unsigned short len);

	ReceivedQunIMJoinRequest &operator=(const ReceivedQunIMJoinRequest &rhs);
private:
	unsigned int externalID;
	// sender is the admin or other member
	unsigned int sender;
	unsigned char type; // type of Qun
	std::string message;
	// when commander used, sender will be the member the admin
	// will action on
	unsigned int commander;// from starkwong's patch
	unsigned char *m_Code;
	unsigned short m_CodeLen;
	unsigned char *m_Token;
	unsigned short m_TokenLen;
};

class SignatureChangedPacket{
public:
	SignatureChangedPacket(const unsigned char *buf, const int len);
	SignatureChangedPacket( const SignatureChangedPacket &rhs);
	~SignatureChangedPacket() {};
	
	const unsigned int getQQ() const { return qq; }
	const unsigned int getTime() const { return time; }
	const std::string &getSignature() const { return signature; }
	SignatureChangedPacket &operator=(const SignatureChangedPacket &rhs);
private:
	unsigned int qq;
	unsigned int time;
	std::string signature;
	void parseData(const unsigned char *buf, const int len);
};


/*
	this class used in:
			QQ_IM_UDP_REQUEST
			QQ_IM_TCP_REQUEST
			QQ_IM_ACCEPT_UDP_REQUEST
			QQ_IM_NOTIFY_IP
			QQ_IM_UDP_EX_REQUEST        0x81
			QQ_IM_EX_REQUEST_ACCEPTED   0x83
			QQ_IM_EX_REQUEST_CANCELED   0x85
*/
class ReceivedFileIM : public NormalIMBase{
public:
	ReceivedFileIM() {}
	ReceivedFileIM(const unsigned char *buf, const int len);
	ReceivedFileIM(const ReceivedFileIM &rhs);
	virtual ~ReceivedFileIM() {};
	
	const unsigned char getTransferType() const { return m_TransferType; }
	const unsigned char getConnectMode() const { return m_ConnectMode; }
	//const unsigned short getSessionId() const { return m_SessionId; }
	const unsigned int getSessionId() const { return m_SessionId; }
	const unsigned int getWanIp() const { return m_WanIp; }
	const unsigned short getWanPort() const { return m_WanPort; }
	//const unsigned short getMajorPort() const { return m_MajorPort; }
	//const unsigned int getLanIp() const { return m_LanIp; }
	//const unsigned short getLanPort() const { return m_LanPort; }
	const unsigned char *getAgentServerKey() const { return m_AgentServerKey; }

	const std::string &getFileName() const { return m_FileName; }
	const unsigned int getFileSize() const { return m_FileSize; }
	
	ReceivedFileIM &operator=(const ReceivedFileIM &rhs);
protected:
	virtual void parseContents(const unsigned char *buf, const int len);
private:
	unsigned char m_TransferType;
	unsigned char m_ConnectMode;
	//unsigned short m_SessionId;
	unsigned int m_SessionId;
	unsigned int m_WanIp;
	unsigned short m_WanPort;

	unsigned char m_AgentServerKey[16];
	//unsigned short m_MajorPort;
	//unsigned int m_LanIp;
	//unsigned short m_LanPort;

	// the following two used when other side start a sending request:
	// types are QQ_IM_UDP_REQUEST and QQ_IM_TCP_REQUEST
	std::string m_FileName;
	unsigned int m_FileSize;
};


// QQ_IM_EX_NOTIFY_IP

class ReceivedFileExIpIM : public NormalIMBase{
public:
	ReceivedFileExIpIM() {}
	ReceivedFileExIpIM(const unsigned char *buf, const int len);
	ReceivedFileExIpIM(const ReceivedFileExIpIM &rhs);
	virtual ~ReceivedFileExIpIM() {};
	
	const unsigned char getTransferType() const { return m_TransferType; }
	const unsigned char getConnectMode() const { return m_ConnectMode; }
	const unsigned int getSessionId() const { return m_SessionId; }
	const unsigned int getWanIp1() const { return m_WanIp1; }
	const unsigned int getWanPort1() const { return m_WanPort1; }
	const unsigned int getWanIp2() const { return m_WanIp2; }
	const unsigned int getWanPort2() const { return m_WanPort2; }
	const unsigned int getWanIp3() const { return m_WanIp3; }
	const unsigned int getWanPort3() const { return m_WanPort3; }

	const unsigned int getLanIp1() const { return m_LanIp1; }
	const unsigned int getLanPort1() const { return m_LanPort1; }
	const unsigned int getLanIp2() const { return m_LanIp2; }       // in most cases this is 0
	const unsigned int getLanPort2() const { return m_LanPort2; }       // in most cases this is 0
	const unsigned int getLanIp3() const { return m_LanIp3; }
	const unsigned int getLanPort3() const { return m_LanPort3; }

	const unsigned int getSyncIp() const { return m_SyncIp; }
	const unsigned int getSyncPort() const { return m_SyncPort; }	
	const unsigned int getSyncSession() const { return m_SyncSession; }

	const bool isSender() const { return m_IsSender; }


	ReceivedFileExIpIM &operator=(const ReceivedFileExIpIM &rhs);

protected:
	virtual void parseContents(const unsigned char *buf, const int len);
private:
	unsigned char m_TransferType;
	unsigned char m_ConnectMode;
	unsigned int m_SessionId;

	bool m_IsSender;

	unsigned int m_WanIp1;
	unsigned int m_WanPort1;
	unsigned int m_WanIp2;
	unsigned int m_WanPort2;
	unsigned int m_WanIp3;
	unsigned int m_WanPort3;

	unsigned int m_LanIp1;
	unsigned int m_LanPort1;
	unsigned int m_LanIp2;
	unsigned int m_LanPort2;
	unsigned int m_LanIp3;
	unsigned int m_LanPort3;

	unsigned int m_SyncIp;
	unsigned int m_SyncPort;
	unsigned int m_SyncSession;
};

class ReceivedQQNews {
public:
	ReceivedQQNews(const unsigned char *buf, const int len);
	ReceivedQQNews( const ReceivedQQNews &rhs);
	~ReceivedQQNews() {};

	const std::string &getTitle() const { return m_Title; }
	const std::string &getBrief() const { return m_Brief; }
	const std::string &getURL() const { return m_URL; }

	ReceivedQQNews &operator=(const ReceivedQQNews &rhs);
private:
	std::string m_Title;
	std::string m_Brief;
	std::string m_URL;
	void parseData(const unsigned char *buf, const int len);
};

#endif

