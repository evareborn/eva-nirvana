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
#ifndef LIBEVAADDFRIEND_H
#define LIBEVAADDFRIEND_H

#include "evapacket.h"
#ifndef WIN32
#include <inttypes.h>
#endif
#include <string>

class AddFriendPacket : public OutPacket
{
public:
	AddFriendPacket();
	AddFriendPacket(const int id);
	AddFriendPacket(const AddFriendPacket &rhs);
	virtual ~AddFriendPacket() {};
	
	OutPacket *copy(){return new AddFriendPacket(*this);}
	AddFriendPacket &operator=( const AddFriendPacket &rhs );
	
	void setAddQQ(const int id) { qqNum = id; }
	const int getAddQQ() const { return qqNum; }
protected:
	virtual int putBody(unsigned char* buf);
private:
	int qqNum;
};

class AddFriendReplyPacket : public InPacket
{
public:
	AddFriendReplyPacket() {}
	AddFriendReplyPacket(unsigned char* buf, int len);
	AddFriendReplyPacket(const AddFriendReplyPacket &rhs);	
	virtual ~AddFriendReplyPacket() {};
	
	InPacket *copy() { return new AddFriendReplyPacket(*this); }
	AddFriendReplyPacket &operator=(const AddFriendReplyPacket &rhs);
	
	const int getMyQQ() const { return myQQ; }
	const uint8_t getReplyCode() const { return replyCode; }	
protected:
	virtual void parseBody();
private:		
	uint8_t replyCode;
	int myQQ;
};

class AddFriendAuthPacket : public OutPacket
{
public:
	AddFriendAuthPacket();
	AddFriendAuthPacket(const int id, const uint8_t type);
	AddFriendAuthPacket( AddFriendAuthPacket& rhs);
	
	virtual ~AddFriendAuthPacket() {};
	
	
	virtual OutPacket* copy(){return new AddFriendAuthPacket(*this);}
	AddFriendAuthPacket &operator=(const AddFriendAuthPacket &rhs);
	
	const std::string getMessage() const { return message; }
	void setMessage(const std::string &msg) { message = msg; }
	
	const int getBuddyQQ() const { return buddyQQNum; }
	void setBuddyQQ(const int id) { buddyQQNum = id; };
	
	const uint8_t getType() const { return type; }
	void setType(const uint8_t type) { this->type = type; }
protected:		
	virtual int putBody(unsigned char* buf);

private:
	static const unsigned char DIVIDER = 0x1f;
	uint8_t type;
	int buddyQQNum;
	std::string message;
};

class AddFriendAuthReplyPacket : public InPacket
{
public:
	AddFriendAuthReplyPacket() {}
	AddFriendAuthReplyPacket(unsigned char* buf, int len);
	AddFriendAuthReplyPacket(const AddFriendAuthReplyPacket &rhs);
	virtual ~AddFriendAuthReplyPacket() {};

	virtual InPacket* copy(){return new AddFriendAuthReplyPacket(*this);}
	AddFriendAuthReplyPacket &operator=(const AddFriendAuthReplyPacket &rhs);
	
	const uint8_t getReplyCode() const { return replyCode; }
	const bool isSentOK() const;
	
protected:
	virtual void parseBody();
private:
	uint8_t replyCode;
};

class DeleteFriendPacket : public OutPacket
{
public:
	DeleteFriendPacket();
	DeleteFriendPacket(const int id);
	DeleteFriendPacket(const DeleteFriendPacket &rhs);
	virtual ~DeleteFriendPacket() {};
	
	OutPacket *copy(){return new DeleteFriendPacket(*this);}
	DeleteFriendPacket &operator=(const DeleteFriendPacket &rhs);
	
	void setBuddyQQ(int id) { buddyQQNum = id; }
	const int getBuddyQQ() const { return buddyQQNum; }
protected:		
	virtual int putBody(unsigned char* buf);
private:
	int buddyQQNum;
};

class DeleteFriendReplyPacket : public InPacket
{
public:
	DeleteFriendReplyPacket() {}
	DeleteFriendReplyPacket(unsigned char* buf, int len);
	DeleteFriendReplyPacket( const DeleteFriendReplyPacket &rhs);
	virtual ~DeleteFriendReplyPacket() {};
		
	virtual InPacket* copy(){return new DeleteFriendReplyPacket(*this);}
	DeleteFriendReplyPacket &operator=(const DeleteFriendReplyPacket &rhs);
	
	const uint8_t getReplyCode() const { return replyCode; }
	const bool isDeleted() const;		
protected:
	virtual void parseBody();
private:
	uint8_t replyCode;

};

class DeleteMePacket : public OutPacket 
{
public:
	DeleteMePacket();
	DeleteMePacket(const int id);
	DeleteMePacket(const DeleteMePacket &rhs);
	virtual ~DeleteMePacket() {};
	
	OutPacket *copy(){return new DeleteMePacket(*this);}
	DeleteMePacket &operator=(const DeleteMePacket &rhs);
	
	void setBuddyQQ(int id) { buddyQQNum = id; }
	const int getBuddyQQ() const { return buddyQQNum; }
protected:		
	virtual int putBody(unsigned char* buf);
private:
	int buddyQQNum;
};

class DeleteMeReplyPacket : public InPacket
{
public:
	DeleteMeReplyPacket() {}
	DeleteMeReplyPacket(unsigned char* buf, int len);
	DeleteMeReplyPacket( const DeleteMeReplyPacket &rhs);
	virtual ~DeleteMeReplyPacket() {};
		
	virtual InPacket* copy(){return new DeleteMeReplyPacket(*this);}
	DeleteMeReplyPacket &operator=(const DeleteMeReplyPacket &rhs);
	
	const uint8_t getReplyCode() const { return replyCode; }
	const bool isDeleted() const;
protected:
	virtual void parseBody();
private:
	uint8_t replyCode;

};

class SystemNotificationPacket : public InPacket
{
public:
	SystemNotificationPacket() ;
	SystemNotificationPacket(unsigned char* buf, int len);
	SystemNotificationPacket( const SystemNotificationPacket &rhs);	
	virtual ~SystemNotificationPacket();
	
	virtual InPacket* copy(){return new SystemNotificationPacket(*this);}
	SystemNotificationPacket &operator=(const SystemNotificationPacket &rhs); 
	
	const unsigned char getType() const { return type; }
	const unsigned int getMyQQ() const { return myQQ; }
	const unsigned int getFromQQ() const { return fromQQ; }
	const std::string getMessage() const { return message; }

	const unsigned char *getCode() const { return m_Code; }
	const unsigned int getCodeLength() const { return m_CodeLen; }

	void setCode(const unsigned char *code, const unsigned int len);

	const bool allowReverse() const { return m_AllowReverse; }
protected:		
	virtual void parseBody();
private:
	static const uint8_t DIVIDER = 0x1f;
	unsigned char type;
	unsigned int fromQQ;
	unsigned int myQQ;
	std::string message;
	bool m_AllowReverse;
	unsigned char * m_Code;
	unsigned int m_CodeLen;
};

class VerifyAddingMsgPacket : public OutPacket
{
public:
	VerifyAddingMsgPacket();
	VerifyAddingMsgPacket(const unsigned int id);
	VerifyAddingMsgPacket(const VerifyAddingMsgPacket &rhs);
	virtual ~VerifyAddingMsgPacket();
	
	OutPacket *copy(){return new VerifyAddingMsgPacket(*this);}
	VerifyAddingMsgPacket &operator=(const VerifyAddingMsgPacket &rhs);
	
	void setBuddyQQ(unsigned int id) { m_BuddyQQNum = id; }
	const unsigned int getBuddyQQ() const { return m_BuddyQQNum; }
	void setCode(const unsigned char *code, const unsigned int len);
	
	const unsigned char *getCode() const { return m_Code; }
	const unsigned int getCodeLength() const { return m_CodeLen; }
protected:		
	virtual int putBody(unsigned char* buf);
private:
	unsigned int m_BuddyQQNum;
	unsigned char * m_Code;
	unsigned int m_CodeLen;
};

class VerifyAddingMsgReplyPacket : public InPacket
{
public:
	VerifyAddingMsgReplyPacket() {}
	VerifyAddingMsgReplyPacket(unsigned char* buf, int len);
	VerifyAddingMsgReplyPacket( const VerifyAddingMsgReplyPacket &rhs);
	virtual ~VerifyAddingMsgReplyPacket() {};
		
	virtual InPacket* copy(){return new VerifyAddingMsgReplyPacket(*this);}
	VerifyAddingMsgReplyPacket &operator=(const VerifyAddingMsgReplyPacket &rhs);
	
	const unsigned char getFlag1() const { return m_Flag1; }
	const unsigned char getFlag2() const { return m_Flag2; }
	const unsigned int getBuddyQQ() const { return m_BuddyQQNum; }
	const unsigned char getReplyCode() const { return m_ReplyCode; }
protected:
	virtual void parseBody();
private:
	unsigned int m_BuddyQQNum;
	unsigned char m_Flag1;
	unsigned char m_Flag2;
	unsigned char m_ReplyCode;
};

#endif

