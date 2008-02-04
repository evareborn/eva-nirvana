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
#ifndef LIBEVAFRIENDLIST_H
#define LIBEVAFRIENDLIST_H
#include <map>
#include <list>
#include "evadefines.h"
#include "evafriend.h"
#include "evaonlinestatus.h"
#include "evauserinfo.h"
#include "evalevel.h"
#include "evamemo.h"


#define BUDDY_INVALID_FIELD 01

// for eva use only. one bit for one flag
static const unsigned int NickChanged     = 1;
static const unsigned int FaceChanged     = (1 << 1);
static const unsigned int SignatureChanged= (1 << 2);
static const unsigned int StatusChanged   = (1 << 3);
static const unsigned int MemoChanged     = (1 << 4);

// class QQFriend try to encapculate all information about your buddies,
// and provides enough methods to update them from time to time
class QQFriend{
public:
	QQFriend();
	QQFriend(const unsigned int qqid, const unsigned short qqFace);
	QQFriend(const QQFriend &rhs);
	
	const unsigned int getQQ() const { return qqNum; }
	const unsigned short getFace() const { return face; }
	const unsigned char getAge() const { return age; }
	const char getGender() const { return gender; }
	const std::string &getNick() const { return nick; } 
	const char getExtFlag() const { return extFlag; }
	const char getCommonFlag() const { return commonFlag; }
	const long getLoginTime() const { return loginTime; }
	const long getIdleTime() const { return idleTime; }
	const long getLastRefreshTime() const { return lastRefreshTime; }
	
	void setAge(const unsigned char age) { this->age = age; }
	void setGender(const char gender) { this->gender = gender; }
	void setNick(const std::string nick) { this->nick = nick; m_evaUpdateFlag |= NickChanged; }
	void setExtFlag(const char flag) { extFlag = flag; }
	void setCommonFlag(const char flag) {commonFlag = flag; }
	
	const bool isMember() const { return (commonFlag & 0x2) != 0; }
	const bool isBoy() const { return gender == QQ_FRIEND_GENDER_GG;}           // aboves from FriendItem

	const unsigned int getIP() const { return IP; }
	const unsigned short getPort() const { return port;}
	const char getStatus() const { return status;}
	const char getUnknown1_4() const { return unknown4;}    // bit 4
	const char getUnknown2_11() const { return unknown11;}   // bit 11
	const short getUnknown3_13_14() const { return unknown13_14;}  // bit 13-14
	const unsigned char * getUnknownKey() const { return unknownKey;}   
	const short getUnknown4_31_32() const { return unknown31_32; }
	const char getOnlineExtFlag() const { return onlineExtFlag; }
	const char getOnlineCommFlag() const { return onlineCommonFlag; }
	const short getUnknown5_35_36() const { return unknown35_36; }      // aboves from FriendOnlineEntry
	
	
	void setFace( const unsigned short faceCode ) { face = faceCode; m_evaUpdateFlag |= FaceChanged; } // these 7 funcs update status of a 
	void setIP( const unsigned int ip ) { IP = ip; }                  // buddy whenever her/him send msg 
	void setPort( const unsigned short p) { port = p; }                 // or change online status
	void setStatus( const char s) { status = s; m_evaUpdateFlag |= StatusChanged; }
	void setUnknown1_4( const char un) { unknown4 = un; }
	void setUnknown2_11( const char un) { unknown11 = un; }
	void setUnknown3_13_14( const short un) { unknown13_14 = un; }
	void setUnknownKey( const unsigned char *key) { memcpy(unknownKey, key, 16); }
	
	unsigned int getOnlineTime() const { return timeOnline;}     // the following 6 funs are about the level requst
	unsigned short getLevel() const { return level; }
	unsigned short getHoursToLevelUp() const { return hoursToLevelUp;}
	
	void setOnlineTime(const unsigned int time );
	void setLevel(const unsigned short l ) { level = l;}
	void setHoursToLevelUp(const unsigned short time ) { hoursToLevelUp = time;}
	
	const short getVersion() const { return clientVersion; }    
	void setVersion( const short version) { clientVersion = version; } // set it once got msg from buddy
	const unsigned char *getFileSessionKey() const { return fileSessionKey; }
	void setFileSessionKey( const unsigned char *key)                // set it once got msg from buddy
		{ memcpy(fileSessionKey, key, 16); }
	
	void setFriendItem( const FriendItem &item);               // when u get friend list, you set this
	void setFriendOnlineEntry( const FriendOnlineEntry &entry);  // when u get online friend you set this
	
	const ContactInfo &getUserInformation() const { return userInfo; }
	void setUserInformation( const ContactInfo &info);   // once got user info, set it and update some info
	
	void setGroupIndex( const int index) { groupIndex = index; }
	const int getGroupIndex() const { return groupIndex; }
	
	void setExtraInfo(const unsigned int info) { mExtraInfo = info; }
	const unsigned long long getExtraInfo() const { return mExtraInfo; }
	const bool hasSignature() const { return mExtraInfo & QQ_EXTAR_INFO_SIGNATURE; }
	const bool hasQQTang() const { return mExtraInfo & QQ_EXTAR_INFO_TANG; }
	const bool hasQQAlbum() const { return mExtraInfo & QQ_EXTAR_INFO_ALBUM; }
	const bool hasPalEntry() const { return mExtraInfo & QQ_EXTAR_INFO_PAL; }
	const bool hasUserHead() const { return mExtraInfo & QQ_EXTAR_INFO_USER_HEAD; }
	
	void setSignature(const std::string sig, const unsigned int time) 
			{ mSignature = sig;  mSignatureModifyTime = time; m_evaUpdateFlag |= SignatureChanged; }
	const std::string &getSignature() const { return mSignature; }
	const unsigned int getSignatureModifyTime() const { return mSignatureModifyTime; }

	void setMemo( const MemoItem &memo ) { m_Memo = memo; m_evaUpdateFlag |= MemoChanged; }
	const MemoItem &getMemo() const { return m_Memo; }
	
	enum sortItem { Sort_QQ, Sort_Nick, Sort_Status};
	static sortItem getSortField() { return field2Sort; }
	static void setSortField(const sortItem field) { field2Sort = field; }
	QQFriend &operator=(const QQFriend &rhs);
	int operator==(const QQFriend &rhs) const ; // note: we only test qqNum
	int operator<(const QQFriend &rhs) const ;

	void setSequence(const unsigned short seq) { m_Sequence = seq; }
	const unsigned short getSequence() const { return m_Sequence; }
	const unsigned short getNextSequence() { return ++m_Sequence; }

	const int getChatFontSize() const { return m_ChatFontSize; }
	const int getChatFontColor() const { return m_ChatFontColor; }
	const int getChatFontRed()     const { return (int)(( m_ChatFontColor>>16) & 0xff); }
	const int getChatFontGreen() const { return (int)(( m_ChatFontColor>>8) & 0xff); }
	const int getChatFontBlue()   const { return (int)(  m_ChatFontColor & 0xff); }

	void setChatFontSize( const int size) { m_ChatFontSize = size; }
	void setChatFontColor(  const int color) { m_ChatFontColor = color; }
	void setChatFontColor(const int red, const int green, const int blue) {
			m_ChatFontColor = (( 0xff << 24) | ( ( red & 0xff) << 16) | ( (green & 0xff) << 8) | ( blue & 0xff) ); }

	const unsigned int getEvaUpdateFlag() const { return m_evaUpdateFlag;}
	inline void inactiveEvaUpdateFlag( const unsigned int bit);
	inline int isNickChanged();
	inline int isFaceChanged();
	inline int isSignatureChanged();
	inline int isStatusChanged();
	inline int isMemoChanged();
private:
	unsigned int qqNum;
	unsigned short face;
	unsigned char age;
	char gender;
	std::string nick;	
	char extFlag;  // bit1: if has qqshow, the rest are unknown  
	char commonFlag;  //  bit1: memeber,   bit4: TCP mode,   bit5 : mobile QQ, 
			  //  bit6: mobile binding,   bit7: webcam
	long loginTime;
	long idleTime;
	long lastRefreshTime;
		
	char unknown4;
	unsigned int IP;  // 4 bytes
	unsigned short port;
	char unknown11;
	char status;
	short unknown13_14;
	unsigned char unknownKey[QQ_KEY_LENGTH];
	short unknown31_32;
	char onlineExtFlag;
	char onlineCommonFlag;
	short unknown35_36;
	
	unsigned int timeOnline;
	unsigned short level;
	unsigned short hoursToLevelUp;
	
	short clientVersion;
	
	unsigned long long mExtraInfo;
	std::string mSignature;
	unsigned int mSignatureModifyTime;
	
	unsigned char fileSessionKey[QQ_KEY_LENGTH];
	
	ContactInfo userInfo;
	
	static sortItem field2Sort;
	
	int groupIndex;  // 0: default "My Buddies",  maximum is "Black List", the last second is "Anonymous"

	unsigned short m_Sequence;
	MemoItem m_Memo;
	
	int m_ChatFontSize;
	int m_ChatFontColor;
	
	unsigned int m_evaUpdateFlag; // used for Eva to paint the cell if anything changed. Check the change by flags

	void initalizeBuddy(const unsigned int qqid = 0, const unsigned short qqFace = 0);
};

inline void QQFriend::inactiveEvaUpdateFlag( const unsigned int bit) { m_evaUpdateFlag &= (~bit); }
inline int QQFriend::isNickChanged()      { return m_evaUpdateFlag & NickChanged; }
inline int QQFriend::isFaceChanged()      { return m_evaUpdateFlag & FaceChanged; }
inline int QQFriend::isSignatureChanged() { return m_evaUpdateFlag & SignatureChanged; }
inline int QQFriend::isStatusChanged()    { return m_evaUpdateFlag & StatusChanged; }
inline int QQFriend::isMemoChanged()      { return m_evaUpdateFlag & MemoChanged; }

class FriendList{
public:
    FriendList() {}
    ~FriendList() { privateList.clear(); }	
    const bool hasFriend(const unsigned int id);
    QQFriend *getFriend(const unsigned int id);
    bool deleteFriend(const unsigned int id);
    void addFriend(const QQFriend &frd);
    void updateFriend( const QQFriend &frd);  // if frd dosen't exists, add a new one
    void clearFriendList() { privateList.clear(); }
    
    bool updateFriendIP(const unsigned int id, const unsigned int ip);
    bool updateFriendPort(const unsigned int id, const unsigned short p);
    bool updateFriendFace(const unsigned int id, const unsigned short face);
    bool updateFriendStatus(const unsigned int id, const char status);
    bool updateFriendFileSessionKey(const unsigned int id, const unsigned char *key); 
    bool updateFriendGroupIndex(const unsigned int id, const int index);
    
    bool updateFriendLevel(const unsigned int id, const unsigned int online, const unsigned short level, const unsigned short hours);
    
    bool addFriendItemTo( const unsigned int id, const FriendItem &item);
    bool addOnlineFriendEntryTo( const unsigned int id, const FriendOnlineEntry & entry);
    bool addContactInfoTo( const unsigned int id, const ContactInfo &info);
    
    const int numberOfFriends() const { return privateList.size(); }
    const int numberOfOnlines() ;
    std::list<QQFriend> getFriendsInGroupIndexOf( const int index);
    std::list<QQFriend> getAllFriends( const unsigned int myId = 0);
    std::map<unsigned int, QQFriend> &getAllFriendsMap() { return privateList; }
    
    const std::map<unsigned int, QQFriend> &getFriendList() const { return privateList; }
    
    bool setExtraInfo(const unsigned int id, const short info);
    bool setSignature(const unsigned int id, const std::string sig, const unsigned int time);
    const std::map<unsigned int, unsigned int> getMemberSignatureList(const unsigned int start, const unsigned int myId, const bool hasSignature);
    const std::list<unsigned int> getUserHeadList(const unsigned int myId, const bool hasUserHead);
    bool setMemo(const unsigned int id, const MemoItem &memo);

    QQFriend *firstFriend();
    QQFriend *nextFriend();
    FriendList &operator=(const FriendList &rhs) { privateList = rhs.getFriendList(); return *this;}
private:
    std::map<unsigned int, QQFriend> privateList;
    std::map<unsigned int, QQFriend>::iterator m_iter; // used only for trieverse outside of this class
    std::map<unsigned int, QQFriend>::iterator getFriendPosition(const unsigned int id);
};

#endif


 
