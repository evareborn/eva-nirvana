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
#include "defines.h"
#include <vector>
#include <stdlib.h>
#include <qdatastream.h>
#include <qstringlist.h>
#include <qfile.h>
#include <qdir.h>
#include <QSettings>
#include <Q3TextStream>
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
	
	m_NeedRepaint = true;
        loadSettings();
}

EvaUserSetting::~EvaUserSetting()
{
    saveSettings();
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
        QSettings * cfg = new QSettings( getEvaUserDir() + "/" + userConfigFileName, QSettings::IniFormat);
        cfg->beginGroup("General" );
	cfg->setValue("DISPLAY_QQ_BROADCAST", showQQBroadcast);
	cfg->setValue("DISPLAY_QQ_NEWS", m_ShowQQNews);
	cfg->setValue("DISPLAY_TIP_MESSAGE_TIP_WINDOW", showMessageTipWindow);
	cfg->setValue("DISPLAY_BUDDY_ONLINE_NOTIFY", showBudyOnlineNotifyWindow);
	cfg->setValue("DISPLAY_ONLINE_BUDDIES", showOnlineUsers);
	cfg->setValue("SEND_KEY", isSendKeyEnter);
	cfg->setValue("PLAY_SOUND", playSound);
	cfg->setValue("SHOW_SMILEY_IN_NICK", m_ShowSmileyInNickName);
	cfg->setValue("SHOW_SIG_IN_SEPERATE_LINE", m_ShowSignatureInSeperateLine);
        cfg->endGroup( );
//X 	delete groupSaver;
//X 	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Font");
        cfg->beginGroup("Font" );
	cfg->setValue("BUDDY_NAME_COLOR", m_BuddyNameColor);
	cfg->setValue("BUDDY_NAME_FONT_B", m_BuddyNameFontB);
	cfg->setValue("BUDDY_NAME_FONT_U", m_BuddyNameFontU);
	cfg->setValue("BUDDY_NAME_FONT_I", m_BuddyNameFontI);

	cfg->setValue("BUDDY_SIGNATURE_COLOR", m_BuddySigColor);
	cfg->setValue("BUDDY_SIGNATURE_FONT_B", m_BuddySigFontB);
	cfg->setValue("BUDDY_SIGNATURE_FONT_U", m_BuddySigFontU);
	cfg->setValue("BUDDY_SIGNATURE_FONT_I", m_BuddySigFontI);

	cfg->setValue("BUDDY_FLASH_COLOR", m_BuddyFlashColor);
	cfg->setValue("BUDDY_FLASH_FONT_B", m_BuddyFlashFontB);
	cfg->setValue("BUDDY_FLASH_FONT_U", m_BuddyFlashFontU);
	cfg->setValue("BUDDY_FLASH_FONT_I", m_BuddyFlashFontI);

	// save Qun font settings
	cfg->setValue("QUN_NAME_COLOR", m_QunNameColor);
	cfg->setValue("QUN_NAME_FONT_B", m_QunNameFontB);
	cfg->setValue("QUN_NAME_FONT_U", m_QunNameFontU);
	cfg->setValue("QUN_NAME_FONT_I", m_QunNameFontI);

	cfg->setValue("QUN_FLASH_COLOR", m_QunFlashColor);
	cfg->setValue("QUN_FLASH_FONT_B", m_QunFlashFontB);
	cfg->setValue("QUN_FLASH_FONT_U", m_QunFlashFontU);
	cfg->setValue("QUN_FLASH_FONT_I", m_QunFlashFontI);

	// save group font settings
	cfg->setValue("GROUP_NAME_COLOR", m_GroupNameColor);
	cfg->setValue("GROUP_NAME_FONT_B", m_GroupNameFontB);
	cfg->setValue("GROUP_NAME_FONT_U", m_GroupNameFontU);
	cfg->setValue("GROUP_NAME_FONT_I", m_GroupNameFontI);

	cfg->setValue("GROUP_ONLINE_COUNT_COLOR", m_GroupCountColor);
	cfg->setValue("GROUP_ONLINE_COUNT_FONT_B", m_GroupOnlineCountFontB);
	cfg->setValue("GROUP_ONLINE_COUNT_FONT_U", m_GroupOnlineCountFontU);
	cfg->setValue("GROUP_ONLINE_COUNT_FONT_I", m_GroupOnlineCountFontI);

	cfg->setValue("GROUP_FLASH_COLOR", m_GroupFlashColor);
	cfg->setValue("GROUP_FLASH_FONT_B", m_GroupFlashFontB);
	cfg->setValue("GROUP_FLASH_FONT_U", m_GroupFlashFontU);
	cfg->setValue("GROUP_FLASH_FONT_I", m_GroupFlashFontI);
        cfg->endGroup( );

//X 	delete groupSaver;
//X 	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Theme");
        cfg->beginGroup("Theme" );
	cfg->setValue("THEME_DIR", themeDir);
	cfg->setValue("SOUND_DIR", soundDir);
        cfg->endGroup( );
//X 	delete groupSaver;
//X 	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Reply");
        cfg->beginGroup("Reply" );
	cfg->setValue("AUTO_REPLY_SELECTED_INDEX", autoSelectedIndex);
	std::list<QString>::iterator iter;
	QStringList strList;
	for(iter = autoList.begin(); iter!=autoList.end(); ++iter){
		strList += (*iter);
	}
	cfg->writeEntry("AUTO_REPLY", strList, '\255');
	strList.clear();
	for(iter = quickList.begin(); iter!=quickList.end(); ++iter){
		strList += (*iter);
	}
	cfg->writeEntry("QUICK_REPLY", strList, '\255');	
        cfg->endGroup( );
//X 	delete groupSaver;
//X 
//X 	groupSaver = new KConfigGroupSaver(cfg, "Others");
        cfg->beginGroup("Others" );
	cfg->setValue("FACE_SIZE", faceSize);
	cfg->setValue("MESSAGE_PAGE_SIZE", pageSize);
	cfg->setValue("MESSAGE_SHORTCUT", keysequence.toString());
	cfg->setValue("WIN_GEOMETRY_POINT", m_WinLeftTop);
	cfg->setValue("WIN_GEOMETRY_SIZE", m_WinSize);
	cfg->setValue("MAX_IDLE_TIME", idleMaxTime);
        cfg->endGroup( );
//X 	delete groupSaver;
//X 	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Cached_ToBeAdded_Buddies");
        cfg->beginGroup("Cached_ToBeAdded_Buddies" );
	cfg->setValue("NUM_BUDDIES", m_CachedToBeAddedBuddy.size());
	std::map<unsigned int, QString>::iterator itr;
	int num = 0;
	for(itr = m_CachedToBeAddedBuddy.begin(); itr!=m_CachedToBeAddedBuddy.end(); ++itr){
		cfg->setValue(QString("QQ_%1").arg(num++), itr->second);
	}	
        cfg->endGroup( );
//X 	delete groupSaver;
//X 
//X 	groupSaver = new KConfigGroupSaver(cfg, "Reject_Forever");
        cfg->beginGroup("Reject_Forever" );
	cfg->setValue("QQ_REJECT_LIST_SIZE", m_RejectForever.size());
	std::list<unsigned int>::iterator rejectIter;
	num = 0;
	for(rejectIter = m_RejectForever.begin(); rejectIter!=m_RejectForever.end(); ++rejectIter){
		cfg->setValue(QString("QQ_%1").arg(num++), (unsigned int)(*rejectIter));
	}
        cfg->endGroup( );
//X 	delete groupSaver;
//X 
//X 	groupSaver = new KConfigGroupSaver(cfg, "Qun_Reject_Forever");
        cfg->beginGroup("Qun_Reject_Forever" );
	cfg->setValue("QUN_REJECT_LIST_SIZE", m_QunRejectForever.size());
	std::map<unsigned int, std::list<unsigned int> >::iterator qunRejectIter;
	num = 0;
	for(qunRejectIter = m_QunRejectForever.begin(); qunRejectIter!=m_QunRejectForever.end(); ++qunRejectIter){
		cfg->setValue(QString("Qun_%1").arg(num++), qunRejectIter->first);
		cfg->setValue("QUN_REJECT_MEMBER_SIZE", (qunRejectIter->second).size());
		int memberCount = 0;
		for(rejectIter = qunRejectIter->second.begin(); rejectIter!= qunRejectIter->second.end(); ++rejectIter){
			cfg->setValue(QString("Qun_QQ_%1").arg(memberCount++), (unsigned int)(*rejectIter));
		}
	}
        cfg->endGroup( );
//X 	delete groupSaver;
//X 
	cfg->sync();
	delete cfg;
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
	} else {
            if (!d.exists( getEvaUserDir() + "/" + userConfigFileName ) ) {
                loadDefaultSettings();
                saveSettings();
            }
        }
 
	
//X 	KConfig *cfg = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
	
//X 	KConfigGroupSaver *groupSaver = new KConfigGroupSaver(cfg, "General");
        QSettings * cfg = new QSettings( getEvaUserDir() + "/" + userConfigFileName, QSettings::IniFormat);
        cfg->beginGroup("General" );
	showQQBroadcast = cfg->readBoolEntry("DISPLAY_QQ_BROADCAST", true);
	m_ShowQQNews = cfg->readBoolEntry("DISPLAY_QQ_NEWS", true);
	showMessageTipWindow = cfg->readBoolEntry("DISPLAY_TIP_MESSAGE_TIP_WINDOW", true);
	showBudyOnlineNotifyWindow = cfg->readBoolEntry("DISPLAY_BUDDY_ONLINE_NOTIFY", true);
	showOnlineUsers = cfg->readBoolEntry("DISPLAY_ONLINE_BUDDIES", false);
	isSendKeyEnter = cfg->readBoolEntry("SEND_KEY", false);
	playSound = cfg->readBoolEntry("PLAY_SOUND", true);
	m_ShowSmileyInNickName = cfg->readBoolEntry("SHOW_SMILEY_IN_NICK", false);
	m_ShowSignatureInSeperateLine = cfg->readBoolEntry("SHOW_SIG_IN_SEPERATE_LINE", false);
        cfg->endGroup( );
//X 	delete groupSaver;
	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Font");
        cfg->beginGroup("Font" );
	m_BuddyNameColor = cfg->value("BUDDY_NAME_COLOR").value<QColor>();
	m_BuddyNameFontB = cfg->readBoolEntry("BUDDY_NAME_FONT_B", false);
	m_BuddyNameFontU = cfg->readBoolEntry("BUDDY_NAME_FONT_U", false);
	m_BuddyNameFontI = cfg->readBoolEntry("BUDDY_NAME_FONT_I", false);

	m_BuddySigColor = cfg->value("BUDDY_SIGNATURE_COLOR").value<QColor>();
	m_BuddySigFontB = cfg->readBoolEntry("BUDDY_SIGNATURE_FONT_B", false);
	m_BuddySigFontU = cfg->readBoolEntry("BUDDY_SIGNATURE_FONT_U", false);
	m_BuddySigFontI = cfg->readBoolEntry("BUDDY_SIGNATURE_FONT_I", false);

	m_BuddyFlashColor = cfg->value("BUDDY_FLASH_COLOR").value<QColor>();
	m_BuddyFlashFontB = cfg->readBoolEntry("BUDDY_FLASH_FONT_B", false);
	m_BuddyFlashFontU = cfg->readBoolEntry("BUDDY_FLASH_FONT_U", false);
	m_BuddyFlashFontI = cfg->readBoolEntry("BUDDY_FLASH_FONT_I", false);

	m_QunNameColor = cfg->value("QUN_NAME_COLOR").value<QColor>();
	m_QunNameFontB = cfg->readBoolEntry("QUN_NAME_FONT_B", false);
	m_QunNameFontU = cfg->readBoolEntry("QUN_NAME_FONT_U", false);
	m_QunNameFontI = cfg->readBoolEntry("QUN_NAME_FONT_I", false);

	m_QunFlashColor = cfg->value("QUN_FLASH_COLOR").value<QColor>();
	m_QunFlashFontB = cfg->readBoolEntry("QUN_FLASH_FONT_B", false);
	m_QunFlashFontU = cfg->readBoolEntry("QUN_FLASH_FONT_U", false);
	m_QunFlashFontI = cfg->readBoolEntry("QUN_FLASH_FONT_I", false);

	m_GroupNameColor = cfg->value("GROUP_NAME_COLOR").value<QColor>();
	m_GroupNameFontB = cfg->readBoolEntry("GROUP_NAME_FONT_B", false);
	m_GroupNameFontU = cfg->readBoolEntry("GROUP_NAME_FONT_U", false);
	m_GroupNameFontI = cfg->readBoolEntry("GROUP_NAME_FONT_I", false);

	m_GroupCountColor = cfg->value("GROUP_ONLINE_COUNT_COLOR").value<QColor>();
	m_GroupOnlineCountFontB = cfg->readBoolEntry("GROUP_ONLINE_COUNT_FONT_B", false);
	m_GroupOnlineCountFontU = cfg->readBoolEntry("GROUP_ONLINE_COUNT_FONT_U", false);
	m_GroupOnlineCountFontI = cfg->readBoolEntry("GROUP_ONLINE_COUNT_FONT_I", false);

	m_GroupFlashColor = cfg->value("GROUP_FLASH_COLOR").value<QColor>();
	m_GroupFlashFontB = cfg->readBoolEntry("GROUP_FLASH_FONT_B", false);
	m_GroupFlashFontU = cfg->readBoolEntry("GROUP_FLASH_FONT_U", false);
	m_GroupFlashFontI = cfg->readBoolEntry("GROUP_FLASH_FONT_I", false);
        cfg->endGroup( );

//X 	delete groupSaver;
	
//X 	groupSaver = new KConfigGroupSaver(cfg, "Theme");
        cfg->beginGroup("Theme" );
	themeDir = cfg->readEntry("THEME_DIR");
	soundDir = cfg->readEntry("SOUND_DIR");
        cfg->endGroup( );
//X 	delete groupSaver;

		
	autoList.clear();
	quickList.clear();
 
        cfg->beginGroup("Reply" );
//X 	groupSaver = new KConfigGroupSaver(cfg, "Reply");
	autoSelectedIndex = cfg->readNumEntry("AUTO_REPLY_SELECTED_INDEX");
	QStringList strList;
	strList = cfg->readListEntry("AUTO_REPLY", '\255');
	if(strList.size()){
		for(QStringList::Iterator iter = strList.begin(); iter != strList.end(); ++iter)
			autoList.push_back(*iter);
	} else {
		autoList.push_back(QString(i18n("Sorry, I am working.")));
		autoList.push_back(QString(i18n("Having dinner now./fa")));
		autoList.push_back(QString(i18n("I am not available now, sorry.")));	
	}
	
	strList.clear();
	strList = cfg->readListEntry("QUICK_REPLY", '\255');
	if(strList.size()){
		for(QStringList::Iterator iter = strList.begin(); iter != strList.end(); ++iter)
			quickList.push_back(*iter);
	} else {
		quickList.push_back(QString(i18n("Oh.")));
		quickList.push_back(QString(i18n("OK, OK, I got you.")));
		quickList.push_back(QString(i18n("/jy , really?")));	
	}
        cfg->endGroup( );
//X 	delete groupSaver;
//X 
//X 	groupSaver = new KConfigGroupSaver(cfg, "Others");
        cfg->beginGroup("Others" );
	QSize size(16, 16);
	faceSize = cfg->value("FACE_SIZE").value<QSize>();
        printf( "FACE_SIZE: %d, %d \n",faceSize.height( ), faceSize.width( ) );
	pageSize = cfg->readNumEntry("MESSAGE_PAGE_SIZE", 25);
	QString shortkey = cfg->readEntry("MESSAGE_SHORTCUT") ;
	if(shortkey.isEmpty())
		keysequence = QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_Z);
	else{
		if(!( keysequence.toString() == shortkey) )
			keysequence == QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_Z);
	}
	QPoint p(600, 100); 
	m_WinLeftTop = cfg->value("WIN_GEOMETRY_POINT").value<QPoint>();
	QSize winSize(195, 376);
	m_WinSize = cfg->value("WIN_GEOMETRY_SIZE").value<QSize>();
	idleMaxTime = cfg->readNumEntry("MAX_IDLE_TIME", 5);
        cfg->endGroup( );
//X 	delete groupSaver;

	m_CachedToBeAddedBuddy.clear();
        cfg->beginGroup("Cached_ToBeAdded_Buddies" );

//X 	groupSaver = new KConfigGroupSaver(cfg, "Cached_ToBeAdded_Buddies");
	int num = cfg->readNumEntry("NUM_BUDDIES", 0);
	QString buddyInfo;
	QStringList items;
	bool ok;
	for(int i = 0; i< num; i++){
		buddyInfo = cfg->readEntry(QString("QQ_%1").arg(i));
		if(!buddyInfo.isEmpty()){
			items = QStringList::split(",", buddyInfo);
			unsigned int id = items[0].toUInt(&ok);
			if(ok)
				m_CachedToBeAddedBuddy[id] = buddyInfo;
		}
	}
        cfg->endGroup( );
//X 	delete groupSaver;

	m_RejectForever.clear();
        cfg->beginGroup("Reject_Forever" );
//X 	groupSaver = new KConfigGroupSaver(cfg, "Reject_Forever");
	num = cfg->readNumEntry("QQ_REJECT_LIST_SIZE", 0);
	unsigned int id = 0;
	for(int i = 0; i< num; i++){
		id = cfg->value( QString("QQ_%1").arg(i)).value<unsigned int>();
		printf("read id: %d\n", id);
		if(id){
			m_RejectForever.push_back(id);
		}
	}
        cfg->endGroup( );

//X 	delete groupSaver;

	m_QunRejectForever.clear();
	unsigned int tmpQun, tmpQQ;
	int memberCount;
        cfg->beginGroup("Qun_Reject_Forever" );
//X 	groupSaver = new KConfigGroupSaver(cfg, "Qun_Reject_Forever");
	num = cfg->readNumEntry("QUN_REJECT_LIST_SIZE", 0);
	for(int q = 0; q < num; q++){
		tmpQun = cfg->readNumEntry(QString("Qun_%1").arg(q), 0);
		memberCount = cfg->readNumEntry("QUN_REJECT_MEMBER_SIZE", 0);
		std::list<unsigned int> members;
		for(int i = 0; i < memberCount; i++){
			tmpQQ = cfg->readNumEntry( QString("Qun_QQ_%1").arg(i), 0);
			members.push_back(tmpQQ);
		}
		m_QunRejectForever[tmpQun] = members;
	}
        cfg->endGroup( );
//X 	delete groupSaver;

	//cfg->sync();
	delete cfg;
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
//X 			if(!keysequence.init(lineList[1]))
//X 				keysequence.init(Qt::CTRL + Qt::ALT + Qt::Key_Z);
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
	keysequence = QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_Z);
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

QString EvaUserSetting::getUserHomeDir()
{
	return QDir::homeDirPath();
}

QString EvaUserSetting::getEvaSettingDir()
{
	return getUserHomeDir()+"/.eva";
}

QString EvaUserSetting::getEvaUserDir()
{
	return getEvaSettingDir() + "/" + QString::number(qqNum);
}
 
QString EvaUserSetting::getEvaBuddyListFilename()
{
    return getEvaUserDir() + "/" + buddyListFileName;
}

QString EvaUserSetting::getEvaQunListFilename()
{
    return getEvaUserDir() + "/" + qunListFileName;
}

bool EvaUserSetting::isDirExisted(const QString &dir)
{
	QDir d;
	if (d.exists(dir))
		return true;
	else
		return false;
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
        QSettings *cfg = new QSettings( getEvaUserDir() + "/" + userConfigFileName, QSettings::IniFormat );
        cfg->beginGroup( "Recent_Contacts" );
	cfg->writeEntry("RECENT_LIST", s, '\255');
        cfg->endGroup();
//X 	delete groupSaver;
	delete cfg;	
}

std::list< RecentContact > EvaUserSetting::getRecentContacts( )
{
//X 	KConfig *cfg = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
//X 	
//X 	KConfigGroupSaver *groupSaver = new KConfigGroupSaver(cfg, "Recent_Contacts");
        QSettings *cfg = new QSettings( getEvaUserDir() + "/" + userConfigFileName, QSettings::IniFormat );
        cfg->beginGroup("Recent_Contacts" );
	QStringList s = cfg->readListEntry( "RECENT_LIST", '\255');
	std::list< RecentContact > list;
	RecentContact contact;
	printf("recent size: %d\n", s.size());
	if(s.size()){
		for(QStringList::Iterator iter = s.begin(); iter != s.end(); ++iter){
			QString line = (*iter);
			QStringList parts = QStringList::split("_", line);
			if(parts.size() != 3) continue;
			
			if(parts[0] == "QQ")
				contact.isQun = false;
			else
				contact.isQun = true;

			bool ok;
			contact.id = parts[1].toUInt(&ok);
			if(!ok) continue;
			contact.time = parts[2].toUInt(&ok);
			if(ok)
				list.push_back(contact);
		}
	}
        cfg->endGroup( );
	cfg->sync();
//X 	delete groupSaver;
	delete cfg;

	return list;
}

void EvaUserSetting::setRecentContactListSize( const int size )
{
//X 	KConfig *cfg = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
//X 	
//X 	KConfigGroupSaver *groupSaver = new KConfigGroupSaver(cfg, "Recent_Contacts");
        QSettings *cfg = new QSettings( getEvaUserDir() + "/" + userConfigFileName, QSettings::IniFormat );
        cfg->beginGroup("Recent_Contacts" );
	cfg->setValue("RECENT_MAX_SIZE", size);
        cfg->endGroup( );
	cfg->sync();
//X 	delete groupSaver;
	delete cfg;
}

int EvaUserSetting::recentContactListSize( )
{
//X 	KConfig *cfg = new KConfig(getEvaUserDir() + "/" + userConfigFileName);
//X 	
//X 	KConfigGroupSaver *groupSaver = new KConfigGroupSaver(cfg, "Recent_Contacts");
        QSettings *cfg = new QSettings( getEvaUserDir() + "/" + userConfigFileName, QSettings::IniFormat );
        cfg->beginGroup("Recent_Contacts" );
	int size = cfg->readNumEntry("RECENT_MAX_SIZE", 10);
        cfg->endGroup( );
//X 	delete groupSaver;
	delete cfg;
	return size;
        return 0;
}
//X 
//X KConfig *EvaUserSetting::config( const QString &/*group*/ )
//X {
//X     return NULL;
//X 	//printf("EvaUserSetting::config: %s\n", group.local8Bit().data());
//X //X 	m_config->setGroup(group);
//X //X 	return m_config;
//X }

