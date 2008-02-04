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
#ifndef EVANOTIFYWINDOW_H  
#define EVANOTIFYWINDOW_H

#include "evanotifyuibase.h"

class EvaNotifyWindow : public EvaNotifyUIBase
{
	Q_OBJECT
public:
	EvaNotifyWindow(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
	~EvaNotifyWindow();
public slots:
	void slotAddBuddyReady();
	void slotAddBuddyRejected();
	void slotBuddyNeedAuth();
	void slotAddBuddySentToServer(bool ok);
	
	void slotMeBeenAdded(int);
	void slotAddMeRequest(int, QString);
	void slotAddRequestApproved(int);
	void slotAddRequestRejected(int, QString);
	
	void slotOtherNotification(int, QString);
	
	void slotAddBuddy(int);
	
	
	void slotQunJoinAuthReply(const int internalQunID);
	
	void slotQunCreated(const int from, const int extQunID, const int internalQunID);
	void slotQunAdded(const int from, const int extQunID, const int internalQunID);
	void slotQunRemoved(const int  from, const int extQunID, const int internalQunID);
	void slotQunJoinRequest(const int from, const int extQunID, QString message, const int internalQunID);
	void slotQunRequestApproved(const int from, const int extQunID, QString  message, const int internalQunID);
	void slotQunRequestRejected(const int from, const int extQunID, QString  message, const int internalQunID);
signals:
	void requestUserInfo(const int);
	void requestAddAuthBuddy(const int, const QString &);
	void requestAddBuddy(const int);
	
	void requestApproveBuddy(const int);
	void requestRejectBuddy(const int, const QString &);
	
	void requestQunInfo(const int);
	void requestApproveQun(const int, const int);
	void requestRejectQun(const int, const int, const QString &);
private:
	bool needAuth;
	bool isQun;
	int mInternalQunID;
	//enum QunOperationType {, };
private slots:
	void slotPbAddClicked();
	void slotPbRejectClicked();
	void slotPbApproveClicked();
	void slotTbDetailsClicked();
};

#endif


