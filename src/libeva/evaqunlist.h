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
#ifndef LIBEVAQUNLIST_H
#define LIBEVAQUNLIST_H
#include "evaqun.h"
#include "evafriend.h"
#include <list>
#include <map>

/*
         this file is for holding all Qun information for a user
*/

// TempQun is only for temporary Qun

class TmpQun{
public:
	TmpQun();  // this constructor is for create TmpQun only
	TmpQun(const unsigned int pid);
	
	TmpQun &operator=(const TmpQun &rhs);
	
	void setQunID(const unsigned int id) { qunID = id; }
	void setParentID(const unsigned int id) { parentID = id; }
	void setName(const std::string &n) { name = n; }
	// set all members at one go
	void setMemberList(const std::list<unsigned int> &list) { memberList = list; }
	
	void addMember(const unsigned int qqNum); // add one more member
	const bool removeMember(const unsigned int qqNum);
	
	const unsigned int getQunID() const { return qunID; }
	const unsigned int getParentID() const { return parentID; }
	const std::string &getName() const { return name; }
	const std::list<unsigned int> &getMemberQQList() const { return memberList; }
private:
	unsigned int qunID;
	unsigned int parentID;
	std::string name;
	std::list<unsigned int> memberList; // all member details are in its parent
};

// Qun is only for permenant Qun
class Qun{
public:
	/*
	* constructor for Qun
	*@param id   internal ID of a Qun
	*/
	Qun(const unsigned int id);
	Qun(const Qun &rhs);
	enum MessageType{Notify, Popup, Numbers, RecordOnly, Reject};
	Qun &operator=(const Qun &rhs);
	const unsigned int getQunID() const { return qunID; }
	const unsigned int getExtQunID() const { return info.getExtID(); }
	const QunInfo &getDetails() const { return info; }
	const std::list<FriendItem> &getMembers() const { return memberList; }
	const FriendItem *getMemberDetails(const unsigned int qqNum);
	FriendItem *memberDetails(const unsigned int qqNum);
	
	void setDetails(const QunInfo &info) { this->info = info; }
	
	void setMember(const FriendItem &fi);
	void setMembers(const std::list<FriendItem> &list);

	void updateOnlineMembers(const std::list<unsigned int> &list);
	
	//void setQQList(const std::list<unsigned int> &list) { numberList = list; }
	//const std::list<unsigned int> getQQList() const { return numberList; }
	
	void setMemberArgs(const std::map<unsigned int, QunMember> &list);
	const std::map<unsigned int, QunMember> &getMemberArgs() const { return memberArgs; }
	const bool isAdmin( const unsigned int id);
	
	const int numMembers() const { return memberArgs.size(); }
	
	void setCardName(const std::string n) { cardName = n; }
	void setCardGender(const unsigned char g ) { cardGender = g; }
	void setCardPhone(const std::string p) { cardPhone = p; }
	void setCardEmail(const std::string e) { cardEmail = e; }
	void setCardMemo(const std::string m) { cardMemo = m; }
	
	const std::string &getCardName() const { return cardName; }
	const unsigned char &getCardGender() const { return cardGender; }
	const std::string &getCardPhone() const { return cardPhone; }
	const std::string &getCardEmail() const { return cardEmail; }
	const std::string &getCardMemo() const { return cardMemo; }
	
	void setMessageType(const MessageType type) { msgType = type; }
	const MessageType getMessageType() const { return msgType; }
	
	const std::list<TmpQun> &getTmpQunList() const { return tmpQunList; }
	void addTmpQun(const TmpQun &q);
	
	const bool hasTmpQun() const { return tmpQunList.size() != 0; }
	
	const bool hasMember(const unsigned int id);

	void setRealNamesVersion(const unsigned int v) { m_RealNamesVersion = v; }
	const unsigned int getRealNamesVersion() const { return m_RealNamesVersion; }

	const int getChatFontSize() const { return m_ChatFontSize; }
	const int getChatFontColor() const { return m_ChatFontColor; }
	const int getChatFontRed()     const { return (int)(( m_ChatFontColor>>16) & 0xff); }
	const int getChatFontGreen() const { return (int)(( m_ChatFontColor>>8) & 0xff); }
	const int getChatFontBlue()   const { return (int)(  m_ChatFontColor & 0xff); }

	void setChatFontSize( const int size) { m_ChatFontSize = size; }
	void setChatFontColor(  const int color) { m_ChatFontColor = color; }
	void setChatFontColor(const int red, const int green, const int blue) {
			m_ChatFontColor = (( 0xff << 24) | ( ( red & 0xff) << 16) | ( (green & 0xff) << 8) | ( blue & 0xff) ); 
	}
	void resetMemberIterator() { m_memberStartCount = 0; }
	std::list<unsigned int> getNextMembers();
	const bool isNextMembersEnd() { return (m_memberStartCount == memberList.size()); }
	
	const bool isFirstRefresh () const { return m_bFirstRefresh;}
	void firstRefreshDone() { m_bFirstRefresh = false; }
private:
	unsigned int qunID;
	QunInfo info;
	//std::list<unsigned int> numberList;
	std::map<unsigned int, QunMember> memberArgs;
	std::list<FriendItem> memberList;
	std::list<TmpQun> tmpQunList;
	
	unsigned int m_memberStartCount;                   // used for getting member list
	// my Qun card information
	std::string cardName;
	unsigned char cardGender;
	std::string cardPhone;
	std::string cardEmail;
	std::string cardMemo;

	unsigned int m_RealNamesVersion;
	
	MessageType msgType;

	int m_ChatFontSize;
	int m_ChatFontColor;
	
	bool m_bFirstRefresh; // as every time user open a Qun chat window, only the first time will
	                      // retreive Qun members. this one is the flag for that purpose, nothing about protocol
};

class QunList {
public:
	QunList() {};
	
	QunList &operator=(const QunList &rhs);
	void add(const Qun &q);
	bool remove(const unsigned int id);
	void update(const Qun &q);
	Qun *getQun(const unsigned int id);
	Qun *getQunByExtID(const unsigned int ext);	
	const unsigned int getQunExtID(const unsigned int id);
	
	void setDetails(const QunInfo &info);
	void setMember(const unsigned int id, const FriendItem &fi);
	void setMembers(const unsigned int id, const std::list<FriendItem> &list);
	
	void setOnlineMembers(const unsigned int id, const std::list<unsigned int> &list);
	
	//void setQQList(const unsigned int id, const std::list<unsigned int> &list);
	void setMemberArgs(const unsigned int id, const std::map<unsigned int, QunMember> &member);
	
	const std::list<Qun> &getQunList() const { return qunList; }
	void clearQunList() { qunList.clear(); }
	
	void setMyQunCardInfo(const unsigned int id, const std::string name, const unsigned char gender, const std::string phone,
				const std::string email, const std::string memo);
	const int numQuns() const { return qunList.size(); }


	Qun *first();
	Qun *next();
	
private:
	std::list<Qun> qunList;
	std::list<Qun>::iterator m_iter;
};

#endif
