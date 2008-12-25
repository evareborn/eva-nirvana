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

#include <inttypes.h>
#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <QCustomEvent>
#include <QHostAddress>

#include "libeva/evamemo.h"
#include "evaidt.h"
#include "evaapi.h"

 
// Predefinitions

//   Versions should be kept in one place
#define	EVA_VERSION	"0.4.2"
#define EVA_BUILD	"20081224"


// External classes?
class QMenu;
//X class QHelpMenu;
class QTextCodec;
class QTimer;
class QShortcut;

class EvaSession;
class EvaUserSetting;
class EvaGlobal;
class EvaImageResource;
class EvaSoundResource;
class EvaSetting;
class EvaFileManager;
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


class EvaMain : public QObject
{
	Q_OBJECT

public:
	EvaMain();
	~EvaMain();

//X 	// store some global system setting such as paths
//X 	static EvaGlobal *global;

	// store resources like images and sounds
	static EvaImageResource *images;
 
        static EvaSoundResource *sound;

	// ??
	static EvaHelper *helper;	

	// chat window, second important window in this game
	static EvaChatWindowManager *g_ChatWindowManager;

	// the main guy, with your friend list in
	static EvaMainWindow *g_mainWin;
 
	// QString path, for encoding converting
	static QTextCodec *codec;

        static EvaMain* g_eva;

        static EvaSession* session;
        static EvaMain* getInstance() { return g_eva; }
        EvaUHManager* getUHManager() { return uhManager; }
        EvaPicManager* getPicManager() { return picManager; }
        EvaAddingManager* getAddingManager() { return addingManager; }
        EvaFileManager* getFileManager() { return fileManager; }

protected:
	virtual void customEvent( QEvent * e );
	
private:
	bool initUI();
	void initMenus();
	void setupUser();
	void setupNetwork();
	void doSlotConnection();
	void setupEvaClient();
	void setupWindowManager();

	// process console arguments
	void processCLIArgs();
	
	// initialize user custom settings
	bool initSettings();

	// initialize user customized leaving messages
	void initUserLeaveMenu();

        EvaUserSetting* settings;

	// user head pictures manager
	EvaUHManager *uhManager;

	// ??
	EvaPicManager *picManager;

	// adding frioend manager
	EvaAddingManager *addingManager;

	// system notification handler
	EvaSysMsgManager *m_SysMsgManager;

        EvaFileManager* fileManager;
	// detecting idle time
	IdleTimeDetector *idt;

	// for connection interruption detecting
	int numOfLostKeepAlivePackets;

	// guard against deleting one friend from more than one friend group
	int deleteFromQQ;

	// idle status
	bool inIdleStatus;

	// store user defined system settings such as user home diretory, message history etc.
	EvaSetting *sysSetting;
	
	// system tray menu
	QMenu *sysMenu;
//X 	QHelpMenu *m_helpMenu;

	// user status setting menu, appeared both in main window and tray menu
	QMenu *statusMenu;

	// user status setting submenu, custumized leaving auto-reply messages
	QMenu *autoMenu;

	// login window, first one you will see
	EvaLoginWindow *loginWin;

	// system tray icon
	EvaSystemTray *tray;

	// login retry times
	int numLoginRetry;

	// if not set, all the windows should be constructed at once, otherwise just do login thing
	bool isClientSet;

private slots:
 
        /**
         * We use emited signals rather than notified events so that
         * the core session need not to know whom to send event to
         * in its own code.
         */

        void slotLoginProcessUpdate( EvaLoginProcess );
        void slotError( EvaError );

	// TODO: should we put so many slots in the same place?
	void slotIdleTimeUp();
	void slotIdleBack();

	void slotUserSettingChanged();
        void slotStatusChanged( UserStatus );
	void slotQunListChanged();
	void slotGroupChanged(const unsigned int, int);
	void slotUserMemoChanged(const unsigned int id, const MemoItem &memo);
	void slotFriendSignatureChanged(const unsigned int qq, const QDateTime time, const QString signature);

	void slotTxtMessage(unsigned int sender, bool isNormal, QString message, QDateTime time, const char fontSize, 
			const bool u, const bool i, const bool b, 
			const char blue, const char green, const char red); 
	void slotReceivedQunMessage( unsigned int, unsigned int, QString, QDateTime, const char, 
			const bool, const bool, const bool, const char, const char, const char);
	
        /**
         * Start: the '*Ready' messages.
         */
	void slotExtraInfoReady();
	void slotFriendGroupsUploadReady(bool);
	void slotQQShowReady(const unsigned int);
	void slotDeleteBuddyReady(unsigned int, bool);
	void slotQunInfomationReady(const unsigned int, const bool, QString);
	void slotQunPictureReady(const unsigned int id, const QString fileName, const QString tmpFileName);
	void slotRequestQunCardReady(const unsigned int id, const bool ok, const unsigned int qq, QString realName, const unsigned char gender, 
					QString phone, QString email, QString memo, QString msg);
	
	void slotDeleteMeReply(bool);
	void slotQunExitReply(const unsigned int, const bool, QString);
	void slotAddAnonymous(const unsigned int id, const unsigned short face);

        /* End: the '*Ready' messages. */
	
        /**
         * Start:  slots for gui actions, we are considering to move them.
         */

	void slotDoLogin();
	void slotDoCancel();
	void slotAutoReplyMenuActivated(int id);
	void slotDoOnline();
	void slotDoOffline();
	void slotDoLeave();
	void slotDoInvisible();
	void slotDoLogout();
	void slotDoQuit();
 
        // This slot should be moved to the EvaLoginManager class
	void slotDoLoginClick();
	
	void slotDoDownloadGroups();
	void slotDoUploadGroups();
	
	void slotDoDownloadBuddies();
	void slotDoChangeUser();
	
	void slotGroupDeleted(const int);
	void slotGroupRenamed(QString, int);
	
	void slotRequestDetails(const unsigned int);
	void slotRequestLevel(const unsigned int);
	void slotRequestChat(const unsigned int);
	void slotRequestSendFile(const unsigned int);
	void slotRequestHistory(const unsigned int);
	void slotRequestQQShow(const unsigned int);
	void slotRequestMyQQShow();
	void slotRequestSearch();
	
	void slotRequestSystemMessages();
	
	void slotRequestDelete(const unsigned int);
	void slotBuddyAdded(const unsigned int id, const QString nick, const unsigned short face, const int group);
	
	void slotRequestSystemSettingWindow();
	void slotDeleteMeFrom(const unsigned int);
	void slotRequestAddBuddy(const unsigned int);
	
	
	void slotRequestQunDetails(const unsigned int);
	void slotRequestQunHistory(const unsigned int);
	void slotRequestQunChat(const unsigned int);
	void slotDoQunExit(const unsigned int id);
	void slotQunCreateFailed(QString);
	void slotQunCreate();

	void slotUpdateQunMessageSettings(const unsigned int id, const signed char type);
	
	void slotUpdateShortcut();
	void slotShotcutKeyPressed();
	
	void slotModifyMemo(const unsigned int id );
         /*End:  slots for gui actions, we are considering to move them. */

        /**
         * Start: slots for file transfering
         */
	void slotFileTransferSend(const unsigned int, const unsigned int, const QList<QString>,
				const QList<unsigned int>, const unsigned char transferType);
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
        /*End: slots for file transfering */
 
public:
	
	friend class EvaMainWindow;
};

#endif

