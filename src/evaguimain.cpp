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

#include <string>
#include <assert.h>
#include <QApplication>
#include <QShortcut>
#include <QPixmap>
#include <QCustomEvent>
#include <QDesktopWidget>
#include <QMessageBox>

#include "evaidt.h"
#include "defines.h"
#include "evaguimain.h"
#include "evasession.h"
#include "evautil.h"
#include "evaapi.h"
#include "evaui.h"
#include "evasetting.h"
#include "evaservers.h"
#include "evaconnecter.h"
#include "evafilemanager.h"
#include "evapacketmanager.h"
#include "evauhmanager.h"
#include "evaresource.h"
#include "evapicmanager.h"
#include "evachatwindowmanager.h"
#include "evasystemtray.h"
#include "evaaddingmanager.h"
#include "evasysmsgmanager.h"

#define MaxRetryTimes  5

		
//X EvaUser *EvaMain::user = NULL;
EvaSession* EvaMain::session = NULL;
EvaGlobal *EvaMain::global = NULL;
EvaImageResource *EvaMain::images = NULL;
EvaHelper *EvaMain::helper = NULL;
//X EvaConnecter *EvaMain::connecter = NULL;
//X EvaPicManager *EvaMain::picManager = NULL;
//X EvaUHManager *EvaMain::uhManager = NULL;
//X EvaAddingManager *EvaMain::m_addingManager = NULL;
EvaChatWindowManager *EvaMain::g_ChatWindowManager = NULL;
EvaMainWindow *EvaMain::g_mainWin = NULL;
QTextCodec* EvaMain::codec = QTextCodec::codecForName("GB18030");
EvaMain* EvaMain::g_eva = NULL;
//X EvaContactManager* m_contactManager = NULL;
 
EvaMain::EvaMain() :
//X 	DCOPObject("Actions"),
        uhManager( NULL ),
        picManager( NULL ),
	m_SysMsgManager(NULL),
        fileManager( NULL ),
	numOfLostKeepAlivePackets(0),
	sysMenu(NULL),
	statusMenu(NULL),
	autoMenu(NULL),
        loginWin( NULL )
// 	accelKey(NULL)
{
	
	//loggedIn=false;
	isClientSet=false;
	numLoginRetry = 0;
	deleteFromQQ = -1;
	m_IsShowingGroups = false;
	inIdleStatus = false;
	EvaUtil::initMap();
//X 	onlineFriendTimer = new QTimer();
//X 	codec = QTextCodec::codecForName("GB18030");
        if ( !initSettings() )
            return; // load image first
	loginWin = new EvaLoginWindow(0);
	g_mainWin = new EvaMainWindow(this);
	tray = new EvaSystemTray(g_mainWin);
	helper = new EvaHelper();
	idt = new IdleTimeDetector(5);
	if(!doInitUI()){
		doQuit();
	}

//X 	if( !kapp->dcopClient()->isRegistered()){
//X 		kapp->dcopClient()->registerAs("evadcop");
//X 		kapp->dcopClient()->setDefaultObject( objId() );
//X 	}
	doSlotConnection();
	processCLIArgs();
        g_eva = this;
}

EvaMain::~EvaMain()
{
	if(uhManager){
		uhManager->stop();
		uhManager->wait();
		delete uhManager;
		uhManager = 0;
	}
	if(picManager){
		delete picManager;
		picManager = 0;
	}
//X 	if(onlineFriendTimer->isActive())
//X 		onlineFriendTimer->stop();
	if(session->isLoggedIn())
		session->logout();

//X 	GetScriptManager()->releaseScripts();
	
	delete loginWin;
	delete g_mainWin;

	if(m_SysMsgManager){
		delete m_SysMsgManager;
	}
	
	if(global) delete global;
	if(helper->running())
		helper->terminate();
	delete helper;
	if(idt) delete idt;

	if(session){
		settings->setWinGeometry(g_mainWin->getPosition(), g_mainWin->getSize());
		settings->saveSettings();
		delete session;
		session = 0;
	}
}

void EvaMain::processCLIArgs()
{
//X 	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
//X 
//X 	if(args->isSet("last"))
//X 		loginWin->loginClickSlot();
//X 	else{
//X 		QCString user, password;
//X 		bool ok = false;
//X 		user  = args->getOption("user");
//X 		if(!user.isEmpty()) {
//X 			//unsigned int id = user.toUInt(&ok);
//X 			if(!ok){
//X 				printf(I18N_NOOP("username must be in numeric format.\n"));
//X 				QTimer::singleShot(200, this, SLOT(doQuit()));
//X 				return;
//X 			}
//X 			password = args->getOption("passwd");
//X 			if(password.isEmpty()){
//X 				printf(I18N_NOOP("password missing, type \"eva --help\" to see details.\n"));
//X 				QTimer::singleShot(200, this, SLOT(doQuit()));
//X 				return;
//X 			}
//X 			loginWin->cbQQ->setCurrentText(QString(user.data()));
//X 			loginWin->lePwd->setText(QString(password.data()));
//X 			loginWin->slotPasswordChanged(QString(password.data()));
//X 			
//X 			if(args->isSet("hide")) 
//X 				loginWin->chbLoginMode->setChecked(true);
//X 			else 
//X 				loginWin->chbLoginMode->setChecked(false);
//X 
//X 			if(args->isSet("remember")) 
//X 				loginWin->chbRecordPwd->setChecked(true);
//X 			else 
//X 				loginWin->chbRecordPwd->setChecked(false);
//X 
//X 			QCString mode = args->getOption("mode");
//X 			
//X 			if(mode.isEmpty()) mode = "UDP";
//X 			if(mode == "UDP")
//X 				loginWin->cbbLoginType->setCurrentItem(0);
//X 			else	if(mode == "TCP") 
//X 						loginWin->cbbLoginType->setCurrentItem(1);
//X 					else	if(mode == "HTTP_PROXY")
//X 								loginWin->cbbLoginType->setCurrentItem(2);
//X 							else {
//X 								printf(I18N_NOOP("mode can only be one of UDP, TCP or HTTP_PROXY.\n"));
//X 								QTimer::singleShot(200, this, SLOT(doQuit()));
//X 								return;
//X 							}
//X 
//X 			if(mode == "HTTP_PROXY"){
//X 				QCString ip, port;
//X 				ip = args->getOption("pip");
//X 				if(ip.isEmpty()) {
//X 					printf(I18N_NOOP("http proxy server IP address missing.\n"));
//X 					QTimer::singleShot(200, this, SLOT(doQuit()));
//X 					return;
//X 				}
//X 				port = args->getOption("pport");
//X 				if(port.isEmpty()) {
//X 					printf(I18N_NOOP("http proxy server port number missing.\n"));
//X 					QTimer::singleShot(200, this, SLOT(doQuit()));
//X 					return;
//X 				}
//X 				QHostAddress addr;
//X 				if(!addr.setAddress(QString(ip.data()))){
//X 					printf(I18N_NOOP("http proxy server IP address is in wrong format.\n"));
//X 					QTimer::singleShot(200, this, SLOT(doQuit()));
//X 					return;
//X 				}
//X 				bool ok = false;
//X 			//	unsigned int pp = port.toUInt(&ok);
//X 				if(!ok){
//X 					printf(I18N_NOOP("http proxy server port number is in wrong format.\n"));
//X 					QTimer::singleShot(200, this, SLOT(doQuit()));
//X 					return;
//X 				}
//X 				loginWin->leIP->setText(addr.toString());
//X 				loginWin->lePort->setText(QString(port.data()));
//X 				QCString puser, ppwd;
//X 				puser = args->getOption("pu");
//X 				if(!puser.isEmpty()) loginWin->slotProxyUserChanged(QString(puser.data()));
//X 				ppwd = args->getOption("ppw");
//X 				if(!ppwd.isEmpty()) loginWin->slotProxyPasswordChanged(QString(ppwd.data()));	
//X 			}
//X 			loginWin->loginClickSlot();
//X 			//loginWin->show();
//X 		} else
			loginWin->show();
//X 	}	
//X 	args->clear();
}

bool EvaMain::initSettings()
{
	global = new EvaGlobal();
	if(!global->loadImage()) {
		QMessageBox::information(0, i18n("Eva Initializing"),
                        i18n("Sorry, image resource cannot be loaded properly, check image directory please."));
		//exit(0);
		doQuit();
                return false;
	}
        printf( "continued.\n" );
	images = global->getImageResource();
	sysSetting = global->getEvaSetting();
	QObject::connect(images, SIGNAL(qqShowReady(const unsigned int)), SLOT(slotQQShowReady(const unsigned int)));
	
	m_SysMsgManager = new EvaSysMsgManager(this);
	//_wallet = 0;
        return true;
}

//X void EvaMain::slotLoadGroupedBuddiesReady()
//X {
//X 	if(loggedIn) return;
//X 	g_mainWin->updateBuddy(session->getQQ());
//X 	g_ChatWindowManager->setMyName(nick, session->getQQ());
//X 
//X 	g_mainWin->clearList();
//X 	g_mainWin->updateContacts();
//X 	
//X 	tray->changeToolTip(session->getQQ(), nick, faceId);
//X }

void EvaMain::slotLoadQunListReady()
{
	g_mainWin->updateQuns();
	g_mainWin->updateRecentContacts();
}

// void EvaMain::doAbout()
// {
// 	KAboutApplication dlg(g_mainWin);
// 	dlg.exec();
// }

void EvaMain::doQuit()
{
//X 	kdDebug() << "Quiting Eva..."<< endl;
 	printf( "Quiting Eva...\n" );
	if(loginWin){
		loginWin->hide();
	}
	
//X 	GetScriptManager()->releaseScripts();
	
	if(g_mainWin){
		g_mainWin->hide();
	}
	if(uhManager){
		uhManager->stop();
		uhManager->wait();
		delete uhManager;
		uhManager = 0;
	}
	if(picManager){
		delete picManager;
		picManager = 0;
	}

	if(m_SysMsgManager)
		delete m_SysMsgManager;

//X 	if(onlineFriendTimer->isActive())
//X 		onlineFriendTimer->stop();

	if(session->isLoggedIn())
		session->logout();

	
	if(session){
		settings->setWinGeometry(g_mainWin->getPosition(), g_mainWin->getSize());
		settings->saveSettings();
		delete session;
		session = 0;
	}
 
        if ( helper ) {
            delete helper;
        }
	
        printf( "Quiting Eva..\n" );
        exit( 0 );
	//KApplication::kApplication()->quit();
}

bool EvaMain::doInitUI()
{
	// login window	
	loginWin->setLogo(images->getIcon("EVALOGO"));
	loginWin->setIcon(*images->getIcon("EVA"));
	if(sysSetting){
		loginWin->setEvaSetting(sysSetting);
	}
	
	// main window
	initMenus();
	g_mainWin->setStatusMenu(statusMenu);
	g_mainWin->setSystemMenu(sysMenu);
	
	tray->setImagesResource(images);
	tray->setupMenus(sysMenu, statusMenu);
	
	EvaDetailsWindow::setImageResource(images);
	return true;
}

void EvaMain::initMenus()
{
	statusMenu = new QMenu();
	statusMenu->insertItem(QIcon(*(images->getIcon("ONLINE"))), i18n("Online"), this,SLOT(slotDoOnline()));
	statusMenu->insertItem(QIcon(*(images->getIcon("OFFLINE"))), i18n("Offline"), this,SLOT(slotDoOffline()));
	statusMenu->insertItem(QIcon(*(images->getIcon("LEAVE"))), i18n("Leave"), this,SLOT(slotDoLeave()));
	statusMenu->insertItem(QIcon(*(images->getIcon("INVISIBLE"))), i18n("Invisible"), this,SLOT(slotDoInvisible()));
	
	sysMenu = new QMenu();
	QMenu *group = new QMenu();

	group->insertItem(QIcon(*(images->getIcon("UPLOAD_GROUPS"))), i18n("Upload Group"),this, SLOT(slotDoUploadGroups()));
	group->insertItem(QIcon(*(images->getIcon("DOWNLOAD_GROUPS"))), i18n("Download Group"),this, SLOT(slotDoDownloadGroups()));
	
	sysMenu->insertItem(QIcon(*(images->getIcon("GROUP"))), i18n("Group"),group);
	sysMenu->insertItem(QIcon(*(images->getIcon("REFRESH_BUDDIES"))), i18n("Update Buddies"), this,SLOT(slotDoDownloadBuddies()));
	sysMenu->insertSeparator(-1);
	sysMenu->insertItem(QIcon(*(images->getIcon("SYSTEM_OPTIONS"))), i18n("System Options"),this, SLOT(slotRequestSystemSettingWindow()));
//X 	sysMenu->insertItem(QIcon(*(images->getIcon("SCRIPT"))), i18n("Script Manager"),this, SLOT(slotShowScriptManager()));
	sysMenu->insertItem(QIcon(*(images->getIcon("CHANGE_USER"))), i18n("Change User"), this,SLOT(slotDoChangeUser()));
	sysMenu->insertSeparator(-1);
	//sysMenu->insertItem(QIconSet(*(images->getIcon("EVA"))), i18n("About Eva"), this,SLOT(doAbout()), -1);
//X 	m_helpMenu = new QHelpMenu(g_mainWin, KGlobal::instance()->aboutData(), 
//X 				 false);
//X 	sysMenu->insertItem(SmallIcon("help"),KStdGuiItem::help().text(), m_helpMenu->menu());
	sysMenu->insertSeparator(-1);
	sysMenu->insertItem(QIcon(*(images->getIcon("QUIT"))), i18n("Quit"), this,SLOT(doQuit()));
}

void EvaMain::initUserLeaveMenu()
{
	if(!statusMenu) return;
	std::list<QString>::iterator iter;
	std::list<QString> autoList = settings->getAutoReplyList();
	//if(!autoList.size()) return;
	
	statusMenu->removeItemAt(2);
	if(autoMenu) delete autoMenu;
	autoMenu = new QMenu();
	autoMenu->setCheckable(true);
	autoMenu->insertItem(QString(i18n("No Auto-Reply")), 0);
	int id=1;
	for(iter=autoList.begin(); iter!=autoList.end(); ++iter){
		autoMenu->insertItem(*iter, id++);
	}
	autoMenu->setItemChecked(settings->getAutoReplySelectedIndex()+1, true);
	statusMenu->insertItem(QIcon(*(images->getIcon("LEAVE"))), i18n("Leave"), autoMenu, -1, 2);
	
	QObject::connect(autoMenu, SIGNAL(activated(int)), this,  SLOT(slotAutoReplyMenuActivated(int)));
}

void EvaMain::slotAutoReplyMenuActivated(int id)
{
	if(id==0)
		settings->setAutoReplyEnabled(false);
	else
		settings->setAutoReplyEnabled(true);
	settings->setAutoReplySelectedIndex(id - 1);
	for(uint i=0; i <= settings->getAutoReplyList().size(); i++){
		if((int)i==id)
			autoMenu->setItemChecked(i, true);
		else
			autoMenu->setItemChecked(i, false);
	}
	if(session->getStatus() != Eva_Leave)
		slotDoLeave();
}

void EvaMain::doSlotConnection()
{
//	QObject::connect(onlineFriendTimer, SIGNAL(timeout()), this, SLOT(slotGetOnlineStatus()));
	QObject::connect(loginWin, SIGNAL(doLogin()), this, SLOT(slotDoLoginClick()));
	QObject::connect(loginWin, SIGNAL(doCancel()), this, SLOT(slotDoCancel()));
	
	QObject::connect(g_mainWin, SIGNAL(groupDeleted(const int)), this, SLOT(slotGroupDeleted(const int)));
	//QObject::connect(g_mainWin, SIGNAL(groupAdded(QString, int)), this, SLOT(slotGroupAdded(QString, int)));
	QObject::connect(g_mainWin, SIGNAL(groupRenamed(QString, int)), this, SLOT(slotGroupRenamed(QString, int)));
	QObject::connect(g_mainWin, SIGNAL(deleteMeFrom(const unsigned int)), this, SLOT(slotDeleteMeFrom(const unsigned int)));
	QObject::connect(g_mainWin, SIGNAL(groupChanged(const unsigned int, int)), this, SLOT(slotGroupChanged(const unsigned int, int)));
	//QObject::connect(g_mainWin, SIGNAL(requestAddBuddy(const unsigned int)),m_addingManager, SLOT(slotAddBuddy(const unsigned int)));
	
	
	QObject::connect(g_mainWin, SIGNAL(requestDelete(const unsigned int)), this, SLOT(slotRequestDelete(const unsigned int)));
	QObject::connect(g_mainWin, SIGNAL(requestDetails(const unsigned int)), this, SLOT(slotRequestDetails(const unsigned int)));
	QObject::connect(g_mainWin, SIGNAL(requestChat(const unsigned int)), this, SLOT(slotRequestChat(const unsigned int)));
	QObject::connect(g_mainWin, SIGNAL(requestSendFile(const unsigned int)), this, SLOT(slotRequestSendFile(const unsigned int)));

	QObject::connect(g_mainWin, SIGNAL(requestLevel(const unsigned int)), this, SLOT(slotRequestLevel(const unsigned int)));
	QObject::connect(g_mainWin, SIGNAL(requestSearch()), this, SLOT(slotRequestSearch()));
	QObject::connect(g_mainWin, SIGNAL(requestSystemMessages()), this, SLOT(slotRequestSystemMessages()));
	QObject::connect(g_mainWin, SIGNAL(requestHistory(const unsigned int)), this, SLOT(slotRequestHistory(const unsigned int)));
	QObject::connect(g_mainWin, SIGNAL(requestQunChat(const unsigned int)), this, SLOT(slotRequestQunChat(const unsigned int)));
	QObject::connect(g_mainWin, SIGNAL(requestQunDetails(const unsigned int)), this, SLOT(slotRequestQunDetails(const unsigned int)));
	QObject::connect(g_mainWin, SIGNAL(requestQunExit(const unsigned int)), this, SLOT(slotDoQunExit( const unsigned int)));
	QObject::connect(g_mainWin, SIGNAL(requestQunCreate()), this, SLOT(slotQunCreate()));
	QObject::connect(g_mainWin, SIGNAL(requestQunHistory(const unsigned int)), this, SLOT(slotRequestQunHistory(const unsigned int)));
	
	QObject::connect(tray, SIGNAL(requestChat(const unsigned int)), this, SLOT(slotRequestChat(const unsigned int)));
	QObject::connect(tray, SIGNAL(requestQunChat(const unsigned int)), this, SLOT(slotRequestQunChat(const unsigned int)));
	QObject::connect(tray, SIGNAL(requestSystemMessage()), m_SysMsgManager, SLOT(showSysMessage()));
	QObject::connect(g_mainWin, SIGNAL(requestModifyMemo(const unsigned int)), this, SLOT(slotModifyMemo(const unsigned int)));

	QObject::connect(m_SysMsgManager, SIGNAL(requestDetails(const unsigned int)), SLOT(slotRequestDetails(const unsigned int)));
	QObject::connect(m_SysMsgManager, SIGNAL(requestQunDetails(const unsigned int)), SLOT(slotRequestQunDetails( const unsigned int)));
	QObject::connect(m_SysMsgManager, SIGNAL(buddyAdded(const unsigned int, const QString, const unsigned short, const int)),
					SLOT(slotBuddyAdded(const unsigned int, const QString, const unsigned short, const int)));
//X 	QObject::connect(m_SysMsgManager, SIGNAL(sysMessage()), tray, SLOT(newSysMessage()));
	QObject::connect(m_SysMsgManager, SIGNAL(qunListChanged()), SLOT(slotLoadQunListReady()));

	QObject::connect(idt, SIGNAL(idleTimeUp()), this, SLOT(slotIdleTimeUp()));
	QObject::connect(idt, SIGNAL(idleBack()), this, SLOT(slotIdleBack()));
}

void EvaMain::slotGetOnlineStatus()
{
//	printf("slotGetOnlineStatus() called!\n");
	std::map<unsigned int, QQFriend>::iterator iter;
	std::map<unsigned int, QQFriend> list = (session->getUser()->getFriendList()).getAllFriendsMap();
	for(iter = list.begin(); iter != list.end(); ++iter){
		switch(iter->second.getStatus()){
		case QQ_FRIEND_STATUS_ONLINE:
			g_mainWin->changeToOnline(iter->second.getQQ());
			break;
//		case QQ_FRIEND_STATUS_OFFLINE:
//			g_mainWin->changeToOffline(iter->second.getQQ());
//			break;
		case QQ_FRIEND_STATUS_LEAVE:
			g_mainWin->changeToLeave(iter->second.getQQ());
			break;
//		case QQ_FRIEND_STATUS_INVISIBLE:
//			g_mainWin->changeToInvisible(iter->second.getQQ());
//			break;
		}	
	}	
}

void EvaMain::slotDoLoginClick()
{
//X 	GetScriptManager()->releaseScripts();
	if(g_mainWin) g_mainWin->offline();
	
	if(tray) tray->reset();
	numLoginRetry = 0;
	isClientSet = false;
	g_mainWin->showInfoFrame(true);
        slotSetupEvaClient();
}
//X 
//X void EvaMain::slotFetchQQServer()
//X {
//X //X 	EvaServers *server = global->getEvaServers();
//X //X 	QObject::disconnect(server, 0, 0, 0);
//X //X 	QObject::connect(server, SIGNAL(isReady(QHostAddress)), SLOT(slotGotServer(QHostAddress)));
//X //X         EvaNetworkPolicy policy = loginWin->getNetworkPolicy();
//X //X 	server->fetchAddress(policy.getConnectionType() == CONN_UDP ? true: false);
//X }
//X 
//X void EvaMain::slotGotServer(QHostAddress addr)
//X {
//X 	printf("Got server :%s\n", addr.toString().ascii());
//X //X 	global->getEvaServers()->stopDns();
//X //X 	QQServer = addr;
//X //X 	slotDoLogin();
//X }
//X 
void EvaMain::slotServerBusy()
{
//X 	if(numLoginRetry > MaxRetryTimes){
//X 		slotPacketException( QQ_CMD_LOGIN);
//X 	}else
//X 		slotFetchQQServer();
}

void EvaMain::slotSetupUser()
{
	if(session){
		QObject::disconnect(session, 0, 0, 0);
	}
        EvaNetworkPolicy policy = loginWin->getNetworkPolicy();

	//printf("EvaMain::slotSetupEvaClient\n");
	session = new EvaSession(loginWin->getQQ(),loginWin->getMd5Password(), policy);
        settings = session->getUser()->getSetting();
	
        // setup adding manager
        addingManager = new EvaAddingManager( session );	
//X 	QObject::connect(user, SIGNAL(loadGroupedBuddiesReady()), SLOT(slotLoadGroupedBuddiesReady()));
	QObject::connect(session->getUser(), SIGNAL(loadQunListReady()), SLOT(slotLoadQunListReady()));
	QObject::connect(addingManager, SIGNAL(requestDetails(const unsigned int)), SLOT(slotRequestDetails(const unsigned int)));
	QObject::connect(addingManager, SIGNAL(buddyAdded(const unsigned int, const QString, const unsigned short, const int)),
					SLOT(slotBuddyAdded(const unsigned int, const QString, const unsigned short, const int)));
	QObject::connect(m_SysMsgManager, SIGNAL(requestAddBuddy(const unsigned int, const QString, const unsigned short)),
			addingManager, SLOT(slotAddBuddy(const unsigned int, const QString, const unsigned short)));

//X 	QString qname, aname, bname;
//X 	qname = i18n("Qun List");
//X 	aname = i18n("Anonymous");
//X 	bname = i18n("Black List");
//X 	session->getUser()->setQunGroupName(codec->fromUnicode(qname).data());
//X 	session->getUser()->setAnonymousGroupName(codec->fromUnicode(aname).data());
//X 	session->getUser()->setBlackGroupName(codec->fromUnicode(bname).data());
//X 	settings->loadSettings();
	images->setThemePath(settings->getThemeDir());
	global->getSoundResource()->setSoundDir(settings->getSoundDir());
	// when we set faee size, the face images will be reloaded as well
	global->setFaceSize(QSize(settings->getFaceSize()));
	
	slotUpdateShortcut();

	if(loginWin->isHiddenLoginMode())
		session->invisible();
	else
		session->online();
	
	//QString name = i18n("Buddy");
//X 	session->getUser()->newGroup(codec->fromUnicode(i18n("Buddy List")).data()); // we must do this if cannot load from disk
	
	// save the tryin-to-login user's parameters anyway
	QHostAddress addr(policy.getProxyIP());
	sysSetting->saveSetting(loginWin->getQQ(), loginWin->getMd5Password(),
							loginWin->isRememberChecked(), loginWin->isHiddenLoginMode(),  
							(int)policy.getConnectionType(),
							addr.toIPv4Address(), policy.getProxyPort(), policy.getProxyUsername(),
							policy.getProxyParam());

}

///NOTE: must call slotSetupUser before calling this method
void EvaMain::slotSetupNetwork( )
{
	if(session->isLoggedIn()) slotDoLogout();

	if(picManager)
		delete picManager;
	picManager = NULL;
	if(uhManager){
		uhManager->stop();
		uhManager->wait();
		delete uhManager;
	}
	uhManager = new EvaUHManager(this, settings->getUserHeadDir());
	uhManager->initiate(global->getFaceSize());// load user head pictures

	if(images){
		images->setUserHeadImage(uhManager->getOnList(), uhManager->getOffList());
	}

        fileManager = new EvaFileManager(session->getQQ(), this);
        EvaNetworkPolicy policy = loginWin->getNetworkPolicy();
        picManager = new EvaPicManager(session->getUser(), policy);
        fileManager->setNetworkPolicy(policy);
	EvaPacketManager* packetManager = session->getPacketManager();

	// connect signals of fileManager
	QObject::connect(fileManager, SIGNAL(notifyTransferStatus(const unsigned int, const unsigned int, const unsigned int, const unsigned int, const int )), 
					SLOT(slotNotifyTransferStatus(const unsigned int, const unsigned int, const unsigned int, const unsigned int, const int )));
	QObject::connect(fileManager, SIGNAL(notifyAddressRequest(const unsigned int, const unsigned int, const unsigned int, 
							const unsigned int, const unsigned short,  const unsigned int, const unsigned short)), 
					SLOT(slotNotifyAddressRequest(const unsigned int, const unsigned int, const unsigned int, 
							const unsigned int, const unsigned short,  const unsigned int, const unsigned short)));

	// connet the signal of connecter so that I will know the network is ready
	//QObject::connect(connecter, SIGNAL(isReady()), packetManager, SLOT(slotConnectReady()));

	// connect network events with packetManager
	QObject::connect(packetManager, SIGNAL(networkException(int)), SLOT(slotNetworkException(int)));
	QObject::connect(packetManager, SIGNAL(packetException(int)), SLOT(slotPacketException(int)));
	QObject::connect(packetManager, SIGNAL(serverBusy()), SLOT(slotServerBusy()));

	// some Tencent server related signals
	//QObject::connect(packetManager, SIGNAL(wrongPassword(QString)), SLOT(slotWrongPassword(QString)));
	QObject::connect(packetManager, SIGNAL(kickedOut(const QString)), SLOT(slotKickedOut(const QString)));
	//QObject::connect(packetManager, SIGNAL(loginOK()), SLOT(slotLoginOK()));
	//QObject::connect(packetManager, SIGNAL(loginNeedVerification( )), 
	//				this, SLOT(slotLoginVerification( )));

	// user information signal
	//QObject::connect(packetManager, SIGNAL(myInfoReady()), SLOT(slotMyInfoReady()));
	QObject::connect(packetManager, SIGNAL(extraInfoReady()), this, SLOT(slotExtraInfoReady()));
	QObject::connect(packetManager, SIGNAL(friendSignatureChanged(const unsigned int, const QDateTime, const QString)), 
			SLOT(slotFriendSignatureChanged(const unsigned int, const QDateTime, const QString)));

	// online status signals
	QObject::connect(packetManager, SIGNAL(onlineReady()), SLOT(slotOnlineReady()));
	QObject::connect(packetManager, SIGNAL(offlineReady()), SLOT(slotOfflineReady()));
	QObject::connect(packetManager, SIGNAL(invisibleReady()), SLOT(slotInvisibleReady()));
	QObject::connect(packetManager, SIGNAL(leaveReady()), SLOT(slotLeaveReady()));	
	QObject::connect(packetManager, SIGNAL(friendStatusChanged(unsigned int)), SLOT(slotFriendStatusChanged(unsigned int)));
	QObject::connect(packetManager, SIGNAL(friendListReady()), SLOT(slotGetOnlineStatus()));

	// friend list related signals
//X 	QObject::connect(packetManager, SIGNAL(friendListReady()), SLOT(slotFriendListReady()));
	QObject::connect(packetManager, SIGNAL(deleteMeReady(bool)), SLOT(slotDeleteMeReply( bool )));
	//QObject::connect(packetManager, SIGNAL(friendGroupsReady()), SLOT(slotFriendGroupsReady()));
	QObject::connect(packetManager, SIGNAL(friendGroupsUploadReady(bool)), SLOT(slotFriendGroupsUploadReady(bool)));
	QObject::connect(packetManager, SIGNAL(deleteBuddyReady(unsigned int, bool)), SLOT(slotDeleteBuddyReady(unsigned int, bool)));

	// add friend
	//QObject::connect(packetManager, SIGNAL(addBuddyReady()), SLOT(slotFriendListReady( )));
	QObject::connect(packetManager, SIGNAL(addAnonymous(const unsigned int, const unsigned short)), SLOT(slotAddAnonymous(const unsigned int, const unsigned short)));

	// budy chatting signal
	QObject::connect(packetManager, 
			SIGNAL(txtMessage(unsigned int, bool, QString, QDateTime, const char,const bool, const bool, const bool, 
				const char , const char, const char)), 
			SLOT(slotTxtMessage(unsigned int, bool, QString, QDateTime, const char,const bool, const bool, const bool, 
				const char , const char, const char)));

	// Qun chatting signal
	QObject::connect(packetManager, SIGNAL(qunTxtMessage(unsigned int, unsigned int, QString, QDateTime, const char , 
			const bool, const bool, const bool, const char, const char, const char)), 
			SLOT(slotReceivedQunMessage( unsigned int, unsigned int, QString, QDateTime, const char, 
			const bool, const bool, const bool, const char, const char, const char)));

	// Qun related signals
	QObject::connect(packetManager, SIGNAL(qunSystemMessageRequest(const unsigned int, QString)), tray, SLOT(newSysMessage()));	
	QObject::connect(packetManager, SIGNAL(qunInfomationReady(const unsigned int, const bool, QString)), 
					SLOT(slotQunInfomationReady(const unsigned int, const bool, QString)));
	QObject::connect(packetManager, SIGNAL(qunSystemMessageRequest(const unsigned int, QString)), 
					SLOT(slotQunSystemMessageRequest(const unsigned int, QString)));
	QObject::connect(packetManager, SIGNAL(qunRequestUpdateDisplay()), this, SLOT(slotLoadQunListReady()));
	QObject::connect(packetManager, SIGNAL(qunRequestQunCardReply(const unsigned int, const bool,
									const unsigned int, QString, const unsigned char, 
									QString, QString, QString, QString)),
				this,	SLOT(slotRequestQunCardReady(const unsigned int, const bool,
									const unsigned int, QString, const unsigned char, 
									QString, QString, QString, QString)));
	QObject::connect(packetManager, SIGNAL(qunExitReply(const unsigned int, const bool, QString)), this, 
			SLOT(slotQunExitReply( const unsigned int , const bool, QString)));

	// file transfer related signals
	QObject::connect(packetManager, SIGNAL(receivedFileRequest(const unsigned int, const unsigned int,
								const QString,  const int, 
								const unsigned char)),
					SLOT(slotReceivedFileRequest(const unsigned int, const unsigned int,
								const QString,   const int, 
								const unsigned char)));
	QObject::connect(packetManager, SIGNAL(receivedFileAccepted(const unsigned int,    const unsigned int,
								const unsigned int, const bool,
								const unsigned char)),
					SLOT(slotReceivedFileAccepted(const unsigned int,   const unsigned int,
								const unsigned int, const bool,
								const unsigned char)));
	QObject::connect(packetManager, SIGNAL(receivedFileAgentInfo(const unsigned int,   const unsigned int ,
								const unsigned int, const unsigned int,
								const unsigned short, const unsigned char *)),
					SLOT(slotReceivedFileAgentInfo(const unsigned int, const unsigned int,
								const unsigned int, const unsigned int,
								const unsigned short, const unsigned char *)));
	QObject::connect(packetManager, SIGNAL(receivedFileNotifyIpEx(const unsigned int, const bool, 
								const unsigned int , const unsigned char,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int )),
					SLOT(slotReceivedFileNotifyIpEx(const unsigned int , const bool , 
								const unsigned int , const unsigned char,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int , const unsigned short ,
								const unsigned int )));
	QObject::connect(fileManager, SIGNAL(notifyAgentRequest(const unsigned int, const unsigned int, const unsigned int, 
							const unsigned int, const unsigned short, const unsigned char)),
			  packetManager, SLOT(doNotifyAgentTransfer(const unsigned int, const unsigned int, const unsigned int,
							const unsigned int, const unsigned short, const unsigned char)));

	// let Adding Manager know connection changed
	addingManager->setPacketManager( packetManager);
	m_SysMsgManager->setPacketManager( packetManager);
}

/**
 *  connect all necessary signals so that Chatting windows could show
 *  notifications in some ways.
 */
///NOTE: must call slotSetupNetwork before calling this method
void EvaMain::slotSetupWindowManager()
{
	if(g_ChatWindowManager) delete g_ChatWindowManager;

	// if user nickname does not exist, default "EVA" will be used	
	QString nick = codec->toUnicode(session->getUser()->getDetails().at(ContactInfo::Info_nick).c_str());
	g_ChatWindowManager = new EvaChatWindowManager(images, settings, nick, session->getQQ());
	EvaPacketManager* packetManager = session->getPacketManager();

	QObject::connect(packetManager, SIGNAL(txtMessage(unsigned int, bool, QString, QDateTime, const char, 
			const bool, const bool, const bool, const char , const char, const char)), g_ChatWindowManager, 
			SLOT(slotReceivedMessage(unsigned int, bool, QString, QDateTime, const char,const bool, const bool, const bool, 
			const char , const char, const char)));

	QObject::connect(packetManager, SIGNAL(sentMessageResult(unsigned int, bool)), g_ChatWindowManager, SLOT(slotSendResult(unsigned int, bool)));
	QObject::connect(session->getConnecter(), SIGNAL(sendMessage( unsigned int, bool)), g_ChatWindowManager, SLOT(slotSendResult(unsigned int, bool)));

	QObject::connect(packetManager, SIGNAL(qunRequestUpdateDisplay()), g_ChatWindowManager, SLOT(slotQunListUpdated()) );
	QObject::connect(g_ChatWindowManager, SIGNAL(requestQunCard(const unsigned int, const unsigned int)),
			packetManager, SLOT(doRequestQunCard( const unsigned int,  const unsigned int)));
	QObject::connect(g_ChatWindowManager, SIGNAL(requestQunRealNames(const unsigned int)), packetManager, SLOT(doRequestQunRealNames(const unsigned int )));
	QObject::connect(g_ChatWindowManager, SIGNAL(requestDetails(const unsigned int)), this, SLOT(slotRequestDetails(const unsigned int)));	
	QObject::connect(g_ChatWindowManager, SIGNAL(sendMessage(const unsigned int , const bool , QString &, const char , 
			const bool , const bool , const bool , 
			const char, const char , const char)), packetManager, 
			SLOT(doSendMessage(const unsigned int , const bool  , QString &, const char, 
			const bool , const bool , const bool , 
			const char , const char, const char)));	
	QObject::connect(g_ChatWindowManager, SIGNAL(requestHistory(const unsigned int)), this, SLOT(slotRequestHistory(const unsigned int)));	
	QObject::connect(g_ChatWindowManager, SIGNAL(requestBuddyQQShow(const unsigned int)), this, SLOT(slotRequestQQShow(const unsigned int)));	
	QObject::connect(g_ChatWindowManager, SIGNAL(requestMyQQShow()), this, SLOT(slotRequestMyQQShow()));

	QObject::connect(g_ChatWindowManager, SIGNAL(fileTransferSend(const unsigned int, const unsigned int, const QList<QString>,
							const QList<unsigned int>, const unsigned char)), 
				SLOT(slotFileTransferSend(const unsigned int, const unsigned int, const QList<QString>,
							const QList<unsigned int>, const unsigned char)));
	QObject::connect(g_ChatWindowManager, SIGNAL(fileTransferAccept(const unsigned int, const unsigned int,
							const QString, const unsigned char)),
				SLOT(slotFileTransferAccept(const unsigned int, const unsigned int,
							const QString, const unsigned char)));
	QObject::connect(g_ChatWindowManager, SIGNAL(fileTransferCancel(const unsigned int, const unsigned int)),
				SLOT(slotFileTransferCancel(const unsigned int, const unsigned int)));
	
	// connect Qun stuff
	QObject::connect(packetManager, SIGNAL(qunTxtMessage(unsigned int, unsigned int, QString, QDateTime, const char , 
			const bool, const bool, const bool, const char, const char, const char)), g_ChatWindowManager, 
			SLOT(slotReceivedQunMessage( unsigned int, unsigned int, QString, QDateTime, const char, 
				const bool, const bool, const bool, const char, const char, const char)));
	QObject::connect(packetManager, SIGNAL(qunSentMessageResult(unsigned int, bool, QString)), g_ChatWindowManager, SLOT(slotQunSendResult(unsigned int, bool, QString)));
	QObject::connect(packetManager, SIGNAL(qunJoinEvent(const unsigned int,
								const short,
								const unsigned int,
								const unsigned int)),
			g_ChatWindowManager, SLOT(slotQunJoinEvent(const unsigned int,
								const short,
								const unsigned int,
								const unsigned int)) );
	QObject::connect(session->getConnecter(), SIGNAL(sendQunMessage(unsigned int, bool, QString)),
			g_ChatWindowManager, SLOT(slotQunSendResult(unsigned int, bool, QString)));
	QObject::connect(g_ChatWindowManager, SIGNAL(requestQunDetails(const unsigned int)), this, SLOT(slotRequestQunDetails(const unsigned int)));
	QObject::connect(g_ChatWindowManager, SIGNAL(sendQunMessage(const unsigned int, QString &, const char , 
			const bool, const bool, const bool, 
			const char, const char, const char)), packetManager, 
			SLOT(doSendQunMessage(const unsigned int, QString &, const char, 
			const bool , const bool, const bool , const char , const char , const char )));
	QObject::connect(g_ChatWindowManager, SIGNAL(requestQunHistory(const unsigned int)), this, SLOT(slotRequestQunHistory(const unsigned int)));
	QObject::connect(g_ChatWindowManager, SIGNAL(requestQunOnlineList(const unsigned int)), packetManager, 
			SLOT(doRequestQunOnlineMembers(const unsigned int)));
	QObject::connect(packetManager, SIGNAL(qunOnlineListReady(const unsigned int)), g_ChatWindowManager, SLOT(slotQunOnlineMembers(const unsigned int)));
	QObject::connect(packetManager, SIGNAL(qunMemberInfoReady(const unsigned int)), g_ChatWindowManager, 
			SLOT(slotQunMemberInfoReady( const unsigned int)));
			
	QObject::connect(picManager, SIGNAL(pictureReady(const unsigned int, const QString, const QString)), g_ChatWindowManager,
			SLOT(slotQunPicReady(const unsigned int, const QString, const QString)));
	QObject::connect(picManager, SIGNAL(pictureSent(const unsigned int, const unsigned int, const unsigned int, const unsigned short)), g_ChatWindowManager, 
			SLOT(slotQunPicSent(const unsigned int, const unsigned int, const unsigned int, const unsigned short)));
	QObject::connect(picManager, SIGNAL(sendErrorMessage(const unsigned int, const QString)), g_ChatWindowManager, 
			SLOT(slotSendImageError(const unsigned int, const QString)));

	QObject::connect(fileManager, SIGNAL(notifyTransferSessionChanged(const unsigned int, const unsigned int, const unsigned int)), g_ChatWindowManager, 
			SLOT(slotChangeFileSessionTo(const unsigned int, const unsigned int, const unsigned int)));
	QObject::connect(fileManager, SIGNAL(notifyTransferNormalInfo(const unsigned int, const unsigned int,
						EvaFileStatus, const QString, const QString,
						const unsigned int, const unsigned char)),
			g_ChatWindowManager, SLOT(slotFileNotifyNormalInfo(const unsigned int, const unsigned int,
						EvaFileStatus, const QString, const QString,
						const unsigned int, const unsigned char)));
	QObject::connect(g_ChatWindowManager, SIGNAL(fileTransferResume(const unsigned int, const unsigned int, const bool)),
			fileManager, SLOT(slotFileTransferResume(const unsigned int, const unsigned int, const bool)));
}

void EvaMain::slotSetupEvaClient()
{	
	g_mainWin->show();
	g_mainWin->clearList();
	loginWin->hide();

	// make sure that slotSetupUser and
	// slotSetupNetwork have been called.
	slotSetupUser();
	slotSetupNetwork();
	slotSetupWindowManager();

	tray->show();

	g_mainWin->updateMyInfo();
	g_mainWin->showInfoFrame(true);
	g_mainWin->resize(settings->getWinSize());
	g_mainWin->move(settings->getWinPoint());
        g_mainWin->updateContacts();
        g_mainWin->updateQuns();
        g_mainWin->updateRecentContacts();

	isClientSet = true;
}

void EvaMain::slotDoLogin()
{
//X 	if(!isClientSet)
//X 		slotSetupEvaClient();
	tray->setLoginWaiting();
	//ServerDetectorPacket::setStep(0);
	//ServerDetectorPacket::setFromIP(0);
	//connecter->redirectTo(QQServer.toIPv4Address(), -1);
	session->login();
}

void EvaMain::slotDoCancel()
{
	if(loginWin->isVisible())
		loginWin->hide();
	if(!g_mainWin->isVisible() && !isClientSet )
		doQuit();
	else{
		if(!tray->isVisible())
		{
			tray->show();
		}
		if(!g_mainWin->isVisible())
			g_mainWin->show();
	}
}

// void EvaMain::slotConnectReady( )
// {
// 	kdDebug() << "[QQ: " << session->getQQ() << "] start logging in server \"" << connecter->getHostAddress().toString()  << "\" ..." << endl;
// 	packetManager->doLogin();
// }

void EvaMain::loginOK( )
{
	initUserLeaveMenu();
	switch(session->getStatus()){
	case Eva_Online:
		g_mainWin->online();
		tray->setOnline();
		break;
	case Eva_Offline:
		g_mainWin->offline();
		tray->setOffline();
		break;
	case Eva_Leave:
		g_mainWin->leave();
		tray->setLeave();
		break;
	case Eva_Invisible:
		g_mainWin->invisible();
		tray->setInvisible();
	}

	//packetManager->doGetUserInfo(session->getQQ());
	
	//loggedIn = true;
	//QTimer::singleShot(5000,this, SLOT(slotGetOnlineStatus()));

	idt->setMaxIdle(settings->getIdleMaxTime());
	printf("idt created %d minutes\n", settings->getIdleMaxTime());
	idt->toggleOverAllIdleDetection(TRUE);
	idt->startIdleDetection();
}

void EvaMain::slotOfflineReady()
{ 
	g_mainWin->offline();
	tray->setOffline();
	//loggedIn = false;
	session->logout();
	picManager->stop();
//X 	if(onlineFriendTimer->isActive())
//X 		onlineFriendTimer->stop();
	
//X 	GetScriptManager()->notifyStatusChange(session->getQQ());
}

void EvaMain::slotOnlineReady( )
{
	g_mainWin->online();
	tray->setOnline();
//X 	GetScriptManager()->notifyStatusChange(session->getQQ());
}

void EvaMain::slotInvisibleReady( )
{
	g_mainWin->invisible();
	tray->setInvisible();
//X 	GetScriptManager()->notifyStatusChange(session->getQQ());
}

void EvaMain::slotLeaveReady( )
{
	g_mainWin->leave();
	tray->setLeave();
//X 	GetScriptManager()->notifyStatusChange(session->getQQ());
}

void EvaMain::slotNetworkException( int/* exp*/)
{
	g_mainWin->offline();
	tray->setOffline();
	numOfLostKeepAlivePackets++; 
//X 	if(onlineFriendTimer->isActive())
//X 		onlineFriendTimer->stop();
	session->logout();
	picManager->stop();
	//loggedIn = false;
	
	QMessageBox::information(0, i18n("Network error occurred, check your network then try Eva later please."),
				i18n("Network"));
}

void EvaMain::slotPacketException( int cmd)
{
	printf("packet exception -- command code: 0x%4x\n", cmd);
	if(cmd == QQ_CMD_SERVER_DETECT){
		slotServerBusy();
		return;
	}

	if(cmd == QQ_CMD_KEEP_ALIVE){
		if(numOfLostKeepAlivePackets < 2)
			numOfLostKeepAlivePackets++; 
			
	}
	if(!session->isLoggedIn() && ( cmd == QQ_CMD_LOGIN || cmd == QQ_CMD_REQUEST_LOGIN_TOKEN )){
		g_mainWin->offline();
		tray->setOffline();
		session->logout();
		
//X 		if(onlineFriendTimer->isActive())
//X 			onlineFriendTimer->stop();
		QMessageBox::information(0, i18n("Cannot login, try later please."), i18n("Eva Login"));
	}
	
	if(numOfLostKeepAlivePackets>=2){
		g_mainWin->offline();
		tray->setOffline();
		session->logout();
		picManager->stop();
		numOfLostKeepAlivePackets = 0;
//X 		if(onlineFriendTimer->isActive())
//X 			onlineFriendTimer->stop();
		//loggedIn = false;
		QMessageBox::information(0, i18n("Lost connection with server, try later please."), i18n("Eva Connection"));
	}
}

// void EvaMain::slotWrongPassword( QString msg)
// {
// 	g_mainWin->offline();
// 	tray->setOffline();
// 	loggedIn = false;
// 	session->setStatus(EvaSession::Eva_Offline);
// 	numOfLostKeepAlivePackets = 0;
// 	//QMessageBox::information(g_mainWin, msg, i18n("Eva Login"));
// 	slotDoChangeUser();
// }

void EvaMain::slotKickedOut(const QString msg)
{
	g_mainWin->offline();
	tray->setOffline();
	//loggedIn = false;
	session->logout();
	numOfLostKeepAlivePackets = 0;
	QMessageBox::critical(g_mainWin, msg, i18n("Eva Login"));
}

// void EvaMain::slotFriendListReady( )
// {
// 	if(!user->isBuddiesLoaded()){
// 		session->getUser()->saveGroupedBuddyList();
// 		session->getUser()->setBuddyLoadedEnabled(true);
// 		if(!m_IsShowingGroups) slotDoDownloadGroups();
// 	}
// 
// 	if(!onlineFriendTimer->isActive())
// 		onlineFriendTimer->start(60000, false);
// }

void EvaMain::slotFriendStatusChanged( unsigned int id )
{
	const QQFriend *frd = (session->getUser()->getFriendList()).getFriend(id);
	if(frd){
		printf("IP is %d\n", frd->getIP());
		switch(frd->getStatus()){
		case QQ_FRIEND_STATUS_ONLINE:
			g_mainWin->changeToOnline(id);
			if(settings->isShowBudyOnlineNotifyEnabled()){
//X                                 tray->showMessageTip( id, codec->toUnicode(frd->getNick().c_str()), i18n( "I am online." ) );
//X 				EvaTipWindow *tip = new EvaTipWindow(images, codec->toUnicode(frd->getNick().c_str()), 
//X 									id, frd->getFace(), i18n("I am online."));
//X 				QObject::connect(tip, SIGNAL(requestChat(const unsigned int)), SLOT(slotRequestChat(const unsigned int)));
//X 				tip->show();
				if(settings->isSoundEnabled())
					global->getSoundResource()->playOnlineSound();
			}
			break;
		case QQ_FRIEND_STATUS_OFFLINE:
			g_mainWin->changeToOffline(id);
			break;
		case QQ_FRIEND_STATUS_LEAVE:
			g_mainWin->changeToLeave(id);
			if(settings->isShowBudyOnlineNotifyEnabled()){
//X                                 tray->showMessageTip( id, codec->toUnicode(frd->getNick().c_str()), i18n( "I am busy ..." ) );
//X 				EvaTipWindow *tip = new EvaTipWindow(images, codec->toUnicode(frd->getNick().c_str()), 
//X 									id, frd->getFace(), i18n("I am busy ..."));
//X 				QObject::connect(tip, SIGNAL(requestChat(const unsigned int)), SLOT(slotRequestChat(const unsigned int)));
//X 				tip->show();
			}
			break;
		case QQ_FRIEND_STATUS_INVISIBLE:
			g_mainWin->changeToInvisible(id);
			break;
		}
	}else{
		printf("buddy %d not in list\n", id);
	}

//X 	GetScriptManager()->notifyStatusChange(id);
}

void EvaMain::slotTxtMessage(unsigned int sender, bool, QString message, QDateTime, const char, 
			const bool, const bool, const bool, 
			const char, const char, const char)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	QString reply="";
	if(sender != session->getQQ() &&
	   session->getStatus() == Eva_Leave && 
	   settings->isAutoReplyEnabled()){
		reply = settings->getSelectedAutoReply();
		packetManager->doSendMessage(sender, false, reply, 9, false, false, false, 0,0,0);
	}
	if(!(g_ChatWindowManager && g_ChatWindowManager->isChatWindowExisted(sender))){	
		g_mainWin->newMessage(sender);
		const QQFriend *frd = (session->getUser()->getFriendList()).getFriend(sender);
		short faceId = 0x0000;
		if(frd)
			faceId = frd->getFace();
		tray->newTxtMessage(sender, codec->toUnicode( frd->getNick().c_str() ), message);
		if(settings->isShowMessageTipEnabled()){
//X                         tray->showMessageTip( sender, codec->toUnicode(frd->getNick().c_str() ), message  );
//X 			EvaTipWindow *tip = new EvaTipWindow(images, codec->toUnicode(frd->getNick().c_str()), sender, faceId, message);
//X 			QObject::connect(tip, SIGNAL(requestChat(const unsigned int)), SLOT(slotRequestChat(const unsigned int)));
//X 			tip->show();
		}
	}
	if( g_ChatWindowManager && g_ChatWindowManager->isChatWindowExisted(sender)){
		g_mainWin->addBuddyToRecentList( sender, true);
	}
	if(settings->isSoundEnabled() && !(g_ChatWindowManager && g_ChatWindowManager->isChatWindowExisted(sender)) )
		global->getSoundResource()->playNewMessage();
}

// void EvaMain::slotFriendGroupsReady( )
// {
// 	m_IsShowingGroups = true;
// 	//slotFriendListReady();
// 	m_IsShowingGroups = false;
// 
// 	g_mainWin->clearList();
// 	g_mainWin->updateContacts();
// 	g_mainWin->updateQuns();
// 	g_mainWin->updateRecentContacts();
// 	if(!user->isBuddiesLoaded()){
// 		session->getUser()->saveGroupedBuddyList();
// 		session->getUser()->setBuddyLoadedEnabled(true);
// 	}
// 	std::list<int>msgs = g_ChatWindowManager->getMessages();
// 	std::list<int>::iterator itr;
// 	for(itr = msgs.begin(); itr != msgs.end(); ++itr){
// 		g_mainWin->newMessage(*itr);
// 	}
// }

void EvaMain::slotDoOnline()
{
    session->online();
}

void EvaMain::slotDoOffline()
{
    session->offline();
}

void EvaMain::slotDoLeave()
{
    session->leave();
}

void EvaMain::slotDoInvisible()
{
    session->invisible();
}

void EvaMain::slotDoLogout()
{
	settings->setWinGeometry(g_mainWin->pos(), g_mainWin->size());
	settings->saveSettings();
//X 	if(onlineFriendTimer->isActive())
//X 		onlineFriendTimer->stop();
	session->logout();
}

void EvaMain::slotDoDownloadGroups()
{
	EvaPacketManager* packetManager = session->getPacketManager();
//X 	session->getUser()->setBuddyLoadedEnabled(false);
	packetManager->doGetGroups();
}

void EvaMain::slotDoUploadGroups()
{
	EvaPacketManager* packetManager = session->getPacketManager();
	packetManager->doUploadGroups();
}

void EvaMain::slotFriendGroupsUploadReady(bool ok)
{
	if(ok)
		QMessageBox::information(g_mainWin, i18n("Uploading grouped friends successfully."), i18n("Eva Upload Groups"));
	else
		QMessageBox::information(g_mainWin, i18n("Uploading grouped friends failed."), i18n("Eva Upload Groups"));
}

void EvaMain::slotDoDownloadBuddies()
{
	EvaPacketManager* packetManager = session->getPacketManager();
//X 	session->getUser()->setBuddyLoadedEnabled(false);
	packetManager->doGetAllFriends();
}

void EvaMain::slotDoChangeUser()
{
	if(g_mainWin->isVisible())
		g_mainWin->hide();
//	if(tray->isVisible())
//		tray->hide();
	if(!loginWin->isVisible())
		loginWin->show();
}

void EvaMain::slotGroupDeleted( const int index)
{
	session->getUser()->removeGroupName(index);
}

// void EvaMain::slotGroupAdded( QString gn, int index)
// {
// 	std::string name = (codec->fromUnicode(gn)).data();
// 	user->newGroup(name);
// 	int newIndex = session->getUser()->getGroupIndexOf(name);
// 	g_mainWin->listView->findGroup(index)->setGroupIndex(newIndex);
// 	session->getUser()->saveGroupedBuddyList();
// }

void EvaMain::slotGroupRenamed( QString gn, int index)
{
	std::string name = (codec->fromUnicode(gn)).data();
	session->getUser()->updateGroupName(name, index);
}

void EvaMain::slotGroupChanged(const unsigned int id, int index)
{
	session->getUser()->updateFriendGroupIndex(id, index);
}

void EvaMain::slotRequestDetails(const unsigned int id)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	QStringList details;
	details.append(QString::number(id));
	
	const QQFriend *frd = (session->getUser()->getFriendList()).getFriend(id); 
	if(frd){
		std::string qq = frd->getUserInformation().at(0);
		if(qq!="-"){
			details = packetManager->convertDetails(frd->getUserInformation());
		}
	}
		
	if(id == session->getQQ()){
		slotRequestSystemSettingWindow();
		
	}else{
		EvaDetailsWindow *detailWin;
		detailWin = new EvaDetailsWindow(details);
		
		QObject::connect(detailWin, SIGNAL(requestUpdate(const unsigned int)), packetManager, SLOT(doGetUserInfo(const unsigned int)));
		QObject::connect(detailWin, SIGNAL(requestQQShow(const unsigned int)), this, SLOT(slotRequestQQShow(const unsigned int)));
	
		QObject::connect(packetManager, SIGNAL(userInfoReady(QStringList)), detailWin, SLOT(slotDetailsUpdated(QStringList)));
		QObject::connect(images, SIGNAL(qqShowReady(const unsigned int)), detailWin, SLOT(slotBuddyQQShowReady(const unsigned int)));
	
		QObject::connect(detailWin, SIGNAL(requestDownloadMemo(const unsigned int )), packetManager, SLOT(doDownloadMemo(const unsigned int )));
		QObject::connect(packetManager, SIGNAL(memoDownloadReply(const MemoItem& )), detailWin, SLOT(slotUpdateMemo(const MemoItem& )));
		QObject::connect(packetManager, SIGNAL(memoNoMemoFound()), detailWin, SLOT(slotNoMemoFound()));
	
		QObject::connect(detailWin, SIGNAL(requestUploadMemo(const unsigned int, const MemoItem&)), packetManager, SLOT(doUploadMemo(const unsigned int, const MemoItem&)));
		QObject::connect(packetManager, SIGNAL( memoUploadReply(const bool)), detailWin, SLOT(slotUploadMemoReply(const bool)));
		
		QObject::connect(detailWin, SIGNAL(memoChanged(const unsigned int, const MemoItem &)), this, SLOT(slotUserMemoChanged(const unsigned int, const MemoItem &)));
	        
		detailWin->show();
	}
}

void EvaMain::slotRequestChat(const unsigned int id)
{
	QQFriend *frd = session->getUser()->getFriendList().getFriend(id);
	g_mainWin->gotMessage(id);
	tray->gotTxtMessage(id);
	g_ChatWindowManager->openChatWindow(frd);
}

void EvaMain::slotRequestSendFile(const unsigned int id)
{
	QQFriend *frd = session->getUser()->getFriendList().getFriend(id);
	g_mainWin->gotMessage(id);
	tray->gotTxtMessage(id);
	g_ChatWindowManager->openChatWindow(frd, true);
}

void EvaMain::slotRequestLevel(const unsigned int id)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	if(session->isLoggedIn())
		packetManager->requestFriendLevel(id);
}

void EvaMain::slotRequestHistory(const unsigned int id)
{
	QQFriend *frd = session->getUser()->getFriendList().getFriend(id);
	QString nick = codec->toUnicode(frd->getNick().c_str());
	
	EvaChatWindow *win = g_ChatWindowManager->getWindow(id);
	EvaHistoryViewer *viewer = new EvaHistoryViewer(id, nick, settings);
	
	//int faceId = atoi(session->getUser()->getDetails().at(ContactInfo::Info_face).c_str());
	unsigned short faceId = frd->getFace();
	QPixmap *face = images->getFaceByID(faceId);
	viewer->setIcon(*face);
	
	if(win){
		QObject::connect(viewer, SIGNAL(historyDoubleClicked(unsigned int, QString, unsigned int, QString, bool,
					 QString, QDateTime, const char, 
					const bool, const bool, const bool, 
					const char, const char, const char)), 
				win, 
				SLOT(slotAddMessage(unsigned int, QString, unsigned int, QString, bool,
					 QString, QDateTime, const char, 
					const bool, const bool, const bool, 
					const char, const char, const char)));
		viewer->move(win->x(), win->y() + win->height() + 25);
	}else{
		QRect scr = QApplication::desktop()->screenGeometry();    
		viewer->move(scr.center() - viewer->rect().center());
	}
	viewer->show();

}

void EvaMain::slotRequestQQShow(const unsigned int id)
{
	images->requestQQShow(id);
}

void EvaMain::slotRequestMyQQShow()
{
	slotRequestQQShow(session->getQQ());
}

void EvaMain::slotQQShowReady( const unsigned int id)
{
	if(id == session->getQQ())
		g_ChatWindowManager->slotMyQQShowReady();
	else
		g_ChatWindowManager->slotBuddyQQShowReady(id);
}

void EvaMain::slotRequestSearch( )
{
	EvaPacketManager* packetManager = session->getPacketManager();
	EvaSearchWindow *win = new EvaSearchWindow(images, packetManager->getNumOnlineUsers(), 0, "Searching");

	QObject::connect(win, SIGNAL(requestUserInfo(const unsigned int)), this, SLOT(slotRequestDetails(const unsigned int)));
	
	QObject::connect(win, SIGNAL(requestSearchUsers(const bool, const QString, const QString, const QString, const QString, const bool)),
			packetManager, SLOT(doSearchUsers(const bool, const QString, const QString, const QString, const QString, const bool)));
	
//	QObject::connect(win, SIGNAL(requestBuddyAuthStatus(const int, const short, const QString&)), 
//				SLOT(slotRequestBuddyAuthStatus(const int, const short, const QString&)));
// 	QObject::connect(win, SIGNAL(requestAddBuddy(const unsigned int, const QString, const unsigned short)),
// 			m_addingManager, SLOT(slotAddBuddy(const unsigned int, const QString, const unsigned short)) );

	QObject::connect(win, SIGNAL(requestAdvancedSearch(const int, const bool, const bool, const int, const int, const int, const int)),
			packetManager, SLOT(doAdvancedSearch(const int, const bool, const bool, const int, const int, const int, const int)));
	
	QObject::connect(packetManager, SIGNAL(searchUsersReady(const bool, const std::list<OnlineUser>)),
			win, SLOT(slotSearchUsersReady(const bool, const std::list<OnlineUser>)));
			
	QObject::connect(packetManager, SIGNAL(advancedSearchReady(const int, const bool, const std::list<AdvancedUser>)),
			win, SLOT(slotAdvancedSearchReady(const int, const bool, const std::list<AdvancedUser>)));
	
	QObject::connect(win, SIGNAL(requestQunSearch(const unsigned int)), packetManager, SLOT(doSearchQun(const unsigned int)));
	QObject::connect(packetManager, SIGNAL(qunSearchReady(const std::list<QunInfo>, QString)),
			win, SLOT(slotQunSearchReady(const std::list<QunInfo>, QString)));
	
	//QObject::connect(win, SIGNAL(requestQunInfo(const unsigned int)), this, SLOT(slotRequestQunDetails(const unsigned int)));
	
	/*QObject::connect(win, SIGNAL(requestUserInfo(const int)), this, SLOT(slotRequestDetails(const int)));
	QObject::connect(win, SIGNAL(requestAddAuthBuddy(const int, const QString &)), 
			packetManager, 
			SLOT(doAddAuthBuddy(const int , const QString &)));
	QObject::connect(win, SIGNAL(requestAddBuddy(const int)), packetManager, SLOT(doAddBuddy(const int)));
	QObject::connect(win, SIGNAL(requestSearchQun(const int)), packetManager, SLOT(doSearchQun(const int)));
	QObject::connect(win, SIGNAL(requestJoinQun(const int)), packetManager, SLOT(doJoinQun(const int)));
	QObject::connect(win, SIGNAL(requestJoinQunAuth(const int, const QString &)), packetManager, SLOT(doJoinQunAuth(const int, const QString &)));
	
	
	QObject::connect(packetManager, SIGNAL(addBuddyReady()), win, SLOT(slotAddBuddyReady()));
	QObject::connect(packetManager, SIGNAL(addBuddyRejected()), win, SLOT(slotAddBuddyRejected()));
	QObject::connect(packetManager, SIGNAL(addBuddyNeedAuth()), win, SLOT(slotBuddyNeedAuth()));
	QObject::connect(packetManager, SIGNAL(addBuddySentToServer(bool)), win, SLOT(slotAddBuddySentToServer(bool)));
	QObject::connect(packetManager, SIGNAL(qunSearchReady(const std::list<QunInfo>, QString)), 
			win, SLOT(slotQunSearchReady(const std::list<QunInfo>, QString)));
	QObject::connect(packetManager, SIGNAL(qunJoinReplyOK(const int)), win, SLOT(slotQunJoinReplyOK(const int)));
	QObject::connect(packetManager, SIGNAL(qunJoinReplyNeedAuth(const int)), win, SLOT(slotQunJoinReplyNeedAuth(const int)));
	QObject::connect(packetManager, SIGNAL(qunJoinReplyDenied(const int)), win, SLOT(slotQunJoinReplyDenied(const int)));
	QObject::connect(packetManager, SIGNAL(qunJoinAuthReply(const int)), win, SLOT(slotQunJoinAuthReply(const int)));*/
	
	QRect scr = QApplication::desktop()->screenGeometry();    
	win->move(scr.center() - win->rect().center());	
	win->show();
}

// void EvaMain::slotSystemMeBeenAdded(unsigned int)
// {
// 	if(settings->isSoundEnabled())
// 		global->getSoundResource()->playSysMessage();
// }

// void EvaMain::slotSystemAddMeRequest(unsigned int, QString)
// {
// 	if(settings->isSoundEnabled())
// 		global->getSoundResource()->playSysMessage();
// }

// void EvaMain::slotSystemAddRequestApproved(unsigned int)
// {
// 	if(settings->isSoundEnabled())
// 		global->getSoundResource()->playSysMessage();
// }

// void EvaMain::slotSystemAddRequestRejected(unsigned int, QString)
// {
// 	if(settings->isSoundEnabled())
// 		global->getSoundResource()->playSysMessage();
// }

/*
// void EvaMain::slotRequestSystemMessage( )
// {
// 	EvaUserSetting::sysMessage m = settings->getLastSysMessage();
// 	if(m.type == 0 && m.from == 0) return;
// 	slotShowSystemMessage( m.messageType, m.type, m.from, m.to, m.message, m.internalQunID);
// }

void EvaMain::slotShowSystemMessage(const short msgType, const uint8_t type, const unsigned int from, const unsigned int to, 
					const QString message, const unsigned int internalQunID)
{	
	EvaNotifyWindow *win = new EvaNotifyWindow(0);
	if(msgType == SYSTEM_MESSAGE_NORMAL){
		switch(type){
		case 200:
			win->slotAddBuddy(from);
		case QQ_MSG_SYS_BEING_ADDED:
			win->slotMeBeenAdded( from);
			break;       
		case QQ_MSG_SYS_ADD_FRIEND_REQUEST:
			win->slotAddMeRequest(from, message);
			break;
		case QQ_MSG_SYS_ADD_FRIEND_APPROVED:
			win->slotAddRequestApproved(from);
			break;
		case QQ_MSG_SYS_ADD_FRIEND_REJECTED:
			win->slotAddRequestRejected(from, message);
			break;
		default:
			if(settings->isShowSystemBroadcastEnabled())
				win->slotOtherNotification(from, message);
		}
		QObject::connect(win, SIGNAL(requestUserInfo(const unsigned int)), this, SLOT(slotRequestDetails(const unsigned int)));
		QObject::connect(win, SIGNAL(requestAddAuthBuddy(const unsigned int, const QString &)), 
				packetManager, SLOT(doAddAuthBuddy(const unsigned int, const QString &)));
		QObject::connect(win, SIGNAL(requestAddBuddy(const unsigned int)), packetManager, SLOT(doAddBuddy(const unsigned int)));
		QObject::connect(win, SIGNAL(requestApproveBuddy(const unsigned int)), packetManager, SLOT(doApproveBuddy(const unsigned int)));
		QObject::connect(win, SIGNAL(requestRejectBuddy(const unsigned int, const QString &)), 
				packetManager, SLOT(doRejectBuddy(const unsigned int, const QString &)));
		
		QObject::connect(packetManager, SIGNAL(addBuddyReady()), win, SLOT(slotAddBuddyReady()));
		QObject::connect(packetManager, SIGNAL(addBuddyRejected()), win, SLOT(slotAddBuddyRejected()));
		QObject::connect(packetManager, SIGNAL(addBuddyNeedAuth()), win, SLOT(slotBuddyNeedAuth()));
		QObject::connect(packetManager, SIGNAL(addBuddySentToServer(bool)), win, SLOT(slotAddBuddySentToServer(bool)));
		
		QObject::connect(packetManager, SIGNAL(systemMeBeenAdded(unsigned int)), win, SLOT(slotMeBeenAdded(unsigned int)));
		QObject::connect(packetManager, SIGNAL(systemAddMeRequest(unsigned int, QString)), win, SLOT(slotAddMeRequest(unsigned int, QString)));
		QObject::connect(packetManager, SIGNAL(systemAddRequestApproved(unsigned int)), win, SLOT(slotAddRequestApproved(unsigned int)));
		QObject::connect(packetManager, SIGNAL(systemAddRequestRejected(unsigned int, QString)), win, SLOT(slotAddRequestRejected(unsigned int, QString)));
	}else{ // now should be Qun messages
		switch(msgType){
		case QQ_RECV_IM_CREATE_QUN:
			win->slotQunCreated(from, to, internalQunID);
			break;
		case QQ_RECV_IM_ADDED_TO_QUN:
			win->slotQunAdded(from, to, internalQunID);
			break;       
		case QQ_RECV_IM_DELETED_FROM_QUN:
			win->slotQunRemoved(from, to, internalQunID);
			break;
		case QQ_RECV_IM_REQUEST_JOIN_QUN:
			win->slotQunJoinRequest(from, to, message, internalQunID);
			break;
		case QQ_RECV_IM_APPROVE_JOIN_QUN:
			win->slotQunRequestApproved(from, to, message, internalQunID);
			break;
		case QQ_RECV_IM_REJECT_JOIN_QUN:
			win->slotQunRequestRejected(from, to, message, internalQunID);
			break;
		}
		QObject::connect(win, SIGNAL(requestUserInfo(const unsigned int)), this, SLOT(slotRequestDetails(const unsigned int)));
		QObject::connect(win, SIGNAL(requestApproveQun(const unsigned int, const unsigned int)), packetManager, SLOT(doApproveQun(const unsigned int, const unsigned int)));
		QObject::connect(win, SIGNAL(requestRejectQun(const unsigned int, const unsigned int, const QString &)), 
				packetManager, SLOT(doRejectQun(const unsigned int,const unsigned int, const QString &)));
		QObject::connect(packetManager, SIGNAL(qunJoinAuthReply(const unsigned int)), win, SLOT(slotQunJoinAuthReply(const unsigned int)));
	}
	if(settings->isShowSystemBroadcastEnabled())
		QObject::connect(packetManager, SIGNAL(systemNotification(int, QString)), win, SLOT(slotOtherNotification(int, QString)));
	QRect scr = KApplication::desktop()->screenGeometry();    
	win->move(scr.center() - win->rect().center());
	win->show();
}
*/

void EvaMain::slotRequestSystemMessages( )
{
	EvaSysHistoryViewer *viewer = new EvaSysHistoryViewer( settings);
	QObject::connect(viewer, SIGNAL(historyDoubleClicked( const unsigned short,
								const unsigned char,
								const unsigned int,
								const unsigned int,
								const QString,
								const unsigned int,
								const unsigned int,
								const unsigned char * , const unsigned short,
								const unsigned char *, const unsigned short)),
			m_SysMsgManager, SLOT(showSysMessage(  const unsigned short,
								const unsigned char,
								const unsigned int,
								const unsigned int,
								const QString,
								const unsigned int,
								const unsigned int,
								const unsigned char *, const unsigned short ,
								const unsigned char *, const unsigned short)));
	
	QRect scr = QApplication::desktop()->screenGeometry();    
	viewer->move(scr.center() - viewer->rect().center());
	viewer->show();
}

void EvaMain::slotRequestDelete( const unsigned int id)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	packetManager->doDeleteBuddy(id);
}

void EvaMain::slotDeleteBuddyReady( unsigned int id, bool ok)
{
	if(ok)
		g_mainWin->deleteBuddy(id);
	else{
		QMessageBox::information(g_mainWin, QString(i18n("Delete buddy \"%1\" failed.")).arg(id), i18n("Eva Delete Buddy"));
	}
}

void EvaMain::slotRequestSystemSettingWindow( )
{
	EvaPacketManager* packetManager = session->getPacketManager();
	QStringList details;
	if(session->getUser()->getDetails().at(0)!="-"){
		details = packetManager->convertDetails(session->getUser()->getDetails());
	
		EvaSysSettingWindow *win = new EvaSysSettingWindow(details, images, settings, g_mainWin);
	
		QObject::connect(win, SIGNAL(requestUpdate(const unsigned int)), packetManager, SLOT(doGetUserInfo(const unsigned int)));
		QObject::connect(win, SIGNAL(settingChanged()), SLOT(slotUserSettingChanged()));
		QObject::connect(win, SIGNAL(faceSizeChanged()), SLOT(slotFaceSizeChanged()));
		QObject::connect(packetManager, SIGNAL(userInfoReady(QStringList)), win, SLOT(slotDetailsUpdated(QStringList)));
	
		QObject::connect(win, SIGNAL(requestUpdateSignature(const QString)), packetManager, SLOT(doModifySignature( const QString))); 
		QObject::connect(win, SIGNAL(requestDeleteSignature()), packetManager,SLOT(doDeleteSignature()));

		QObject::connect(win, SIGNAL(requestMyAuthQuestionSetting()),
				packetManager, SLOT(doRequestMyAuthQuestionSetting()));
		QObject::connect(win, SIGNAL(requestMyUpdateQuestion(const QString &, const QString &)),
				packetManager, SLOT(doSetMyAuthQuestion(const QString &, const QString &)));

		QObject::connect(packetManager, SIGNAL(receivedMyAuthSettings(const unsigned char,
							const QString&, const QString&)), 
				win, SLOT(slotReceivedMyAuthSettings(const unsigned char,
								const QString&,const QString &)));
		QObject::connect(packetManager, SIGNAL(updateAuthSettingResult(const unsigned char, 
								const unsigned char)),
				win, SLOT(slotUpdateAuthSettingResult(const unsigned char,
								const unsigned char)));
	
		QObject::connect(win, SIGNAL(requestModify(QStringList, QString, QString)),
				packetManager, SLOT(doModifyDetails(QStringList, QString, QString)));	
		QObject::connect(packetManager, SIGNAL(modifyInfo(bool)), win, SLOT(slotUpdateResult(bool)));
	
		QObject::connect(packetManager, SIGNAL(signatureModifyReply(const bool)), win, SLOT(slotSignatureReply(const bool)));
		QObject::connect(packetManager, SIGNAL(signatureDeleteReply(const bool)), win, SLOT(slotSignatureReply(const bool)));
	
		QObject::connect(win, SIGNAL(requestQQShow(const unsigned int)), this, SLOT(slotRequestQQShow(const unsigned int)));
		QObject::connect(images, SIGNAL(qqShowReady(const unsigned int)), win, SLOT(slotBuddyQQShowReady(const unsigned int)));
	
	
		packetManager->doGetUserInfo(session->getQQ());
		QRect scr = QApplication::desktop()->screenGeometry();
	
		win->move(scr.center() - win->rect().center());	
		win->show();
		printf("sys setting shown now\n");
	}
	else{
		printf("get user details ERROR,stop init setting window\n!");
	}
}

void EvaMain::slotDeleteMeFrom( const unsigned int id)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	if(deleteFromQQ != -1) return;  // make sure we only delete from one friend's list at a moment
	const QQFriend *frd = session->getUser()->getFriendList().getFriend(id);
	if(!frd) return;
	QString nick = codec->toUnicode(frd->getNick().c_str());
	if(QMessageBox::question( g_mainWin, QString(i18n("put \"%1\" into Blacklist, are you sure?")).arg(nick),
			i18n( "Eva Blacklist")) == QMessageBox::No)
		return;
	deleteFromQQ = id;
	packetManager->doDeleteMeFrom(id);
}

void EvaMain::slotDeleteMeReply( bool ok)
{
	if(ok){
		g_mainWin->changeGroupTo(deleteFromQQ, session->getUser()->getBlackIndex());
		slotGroupChanged(deleteFromQQ, session->getUser()->getBlackIndex());
		
		deleteFromQQ = -1;
	}else
		QMessageBox::information(g_mainWin, QString(i18n("Blacklist operation on \"%1\" failed.")).arg(deleteFromQQ),
				i18n("Eva Blacklist"));
}

void EvaMain::slotRequestAddBuddy(const unsigned int id)
{
	//NOTE: this method is no longer used, use adding manager to add buddy instead
	if(QMessageBox::question( g_mainWin,
		QString(i18n("add \"%1\" into your Buddy list?")).arg(id), i18n( "Eva Add Buddy")) == QMessageBox::No)
		return;
		
	//slotShowSystemMessage(SYSTEM_MESSAGE_NORMAL, 200, id, id,"");
}

/*
void EvaMain::slotMyInfoReady( )
{	
// 	QString myNick = codec->toUnicode(session->getUser()->getDetails().at(ContactInfo::Info_nick).c_str());
// 	if(myNick.isNull()) myNick = "";
// 	g_mainWin->setCaption(myNick + " - Eva");
// 	int myFaceId = atoi(session->getUser()->getDetails().at(ContactInfo::Info_face).c_str());
// 	QPixmap *face = images->getFaceByID(myFaceId);
// 	if(session->getUser()->hasUserHead()){
// 		QPixmap *uhPic = images->getUserHeadPixmap(session->getQQ()); // color pixmap
// 		if(uhPic) face = uhPic;
// 	}
// 	g_mainWin->setIcon(*face);
// 	g_mainWin->setMainInfo(session->getQQ(), myNick, face?face:images->getFace(0));
	g_mainWin->updateBuddy(session->getQQ());
	g_ChatWindowManager->setMyName(myNick, session->getQQ());
	
	if(!user->isBuddiesLoaded())
		slotDoDownloadBuddies();
}
*/
void EvaMain::slotQunInfomationReady(unsigned int id, const bool /*ok*/, QString /*errorMsg*/)
{
	const Qun *qun = session->getUser()->getQunList()->getQun(id);
	session->getUser()->saveQunList();
	if(qun){
		g_mainWin->updateQun(id);
	}
}

void EvaMain::slotRequestQunDetails( const unsigned int id)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	Qun *qun = session->getUser()->getQunList()->getQun(id);
	QunDetailsWindow *win;
	if(!qun){
		qun = new Qun(id);
		win = new QunDetailsWindow(qun, false);
		delete qun;
		qun = NULL;
	} else
		win = new QunDetailsWindow(qun);
	
	QObject::connect(win, SIGNAL(requestQunInfo(const unsigned int)), 
			packetManager, SLOT(doRequestQunInfo(const unsigned int)));
	if(!qun) packetManager->doRequestQunInfo( id);

	QObject::connect(packetManager, SIGNAL(qunInfomationReady(const unsigned int, const bool, QString)), 
			win, SLOT(slotQunInfomationReady(const unsigned int, const bool, QString)));

	QObject::connect(win, SIGNAL(requestQunCard(const unsigned int, const unsigned int)), packetManager, 
			SLOT(doRequestQunCard( const unsigned int,  const unsigned int)));
	QObject::connect(win, SIGNAL(requestModifyQunInfo(const unsigned int, unsigned char, unsigned short, QString, QString, QString)), 
			packetManager, SLOT(doModifyQunInfo(const unsigned int, unsigned char, unsigned short, QString, QString, QString)));
	QObject::connect(packetManager, SIGNAL(qunModifyInfoReply(const unsigned int, const bool, QString)), 
					win, SLOT(slotModifyQunInfo(const unsigned int, const bool, QString)));
					
	QObject::connect(win, SIGNAL(requestModifyQunCard(const unsigned int, const unsigned int, QString, unsigned char, QString, QString, QString)), 
			packetManager, SLOT(doModifyQunCard(const unsigned int,  const unsigned int, QString, unsigned char, QString, QString, QString)));
	QObject::connect(packetManager, SIGNAL(qunModifyQunCardReply(const unsigned int, const bool, const unsigned int, QString)), 
			win, SLOT(slotModifyQunCardReply( const unsigned int, const bool, const unsigned int, QString)));
			
	QObject::connect(win, SIGNAL(requestQunSetAdmin(const unsigned int, const unsigned int, const bool)), 
			packetManager, SLOT(doQunSetAdmin(const unsigned int, const unsigned int, const bool)));
	QObject::connect(packetManager, SIGNAL(qunSetAdminReply(const unsigned int , const bool , const unsigned int, const bool , QString)), 
			win, SLOT(slotSetAdmin(const unsigned int, const bool, const unsigned int, const bool, QString)));

	QObject::connect(win, SIGNAL(requestQunTransfer(const unsigned int, const unsigned int)), 
			packetManager, SLOT(doQunTransfer(const unsigned int, const unsigned int)));
	QObject::connect(packetManager, SIGNAL(qunTransferReply(const unsigned int , const bool, const unsigned int, QString)), 
			win, SLOT(slotTransferQun(const unsigned int, const bool, const unsigned int, QString)));
			
	QObject::connect(win, SIGNAL(requestModifyQunMembers(const unsigned int, const std::list<unsigned int>, const bool)), 
			packetManager, SLOT(doModifyQunMembers(const unsigned int, const std::list<unsigned int>, const bool)));
	QObject::connect(packetManager, SIGNAL(qunModifyQunMembersReply(const unsigned int, const bool, QString)), 
			win, SLOT(slotModifyQunMembers(const unsigned int, const bool, QString)));

	QObject::connect(win, SIGNAL(requestAddBuddy(const unsigned int , const QString, const unsigned short )),
			addingManager, SLOT(slotAddBuddy(const unsigned int , const QString, const unsigned short )));
	
	QObject::connect(packetManager, SIGNAL(qunMemberInfoReady(const unsigned int)), 
			win, SLOT(slotMembersUpdated(const unsigned int)));
			
	QObject::connect(win, SIGNAL(requestUpdateQunMessageSettings(const unsigned int, const signed char)), 
				SLOT(slotUpdateQunMessageSettings(const unsigned int, const signed char)));
	QRect scr = QApplication::desktop()->screenGeometry();    
	win->move(scr.center() - win->rect().center());
	win->show();
}

void EvaMain::slotRequestQunCardReady(const unsigned int id, const bool ok, const unsigned int qq, QString realName, const unsigned char gender, 
					QString phone, QString email, QString memo, QString msg)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	Qun *qun = session->getUser()->getQunList()->getQun(id);
	QunDetailsWindow *win = new QunDetailsWindow(qun);
	win->slotReceivedQunCard(id, ok, qq, realName, gender, phone, email, memo, msg);
	
	QObject::connect(win, SIGNAL(requestQunInfo(const unsigned int)), 
			packetManager, SLOT(doRequestQunInfo(const unsigned int)));
	QObject::connect(packetManager, SIGNAL(qunInfomationReady(const unsigned int, const bool, QString)), 
			win, SLOT(slotQunInfomationReady(const unsigned int, const bool, QString)));
	
	QObject::connect(win, SIGNAL(requestModifyQunInfo(const unsigned int, unsigned char, unsigned short, QString, QString, QString)), 
			packetManager, SLOT(doModifyQunInfo(const unsigned int, unsigned char, unsigned short, QString, QString, QString)));
	QObject::connect(packetManager, SIGNAL(qunModifyInfoReply(const unsigned int, const bool, QString)), 
					win, SLOT(slotModifyQunInfo(const unsigned int, const bool, QString)));
					
	QObject::connect(win, SIGNAL(requestModifyQunCard(const unsigned int, const unsigned int, QString, unsigned char, QString, QString, QString)), 
			packetManager, SLOT(doModifyQunCard(const unsigned int,  const unsigned int, QString, unsigned char, QString, QString, QString)));
	QObject::connect(packetManager, SIGNAL(qunModifyQunCardReply(const unsigned int, const bool, const unsigned int, QString)), 
			win, SLOT(slotModifyQunCardReply( const unsigned int, const bool, const unsigned int, QString)));
			
	QObject::connect(win, SIGNAL(requestQunSetAdmin(const unsigned int, const unsigned int, const bool)), 
			packetManager, SLOT(doQunSetAdmin(const unsigned int, const unsigned int, const bool)));
	QObject::connect(packetManager, SIGNAL(qunSetAdminReply(const unsigned int , const bool , const unsigned int, const bool , QString)), 
			win, SLOT(slotSetAdmin(const unsigned int, const bool, const unsigned int, const bool, QString)));

	QObject::connect(win, SIGNAL(requestQunTransfer(const unsigned int, const unsigned int)), 
			packetManager, SLOT(doQunTransfer(const unsigned int, const unsigned int)));
	QObject::connect(packetManager, SIGNAL(qunTransferReply(const unsigned int , const bool, const unsigned int, QString)), 
			win, SLOT(slotTransferQun(const unsigned int, const bool, const unsigned int, QString)));
			
	QObject::connect(win, SIGNAL(requestModifyQunMembers(const unsigned int, const std::list<unsigned int>, const bool)), 
			packetManager, SLOT(doModifyQunMembers(const unsigned int, const std::list<unsigned int>, const bool)));
	QObject::connect(packetManager, SIGNAL(qunModifyQunMembersReply(const unsigned int, const bool, QString)), 
			win, SLOT(slotModifyQunMembers(const unsigned int, const bool, QString)));
	
	QObject::connect(win, SIGNAL(requestAddBuddy(const unsigned int , const QString, const unsigned short )),
			addingManager, SLOT(slotAddBuddy(const unsigned int , const QString, const unsigned short )));

	QObject::connect(packetManager, SIGNAL(qunMemberInfoReady(const unsigned int)), 
			win, SLOT(slotMembersUpdated(const unsigned int)));
	
	QObject::connect(win, SIGNAL(requestUpdateQunMessageSettings(const unsigned int, const signed char)), 
				SLOT(slotUpdateQunMessageSettings( const unsigned int, const signed char)));
	QRect scr = QApplication::desktop()->screenGeometry();    
	win->move(scr.center() - win->rect().center());
	win->show();
}

void EvaMain::slotRequestQunHistory( const unsigned int id)
{
	Qun *qun = session->getUser()->getQunList()->getQun(id);
	QString qName = i18n("Qun");
	if(qun){
		QunInfo info = qun->getDetails();
		qName = codec->toUnicode(info.getName().c_str());
	}
	
	EvaQunChatWindow *win = g_ChatWindowManager->getQunWindow(id);
	EvaHistoryViewer *viewer = new EvaHistoryViewer(id, qName, settings, true);
	
	int faceId = atoi(session->getUser()->getDetails().at(ContactInfo::Info_face).c_str());
	QPixmap *face = images->getFaceByID(faceId);
	viewer->setIcon(*face);
	
	if(win){
		QObject::connect(viewer, SIGNAL(historyDoubleClicked(unsigned int, QString, unsigned int, QString, bool,
					 QString, QDateTime, const char, 
					const bool, const bool, const bool, 
					const char, const char, const char)), 
				win, 
				SLOT(slotAddMessage(unsigned int, QString, unsigned int, QString, bool,
					 QString, QDateTime, const char, 
					const bool, const bool, const bool, 
					const char, const char, const char)));
		viewer->move(win->x(), win->y() + win->height() + 25);
	}else{
		QRect scr = QApplication::desktop()->screenGeometry();    
		viewer->move(scr.center() - viewer->rect().center());
	}
	viewer->show();
}

void EvaMain::slotRequestQunChat( const unsigned int id)
{
	Qun *qun = session->getUser()->getQunList()->getQun(id);
	if(!qun) return;
	g_mainWin->gotQunMessage(id);
	tray->gotTxtMessage(id);
	g_ChatWindowManager->openQunChatWindow(qun);
}

void EvaMain::slotReceivedQunMessage( unsigned int qunID, unsigned int senderQQ, QString msg, QDateTime /*time*/, const char /*fontSize*/, 
				const bool /*u*/, const bool /*i*/, const bool /*b*/, const char /*blue*/, const char /*green*/, const char /*red*/)
{
	//note that: Qun never show tip
	Qun *qun = session->getUser()->getQunList()->getQun(qunID);
        QString message = msg;
	if(!qun) return;
	if(senderQQ == session->getQQ()) return;
	if(!(g_ChatWindowManager && g_ChatWindowManager->isQunChatWindowExisted(qunID))){
		bool needSound = false;
		switch(qun->getMessageType()){
			case Qun::Popup:
				slotRequestQunChat(qunID);
				g_mainWin->addQunToRecentList( qunID);
				needSound = true;
				break;
			case Qun::Numbers:
				g_mainWin->newQunMessage(qunID); 
				break;
			default:
				g_mainWin->newQunMessage(qunID);
                                const FriendItem* frd = qun->getMemberDetails( senderQQ );
                                if ( frd ) {
                                    std::string name = frd->getQunRealName( );
                                    name = (!name.empty() ) ? name : frd->getNick();
                                    tray->newQunMessage(qunID, codec->toUnicode(name.c_str()), message ); 
                                }
				needSound = true;
		}
		if(settings->isSoundEnabled() && needSound)
			global->getSoundResource()->playNewMessage();
	}
}

void EvaMain::slotQunSystemMessageRequest( const unsigned int /*sender*/, QString /*message*/)
{
	if(settings->isSoundEnabled())
		global->getSoundResource()->playSysMessage();
}

void EvaMain::slotQunPictureReady( const unsigned int id, const QString fileName , const QString tmpFileName)
{
	g_ChatWindowManager->slotQunPicReady(id, fileName, tmpFileName);
}

void EvaMain::slotShotcutKeyPressed( )
{
	if(!tray) return;
	int id = tray->getSenderID();
	
	QQFriend *frd = session->getUser()->getFriendList().getFriend(id);
	if(frd){
		g_mainWin->gotMessage(id);
		tray->gotTxtMessage(id);
		g_ChatWindowManager->openChatWindow(frd);
		return;
	}
	
	Qun *qun = session->getUser()->getQunList()->getQun(id);
	if(qun){
		g_mainWin->gotQunMessage(id);
		tray->gotTxtMessage(id);
		g_ChatWindowManager->openQunChatWindow(qun);
		return;
	}
/*	EvaUserSetting::sysMessage m = settings->getLastSysMessage();
	if(!(m.type == 0 && m.from == 0)){
		m_SysMsgManager->showSysMessage( m.messageType, m.type, m.from, m.to, m.message, m.internalQunID);
		return;
	}*/	
	m_SysMsgManager->showSysMessage();

	if(!g_mainWin->isVisible()) g_mainWin->show();
//X 	else
//X 		KWin::forceActiveWindow(g_mainWin->winId());
}

void EvaMain::slotQunExitReply( const unsigned int id, const bool ok, QString msg)
{
	QString qunName;
	Qun *qun = session->getUser()->getQunList()->getQun(id);
	if(!qun) qunName = QString::number(id);
	else
		qunName = codec->toUnicode(qun->getDetails().getName().c_str());
	
	QString desc;
	if(ok)
		desc = QString(i18n("You have exited the Qun \"%1\" successfully.")).arg(qunName);
	else
		desc = QString(i18n("Exit Qun \"%1\" failed\n%2")).arg(qunName).arg(msg);
	
	QMessageBox::information( g_mainWin, desc, i18n("Qun Operation"));
}

void EvaMain::slotDoQunExit( const unsigned int id )
{
	EvaPacketManager* packetManager = session->getPacketManager();
	QString qunName;
	Qun *qun = session->getUser()->getQunList()->getQun(id);
	if(!qun) qunName = QString::number(id);
	else
		qunName = codec->toUnicode(qun->getDetails().getName().c_str());
	
	QunInfo info = qun->getDetails();
	QString msg;
	if(info.getExtID() != 0 && info.getCreator() == session->getQQ()){
		msg = QString(i18n("You are the creator of Qun \"%1\", if you exit this Qun this Qun will be deleted, are you sure?")).arg(qunName);
	}else{
		msg = QString(i18n("Are you sure you want to exit Qun \"%1\"?")).arg(qunName);
	}
	if(QMessageBox::question(g_mainWin, msg, i18n("Qun Operation")) != QMessageBox::No)
		packetManager->doQunExit(id);
}

void EvaMain::slotQunCreateFailed(QString msg)
{
	QMessageBox::information(g_mainWin, msg, i18n("Qun Operation"));
}

void EvaMain::slotQunCreate( )
{
	EvaPacketManager* packetManager = session->getPacketManager();
	EvaQunCreateWindow *win = new EvaQunCreateWindow();
	QObject::connect(win, SIGNAL(doQunCreate(const QString &, const unsigned short, const unsigned char, const QString &, 
					const QString &, const std::list<unsigned int> &)), 
		packetManager, SLOT(doQunCreate(const QString &, const unsigned short, const unsigned char, const QString &, 
					const QString &, const std::list<unsigned int> &)));
	QObject::connect(packetManager, SIGNAL(qunCreateDone(const unsigned int)), win, SLOT(slotQunCreateDone(const unsigned int)));
	QObject::connect(packetManager, SIGNAL(qunCreateFailed(QString)), win, SLOT(slotQunCreateFailed(QString)));
	win->show();
}

void EvaMain::slotFriendSignatureChanged( const unsigned int qq, const QDateTime /*time*/, const QString /*signature*/ )
{
	
	const QQFriend *frd = (session->getUser()->getFriendList()).getFriend(qq);
	if(frd){
		if(g_mainWin) g_mainWin->updateBuddy(qq);
		QString nick = codec->toUnicode(frd->getNick().c_str());
		nick = nick + "(" + i18n("signature") + ")" ;
//X                 tray->showMessageTip( qq, nick, signature  );
//X 		EvaTipWindow *tip = new EvaTipWindow(images, nick, qq, frd->getFace(), signature);
//X 		QObject::connect(tip, SIGNAL(requestChat(const unsigned int)), SLOT(slotRequestChat(const unsigned int)));
//X 		tip->show();
	}else{
		printf("buddy %d not in list\n", qq);
	}	
}

/** we simply request the qun information which will update all details */
// void EvaMain::slotRequestQunMembers( const unsigned int id)
// {
// 	packetManager->doRequestQunInfo(id);
// }

void EvaMain::slotUpdateQunMessageSettings( const unsigned int id, const signed char type )
{
	Qun *qun = session->getUser()->getQunList()->getQun(id);
	if(!qun) return;
	qun->setMessageType((Qun::MessageType)type);
	printf("EvaMain::slotUpdateQunMessageSettings -- qun messageType: %d\n", 0xff&type);
	session->getUser()->saveQunList();
}

void EvaMain::slotUpdateShortcut( )
{
//X 	if(accelKey) delete accelKey;
//X 	accelKey  = new KGlobalAccel(this, "Eva Short Key");
//X 	accelKey->insert("Eva Global Key", i18n("Eva Global Key"), 
//X 			i18n("This action allows you to read your new Eva message globally."),
//X 			settings->getMessageShortcut(), KKey::QtWIN+Key_F12, this, SLOT(slotShotcutKeyPressed( )));
//X 	if(!accelKey->updateConnections())
//X 		kdDebug() << "F12 Key registered failed!" << endl;
        /**
         * Fix this: this work went the main window get minimized! -- George Ang
         */
//X         accelKey = new QShortcut( g_mainWin );
//X         accelKey->setKey( session->getUser()->getSetting( )->getMessageShortcut( ) );
//X         connect( accelKey, SIGNAL( activated() ), tray, SLOT( popMessageOrMainWin() ) );
}

void EvaMain::slotFaceSizeChanged()
{
	g_mainWin->slotFaceSizeChanged();
}

void EvaMain::slotUserSettingChanged( )
{
	slotUpdateShortcut();
	settings->setNeedRepaint(true);
	// set face size will reload all face images as well
	global->setFaceSize(QSize(settings->getFaceSize()));
	uhManager->initiate(settings->getFaceSize());
	if(images){
		images->setUserHeadImage(uhManager->getOnList(), uhManager->getOffList());
	}	
	QString newSoundDir=settings->getSoundDir();
	if(!newSoundDir.isEmpty()){
		global->getSoundResource()->setSoundDir(newSoundDir);
	}
	g_mainWin->repaint();
	initUserLeaveMenu();
	idt->setMaxIdle(settings->getIdleMaxTime());
	idt->stopIdleDetection();
	idt->startIdleDetection();
				
}
/*
void EvaMain::slotLoginProcessReady( )
{
	printf("user login process done!\n");
	packetManager->lastLoginStep();
	//QTimer::singleShot( 500,  connecter, SLOT(slotClientReady()));
}*/

void EvaMain::slotAddAnonymous(const unsigned int id, const unsigned short/* face*/)
{
	if(g_mainWin){
// 		QString nick = QString::number(id);
// 		g_mainWin->addBuddy(nick, (int)id, QString(""), EvaSession::getAnonymousIndex(),
// 				images->getFaceByID(face, true), images->getFaceByID(face, false));
		g_mainWin->addBuddy(id);
	}
}

void EvaMain::slotExtraInfoReady( )
{
	if(uhManager){
		uhManager->stop();
		uhManager->wait();
		delete uhManager;
		uhManager = NULL;
	}
	
	uhManager = new EvaUHManager(this, settings->getUserHeadDir());
	uhManager->initiate(global->getFaceSize());
	
	std::list<unsigned int> list = session->getUser()->getFriendList().getUserHeadList(session->getQQ(), session->getUser()->hasUserHead());
	if(list.size()){
		uhManager->setQQList(list);
		uhManager->start();
	}
	
}

void EvaMain::customEvent( QEvent * e )
{
// 	if(e->type() == EvaUserHeadReadyEvent){
// 		EvaUHReadyEvent *event = (EvaUHReadyEvent *)e;
// 		unsigned int id = event->getQQ();
// 		QImage imgOn = event->getOnImage();
// 		QImage imgOff = event->getOffImage();
// 		if(images){
// 			images->addUserHeadImage(id, imgOn, imgOff);
// 			if(id == session->getQQ()){
// 				slotMyInfoReady( );
// 			}
// 			g_mainWin->updateBuddy(id);
// 		}
// 	}
        printf( "recieved custom event \n" );
	switch(e->type()){
		case EvaUserHeadReadyEvent:
		{
			EvaUHReadyEvent *event = (EvaUHReadyEvent *)e;
			unsigned int id = event->getQQ();
			QImage imgOn = event->getOnImage();
			QImage imgOff = event->getOffImage();
			if(images){
				images->addUserHeadImage(id, imgOn, imgOff);
// 				if(id == session->getQQ()){
// 					slotMyInfoReady( );
// 				}
				g_mainWin->updateBuddy(id);
			}
			
		}
			break;
		case EvaEventNotify:
                        printf( "[EvaMain] EvaEventNotify ready\n" );
			dispatchEvaEvent((EvaNotifyEvent *)e);
			break;
		default:break;
	}
}

void EvaMain::slotUserMemoChanged(const unsigned int id ,const MemoItem &memo)
{
//X 	QString memoName = memo.name.c_str();
	session->getUser()->setContactMemo(id, memo);
	g_mainWin->updateBuddy(id);
}

void EvaMain::slotModifyMemo(const unsigned int id )
{
	EvaPacketManager* packetManager = session->getPacketManager();
	QStringList details;
	details.append(QString::number(id));
	
	const QQFriend *frd = (session->getUser()->getFriendList()).getFriend(id); 
	if(frd){
		std::string qq = frd->getUserInformation().at(0);
		if(qq!="-")
			details = packetManager->convertDetails(frd->getUserInformation());
	}
	
	EvaDetailsWindow *detailWin;
	detailWin = new EvaDetailsWindow(details);
	QObject::connect(detailWin, SIGNAL(requestUpdate(const unsigned int)), packetManager, SLOT(doGetUserInfo(const unsigned int)));
	QObject::connect(detailWin, SIGNAL(requestQQShow(const unsigned int)), this, SLOT(slotRequestQQShow(const unsigned int)));
	QObject::connect(packetManager, SIGNAL(userInfoReady(QStringList)), detailWin, SLOT(slotDetailsUpdated(QStringList)));
	QObject::connect(images, SIGNAL(qqShowReady(const unsigned int)), detailWin, SLOT(slotBuddyQQShowReady(const unsigned int)));
	
	QObject::connect(detailWin, SIGNAL(requestDownloadMemo(const unsigned int )), packetManager, SLOT(doDownloadMemo(const unsigned int )));
	QObject::connect(packetManager, SIGNAL(memoDownloadReply(const MemoItem& )), detailWin, SLOT(slotUpdateMemo(const MemoItem& )));
	QObject::connect(packetManager, SIGNAL(memoNoMemoFound()), detailWin, SLOT(slotNoMemoFound()));
	QObject::connect(detailWin, SIGNAL(requestUploadMemo(const unsigned int, const MemoItem&)), 
				packetManager, SLOT(doUploadMemo(const unsigned int, const MemoItem&)));
	QObject::connect(packetManager, SIGNAL( memoUploadReply(const bool)), detailWin, SLOT(slotUploadMemoReply(const bool)));
	QObject::connect(detailWin, SIGNAL(memoChanged(const unsigned int, const MemoItem &)), 
				SLOT(slotUserMemoChanged(const unsigned int, const MemoItem &)));
	     
	detailWin->twTabMain->setCurrentPage(2);
	detailWin->show();
}

void EvaMain::slotFileTransferSend( const unsigned int receiver, const unsigned int sessionId,
				const QList<QString> fileNameList,
				const QList<unsigned int> sizeList, const unsigned char transferType)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	if(session->getUser()->getFriendList().hasFriend(receiver)){
		QList<QString>::const_iterator iter = fileNameList.begin();
		if(iter == fileNameList.end()) return;
		QString fileName = *iter;
		QString dir = fileName.left(fileName.findRev("/"));
		//printf("evamain::slotFileTransferSend -- dir:%s\n", dir.ascii());
		QString file = fileName.right(fileName.length() - fileName.findRev("/") - 1);
		unsigned int size = sizeList.first();
		if(packetManager)
			packetManager->doSendFileUdpRequest(receiver, file, size, sessionId, transferType);
		QList<QString> dirList;
		QList<QString> fileList;
		for(iter = fileNameList.begin(); iter != fileNameList.end(); ++iter){
			dir = (*iter).left((*iter).findRev("/"));
			file = (*iter).right((*iter).length() - (*iter).findRev("/") - 1);
			dirList.append(dir);
			fileList.append(file);
		}
		// create udp upload session
		if(fileManager){
			fileManager->newSession(receiver, sessionId, dirList, fileList, sizeList, false, transferType);
		}
	}
}

void EvaMain::slotFileTransferAccept( const unsigned int receiver, const unsigned int sessionId, const QString dir,
					const unsigned char transferType)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	printf("EvaMain::slotFileTransferAccept -- session: %d\tdir:%s\n", sessionId, dir.ascii());
	if(fileManager && !(dir.isEmpty()))
		fileManager->saveFileTo(receiver, sessionId, dir);
	if(packetManager)
		packetManager->doAcceptFileRequest(receiver, sessionId, session->getLoginWanIp(), transferType);
}

void EvaMain::slotFileTransferCancel( const unsigned int  receiver, const unsigned int sessionId)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	if(packetManager){
		unsigned char type = fileManager->getTransferType(receiver, sessionId);
		if(type == QQ_TRANSFER_IMAGE) return;
		if(packetManager)
			packetManager->doCancelFileRequest(receiver, sessionId, 
					fileManager->getTransferType(receiver, sessionId));
	}
	if(fileManager)
		fileManager->stopThread(receiver, sessionId);
}

void EvaMain::slotReceivedFileRequest( const unsigned int id,  const unsigned int sessionId,
					const QString file, const int size,
					const unsigned char transferType)
{
	QQFriend *frd = session->getUser()->getFriendList().getFriend(id);
	if(g_ChatWindowManager && frd){
		if(fileManager){
			printf("EvaMain::slotReceivedFileRequest: -- new session: %d\n", sessionId);
			QList<QString> dirList;
			QList<QString> fileList;
			QList<unsigned int> sizeList;
			switch(transferType){
			case QQ_TRANSFER_IMAGE:
				dirList.append(settings->getPictureCacheDir());
				tray->gotTxtMessage( id); // since the chat win will show very shortly,
							// we cancel the face flashing
				break;
			case QQ_TRANSFER_FILE:
				dirList.append(sysSetting->defaultDownloadDir());
				break;
			default:
				dirList.append(sysSetting->defaultDownloadDir());
			}
			fileList.append(file);
			sizeList.append(size);
			// create udp download session, and wait for ip notification from your friend
			fileManager->newSession(id, sessionId, dirList, fileList, sizeList, true, transferType);
			g_ChatWindowManager->slotReceivedFileRequest(frd, sessionId, file, size, transferType);
		}
	}
}

void EvaMain::slotReceivedFileAccepted(const unsigned int id, const unsigned int sessionId,
					const unsigned int /*ip*/, const bool isAccepted,
					const unsigned char transferType)
{
	if(g_ChatWindowManager){
		g_ChatWindowManager->slotReceivedFileAccepted(id, sessionId, isAccepted, transferType);
	}
	if(!fileManager) return;
	if(isAccepted){
 		// here we start udp upload session while buddy is waiting your ip notification
		// if user is using proxy, we send ip notification directly
                EvaNetworkPolicy policy = loginWin->getNetworkPolicy();
		if(policy.getConnectionType() == HTTP_Proxy)
			slotNotifyAddressRequest(id, sessionId, 0, 0, 0, 0, 0);
		else
			fileManager->startSession(id, sessionId);
	} else {
		fileManager->stopThread(id, sessionId);
	}
}

void EvaMain::slotNotifyTransferStatus(const unsigned int id, const unsigned int session, const unsigned int size,
				const unsigned int bytesSent, const int time)
{
	printf("Buddy:%d\tsession:0x%8x size:%d\t sent:%d\ttime:%d secs\n", id, session, size, bytesSent, time);
//X 	QURL url;
//X 	url.setPath(fileManager->getFileName(id, session, true));
//X 	if(g_ChatWindowManager) 
//X 		g_ChatWindowManager->slotFileStatusNotification(id, session, url, size, bytesSent, time);
}

void EvaMain::slotReceivedFileAgentInfo( const unsigned int id, const unsigned int oldSession,
				const unsigned int newSession, const unsigned int ip, 
				const unsigned short port, const unsigned char * key)
{
	if(g_ChatWindowManager)
		g_ChatWindowManager->slotChangeFileSessionTo(id, oldSession, newSession);
	if(fileManager){
		// buddy ask you to download the file from relay server
		fileManager->changeToAgent(id, oldSession);printf("EvaMain::slotReceivedFileAgentInfo -- changeToAgent\n");
		fileManager->changeSessionTo(id, oldSession, newSession);printf("EvaMain::slotReceivedFileAgentInfo -- changeSessionTo\n");
		fileManager->setBuddyAgentKey(id, newSession, key);printf("EvaMain::slotReceivedFileAgentInfo -- setBuddyAgentKey\n");
		fileManager->setAgentServer(id, newSession, ip, port);printf("EvaMain::slotReceivedFileAgentInfo -- setAgentServer\n");
		fileManager->startSession(id, newSession); // we start agent download session
		printf("EvaMain::slotReceivedFileAgentInfo -- startSession\n");
		printf("EvaMain::slotReceivedFileAgentInfo: -- ip:%s\tport:%d\told:%d\tnew:%d\n", 
			QHostAddress(ip).toString().ascii(), port, oldSession, newSession);
	}
}

void EvaMain::slotReceivedFileNotifyIpEx( const unsigned int id, const bool isSender,
				const unsigned int sessionId,const unsigned char transferType,
				const unsigned int wanIp1, const unsigned short /*wanPort1*/,
				const unsigned int /*wanIp2*/, const unsigned short /*wanPort2*/,
				const unsigned int /*wanIp3*/, const unsigned short /*wanPort3*/,
				const unsigned int /*lanIp1*/, const unsigned short /*lanPort1*/,
				const unsigned int /*lanIp2*/, const unsigned short /*lanPort2*/,
				const unsigned int /*lanIp3*/, const unsigned short /*lanPort3*/,
				const unsigned int /*syncIp*/, const unsigned short /*syncPort*/,
				const unsigned int /*syncSession*/ )
{
	EvaPacketManager* packetManager = session->getPacketManager();
	if(isSender) {  // true means buddy started udp upload session
		// we should start udp download session, but now
		// we simply ask buddy to use relay server :)
		packetManager->doNotifyIpEx(id, false, sessionId, transferType, 0,0,  0,0,  0,0,  0,0,  0,0,  0,0,
					 0,0,0);
	}else{ // if I am the sender, I should start udp session connecting to buddy directly
		// but now I ask relay server to do the job
		fileManager->changeToAgent(id, sessionId);
		fileManager->updateIp(id, sessionId, wanIp1);
		fileManager->startSession(id, sessionId); // here we start agent upload session
	}
}

void EvaMain::slotNotifyAddressRequest( const unsigned int id, const unsigned int sessionId, const unsigned int /*synSession*/, 
				const unsigned int /*synIp*/, const unsigned short /*synPort*/,
				const unsigned int /*myIp*/, const unsigned short /*myPort*/)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	bool ok = false;
	if(fileManager){
		bool isSender = fileManager->isSender(id, sessionId, &ok);
		if(!ok) return;
		unsigned char type = fileManager->getTransferType(id, sessionId);
		// we send our ip information to buddy
// 		packetManager->doNotifyIpEx(id, isSender, session, 
// 					session->getUser()->getLoginWanIp(), session->getUser()->getLoginWanPort()+100,
// 					session->getUser()->getLoginWanIp(), session->getUser()->getLoginWanPort()+101,
// 					myIp, myPort,
// 					0x0a010113, session->getUser()->getLoginLanPort()+15,
// 					0, 0, 0x0a010113, session->getUser()->getLoginLanPort()+50, synIp, synPort, synSession);
		packetManager->doNotifyIpEx(id, isSender, sessionId, type, 
					0, 0,
					0, 0,
					0, 0,
					0, 0,
					0, 0, 0, 0, 0, 0, 0);
	}
}

void EvaMain::slotIdleTimeUp()
{
	if ( (session->getStatus() == Eva_Online) && (!inIdleStatus)) {
		slotDoLeave();
		inIdleStatus = true;
	}

	return;
}

void EvaMain::slotIdleBack()
{
	if ( (session->getStatus() == Eva_Leave) && (inIdleStatus) ) {
		slotDoOnline();
		inIdleStatus = false;
	}

	return;
}

// void EvaMain::slotLoginVerification( )
// {
// 	EvaLoginVeriWindow *win = new EvaLoginVeriWindow();
// 	QObject::connect(win, SIGNAL(changeImage()), packetManager, SLOT(doRequestLoginTokenEx()));
// 	QObject::connect(win, SIGNAL(sendVerifyCode(const QString &)), 
// 					packetManager, SLOT(doRequestLoginTokenEx( const QString &) ) );
// 
// 	QObject::disconnect(packetManager, SIGNAL(loginNeedVerification()), 
// 					this, SLOT(slotLoginVerification()));
// 	QObject::connect(packetManager, SIGNAL(loginNeedVerification()), 
// 					win, SLOT(slotImageReady()));
// 	QObject::connect(packetManager, SIGNAL(loginVerifyPassed()), 
// 					win, SLOT(slotVerifyPassed())); 
// 	
// 	QRect scr = KApplication::desktop()->screenGeometry();
// 	//QRect scr = g_mainWin->rect();
// 	win->move(scr.center() - win->rect().center());
// 	win->show();
// }

void EvaMain::slotBuddyAdded( const unsigned int id, const QString /*nick*/, const unsigned short /*face*/, const int /*group*/)
{
	EvaPacketManager* packetManager = session->getPacketManager();
	//g_mainWin->addBuddy(nick, id, QString(""), group, images->getFaceByID(face, true), images->getFaceByID(face, false));
	g_mainWin->addBuddy(id);
	packetManager->doGetUserInfo(id);
}

//X void EvaMain::changeToOnline( )
//X {
//X 	slotDoOnline();
//X }
//X 
//X void EvaMain::changeToOffline( )
//X {
//X 	slotDoOffline();
//X }
//X 
//X void EvaMain::changeToLeave( )
//X {
//X 	slotDoLeave();
//X }
//X 
//X void EvaMain::changeToInvisible( )
//X {
//X 	slotDoInvisible();
//X }
//X 
//X void EvaMain::changeNick( QString nick )
//X {
//X 	EvaPacketManager* packetManager = session->getPacketManager();
//X 	QStringList details = packetManager->convertDetails( session->getUser()->getDetails() );
//X 	details[ContactInfo::Info_nick] = nick;
//X 	packetManager->doModifyDetails(details);
//X }
//X 
//X void EvaMain::changeSignature( QString contents )
//X {
//X 	EvaPacketManager* packetManager = session->getPacketManager();
//X 	packetManager->doModifySignature( contents );
//X }

//X void EvaMain::sendToContact( unsigned int id, QString msg )
//X {
//X 	EvaPacketManager* packetManager = session->getPacketManager();
//X 	packetManager->doSendMessage(id , true , msg);
//X }
//X 
//X void EvaMain::sendToQun( unsigned int ext, QString msg)
//X {
//X 	EvaPacketManager* packetManager = session->getPacketManager();
//X 	Qun *qun = session->getUser()->getQunList()->getQunByExtID( ext);
//X 	if(qun)
//X 		packetManager->doSendQunMessage(qun->getQunID(), msg);
//X }
//X 
//X void EvaMain::textReady( unsigned int id, QString text, bool isQun )
//X {
//X 	if(isQun){
//X 		Qun *qun = session->getUser()->getQunList()->getQunByExtID( id);
//X 		if(qun){
//X 			EvaQunChatWindow *win = g_ChatWindowManager->getQunWindow(qun->getQunID());
//X 			if(win)
//X 				win->appendText(text);
//X 		}
//X 	} else {
//X 		EvaChatWindow *win = g_ChatWindowManager->getWindow(id);
//X 		if(win)
//X 			win->appendText(text);
//X 	}
//X }
//X 
//X void EvaMain::imageReady( unsigned int id, QString path, bool isQun )
//X {
//X 	QString destDir = EvaMain::settings->getPictureCacheDir();
//X 	
//X 	if(isQun){
//X 		Qun *qun = session->getUser()->getQunList()->getQunByExtID( id);
//X 		if(qun){
//X 			EvaQunChatWindow *win = g_ChatWindowManager->getQunWindow(qun->getQunID());
//X 			if(win){
//X 				QString destFile = EvaHelper::generateCustomSmiley(path, destDir);
//X 				if(destFile.isEmpty()) return;
//X 				win->slotAddImageToInputEdit(destFile);
//X 			}
//X 		}
//X 	} else {
//X 		EvaChatWindow *win = g_ChatWindowManager->getWindow(id);
//X 		if(win){			
//X 			QPixmap cache(path);
//X 			if(cache.isNull()) return;
//X 	
//X 			QString destFile = QUuid::createUuid().toString().upper() + ".jpg";
//X 			QString destFullName = destDir + "/" + destFile ;
//X 			cache.save(destFullName, "JPEG", 100);
//X 			win->slotAddImageToInputEdit(destFile);
//X 		}
//X 	}
//X }
//X 
//X void EvaMain::openAddFriendDialog( unsigned int id )
//X {
//X 	addingManager->slotAddBuddy( id);
//X }
//X 
void EvaMain::dispatchEvaEvent( EvaNotifyEvent * e )
{
	assert(e);
	switch(e->m_id){
		case E_Err:
			g_mainWin->UpdateLoginInfo(E_Err + 1, s_ENotify[E_Err]);
			break;
		case E_SvrConnected:
			g_mainWin->UpdateLoginInfo(E_SvrConnected + 1, s_ENotify[E_SvrConnected]);
			break;
		case E_SvrRedirect:
			g_mainWin->UpdateLoginInfo(E_SvrRedirect + 1, s_ENotify[E_SvrRedirect]);
			break;
		case E_SvrLoginFailed:
			g_mainWin->UpdateLoginInfo(E_SvrLoginFailed + 1, s_ENotify[E_SvrLoginFailed]);
			break;
		case E_PwWrong:
			g_mainWin->UpdateLoginInfo(E_PwWrong + 1, s_ENotify[E_PwWrong]);
			g_mainWin->offline();
			tray->setOffline();
			//loggedIn = false;
			session->logout();
			numOfLostKeepAlivePackets = 0;
			QMessageBox::information(g_mainWin, e->m_desc, i18n("Eva Login"));
			slotDoChangeUser();
			break;
		case E_LoggedIn:
		{
			g_mainWin->UpdateLoginInfo(E_LoggedIn + 1, s_ENotify[E_LoggedIn]);			
//X 			KConfig* config = new KConfig( QDir::homeDirPath() + "/.eva/eva.cfg" );
//X 			config->setGroup("General");
//X 			config->writeEntry("Last Login IP", QQServer.toString());
//X 			switch(loginWin->getConnectionType()){
//X 				case EvaLoginWindow::UDP:
//X 					config->writeEntry("Server Type", "UDP");
//X 					break;
//X 				case EvaLoginWindow::TCP:
//X 				case EvaLoginWindow::HTTP_Proxy:
//X 					config->writeEntry("Server Type", "TCP");
//X 					break;
//X 				default: break;
//X 			}
//X 			config->sync();
//X 			delete config;
			global->getEvaServers()->resetDefaultIP();
			loginOK();
		}
			break;
		case E_MyInfo:
			g_mainWin->UpdateLoginInfo(E_MyInfo + 1, s_ENotify[E_MyInfo]);
			g_mainWin->updateMyInfo();
//X 			g_ChatWindowManager->setMyName(codec->toUnicode(session->getUser()->getDetails().at(ContactInfo::Info_nick).c_str()), 
//X 					session->getQQ());
			break;
		case E_KeyFileAgent:
			g_mainWin->UpdateLoginInfo(E_KeyFileAgent + 1, s_ENotify[E_KeyFileAgent]);
			break;
		case E_ContactsDownloading:
			g_mainWin->UpdateLoginInfo(E_ContactsDownloading + 1, s_ENotify[E_ContactsDownloading]);
			break;
		case E_ContactsDone:
			g_mainWin->UpdateLoginInfo(E_ContactsDone + 1, s_ENotify[E_ContactsDone]);
			//m_contactManager->fetchGroupNames();
			break;
		case E_GroupNameDownloadDone:
			g_mainWin->UpdateLoginInfo(E_GroupNameDownloadDone + 1, s_ENotify[E_GroupNameDownloadDone]);
			//m_contactManager->fetchGroupedFriends();
			break;
		case E_GroupContactsReceived:
			g_mainWin->UpdateLoginInfo(E_GroupContactsReceived + 1, s_ENotify[E_GroupContactsReceived]);
			break;
		case E_GroupContactsDone:
			g_mainWin->UpdateLoginInfo(E_GroupContactsDone + 1, s_ENotify[E_GroupContactsDone]);
			break;
		case E_QunInfoFinished:
		{	
			g_mainWin->UpdateLoginInfo(E_QunInfoFinished + 1, s_ENotify[E_QunInfoFinished]);
			const Qun *qun = session->getUser()->getQunList()->getQun((unsigned int)(e->m_param));
			if(qun){
				g_mainWin->updateQun((unsigned int)(e->m_param));
			}
			//m_contactManager->fetchQunMembersInfo( (unsigned int)(e->m_param));
		}
			break;
		case E_QunMemberFinished:
			g_mainWin->UpdateLoginInfo(E_QunMemberFinished + 1, s_ENotify[E_QunMemberFinished]);
			g_ChatWindowManager->slotQunMemberInfoReady((unsigned int)(e->m_param));
			break;
		case E_ContactDetailDone:
			break;
		case E_SignatureDone:
			break;
		case E_AllSignatureDone:
			break;
		case E_LevelDone:
			break;
		case E_LoginProcessDone:
                        // 			std::list<int>msgs = g_ChatWindowManager->getMessages();
                        // 			std::list<int>::iterator itr;
                        // 			for(itr = msgs.begin(); itr != msgs.end(); ++itr){
                        // 				g_mainWin->newMessage(*itr);
                        // 			}
                        g_mainWin->UpdateLoginInfo(E_LoginProcessDone + 1, s_ENotify[E_LoginProcessDone]);

                        //			g_mainWin->clearList();
                        g_mainWin->updateMyInfo();//update my info first

                        g_mainWin->updateContacts();
                        g_mainWin->updateQuns();
                        g_mainWin->updateRecentContacts();

                        //X 			GetScriptManager()->findScripts();

                        //X 			if(!onlineFriendTimer->isActive())
                        //X 				onlineFriendTimer->start(60000, false);
                        break;
		case E_LoginFinished:
                        {
                            printf( "[EvaMain] E_LoginFinished ready\n" );
                            g_mainWin->UpdateLoginInfo(E_LoginFinished + 1, s_ENotify[E_LoginFinished]);
                            g_mainWin->updateMyInfo();
                            // should be everyting ready(contacts, groups, Quns)
                            QString nick = codec->toUnicode(session->getUser()->getDetails().at(ContactInfo::Info_nick).c_str());
                            int faceId = atoi(session->getUser()->getDetails().at(ContactInfo::Info_face).c_str());
                            g_ChatWindowManager->setMyName(codec->toUnicode(session->getUser()->getDetails().at(ContactInfo::Info_nick).c_str()), 
                                    session->getQQ());
                            g_ChatWindowManager->setMyName(nick, session->getQQ());


                            tray->changeToolTip(session->getQQ(), nick, faceId);			
                            g_mainWin->showInfoFrame(false);

                            g_mainWin->ShowTab(g_mainWin->m_buddyTabKey);

                            //EvaScriptManager::instance();// very urgly way to ensure dcop category Tools registered
                            // we should start getting buddy list
                        }
			break;

		default:break;
	}
}

//X void EvaMain::slotShowScriptManager( )
//X {
//X //X 	GetScriptManager()->show();
//X }
