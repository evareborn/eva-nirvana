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
#include "evamain.h"
#include "evaqtutil.h"
#include "evautil.h"
#include "evafriend.h"
#include <string.h>
#include <cstring>
#include <cstdlib>
#include <qtextcodec.h>
#include <qevent.h>
#include <klocale.h>

std::string EvaUser::qunName = "Qun List";
std::string EvaUser::anonymousName = "Anonymous";
std::string EvaUser::blackName = "Black List";

EvaUser::EvaUser(const unsigned int id, const std::string &password)
	: DCOPObject("Contacts"),
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
	: DCOPObject("Contacts")
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

const bool EvaUser::newGroup( const std::string & name )
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

const int EvaUser::getGroupIndexOf(const std::string &name)
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

const bool EvaUser::loadGroupedBuddyList()
{
	if(setting->loadBuddyList()){
		isBuddyListLoaded = true;
	}else{
		isBuddyListLoaded = false;
	}
	return isBuddyListLoaded;
}

const bool EvaUser::saveGroupedBuddyList()
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

const bool EvaUser::loadQunList( )
{
	if(setting->loadQunList()){
		isQunListLoaded = true;
	}else{
		isQunListLoaded = false;
	}
	return isQunListLoaded;
}

const bool EvaUser::saveQunList( )
{
	return setting->saveQunList(this, qunList);
}


void EvaUser::addLoginVerifyInfo( const GraphicVerifyCode & info )
{
	m_CodeList.push_back(info);
}

const GraphicVerifyCode EvaUser::getLoginVerifyInfo( )
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

KConfig *EvaUser::config(const QString &group)
{
	return setting->config(group);
}

/// DCOP calls for category Contacts

int EvaUser::numFriends( )
{
	return myFriends.numberOfFriends();
}

QStringList EvaUser::friends( )
{
	QStringList list;
	std::map<unsigned int, QQFriend>::iterator iter = myFriends.getAllFriendsMap().begin();
	for(; iter != myFriends.getAllFriendsMap().end(); ++iter){
		list.append( QString::number(iter->first));
	}
	return list;
}

bool EvaUser::hasFriend( unsigned int id )
{
	 return myFriends.hasFriend( id);
}

QString EvaUser::nickOf( unsigned int id )
{
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	std::string nick = "";
	if(id == qqNum){
		nick = details.at( ContactInfo::Info_nick );
	} else {
		QQFriend * frd = myFriends.getFriend( id);
		if(frd){
			nick = frd->getNick();
		}
	}
	return codec->toUnicode(nick.c_str());
}

QString EvaUser::faceOf( unsigned int id, bool isOff)
{
	unsigned short face = 0;
	if(id == qqNum){
		std::string str = details.at( ContactInfo::Info_face );
		face = atoi(str.c_str());
	} else {
		QQFriend * frd = myFriends.getFriend( id);
		if(frd){
			face = frd->getFace();
		}
	}
	int index = EvaMain::images->getFaceFileIndex(face);
	QString ret = EvaMain::images->getFacePath() + "/";
		ret +=  QString::number(index);
		ret += (isOff?("off"):"" + QString(".png"));
	return ret;
}

int EvaUser::genderOf( unsigned int id )
{
	int g = 0;
	if(id == qqNum){
		std::string str = details.at( ContactInfo::Info_gender );
		g = atoi(str.c_str());
	} else {
		QQFriend * frd = myFriends.getFriend( id);
		if(frd){
			g = frd->getGender();
		}
	}
	return g;
}

int EvaUser::levelOf( unsigned int id )
{
	int l = 0;
	if(id == qqNum){
		l = level;
	} else {
		QQFriend * frd = myFriends.getFriend( id);
		if(frd){
			l = frd->getLevel();
		}
	}
	return l;
}

QString EvaUser::signatureOf( unsigned int id)
{
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	std::string s = "";
	if(id == qqNum){
		s = mSignature;
	} else {
		QQFriend * frd = myFriends.getFriend( id);
		if(frd){
			s = frd->getSignature();
		}
	}
	return codec->toUnicode(s.c_str());
}

int EvaUser::numGroups( )
{
	return groupNames.size();
}

QString EvaUser::groupName( int index )
{
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	std::string g = groupNameAtIndex(index);
	return codec->toUnicode(g.c_str());
}

int EvaUser::group( unsigned int id )
{
	int index = 0;
	QQFriend * frd = myFriends.getFriend( id);
	if(frd){
		index = frd->getGroupIndex();
	}
	return index;
}

int EvaUser::numQuns( )
{
	return qunList.numQuns();
}

QStringList EvaUser::Quns( )
{
	QStringList qlist;
	std::list<Qun> list = qunList.getQunList();
	std::list<Qun>::iterator iter;
	for(iter = list.begin(); iter!=list.end(); ++iter){
		qlist.append(QString::number(iter->getExtQunID()));
	}
	return qlist;
}

QString EvaUser::QunName( unsigned int ext )
{
	std::string q = "";
	QTextCodec *codec = QTextCodec::codecForName("GB18030");

	Qun *qun = qunList.getQunByExtID( ext);
	if(qun){
		q = qun->getDetails().getName();
	}

	return codec->toUnicode(q.c_str());
}

QString EvaUser::QunNotice( unsigned int ext )
{
	std::string n = "";
	QTextCodec *codec = QTextCodec::codecForName("GB18030");

	Qun *qun = qunList.getQunByExtID( ext);
	if(qun){
		n = qun->getDetails().getNotice();
	}

	return codec->toUnicode(n.c_str());
}

QString EvaUser::QunDescription( unsigned int ext )
{
	std::string d = "";
	QTextCodec *codec = QTextCodec::codecForName("GB18030");

	Qun *qun = qunList.getQunByExtID( ext);
	if(qun){
		d = qun->getDetails().getDescription();
	}

	return codec->toUnicode(d.c_str());
}

int EvaUser::numQunMembers( unsigned int ext )
{
	int n = 0;
	Qun *qun = qunList.getQunByExtID( ext);
	if(qun){
		n = qun->numMembers();
	}
	return n;
}

QStringList EvaUser::QunMembers( unsigned int ext )
{
	QStringList members;

	Qun *qun = qunList.getQunByExtID( ext);
	if(qun){
		//std::list<unsigned int> list = qun->getQQList();
		std::list<FriendItem> list = qun->getMembers();
		std::list<FriendItem>::iterator iter;
		for(iter = list.begin(); iter!= list.end(); ++iter){
			members.append(QString::number(iter->getQQ()));
		}
	}
	return members;
}

QString EvaUser::QunMemberNick( unsigned int ext, unsigned int id )
{
	std::string n = "";

	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	Qun *qun = qunList.getQunByExtID( ext);
	if(qun){
		const FriendItem *m = qun->getMemberDetails(id);
		n = m->getNick();
	}
	return codec->toUnicode(n.c_str());
}

QString EvaUser::QunMemberFace( unsigned int ext, unsigned int id, bool isOff)
{
	unsigned short face = 0;

	Qun *qun = qunList.getQunByExtID( ext);
	if(qun){
		const FriendItem *m = qun->getMemberDetails(id);
		face = m->getFace();
	}

	int index = EvaMain::images->getFaceFileIndex(face);
	QString ret = EvaMain::images->getFacePath() + "/";
		ret += QString::number(index);
		ret += (isOff?("off"):"" + QString(".png"));
	return ret;
}

unsigned int EvaUser::myQQ( )
{
	return qqNum;
}

int EvaUser::onlineStatus( unsigned int id )
{
	int s = 0;
	if(id == qqNum){
		s = status;
	} else {
		QQFriend * frd = myFriends.getFriend( id);
		if(frd){
			s = frd->getStatus();
		}
	}
	return s;
}

QString EvaUser::currentLoginIP( )
{
	QHostAddress addr(getLoginWanIp());
	return addr.toString();
}

QString EvaUser::lastLoginIP( )
{
	//FIXME: this one is wrong getLastLoginIP is actually not the right one
	//       we should use 0x00DA command to get the information
	QHostAddress addr(getLastLoginIp());
	return addr.toString();
}

QString EvaUser::lastLoginTime( )
{
	QDateTime time;
	time.setTime_t(getLastLoginTime());
	return time.toString("yyyy-MM-dd hh:mm:ss");
}


