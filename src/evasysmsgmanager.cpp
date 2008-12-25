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

#include "evasysmsgmanager.h"

#include "evapacketmanager.h"
#include "evaguimain.h"
#include "evaapi.h"
#include "evauser.h"
#include "evausersetting.h"
#include "evaresource.h"
#include "evaaddingnoticewindow.h"
#include "evasysbroadcastwindow.h"
#include "evaqunlist.h"
#include "evachatwindowmanager.h"
#include "evaqunsysmsgwindow.h"
#include "evacontactmanager.h"
#include "defines.h"

#include <qrect.h>
#include <qtextcodec.h>
#include <qmessagebox.h>

//X #include <kmessagebox.h>
//X #include <kapp.h>
//X #include <kdebug.h>
//X #include <klocale.h>

EvaSysMsgManager::EvaSysMsgManager( EvaMain* evaapp)
{
    g_eva = evaapp;
}

EvaSysMsgManager::~ EvaSysMsgManager( )
{
}

void EvaSysMsgManager::setPacketManager( EvaPacketManager * packetManager )
{
	m_PacketManager = packetManager;
	if(!packetManager){
//X 		kdDebug() << "[EvaAddingManager] "<< " NULL pointer for packet manager" << endl;
		return;
	}
	QObject::connect(m_PacketManager, SIGNAL( receivedQQNews(const QString &, const QString &, const QString &)),
					SLOT(slotReceivedQQNews(const QString &, const QString &, const QString &)));
	QObject::connect(m_PacketManager, SIGNAL( receivedSysMessage(const unsigned char, const unsigned int,
								const unsigned int, const QString &, const bool,
								const unsigned char *, const unsigned int)),
					SLOT(slotReceivedSysMessage(const unsigned char, const unsigned int,
								const unsigned int, const QString &, const bool,
								const unsigned char *, const unsigned int)));
	QObject::connect(m_PacketManager, SIGNAL(receivedVerifyAddingMessageReply(const unsigned int,
										const unsigned char,
										const unsigned char,
										const unsigned char)),
					SLOT(slotReceivedVerifyAddingMessageReply(const unsigned int,
										const unsigned char,
										const unsigned char,
										const unsigned char)));
	QObject::connect(m_PacketManager, SIGNAL(qunSysMessage(const unsigned int,
								const unsigned short,
								const unsigned int,
								const unsigned char,
								const unsigned int,
								const unsigned int,
								const QString &,
								const unsigned char *,
								const unsigned short,
								const unsigned char *,
								const unsigned short)),
					SLOT(slotQunSysMessage(const unsigned int,
								const unsigned short,
								const unsigned int,
								const unsigned char,
								const unsigned int,
								const unsigned int,
								const QString &,
								const unsigned char *,
								const unsigned short,
								const unsigned char *,
								const unsigned short)));
}

void EvaSysMsgManager::slotReceivedSysMessage(const unsigned char type, const unsigned int myID, const unsigned int fromID,
					const QString &msg, const bool allowReverse,
					const unsigned char *code, const unsigned int codeLen)
{
	if(myID != EvaMain::session->getQQ()){
//X 		kdDebug() << "[EvaSysMsgManager] you shouldn't receive this! This sys message is for QQ(" << myID << ") only!" << endl;
		return;
	}

	EvaMain::session->getUser()->getSetting()->saveSysMessage( SYSTEM_MESSAGE_NORMAL, type,
					fromID, myID, msg,
					allowReverse?1:0);

	if(type != QQ_MSG_SYS_BROADCAST){
		m_PacketManager->doVerifyAddingMessage(fromID, code, codeLen);
	}

	if( type == QQ_MSG_SYS_ADD_FRIEND_APPROVED ||
		type == QQ_MSG_SYS_ADD_FRIEND_APPROVED_AND_ADD){
		// update friend list	
		BuddyInfoCacheItem buddy = EvaMain::session->getUser()->getSetting()->getToBeAddedBuddy(fromID);
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QQFriend frd(buddy.id, buddy.face);
		frd.setNick( std::string(codec->fromUnicode(buddy.nick).data()));
		frd.setGroupIndex( buddy.group);
		EvaMain::session->getUser()->getFriendList().addFriend(frd);
		emit buddyAdded(buddy.id, buddy.nick, buddy.face, buddy.group);
	}

	if( type == QQ_MSG_SYS_BROADCAST && 
		!EvaMain::session->getUser()->getSetting()->isShowSystemBroadcastEnabled()){
		return;
	}

	if(EvaMain::session->getUser()->getSetting()->isSoundEnabled())
		EvaGlobal::getInstance()->getSoundResource()->playSysMessage();

	emit sysMessage();
}

void EvaSysMsgManager::slotReceivedVerifyAddingMessageReply(const unsigned int id,
							const unsigned char reply,
							const unsigned char /*flag1*/,
							const unsigned char /*flag2*/)
{
	if(reply == 0x00){
		/// reply code -- 0x00: failed,  0x01: sucess
		QMessageBox::information(0, QString(i18n("Verifying system message for buddy %1 failed.")).arg(id),
			i18n("Eva Search/Add Friend"));
		return;
	}
//X 	kdDebug()  << "[EvaSysMsgManager->slotReceivedVerifyAddingMessageReply] id: " << id << ", flag1: " << flag1 << ", flag2: " << flag2 << endl;
}

void EvaSysMsgManager::showSysMessage( )
{
	EvaUserSetting::sysMessage m = EvaMain::session->getUser()->getSetting()->getLastSysMessage();
	if(m.type == 0 && m.from == 0) return;
	showSysMessage( m.messageType, m.type, m.from, m.to, m.message, m.internalQunID, m.commander,
			m.code, m.codeLen, m.token, m.tokenLen);
	
}

void EvaSysMsgManager::showSysMessage( const unsigned short msgType, const unsigned char type,
					const unsigned int from, const unsigned int to,
					const QString message, const unsigned int internalQunID, const unsigned int commander,
					const unsigned char *code, const unsigned short codeLen,
					const unsigned char *token, const unsigned short tokenLen)
{
//X 	QRect scr = KApplication::desktop()->screenGeometry();
	if(msgType == SYSTEM_MESSAGE_NORMAL){
		switch(type){
			case Q_MSG_SYS_EVA_QQ_NEWS:{
				if(EvaMain::session->getUser()->getSetting()->isShowSystemNewsEnabled()){
					EvaSysBroadcastWindow *win = new EvaSysBroadcastWindow();
					QString brief = QString::fromUtf8( (const char*)(code), codeLen);
					QString url = QString::fromUtf8( (const char*)(token), tokenLen);
					win->setNews( message, brief, url); // message is the title
					//win->move(scr.center() - win->rect().center());
					win->show();
				}
				}
				break;
			case QQ_MSG_SYS_BROADCAST:{
				if(EvaMain::session->getUser()->getSetting()->isShowSystemBroadcastEnabled()){
					EvaSysBroadcastWindow *win = new EvaSysBroadcastWindow();
					win->setMessage( message);
					//win->move(scr.center() - win->rect().center());
					win->show();
				}
				}
				break;
			default:{		
				EvaAddingNoticeWindow *win = new EvaAddingNoticeWindow(m_PacketManager);
// 				QObject::connect(win, SIGNAL(buddyAdded(const unsigned int, const QString, const unsigned short, const int)),
// 						SIGNAL(buddyAdded(const unsigned int, const QString, const unsigned short, const int)) );
				QObject::connect(win, SIGNAL(requestDetails(const unsigned int)),
						SIGNAL(requestDetails(const unsigned int)) );
				QObject::connect(win, SIGNAL(requestAddBuddy(const unsigned int, const QString, const unsigned short)),
						SIGNAL(requestAddBuddy(const unsigned int, const QString, const unsigned short)));
	
				// internalQunID is 1: allow reverse, otherwise 0: not allowed reversely
				win->setMessage( type, from, message, internalQunID);
				
//X 				win->move(scr.center() - win->rect().center());
				win->show();
				}
				break;
		}
	}else{ 
		EvaQunSysMsgWindow *win = new EvaQunSysMsgWindow(m_PacketManager);
		QObject::connect(win, SIGNAL(requestDetails(const unsigned int)),
					SIGNAL(requestDetails(const unsigned int)) );
		QObject::connect(win, SIGNAL(requestQunDetails(const unsigned int)),
					SIGNAL(requestQunDetails(const unsigned int)));
		win->setMessage( msgType, type, from, to, message, internalQunID, commander);
		win->setCode(code, codeLen);
		win->setToken(token, tokenLen);
		
//X 		win->move(scr.center() - win->rect().center());
		win->show();
	}
}

void EvaSysMsgManager::slotQunSysMessage( const unsigned int id, const unsigned short imType, const unsigned int ext,
					const unsigned char qunType, const unsigned int sender, const unsigned int commander,
					const QString & msg, const unsigned char * code, const unsigned short codeLen,
					const unsigned char *token, const unsigned short tokenLen )
{
	EvaMain::session->getUser()->getSetting()->saveSysMessage( imType, qunType, sender, ext,
					msg , id, commander, code, codeLen, token, tokenLen);
	switch(imType){
		case QQ_RECV_IM_ADDED_TO_QUN:
			if(sender == EvaMain::session->getUser()->getQQ()){
				EvaMain::session->getUser()->getQunList()->add(Qun(id));
				EvaMain::session->getUser()->saveQunList();
				emit qunListChanged();
				emit sysMessage();
			} else {
				EvaMain::g_ChatWindowManager->slotQunJoinEvent(id, imType,sender, commander);
			}
			//m_PacketManager->doRequestQunInfo( id);
			EvaMain::session->getContactManager()->fetchQunDetails(id);
			break;
		case QQ_RECV_IM_CREATE_QUN:
		case QQ_RECV_IM_APPROVE_JOIN_QUN:
			EvaMain::session->getUser()->getQunList()->add(Qun(id));
			EvaMain::session->getUser()->saveQunList();
			emit qunListChanged();
			emit sysMessage();
			//m_PacketManager->doRequestQunInfo( id);
			EvaMain::session->getContactManager()->fetchQunDetails(id);
			break;
		case QQ_RECV_IM_DELETED_FROM_QUN:
			if( sender == EvaMain::session->getUser()->getQQ()) {
				EvaMain::session->getUser()->getQunList()->remove(id);
				EvaMain::session->getUser()->saveQunList();
				emit qunListChanged();
				emit sysMessage();
			}else {
				Qun *qun = EvaMain::session->getUser()->getQunList()->getQun(id);
				if(qun){
					if(qun->getDetails().getCreator() == EvaMain::session->getUser()->getQQ() ||
						 qun->isAdmin( EvaMain::session->getUser()->getQQ())) {// if you are admin, should notify you
						emit sysMessage();
					}
					EvaMain::g_ChatWindowManager->slotQunJoinEvent(id, imType,sender, 0);
				}
				//m_PacketManager->doRequestQunInfo( id);
				EvaMain::session->getContactManager()->fetchQunDetails(id);
			}
			break;
		case QQ_RECV_IM_SET_QUN_ADMIN:
			//m_PacketManager->doRequestQunInfo( id);
			EvaMain::session->getContactManager()->fetchQunDetails(id);
			emit sysMessage();
			break;
		case QQ_RECV_IM_REQUEST_JOIN_QUN:
		case QQ_RECV_IM_REJECT_JOIN_QUN:
			emit sysMessage();
			break;
	}
	
}

void EvaSysMsgManager::slotReceivedQQNews( const QString & title, const QString & brief, const QString & url )
{
	EvaMain::session->getUser()->getSetting()->saveSysMessage(SYSTEM_MESSAGE_NORMAL, Q_MSG_SYS_EVA_QQ_NEWS,
					0, 0, title, 0, 0,
					(unsigned char *)(brief.utf8().data()), brief.utf8().size(),
					(unsigned char *)(url.utf8().data()), url.utf8().size());

	if( EvaMain::session->getUser()->getSetting()->isShowSystemNewsEnabled()){
		EvaSysBroadcastWindow *win = new EvaSysBroadcastWindow();
		win->setNews(title, brief, url);

// 		QRect scr = KApplication::desktop()->screenGeometry();
// 		win->move(scr.center() - win->rect().center());
		win->show();
	}
}


