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

#ifndef EVAUSERHEADPROTOCOLS_H
#define EVAUSERHEADPROTOCOLS_H

#include "evauhpacket.h"
#include <list>

class EvaUHInfoRequest : public EvaUHPacket {
public:
	EvaUHInfoRequest();
	virtual ~EvaUHInfoRequest() {}

	void setQQList(const std::list<unsigned int> &l)	 { mList = l; }
	std::list<unsigned int> getQQList() { return mList; }
protected:
	virtual const bool fillBody(unsigned char *buf, int *len);
private:
	std::list<unsigned int> mList;
};

typedef struct {
	int id;
	unsigned int sessionId;
	char md5[16];  // the md5 value used as file name as well
	bool isUpdated; // useless for protocol, just help the client side code
} UHInfoItem;

class EvaUHInfoReply : public EvaUHPacket {
public:
	EvaUHInfoReply( const unsigned char *buf, const int len);
	virtual ~EvaUHInfoReply() {}

	const int numInfoItems() { return mList.size(); }
	std::list<UHInfoItem> getInfoItems() { return mList; }
	const bool isSuccessful() const { return replyCode == 0x00; }
protected:
	virtual const bool parseBody();
private:
	unsigned char replyCode;
	std::list<UHInfoItem> mList;
};

class EvaUHTransferRequest : public EvaUHPacket{
public:
	EvaUHTransferRequest();
	virtual ~EvaUHTransferRequest() {}

	void setUHInfo(const int id, const unsigned int sessionId) 
		{ mId = id; mSessionId = sessionId; }
	void setFileInfo(const unsigned  int start, const unsigned  int end) 
		{ mStart = start; mEnd = end; }
protected:
	virtual const bool fillBody(unsigned char *buf, int *len);
private:
	int mId;
	unsigned int mSessionId;
	unsigned int mStart;
	unsigned int mEnd;
};

class EvaUHTransferReply : public EvaUHPacket{
public:
	EvaUHTransferReply( const unsigned char *buf, const int len);
	virtual ~EvaUHTransferReply() {}

	const int getQQ() const { return mId; }
	const unsigned int getSessionId() const { return mSessionId; }
	const unsigned int getFileSize() const { return mFileSize; }
	const unsigned int getStart() const { return mStart; }
	const unsigned int getPartSize() const { return mPartSize; }
	const unsigned char *getPartData() const { return mPartData; }

protected:
	virtual const bool parseBody();
private:
	int mId;
	unsigned int mSessionId;
	unsigned int mFileSize;
	unsigned int mStart;
	unsigned int mPartSize;
	unsigned char *mPartData;
};

#endif // EVAUSERHEADPROTOCOLS_H
