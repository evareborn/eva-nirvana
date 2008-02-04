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
#ifndef LIBEVAQUN_H
#define LIBEVAQUN_H

#include "evadefines.h" 
#include "evapacket.h"
#include "evafriend.h"
#include <string>
#include <list>
#include <map>

/*
     this file does all qun operations
*/

class QunInfo {
public:
	QunInfo();
	QunInfo(const QunInfo &rhs);
	~QunInfo();
	void clearInfo();
	
	const unsigned int getQunID() const { return qunID; }
	const unsigned int getExtID() const { return externalID; }
	const unsigned char getType() const { return type;}
	const unsigned int getCreator() const { return creator; }
	const unsigned char getAuthType() const { return authType; }
	const short getUnknown1() const { return unknown1; }
	const unsigned int getCategory() const { return category; }
	const unsigned int getVersionID() const { return versionID; }
	const std::string getName() const { return name; }
	const short getUnknown2() const { return unknown2; }
	const std::string getDescription() const { return description; }
	const std::string getNotice() const { return notice; }
	
	void setQunID(const unsigned int id) { qunID = id; }
	void setExtID(const unsigned int id) { externalID = id; }
	void setType(const unsigned char t) { type = t;}
	void setCreator(const unsigned int qq) { creator = qq; }
	void setAuthType(const unsigned char type) { authType = type; }
	void setUnknown1(const short unknown) { unknown1 = unknown; }
	void setCategory(const unsigned  int cate) { category = cate; }
	void setVersionID(const unsigned int vID) { versionID = vID; }
	void setName(const std::string &n) { name = n; }
	void setUnknown2(const short unknown) { unknown2 = unknown; }
	void setDescription(const std::string &descrip) { description = descrip; }
	void setNotice(const std::string &n) { notice = n; }	
	
	QunInfo &operator=(const QunInfo &rhs);
	const bool operator==(const QunInfo &rhs);
	
	int readTempQunInfo(unsigned char *buf);
	int readQunInfo(unsigned char *buf);
	int readQunInfoFromSearchReply(unsigned char *buf);

	// new stuff in 2006 (Qun search reply)
	const unsigned int getUnknownTokenLength() const { return m_UnknownTokenLength; }
	const unsigned char *getUnknownToken() const { return m_UnknownToken; }
private:	
	// internal Qun ID used in QQ protocol
	unsigned int qunID;
	// if is permanent Qun, external ID, if temporary Qun, parent's Qun ID
	unsigned int externalID;
	// type of permanent or temporary Qun
	unsigned char type;
	unsigned int creator;
	unsigned char authType;
	short unknown1;
	unsigned int category;
	unsigned int versionID;
	std::string name;
	short unknown2;
	std::string description;
	std::string notice;

	// used in Qun search reply
	unsigned int m_UnknownTokenLength;
	unsigned char *m_UnknownToken;
};

class QunPacket : public OutPacket {
public:
	QunPacket() {};
	QunPacket(const char qunCmd);
	QunPacket(const char qunCmd, const unsigned int id);  
	QunPacket(const QunPacket &rhs);
	virtual ~QunPacket();

	OutPacket * copy() { return new QunPacket(*this);}
	QunPacket &operator=(const QunPacket &rhs);
	
	const char getQunCommand() const { return qunCommand; }
	const unsigned int getQunID() const { return qunID; }
	
	void setQunCommand(const char cmd) { qunCommand = cmd; }
	void setQunID(const unsigned int id) { qunID = id; }
	
protected:
	char qunCommand;
	unsigned int qunID;
};

typedef struct QunMember{
unsigned int qqNum;
unsigned char groupIndex;
unsigned char admin;// note: 1 means admin, 6 means shareholder, value 4 is unknown
} QunMember;

class QunReplyPacket : public InPacket {
public:
	QunReplyPacket() {}
	QunReplyPacket(unsigned char *buf, int len) ;  
	QunReplyPacket(const QunReplyPacket &rhs);
	virtual ~QunReplyPacket();
	
	InPacket * copy() { return new QunReplyPacket(*this);}
	QunReplyPacket &operator=(const QunReplyPacket &rhs);
	
	const char getQunCommand() const { return qunCommand; }
	const char getReplyCode() const { return replyCode; }
	const unsigned int getQunID() const { return qunID; }
	const unsigned int getExtID() const {  return externalID; }
	const char getType() const { return type; }
	const unsigned int getParentQunID() const { return parentQunID; }
	const std::string getErrorMessage() const { return errorMessage; }
	
	const unsigned char getSearchType() const { return searchType; }
	const QunInfo &getQunInfo() const { return info; }
	const std::map<unsigned int, QunMember> &getMemberList() const { return memberList; }
	const std::list<QunInfo> &getQunInfoList() const { return qunInfoList; }
	const std::list<unsigned int> &getQQNumberList() const { return qunQQNumberList; }
	const std::list<FriendItem> &getMemberInfoList() const { return memberInfoList; }
	const unsigned char getJoinReply() const { return joinReply; }
	
	const bool isReplyOK() const;
	
	const unsigned int getTargetQQ() const { return targetQQ; }
	const unsigned char getOpCode() const { return opCode;}
	const unsigned int getQunVersionID() const { return versionID; }
	
	const std::string &getRealName() const { return realName; }
	const unsigned char getGender() const { return gender; }
	const std::string &getPhone() const { return phone; }
	const std::string &getEmail() const { return email; }
	const std::string &getMemo() const { return memo; }

	const unsigned int getCardVersion() const { return m_CardVersion; }
	const unsigned int getNextStartIndex() const { return m_NextStart; }
	const std::map<int, std::string> &getRealNames() const { return m_RealNameList; }
protected:
	virtual void parseBody();
	
private: 
	/** sub-command of Qun */
	char qunCommand;
	// reply code, determine if request ok or not
	char replyCode;
	/** internal ID of a Qun */
	unsigned int qunID;
	/** external ID of a Qun */
	unsigned int externalID;
	/** type of a Qun  */
	char type;
	/** internal ID of parent Qun, used for temporary Qun */
	unsigned int parentQunID;
	
	/** error messages if replyCode is QQ_QUN_CMD_REPLY_OK */
	std::string errorMessage;
	
	unsigned char searchType;
	QunInfo info;
	/// Qun information for 
	/// 	getting Qun information request
	/// 	searching Qun request 
	std::list<QunInfo> qunInfoList;
	
	/// memebers of a Qun, only used for 
	///	getting tmp Qun member list request
	///	getting member online status of Qun request
	std::list<unsigned int> qunQQNumberList;
	
	/// used for getting Qun information request only
	std::map<unsigned int,QunMember> memberList;
	
	///public List memberInfos;
	std::list<FriendItem> memberInfoList;
	/// join Qun request use only
	unsigned char joinReply;
	
	unsigned int targetQQ; // for modify Qun card , requesting Qun card,  set/unset Admin, transfer
	unsigned char opCode; // set/unset Admin
	unsigned int versionID; // set/unset Admin, transfer

	/// used for Get All Realname operation
	unsigned int m_NextStart;  // next start index
	unsigned int m_CardVersion;
	std::map<int, std::string>m_RealNameList;
	
	/// the following 5 varaibles are for the result of requesting Qun card, and still the qunID, targetQQ are used as well
	std::string realName;
	unsigned char gender;
	std::string phone;
	std::string email;
	std::string memo;
	
	
	int parseSendIMReply(unsigned char *buf, int len);
	int parseSendTempQunIMReply(unsigned char *buf, int len);
	int parseCreateReply(unsigned char *buf, int len);
	int parseCreateTempQun(unsigned char *buf, int len);
	int parseActivateReply(unsigned char *buf, int len);
	int parseModifyMemberReply(unsigned char *buf, int len);
	int parseGetInfoReply(unsigned char *buf, int len);
	int parseExitReply(unsigned char *buf, int len);
	int parseGetMemberInfoReply(unsigned char *buf, int len);
	int parseGetOnlineMemberReply(unsigned char *buf, int len);
	int parseJoinReply(unsigned char *buf, int len);
	int parseJoinAuthReply(unsigned char *buf, int len);
	int parseModifyInfoReply(unsigned char *buf, int len);
	int parseSearchReply(unsigned char *buf, int len);
	int parseGetTempQunInfoReply(unsigned char *buf, int len);
	int parseExitTempQunReply(unsigned char *buf, int len);
	int parseGetTempQunMembers(unsigned char *buf, int len);
	int parseModifyQunCard(unsigned char *buf, int len);
	int parseRequestAllRealNames(unsigned char *buf, int len);
	int parseRequestQunCard(unsigned char *buf, int len);
	int parseQunAdminOperation(unsigned char *buf, int len);
	int parseQunTransfer(unsigned char *buf, int len);
};

class QunGetInfoPacket : public QunPacket {
public:
	QunGetInfoPacket() {}
	QunGetInfoPacket(const int id);  
	QunGetInfoPacket(const QunGetInfoPacket &rhs);
	virtual ~QunGetInfoPacket();
protected:
	virtual int putBody(unsigned char *buf);
};

class QunGetMemberInfoPacket : public QunPacket {
public:
	QunGetMemberInfoPacket() {}
	QunGetMemberInfoPacket(const int id);
	QunGetMemberInfoPacket(const QunGetMemberInfoPacket &rhs);
	virtual ~QunGetMemberInfoPacket();
	
	QunGetMemberInfoPacket &operator=( const QunGetMemberInfoPacket &rhs);
	
	void addMember(const unsigned int id);
	
	const std::list<unsigned int> &getMemberList() const { return memberList; }
	void setMemberList(const std::list<unsigned int> list) { memberList = list; }
protected:
	virtual int putBody(unsigned char *buf);
	
private:
	std::list<unsigned int> memberList;
};

class QunGetOnlineMemberPacket : public QunPacket {
public:
	QunGetOnlineMemberPacket() {}
	QunGetOnlineMemberPacket(const unsigned int id);  
	QunGetOnlineMemberPacket(const QunGetOnlineMemberPacket &rhs);
	virtual ~QunGetOnlineMemberPacket();
protected:
	virtual int putBody(unsigned char *buf);
};

class QunActivatePacket : public QunPacket {
public:
	QunActivatePacket() {}
	QunActivatePacket(const unsigned int id);  
	QunActivatePacket(const QunActivatePacket &rhs);
	virtual ~QunActivatePacket();
protected:
	virtual int putBody(unsigned char *buf);
};

class QunAuthPacket : public QunPacket {
public:
	QunAuthPacket();
	QunAuthPacket(const unsigned int id, const unsigned char authType);
	QunAuthPacket(const QunAuthPacket &rhs);
	virtual ~QunAuthPacket();
	
	const unsigned char getAuthType() const { return type; }
	const unsigned int getReceiver() const { return receiver; }
	const std::string getMessage() const { return message; }
	
	void setAuthType(const unsigned char t) { type = t; }
	void setReceiver(const unsigned int r) { receiver = r; }
	void setMessage(const std::string &msg) { message = msg; }

	const unsigned char *getCode() const { return m_Code; }
	const unsigned short getCodeLength() const { return m_CodeLen; }

	void setCode(const unsigned char *code, const unsigned short len);
	void setToken(const unsigned char *code, const unsigned short len);
	
	QunAuthPacket &operator=(const QunAuthPacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned char type;
	unsigned int receiver;
	unsigned char *m_Code;
	unsigned short m_CodeLen;
	std::string message;
};

class QunSendIMPacket : public QunPacket {
public:
	QunSendIMPacket() {}
	QunSendIMPacket(const unsigned int id);  
	QunSendIMPacket(const QunSendIMPacket &rhs);
	virtual ~QunSendIMPacket();
	OutPacket * copy() { return new QunSendIMPacket(*this);}
	QunSendIMPacket &operator=(const QunSendIMPacket &rhs);
	
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
protected:
	virtual int putBody(unsigned char *buf);
	
	short encoding;
	std::string fontName;
	char red, green, blue;
	bool bold, italic, underline;
	unsigned char fontSize;
	unsigned char fontFlag; 
	   
	std::string message; 
};

class QunSendIMExPacket : public QunSendIMPacket{
public: 
	QunSendIMExPacket() {}
	QunSendIMExPacket(const unsigned int id);  
	QunSendIMExPacket(const QunSendIMExPacket &rhs);
	virtual ~QunSendIMExPacket() {};
	OutPacket * copy() { return new QunSendIMExPacket(*this);}
	QunSendIMExPacket &operator=(const QunSendIMExPacket &rhs);
	
	const unsigned char getNumFragments() const { return numFragments; }
	const unsigned char getSeqOfFragments() const { return seqFragments; }
	const short getMessageID() const { return messageID; }
	
	void setNumFragments(const unsigned char num) { numFragments = num; }
	void setSeqOfFragments( const unsigned char seq) { seqFragments = seq; }
	void setMessageID(const short id) { messageID = id; }

protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned char numFragments;
	unsigned char seqFragments;
	static short messageID;
};

class QunSearchPacket : public QunPacket {
public:
	QunSearchPacket();
	QunSearchPacket(int type);  
	QunSearchPacket(const QunSearchPacket &rhs);
	virtual ~QunSearchPacket() {};
	
	const unsigned char getSearchType() const { return type; }
	const unsigned int getExtID() const { return externalID; }
	
	void setSearchType(const unsigned char t) { type = t; }
	void setExtID(const int id) { externalID = id; }
	
	QunSearchPacket &operator=(const QunSearchPacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned char type;
	unsigned int externalID;
};


class QunJoinPacket : public QunPacket {
public:
	QunJoinPacket() {}
	QunJoinPacket(const unsigned int id);  
	QunJoinPacket(const QunJoinPacket &rhs);
	virtual ~QunJoinPacket();
protected:
	virtual int putBody(unsigned char *buf);
};

class QunModifyPacket : public QunPacket {
public:
	QunModifyPacket() {}
	QunModifyPacket(const QunInfo &inf);  
	QunModifyPacket(const QunModifyPacket &rhs);
	virtual ~QunModifyPacket(){};
	
	const QunInfo &getQunInfo() const { return info; }
	
	QunModifyPacket &operator=(const QunModifyPacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);
private:
	QunInfo info;
};

class QunRequestCardPacket :  public QunPacket {
public:
	QunRequestCardPacket() {}
	QunRequestCardPacket(const unsigned int qunID, const unsigned int qqID);  
	QunRequestCardPacket(const QunRequestCardPacket &rhs);
	virtual ~QunRequestCardPacket(){};
	
	const unsigned int getQQ() const { return qqNum; }
	QunRequestCardPacket &operator=(const QunRequestCardPacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned int qqNum;
};

class QunModifyCardPacket : public QunPacket {
public:
	QunModifyCardPacket() { m_AllowAdminChange = 0x02;/* admin cannot change my Qun card*/ }
	QunModifyCardPacket(const unsigned int qunID, const unsigned int qqID);  
	QunModifyCardPacket(const QunModifyCardPacket &rhs);
	virtual ~QunModifyCardPacket(){};
	
	const unsigned int getQQ() const { return qqNum; }
	const unsigned char getAllowAdminChange() const { return m_AllowAdminChange;}
	const std::string &getName() const { return name;}
	const unsigned char getGender() const { return gender; }
	const std::string &getPhone() const { return phone;}
	const std::string &getEmail() const { return email;}
	const std::string &getMemo() const { return memo;}
	
	void setQQ(const unsigned int qqID) { qqNum = qqID; }
	void setAllowAdminChange( const unsigned char allow) { m_AllowAdminChange = allow; }
	void setName(const std::string &n) { name = n; }
	void setGender(const unsigned char g) { gender = g; }
	void setPhone(const std::string &ph) { phone = ph; }
	void setEmail(const std::string &e) { email = e; }
	void setMemo(const std::string &m) { memo = m; }
	
	
	QunModifyCardPacket &operator=(const QunModifyCardPacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned int qqNum;
	unsigned char m_AllowAdminChange;// 0x00: allowed, 0x02: not allowed
	std::string name;
	unsigned char gender;
	std::string phone;
	std::string email;
	std::string memo;
};

class QunAdminOpPacket : public QunPacket {
public:
	QunAdminOpPacket() {}
	QunAdminOpPacket(const int id, const int qqID, const bool isSetAdmin);  
	QunAdminOpPacket(const QunAdminOpPacket &rhs);
	virtual ~QunAdminOpPacket() {};
	
	const unsigned int getQQ() const { return qqNum; }
	const unsigned char getActionCode() const { return action; }
	
	void setQQ(const unsigned int qq) { qqNum = qq; }
	void setActionCode( const unsigned char code) { action = code; }
	
	QunAdminOpPacket &operator=(const QunAdminOpPacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned int qqNum;
	unsigned char action;
};

class QunTransferPacket : public QunPacket {
public:
	QunTransferPacket() {}
	QunTransferPacket(const unsigned int id, const unsigned int qqID);  
	QunTransferPacket(const QunTransferPacket &rhs);
	virtual ~QunTransferPacket() {};
	
	const unsigned int getQQ() const { return qqNum; }
	void setQQ(const unsigned int qq) { qqNum = qq; }
	
	QunTransferPacket &operator=(const QunTransferPacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned int qqNum;
};


class QunModifyMemberPacket : public QunPacket {
public:
	QunModifyMemberPacket() {}
	QunModifyMemberPacket(const unsigned int id, const bool isAdd);  
	QunModifyMemberPacket(const QunModifyMemberPacket &rhs);
	virtual ~QunModifyMemberPacket() {};
	
	const std::list<unsigned int> &getMembers() const { return members; }
	const bool isAdd() const { return mIsAdd; }
	
	void setMembers(const std::list<unsigned int> list) { members=list; }
	void setAdd( const bool isAdd) { mIsAdd = isAdd; }
	
	QunModifyMemberPacket &operator=(const QunModifyMemberPacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);
private:
	std::list<unsigned int> members;
	bool mIsAdd;
};

class QunExitPacket : public QunPacket {
public:
	QunExitPacket() {}
	QunExitPacket(const unsigned int id);  
	QunExitPacket(const QunExitPacket &rhs);
	virtual ~QunExitPacket() {};
	
	QunExitPacket &operator=(const QunExitPacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);
};

class QunCreatePacket : public QunPacket {
public:
	QunCreatePacket() {}
	QunCreatePacket(const std::string &name, const unsigned short category, 
			const unsigned char auth = QQ_QUN_JOIN_NEED_AUTH);
	QunCreatePacket(const QunCreatePacket &rhs);
	
	void setNotice(const std::string &notice) { qunNotice = notice; }
	void setDescription(const std::string &desc) { qunDescription = desc; }
	void setMembers( const std::list<unsigned int> &members) { qunMembers = members; }
	
	const std::string & getName() const { return qunName; }
	const unsigned short getCategory() const { return qunCategory; }
	const unsigned char getAuth() const { return qunAuth; }
	
	const std::string & getNotice() const { return qunNotice; }
	const std::string & getDescription() const { return qunDescription; }
	const std::list<unsigned int> & getMembers() const { return qunMembers; }
	
	QunCreatePacket &operator=(const QunCreatePacket &rhs);
	
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned char qunAuth;
	unsigned short qunCategory;
	std::string qunName;
	std::string qunNotice;
	std::string qunDescription;
	std::list<unsigned int> qunMembers;
};

class QunActivePacket : public QunPacket {
public:
	QunActivePacket() {}
	QunActivePacket(const unsigned int id);  
	QunActivePacket(const QunActivePacket &rhs);
	virtual ~QunActivePacket() {};
	
	QunActivePacket &operator=(const QunActivePacket &rhs);
protected:
	virtual int putBody(unsigned char *buf);	
};

class QunRequestAllRealNames : public QunPacket {
public:
	QunRequestAllRealNames() {}
	QunRequestAllRealNames(const unsigned int id); 
	QunRequestAllRealNames(const QunRequestAllRealNames &rhs);
	virtual ~QunRequestAllRealNames() {};

	void setStartIndex(const unsigned int index) { m_StartIndex = index; }
	const unsigned int getStartIndex() const { return m_StartIndex; }

	QunRequestAllRealNames &operator=(const QunRequestAllRealNames &rhs);
protected:
	virtual int putBody(unsigned char *buf);
private:
	unsigned int m_StartIndex;
};

#endif
