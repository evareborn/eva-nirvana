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

#include "evapacketmanager.h"
#include <qhostaddress.h>
#include <assert.h>
#include "evamain.h"
#include "evasetting.h"
#include "evaconnecter.h"
#include "evaapi.h"
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

EvaLoginManager::EvaLoginManager(EvaMain* evaapp )
	: m_packetManager(NULL)
        , g_eva( evaapp )  
	, m_isLoggedIn(false)
	, m_veriWin(NULL)
{
}

void EvaLoginManager::notifyEvent(const int eId, const QString &msg)
{
        printf( "EvaLoginManager::notifyEvent\n" );
	EvaNotifyEvent *e = new EvaNotifyEvent(eId);
	e->m_desc = msg;
	
//X 	QApplication::postEvent(g_eva, e);
	QApplication::sendEvent(g_eva, e);
 
        if ( QApplication::hasPendingEvents() ) {
            printf( "has pending events!\n" );
        }
	

}

void EvaLoginManager::setPacketManager( EvaPacketManager * pm )
{
	assert(pm);
	m_packetManager = pm;
	
	QObject::disconnect(m_veriWin, 0, 0, 0);
	if(m_veriWin) delete m_veriWin;
	m_veriWin = new EvaLoginVeriWindow();
	QObject::connect(m_veriWin, SIGNAL(changeImage()), m_packetManager, SLOT(doRequestLoginTokenEx()));
	QObject::connect(m_veriWin, SIGNAL(sendVerifyCode(const QString &)), 
									 m_packetManager, SLOT(doRequestLoginTokenEx( const QString &) ) );

// 	QObject::disconnect(m_packetManager, SIGNAL(loginNeedVerification()), 
// 											this, 				SLOT(slotLoginVerification()));
// 	QObject::connect(m_packetManager, 	SIGNAL(loginNeedVerification()), 
// 									 win, 					SLOT(slotImageReady()));
// 	QObject::connect(m_packetManager, 	SIGNAL(loginVerifyPassed()), 
// 									 win, 					SLOT(slotVerifyPassed())); 
}

void EvaLoginManager::serverBusy( )
{
	m_status = EStart;
	notifyEvent(E_Err);	
}

void EvaLoginManager::login( QHostAddress &server)
{
	assert(m_packetManager);
	m_isLoggedIn = false;
	
//X 	kdDebug() << "[EvaLoginManager] login starting" << endl;
	//EvaSetting *s = EvaMain::global->getEvaSetting();	
	ServerDetectorPacket::setStep(0);
	ServerDetectorPacket::setFromIP(0);
	
	m_packetManager->redirectTo(server.toIPv4Address(), -1);
	m_status = EStart;
}

void EvaLoginManager::loginVerification( )
{
//	EvaLoginVeriWindow *win = new EvaLoginVeriWindow();
// 	QObject::connect(win, SIGNAL(changeImage()), m_packetManager, SLOT(doRequestLoginTokenEx()));
// 	QObject::connect(win, SIGNAL(sendVerifyCode(const QString &)), 
// 					 m_packetManager, SLOT(doRequestLoginTokenEx( const QString &) ) );
// 
// 	QObject::disconnect(m_packetManager, SIGNAL(loginNeedVerification()), 
// 						this, 				SLOT(slotLoginVerification()));
// 	QObject::connect(m_packetManager, 	SIGNAL(loginNeedVerification()), 
// 					 win, 					SLOT(slotImageReady()));
// 	QObject::connect(m_packetManager, 	SIGNAL(loginVerifyPassed()), 
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
	m_status = ELoginToken;
	if(m_veriWin) m_veriWin->slotVerifyPassed();
	
	m_packetManager->doLogin();
}

void EvaLoginManager::loginOK( )
{	
        printf( "login Ok!\n" );
	m_status = ELogin;
	notifyEvent(E_LoggedIn);
	//We don't care about the reply of this command	
	m_packetManager->doChangeStatus(EvaMain::user->getStatus());

	m_packetManager->doGetUserInfo(EvaMain::user->getQQ());

}

void EvaLoginManager::wrongPassword( QString msg )
{	
	//KMessageBox::information(0, msg, i18n("Eva Login"));
	m_status = EStart;
	notifyEvent(E_PwWrong, msg);
	//TODO
}

void EvaLoginManager::loginNeedRedirect(const unsigned int fromIp, const unsigned int ip, const short port )
{
	ServerDetectorPacket::nextStep();
	ServerDetectorPacket::setFromIP(fromIp );
	
	m_status = EStart;
	notifyEvent(E_SvrRedirect);
	m_packetManager->redirectTo( ip, port);
	
}

void EvaLoginManager::fileAgentInfoReady( )
{
	printf( "[EvaLoginManager] file agent info ready\n" );
	if(g_eva && g_eva->m_FileManager){
		g_eva->m_FileManager->setMyBasicInfo(Packet::getFileAgentKey(),
						 Packet::getFileAgentToken(), 
						Packet::getFileAgentTokenLength());
		m_status = EFileAgentKey;
		m_isLoggedIn = true;
		notifyEvent(E_KeyFileAgent);
		notifyEvent(E_LoginFinished);
	}
}

void EvaLoginManager::myInfoReady( const ContactInfo info)
{
	m_status = EUserInfo;
	EvaUser *user = EvaMain::user;
	if(user)
		user->setDetails(info);
	notifyEvent(E_MyInfo);
	m_packetManager->doRequestFileAgentKey();
}

