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
  
#include "evausersetting.h"
#include "evaqtutil.h"
#include "evamain.h"
#include <vector>
#include <stdlib.h>
#include <qdatastream.h>
#include <q3textstream.h>
#include <qstringlist.h>
#include <qfile.h>
#include <qdir.h>
//X #include <kconfig.h>
//X #include <klocale.h>
//X #include <kdebug.h>

EvaUserSetting::EvaUserSetting(const int id)
    : m_WinSize( 200, 500 )
{
	qqNum = id;
	isUserDirExisted=false;
	pictureCacheDir = getEvaUserDir() + "/customCaches";
	customSmileyDir = getEvaUserDir() + "/CustomFace";
	userHeadDir = getEvaUserDir() + "/UserHead";
	loadDefaultSettings();
	buddyListFileName = "user.dat";
	qunListFileName = "qun.dat";
	chatMsgFileName = "chat.msg";
	qunMsgFileName = "qun.msg";
	sysMsgFileName = "sys.msg";
	userProfileFileName = "user.profile";
	userConfigFileName = "user.cfg";
	
	if(!createDefaultDirs())
		printf("error, cannot create user directory!\n");
	
//X 	m_config = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
	
	m_NeedRepaint = true;
}

EvaUserSetting::~EvaUserSetting()
{
//X 	if(m_config){
//X 		m_config->sync();
//X 		delete m_config;
//X 	}
}

bool EvaUserSetting::saveBuddyList(QObject *receiver, std::list<std::string> groups, ContactInfo &myInfo, FriendList &list,
				unsigned short extraInfo, std::string sig, unsigned int sigTime)
{
	if(!isDirExisted(getEvaUserDir())){
		QDir d;
		if(!d.mkdir(getEvaUserDir()))
			return false;
	}
	
	QString fullName = getEvaUserDir() + "/" + buddyListFileName;
	QFile file(fullName);
	if(file.exists()) file.remove();
	if(!file.open(QIODevice::WriteOnly)){
		return false;
	}
	EvaMain::helper->setCategory(EvaHelper::SaveGroupedUsers, receiver);
	EvaMain::helper->setSaveGroupedUsersArgs(&file, groups, myInfo, list, extraInfo, sig, sigTime);
	EvaMain::helper->run();
	return true;
}

bool EvaUserSetting::loadBuddyList( /*QObject * receiver*/)
{
	if(!isDirExisted(getEvaUserDir()))
		return false;
	
	QString fullName = getEvaUserDir() + "/" + buddyListFileName;
	QFile file(fullName);
	if(!file.open(QIODevice::ReadOnly)){
		return false;
	}
	if(!isVersionCorrect(fullName)){
		file.close();
		return false;
	}

	std::list<std::string> groupNames;
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
		groupNames.push_back(name);
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
	
	EvaUser *user = EvaMain::user;
	if(user)
	{
		user->details              = myInfo;
		user->groupNames           = groupNames;
		user->myFriends            = list;
		user->mExtraInfo           = myExtraInfo;
		user->mSignature           = signature;
		user->mSignatureModifyTime = sigTime;
		return true;
	} else
		return false;	
	
	//EvaMain::helper->setCategory(EvaHelper::LoadGroupedUsers, receiver);
	//EvaMain::helper->setLoadGroupedUsersArgs(&file);
	//EvaMain::helper->run();
	//return true;
}

bool EvaUserSetting::saveMessage(const int buddy, const int sender, QString sNick, 
			const int receiver, QString rNick,
			const bool isNormal, 
			const QString message, 
			const QDateTime time, const char fontSize, 
			const bool u, const bool i, const bool b, 
			const char blue, const char green, const char red, const bool isQunMsg)
{
	if(!isDirExisted(getEvaUserDir())){
		QDir d;
		if(!d.mkdir(getEvaUserDir()))
			return false;
	}
	
	QString fullName = getEvaUserDir() + "/" + (isQunMsg?qunMsgFileName:chatMsgFileName);
	QFile file(fullName);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
		return false;
	}

	
	QDataStream stream(&file);
	
	Q_UINT32 s_buddy = buddy;
	Q_UINT32 s_sender = sender;
	Q_UINT32 s_receiver = receiver;
	Q_UINT8  s_isNormal = isNormal?1:0;
	Q_UINT32 s_time = time.toTime_t();
	Q_UINT8  s_fontSize = fontSize;
	Q_UINT8  s_flag = (u?1:0)|( (i?1:0)<<1 ) | ( (b?1:0)<<2 );
	Q_UINT8  s_blue = blue;
	Q_UINT8  s_green = green;
	Q_UINT8  s_red = red;
	
	stream<<s_buddy;
	stream<<s_sender;
	stream<<sNick;
	stream<<s_receiver;
	stream<<rNick;
	stream<<s_isNormal;
	stream<<message;
	stream<<s_time;
	stream<<s_fontSize;
	stream<<s_flag;
	stream<<s_blue;
	stream<<s_green;
	stream<<s_red;
	
	file.close();
	return true;
}

std::list<EvaUserSetting::chatMessage> EvaUserSetting::getMessages( const int buddy, const int page, const bool isQunMsg)
{
	std::list<chatMessage>list;
	if(!isDirExisted(getEvaUserDir())){
		return list;
	}
	
	QString fullName = getEvaUserDir() + "/" + (isQunMsg?qunMsgFileName:chatMsgFileName);
	QFile file(fullName);
	if(!file.open(QIODevice::ReadOnly)){
		return list;
	}
	Q_UINT32 r_buddy;
	Q_UINT32 sender;
	QString  sNick;
	Q_UINT32 receiver;
	QString  rNick; 
	Q_UINT8  type; // 0 auto reply,  1 normal
	QString  message;
	Q_UINT32  intTime;
	QDateTime time;
	Q_UINT8   fontSize;
	Q_UINT8   flag; // start from right.  bit 0: u, bit 1: i, bit 2: b
	Q_UINT8   blue;
	Q_UINT8   green;
	Q_UINT8   red;
	
	QDataStream stream(&file);
	std::list<chatMessage>srclist;	
	while(!stream.atEnd()){
		stream>>r_buddy;
		stream>>sender;
		stream>>sNick;
		stream>>receiver;
		stream>>rNick;
		stream>>type;
		stream>>message;
		stream>>intTime;
		stream>>fontSize;
		stream>>flag;
		stream>>blue;
		stream>>green;
		stream>>red;
		if(r_buddy == (uint)buddy){	
			chatMessage m;
			m.sender = sender;
			m.sNick = sNick;
			m.receiver = receiver;
			m.rNick = rNick;
			m.type = type;
			m.message = message;
			time.setTime_t(intTime);
			m.time = time;
			m.fontSize = fontSize; 
			m.flag = flag; // start from right.  bit 0: u, bit 1: i, bit 2: b
			m.blue = blue;
			m.green = green;
			m.red = red;
			srclist.push_back(m);
		}
	}
	file.close();
	
	srclist.reverse();
	
	std::list<chatMessage>::iterator iter;	
	int current=0;
	int start = page * pageSize;
	int end = start + pageSize; // note that this value is always one more than we want
	for(iter=srclist.begin(); iter!=srclist.end(); iter++){
		if(current >= end) break;
		if(current>=start && current < end){
			list.push_back(*iter);
		}
		current++;
	}
	return list;
}

std::list<EvaUserSetting::chatMessage> EvaUserSetting::getMessages(const int buddy, const int page, 
							const QDateTime starttime, const QDateTime endtime, const bool isQunMsg)
{
	std::list<chatMessage>list;
	if(!isDirExisted(getEvaUserDir())){
		return list;
	}
	
	QString fullName = getEvaUserDir() + "/" + (isQunMsg?qunMsgFileName:chatMsgFileName);
	QFile file(fullName);
	if(!file.open(QIODevice::ReadOnly)){
		return list;
	}
	Q_UINT32 r_buddy;
	Q_UINT32 sender;
	QString  sNick;
	Q_UINT32 receiver;
	QString  rNick; 
	Q_UINT8  type; // 0 auto reply,  1 normal
	QString  message;
	Q_UINT32  intTime;
	QDateTime time;
	Q_UINT8   fontSize;
	Q_UINT8   flag; // start from right.  bit 0: u, bit 1: i, bit 2: b
	Q_UINT8   blue;
	Q_UINT8   green;
	Q_UINT8   red;

	int start = starttime.toTime_t();
	int end = endtime.toTime_t();
	
	std::list<chatMessage>srclist;
	QDataStream stream(&file);	
	while(!stream.atEnd()){
		stream>>r_buddy;
		stream>>sender;
		stream>>sNick;
		stream>>receiver;
		stream>>rNick;
		stream>>type;
		stream>>message;
		stream>>intTime;
		stream>>fontSize;
		stream>>flag;
		stream>>blue;
		stream>>green;
		stream>>red;
		if(r_buddy == (uint)buddy){
			if(intTime>=(uint)start && intTime < (uint)end){
				chatMessage m;
				m.sender = sender;
				m.sNick = sNick;
				m.receiver = receiver;
				m.rNick = rNick;
				m.type = type;
				m.message = message;
				time.setTime_t(intTime);
				m.time = time;
				m.fontSize = fontSize; 
				m.flag = flag; // start from right.  bit 0: u, bit 1: i, bit 2: b
				m.blue = blue;
				m.green = green;
				m.red = red;
				srclist.push_back(m);
			}
		}
	}
	file.close();

	srclist.reverse();
	
	std::list<chatMessage>::iterator iter;	
	int current=0;
	start = page * pageSize;
	end = start + pageSize; // note that this value is always one more than we want
	for(iter=srclist.begin(); iter!=srclist.end(); iter++){
		if(current >= end) break;
		if(current>=start && current < end){
			list.push_back(*iter);
		}
		current++;
	}	
	return list;
}

bool EvaUserSetting::saveSysMessage(const short msgType, const unsigned char type, const int fromQQ, const int toQQ, 
					const QString message, const int internalQunID, const unsigned int commander,
					const unsigned char *code, const unsigned short codeLen,
					const unsigned char *token, const unsigned short tokenLen)
{
	if(!isDirExisted(getEvaUserDir())){
		QDir d;
		if(!d.mkdir(getEvaUserDir()))
			return false;
	}

	QString fullName = getEvaUserDir() + "/" + sysMsgFileName;
	QFile file(fullName);

	bool needHeader = false;
	if(file.open(QIODevice::ReadOnly)){
		QDataStream chkStream(&file);
		// check version first, if not matched, delete it
		char *flag = new char[3];
		chkStream.readRawBytes(flag, 3);
		Q_UINT32 version = 0;
		chkStream>>version;
		if(!(flag[0]=='E' && flag[1]=='V' && flag[2]=='A' && version == profileVersion)){
			file.close();
			file.remove();
			needHeader = true;
		}
		delete []flag;
	} else {
		needHeader = true;
	}
	file.close();
	if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
		return false;
	}
	QDataStream stream(&file);

	if(needHeader){
		// save Eva flag and Version
		stream.writeRawBytes("EVA", 3);
		stream<<profileVersion;
	}
	sysMessage m;
	m.messageType = msgType;
	m.type = type;
	m.from = fromQQ;
	m.to = toQQ;
	m.message = message;
	m.internalQunID = internalQunID;
	m.commander = commander;
	m.setCode( code, codeLen);
	m.setToken( token, tokenLen);
	
	Q_UINT16 len = (Q_UINT16)(strlen((m.message.utf8().data())));
	//printf("saving type: %d, from: %d, to: %d, cmder: %d, msg: %s\n", type, fromQQ, toQQ, commander, message.local8Bit().data());
	stream<<m.messageType;
	stream<<m.type;
	stream<<m.from;
	stream<<m.to;
	stream<<m.commander;
	stream<<m.internalQunID;
	stream<< len;
	stream.writeRawBytes(m.message.utf8().data(), len);
	stream<<m.codeLen;
	stream.writeRawBytes( (const char *)(m.code), m.codeLen);
	stream<<m.tokenLen;
	stream.writeRawBytes( (const char *)(m.token), m.tokenLen);
	file.close();
	
	sysStack.push(m);
	return true;
}

std::list<EvaUserSetting::sysMessage> EvaUserSetting::getSysMessages( const int page) // page number starts from 0
{
	std::list<sysMessage>list;
	if(!isDirExisted(getEvaUserDir())){
		return list;
	}
	
	QString fullName = getEvaUserDir() + "/" + sysMsgFileName;
	QFile file(fullName);
	if(!file.open(QIODevice::ReadOnly)){
		return list;
	}

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
		return list;
	}
	delete []flag;

	Q_UINT16 msgType;
	Q_UINT8 type;
	Q_UINT32 from;
	Q_UINT32 to;
	Q_UINT32 commander;
	Q_UINT32 internalQunID;
	QString  message;
	Q_UINT16 len;
	unsigned char *tmp;

	
	std::list<sysMessage>srclist;

	while(!stream.atEnd()){
		sysMessage m;

		stream>>msgType;
		stream>>type;
		stream>>from;
		stream>>to;
		stream>>commander;
		stream>>internalQunID;
		stream>>len;
		if(len){
			tmp = new unsigned char[len+1];
			stream.readRawBytes( (char *)(tmp), len);
			tmp[len] = 0x00;
			message = QString::fromUtf8((char *)(tmp), len);
			delete [] tmp;
			m.message = message;
		}
		//stream>>message;

		stream>>len;
		if(len){
			tmp = new unsigned char[len];
			stream.readRawBytes( (char *)(tmp), len);
			m.setCode( tmp, len);
			delete []tmp;
		}

		stream>>len;
		if(len){
			tmp = new unsigned char[len];
			stream.readRawBytes( (char *)(tmp), len);
			m.setToken( tmp, len);
			delete []tmp;
		}	
		//printf("loading type: %d, from: %d, to: %d, cmder: %d, msg: %s\n", type, from, to, commander, m.message.local8Bit().data());
		m.messageType = msgType;
		m.type = type;
		m.from = from;
		m.to = to;
		m.commander = commander;
		m.internalQunID = internalQunID;
		srclist.push_back(m);
	}
	file.close();

	srclist.reverse();
	
	std::list<sysMessage>::iterator iter;	
	int current=0;
	int start = page * pageSize;
	int end = start + pageSize; // note that this value is always one more than we want
	for(iter=srclist.begin(); iter!=srclist.end(); iter++){
		if(current >= end) break;
		if(current>=start && current < end){
			list.push_back(*iter);
		}
		current++;
	}
	return list;
}

EvaUserSetting::sysMessage EvaUserSetting::getLastSysMessage()
{
	sysMessage m;
	m.type = 0;
	m.from = 0;
	if(!sysStack.empty()){
		m = sysStack.top();
		sysStack.pop();
	}
	return m;
}

bool EvaUserSetting::saveSettings()
{
	if(!isDirExisted(getEvaUserDir())){
		QDir d;
		if(!d.mkdir(getEvaUserDir()))
			return false;
	}

//X 	KConfig *cfg = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
//X 	
//X 	KConfigGroupSaver *groupSaver = new KConfigGroupSaver(cfg, "General");
//X 	cfg->writeEntry("DISPLAY_QQ_BROADCAST", showQQBroadcast);
//X 	cfg->writeEntry("DISPLAY_QQ_NEWS", m_ShowQQNews);
//X 	cfg->writeEntry("DISPLAY_TIP_MESSAGE_TIP_WINDOW", showMessageTipWindow);
//X 	cfg->writeEntry("DISPLAY_BUDDY_ONLINE_NOTIFY", showBudyOnlineNotifyWindow);
//X 	cfg->writeEntry("DISPLAY_ONLINE_BUDDIES", showOnlineUsers);
//X 	cfg->writeEntry("SEND_KEY", isSendKeyEnter);
//X 	cfg->writeEntry("PLAY_SOUND", playSound);
//X 	cfg->writeEntry("SHOW_SMILEY_IN_NICK", m_ShowSmileyInNickName);
//X 	cfg->writeEntry("SHOW_SIG_IN_SEPERATE_LINE", m_ShowSignatureInSeperateLine);
//X 	delete groupSaver;
//X 	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Font");
//X 	cfg->writeEntry("BUDDY_NAME_COLOR", m_BuddyNameColor);
//X 	cfg->writeEntry("BUDDY_NAME_FONT_B", m_BuddyNameFontB);
//X 	cfg->writeEntry("BUDDY_NAME_FONT_U", m_BuddyNameFontU);
//X 	cfg->writeEntry("BUDDY_NAME_FONT_I", m_BuddyNameFontI);
//X 
//X 	cfg->writeEntry("BUDDY_SIGNATURE_COLOR", m_BuddySigColor);
//X 	cfg->writeEntry("BUDDY_SIGNATURE_FONT_B", m_BuddySigFontB);
//X 	cfg->writeEntry("BUDDY_SIGNATURE_FONT_U", m_BuddySigFontU);
//X 	cfg->writeEntry("BUDDY_SIGNATURE_FONT_I", m_BuddySigFontI);
//X 
//X 	cfg->writeEntry("BUDDY_FLASH_COLOR", m_BuddyFlashColor);
//X 	cfg->writeEntry("BUDDY_FLASH_FONT_B", m_BuddyFlashFontB);
//X 	cfg->writeEntry("BUDDY_FLASH_FONT_U", m_BuddyFlashFontU);
//X 	cfg->writeEntry("BUDDY_FLASH_FONT_I", m_BuddyFlashFontI);
//X 
//X 	// save Qun font settings
//X 	cfg->writeEntry("QUN_NAME_COLOR", m_QunNameColor);
//X 	cfg->writeEntry("QUN_NAME_FONT_B", m_QunNameFontB);
//X 	cfg->writeEntry("QUN_NAME_FONT_U", m_QunNameFontU);
//X 	cfg->writeEntry("QUN_NAME_FONT_I", m_QunNameFontI);
//X 
//X 	cfg->writeEntry("QUN_FLASH_COLOR", m_QunFlashColor);
//X 	cfg->writeEntry("QUN_FLASH_FONT_B", m_QunFlashFontB);
//X 	cfg->writeEntry("QUN_FLASH_FONT_U", m_QunFlashFontU);
//X 	cfg->writeEntry("QUN_FLASH_FONT_I", m_QunFlashFontI);
//X 
//X 	// save group font settings
//X 	cfg->writeEntry("GROUP_NAME_COLOR", m_GroupNameColor);
//X 	cfg->writeEntry("GROUP_NAME_FONT_B", m_GroupNameFontB);
//X 	cfg->writeEntry("GROUP_NAME_FONT_U", m_GroupNameFontU);
//X 	cfg->writeEntry("GROUP_NAME_FONT_I", m_GroupNameFontI);
//X 
//X 	cfg->writeEntry("GROUP_ONLINE_COUNT_COLOR", m_GroupCountColor);
//X 	cfg->writeEntry("GROUP_ONLINE_COUNT_FONT_B", m_GroupOnlineCountFontB);
//X 	cfg->writeEntry("GROUP_ONLINE_COUNT_FONT_U", m_GroupOnlineCountFontU);
//X 	cfg->writeEntry("GROUP_ONLINE_COUNT_FONT_I", m_GroupOnlineCountFontI);
//X 
//X 	cfg->writeEntry("GROUP_FLASH_COLOR", m_GroupFlashColor);
//X 	cfg->writeEntry("GROUP_FLASH_FONT_B", m_GroupFlashFontB);
//X 	cfg->writeEntry("GROUP_FLASH_FONT_U", m_GroupFlashFontU);
//X 	cfg->writeEntry("GROUP_FLASH_FONT_I", m_GroupFlashFontI);
//X 
//X 	delete groupSaver;
//X 	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Theme");
//X 	cfg->writeEntry("THEME_DIR", themeDir);
//X 	cfg->writeEntry("SOUND_DIR", soundDir);
//X 	delete groupSaver;
//X 	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Reply");
//X 	cfg->writeEntry("AUTO_REPLY_SELECTED_INDEX", autoSelectedIndex);
//X 	std::list<QString>::iterator iter;
//X 	QStringList strList;
//X 	for(iter = autoList.begin(); iter!=autoList.end(); ++iter){
//X 		strList += (*iter);
//X 	}
//X 	cfg->writeEntry("AUTO_REPLY", strList, '\255');
//X 	strList.clear();
//X 	for(iter = quickList.begin(); iter!=quickList.end(); ++iter){
//X 		strList += (*iter);
//X 	}
//X 	cfg->writeEntry("QUICK_REPLY", strList, '\255');	
//X 	delete groupSaver;
//X 
//X 	groupSaver = new KConfigGroupSaver(cfg, "Others");
//X 	cfg->writeEntry("FACE_SIZE", faceSize);
//X 	cfg->writeEntry("MESSAGE_PAGE_SIZE", pageSize);
//X 	cfg->writeEntry("MESSAGE_SHORTCUT", shortcut.toString());
//X 	cfg->writeEntry("WIN_GEOMETRY_POINT", m_WinLeftTop);
//X 	cfg->writeEntry("WIN_GEOMETRY_SIZE", m_WinSize);
//X 	cfg->writeEntry("MAX_IDLE_TIME", idleMaxTime);
//X 	delete groupSaver;
//X 	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Cached_ToBeAdded_Buddies");
//X 	cfg->writeEntry("NUM_BUDDIES", m_CachedToBeAddedBuddy.size());
//X 	std::map<unsigned int, QString>::iterator itr;
//X 	int num = 0;
//X 	for(itr = m_CachedToBeAddedBuddy.begin(); itr!=m_CachedToBeAddedBuddy.end(); ++itr){
//X 		cfg->writeEntry(QString("QQ_%1").arg(num++), itr->second);
//X 	}	
//X 	delete groupSaver;
//X 
//X 	groupSaver = new KConfigGroupSaver(cfg, "Reject_Forever");
//X 	cfg->writeEntry("QQ_REJECT_LIST_SIZE", m_RejectForever.size());
//X 	std::list<unsigned int>::iterator rejectIter;
//X 	num = 0;
//X 	for(rejectIter = m_RejectForever.begin(); rejectIter!=m_RejectForever.end(); ++rejectIter){
//X 		cfg->writeEntry(QString("QQ_%1").arg(num++), (unsigned int)(*rejectIter));
//X 	}
//X 	delete groupSaver;
//X 
//X 	groupSaver = new KConfigGroupSaver(cfg, "Qun_Reject_Forever");
//X 	cfg->writeEntry("QUN_REJECT_LIST_SIZE", m_QunRejectForever.size());
//X 	std::map<unsigned int, std::list<unsigned int> >::iterator qunRejectIter;
//X 	num = 0;
//X 	for(qunRejectIter = m_QunRejectForever.begin(); qunRejectIter!=m_QunRejectForever.end(); ++qunRejectIter){
//X 		cfg->writeEntry(QString("Qun_%1").arg(num++), qunRejectIter->first);
//X 		cfg->writeEntry("QUN_REJECT_MEMBER_SIZE", (qunRejectIter->second).size());
//X 		int memberCount = 0;
//X 		for(rejectIter = qunRejectIter->second.begin(); rejectIter!= qunRejectIter->second.end(); ++rejectIter){
//X 			cfg->writeEntry(QString("Qun_QQ_%1").arg(memberCount++), (unsigned int)(*rejectIter));
//X 		}
//X 	}
//X 	delete groupSaver;
//X 
//X 	//cfg->sync();
//X 	delete cfg;
	return true;
}

bool EvaUserSetting::loadSettings()
{
	if(!isDirExisted(getEvaUserDir())){
		return false;
	}
	
	QString fullName = getEvaUserDir() + "/" + userProfileFileName;
	QDir d;
	if(d.exists(fullName)){
		loadOldProfile();
		QFile::remove(fullName);
		saveSettings();
	}
	
//X //X 	KConfig *cfg = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
//X 	
//X 	KConfigGroupSaver *groupSaver = new KConfigGroupSaver(cfg, "General");
//X 	showQQBroadcast = cfg->readBoolEntry("DISPLAY_QQ_BROADCAST", true);
//X 	m_ShowQQNews = cfg->readBoolEntry("DISPLAY_QQ_NEWS", true);
//X 	showMessageTipWindow = cfg->readBoolEntry("DISPLAY_TIP_MESSAGE_TIP_WINDOW", true);
//X 	showBudyOnlineNotifyWindow = cfg->readBoolEntry("DISPLAY_BUDDY_ONLINE_NOTIFY", true);
//X 	showOnlineUsers = cfg->readBoolEntry("DISPLAY_ONLINE_BUDDIES", false);
//X 	isSendKeyEnter = cfg->readBoolEntry("SEND_KEY", false);
//X 	playSound = cfg->readBoolEntry("PLAY_SOUND", true);
//X 	m_ShowSmileyInNickName = cfg->readBoolEntry("SHOW_SMILEY_IN_NICK", false);
//X 	m_ShowSignatureInSeperateLine = cfg->readBoolEntry("SHOW_SIG_IN_SEPERATE_LINE", false);
//X 	delete groupSaver;
//X 	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Font");
//X 	m_BuddyNameColor = cfg->readColorEntry("BUDDY_NAME_COLOR", &Qt::black);
//X 	m_BuddyNameFontB = cfg->readBoolEntry("BUDDY_NAME_FONT_B", false);
//X 	m_BuddyNameFontU = cfg->readBoolEntry("BUDDY_NAME_FONT_U", false);
//X 	m_BuddyNameFontI = cfg->readBoolEntry("BUDDY_NAME_FONT_I", false);
//X 
//X 	m_BuddySigColor = cfg->readColorEntry("BUDDY_SIGNATURE_COLOR", &Qt::gray);
//X 	m_BuddySigFontB = cfg->readBoolEntry("BUDDY_SIGNATURE_FONT_B", false);
//X 	m_BuddySigFontU = cfg->readBoolEntry("BUDDY_SIGNATURE_FONT_U", false);
//X 	m_BuddySigFontI = cfg->readBoolEntry("BUDDY_SIGNATURE_FONT_I", false);
//X 
//X 	m_BuddyFlashColor = cfg->readColorEntry("BUDDY_FLASH_COLOR", &Qt::red);
//X 	m_BuddyFlashFontB = cfg->readBoolEntry("BUDDY_FLASH_FONT_B", false);
//X 	m_BuddyFlashFontU = cfg->readBoolEntry("BUDDY_FLASH_FONT_U", false);
//X 	m_BuddyFlashFontI = cfg->readBoolEntry("BUDDY_FLASH_FONT_I", false);
//X 
//X 	m_QunNameColor = cfg->readColorEntry("QUN_NAME_COLOR", &Qt::black);
//X 	m_QunNameFontB = cfg->readBoolEntry("QUN_NAME_FONT_B", false);
//X 	m_QunNameFontU = cfg->readBoolEntry("QUN_NAME_FONT_U", false);
//X 	m_QunNameFontI = cfg->readBoolEntry("QUN_NAME_FONT_I", false);
//X 
//X 	m_QunFlashColor = cfg->readColorEntry("QUN_FLASH_COLOR", &Qt::red);
//X 	m_QunFlashFontB = cfg->readBoolEntry("QUN_FLASH_FONT_B", false);
//X 	m_QunFlashFontU = cfg->readBoolEntry("QUN_FLASH_FONT_U", false);
//X 	m_QunFlashFontI = cfg->readBoolEntry("QUN_FLASH_FONT_I", false);
//X 
//X 	m_GroupNameColor = cfg->readColorEntry("GROUP_NAME_COLOR", &Qt::black);
//X 	m_GroupNameFontB = cfg->readBoolEntry("GROUP_NAME_FONT_B", false);
//X 	m_GroupNameFontU = cfg->readBoolEntry("GROUP_NAME_FONT_U", false);
//X 	m_GroupNameFontI = cfg->readBoolEntry("GROUP_NAME_FONT_I", false);
//X 
//X 	m_GroupCountColor = cfg->readColorEntry("GROUP_ONLINE_COUNT_COLOR", &Qt::blue);
//X 	m_GroupOnlineCountFontB = cfg->readBoolEntry("GROUP_ONLINE_COUNT_FONT_B", false);
//X 	m_GroupOnlineCountFontU = cfg->readBoolEntry("GROUP_ONLINE_COUNT_FONT_U", false);
//X 	m_GroupOnlineCountFontI = cfg->readBoolEntry("GROUP_ONLINE_COUNT_FONT_I", false);
//X 
//X 	m_GroupFlashColor = cfg->readColorEntry("GROUP_FLASH_COLOR", &Qt::red);
//X 	m_GroupFlashFontB = cfg->readBoolEntry("GROUP_FLASH_FONT_B", false);
//X 	m_GroupFlashFontU = cfg->readBoolEntry("GROUP_FLASH_FONT_U", false);
//X 	m_GroupFlashFontI = cfg->readBoolEntry("GROUP_FLASH_FONT_I", false);
//X 
//X 	delete groupSaver;
//X 	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Theme");
//X 	themeDir = cfg->readEntry("THEME_DIR");
//X 	soundDir = cfg->readEntry("SOUND_DIR");
//X 	delete groupSaver;
//X 
//X 		
//X 	autoList.clear();
//X 	quickList.clear();
//X 	groupSaver = new KConfigGroupSaver(cfg, "Reply");
//X 	autoSelectedIndex = cfg->readNumEntry("AUTO_REPLY_SELECTED_INDEX");
//X 	QStringList strList;
//X 	strList = cfg->readListEntry("AUTO_REPLY", '\255');
//X 	if(strList.size()){
//X 		for(QStringList::Iterator iter = strList.begin(); iter != strList.end(); ++iter)
//X 			autoList.push_back(*iter);
//X 	} else {
//X 		autoList.push_back(QString(i18n("Sorry, I am working.")));
//X 		autoList.push_back(QString(i18n("Having dinner now./fa")));
//X 		autoList.push_back(QString(i18n("I am not available now, sorry.")));	
//X 	}
//X 	
//X 	strList.clear();
//X 	strList = cfg->readListEntry("QUICK_REPLY", '\255');
//X 	if(strList.size()){
//X 		for(QStringList::Iterator iter = strList.begin(); iter != strList.end(); ++iter)
//X 			quickList.push_back(*iter);
//X 	} else {
//X 		quickList.push_back(QString(i18n("Oh.")));
//X 		quickList.push_back(QString(i18n("OK, OK, I got you.")));
//X 		quickList.push_back(QString(i18n("/jy , really?")));	
//X 	}
//X 	delete groupSaver;
//X 
//X 	groupSaver = new KConfigGroupSaver(cfg, "Others");
//X 	QSize size(16, 16);
//X 	faceSize = cfg->readSizeEntry("FACE_SIZE", &size);
//X 	pageSize = cfg->readNumEntry("MESSAGE_PAGE_SIZE", 25);
//X 	QString shortkey = cfg->readEntry("MESSAGE_SHORTCUT") ;
//X 	if(shortkey.isEmpty())
//X 		shortcut.init(Qt::CTRL + Qt::ALT + Qt::Key_Z);
//X 	else{
//X 		if(!shortcut.init(shortkey))
//X 			shortcut.init(Qt::CTRL + Qt::ALT + Qt::Key_Z);
//X 	}
//X 	QPoint p(600, 100); 
//X 	m_WinLeftTop = cfg->readPointEntry("WIN_GEOMETRY_POINT", &p);
//X 	QSize winSize(195, 376);
//X 	m_WinSize = cfg->readSizeEntry("WIN_GEOMETRY_SIZE", &winSize);
//X 	idleMaxTime = cfg->readNumEntry("MAX_IDLE_TIME", 5);
//X 	delete groupSaver;
//X 
//X 	m_CachedToBeAddedBuddy.clear();
//X 	groupSaver = new KConfigGroupSaver(cfg, "Cached_ToBeAdded_Buddies");
//X 	int num = cfg->readNumEntry("NUM_BUDDIES", 0);
//X 	QString buddyInfo;
//X 	QStringList items;
//X 	bool ok;
//X 	for(int i = 0; i< num; i++){
//X 		buddyInfo = cfg->readEntry(QString("QQ_%1").arg(i));
//X 		if(!buddyInfo.isEmpty()){
//X 			items = QStringList::split(",", buddyInfo);
//X 			unsigned int id = items[0].toUInt(&ok);
//X 			if(ok)
//X 				m_CachedToBeAddedBuddy[id] = buddyInfo;
//X 		}
//X 	}
//X 	delete groupSaver;
//X 
//X 	m_RejectForever.clear();
//X 	groupSaver = new KConfigGroupSaver(cfg, "Reject_Forever");
//X 	num = cfg->readNumEntry("QQ_REJECT_LIST_SIZE", 0);
//X 	unsigned int id = 0;
//X 	for(int i = 0; i< num; i++){
//X 		id = cfg->readUnsignedNumEntry( QString("QQ_%1").arg(i));
//X 		printf("read id: %d\n", id);
//X 		if(id){
//X 			m_RejectForever.push_back(id);
//X 		}
//X 	}
//X 	delete groupSaver;
//X 
//X 	m_QunRejectForever.clear();
//X 	unsigned int tmpQun, tmpQQ;
//X 	int memberCount;
//X 	groupSaver = new KConfigGroupSaver(cfg, "Qun_Reject_Forever");
//X 	num = cfg->readNumEntry("QUN_REJECT_LIST_SIZE", 0);
//X 	for(int q = 0; q < num; q++){
//X 		tmpQun = cfg->readNumEntry(QString("Qun_%1").arg(q), 0);
//X 		memberCount = cfg->readNumEntry("QUN_REJECT_MEMBER_SIZE", 0);
//X 		std::list<unsigned int> members;
//X 		for(int i = 0; i < memberCount; i++){
//X 			tmpQQ = cfg->readNumEntry( QString("Qun_QQ_%1").arg(i), 0);
//X 			members.push_back(tmpQQ);
//X 		}
//X 		m_QunRejectForever[tmpQun] = members;
//X 	}
//X 	delete groupSaver;
//X 
//X 	//cfg->sync();
//X 	delete cfg;
	return true; 
}

bool EvaUserSetting::loadOldProfile()
{
	if(!isDirExisted(getEvaUserDir())){
		return false;
	}
	
	QString fullName = getEvaUserDir() + "/" + userProfileFileName;
	QDir d;
	if(!d.exists(fullName)){
		loadDefaultSettings();
		return true;
	}
	QFile file(fullName);
	if(!file.open(QIODevice::ReadOnly)){
		return false;
	}
	
	autoList.clear();
	quickList.clear();
	
	QString line;
	QStringList lineList;
	Q3TextStream stream(&file);
	while(!stream.atEnd()){
		line = stream.readLine().stripWhiteSpace();
		lineList = QStringList::split(":", line);
		if(lineList.size() != 2)
			continue;
		
		lineList[0].stripWhiteSpace();
		lineList[1].stripWhiteSpace();
		
		if(lineList[0] ==  "THEME_DIR"){
			themeDir = lineList[1];
		}
		
		if(lineList[0] ==  "SOUND_DIR"){
			soundDir = lineList[1];
		}
		
		if(lineList[0] ==  "FACE_SIZE"){
			QStringList size = QStringList::split(",",lineList[1]);
			int width, height;
			bool ok;
			width = size[0].toInt(&ok);
			if(!ok) width = 16;
			height = size[1].toInt(&ok);
			if(!ok) height = 16;
			faceSize = QSize(width, height);
			continue;
		}
		if(lineList[0] ==  "DISPLAY_QQ_BROADCAST"){
			if(lineList[1] == "false")
				showQQBroadcast = false;
			else 
				showQQBroadcast = true;
			continue;
		}
		if(lineList[0] ==  "DISPLAY_TIP_MESSAGE_TIP_WINDOW"){
			if(lineList[1] == "false")
				showMessageTipWindow = false;
			else 
				showMessageTipWindow = true;
			continue;
		}
		if(lineList[0] ==  "DISPLAY_BUDDY_ONLINE_NOTIFY"){
			if(lineList[1] == "false")
				showBudyOnlineNotifyWindow = false;
			else 
				showBudyOnlineNotifyWindow = true;
			continue;
		}
		if(lineList[0] ==  "DISPLAY_ONLINE_BUDDIES"){
			if(lineList[1] == "false")
				showOnlineUsers = false;
			else 
				showOnlineUsers = true;
			continue;
		}
		if(lineList[0] ==  "SEND_KEY"){
			if(lineList[1] == "false")
				isSendKeyEnter = false;
			else 
				isSendKeyEnter = true;
			continue;
		}
		if(lineList[0] ==  "MESSAGE_SHORTCUT"){
//X 			if(!shortcut.init(lineList[1]))
//X 				shortcut.init(Qt::CTRL + Qt::ALT + Qt::Key_Z);
			continue;
		}
		if(lineList[0] ==  "PLAY_SOUND"){
			if(lineList[1] == "false")
				playSound = false;
			else 
				playSound = true;
			continue;
		}
		if(lineList[0] == "AUTO_REPLY_SELECTED_INDEX"){
			bool ok = false;			
			autoSelectedIndex = lineList[1].toInt(&ok);
			if(!ok)
				autoSelectedIndex = 0;
				
		}
		if(lineList[0] == "AUTO_REPLY"){
			autoList.push_back(lineList[1]);
		}		
		if(lineList[0] == "QUICK_REPLY"){
			quickList.push_back(lineList[1]);
		}
	}
	file.close();
	return true;
}

void EvaUserSetting::loadDefaultSettings()
{
	faceSize = QSize(16,16);
	showQQBroadcast = true;
	m_ShowQQNews = true;
	showMessageTipWindow = true;
	showBudyOnlineNotifyWindow = true;
	showOnlineUsers = false;
	isSendKeyEnter = false;
//X 	shortcut = KShortcut(Qt::CTRL + Qt::ALT + Qt::Key_Z);
	playSound = true;
	pageSize = 25;
	autoReply = true;
	themeDir = "";
	soundDir = "";
	autoSelectedIndex = 0;
	idleMaxTime = 5;

	m_BuddyNameColor = Qt::black;
	m_BuddyNameFontB = m_BuddyNameFontU = m_BuddyNameFontI = false;
	m_BuddySigColor = Qt::gray;
	m_BuddySigFontB = m_BuddySigFontU = m_BuddySigFontI = false;
	m_BuddyFlashColor = Qt::red;
	m_BuddyFlashFontB = m_BuddyFlashFontU = m_BuddyFlashFontI = false;

	m_QunNameColor = Qt::black;
	m_QunNameFontB = m_QunNameFontU = m_QunNameFontI = false;
	m_QunFlashColor = Qt::red;
	m_QunFlashFontB = m_QunFlashFontU = m_QunFlashFontI = false;

	m_GroupNameColor = Qt::black;
	m_GroupNameFontB = m_GroupNameFontU = m_GroupNameFontI = false;
	m_GroupCountColor = Qt::blue;
	m_GroupOnlineCountFontB = m_GroupOnlineCountFontU = m_GroupOnlineCountFontI = false;
	m_GroupFlashColor = Qt::red;
	m_GroupFlashFontB = m_GroupFlashFontU = m_GroupFlashFontI = false;

	m_ShowSmileyInNickName = false;
	m_ShowSignatureInSeperateLine = false;

	if(!autoList.size()){
		autoList.push_back(QString(i18n("Sorry, I am working.")));
		autoList.push_back(QString(i18n("Having dinner now./fa")));
		autoList.push_back(QString(i18n("I am not available now, sorry.")));
	}
	
	if(!quickList.size()){
		quickList.push_back(QString(i18n("Oh.")));
		quickList.push_back(QString(i18n("OK, OK, I got you.")));
		quickList.push_back(QString(i18n("/jy , really?")));
	}
}

const QString &EvaUserSetting::getSelectedAutoReply()
{
	int index = 0;
	std::list<QString>::iterator iter;
	for(iter=autoList.begin(); iter!=autoList.end(); ++iter){
		if(index == autoSelectedIndex) break;
		index++;		
	}
	return *iter;
}

const QString &EvaUserSetting::getQuickReplyMessageAt(const int index)
{
	int i = 0;
	std::list<QString>::iterator iter;
	for(iter=quickList.begin(); iter!=quickList.end(); ++iter){
		if(i == index) break;
		i++;
	}
	return *iter;
}

void EvaUserSetting::addAutoReplyMessage(const QString &message)
{
	autoList.push_back(message);
}

void EvaUserSetting::addQuickReplyMessage(const QString &message)
{
	quickList.push_back(message);
}

const std::list<QString> &EvaUserSetting::getAutoReplyList()
{
	return autoList;
}

const std::list<QString> &EvaUserSetting::getQuickReplyList()
{
	return quickList;
}

const QString EvaUserSetting::getUserHomeDir()
{
	return QDir::homeDirPath();
}

const QString EvaUserSetting::getEvaSettingDir()
{
	return getUserHomeDir()+"/.eva";
}

const QString EvaUserSetting::getEvaUserDir()
{
	return getEvaSettingDir() + "/" + QString::number(qqNum);
}
	
bool EvaUserSetting::isDirExisted(const QString &dir)
{
	QDir d;
	if (d.exists(dir))
		return true;
	else
		return false;
}

bool EvaUserSetting::saveQunList( QObject * receiver, QunList & list )
{
	if(!isDirExisted(getEvaUserDir())){
		QDir d;
		if(!d.mkdir(getEvaUserDir()))
			return false;
	}
	
	QString fullName = getEvaUserDir() + "/" + qunListFileName;
	QFile file(fullName);
	if(file.exists()) file.remove();
	if(!file.open(QIODevice::WriteOnly)){
		return false;
	}
	EvaMain::helper->setCategory(EvaHelper::SaveQunUsers, receiver);
	EvaMain::helper->setSaveQunListArgs(&file, list);
	EvaMain::helper->run();
	return true;
}

bool EvaUserSetting::loadQunList( /*QObject * receiver*/ )
{
	if(!isDirExisted(getEvaUserDir()))
		return false;
	
	QString fullName = getEvaUserDir() + "/" + qunListFileName;
	QFile file(fullName);
	if(!file.open(QIODevice::ReadOnly)){
		return false;
	}
	
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
	
	EvaUser *user = EvaMain::user;
	if(user){
		user->qunList = list;
		return true;
	} else
		return false;
	
	//EvaMain::helper->setCategory(EvaHelper::LoadQunUsers, receiver);
	//EvaMain::helper->setLoadQunListArgs(&file);
	//EvaMain::helper->run();
	//return true;
}

bool EvaUserSetting::createDefaultDirs( )
{
	QDir d;
	if(!isDirExisted(getEvaUserDir())){
		if(!d.mkdir(getEvaUserDir()))
			return false;
	}
	
	if(!isDirExisted(getPictureCacheDir())){
		if(!d.mkdir(getPictureCacheDir()))
			return false;
	}
	
	if(!isDirExisted(getCustomSmileyDir())){
		if(!d.mkdir(getCustomSmileyDir()))
			return false;
	}
	
	if(!isDirExisted(getUserHeadDir())){
		if(!d.mkdir(getUserHeadDir()))
			return false;
	}
	return true;
}

bool EvaUserSetting::isVersionCorrect( const QString fileName )
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

void EvaUserSetting::saveToBeAddedBuddy( BuddyInfoCacheItem info )
{
	QString line = QString("%1,%2,%3,%4").arg(info.id).arg(info.nick).arg(info.face).arg(info.group);
	m_CachedToBeAddedBuddy[info.id] = line;
	
	saveSettings();
}

const BuddyInfoCacheItem EvaUserSetting::removeToBeAddedBuddy( const unsigned int id )
{
	BuddyInfoCacheItem info = getToBeAddedBuddy(id);

	/// remove this item
	std::map<unsigned int, QString>::iterator iter;
	iter = m_CachedToBeAddedBuddy.find(id);
	if(iter == m_CachedToBeAddedBuddy.end())
		return info;
	m_CachedToBeAddedBuddy.erase(iter);

	saveSettings();
	return info;
}

const BuddyInfoCacheItem EvaUserSetting::getToBeAddedBuddy( const unsigned int id )
{
	BuddyInfoCacheItem info(id, QString::number(id), 0, 0);

	std::map<unsigned int, QString>::iterator iter;
	iter = m_CachedToBeAddedBuddy.find(id);
	if(iter == m_CachedToBeAddedBuddy.end())
		return info;
	QStringList items = QStringList::split(",", iter->second);
	bool ok;
	info.id = items[0].toUInt(&ok);
	if( (!ok) || (info.id != id) ){
		info.id = 0;
		return info;
	}
	info.nick = items[1];
	info.face = items[2].toUShort(&ok);
	if(!ok){
		info.face = 0;
		return info;
	}
	info.group = items[3].toInt(&ok);
	if(!ok){
		info.group = 0;
		return info;
	}

	return info;
}

void EvaUserSetting::addToRejectForever( const unsigned int id )
{
	std::list<unsigned int>::iterator iter;

	for(iter = m_RejectForever.begin(); iter != m_RejectForever.end(); ++iter){
		if( *iter == id) return;
	}
	m_RejectForever.push_back(id);
}

bool EvaUserSetting::removeFromRejectForever( const unsigned int id )
{
	std::list<unsigned int>::iterator iter;

	for(iter = m_RejectForever.begin(); iter != m_RejectForever.end(); ++iter){
		if( *iter == id){
			m_RejectForever.erase(iter);
			return true;
		}
	}
	return false;
}

bool EvaUserSetting::isInRejectForever( const unsigned int id )
{
	std::list<unsigned int>::iterator iter;

	for(iter = m_RejectForever.begin(); iter != m_RejectForever.end(); ++iter){
		if( *iter == id) return true;
	}
	return false;
}

void EvaUserSetting::addToQunRejectForever( const unsigned int id, const unsigned int qq )
{
	std::map<unsigned int, std::list<unsigned int> >::iterator iter = m_QunRejectForever.find(id);

	std::list<unsigned int> list;
	if(iter != m_QunRejectForever.end()){
		std::list<unsigned int>::iterator itr;
		list = iter->second;
		for(itr = list.begin(); itr!= list.end(); ++itr){
			if(*itr == qq) return;
		}
		list.push_back(qq);
	} else {
		list.push_back(qq);
	}
	m_QunRejectForever[id] = list;
}

bool EvaUserSetting::removeFromQunRejectForever( const unsigned int id, const unsigned int qq )
{
	std::map<unsigned int, std::list<unsigned int> >::iterator iter = m_QunRejectForever.find(id);

	std::list<unsigned int> list;
	if(iter != m_QunRejectForever.end()){
		if(qq){
			std::list<unsigned int>::iterator itr;
			list = iter->second;
			for(itr = list.begin(); itr!= list.end(); ++itr){
				if(*itr == qq) {
					list.erase(itr);
					if(list.size())
						m_QunRejectForever[id] = list;
					else
						m_QunRejectForever.erase(iter);
					return true;
				}
			}
		}else // remove all members of the Qun(id)
			m_QunRejectForever.erase(iter);
	}
	return false;;
}

bool EvaUserSetting::isInQunRejectForever( const unsigned int id, const unsigned int qq )
{
	std::map<unsigned int, std::list<unsigned int> >::iterator iter = m_QunRejectForever.find(id);

	std::list<unsigned int> list;
	if(iter != m_QunRejectForever.end()){
		std::list<unsigned int>::iterator itr;
		list = iter->second;
		for(itr = list.begin(); itr!= list.end(); ++itr){
			if(*itr == qq) return true;
		}
	}
	return false;	
}

void EvaUserSetting::updateRecentContact( std::list<RecentContact>& list )
{
	QStringList s;
	std::list<RecentContact>::iterator it;
	QString c;
	for(it = list.begin(); it!=list.end(); ++it){
		c = (it->isQun)?"Qun_":"QQ_";
		c += QString::number(it->id);
		c += "_";
		c += QString::number(it->time);
		s.append(c);
	}

	unsigned int size = recentContactListSize();
	if(size < list.size()){
//X 		kdDebug() << "[EvaUserSetting] recent contact list is out of size limit" << endl;
		return;
	}

//X 	KConfig *cfg = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
//X 	
//X 	KConfigGroupSaver *groupSaver = new KConfigGroupSaver(cfg, "Recent_Contacts");
//X 	cfg->writeEntry("RECENT_LIST", s, '\255');
//X 	delete groupSaver;
//X 	delete cfg;	
}

std::list< RecentContact > EvaUserSetting::getRecentContacts( )
{
//X 	KConfig *cfg = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
//X 	
//X 	KConfigGroupSaver *groupSaver = new KConfigGroupSaver(cfg, "Recent_Contacts");
//X 	QStringList s = cfg->readListEntry( "RECENT_LIST", '\255');
	std::list< RecentContact > list;
//X 	RecentContact contact;
//X 	printf("recent size: %d\n", s.size());
//X 	if(s.size()){
//X 		for(QStringList::Iterator iter = s.begin(); iter != s.end(); ++iter){
//X 			QString line = (*iter);
//X 			QStringList parts = QStringList::split("_", line);
//X 			if(parts.size() != 3) continue;
//X 			
//X 			if(parts[0] == "QQ")
//X 				contact.isQun = false;
//X 			else
//X 				contact.isQun = true;
//X 
//X 			bool ok;
//X 			contact.id = parts[1].toUInt(&ok);
//X 			if(!ok) continue;
//X 			contact.time = parts[2].toUInt(&ok);
//X 			if(ok)
//X 				list.push_back(contact);
//X 		}
//X 	}
//X 	cfg->sync();
//X 	delete groupSaver;
//X 	delete cfg;

	return list;
}

void EvaUserSetting::setRecentContactListSize( const int /*size*/ )
{
//X 	KConfig *cfg = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
//X 	
//X 	KConfigGroupSaver *groupSaver = new KConfigGroupSaver(cfg, "Recent_Contacts");
//X 	cfg->writeEntry("RECENT_MAX_SIZE", size);
//X 	cfg->sync();
//X 	delete groupSaver;
//X 	delete cfg;
}

int EvaUserSetting::recentContactListSize( )
{
//X 	KConfig *cfg = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
//X 	
//X 	KConfigGroupSaver *groupSaver = new KConfigGroupSaver(cfg, "Recent_Contacts");
//X 	int size = cfg->readNumEntry("RECENT_MAX_SIZE", 10);
//X 	delete groupSaver;
//X 	delete cfg;
//X 	return size;
        return 0;
}

KConfig *EvaUserSetting::config( const QString &/*group*/ )
{
    return NULL;
	//printf("EvaUserSetting::config: %s\n", group.local8Bit().data());
//X 	m_config->setGroup(group);
//X 	return m_config;
}

