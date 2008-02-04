/***************************************************************************
 *   Copyright (C) 2007 by yunfan                                          *
 *   yunfan_zg@163.com                                                     *
 *   Copyright (C) 2005 by casper                                          *
 *   tlmcasper@163.com                                                     *
 *                                                                         *
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
#ifndef LIBEVAADDFRIENDEX_H
#define LIBEVAADDFRIENDEX_H

#include "evapacket.h"
#include <string>

class EvaAddFriendExPacket : public OutPacket
{
public:
	EvaAddFriendExPacket();
	EvaAddFriendExPacket(const unsigned int id);
	EvaAddFriendExPacket(const EvaAddFriendExPacket &rhs);
	virtual ~EvaAddFriendExPacket() {};
	
	OutPacket *copy(){return new EvaAddFriendExPacket(*this);}
	EvaAddFriendExPacket &operator=( const EvaAddFriendExPacket &rhs );
	
	void setAddQQ(const int id ) { m_AddQQNum = id; }
	const int getAddQQ() const { return m_AddQQNum; }
	
protected:
	virtual int putBody(unsigned char* buf);
private:
	int m_AddQQNum;
};

class EvaAddFriendExReplyPacket : public InPacket
{
public:
	EvaAddFriendExReplyPacket() {}
	EvaAddFriendExReplyPacket(unsigned char* buf, int len);
	EvaAddFriendExReplyPacket(const EvaAddFriendExReplyPacket &rhs);	
	virtual ~EvaAddFriendExReplyPacket() {};
	
	InPacket *copy() { return new EvaAddFriendExReplyPacket(*this); }
	EvaAddFriendExReplyPacket &operator=(const EvaAddFriendExReplyPacket &rhs);
	
	const unsigned int getAddQQ() const { return m_AddQQNum; }
	const unsigned char getReplyCode() const { return m_ReplyCode; }
	const unsigned char getAuthStatus() const { return m_AuthStatus; }
	
	void setAddQQ( const unsigned int id ) { m_AddQQNum = id; }
	void setAuthStatus( const unsigned char authStatus ) { m_AuthStatus = authStatus; }
	void setReplyCode( const unsigned char replyCode ) { m_ReplyCode = replyCode; }
		
protected:
	virtual void parseBody();
private:
	// QQ_ADD_FRIEND_EX_ADDING_POSSIBLE or QQ_ADD_FRIEND_EX_ALREADY_IN_LIST
	unsigned char m_ReplyCode;

	//one of QQ_AUTH_NO_AUTH, QQ_AUTH_NEED_AUTH, QQ_AUTH_NO_ADD and QQ_AUTH_HAS_QUESTION
	unsigned char m_AuthStatus;

	unsigned int m_AddQQNum;
};


class EvaAddFriendAuthExPacket : public OutPacket
{
public:
	EvaAddFriendAuthExPacket();
	EvaAddFriendAuthExPacket(const unsigned int id, const unsigned char authStatus);
	EvaAddFriendAuthExPacket( EvaAddFriendAuthExPacket& rhs);
	
	
	virtual ~EvaAddFriendAuthExPacket();
	
	
	virtual OutPacket* copy(){return new EvaAddFriendAuthExPacket(*this);}
	EvaAddFriendAuthExPacket &operator=(const EvaAddFriendAuthExPacket &rhs);
	
	
	const std::string getMessage() const { return m_Message; }
	void setMessage(const std::string &msg) { m_Message = msg; }
	
	const unsigned int getAddQQ() const { return m_AddQQNum; }
	void setAddQQ(const unsigned int id) { m_AddQQNum = id; }
	
	const unsigned int getDestGroup() const { return m_DestGroup; }
	void setDestGroup(const unsigned int destGroup) { m_DestGroup = destGroup; }
	
	const bool getAllowAddReverse() const { return m_AllowAddReverse; }
	void setAllowAddReverse( const bool allowAddReverse) { m_AllowAddReverse = allowAddReverse; }
	
	const unsigned char getAuthStatus() const { return m_AuthStatus; }
	void setAuthStatus(const unsigned char authStatus ) { m_AuthStatus = authStatus; }
	
	void setAuthCode( const unsigned char *code, const unsigned short len);
	const unsigned char *getAuthCode() const { return m_AuthCode; }
	const unsigned short getAuthCodeLength() const { return m_AuthCodeLen; }

	void setQuestionCode( const unsigned char *code, const unsigned short len);
	const unsigned char *getQuestionCode() const { return m_QuestionCode; }
	const unsigned short getQuestionCodeLength() const { return m_QuestionCodeLen; }

protected:		
	virtual int putBody(unsigned char* buf);

private:
	unsigned char m_AuthStatus;// 0x00: no auth, 0x02: need auth, 0x10: add with question codes
	unsigned int m_DestGroup;
	bool m_AllowAddReverse;
	unsigned int m_AddQQNum;
	unsigned short m_AuthCodeLen;
	unsigned char *m_AuthCode; 
	unsigned short m_QuestionCodeLen;
	unsigned char *m_QuestionCode;
	std::string m_Message;
};

class EvaAddFriendAuthExReplyPacket : public InPacket
{
public:
	EvaAddFriendAuthExReplyPacket() {}
	EvaAddFriendAuthExReplyPacket(unsigned char* buf, int len);
	EvaAddFriendAuthExReplyPacket(const EvaAddFriendAuthExReplyPacket &rhs);
	
	virtual ~EvaAddFriendAuthExReplyPacket() {};

	virtual InPacket* copy(){return new EvaAddFriendAuthExReplyPacket(*this);}
	EvaAddFriendAuthExReplyPacket &operator=(const EvaAddFriendAuthExReplyPacket &rhs);
	
	const unsigned char getReplyCode() const { return m_ReplyCode; }
	void setReplyCode( const unsigned char replyCode ) { m_ReplyCode = replyCode; }
	
	const unsigned char getAuthStatus() const { return m_AuthStatus; }
	void setAuthStatus( const unsigned char authStatus ) { m_AuthStatus = authStatus; }
		
	const unsigned int getAddQQ() const { return m_AddQQNum; }
	void setAddQQ( const unsigned int qq ) { m_AddQQNum = qq; }
	
	const bool isAddSuccessful() const { return m_ReplyCode == 0x00; }
	
protected:
	virtual void parseBody();
private:
	unsigned char m_AuthStatus;
	unsigned char m_ReplyCode;
	unsigned int m_AddQQNum;
	
};

class EvaAddFriendGetAuthInfoPacket : public OutPacket
{
public:
	EvaAddFriendGetAuthInfoPacket();
	EvaAddFriendGetAuthInfoPacket(const unsigned int id, const unsigned char cmd = AUTH_INFO_CMD_INFO, const bool isQun = false);
	EvaAddFriendGetAuthInfoPacket(const EvaAddFriendGetAuthInfoPacket &rhs);
	virtual ~EvaAddFriendGetAuthInfoPacket();
	
	OutPacket *copy(){return new EvaAddFriendGetAuthInfoPacket(*this);}
	EvaAddFriendGetAuthInfoPacket &operator=( const EvaAddFriendGetAuthInfoPacket &rhs );
	
	void setAddID(const unsigned int id ) { m_AddID = id; }
	const unsigned int getAddID() const { return m_AddID; }
	
	void setVerificationStr(  std::string const &str) { m_Verification = str; }
	const std::string getVerificationStr() const { return m_Verification; }

	void setSessionStr( std::string const &session) { m_Session = session; }
	const std::string & getSessionStr() const { return m_Session; }

	void setSubCommand(const unsigned char cmd) { m_Cmd = cmd; }
	const unsigned char getSubCommand() const { return m_Cmd; }

	const bool isQun() const { return m_IsQun; }
protected:
	virtual int putBody(unsigned char* buf);
private:
	 bool m_IsQun;
	unsigned int m_AddID;
	unsigned char m_Cmd;
	// used for AUTH_INFO_CMD_CODE, the contents of the verification images
	std::string m_Verification;
	// used for AUTH_INFO_CMD_CODE, the returned session id from server when asking for verification images
	std::string m_Session;
};

class EvaAddFriendGetAuthInfoReplyPacket : public InPacket
{
public:
	EvaAddFriendGetAuthInfoReplyPacket();
	EvaAddFriendGetAuthInfoReplyPacket(unsigned char* buf, int len);
	EvaAddFriendGetAuthInfoReplyPacket(const EvaAddFriendGetAuthInfoReplyPacket &rhs);
	
	virtual ~EvaAddFriendGetAuthInfoReplyPacket();

	virtual InPacket* copy(){return new EvaAddFriendGetAuthInfoReplyPacket(*this);}
	EvaAddFriendGetAuthInfoReplyPacket &operator=(const EvaAddFriendGetAuthInfoReplyPacket &rhs);
	
	const unsigned char getReplyCode() const { return m_ReplyCode; }
	void setReplyCode( const unsigned char replyCode ) { m_ReplyCode = replyCode; }	

	const unsigned char getSubCommand() const { return m_Cmd; }
	const unsigned char *getCode() const { return m_Code; }
	const int getCodeLength () const { return m_CodeLen; }

	const bool isQun() const { return m_IsQun; }
protected:
	virtual void parseBody();
private:
	bool m_IsQun;
	// one of AUTH_INFO_CMD_INFO and AUTH_INFO_CMD_CODE
	unsigned char m_Cmd;

	// if cmd is AUTH_INFO_CMD_INFO,  reply could be one of AUTH_INFO_TYPE_CODE and AUTH_INFO_TYPE_GRAPHIC
	// if cmd is AUTH_INFO_CMD_CODE, reply could be 0x00(success) or 0x01(failed)
	unsigned char m_ReplyCode;

	int m_CodeLen;
	/**
	 *  if m_ReplyCode is AUTH_INFO_TYPE_CODE, m_Code ganna be the code
	 *  if m_ReplyCode is AUTH_INFO_TYPE_GRAPHIC, m_Code will be the url
	 *  to get the graphic
	**/
	unsigned char *m_Code;
	
};


class EvaAddFriendAuthQuestionPacket : public OutPacket
{
public:
	EvaAddFriendAuthQuestionPacket();
	EvaAddFriendAuthQuestionPacket(const unsigned int id, const unsigned char auth = AUTH_TYPE_QUESTION_REQUEST);
	EvaAddFriendAuthQuestionPacket(const EvaAddFriendAuthQuestionPacket &rhs);
	virtual ~EvaAddFriendAuthQuestionPacket();
	
	OutPacket *copy(){return new EvaAddFriendAuthQuestionPacket(*this);}
	EvaAddFriendAuthQuestionPacket &operator=( const EvaAddFriendAuthQuestionPacket &rhs );
	
	void setAddQQ(const unsigned int id ) { m_AddQQNum = id; }
	const unsigned int getAddQQ() const { return m_AddQQNum; }

	void setAnswer( std::string const &msg) { m_Answer = msg; }
	const std::string & getAnswer() const { return m_Answer; }
	void setQuestion( std::string const &msg) { m_Question = msg; }
	const std::string & getQuestion() const { return m_Question; }

	const unsigned char getAuthStatus() const { return m_AuthStatus; }
	void setAuthStatus(const unsigned char authStatus ) { m_AuthStatus = authStatus; }
protected:
	virtual int putBody(unsigned char* buf);
private:
	unsigned int m_AddQQNum;
	unsigned char m_AuthStatus; // 0x01: get my quest & answer, 0x02: set question, 0x03: request buddy's question, 0x04: verify the answer

	std::string m_Question;
	std::string m_Answer;
};

class EvaAddFriendAuthQuestionReplyPacket : public InPacket
{
public:
	EvaAddFriendAuthQuestionReplyPacket();
	EvaAddFriendAuthQuestionReplyPacket(unsigned char* buf, int len);
	EvaAddFriendAuthQuestionReplyPacket(const EvaAddFriendAuthQuestionReplyPacket &rhs);
	
	virtual ~EvaAddFriendAuthQuestionReplyPacket();

	virtual InPacket* copy(){return new EvaAddFriendAuthQuestionReplyPacket(*this);}
	EvaAddFriendAuthQuestionReplyPacket &operator=(const EvaAddFriendAuthQuestionReplyPacket &rhs);
	
	const unsigned char getReplyCode() const { return m_ReplyCode; }
	void setReplyCode( const unsigned char replyCode ) { m_ReplyCode = replyCode; }
	bool isAnswerOK() const { return m_ReplyCode == 0x00; }

	const unsigned char getAuthStatus() const { return m_AuthStatus; }
	void setAuthStatus(const unsigned char authStatus ) { m_AuthStatus = authStatus; }

	const unsigned char *getCode() const { return m_Code; }
	const unsigned char *getQuestion() const { return m_Code; }
	const int getCodeLength () const { return m_CodeLen; }

	void setAnswer( std::string const &msg) { m_Answer = msg; }
	const std::string & getAnswer() const { return m_Answer; }	
protected:
	virtual void parseBody();

private:
	unsigned char m_AuthStatus; // 0x01: get my quest & answer, 0x02: set question, 0x03: request buddy's question, 0x04: verify the answer

	// 0x00 for success, 0x01 error
	unsigned char m_ReplyCode;

	int m_CodeLen;
	unsigned char *m_Code; // could be question or code

	std::string m_Answer;
};

#endif
