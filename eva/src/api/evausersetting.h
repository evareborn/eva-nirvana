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
#ifndef EVAUSERSETTING_H
#define EVAUSERSETTING_H
 
#include "evafriendlist.h"
#include "evauserinfo.h"
#include "evaqunlist.h"
#include "evauser.h"
#include <list>
#include <string>
#include <stack>
#include <inttypes.h>
#include <qdatetime.h>
#include <qpoint.h>
#include <qsize.h>
#include <qcolor.h>
#include <kshortcut.h>

class KConfig;

typedef struct BuddyInfoCacheItem{
	unsigned int id;
	QString nick;
	unsigned short face;
	int group;
	BuddyInfoCacheItem(const unsigned int qq, const QString &n,
			const unsigned short f, const int g) :
		id(qq),nick(n),face(f),group(g) { };
} BuddyInfoCacheItem;

typedef struct RecentContact{
	unsigned int id;
	bool isQun;
	unsigned int time;
} RecentContact;

// this class processes saving & loading buddy list, message recording & user settings
class EvaUserSetting
{
public:
	EvaUserSetting(const int id);
	~EvaUserSetting();
	typedef struct{
	Q_UINT32 sender;
	QString sNick;
	Q_UINT32 receiver;
	QString rNick;
	Q_UINT8  type; // 0 auto reply,  1 normal
	QString  message;
	QDateTime time;
	Q_UINT8   fontSize;
	Q_UINT8   flag; // start from right.  bit 0: u, bit 1: i, bit 2: b
	Q_UINT8   blue;
	Q_UINT8   green;
	Q_UINT8   red;
	} chatMessage;
	
	class sysMessage {
	public:
		Q_UINT16  messageType;
		Q_UINT8  type;
		Q_UINT32 from;
		Q_UINT32 to;
		Q_UINT32 commander; // for Qun sys message
		Q_UINT32 internalQunID;
		QString  message;
		Q_UINT16 codeLen;
		unsigned char *code;
		Q_UINT16 tokenLen;
		unsigned char *token;
	
		sysMessage(): codeLen(0), code(0), tokenLen(0), token(0){
		}
	
		sysMessage(const sysMessage &rhs): codeLen(0), code(0), tokenLen(0), token(0){
			*this = rhs;
		}

		sysMessage &operator=(const sysMessage &rhs){
			messageType = rhs.messageType;
			type = rhs.type;
			from = rhs.from;
			to = rhs.to;
			commander = rhs.commander;
			internalQunID = rhs.internalQunID;
			message = rhs.message;
			setCode(rhs.code, rhs.codeLen);
			setToken(rhs.token, rhs.tokenLen);
			return *this;
		}

		~sysMessage(){
			if(code) delete []code;
			if(token) delete []token;
		}
		void setCode(const unsigned char *c, const unsigned short len){
			if(c || len){
				if(code) delete []code;
				codeLen = len;
				code = new unsigned char[codeLen];
				memcpy(code, c, codeLen);
			}
		}
		void setToken(const unsigned char *t, const unsigned short len){
			if(t || len){
				if(token) delete []token;
				tokenLen = len;
				token = new unsigned char[tokenLen];
				memcpy(token, t, tokenLen);
			}
		}
	};
	
	// save Qun information method	
	const bool saveQunList(QObject *receiver, QunList &list);
	const bool loadQunList(/*QObject *receiver*/);
	
	// the group names include user creating ones and the first one "buddy list"
	const bool saveBuddyList(QObject *receiver, std::list<std::string> groups, ContactInfo &myInfo, FriendList &list,
				unsigned short extraInfo, std::string sig, unsigned int sigTime);
	const bool loadBuddyList(/*QObject * receiver*/);
	
	const bool saveMessage(const int buddy, const int sender, QString sNick, 
			const int receiver, QString rNick,
			const bool isNormal, 
			const QString message, 
			const QDateTime time, const char fontSize, 
			const bool u, const bool i, const bool b, 
			const char blue, const char green, const char red, const bool isQunMsg = false);
	
	std::list<chatMessage> getMessages( const int buddy, const int page, const bool isQunMsg = false); // page number starts from 0
	std::list<chatMessage> getMessages(const int buddy, const int page, const QDateTime starttime, const QDateTime endtime, 
						const bool isQunMsg = false);
	
	const bool saveSysMessage(const short msgType, const unsigned char type, const int fromQQ, const int toQQ, 
				const QString message, const int internalQunID = 0, const unsigned int commander = 0,
					const unsigned char *code = 0, const unsigned short codeLen = 0,
					const unsigned char *token = 0, const unsigned short tokenLen = 0);
	std::list<sysMessage> getSysMessages( const int page); // page number starts from 0
	sysMessage getLastSysMessage();
	
	const bool saveSettings();
	const bool loadSettings();
	void loadDefaultSettings();
	
	void setThemeDir(const QString &dir) { themeDir = dir; }
	void setSoundDir(const QString &dir) { soundDir = dir; }
	void setPageSize(const int size) { pageSize = size; }
	void setFaceSize(const QSize &size) { faceSize = size; }
	void setShowSystemBroadcastEnabled(const bool show) { showQQBroadcast = show; }
	void setShowSystemNewsEnabled(const bool show) { m_ShowQQNews = show; }
	void setShowMessageTipEnabled( const bool show) { showMessageTipWindow = show; }
	void setShowBudyOnlineNotifyEnabled( const bool show) { showBudyOnlineNotifyWindow = show; }
	void setShowOnlineEnabled( const bool show) { showOnlineUsers = show; }
	void setSendKeyEnterEnabled(const bool ok) { isSendKeyEnter = ok; }
	void setSoundEnabled(const bool play) { playSound = play; }
	void setAutoReplyEnabled(const bool reply) { autoReply = reply; }
	void setShowSmileyInNickName( const bool show ) { m_ShowSmileyInNickName = show; }
	void setShowSignatureInSeperateLine( const bool show) { m_ShowSignatureInSeperateLine = show; }
	
	const QString &getThemeDir() const { return themeDir; }
	const QString &getSoundDir() const { return soundDir; }
	const int getPageSize() const { return pageSize; }
	const QSize &getFaceSize() const { return faceSize; }
	const bool isShowSystemBroadcastEnabled() const { return showQQBroadcast; }
	const bool isShowSystemNewsEnabled() const { return m_ShowQQNews; }

	const bool isShowMessageTipEnabled() const { return showMessageTipWindow;}
	const bool isShowBudyOnlineNotifyEnabled() const { return showBudyOnlineNotifyWindow;}
	const bool isShowOnlineEnabled() const { return showOnlineUsers;}
	const bool isSendKeyEnterEnabled() const { return isSendKeyEnter; }
	const bool isSoundEnabled() const { return playSound;}
	const bool isAutoReplyEnabled() const { return autoReply; }
	const bool isShowSmileyInNickName() const { return m_ShowSmileyInNickName; }
	const bool isShowSignatureInSeperateLine() const { return m_ShowSignatureInSeperateLine; }
	
	const int getAutoReplySelectedIndex() const { return autoSelectedIndex; }
	const QString &getSelectedAutoReply();
	const QString &getQuickReplyMessageAt(const int index);
	void addAutoReplyMessage(const QString &message);
	void addQuickReplyMessage(const QString &message);
	
	void setAutoReplySelectedIndex(const int index) { autoSelectedIndex = index; }
	void setAutoReplyList(std::list<QString> list) { autoList = list;}
	void setQuickReplyList(std::list<QString> list) { quickList = list;}
	const std::list<QString> &getAutoReplyList();
	const std::list<QString> &getQuickReplyList();

	const QString getPictureCacheDir() const { return pictureCacheDir; }
	void setPictureCacheDir(const QString path) { pictureCacheDir = path; }

	const QString getCustomSmileyDir() const { return customSmileyDir; }
	void setCustomSmileyDir(const QString path) { customSmileyDir = path; }

	const QString getUserHeadDir() const { return userHeadDir; }
	void setUserHeadDir(const QString path) { userHeadDir = path; }	
	
	const KShortcut &getMessageShortcut() const { return shortcut; }
	void setMessageShortcut( const KShortcut &key) { shortcut = key; }
	
	static bool isVersionCorrect(const QString fileName);
	
	//set Buddy font settings
	void setBuddyNameColor( const QColor &color) { m_BuddyNameColor = color; }
	void setBuddyNameBold(const bool b) { m_BuddyNameFontB  = b; }
	void setBuddyNameUnderline(const bool b) { m_BuddyNameFontU  = b; }
	void setBuddyNameItalic(const bool b) { m_BuddyNameFontI  = b; }

	void setBuddySigColor( const QColor &color) { m_BuddySigColor = color; }
	void setBuddySigBold(const bool b) { m_BuddySigFontB  = b; }
	void setBuddySigUnderline(const bool b) { m_BuddySigFontU  = b; }
	void setBuddySigItalic(const bool b) { m_BuddySigFontI  = b; }

	void setBuddyFlashColor( const QColor &color) { m_BuddyFlashColor = color; }
	void setBuddyFlashBold(const bool b) { m_BuddyFlashFontB  = b; }
	void setBuddyFlashUnderline(const bool b) { m_BuddyFlashFontU  = b; }
	void setBuddyFlashItalic(const bool b) { m_BuddyFlashFontI  = b; }

	// set Qun font settings
	void setQunNameColor ( const QColor &color) { m_QunNameColor = color; }
	void setQunNameBold(const bool b) { m_QunNameFontB  = b; }
	void setQunNameUnderline(const bool b) { m_QunNameFontU  = b; }
	void setQunNameItalic(const bool b) { m_QunNameFontI  = b; }

	void setQunFlashColor ( const QColor &color) { m_QunFlashColor = color; }
	void setQunFlashBold(const bool b) { m_QunFlashFontB  = b; }
	void setQunFlashUnderline(const bool b) { m_QunFlashFontU  = b; }
	void setQunFlashItalic(const bool b) { m_QunFlashFontI  = b; }

	// set Group font settings
	void setGroupNameColor( const QColor &color) { m_GroupNameColor = color; }
	void setGroupNameBold(const bool b) { m_GroupNameFontB  = b; }
	void setGroupNameUnderline(const bool b) { m_GroupNameFontU  = b; }
	void setGroupNameItalic(const bool b) { m_GroupNameFontI  = b; }

	void setGroupCountColor( const QColor &color) { m_GroupCountColor = color; }
	void setGroupOnlineCountBold(const bool b) { m_GroupOnlineCountFontB  = b; }
	void setGroupOnlineCountUnderline(const bool b) { m_GroupOnlineCountFontU  = b; }
	void setGroupOnlineCountItalic(const bool b) { m_GroupOnlineCountFontI  = b; }

	void setGroupFlashColor ( const QColor &color) { m_GroupFlashColor = color; }
	void setGroupFlashBold(const bool b) { m_GroupFlashFontB  = b; }
	void setGroupFlashUnderline(const bool b) { m_GroupFlashFontU  = b; }
	void setGroupFlashItalic(const bool b) { m_GroupFlashFontI  = b; }	
	void setIdleMaxTime(const int t) { idleMaxTime = t; }

	// get buddy font settings
	const QColor &getBuddyNameColor () const { return m_BuddyNameColor; }
	const bool isBuddyNameBold() const { return m_BuddyNameFontB; }
	const bool isBuddyNameUnderline() const { return m_BuddyNameFontU; }
	const bool isBuddyNameItalic() const { return m_BuddyNameFontI; }

	const QColor &getBuddySigColor () const { return m_BuddySigColor; }
	const bool isBuddySigBold() const { return m_BuddySigFontB; }
	const bool isBuddySigUnderline() const { return m_BuddySigFontU; }
	const bool isBuddySigItalic() const { return m_BuddySigFontI; }

	const QColor &getBuddyFlashColor () const { return m_BuddyFlashColor; }
	const bool isBuddyFlashBold() const { return m_BuddyFlashFontB; }
	const bool isBuddyFlashUnderline() const { return m_BuddyFlashFontU; }
	const bool isBuddyFlashItalic() const { return m_BuddyFlashFontI; }

	// get Qun font settings
	const QColor &getQunNameColor () const { return m_QunNameColor; }
	const bool isQunNameBold() const { return m_QunNameFontB; }
	const bool isQunNameUnderline() const { return m_QunNameFontU; }
	const bool isQunNameItalic() const { return m_QunNameFontI; }

	const QColor &getQunFlashColor () const { return m_QunFlashColor; }
	const bool isQunFlashBold() const { return m_QunFlashFontB; }
	const bool isQunFlashUnderline() const { return m_QunFlashFontU; }
	const bool isQunFlashItalic() const { return m_QunFlashFontI; }

	// get Group font settings
	const QColor &getGroupNameColor() const { return m_GroupNameColor; }
	const bool isGroupNameBold() const { return m_GroupNameFontB; }
	const bool isGroupNameUnderline() const { return m_GroupNameFontU; }
	const bool isGroupNameItalic() const { return m_GroupNameFontI; }

	const QColor &getGroupFlashColor () const { return m_GroupFlashColor; }
	const bool isGroupFlashBold() const { return m_GroupFlashFontB; }
	const bool isGroupFlashUnderline() const { return m_GroupFlashFontU; }
	const bool isGroupFlashItalic() const { return m_GroupFlashFontI; }

	const QColor &getGroupCountColor() const { return m_GroupCountColor; }
	const bool isGroupOnlineCountBold() const { return m_GroupOnlineCountFontB; }
	const bool isGroupOnlineCountUnderline() const { return m_GroupOnlineCountFontU; }
	const bool isGroupOnlineCountItalic() const { return m_GroupOnlineCountFontI; }

	int getIdleMaxTime() const { return idleMaxTime; }

	// main window position
	void setWinGeometry(const QPoint &p, const QSize &size) { m_WinLeftTop = p; m_WinSize = size; }
	const QPoint &getWinPoint() const { return m_WinLeftTop; }
	const QSize &getWinSize() const { return m_WinSize; }

	void saveToBeAddedBuddy( BuddyInfoCacheItem info);
	// the method will remove the record as well
	const BuddyInfoCacheItem getToBeAddedBuddy(const unsigned int id);
	const BuddyInfoCacheItem removeToBeAddedBuddy(const unsigned int id);

	void addToRejectForever(const unsigned int id);
	const bool removeFromRejectForever(const unsigned int id);
	const bool isInRejectForever(const unsigned int id);

	void addToQunRejectForever(const unsigned int id, const unsigned int qq);
	const bool removeFromQunRejectForever(const unsigned int id, const unsigned int qq = 0);// if qq is 0, remove all members of qun id
	const bool isInQunRejectForever(const unsigned int id, const unsigned int qq);

	void setRecentContactListSize(const int size);
	const int recentContactListSize();
	std::list<RecentContact> getRecentContacts ();
	void updateRecentContact(std::list<RecentContact>& list);
	
	KConfig *config( const QString &group );
	
	// used in EvaContactListView to check if setting changed
	inline void setNeedRepaint(const bool repaint = true);
	inline bool needRepaint() const;
// protected:
// 	virtual void customEvent( QCustomEvent *e);
private:
	const QString getUserHomeDir();
	const QString getEvaSettingDir();
	const QString getEvaUserDir();
	const bool isDirExisted(const QString &dir);
	bool createDefaultDirs();
	bool loadOldProfile();
	int qqNum;
	bool isUserDirExisted;
	QString sysSettingDir;
	QString pictureCacheDir;
	QString customSmileyDir;
	QString userHeadDir;
	
	QString themeDir;
	QString soundDir;
	int pageSize;	
	QSize faceSize;
	bool showQQBroadcast;
	bool m_ShowQQNews;
	bool showMessageTipWindow;
	bool showBudyOnlineNotifyWindow;
	bool showOnlineUsers;
	bool isSendKeyEnter;
	bool playSound;
	bool autoReply;
	KShortcut shortcut;
	int idleMaxTime;
	
	QColor m_BuddyNameColor;
	bool m_BuddyNameFontB, m_BuddyNameFontU, m_BuddyNameFontI;
	QColor m_BuddySigColor;
	bool m_BuddySigFontB, m_BuddySigFontU, m_BuddySigFontI;
	QColor m_BuddyFlashColor;
	bool m_BuddyFlashFontB, m_BuddyFlashFontU, m_BuddyFlashFontI;

	QColor m_QunNameColor;
	bool m_QunNameFontB, m_QunNameFontU, m_QunNameFontI;
	QColor m_QunFlashColor;
	bool m_QunFlashFontB, m_QunFlashFontU, m_QunFlashFontI;

	QColor m_GroupNameColor;
	bool m_GroupNameFontB, m_GroupNameFontU, m_GroupNameFontI;
	QColor m_GroupCountColor;
	bool m_GroupOnlineCountFontB, m_GroupOnlineCountFontU, m_GroupOnlineCountFontI;
	QColor m_GroupFlashColor;
	bool m_GroupFlashFontB, m_GroupFlashFontU, m_GroupFlashFontI;
	
	bool m_ShowSmileyInNickName;
	bool m_ShowSignatureInSeperateLine;

	//QRect m_WinGeometry;  /// we don't use QRect as Qt Doc says this would cause flash
	QPoint m_WinLeftTop;
	QSize   m_WinSize;

	int autoSelectedIndex;
	std::list<QString>autoList;
	std::list<QString>quickList;

	
	QString buddyListFileName;
	QString qunListFileName;
	QString chatMsgFileName;
	QString qunMsgFileName;
	QString sysMsgFileName;
	QString userProfileFileName;
	QString userConfigFileName;
	
	std::stack<sysMessage> sysStack;

	std::map<unsigned int, QString> m_CachedToBeAddedBuddy;

	std::list<unsigned int> m_RejectForever;
	std::map<unsigned int, std::list<unsigned int> > m_QunRejectForever;
	KConfig *m_config;
	bool m_NeedRepaint;
	
	friend class EvaUser;
};

inline void EvaUserSetting::setNeedRepaint(const bool repaint) { m_NeedRepaint = repaint; }
inline bool EvaUserSetting::needRepaint() const { return m_NeedRepaint; }

#endif
 
