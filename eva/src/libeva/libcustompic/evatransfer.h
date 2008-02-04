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
 
#ifndef LIBCUSTOMPIC_EVATRANSFER_H
#define LIBCUSTOMPIC_EVATRANSFER_H 

#include "evapicpacket.h" 
#include <string>

class EvaPicTransferPacket : public EvaPicOutPacket{
public:
	EvaPicTransferPacket();
	EvaPicTransferPacket(const bool isDataPacket, const bool isLastPacket);
	EvaPicTransferPacket(const EvaPicTransferPacket &rhs);
	virtual ~EvaPicTransferPacket();
	
	EvaPicTransferPacket &operator=(const EvaPicTransferPacket &rhs);
	
	const unsigned int getSessionID() const {return sessionID; }
	const unsigned char *getMD5() const { return md5;}
	const std::string &getFileName() const { return fileName; }
	const unsigned int getImageLength() const { return imageLength; }
	
	const unsigned char *getFragment() const { return fragment; }
	const unsigned short getFragmentLength() const { return fragLength; }
	
	const bool isDataPacket() const { return isData; }
	const bool isLastPacket() const { return isLast; }
	
	const bool isRequestSend() const { return requestSend; }
	const bool isDataReply() const { return dataReply; }
	
	void setSessionID(const unsigned int id) { sessionID=id;}
	
	void setMd5(const unsigned char *value);
	void setImageLength(const unsigned short len) { imageLength = len; }
	void setFileName( const std::string &name) { fileName = name ;}
	
	void setFragment(const unsigned char *data, const unsigned int len);
	void setDataPacket(const bool ok) { isData = ok; }
	void setLastPacket(const bool ok) { isLast = ok; }
	
	void setRequestSend(const bool ok) { requestSend= ok; }
	void setDataReply( const bool ok) { dataReply = ok; }
	
protected:
	int putBody(unsigned char *buf);
private:
	unsigned int sessionID;
	
	// for send file info
	unsigned char md5[16]; // always 16 bytes long
	unsigned int imageLength;
	std::string fileName;
	
	// for user sending image
	unsigned char *fragment;
	unsigned short fragLength;
	bool isData;
	bool isLast;
	
	bool requestSend;
	bool dataReply;
};

class EvaPicTransferReplyPacket : public EvaPicInPacket{
public:
	EvaPicTransferReplyPacket() {}
	EvaPicTransferReplyPacket(unsigned char *buf, int len);
	EvaPicTransferReplyPacket(const EvaPicTransferReplyPacket &rhs);
	virtual ~EvaPicTransferReplyPacket();
	
	EvaPicTransferReplyPacket &operator=(const EvaPicTransferReplyPacket &rhs);
	
	const unsigned int getSessionID() const { return sessionID; }
	const unsigned char *getMd5() const { return md5; }
	const unsigned char *getFileNameMd5() const { return fileNameMd5; }
	const std::string &getFileName() const { return fileName; }
	const unsigned int getImageLength() const { return imageLength; }
	
	const unsigned char *getData() const { return data; }
	const unsigned int getDataLength() const { return dataLength; }
protected:
	void parseBody();
private:
	unsigned int sessionID;
	// server send to me file info packet
	unsigned char md5[16];
	unsigned char fileNameMd5[16];
	std::string fileName;
	unsigned int imageLength;
	
	// data packet
	unsigned char *data;
	unsigned int dataLength;
};

#endif
