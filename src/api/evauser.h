/***************************************************************************
 *   Copyright (C) 2004 by yunfan                                          *
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

#ifndef EVAUSER_H
#define EVAUSER_H 

#include "evauserinfo.h"
#include "evaqunlist.h"
#include "evadcopcontacts.h"
#include "evausersetting.h"
#include <qobject.h>
#include <qstringlist.h>
#include <string>
#include <list>

// used in the reply of login token ex requests
class GraphicVerifyCode {
public:
	GraphicVerifyCode() :
		m_SessionTokenLen ( 0),
		m_SessionToken (NULL),
		m_DataLen  (0),
		m_Data (NULL)
	{
	};
	GraphicVerifyCode(const GraphicVerifyCode &rhs)  :
		m_SessionTokenLen ( 0),
		m_SessionToken (NULL),
		m_DataLen  (0),
		m_Data (NULL)
	{
		*this = rhs; 
	}
	~GraphicVerifyCode(){
		if(m_SessionToken) delete [] m_SessionToken;
		if(m_Data) delete [] m_Data;
	}
	GraphicVerifyCode &operator=(const GraphicVerifyCode &rhs){
		setSessionToken(rhs.m_SessionToken, rhs.m_SessionTokenLen);
		setData(rhs.m_Data, rhs.m_DataLen);
		return *this;
	}
	void setSessionToken(const unsigned char *token, const unsigned short len){
		if(m_SessionToken) delete []m_SessionToken;
		m_SessionToken = new unsigned char [len];
		memcpy(m_SessionToken, token, len);
		m_SessionTokenLen = len;
	};

	void setData(const unsigned char *data, const unsigned short len){
		if(m_Data) delete []m_Data;
		m_Data = new unsigned char [len];
		memcpy(m_Data, data, len);
		m_DataLen = len;
	};

	unsigned short m_SessionTokenLen;
	unsigned char *m_SessionToken;
	unsigned short m_DataLen;
	unsigned char *m_Data;
};

class EvaUserSetting;
class CustomEvent;
class KConfig;

class EvaUser : public QObject, virtual public EvaDCOPContactsInterface
{
	Q_OBJECT
public:
	EvaUser(const unsigned int id, const std::string &password);
	EvaUser(const unsigned int id, const char *md5Password);
	virtual ~EvaUser();
	
	enum UserStatus {Eva_Online = 10, Eva_Offline = 20, Eva_Leave = 30, Eva_Invisible = 40};
	const unsigned int getQQ() const { return qqNum; }
	const char *getMd5Password() const { return md5Password; }
	
	void setStatus(const UserStatus status) { this->status = status; }
	const UserStatus getStatus() const { return status; }
	static char getStatusCode(const UserStatus status) ;
	
	void setDetails(const ContactInfo &details) { this->details = details; }
	const ContactInfo &getDetails() const { return details; }
	
	FriendList &getFriendList() { return myFriends; }
        FriendList *getFriends() { return &myFriends; }
	void setFriendList(const FriendList &l) { myFriends.clearFriendList(); myFriends = l; }

	
	unsigned int getOnlineTime() const { return timeOnline;}     // the following 6 funs are about the level requst
	unsigned short getLevel() const { return level; }
	unsigned short getHoursToLevelUp() const { return hoursToLevelUp;}
	
	void setOnlineTime(const unsigned int time ) { timeOnline = time;}
	void setLevel(const unsigned short l ) { level = l;}
	void setHoursToLevelUp(const unsigned short time ) { hoursToLevelUp = time;}
		
	const bool newGroup(const std::string &name);
	void clearGroupNames();
	void setGroupNames(const std::list<std::string> &groups);
	void removeGroupName(const int index);
	const int getGroupIndexOf(const std::string &name);
	void updateGroupName(const std::string &newName, const int index);
	
	void setQunGroupName(const std::string &name) { qunName = name; }
	void setAnonymousGroupName(const std::string &name) { anonymousName = name; }
	void setBlackGroupName(const std::string &name) { blackName = name; }	
	
	const std::string getQunGroupName() const { return qunName; }
	const std::string getAnonymousGroupName() const { return anonymousName; }
	const std::string getBlackGroupName() const { return blackName; }
	
	static int getQunIndex() { return qunIndex; }
	static int getAnonymousIndex() { return anonymousIndex; }
	static int getBlackIndex() { return blackIndex; }
	
	std::list<std::string> &getGroupNames() { return groupNames; }
	void setGroupNames(std::list<std::string> &list);
	const std::string groupNameAtIndex(const int index);
	
	EvaUserSetting *getSetting() { return setting; }
	KConfig *config(const QString &group);
	
	const bool loadGroupedBuddyList();
	const bool saveGroupedBuddyList();
	void setBuddyLoadedEnabled( const bool v) { isBuddyListLoaded = v; }
	const bool isBuddiesLoaded() const { return isBuddyListLoaded; }

	QunList *getQunList() { return &qunList; }
	void setQunList( const QunList &l) { qunList = l; }
	const bool loadQunList();
	const bool saveQunList();
	const bool isQunLoaded() const { return isQunListLoaded; }
	
	void setExtraInfo( const unsigned long long info) { mExtraInfo = info; }
	const unsigned long long getExtraInfo() const { return mExtraInfo; }
	const bool hasSignature() { return mExtraInfo & QQ_EXTAR_INFO_SIGNATURE; }
	const bool hasQQTang() { return mExtraInfo & QQ_EXTAR_INFO_TANG; }
	const bool hasQQAlbum() { return mExtraInfo & QQ_EXTAR_INFO_ALBUM; }
	const bool hasPalEntry() { return mExtraInfo & QQ_EXTAR_INFO_PAL; }
	const bool hasUserHead() { return mExtraInfo & QQ_EXTAR_INFO_USER_HEAD; }
		
	void setSignature(const std::string sig, const unsigned int time) { mSignature = sig;  mSignatureModifyTime = time; }
	const std::string &getSignature() const { return mSignature; }
	const unsigned int getSignatureModifyTime() const { return mSignatureModifyTime; }
	
	//EvaLoginProcess *loginManager() { return mLoginManager; }

	void setLoginWanIp(const unsigned int ip) { m_LoginIp = ip; }
	void setLoginWanPort(const unsigned short port) { m_LoginPort = port; }
	void setLoginLanIp(const unsigned int ip) { m_LanIp = ip; }
	void setLoginLanPort(const unsigned short port) { m_LanPort = port; }
	void setLastLoginIp(const unsigned int ip) { m_LastLoginIp = ip; }
	void setLastLoginTime(const unsigned int time) { m_LastLoginTime = time; }

	const unsigned int getLoginWanIp() const { return m_LoginIp; }
	const unsigned short getLoginWanPort() const { return m_LoginPort; }
	const unsigned int getLoginLanIp() const { return m_LanIp; }
	const unsigned short getLoginLanPort() const { return m_LanPort; }
	const unsigned int getLastLoginIp() const { return m_LastLoginIp; }
	const unsigned int getLastLoginTime() const { return m_LastLoginTime; }

	const bool loginNeedVerify() const { return (m_CodeList.size() != 0); }
	void addLoginVerifyInfo(const GraphicVerifyCode &info);
	const GraphicVerifyCode getLoginVerifyInfo();
	GraphicVerifyCode getNextLoginVerifyInfo();
	const int getNumVerifyCodes() const { return m_CodeList.size(); }
	void clearAllVerifyCodes();

// DCOP calls
	int numFriends();
	QStringList friends();
	bool hasFriend(unsigned int id);
	QString nickOf(unsigned int id);
	QString faceOf(unsigned int id, bool isOff);
	int genderOf(unsigned int id);
	int levelOf(unsigned int id);
	QString signatureOf(unsigned int id);
	int numGroups();
	QString groupName(int index);
	int group(unsigned int id);
	int numQuns();
	QStringList Quns();
	QString QunName(unsigned int ext);
	QString QunNotice(unsigned int ext);
	QString QunDescription(unsigned int ext);
	int numQunMembers(unsigned int ext);
	QStringList QunMembers(unsigned int ext);
	QString QunMemberNick(unsigned int ext, unsigned int id);
	QString QunMemberFace(unsigned int ext, unsigned int id, bool isOff);
	unsigned int myQQ();
	int onlineStatus(unsigned int id);
	QString currentLoginIP();
	QString lastLoginIP();
	QString lastLoginTime(); // format in yyyy-MM-dd hh:mm:ss
signals:
	void loadGroupedBuddiesReady();
	void loadQunListReady();
protected:
	virtual void customEvent( QCustomEvent * e );
private:
	unsigned int qqNum;
	char *md5Password;
	UserStatus status;
	ContactInfo details;
	FriendList myFriends;
	std::list<std::string> groupNames;
	EvaUserSetting *setting;
	bool isBuddyListLoaded;
	bool isQunListLoaded;
	
	unsigned int timeOnline;
	unsigned short level;
	unsigned short hoursToLevelUp;
	
	unsigned long long mExtraInfo;
	unsigned int mSignatureModifyTime;
	std::string mSignature;
	
	QunList qunList;
	
	//EvaLoginProcess *mLoginManager;
	
	static std::string qunName;
	static std::string anonymousName;
	static std::string blackName;
	static const int qunIndex = 0xfffd;
	static const int anonymousIndex = 0xfffe;
	static const int blackIndex = 0xffff;

	unsigned int m_LoginIp;
	unsigned short m_LoginPort;
	unsigned int m_LastLoginIp;
	unsigned int m_LastLoginTime;

	unsigned int m_LanIp;
	unsigned short m_LanPort;

	std::list<GraphicVerifyCode> m_CodeList;
	
	friend class EvaUserSetting;
};

#endif


