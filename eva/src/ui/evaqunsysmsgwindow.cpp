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
 
#include "evaqunsysmsgwindow.h"

#include <cstring>
#include <qpushbutton.h>
#include <qframe.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qcheckbox.h>

#include <klocale.h>
#include <kdebug.h>

#include "evamain.h"
#include "evauser.h"
#include "evausersetting.h"
#include "evaaddingmanager.h"
#include "evacontactmanager.h"

EvaQunSysMsgWindow::EvaQunSysMsgWindow( EvaPacketManager * packetManager )
	: m_PacketManager(packetManager)
{
	bgActions->setHidden(true);
	fraMessage->setHidden(true);
	chbRejectForever->setHidden(true);
	btnOk->setHidden(true);

	QObject::connect(tbQQ, SIGNAL(clicked()), SLOT(slotQQClicked()));
	QObject::connect(tbQun, SIGNAL(clicked()), SLOT(slotQunClicked()));

	QObject::connect(rbtnAccept, SIGNAL(clicked()), SLOT(slotRejectClicked( )));
	QObject::connect(rbtnReject, SIGNAL(clicked()), SLOT(slotRejectClicked( )));

	QObject::connect(btnOk, SIGNAL(clicked()), SLOT(slotOkClicked()));
	QObject::connect(btnClose, SIGNAL(clicked()), SLOT(close()));

	QObject::connect(EvaMain::g_AddingManager, SIGNAL(joinQunAuthReply(const unsigned int, const unsigned char)),
					SLOT(slotJoinQunAuthReply(const unsigned int, const unsigned char)));
	adjustSize();
}

void EvaQunSysMsgWindow::closeEvent( QCloseEvent * /*e*/ )
{
	deleteLater();
}

void EvaQunSysMsgWindow::slotQQClicked()
{
	emit requestDetails(m_Sender);
}

void EvaQunSysMsgWindow::slotQunClicked()
{
	emit requestQunDetails(m_QunID);
}

void EvaQunSysMsgWindow::slotRejectClicked()
{
	if( rbtnAccept->isChecked() ){
		fraMessage->setHidden(true);
		chbRejectForever->setHidden(true);
	}
	if( rbtnReject->isChecked()){
		fraMessage->setHidden(false);
		chbRejectForever->setHidden(false);
	}
	adjustSize();
}

void EvaQunSysMsgWindow::slotOkClicked()
{
	if( rbtnAccept->isChecked() ){
		m_PacketManager->doApproveQun(m_Sender, m_QunID, (const unsigned char *)(m_Token.data()), m_Token.size());
	}
	if( rbtnReject->isChecked()){
		if( chbRejectForever->isChecked()){
			EvaMain::user->getSetting()->addToQunRejectForever(m_QunID, m_Sender);
			EvaMain::user->getSetting()->saveSettings();
		}
		m_PacketManager->doRejectQun(m_Sender, m_QunID, leMessage->text(), (const unsigned char *)(m_Token.data()), m_Token.size());
	}
}

void EvaQunSysMsgWindow::setMessage(const unsigned short msgType, const unsigned char type,
					const unsigned int sender, const unsigned int extQunID,
					const QString message, const unsigned int internalQunID,
					const unsigned int commander)
{
	m_QunID = internalQunID;
	m_QunExtID = extQunID;
	m_Sender = sender;
	m_Commander = commander;
	m_MsgType = msgType;
	m_QunType = type;
	m_Message = message;

	switch(msgType){
	case QQ_RECV_IM_CREATE_QUN:
		tbQQ->setHidden(true);
		lblAction->setText(i18n("You have created Qun"));
		tbQun->setText(QString(i18n("(%1)")).arg(m_QunExtID));
		break;
	case QQ_RECV_IM_ADDED_TO_QUN:
		tbQQ->setText(QString::number(m_Commander));
		tbQun->setText(QString::number(m_QunExtID));
		lblAction->setText(i18n("has added you to Qun"));
		break;       
	case QQ_RECV_IM_DELETED_FROM_QUN:
		tbQun->setText(QString::number(m_QunExtID));
		if(m_Sender == EvaMain::user->getQQ()){
			tbQQ->setHidden(true);
			lblAction->setText(i18n("You have been removed from Qun"));
		} else{
			tbQQ->setText(QString::number(m_Sender));
			lblAction->setText(i18n("has quit Qun"));
		}
		break;
	case QQ_RECV_IM_REQUEST_JOIN_QUN:
		tbQQ->setText(QString::number(m_Sender));
		tbQun->setText(QString::number(m_QunExtID));
		lblAction->setText(i18n("requests to join Qun"));
		bgActions->setHidden(false);
		btnOk->setHidden(false);
		break;
	case QQ_RECV_IM_APPROVE_JOIN_QUN:
		tbQQ->setText(QString::number(m_Commander));
		tbQun->setText(QString::number(m_QunExtID));
		lblAction->setText(i18n("has approved your request to be a member of Qun"));
		break;
	case QQ_RECV_IM_REJECT_JOIN_QUN:
		tbQQ->setText(QString::number(m_Commander));
		tbQun->setText(QString::number(m_QunExtID));
		lblAction->setText(i18n("has rejected your request to be a member of Qun"));
		break;
	case QQ_RECV_IM_SET_QUN_ADMIN:
		if(m_Sender == EvaMain::user->getQQ()){
			tbQQ->setHidden(true);
			tbQun->setText(QString::number(m_QunExtID));
			if(m_Commander == 0x01)
				lblAction->setText(i18n("You have been set as an administrator of Qun"));
			else
				lblAction->setText(i18n("You have been set as an non-administrator(normal member) of Qun"));
		} else {
			tbQQ->setText(QString::number(m_Sender));
			tbQun->setText(QString::number(m_QunExtID));
			if(m_Commander == 0x01)
				lblAction->setText(i18n("has been set as an administrator of Qun"));
			else
				lblAction->setText(i18n("has been set as an non-administrator(normal member) of Qun"));
		}
	}
	if(!m_Message.isEmpty()){
		lblMessage->setText(QString(i18n("Additional message: %1")).arg(m_Message));
	}
	adjustSize();
}

void EvaQunSysMsgWindow::setCode( const unsigned char * code, const unsigned short len )
{
	m_Code.resize(len);
	memcpy(m_Code.data(), code, len);
}

void EvaQunSysMsgWindow::setToken( const unsigned char * token, const unsigned short len )
{
	m_Token.resize(len);
	memcpy(m_Token.data(), token, len);
}

void EvaQunSysMsgWindow::slotJoinQunAuthReply( const unsigned int id, const unsigned char reply )
{
	if(reply!=0x00){
		kdDebug() << "[EvaQunSysMsgWindow] operation failed" << endl;
	}
	GetContactManager()->fetchQunDetails( id);
	close();
}
