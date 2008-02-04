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

#include "evaftpacket.h"
#include "../evautil.h"
#include <string.h>
#include <stdio.h>


EvaFTPacket::EvaFTPacket(const unsigned char *buf, const int len)
	: m_HeaderLength(0), m_HasTags(false), m_HasLength(false),
	m_LengthOffset(0), m_Buffer(NULL), m_BufferLength(len)
{
	m_Buffer = new unsigned char[m_BufferLength];
	memcpy(m_Buffer, buf, m_BufferLength);
}

EvaFTPacket::EvaFTPacket()
	: m_HeaderLength(0), m_HasTags(false), m_Buffer(NULL), m_BufferLength(0)
{
}

EvaFTPacket::~EvaFTPacket()
{
	if(m_Buffer) delete m_Buffer;
}

const bool EvaFTPacket::parse()
{
	if(!m_Buffer) return false;
	int pos = 0;
	if(m_HasTags){
		m_Tag = m_Buffer[0];
		m_Tail = m_Buffer[m_BufferLength - 1];
		pos++;
	}
	m_HeaderLength = parseHeader(m_Buffer + pos);
	if(m_HeaderLength == -1) return false;
	pos += m_HeaderLength;
	return parseBody(m_Buffer + pos, m_BufferLength - pos - (m_HasTags?1:0));
}

const bool EvaFTPacket::fill(unsigned char *buf, int *len)
{
	int pos=0;
	if(m_HasTags){
		buf[pos++] = m_Tag;
	}

	int partLen = 0;

	partLen = fillHeader(buf + pos);
	if(partLen == -1) return false;
	pos += partLen;

	partLen = fillBody(buf + pos);
	if(partLen == -1) return false;
	pos += partLen;

	if(m_HasTags){
		buf[pos++] = m_Tail;
	}
	*len = pos;

	// finally, we set the length
	if(m_HasLength)
		EvaUtil::write16(buf + m_LengthOffset, *len);
	return true;
}

void EvaFTPacket::setPacketTag(const unsigned char tag, const unsigned char tail)
{
	m_HasTags = true;
	m_Tag = tag;
	m_Tail = tail;
}

const int EvaFTPacket::parseHeader(unsigned char */*buf*/)
{
	fprintf(stderr, "EvaFTPacket::parseHeader -- Not implemented!\n");
	return -1;
}

const bool EvaFTPacket::parseBody(unsigned char */*buf*/, const int /*len*/)
{
	fprintf(stderr, "EvaFTPacket::parseBody -- Not implemented!\n");
	return false;
}

const int EvaFTPacket::fillHeader(unsigned char */*buf*/)
{
	fprintf(stderr, "EvaFTPacket::fillHeader -- Not implemented!\n");
	return -1;
}

const int EvaFTPacket::fillBody(unsigned char */*buf*/)
{
	fprintf(stderr, "EvaFTPacket::fillBody -- Not implemented!\n");
	return -1;
}


/** ============================================================================ */


EvaFTAgentPacket::EvaFTAgentPacket(const unsigned char *buf, const int len)
	: EvaFTPacket(buf, len)
{
	m_HasTags = true;
}

EvaFTAgentPacket::EvaFTAgentPacket(const short cmd)
	: EvaFTPacket(), m_Command(cmd)
{
	setPacketTag(QQ_FILE_AGENT_PACKET_TAG, QQ_FILE_AGENT_PACKET_TAIL);
	m_HasLength = true;
	m_LengthOffset = 3;
}

const int EvaFTAgentPacket::parseHeader(unsigned char *buf)
{
	if(getTag() != QQ_FILE_AGENT_PACKET_TAG || getTail() != QQ_FILE_AGENT_PACKET_TAIL)
		return -1;
	int pos = 0;
	m_Version = EvaUtil::read16(buf + pos); pos+=2;

	unsigned short tmp2 = EvaUtil::read16(buf + pos); pos+=2;
	if(tmp2 != getBodyLength()) return -1;

	m_Command = EvaUtil::read16(buf + pos); pos+=2;
	m_Sequence = EvaUtil::read16(buf + pos); pos+=2;
	m_Id = EvaUtil::read32(buf + pos); pos+=4;
	pos+=8; // 8 unknown bytes
	if(m_Command != QQ_FILE_AGENT_CMD_CREATE){
		m_Session = EvaUtil::read32(buf + pos); pos+=4;
	}

	return pos;
}

const int EvaFTAgentPacket::fillHeader(unsigned char *buf)
{
	int pos = 0;
	pos += EvaUtil::write16(buf + pos, m_Version);
	pos +=2; // we leave the length for its parent to do;
	pos += EvaUtil::write16(buf + pos, m_Command);
	pos += EvaUtil::write16(buf + pos, m_Sequence);
	pos += EvaUtil::write32(buf + pos, m_Id);
	pos += 8; // 8 unknown bytes

	if(m_Command != QQ_FILE_AGENT_CMD_CREATE){
		pos += EvaUtil::write32(buf + pos, m_Session);
	}
	return pos;
}

void EvaFTAgentPacket::setFileAgentKey(const unsigned char *key)
{
	if(!key) return;
	memcpy(m_FileAgentKey, key, 16);
}




/** ============================================================================ */


EvaFTSynPacket::EvaFTSynPacket(const unsigned char *buf, const int len)
	: EvaFTPacket(buf, len)
{
}

EvaFTSynPacket::EvaFTSynPacket(const short cmd)
	: EvaFTPacket(), m_Command(cmd)
{
	m_HasLength = true;
	m_LengthOffset = 1;
}

const int EvaFTSynPacket::parseHeader(unsigned char *buf)
{
	int pos = 0;
	if(buf[pos++] != m_StartTag) return -1;

	unsigned short tmp2 = EvaUtil::read16(buf + pos); pos+=2;
	if(tmp2 != getBodyLength()) return -1;

	m_Version = EvaUtil::read16(buf + pos); pos+=2;
	m_Sequence = EvaUtil::read16(buf + pos); pos+=2;
	m_Id = EvaUtil::read32(buf + pos); pos+=4;
	m_Command = EvaUtil::read16(buf + pos); pos+=2;

	return pos;
}

const int EvaFTSynPacket::fillHeader(unsigned char *buf)
{
	int pos = 0;
	buf[pos++] = m_StartTag;
	pos +=2; // we leave the length for its parent to do;
	pos += EvaUtil::write16(buf + pos, m_Version);
	pos += EvaUtil::write16(buf + pos, m_Sequence);
	pos += EvaUtil::write32(buf + pos, m_Id);
	pos += EvaUtil::write16(buf + pos, m_Command);

	return pos;
}

void EvaFTSynPacket::setFileAgentKey(const unsigned char *key)
{
	if(!key) return;
	memcpy(m_FileAgentKey, key, 16);
}


































