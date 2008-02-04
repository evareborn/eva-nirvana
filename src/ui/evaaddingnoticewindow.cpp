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

#include "evaaddingnoticewindow.h"

#include <stdlib.h>

#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qframe.h>
#include <qpixmap.h>
#include <qiconset.h>
#include <qdatetime.h>
#include <qtextcodec.h>
#include <qevent.h>

#include <kmessagebox.h>
#include <klocale.h>
#include <kdebug.h>

#include "../evamain.h"
#include "evaresource.h"
#include "evauser.h"
#include "evaresource.h"
#include "evasetting.h"
#include "evausersetting.h"
#include "evapacketmanager.h"
#include "evafriendlist.h"
#include "../evaaddingmanager.h"


EvaAddingNoticeWindow::EvaAddingNoticeWindow( EvaPacketManager * connecter )
	: m_PacketManager(connecter)
{
	init();
}

void EvaAddingNoticeWindow::init()
{
	m_ID = 0;
	m_Type = 0;
	m_Message = "";
	m_AllowReverse = true;
	m_HasVerified = false;

	QPixmap *facePic = EvaMain::images->getFaceByID( 0 );
	if(facePic){
		QIconSet faceIcon;
		faceIcon.setPixmap(*facePic,QIconSet::Large);
		tbFace->setIconSet(faceIcon);
	}

	lblMessageTag->setHidden(true);
	lblMessage->setHidden(true);
	btngrpActions->setHidden(true);
	fraRejectContents->setHidden(true);
	tbtnAuthSettings->setHidden(true);
	adjustSize();

	QObject::connect(EvaMain::g_AddingManager, SIGNAL(addAuthActionReply(const unsigned int, const unsigned char, const bool)),
						SLOT(slotAddAuthActionReply(const unsigned int, const unsigned char, const bool)));
	
	QObject::connect(m_PacketManager, SIGNAL(receivedVerifyAddingMessageReply(const unsigned int,
										const unsigned char,
										const unsigned char,
										const unsigned char)),
				EvaMain::g_AddingManager,
					SLOT(slotReceivedVerifyAddingMessageReply(const unsigned int,
										const unsigned char,
										const unsigned char,
										const unsigned char)));
	//NOTE: VerifyAddingMessageReply is useless. It just means your verify message has been received by host
	//      Verify message is sent whenever you got a adding request like a kind of reply message
	
	QObject::connect(tbFace, SIGNAL(clicked()), SLOT(slotFaceClicked()));
	QObject::connect(btnOk, SIGNAL(clicked()), SLOT(slotOkClicked()));
	QObject::connect(btnCancel, SIGNAL(clicked()), SLOT(close()));
	QObject::connect(rbtnAcceptAdd, SIGNAL(clicked()), SLOT(slotRejectClicked( )));
	QObject::connect(rbtnAccept, SIGNAL(clicked()), SLOT(slotRejectClicked( )));
	QObject::connect(rbtnReject, SIGNAL(clicked()), SLOT(slotRejectClicked( )));
	
}

void EvaAddingNoticeWindow::setMessage( const unsigned char type, const unsigned int fromID,
					const QString & msg, const bool allowReverse)
{
	m_Type = type;
	m_ID =  fromID;
	m_Message = msg;
	m_AllowReverse = allowReverse;

	processSettings();
}

// void EvaAddingNoticeWindow::startVerifying( const unsigned char * code, const unsigned int codeLen )
// {
// 	QByteArray array(codeLen);
// 	array.setRawData( (const char *)(code), codeLen);
// 	m_Code.duplicate( array);
// 
// 	m_PacketManager->doVerifyAddingMessage(m_ID, m_Code.data(), m_Code.size());
// }

void EvaAddingNoticeWindow::processSettings()
{
	printf("m_Type: %d\n", m_Type);
	QString title;
	BuddyInfoCacheItem buddy = EvaMain::user->getSetting()->removeToBeAddedBuddy(m_ID);
	printf("cached: id: %d, nick: %s, face: %d, group:%d\n", buddy.id, buddy.nick.local8Bit().data(), buddy.face, buddy.group);
	m_Nick = buddy.nick;
	m_Face = buddy.face;
	QPixmap *facePic = EvaMain::images->getFaceByID( m_Face );
	if(facePic){
		QIconSet faceIcon;
		faceIcon.setPixmap(*facePic,QIconSet::Large);
		tbFace->setIconSet(faceIcon);
	}

	QString nick = buddy.nick.isEmpty()?(QString::number(m_ID)):(QString("%1(%2)").arg(m_Nick).arg(m_ID));
	switch(m_Type){
		case QQ_MSG_SYS_ADD_FRIEND_APPROVED:{
			title = QString(i18n("%1 has approved your request")).arg(nick);
			}
			break;
		case QQ_MSG_SYS_ADD_FRIEND_REJECTED:{
			title = QString(i18n("%1 has rejected your request")).arg(nick);
			}
			break;
		case QQ_MSG_SYS_ADD_FRIEND_REQUEST_EX:{
			title = QString(i18n("%1 wants to add you onto her/his contact list")).arg(nick);
			btngrpActions->setHidden(false);
			if(!m_AllowReverse || EvaMain::user->getFriendList().hasFriend(m_ID)){
				rbtnAcceptAdd->setEnabled( false );
				rbtnAcceptAdd->setHidden( true );
			}
			/// once user select reject, the lineedit should appear
			tbtnAuthSettings->setHidden(false);
			}
			break;
		case QQ_MSG_SYS_ADD_FRIEND_APPROVED_AND_ADD:{
			title = QString(i18n("%1 has approved your request and added you onto her/his contact list")).arg(nick);
			}
			break;
		case QQ_MSG_SYS_ADDED_BY_CORRECT_ANSWER:
		case QQ_MSG_SYS_BEING_ADDED_EX:{
			title = QString(i18n("%1 has added you onto her/his contact list")).arg(nick);
			if(!EvaMain::user->getFriendList().hasFriend(m_ID))
				btnOk->setText( i18n("&Add to my list") );
			}
			break;
		case QQ_MSG_SYS_BROADCAST:{
			// impossible, cuz this should use differnt dialog
			}
			break;
		default:{
			kdDebug() << "[EvaAddingNoticeWindow] unknown type: " << m_Type << endl;
			}
			break;
	}

	lblTitle->setText(title);

	if(!m_Message.isEmpty()){
		QDateTime date = QDateTime::currentDateTime(Qt::LocalTime);
		lblMessageTag->setText( QString(i18n("Additional message(%1):")).arg(date.toString( "yyyy-MM-dd hh:mm:ss" ) ) );
		lblMessage->setText(m_Message);
		lblMessageTag->setHidden(false);
		lblMessage->setHidden(false);
	}
	adjustSize();
}

void EvaAddingNoticeWindow::slotFaceClicked( )
{
	if(m_ID)
		emit requestDetails(m_ID);
}

void EvaAddingNoticeWindow::closeEvent( QCloseEvent * /*e*/ )
{
	deleteLater();
}

void EvaAddingNoticeWindow::slotOkClicked( )
{
	switch(m_Type){
		case QQ_MSG_SYS_ADD_FRIEND_REQUEST_EX:{
			/// user has selected one of followings
			/// 1. accept
			/// 2. reject
			processAddingRequest();
			}
			break;
		case QQ_MSG_SYS_BEING_ADDED_EX:
			/// should emit adding signal
			if(!EvaMain::user->getFriendList().hasFriend(m_ID))
				emit requestAddBuddy(m_ID, m_Nick, m_Face);
		case QQ_MSG_SYS_ADD_FRIEND_APPROVED:
		case QQ_MSG_SYS_ADD_FRIEND_REJECTED:
		case QQ_MSG_SYS_ADD_FRIEND_APPROVED_AND_ADD:
		case QQ_MSG_SYS_BROADCAST:
			// impossible, cuz this should use differnt dialog
		default:
			close();
			break;
	}
}

void EvaAddingNoticeWindow::processAddingRequest( )
{
	//lblMessageTag->setText(" ");
	//lblMessage->setText( i18n("Sending to server..."));

	// we do accept and add in a lazy way, accept the request first then add the buddy onto
	// our contact list. However, in QQ 2006, it uses one sub-command of 0x00A8 packet
	// to do the 2 steps in one go	
	if( rbtnAcceptAdd->isChecked() || rbtnAccept->isChecked() ){
		m_PacketManager->doAddBuddyAuthEx(m_ID, ADDING_AUTH_TYPE_ACCEPT);
		return;
	}

	if( rbtnReject->isChecked() ){
		m_PacketManager->doAddBuddyAuthEx(m_ID, ADDING_AUTH_TYPE_REJECT, 0, 0,0, 0,0, leRejectMessage->text());
		return;
	}

	KMessageBox::information(0, i18n("Select one of the three actions to perform please."), i18n("Eva Search/Add Friend"));


}

void EvaAddingNoticeWindow::slotAddAuthActionReply( const unsigned int id, const unsigned char auth, const bool ok )
{
	if(ok){
		// the lazy way to do the Accept&Add operation
		if(auth == ADDING_AUTH_TYPE_ACCEPT && rbtnAcceptAdd->isChecked()){
			EvaMain::g_AddingManager->slotAddBuddy(id, m_Nick, m_Face);
		}
		if(auth == ADDING_AUTH_TYPE_REJECT && chbRejectForever->isChecked()){
			EvaMain::user->getSetting()->addToRejectForever(m_ID);
			EvaMain::user->getSetting()->saveSettings();
		}
		close();
	}else{
		QString action;
		switch(auth){
			case ADDING_AUTH_TYPE_ACCEPT_ADD:
				action = i18n("accept and add");
				break;
			case ADDING_AUTH_TYPE_ACCEPT:
				action = i18n("accept");
				break;
			case ADDING_AUTH_TYPE_REJECT:
				action = i18n("reject");
				break;
			default:
				action = i18n("unknown");
				break;
		}
		KMessageBox::information(0, QString(i18n("Operation %1 failed.")).arg(action), i18n("Eva Search/Add Friend"));
	}
}

void EvaAddingNoticeWindow::slotRejectClicked( )
{
	if( rbtnAccept->isChecked() || rbtnAcceptAdd->isChecked()){
		fraRejectContents->setHidden(true);
	}
	if( rbtnReject->isChecked()){
		fraRejectContents->setHidden(false);
	}
	adjustSize();
}




