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
#ifndef LIBEVAEXTRAINFO_H
#define LIBEVAEXTRAINFO_H

#include "evapacket.h" 
#include <string>
#include <map>

class RequestExtraInfoPacket : public OutPacket
{
public:
	RequestExtraInfoPacket();
	RequestExtraInfoPacket(const unsigned short offset);
	RequestExtraInfoPacket(const RequestExtraInfoPacket &rhs);
	virtual ~RequestExtraInfoPacket() {};
	
	OutPacket * copy() { return new RequestExtraInfoPacket(*this);}
	RequestExtraInfoPacket &operator=(const RequestExtraInfoPacket &rhs);
	
	void setOffset(const unsigned short offset) { mOffset = offset; }
	const unsigned short getOffset() const { return mOffset; }
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned short mOffset;
};

class RequestExtraInfoReplyPacket : public InPacket 
{
public: 
	RequestExtraInfoReplyPacket() {}
	RequestExtraInfoReplyPacket(unsigned char *buf, int len);
	RequestExtraInfoReplyPacket(const RequestExtraInfoReplyPacket &rhs);
	virtual ~RequestExtraInfoReplyPacket(){};
 
	const unsigned char getReplyCode() const { return mReplyCode; }
	const unsigned short getOffset() const { return mOffset; }
	const std::map<unsigned int, unsigned long long> &getMembers() const { return mMembers; }
	
	RequestExtraInfoReplyPacket &operator=(const RequestExtraInfoReplyPacket &rhs);
protected:
	virtual void parseBody() ;
private:
	unsigned char mReplyCode;
	unsigned short mOffset;
	std::map<unsigned int, unsigned long long> mMembers;
};

class SignaturePacket : public OutPacket 
{
public:
	SignaturePacket() {}
	SignaturePacket(const unsigned char type); // type includes QQ_SIGNATURE_MODIFY, QQ_SIGNATURE_DELETE and QQ_SIGNATURE_REQUEST
	SignaturePacket(const SignaturePacket &rhs);
	virtual ~SignaturePacket() {};
	
	SignaturePacket &operator=(const SignaturePacket &rhs);
	
	void setType(const unsigned char type) { mType = type; }
	const unsigned char getType() const { return mType; }
	const std::map<unsigned int, unsigned int> &getMembers() const { return mMembers; }
	void setMembers(const std::map<unsigned int, unsigned int> &list) { mMembers = list; }
	//Only used for downloading single contact signature
	void setSignatureArgs(const unsigned int id, const unsigned int time) { mMembers[id] = time;}
	
	void setSignature( const std::string sig) { mSignature = sig; }
	const std::string &getSignature() const { return mSignature; }
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned char mType;
	
	std::string mSignature;
	std::map<unsigned int, unsigned int> mMembers;
};

typedef struct SignatureElement{
	unsigned int lastModifyTime;
	std::string signature;
} SignatureElement;

class SignatureReplyPacket : public InPacket
{
public: 
	SignatureReplyPacket() {}
	SignatureReplyPacket(unsigned char *buf, int len);
	SignatureReplyPacket(const SignatureReplyPacket &rhs);
	virtual ~SignatureReplyPacket(){};
 
	const unsigned char getType() const { return mType; }
	const char getReplyCode() const { return mReplyCode; }
	const bool isChangeAccepted() const { return mReplyCode == 0x00; }
	
	const unsigned int nextStartID() const { return mNextStartId; }
	const std::map<unsigned int, SignatureElement> &getMembers() const { return mMembers; }
	
	SignatureReplyPacket &operator=(const SignatureReplyPacket &rhs);
protected:
	virtual void parseBody() ;
private:
	unsigned char mType;
	unsigned char mReplyCode;
	unsigned int mNextStartId;
	std::map<unsigned int, SignatureElement> mMembers;
};

#endif

