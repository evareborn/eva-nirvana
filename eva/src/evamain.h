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

#ifndef EVAMAIN_H
#define EVAMAIN_H

#include <qobject.h>
#include <qdatetime.h>
#include <qstringlist.h>
#include <inttypes.h>
#include <qhostaddress.h>
#include <qvaluelist.h>
#include <dcopobject.h>

#include "config.h"
#include "libeva/evamemo.h"
#include "evaidt.h"
#include "evadcopactions.h"

// FIXME: wanna integrate with kwallet? why not then?
//#include <kwallet.h>

// Predefinitions

//   Versions should be kept in one place
#define	EVA_VERSION	"0.4.2"
#define EVA_BUILD	"20070320"


// External classes?
class KPopupMenu;
class KHelpMenu;
class QTextCodec;
class QTimer;
//class KAboutData;
//class KAboutApplication;
class KGlobalAccel;

class EvaUser;
class EvaConnecter;
class EvaPacketManager;
class EvaFileManager;
class EvaGlobal;
class EvaImageResource;
class EvaSetting;
class EvaAddingManager;
class EvaSysMsgManager;

class EvaLoginWindow;
class EvaMainWindow;
class EvaChatWindowManager;
class EvaSystemTray;
class EvaHelper;
class EvaPicManager;
class EvaUHManager;
class IdleTimeDetector;
class EvaNotifyEvent;

// FIXME: wanna integrate with kwallet? why not then?
//class Wallet;


class EvaMain : public QObject, virtual public EvaDCOPActionsInterface
{
	Q_OBJECT

public:
	EvaMain();
	~EvaMain();

	// store user information
	static EvaUser *user;

	// store some global system setting such as paths
	static EvaGlobal *global;

	// store resources like images and sounds
	static EvaImageResource *images;

	// ??
	static EvaHelper *helper;	

	// class to connect to QQ server
	static EvaConnecter *connecter;

	// ??
	static EvaPicManager *picManager;

	// user head pictures manager
	static EvaUHManager *uhManager;

	// adding frioend manager
	static EvaAddingManager *g_AddingManager;

	// chat window, second important window in this game
	static EvaChatWindowManager *g_ChatWindowManager;

	// the main guy, with your friend list in
	static EvaMainWindow *g_mainWin;

	// transferring files manager
	EvaFileManager *m_FileManager;
	
	/// DCOP Actions calls
	void changeToOnline();
	void changeToOffline();
	void changeToLeave();
	void changeToInvisible();
	void changeNick(QString nick);
	void changeSignature( QString contents);
	void sendToContact(unsigned int id, QString msg);
	void sendToQun(unsigned int ext, QString msg);
	void textReady(unsigned int id, QString text, bool isQun);
	void imageReady(unsigned int id, QString path, bool isQun);
	void openAddFriendDialog(unsigned int id);

protected:
	virtual void customEvent( QCustomEvent * e );
	void dispatchEvaEvent( EvaNotifyEvent * e);
	
private:
	// process console arguments
	void processCLIArgs();
	
	// initialize user custom settings
	void initSettings();

	// initialize user customized leaving messages
	void initUserLeaveMenu();

	// QString path, for encoding converting
	QTextCodec *codec;

	// every timer interval to get online friends' status
	QTimer *onlineFriendTimer;

	// packetize QQ messages
	EvaPacketManager *packetManager;

	// system notification handler
	EvaSysMsgManager *m_SysMsgManager;

	// detecting idle time
	IdleTimeDetector *idt;

	// FIXME: do the following two need existing in the code?
	//QString lastSentName;
	//int lastSentID;
	
	// for connection interruption detecting
	int numOfLostKeepAlivePackets;

	// whether user is logged in
	//bool loggedIn;

	// one QQ server, choose random from config file
	QHostAddress QQServer;

	// guard against deleting one friend from more than one friend group
	int deleteFromQQ;

	// control groups showing while getting friend list from server
	bool m_IsShowingGroups;
	
	// idle status
	bool inIdleStatus;

	// about window
	//KAboutApplication *aboutEva;

	// store user defined system settings such as user home diretory, message history etc.
	EvaSetting *sysSetting;
	
	// system tray menu
	KPopupMenu *sysMenu;
	KHelpMenu *m_helpMenu;

	// user status setting menu, appeared both in main window and tray menu
	KPopupMenu *statusMenu;

	// user status setting submenu, custumized leaving auto-reply messages
	KPopupMenu *autoMenu;

	// login window, first one you will see
	EvaLoginWindow *loginWin;

// 	// the main guy, with your friend list in
// 	EvaMainWindow *mainWin;	

	// system tray icon
	EvaSystemTray *tray;

	// login retry times
	int numLoginRetry;

	// if not set, all the windows should be constructed at once, otherwise just do login thing
	bool isClientSet;

	// accelerate key, maybe officially CTRL+ALT+Z
	KGlobalAccel *accelKey;

	// FIXME: wanna integrate with kwallet? why not then?
	//KWallet::Wallet *_wallet;
	
	void loginOK();
private slots:
	// TODO: should we put so many slots in the same place?
	void slotIdleTimeUp();
	void slotIdleBack();
	//void slotLoadGroupedBuddiesReady();
	void slotLoadQunListReady();
	void doQuit();
	bool doInitUI();
	void initMenus();
	void doSlotConnection();
	void slotGetOnlineStatus();

	void slotSetupUser();
	void slotSetupNetwork();
	void slotSetupWindowManager();
	
	void slotNetworkException(int);
	void slotPacketException(int);

	//void slotConnectReady();
	void slotDoLoginClick();
	void slotServerBusy();
	void slotSetupEvaClient();
	void slotFetchQQServer();
	void slotGotServer(QHostAddress addr);
	//void slotLoginOK();
	//void slotWrongPassword(QString msg);
	void slotKickedOut(const QString msg);
	void slotOnlineReady();
	void slotInvisibleReady();
	void slotLeaveReady();
	void slotOfflineReady();
	void slotFriendStatusChanged(unsigned int);
	//void slotFriendListReady();
	//void slotSentMessageResult(bool); 
	void slotTxtMessage(unsigned int sender, bool isNormal, QString message, QDateTime time, const char fontSize, 
			const bool u, const bool i, const bool b, 
			const char blue, const char green, const char red); 
	//void slotFriendGroupsReady();  
	
	// login window
	void slotDoLogin();
	void slotDoCancel();
	
	void slotDoOnline();
	void slotDoOffline();
	void slotDoLeave();
	void slotAutoReplyMenuActivated(int id);
	void slotDoInvisible();
	void slotDoLogout();
	
	void slotDoDownloadGroups();
	void slotDoUploadGroups();
	void slotFriendGroupsUploadReady(bool);
	
	void slotDoDownloadBuddies();
	void slotDoChangeUser();
	
	void slotGroupDeleted(const int);
	//void slotGroupAdded(QString, int);
	void slotGroupRenamed(QString, int);
	void slotGroupChanged(const unsigned int, int);
	
	void slotRequestDetails(const unsigned int);
	void slotRequestLevel(const unsigned int);
	void slotRequestChat(const unsigned int);
	void slotRequestSendFile(const unsigned int);
	void slotRequestHistory(const unsigned int);
	void slotRequestQQShow(const unsigned int);
	void slotRequestMyQQShow();
	void slotQQShowReady(const unsigned int);
	void slotRequestSearch();
	
	//void slotSystemMeBeenAdded(unsigned int);
	//void slotSystemAddMeRequest(unsigned int, QString);
	//void slotSystemAddRequestApproved(unsigned int);
	//void slotSystemAddRequestRejected(unsigned int, QString);
	
	//void slotRequestSystemMessage();
	//void slotShowSystemMessage(const short, const uint8_t type, const unsigned int from, const unsigned int me, const QString message, const unsigned int internalQunID = 0);
	void slotRequestSystemMessages();
	
	void slotRequestDelete(const unsigned int);
	void slotDeleteBuddyReady(unsigned int, bool);
	
	void slotRequestSystemSettingWindow();
	void slotDeleteMeFrom(const unsigned int);
	void slotDeleteMeReply(bool);
	void slotRequestAddBuddy(const unsigned int);
	
	//void slotMyInfoReady();
	
	void slotQunInfomationReady(const unsigned int, const bool, QString);
	
	void slotRequestQunDetails(const unsigned int);
	void slotRequestQunHistory(const unsigned int);
	void slotRequestQunChat(const unsigned int);
	void slotReceivedQunMessage( unsigned int, unsigned int, QString, QDateTime, const char, 
			const bool, const bool, const bool, const char, const char, const char);
	void slotQunSystemMessageRequest(const unsigned int, QString);
	
	void slotQunPictureReady(const unsigned int id, const QString fileName, const QString tmpFileName);
	void slotRequestQunCardReady(const unsigned int id, const bool ok, const unsigned int qq, QString realName, const unsigned char gender, 
					QString phone, QString email, QString memo, QString msg);
	void slotDoQunExit(const unsigned int id);
	void slotQunExitReply(const unsigned int, const bool, QString);
	void slotQunCreateFailed(QString);
	void slotQunCreate();

	void slotFriendSignatureChanged(const unsigned int qq, const QDateTime time, const QString signature);
	void slotUpdateQunMessageSettings(const unsigned int id, const signed char type);
	
	void slotUserSettingChanged();
	void slotUpdateShortcut();
	void slotShotcutKeyPressed();
	
	//void slotLoginProcessReady();
	void slotAddAnonymous(const unsigned int id, const unsigned short face);
	
	void slotExtraInfoReady();
	//void slotShowOnlineSelected();
	//void slotShowAllSelected();

	void slotUserMemoChanged(const unsigned int id, const MemoItem &memo);
	void slotModifyMemo(const unsigned int id );

	void slotFileTransferSend(const unsigned int, const unsigned int, const QValueList<QString>,
				const QValueList<unsigned int>, const unsigned char transferType);
	void slotFileTransferAccept(const unsigned int, const unsigned int, const QString,
				const unsigned char transferType);
	void slotFileTransferCancel(const unsigned int, const unsigned int);

	void slotReceivedFileRequest(const unsigned int,  const unsigned int, const QString, const int, const unsigned char);
	void slotReceivedFileAccepted(const unsigned int, const unsigned int, const unsigned int, const bool, const unsigned char);

	void slotNotifyTransferStatus(const unsigned int, const unsigned int, const unsigned int, 
				const unsigned int, const int );
	void slotReceivedFileAgentInfo(const unsigned int, const unsigned int , const unsigned int,
					const unsigned int , const unsigned short, const unsigned char *);
	void slotReceivedFileNotifyIpEx(const unsigned int id, const bool isSender,
				const unsigned int session, const unsigned char transferType,
				const unsigned int wanIp1, const unsigned short wanPort1,
				const unsigned int wanIp2, const unsigned short wanPort2,
				const unsigned int wanIp3, const unsigned short wanPort3,
				const unsigned int lanIp1, const unsigned short lanPort1,
				const unsigned int lanIp2, const unsigned short lanPort2,
				const unsigned int lanIp3, const unsigned short lanPort3,
				const unsigned int syncIp, const unsigned short syncPort,
				const unsigned int syncSession);
	void slotNotifyAddressRequest(const unsigned int, const unsigned int, const unsigned int, 
					const unsigned int, const unsigned short,  
					const unsigned int, const unsigned short);
	//void slotLoginVerification();
	void slotBuddyAdded(const unsigned int id, const QString nick, const unsigned short face, const int group);
	void slotShowScriptManager();
	
	friend class EvaMainWindow;
};

extern EvaMain *g_eva;
#endif

