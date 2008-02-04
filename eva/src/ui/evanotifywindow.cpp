/***************************************************************************
 *   Copyright (C) 2005 by yunfan                                          *
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
#include "evanotifywindow.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>
#include <qgroupbox.h>
#include <qtoolbutton.h>
#include <qtextedit.h>
#include <qtimer.h>
#include <klocale.h>
  
EvaNotifyWindow::EvaNotifyWindow(QWidget* parent , const char* name, WFlags fl)
	: EvaNotifyUIBase(parent, name, fl)
{
	needAuth = false;
	isQun = false;
	mInternalQunID = 0;
	QObject::connect( tbDetails, SIGNAL( clicked() ), this, SLOT( slotTbDetailsClicked() ) );
	QObject::connect( pbApprove, SIGNAL( clicked() ), this, SLOT( slotPbApproveClicked() ) );
	QObject::connect( pbReject, SIGNAL( clicked() ), this, SLOT( slotPbRejectClicked() ) );
	QObject::connect( pbAdd, SIGNAL( clicked() ), this, SLOT( slotPbAddClicked() ) );
	QObject::connect( pbClose, SIGNAL( clicked() ), this, SLOT( close() ) );
}

EvaNotifyWindow::~EvaNotifyWindow()
{
}

void EvaNotifyWindow::slotAddBuddyReady()
{
	gbMain->setTitle(i18n("Add Buddy"));
	tlP0Tip->setText(i18n("Buddy added."));
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotAddBuddyRejected()
{
	gbMain->setTitle(i18n("Add Buddy"));
	tlP0Tip->setText(i18n("Adding buddy failed."));
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(true);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotBuddyNeedAuth()
{
	gbMain->setTitle(i18n("Authentication"));
	tlP1Tip->setText(i18n("Authentication is needed for adding her/him, you can send a message to her/him."));
	wsMain->raiseWidget(1);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(true);
	pbClose->setEnabled(true);
	needAuth=true;
}

void EvaNotifyWindow::slotAddBuddySentToServer(bool ok)
{
	if(!ok) {
		gbMain->setTitle(i18n("Add Buddy"));
		tlP0Tip->setText(i18n("Sending error, please try again."));
		pbApprove->setEnabled(false);
		pbReject->setEnabled(false);
		pbAdd->setEnabled(true);
		pbClose->setEnabled(true);	
	} else {
		gbMain->setTitle(i18n("Add Buddy"));
		tlP0Tip->setText(i18n("Authentication sent."));
		pbApprove->setEnabled(false);
		pbReject->setEnabled(false);
		pbAdd->setEnabled(true);
		pbClose->setEnabled(true);
	}	
	wsMain->raiseWidget(0);
}

void EvaNotifyWindow::slotMeBeenAdded(int from)
{
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	tlNick->setText(tlQQ->text());
    
	QString msg = QString(i18n("You are added by %1")).arg(from);
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(true);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotAddMeRequest(int from, QString message)
{
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	tlNick->setText(tlQQ->text());

	QString msg = QString(i18n("%1 is waiting for your approve.")).arg(from);
	if(!message.stripWhiteSpace().isEmpty()){
		msg+= i18n("\nAdditional message")+": " + message;
	}
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);	
	pbApprove->setEnabled(true);
	pbReject->setEnabled(true);
	pbAdd->setEnabled(true);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotAddRequestApproved(int from)
{
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	tlNick->setText(tlQQ->text());

	QString msg =  QString(i18n("You'v been approved by \"%1\"")).arg(from);
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotAddRequestRejected(int from, QString message)
{
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	tlNick->setText(tlQQ->text());

	QString msg = QString(i18n("Your Request has been rejected by %1").arg(from));
	if(!message.stripWhiteSpace().isEmpty()){
		msg+= i18n("\nAdditional message")+": " + message;
	}	
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(true);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotOtherNotification(int from, QString message)
{
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	tlNick->setText(tlQQ->text());

	QString msg = QString(i18n("Received message from %1").arg(from));
	if(!message.stripWhiteSpace().isEmpty()){
		msg+= "\n" + message;
	}	
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotPbAddClicked()
{
	int from = tlQQ->text().toInt();
	
	gbMain->setTitle(i18n("Add Buddy"));
	tlP0Tip->setText(QString(i18n("adding buddy %1, please wait...").arg(from)));
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
	
	if(!needAuth){
		emit requestAddBuddy(from);
	}else{
		emit requestAddAuthBuddy(from, teP1Msg->text());
	}

}

void EvaNotifyWindow::slotPbRejectClicked()
{
	if(wsMain->id(wsMain->visibleWidget()) == 0){
		gbMain->setTitle(i18n("Reject Request"));
		tlP1Tip->setText(i18n("You can send a rejection message"));
                wsMain->raiseWidget(1);
		pbApprove->setEnabled(false);
		pbReject->setEnabled(true);
		pbAdd->setEnabled(false);
		pbClose->setEnabled(true);
		return;
	}
	tlP1Tip->setText(i18n("Rejecting request, please wait..."));
	pbReject->hide();
	//pbReject->setEnabled(false);
	int from = tlQQ->text().toInt();
	if(!mInternalQunID)
		emit requestRejectBuddy(from, teP1Msg->text());
	else
		emit requestRejectQun(from, mInternalQunID, teP1Msg->text());
}

void EvaNotifyWindow::slotPbApproveClicked()
{	
	gbMain->setTitle(i18n("Approve Request"));
	tlP1Tip->setText(i18n("Approving request, please wait..."));
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
	
	int from = tlQQ->text().toInt();
	if(!mInternalQunID)
		emit requestApproveBuddy(from);
	else
		emit requestApproveQun(from, mInternalQunID );
	
}

void EvaNotifyWindow::slotTbDetailsClicked()
{
	int from = tlQQ->text().toInt();
	if(!isQun)
		emit requestUserInfo(from);
	else
		emit requestQunInfo(mInternalQunID);
}

void EvaNotifyWindow::slotAddBuddy( int id)
{
	tlQQ->setText(QString::number(id));
	gbMain->setTitle(i18n("Add Buddy"));
	tlP0Tip->setText(QString(i18n("Do you want to add \"%1\" into your list?")).arg(id));
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(true);
	pbClose->setEnabled(true);
	QTimer::singleShot(10, this, SLOT(slotPbAddClicked()));
}

void EvaNotifyWindow::slotQunCreated( const int from, const int extQunID, const int internalQunID )
{
	mInternalQunID = internalQunID;
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	QString tip = i18n("Qun") + "(" + i18n("created") + ")";
	tlNick->setText(tip);
	
	QString msg =  QString(i18n("Qun \"%1\" has been created.")).arg(extQunID);
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotQunAdded( const int from, const int extQunID, const int internalQunID )
{
	mInternalQunID = internalQunID;
	isQun = true;
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	QString tip = i18n("Qun") + "(" + i18n("joined") + ")";
	tlNick->setText(tip);
	
	QString msg =  QString(i18n("You'v joined Qun \"%1\".")).arg(extQunID);
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotQunRemoved( const int from, const int extQunID, const int internalQunID )
{
	mInternalQunID = internalQunID;
	isQun = true;
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	QString tip = i18n("Qun") + "(" + i18n("removed") + ")";
	tlNick->setText(tip);

	QString msg =  QString(i18n("You have been removed out from Qun \"%1\".")).arg(extQunID);
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotQunJoinRequest( const int from, const int extQunID, QString message, const int internalQunID )
{
	mInternalQunID = internalQunID;
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	QString tip = i18n("Qun") + "(" + i18n("request") + ")";
	tlNick->setText(tip);

	QString msg = QString(i18n("%1 is willing to join Qun \"%2\".")).arg(from).arg(extQunID);
	if(!message.stripWhiteSpace().isEmpty()){
		msg+= i18n("\nAdditional message")+": " + message;
	}
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(true);
	pbReject->setEnabled(true);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotQunRequestApproved( const int from, const int extQunID, QString message , const int internalQunID)
{
	mInternalQunID = internalQunID;
	isQun = true;
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	QString tip = i18n("Qun") + "(" + i18n("approved") + ")";
	tlNick->setText(tip);
	
	QString msg =  QString(i18n("You'v been approved by creator \"%1\" of Qun \"%2\".")).arg(from).arg(extQunID) + "\n" + message;
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotQunRequestRejected( const int from, const int extQunID, QString message , const int internalQunID)
{
	mInternalQunID = internalQunID;
	isQun = true;
	gbMain->setTitle(i18n("Contents"));
	tlQQ->setText(QString::number(from));
	QString tip = i18n("Qun") + "(" + i18n("rejected") + ")";
	tlNick->setText(tip);

	QString msg = QString(i18n("Your Request has been rejected by \"%1\" of Qun \"%2\".").arg(from)).arg(extQunID);
	if(!message.stripWhiteSpace().isEmpty()){
		msg+= i18n("\nAdditional message")+": " + message;
	}	
	tlP0Tip->setText(msg);
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
}

void EvaNotifyWindow::slotQunJoinAuthReply( const int internalQunID )
{
	if(internalQunID != mInternalQunID ) return;
	//gbMain->setTitle(i18n("Add Buddy"));
	tlP0Tip->setText(i18n("Authentication sent."));
	wsMain->raiseWidget(0);
	pbApprove->setEnabled(false);
	pbReject->setEnabled(false);
	pbAdd->setEnabled(false);
	pbClose->setEnabled(true);
}


