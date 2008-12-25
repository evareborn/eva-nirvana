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
 
#include "evauser.h"
#include "evaguimain.h"
#include "evaresource.h"
#include "evaqtutil.h"
#include "evautil.h"
#include "evafriend.h"
#include <string.h>
#include <cstring>
#include <cstdlib>
#include <qtextcodec.h>
#include <qevent.h>
//Added by qt3to4:
#include <QCustomEvent>
#include <QDir>
//X #include <klocale.h>

std::string EvaUser::qunName = "Qun List";
std::string EvaUser::anonymousName = "Anonymous";
std::string EvaUser::blackName = "Black List";

EvaUser::EvaUser(const unsigned int id, const char *md5Password)
    //X 	: DCOPObject("Contacts")
{
    qqNum = id;

    this->md5Password = new char[16];
    memcpy(this->md5Password, md5Password, 16);

    timeOnline = 0;
    level = 0;
    hoursToLevelUp = 0;

    mSignature = "";
    mSignatureModifyTime = 0;

    groupNames.clear();

    setting = new EvaUserSetting(id);
    //mLoginManager = new EvaLoginProcess();
    loadGroupedBuddyList();
    loadQunList();
}

EvaUser::EvaUser(const unsigned int id, const std::string &password)
    : QObject()
{
    EvaUser( id, EvaUtil::doMd5Md5((char *)password.c_str(), password.length()) );
//X     qqNum = id;
//X     md5Password = new char[16];
//X     memcpy(md5Password, EvaUtil::doMd5Md5((char *)password.c_str(), password.length()), 16);
//X 
//X     timeOnline = 0;
//X     level = 0;
//X     hoursToLevelUp = 0;
//X 
//X     mSignature = "";
//X     mSignatureModifyTime = 0;
//X 
//X     //X 	status = Eva_Offline;
//X     groupNames.clear();
//X     setting = new EvaUserSetting(id);
//X     setting->loadSettings();
//X     isBuddyListLoaded = false;
//X     isQunListLoaded = false;
//X 
//X     //mLoginManager = new EvaLoginProcess();
}

EvaUser::~EvaUser()
{
    saveGroupedBuddyList();
    saveQunList();
    delete md5Password;
    delete setting;
}


void EvaUser::setDetails(const ContactInfo &details)
{ 
    this->details = details;
}

void EvaUser::addContact( const QQFriend& f )
{
    myFriends.addFriend(f);
}

void EvaUser::deleteContact( unsigned int id )
{
    myFriends.deleteFriend(id);
}

void EvaUser::setContactInfo( unsigned int id, const ContactInfo& info )
{
    if ( myFriends.hasFriend( id ) ) {
        myFriends.addContactInfoTo( id, info );
    }
}

void EvaUser::setContactSignature( unsigned int id, const std::string& sig, unsigned int time)
{
    if ( myFriends.hasFriend( id ) ) {
        myFriends.setSignature(id, sig, time );
    }
}

void EvaUser::setContactMemo( unsigned int id, const MemoItem& memo )
{
    if ( myFriends.hasFriend( id ) ) {
        myFriends.setMemo(id, memo );
    }
}

void EvaUser::updateFriendLevel(unsigned int id, unsigned int online, unsigned short level, unsigned short hours)
{
    if ( myFriends.hasFriend(id ) ) {
        myFriends.updateFriendLevel(id, online, level, hours );
    }
}

void EvaUser::updateFriendGroupIndex( unsigned int id, int index)
{
    if ( myFriends.hasFriend(id ) ) {
        myFriends.updateFriendGroupIndex(id, index );
    }
}

void EvaUser::newGroup( const std::string& name )
{
    groupNames.push_back(name);
}

void EvaUser::clearGroupNames()
{
    //std::string buddy = groupNameAtIndex(0); 
    groupNames.clear();
    groupNames.push_back("Buddy List");
}

void EvaUser::setGroupNames( std::list<std::string> &groups)
{
    groupNames = groups;
    groupNames.push_front("Buddy List");
    printf("==========EvaUser::setGroupNames:%d\n", groupNames.size());
}

void EvaUser::removeGroupName(int index)
{
    if(index == 0 ) return; // the first group name cannot be removed
    if((uint)index >= groupNames.size()) return; 
    std::list<std::string>::iterator iter;
    iter = groupNames.begin();
    for(int i=0; i<index; i++)
        iter++;
    groupNames.erase(iter);
}

int EvaUser::getGroupIndexOf(const std::string &name)
{
    std::list<std::string>::iterator iter;
    int i=0;	
    for(iter=groupNames.begin(); iter != groupNames.end(); ++iter){
        if(name== *iter) break;
        ++i;
    }
    if((uint)i>= groupNames.size()) 
        return -1;
    else 
        return i;	
}

void EvaUser::updateGroupName(const std::string& newName, int index)
{
    if(index == 0 ) return; // the first group name cannot be removed
    if((uint)index >= groupNames.size()) return; 
    std::list<std::string>::iterator iter;
    iter = groupNames.begin();
    int i;
    for(i=0; i<index; i++){
        iter++;
    }
    groupNames.insert(iter, newName);
    groupNames.erase(iter);
}

const std::string EvaUser::groupNameAtIndex( const int index )
{
    if(index == anonymousIndex) return anonymousName;
    if(index == blackIndex) return blackName;

    if((uint)index>= groupNames.size()) return "Buddy List";

    std::list<std::string>::iterator iter;
    int i=0;	
    for(iter=groupNames.begin(); iter != groupNames.end(); ++iter){
        if(i==index) break;
        ++i;
    }
    return *iter;
}

bool EvaUser::loadGroupedBuddyList()
{
    //X     if(!isDirExisted(setting->getEvaUserDir()))
    //X         return false;

    QString fullName = setting->getEvaBuddyListFilename();
    QFile file(fullName);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }

    if(!isVersionCorrect(fullName)){
        file.close();
        return false;
    }

    std::list<std::string> tmpGroupNames;
    ContactInfo myInfo;
    FriendList list;	
    Q_UINT32 numGroups=0;
    MemoItem memo;

    QDataStream stream(&file);

    // check version first
    char *flag = new char[3];
    stream.readRawBytes(flag, 3);
    Q_UINT32 version = 0;
    stream>>version;
    if(!(flag[0]=='E' && flag[1]=='V' && flag[2]=='A' && version == profileVersion)){
        file.close();
        file.remove();
        delete flag;
        return false;
    }
    delete []flag;

    // load my details first
    Q_UINT32 size=0;
    std::string item;
    std::vector<std::string> strlist;

    stream>>size;
    char *str = new char[512];
    for(uint i=0; i<size; i++){
        stream>>str;
        item = str;
        strlist.push_back(item);
    }	
    myInfo.setDetails(strlist);

    // read my extra info
    Q_UINT16 myExtraInfo;
    stream>>myExtraInfo;

    // read signature & time
    std::string signature;
    stream>>str; 
    signature = str;

    Q_UINT32 sigTime;
    stream>>sigTime;

    // read in how many groups
    stream>>numGroups;
    std::string name;
    // read all groups in
    for(uint i=0; i<numGroups; i++){
        stream>>str;
        name = str;
        tmpGroupNames.push_back(name);
    }

    Q_UINT32 id;
    Q_UINT16 face;
    Q_UINT8  age;
    Q_UINT8  gender;
    std::string nick;
    Q_UINT8  extFlag;
    Q_UINT8  commonFlag;
    Q_UINT32 groupIndex;
    Q_UINT16 extraInfo;
    std::string frdSig;
    Q_UINT32 frdSigTime;
    Q_UINT32 fontSize;
    Q_UINT32 fontColor;

    // read in all friends
    while(!stream.atEnd()){
        stream>>id;
        stream>>face;
        stream>>age;
        stream>>gender;
        stream>>str;
        nick = str;
        stream>>extFlag;
        stream>>commonFlag;
        stream>>groupIndex;
        stream>>extraInfo;
        stream>>str;
        frdSig = str;
        stream>>frdSigTime;

        strlist.clear();
        stream>>size;
        for(uint i=0; i<size; i++){
            stream>>str;
            item = str;
            strlist.push_back(item);
        }
        stream>>str;memo.name = str;
        stream>>str;memo.mobile = str;
        stream>>str;memo.telephone = str;
        stream>>str;memo.address = str;
        stream>>str;memo.email = str;
        stream>>str;memo.zipcode = str;
        stream>>str;memo.note = str;

        stream>>fontSize>>fontColor;
        QQFriend f(id, face);

        f.setAge(age);
        f.setGender(gender);
        f.setNick(nick);
        f.setExtFlag(extFlag);
        f.setCommonFlag(commonFlag);
        f.setGroupIndex(groupIndex);
        f.setExtraInfo(extraInfo);
        f.setSignature(frdSig, frdSigTime);
        ContactInfo info;
        info.setDetails(strlist);
        f.setUserInformation(info);
        f.setMemo(memo);
        f.setChatFontSize(fontSize);
        f.setChatFontColor(fontColor);

        list.addFriend(f);
    }
    delete []str;
    file.close();

    //X     EvaUser *user = ( EvaUser* ) receiver;
    //X     if(user)
    //X     {
    details              = myInfo;
    groupNames           = tmpGroupNames;
    myFriends            = list;
    mExtraInfo           = myExtraInfo;
    mSignature           = signature;
    mSignatureModifyTime = sigTime;
    //X         return true;
    //X     } else
    //X         return false;	

    //EvaMain::helper->setCategory(EvaHelper::LoadGroupedUsers, receiver);
    //EvaMain::helper->setLoadGroupedUsersArgs(&file);
    //EvaMain::helper->run();
    //return true;

    //X 	if(setting->loadBuddyList(this)){
    //X 		isBuddyListLoaded = true;
    //X 	}else{
    //X 		isBuddyListLoaded = false;
    //X 	}
    //X 	return isBuddyListLoaded;
    buddylistMutex.unlock();
    return true;
}

bool EvaUser::saveGroupedBuddyList()
{
    buddylistMutex.lock();
    QString dir = setting->getEvaUserDir();
    QDir d;
    if ( d.exists( dir ) && (!d.mkdir(dir))) {
        return false;
    }

    QString fullName = setting->getEvaBuddyListFilename();
    QFile file(fullName);
    if(file.exists()) file.remove();
    if(!file.open(QIODevice::WriteOnly)){
        return false;
    }
    EvaMain::helper->setCategory(EvaHelper::SaveGroupedUsers, this);
    EvaMain::helper->setSaveGroupedUsersArgs(&file, groupNames, details, myFriends, mExtraInfo, mSignature, mSignatureModifyTime);
    EvaMain::helper->run();
    return true;
    //X 
    //X 	return setting->saveBuddyList(this, groupNames, details, myFriends, mExtraInfo, mSignature, mSignatureModifyTime);
}

bool EvaUser::isVersionCorrect( const QString&fileName )
{
    QFile file(fileName);
    if(!file.exists()) return false;
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }
    Q_UINT32 version;
    QDataStream stream(&file);
    char *flag = new char[3];
    stream.readRawBytes(flag, 3);
    stream>>version;
    file.close();
    if(!(flag[0]=='E' && flag[1]=='V' && flag[2]=='A' && version == profileVersion)){
        file.remove();
        delete flag;
        return false;
    }
    delete flag;
    return true;
}
void EvaUser::setFriendList(const FriendList &l)
{ 
    myFriends.clearFriendList();
    myFriends = l;
}
void EvaUser::customEvent( QCustomEvent * e )
{
    if(e->type() == EvaLoadGroupedUsersEvent){
        EvaBuddyListEvent *ge = (EvaBuddyListEvent *)e;
        details = ge->getMyInfo();
        groupNames = ge->getGroupNames();
        myFriends = ge->getMyFriendList();
        mExtraInfo = ge->getExtraInfo();
        mSignature = ge->getSignature();
        mSignatureModifyTime = ge->getSignatureTime();
        emit loadGroupedBuddiesReady();
    }
    if(e->type() == EvaLoadQunUsersEvent){
        EvaQunListEvent *gl = (EvaQunListEvent *)e;
        qunList = gl->getQunList();
        emit loadQunListReady();
    }
}

bool EvaUser::loadQunList( )
{
    QString fullName = setting->getEvaQunListFilename();
    QFile file(fullName);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }

    qunlistMutex.lock();
    QDataStream stream(&file);

    // check version first
    char *flag = new char[3];
    stream.readRawBytes(flag, 3);
    Q_UINT32 version = 0;
    stream>>version;
    if(!(flag[0]=='E' && flag[1]=='V' && flag[2]=='A' && version == profileVersion)){
        file.close();
        file.remove();
        delete flag;
        return false;
    }
    delete flag;

    QunList list;

    Q_UINT32 qunID;
    Q_UINT32 extID;
    Q_UINT8 type;
    Q_UINT32 creator;
    Q_UINT8 authType;
    Q_UINT16 unknown1;
    Q_UINT16 category;
    Q_UINT32 versionID;
    std::string name;
    Q_UINT16 unknown2;
    std::string description;
    std::string notice;
    Q_UINT32 realNamesVersion;

    Q_UINT32 fontSize;
    Q_UINT32 fontColor;

    Q_UINT8 cardGender;
    Q_UINT8 msgType;
    char *str = new char[1024];
    memset(str, 0, 1024);

    while(!stream.atEnd()){
        // load qun basic info
        stream>>qunID>>extID>>type>>creator>>authType>>unknown1>>category>>versionID>>str;
        name = str;
        stream>>unknown2>>str;
        description = str;
        stream>>str;
        notice = str;
        QunInfo info;
        info.setQunID(qunID);
        info.setExtID(extID);
        info.setType(type);
        info.setCreator(creator);
        info.setAuthType(authType);
        info.setUnknown1(unknown1);
        info.setCategory(category);
        info.setVersionID(versionID);
        info.setName(name);
        info.setUnknown2(unknown2);
        info.setDescription(description);
        info.setNotice(notice);

        Qun qun(info.getQunID());
        qun.setDetails(info);

        stream >> fontSize >> fontColor;	
        qun.setChatFontSize(fontSize);
        qun.setChatFontColor(fontColor);

        // load message type
        stream>>msgType;
        qun.setMessageType((Qun::MessageType)msgType);

        stream >> realNamesVersion;
        qun.setRealNamesVersion(realNamesVersion);
        // load my card for this qun
        stream>>str;
        qun.setCardName(str);
        stream>>cardGender;
        qun.setCardGender(cardGender);
        stream>>str;
        qun.setCardPhone(str);
        stream>>str;
        qun.setCardEmail(str);
        stream>>str;
        qun.setCardMemo(str);

        // load all members details
        std::list<FriendItem> members;
        Q_UINT16 size;
        stream >> size;
        for(int i=0; i< size; i++){
            Q_UINT32 qqNum;
            Q_UINT16 face;
            Q_UINT8 age;
            Q_UINT8 gender;
            std::string nick;
            Q_UINT8 extFlag;  
            Q_UINT8 commonFlag;
            Q_UINT16 qunGroupIndex;
            Q_UINT16 qunAdminValue;
            std::string realName;   // added by henry

            stream>>qqNum>>face>>age>>gender>>str;
            nick = str;
            stream>>extFlag>>commonFlag>>qunGroupIndex>>qunAdminValue>>str;
            realName = str;
            FriendItem item;
            item.setQQ(qqNum);
            item.setFace(face);
            item.setAge(age);
            item.setGender(gender);
            item.setNick(nick);
            item.setExtFlag(extFlag);
            item.setCommonFlag(commonFlag);
            item.setQunGroupIndex(qunGroupIndex);
            item.setQunAdminValue(qunAdminValue);
            item.setQunRealName( realName); // henry
            members.push_back(item);
        }
        qun.setMembers(members);
        list.add(qun);
    }
    delete []str;
    file.close();
    qunlistMutex.unlock();
    qunList = list;
    return true;

    //X 	if(setting->loadQunList(this)){
    //X 		isQunListLoaded = true;
    //X 	}else{
    //X 		isQunListLoaded = false;
    //X 	}
    //X 	return isQunListLoaded;
}

bool EvaUser::saveQunList( )
{
    QString dir = setting->getEvaUserDir();
    QDir d;
    if ( d.exists( dir ) && (!d.mkdir(dir))) {
        return false;
    }

    QString fullName = setting->getEvaQunListFilename();
    QFile file(fullName);
    if(file.exists()) file.remove();
    if(!file.open(QIODevice::WriteOnly)){
        return false;
    }
    qunlistMutex.lock();
    EvaMain::helper->setCategory(EvaHelper::SaveQunUsers, this);
    EvaMain::helper->setSaveQunListArgs(&file, qunList);
    EvaMain::helper->run();
    qunlistMutex.lock();
    return true;

    //X 	return setting->saveQunList(this, qunList);
}


void EvaUser::setOnlineTime(unsigned int time )
{ 
    timeOnline = time;
}

void EvaUser::setLevel(unsigned short l )
{ 
    level = l;
}

void EvaUser::setHoursToLevelUp(unsigned short time )
{ 
    hoursToLevelUp = time;
}

void EvaUser::setSignature(const std::string& sig, unsigned int time)
{ 
    mSignature = sig;
    mSignatureModifyTime = time;
}

//X void EvaUser::addLoginVerifyInfo( const GraphicVerifyCode & info )
//X {
//X 	m_CodeList.push_back(info);
//X }
//X 
//X GraphicVerifyCode EvaUser::getLoginVerifyInfo( )
//X {
//X 	GraphicVerifyCode code;
//X 	if(m_CodeList.empty()) return code;
//X 	return m_CodeList.front();
//X }
//X 
//X 
//X GraphicVerifyCode EvaUser::getNextLoginVerifyInfo( )
//X {
//X 	GraphicVerifyCode code;
//X 	if(m_CodeList.empty()) return code;
//X 	code = m_CodeList.front();
//X 	m_CodeList.pop_front();
//X 	return code;
//X }
//X 
//X 
//X void EvaUser::clearAllVerifyCodes( )
//X {
//X 	if(m_CodeList.size())
//X 		m_CodeList.clear();
//X }
//X 
//X KConfig *EvaUser::config(const QString &group)
//X {
//X 	return setting->config(group);
//X }

/// DCOP calls for category Contacts
//X 
//X int EvaUser::numFriends( )
//X {
//X 	return myFriends.numberOfFriends();
//X }
//X 
//X QStringList EvaUser::friends( )
//X {
//X 	QStringList list;
//X 	std::map<unsigned int, QQFriend>::iterator iter = myFriends.getAllFriendsMap().begin();
//X 	for(; iter != myFriends.getAllFriendsMap().end(); ++iter){
//X 		list.append( QString::number(iter->first));
//X 	}
//X 	return list;
//X }
//X 
//X bool EvaUser::hasFriend( unsigned int id )
//X {
//X 	 return myFriends.hasFriend( id);
//X }
//X 
//X QString EvaUser::nickOf( unsigned int id )
//X {
//X 	QTextCodec *codec = QTextCodec::codecForName("GB18030");
//X 	std::string nick = "";
//X 	if(id == qqNum){
//X 		nick = details.at( ContactInfo::Info_nick );
//X 	} else {
//X 		QQFriend * frd = myFriends.getFriend( id);
//X 		if(frd){
//X 			nick = frd->getNick();
//X 		}
//X 	}
//X 	return codec->toUnicode(nick.c_str());
//X }
//X 
//X QString EvaUser::faceOf( unsigned int id, bool isOff)
//X {
//X 	unsigned short face = 0;
//X 	if(id == qqNum){
//X 		std::string str = details.at( ContactInfo::Info_face );
//X 		face = atoi(str.c_str());
//X 	} else {
//X 		QQFriend * frd = myFriends.getFriend( id);
//X 		if(frd){
//X 			face = frd->getFace();
//X 		}
//X 	}
//X 	int index = EvaMain::images->getFaceFileIndex(face);
//X 	QString ret = EvaMain::images->getFacePath() + "/";
//X 		ret +=  QString::number(index);
//X 		ret += (isOff?("off"):"" + QString(".png"));
//X 	return ret;
//X }
//X 
//X int EvaUser::genderOf( unsigned int id )
//X {
//X 	int g = 0;
//X 	if(id == qqNum){
//X 		std::string str = details.at( ContactInfo::Info_gender );
//X 		g = atoi(str.c_str());
//X 	} else {
//X 		QQFriend * frd = myFriends.getFriend( id);
//X 		if(frd){
//X 			g = frd->getGender();
//X 		}
//X 	}
//X 	return g;
//X }
//X 
//X int EvaUser::levelOf( unsigned int id )
//X {
//X 	int l = 0;
//X 	if(id == qqNum){
//X 		l = level;
//X 	} else {
//X 		QQFriend * frd = myFriends.getFriend( id);
//X 		if(frd){
//X 			l = frd->getLevel();
//X 		}
//X 	}
//X 	return l;
//X }
//X 
//X QString EvaUser::signatureOf( unsigned int id)
//X {
//X 	QTextCodec *codec = QTextCodec::codecForName("GB18030");
//X 	std::string s = "";
//X 	if(id == qqNum){
//X 		s = mSignature;
//X 	} else {
//X 		QQFriend * frd = myFriends.getFriend( id);
//X 		if(frd){
//X 			s = frd->getSignature();
//X 		}
//X 	}
//X 	return codec->toUnicode(s.c_str());
//X }
//X 
//X int EvaUser::numGroups( )
//X {
//X 	return groupNames.size();
//X }
//X 
//X QString EvaUser::groupName( int index )
//X {
//X 	QTextCodec *codec = QTextCodec::codecForName("GB18030");
//X 	std::string g = groupNameAtIndex(index);
//X 	return codec->toUnicode(g.c_str());
//X }
//X 
//X int EvaUser::group( unsigned int id )
//X {
//X 	int index = 0;
//X 	QQFriend * frd = myFriends.getFriend( id);
//X 	if(frd){
//X 		index = frd->getGroupIndex();
//X 	}
//X 	return index;
//X }
//X 
//X int EvaUser::numQuns( )
//X {
//X 	return qunList.numQuns();
//X }
//X 
//X QStringList EvaUser::Quns( )
//X {
//X 	QStringList qlist;
//X 	std::list<Qun> list = qunList.getQunList();
//X 	std::list<Qun>::iterator iter;
//X 	for(iter = list.begin(); iter!=list.end(); ++iter){
//X 		qlist.append(QString::number(iter->getExtQunID()));
//X 	}
//X 	return qlist;
//X }
//X 
//X QString EvaUser::QunName( unsigned int ext )
//X {
//X 	std::string q = "";
//X 	QTextCodec *codec = QTextCodec::codecForName("GB18030");
//X 
//X 	Qun *qun = qunList.getQunByExtID( ext);
//X 	if(qun){
//X 		q = qun->getDetails().getName();
//X 	}
//X 
//X 	return codec->toUnicode(q.c_str());
//X }
//X 
//X QString EvaUser::QunNotice( unsigned int ext )
//X {
//X 	std::string n = "";
//X 	QTextCodec *codec = QTextCodec::codecForName("GB18030");
//X 
//X 	Qun *qun = qunList.getQunByExtID( ext);
//X 	if(qun){
//X 		n = qun->getDetails().getNotice();
//X 	}
//X 
//X 	return codec->toUnicode(n.c_str());
//X }
//X 
//X QString EvaUser::QunDescription( unsigned int ext )
//X {
//X 	std::string d = "";
//X 	QTextCodec *codec = QTextCodec::codecForName("GB18030");
//X 
//X 	Qun *qun = qunList.getQunByExtID( ext);
//X 	if(qun){
//X 		d = qun->getDetails().getDescription();
//X 	}
//X 
//X 	return codec->toUnicode(d.c_str());
//X }
//X 
//X int EvaUser::numQunMembers( unsigned int ext )
//X {
//X 	int n = 0;
//X 	Qun *qun = qunList.getQunByExtID( ext);
//X 	if(qun){
//X 		n = qun->numMembers();
//X 	}
//X 	return n;
//X }
//X 
//X QStringList EvaUser::QunMembers( unsigned int ext )
//X {
//X 	QStringList members;
//X 
//X 	Qun *qun = qunList.getQunByExtID( ext);
//X 	if(qun){
//X 		//std::list<unsigned int> list = qun->getQQList();
//X 		std::list<FriendItem> list = qun->getMembers();
//X 		std::list<FriendItem>::iterator iter;
//X 		for(iter = list.begin(); iter!= list.end(); ++iter){
//X 			members.append(QString::number(iter->getQQ()));
//X 		}
//X 	}
//X 	return members;
//X }
//X 
//X QString EvaUser::QunMemberNick( unsigned int ext, unsigned int id )
//X {
//X 	std::string n = "";
//X 
//X 	QTextCodec *codec = QTextCodec::codecForName("GB18030");
//X 	Qun *qun = qunList.getQunByExtID( ext);
//X 	if(qun){
//X 		const FriendItem *m = qun->getMemberDetails(id);
//X 		n = m->getNick();
//X 	}
//X 	return codec->toUnicode(n.c_str());
//X }
//X 
//X QString EvaUser::QunMemberFace( unsigned int ext, unsigned int id, bool isOff)
//X {
//X 	unsigned short face = 0;
//X 
//X 	Qun *qun = qunList.getQunByExtID( ext);
//X 	if(qun){
//X 		const FriendItem *m = qun->getMemberDetails(id);
//X 		face = m->getFace();
//X 	}
//X 
//X 	int index = EvaMain::images->getFaceFileIndex(face);
//X 	QString ret = EvaMain::images->getFacePath() + "/";
//X 		ret += QString::number(index);
//X 		ret += (isOff?("off"):"" + QString(".png"));
//X 	return ret;
//X }
//X 
//X unsigned int EvaUser::myQQ( )
//X {
//X 	return qqNum;
//X }
//X 
//X int EvaUser::onlineStatus( unsigned int id )
//X {
//X 	int s = 0;
//X 	if(id == qqNum){
//X 		s = status;
//X 	} else {
//X 		QQFriend * frd = myFriends.getFriend( id);
//X 		if(frd){
//X 			s = frd->getStatus();
//X 		}
//X 	}
//X 	return s;
//X }
//X 
//X QString EvaUser::currentLoginIP( )
//X {
//X 	QHostAddress addr(getLoginWanIp());
//X 	return addr.toString();
//X }
//X 
//X QString EvaUser::lastLoginIP( )
//X {
//X 	//FIXME: this one is wrong getLastLoginIP is actually not the right one
//X 	//       we should use 0x00DA command to get the information
//X 	QHostAddress addr(getLastLoginIp());
//X 	return addr.toString();
//X }
//X 
//X QString EvaUser::lastLoginTime( )
//X {
//X 	QDateTime time;
//X 	time.setTime_t(getLastLoginTime());
//X 	return time.toString("yyyy-MM-dd hh:mm:ss");
//X }
//X 
//X 
