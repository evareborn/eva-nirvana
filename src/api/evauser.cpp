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
#include "evaresource.h"
#include "evaguimain.h"
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
//X #include <klocale.h>

std::string EvaUser::qunName = "Qun List";
std::string EvaUser::anonymousName = "Anonymous";
std::string EvaUser::blackName = "Black List";

EvaUser::EvaUser(const unsigned int id, const std::string &password)
	: //DCOPObject("Contacts"),
	QObject()
{
	qqNum = id;
	md5Password = new char[16];
	memcpy(md5Password, EvaUtil::doMd5Md5((char *)password.c_str(), password.length()), 16);

	timeOnline = 0;
	level = 0;
	hoursToLevelUp = 0;

	mSignature = "";
	mSignatureModifyTime = 0;

	status = Eva_Offline;
	groupNames.clear();
	setting = new EvaUserSetting(id);
	setting->loadSettings();
	isBuddyListLoaded = false;
	isQunListLoaded = false;

	//mLoginManager = new EvaLoginProcess();
}

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
	
	status = Eva_Offline;
	groupNames.clear();

	setting = new EvaUserSetting(id);
	//mLoginManager = new EvaLoginProcess();
}

EvaUser::~EvaUser()
{
	delete md5Password;
	delete setting;
}

char EvaUser::getStatusCode(const UserStatus status) 
{
	char statusCode;
	switch(status){
	case Eva_Online:
		statusCode = QQ_FRIEND_STATUS_ONLINE;
		break;
	case Eva_Offline:
		statusCode = QQ_FRIEND_STATUS_OFFLINE;
		break;
	case Eva_Invisible:
		statusCode = QQ_FRIEND_STATUS_INVISIBLE;
		break;
	case Eva_Leave:
		statusCode = QQ_FRIEND_STATUS_LEAVE;
		break;
	default:
		statusCode = QQ_FRIEND_STATUS_OFFLINE;
	}
	return statusCode;
}

bool EvaUser::newGroup( const std::string & name )
{
	groupNames.push_back(name);
	return true;
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

void EvaUser::removeGroupName(const int index)
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

void EvaUser::updateGroupName(const std::string &newName, const int index)
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
	if(setting->loadBuddyList()){
		isBuddyListLoaded = true;
	}else{
		isBuddyListLoaded = false;
	}
	return isBuddyListLoaded;
}

bool EvaUser::saveGroupedBuddyList()
{
	return setting->saveBuddyList(this, groupNames, details, myFriends, mExtraInfo, mSignature, mSignatureModifyTime);
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
	if(setting->loadQunList()){
		isQunListLoaded = true;
	}else{
		isQunListLoaded = false;
	}
	return isQunListLoaded;
}

bool EvaUser::saveQunList( )
{
	return setting->saveQunList(this, qunList);
}


void EvaUser::addLoginVerifyInfo( const GraphicVerifyCode & info )
{
	m_CodeList.push_back(info);
}

GraphicVerifyCode EvaUser::getLoginVerifyInfo( )
{
	GraphicVerifyCode code;
	if(m_CodeList.empty()) return code;
	return m_CodeList.front();
}


GraphicVerifyCode EvaUser::getNextLoginVerifyInfo( )
{
	GraphicVerifyCode code;
	if(m_CodeList.empty()) return code;
	code = m_CodeList.front();
	m_CodeList.pop_front();
	return code;
}


void EvaUser::clearAllVerifyCodes( )
{
	if(m_CodeList.size())
		m_CodeList.clear();
}

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
