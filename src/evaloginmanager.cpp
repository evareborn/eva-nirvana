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
//X #include <kapplication.h>
//X #include <kdebug.h>


//X 
//X EvaLoginManager *GetLoginManager()
//X {
//X 	static EvaLoginManager manager( g_eva );
//X 	return &manager;
//X }

EvaLoginManager::EvaLoginManager(EvaSession* session, EvaConnecter* connecter, EvaContactManager* contactManager, EvaPacketManager* packetManager )
        : session( session )  
        , connecter( connecter )
        , contactManager( contactManager )
	, packetManager(packetManager)
	, m_isLoggedIn(false)
	, m_veriWin(NULL)
{

        QObject::connect( connecter, SIGNAL( isNetworkReady() ), SLOT( slotDoLogin() ) );
        QObject::connect( packetManager, SIGNAL( fileAgentInfoReady() ), SLOT( fileAgentInfoReady() ) );
}

void EvaLoginManager::notifyEvent(const int eId, const QString &msg)
{
        printf( "EvaLoginManager::notifyEvent\n" );
	EvaNotifyEvent *e = new EvaNotifyEvent(eId);
	e->m_desc = msg;
	
//X 	QApplication::postEvent(g_eva, e);
	QApplication::sendEvent(EvaMain::getInstance(), e);
	QApplication::sendEvent(contactManager, e);
 
        if ( QApplication::hasPendingEvents() ) {
            printf( "has pending events!\n" );
        }

}

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
	notifyEvent(E_Err);	
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
	notifyEvent(E_LoggedIn);
	//We don't care about the reply of this command	
//X 	packetManager->doChangeStatus(session->getStatus());

//X         packetManager->doChangeStatus(status );
	packetManager->doGetUserInfo(session->getQQ());

}

void EvaLoginManager::wrongPassword( QString msg )
{	
	//KMessageBox::information(0, msg, i18n("Eva Login"));
	loginStatus = EStart;
	notifyEvent(E_PwWrong, msg);
	//TODO
}

void EvaLoginManager::loginNeedRedirect(const unsigned int fromIp, const unsigned int ip, const short port )
{
	ServerDetectorPacket::nextStep();
	ServerDetectorPacket::setFromIP(fromIp );
	
	loginStatus = EStart;
	notifyEvent(E_SvrRedirect);
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
//X 		notifyEvent(E_KeyFileAgent);
//X 	}
}

void EvaLoginManager::myInfoReady( const ContactInfo info)
{
	loginStatus = EUserInfo;
	EvaUser *user = session->getUser();
	if(user)
		user->setDetails(info);
//X 	notifyEvent(E_MyInfo);
        m_isLoggedIn = true;
        notifyEvent(E_LoginFinished);
        contactManager->fetchContacts();
        packetManager->doGetOnlineFriends();
//X 	packetManager->doRequestFileAgentKey();
}

