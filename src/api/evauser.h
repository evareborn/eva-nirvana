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
// #include "evadcopcontacts.h"
#include "evausersetting.h"
#include <QObject>
#include <qstringlist.h>
//Added by qt3to4:
#include <QCustomEvent>
#include <cstring>
#include <list>
#include <QMutex>

// used in the reply of login token ex requests
class QQFriend;
class EvaUserSetting;
class CustomEvent;
//X class KConfig;

class EvaUser : public QObject//, virtual public EvaDCOPContactsInterface
{
    Q_OBJECT
    public:
        EvaUser(const unsigned int id, const std::string &password);
        EvaUser(const unsigned int id, const char *md5Password);
        virtual ~EvaUser();

        unsigned int getQQ() const { return qqNum; }
        const char *getMd5Password() const { return md5Password; }

        void setDetails(const ContactInfo &details);
        const ContactInfo &getDetails() const { return details; }

        int getNumberOfFriends() { return myFriends.numberOfFriends(); }
        FriendList &getFriendList() { return myFriends; }
        FriendList *getFriends() { return &myFriends; }
        void setFriendList(const FriendList &l);


        unsigned int getOnlineTime() const { return timeOnline;}     // the following 6 funs are about the level requst
        unsigned short getLevel() const { return level; }
        unsigned short getHoursToLevelUp() const { return hoursToLevelUp;}

        void setOnlineTime(unsigned int time );
        void setLevel( unsigned short l );
        void setHoursToLevelUp( unsigned short time );
 
        void addContact( const QQFriend& f );
        void deleteContact( unsigned int id );
        void setContactInfo( unsigned int id, const ContactInfo& info );
        void setContactSignature( unsigned int id, const std::string& sig, unsigned int time);
        void setContactMemo( unsigned int id, const MemoItem& memo );
        void updateFriendLevel( unsigned int id, unsigned int time, unsigned short level, unsigned short hours);
        void updateFriendGroupIndex(unsigned int id, int index);

        void newGroup(const std::string& name);
        void clearGroupNames();
        void setGroupNames(const std::list<std::string> &groups);
        void removeGroupName(int index);
        int getGroupIndexOf(const std::string &name);
        void updateGroupName(const std::string& newName, int index);

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
        //X 	KConfig *config(const QString &group);

        QunList *getQunList() { return &qunList; }
        void setQunList( const QunList &l) { qunList = l; }
        bool loadQunList();
        bool saveQunList();

        bool loadGroupedBuddyList();
        bool saveGroupedBuddyList();
        //X 	 bool isQunLoaded() const { return isQunListLoaded; }

        void setExtraInfo( const unsigned long long info) { mExtraInfo = info; }
        unsigned long long getExtraInfo() const { return mExtraInfo; }
        bool hasSignature() { return mExtraInfo & QQ_EXTAR_INFO_SIGNATURE; }
        bool hasQQTang() { return mExtraInfo & QQ_EXTAR_INFO_TANG; }
        bool hasQQAlbum() { return mExtraInfo & QQ_EXTAR_INFO_ALBUM; }
        bool hasPalEntry() { return mExtraInfo & QQ_EXTAR_INFO_PAL; }
        bool hasUserHead() { return mExtraInfo & QQ_EXTAR_INFO_USER_HEAD; }

        void setSignature(const std::string& sig, unsigned int time);
        const std::string &getSignature() const { return mSignature; }
        unsigned int getSignatureModifyTime() const { return mSignatureModifyTime; }

        //EvaLoginProcess *loginManager() { return mLoginManager; }

        /**
         * Session related methods
         */

//X         bool loginNeedVerify() const { return (m_CodeList.size() != 0); }
//X         void addLoginVerifyInfo(const GraphicVerifyCode &info);
//X         GraphicVerifyCode getLoginVerifyInfo();
//X         GraphicVerifyCode getNextLoginVerifyInfo();
//X         int getNumVerifyCodes() const { return m_CodeList.size(); }
//X         void clearAllVerifyCodes();
//X 
        // DCOP calls
        //X 	int numFriends();
        //X 	QStringList friends();
        //X 	bool hasFriend(unsigned int id);
        //X 	QString nickOf(unsigned int id);
        //X 	QString faceOf(unsigned int id, bool isOff);
        //X 	int genderOf(unsigned int id);
        //X 	int levelOf(unsigned int id);
        //X 	QString signatureOf(unsigned int id);
        //X 	int numGroups();
        //X 	QString groupName(int index);
        //X 	int group(unsigned int id);
        //X 	int numQuns();
        //X 	QStringList Quns();
        //X 	QString QunName(unsigned int ext);
        //X 	QString QunNotice(unsigned int ext);
        //X 	QString QunDescription(unsigned int ext);
        //X 	int numQunMembers(unsigned int ext);
        //X 	QStringList QunMembers(unsigned int ext);
        //X 	QString QunMemberNick(unsigned int ext, unsigned int id);
        //X 	QString QunMemberFace(unsigned int ext, unsigned int id, bool isOff);
        //X 	unsigned int myQQ();
        //X 	int onlineStatus(unsigned int id);
        //X 	QString currentLoginIP();
        //X 	QString lastLoginIP();
        //X 	QString lastLoginTime(); // format in yyyy-MM-dd hh:mm:ss
    signals:
        void loadGroupedBuddiesReady();
        void loadQunListReady();
    protected:
        virtual void customEvent( QCustomEvent * e );
 
    private:

	static bool isVersionCorrect(const QString& fileName);

    private:
        QMutex buddylistMutex;
        QMutex qunlistMutex;
        unsigned int qqNum;
        char *md5Password;
        ContactInfo details;
        FriendList myFriends;
        std::list<std::string> groupNames;
        EvaUserSetting *setting;

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


        friend class EvaUserSetting;
};

#endif


