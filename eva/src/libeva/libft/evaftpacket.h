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

#ifndef EVAFTPACKET_H
#define EVAFTPACKET_H

#define QQ_FILE_AGENT_PACKET_TAG           0x04
#define QQ_FILE_AGENT_PACKET_TAIL          0x03

#define QQ_FILE_AGENT_CMD_CREATE           0x0001
#define QQ_FILE_AGENT_CMD_LOGIN            0x0002
#define QQ_FILE_AGENT_CMD_TRANSFER         0x0003
#define QQ_FILE_AGENT_CMD_READY            0x0005
#define QQ_FILE_AGENT_CMD_START            0x0006

#define QQ_FILE_SYN_CMD_CREATE             0x0001
#define QQ_FILE_SYN_CMD_REGISTER           0x0002


class EvaFTPacket {
public:
	// for incoming packets
	EvaFTPacket(const unsigned char *buf, const int len);
	// for outcoming packets
	EvaFTPacket();
	virtual ~EvaFTPacket();

	// for incoming packets
	const bool parse();
	// for outcoming packets
	const bool fill(unsigned char *buf, int *len);

	// for incoming packets
	const unsigned char *getBody() const { return m_Buffer; }
	const int getBodyLength() const { return m_BufferLength; }
protected:
	void setPacketTag(const unsigned char tag, const unsigned char tail);
	virtual const int parseHeader(unsigned char *buf);
	virtual const bool parseBody(unsigned char *buf, const int len);
	virtual const int fillHeader(unsigned char *buf);
	virtual const int fillBody(unsigned char *buf);

	inline const unsigned char getTag() const { return m_Tag; }
	inline const unsigned char getTail() const { return m_Tail; }

	int m_HeaderLength;
	bool m_HasTags;
	bool m_HasLength;
	unsigned int m_LengthOffset;
private:
	unsigned char m_Tag, m_Tail;

	// for incoming packets
	unsigned char *m_Buffer;
	int m_BufferLength;
};

class EvaFTAgentPacket : public EvaFTPacket{
public:
	EvaFTAgentPacket(const unsigned char *buf, const int len);
	EvaFTAgentPacket(const short cmd);


	inline void setVersion(const unsigned short version) { m_Version = version; }
	inline void setSequence(const unsigned short seq) { m_Sequence = seq; }
	inline void setCommand(const unsigned short cmd) { m_Command = cmd; }
	inline void setQQ(const int qq) { m_Id = qq; }
	inline void setSessionId(const unsigned int sid) { m_Session = sid; }
	inline const unsigned short getVersion() const { return m_Version; }
	inline const unsigned short getSequence() const { return m_Sequence; }
	inline const unsigned short getCommand() const { return m_Command; }
	inline const int getQQ() const { return m_Id; }
	inline const unsigned int getSessionId() const { return m_Session; }

	void setFileAgentKey(const unsigned char *key);
protected:
	const int parseHeader(unsigned char *buf);
	//virtual const bool pareseBody();
	const int fillHeader(unsigned char *buf);
	//virtual const unsigned int fillBody(unsigned char *buf);

	unsigned char m_FileAgentKey[16];
	unsigned int m_Session;
private:
	unsigned short m_Version;
	unsigned short m_Command;
	unsigned short m_Sequence;
	int m_Id;
};

class EvaFTSynPacket : public EvaFTPacket{
public:
	EvaFTSynPacket(const unsigned char *buf, const int len);
	EvaFTSynPacket(const short cmd);

	inline void setVersion(const unsigned short version) { m_Version = version; }
	inline void setSequence(const unsigned short seq) { m_Sequence = seq; }
	inline void setCommand(const unsigned short cmd) { m_Command = cmd; }
	inline void setQQ(const int qq) { m_Id = qq; }
	inline void setSessionId(const unsigned int sid) { m_Session = sid; }
	inline const unsigned short getVersion() const { return m_Version; }
	inline const unsigned short getSequence() const { return m_Sequence; }
	inline const unsigned short getCommand() const { return m_Command; }
	inline const int getQQ() const { return m_Id; }
	inline const unsigned int getSessionId() const { return m_Session; }

	void setFileAgentKey(const unsigned char *key);
protected:
	const int parseHeader(unsigned char *buf);
	const int fillHeader(unsigned char *buf);

	unsigned char m_FileAgentKey[16];
	unsigned int m_Session;
private:
	static const unsigned char m_StartTag = 0x02;
	unsigned short m_Version;
	unsigned short m_Command;
	unsigned short m_Sequence;
	// for sending , m_Id is my qq number
	int m_Id;
};

#endif // #ifndef EVAFTPACKET_H

