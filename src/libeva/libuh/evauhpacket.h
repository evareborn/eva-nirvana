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

// the base class for all user head packets
// Tencent QQ uses User Head instead of avatar, so we just follow the same way -__-!

#ifndef EVAUSERHEADPACKET_H
#define EVAUSERHEADPACKET_H

#define UH_HEAD_TAG      0x03

#define UH_CMD_INFO      0x04
#define UH_CMD_TRANSFER  0x02

class EvaUHPacket {
public:
	// for incoming packets
	EvaUHPacket( const unsigned char *buf, const int len);
	// for outcoming packets
	EvaUHPacket(const bool isInfo);
	virtual ~EvaUHPacket();
	
	// for incoming packets	
	const bool parse();
	// for outcoming packets
	const bool fill(unsigned char *buf, int *len);

	const unsigned char getCommand() { return mCommand; }
	const int getNumPackets() const { return (int)mNumPackets; }
	const int getPacketNum() const { return (int)mPacketNum; }
	void setNumPackets(const int num) { mNumPackets = (0xff & num); }
	void setPacketNum(const int num) { mPacketNum = (0xff & num); }

	const bool isDataPacket() { return mIsData; }
protected:
	virtual const bool parseBody();
	virtual const bool fillBody(unsigned char *buf, int *len);
	int mBodyStart;
	unsigned char *buffer;
	int mBufferLen;
	unsigned short mVersion;
	bool mIsData;
private:
	static unsigned short seq_random;
	static unsigned short seq_info;
	static unsigned short seq_transfer;
	unsigned char mCommand;
	unsigned char mNumPackets;
	unsigned char mPacketNum; 
	const int parseHead();
	const bool fillHead(unsigned char *buf);
};

#endif // EVAUSERHEADPACKET_H

 
