/***************************************************************************
 *   Copyright (C) 2007 by yunfan                                          *
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

#include "evaloginmanager.h"
#include "evacontactmanager.h"
#include "evapacketmanager.h"
#include "evafilemanager.h"
#include "evauser.h"
#include <qhostaddress.h>
#include <assert.h>
#include "evaguimain.h"
#include "evaqtutil.h"
#include "evaapi.h"
#include "evasession.h"
#include "evasetting.h"
#include "evaconnecter.h"
#include "evaloginveriwindow.h"
#include <QApplication>

EvaLoginManager::EvaLoginManager(EvaUser* user, EvaConnecter* connecter, EvaContactManager* contactManager, EvaPacketManager* packetManager )
        : user( user )  
        , connecter( connecter )
        , contactManager( contactManager )
	, packetManager(packetManager)
	, m_isLoggedIn(false)
	, m_veriWin(NULL)
{
    status = Eva_Offline;
    QObject::connect( connecter, SIGNAL( isNetworkReady() ), SLOT( slotDoLogin() ) );
    QObject::connect( packetManager, SIGNAL( fileAgentInfoReady() ), SLOT( fileAgentInfoReady() ) );
}
//X 
//X void EvaLoginManager::emit loginProcessUpdate(const int eId, const QString &msg)
//X {
//X         printf( "EvaLoginManager::emit loginProcessUpdate\n" );
//X 	EvaNotifyEvent *e = new EvaNotifyEvent(eId);
//X 	e->m_desc = msg;
//X 	
//X //X 	QApplication::postEvent(g_eva, e);
//X 	QApplication::sendEvent(EvaMain::getInstance(), e);
//X 	QApplication::sendEvent(contactManager, e);
//X  
//X         if ( QApplication::hasPendingEvents() ) {
//X             printf( "has pending events!\n" );
//X         }
//X 
//X }

void EvaLoginManager::setPacketManager( EvaPacketManager * pm )
{
	assert(pm);
	packetManager = pm;
	
	QObject::disconnect(m_veriWin, 0, 0, 0);
	if(m_veriWin) delete m_veriWin;
	m_veriWin = new EvaLoginVeriWindow();
	QObject::connect(m_veriWin, SIGNAL(changeImage()), packetManager, SLOT(doRequestLoginTokenEx()));
	QObject::connect(m_veriWin, SIGNAL(sendVerifyCode(const QString &)), 
									 packetManager, SLOT(doRequestLoginTokenEx( const QString &) ) );

// 	QObject::disconnect(packetManager, SIGNAL(loginNeedVerification()), 
// 											this, 				SLOT(slotLoginVerification()));
// 	QObject::connect(packetManager, 	SIGNAL(loginNeedVerification()), 
// 									 win, 					SLOT(slotImageReady()));
// 	QObject::connect(packetManager, 	SIGNAL(loginVerifyPassed()), 
// 									 win, 					SLOT(slotVerifyPassed())); 
}

void EvaLoginManager::serverBusy( )
{
	loginStatus = EStart;
	emit loginProcessUpdate(E_Err);	
}

void EvaLoginManager::slotDoLogin()
{
    QHostAddress server = connecter->getSocketIp();
    assert(packetManager);
    m_isLoggedIn = false;

    //X 	kdDebug() << "[EvaLoginManager] login starting" << endl;
    //EvaSetting *s = EvaMain::global->getEvaSetting();	
    ServerDetectorPacket::setStep(0);
    ServerDetectorPacket::setFromIP(0);

    packetManager->redirectTo(server.toIPv4Address(), -1);
    loginStatus = EStart;
}
 
void EvaLoginManager::login()
{
    slotDoLogin();
}
 
void EvaLoginManager::logout()
{
    m_isLoggedIn = false; 
    packetManager->doLogout();
}

void EvaLoginManager::setStatus(const UserStatus status)
{ 
    this->status = status;
}

UserStatus EvaLoginManager::getStatus() const
{ 
    return status;
}

char EvaLoginManager::getStatusCode(const UserStatus status) 
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

void EvaLoginManager::online()
{
    if(status == Eva_Offline){
        status = Eva_Online;
//X         login();
    }else
        packetManager->doChangeStatus(Eva_Online);
}
 
void  EvaLoginManager::offline()
{
    status = Eva_Offline;
    packetManager->doChangeStatus(Eva_Offline);
}
 
void EvaLoginManager::leave()
{
	if(status == Eva_Offline){
		setStatus(Eva_Leave);
//X 		login();
	}else
		packetManager->doChangeStatus(Eva_Leave);

}

void EvaLoginManager::invisible()
{
    if(status == Eva_Offline){
        setStatus(Eva_Invisible);
//X         login();
    }else
        packetManager->doChangeStatus(Eva_Invisible);
}

void EvaLoginManager::loginVerification( )
{
//	EvaLoginVeriWindow *win = new EvaLoginVeriWindow();
// 	QObject::connect(win, SIGNAL(changeImage()), packetManager, SLOT(doRequestLoginTokenEx()));
// 	QObject::connect(win, SIGNAL(sendVerifyCode(const QString &)), 
// 					 packetManager, SLOT(doRequestLoginTokenEx( const QString &) ) );
// 
// 	QObject::disconnect(packetManager, SIGNAL(loginNeedVerification()), 
// 						this, 				SLOT(slotLoginVerification()));
// 	QObject::connect(packetManager, 	SIGNAL(loginNeedVerification()), 
// 					 win, 					SLOT(slotImageReady()));
// 	QObject::connect(packetManager, 	SIGNAL(loginVerifyPassed()), 
// 					 win, 					SLOT(slotVerifyPassed())); 
	if(!m_veriWin) return;
	
	if(m_veriWin->isVisible())
		m_veriWin->slotImageReady();
	else{
//X 		QRect scr = KApplication::desktop()->screenGeometry();
//X 		//QRect scr = g_mainWin->rect();
//X 		m_veriWin->move(scr.center() - m_veriWin->rect().center());
		m_veriWin->show();
	}
}

void EvaLoginManager::verifyPassed( )
{
	loginStatus = ELoginToken;
	if(m_veriWin) m_veriWin->slotVerifyPassed();
	
	packetManager->doLogin();
}

void EvaLoginManager::loginOK( )
{	
        printf( "login Ok!\n" );
	loginStatus = ELogin;
	emit loginProcessUpdate(E_LoggedIn);
	//We don't care about the reply of this command	
//X 	packetManager->doChangeStatus(session->getStatus());

//X         packetManager->doChangeStatus(status );
	packetManager->doGetUserInfo(user->getQQ());

}

void EvaLoginManager::wrongPassword( QString /*msg*/ )
{	
	//KMessageBox::information(0, msg, i18n("Eva Login"));
	loginStatus = EStart;
	emit loginProcessUpdate(E_PwWrong);
	//TODO
}

void EvaLoginManager::loginNeedRedirect(const unsigned int fromIp, const unsigned int ip, const short port )
{
	ServerDetectorPacket::nextStep();
	ServerDetectorPacket::setFromIP(fromIp );
	
	loginStatus = EStart;
	emit loginProcessUpdate(E_SvrRedirect);
	packetManager->redirectTo( ip, port);
	
}

void EvaLoginManager::fileAgentInfoReady( )
{
//X 	printf( "[EvaLoginManager] file agent info ready\n" );
//X 	if(session->getFileManager()){
//X 		session->getFileManager()->setMyBasicInfo(Packet::getFileAgentKey(),
//X 						 Packet::getFileAgentToken(), 
//X 						Packet::getFileAgentTokenLength());
//X 		loginStatus = EFileAgentKey;
//X 		m_isLoggedIn = true;
//X             contactManager->fetchContacts();
//X 		emit loginProcessUpdate(E_KeyFileAgent);
//X 	}
}

void EvaLoginManager::myInfoReady( const ContactInfo info)
{
	loginStatus = EUserInfo;
	if(user)
		user->setDetails(info);
//X 	emit loginProcessUpdate(E_MyInfo);
        m_isLoggedIn = true;
        emit loginProcessUpdate(E_LoginFinished);
        contactManager->fetchContacts();
        packetManager->doGetOnlineFriends();
//X 	packetManager->doRequestFileAgentKey();
}

void EvaLoginManager::setLoginWanIp(const unsigned int ip)
{ 
    loginIp = ip;
}
void EvaLoginManager::setLoginWanPort(const unsigned short port)
{ 
    loginPort = port;
}

void EvaLoginManager::setLoginLanIp(const unsigned int ip) 
{ 
    lanIp = ip;
}
 
void EvaLoginManager::setLoginLanPort(const unsigned short port)
{ 
    lanPort = port;
}

void EvaLoginManager::setLastLoginIp(const unsigned int ip)
{ 
    lastLoginIp = ip;
}

void EvaLoginManager::setLastLoginTime(const unsigned int time)
{ 
    lastLoginTime = time;
}

unsigned int EvaLoginManager::getLoginWanIp() const
{ 
    return loginIp;
}

unsigned short EvaLoginManager::getLoginWanPort() const
{ 
    return loginPort;
}

unsigned int EvaLoginManager::getLoginLanIp() const
{ 
    return lanIp;
}

unsigned short EvaLoginManager::getLoginLanPort() const
{ 
    return lanPort;
}

unsigned int EvaLoginManager::getLastLoginIp() const
{ 
    return lastLoginIp;
}

unsigned int EvaLoginManager::getLastLoginTime() const
{ 
    return lastLoginTime;
}
void EvaLoginManager::addLoginVerifyInfo( const GraphicVerifyCode & info )
{
	codeList.push_back(info);
}

GraphicVerifyCode EvaLoginManager::getLoginVerifyInfo( )
{
	GraphicVerifyCode code;
	if(codeList.empty()) return code;
	return codeList.front();
}


GraphicVerifyCode EvaLoginManager::getNextLoginVerifyInfo( )
{
	GraphicVerifyCode code;
	if(codeList.empty()) return code;
	code = codeList.front();
	codeList.pop_front();
	return code;
}
 
bool EvaLoginManager::isLoggedIn() const
{
    return m_isLoggedIn;
}

void EvaLoginManager::clearAllVerifyCodes( )
{
	if(codeList.size())
		codeList.clear();
}


int EvaLoginManager::getNumVerifyCodes() const
{ 
    return codeList.size();
}

bool EvaLoginManager::isLoginNeedVerify() const
{ 
    return (codeList.size() != 0);
}

