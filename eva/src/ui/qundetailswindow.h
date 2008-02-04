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

#ifndef QUNDETAILSWINDOW_H
#define QUNDETAILSWINDOW_H

#include "qundetailsui.h"
#include <list>

class Qun;
class QTextCodec;
class EvaQunMemberPicker;
class QCloseEvent;
class QMoveEvent;
class QunDetailsWindow : public QunDetailsUI
{
	Q_OBJECT

public:
	//QunDetailsWindow(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	QunDetailsWindow(Qun *qun, const bool isInList = true, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~QunDetailsWindow();
	
public slots:
	void slotQunInfomationReady(const unsigned int id, const bool ok, QString msg);
	void slotModifyQunInfo(const unsigned int id, const bool ok, QString msg = QString::null);
	void slotModifyQunCardReply(const unsigned int id, const bool ok, const unsigned int qq, QString msg = QString::null);
	void slotSetAdmin(const unsigned int id, const bool ok, const unsigned int qq, const bool isSetAdmin, QString msg = QString::null);
	void slotTransferQun(const unsigned int id, const bool ok, const unsigned int qq, QString msg = QString::null);
	void slotModifyQunMembers(const unsigned int id, const bool ok, QString msg = QString::null);
	void slotReceivedQunCard(const unsigned int id, const bool ok, const unsigned int qq, QString realName, const unsigned char gender, 
				QString phone, QString email, QString memo, QString msg);
	void slotMembersUpdated(const unsigned int id);
signals:
	void requestQunInfo(const unsigned int id);
	void requestQunCard(const unsigned int, const unsigned int);
	void requestModifyQunInfo(const unsigned int id, unsigned char auth, unsigned short cate, QString name, QString notice, QString description);
	void requestModifyQunCard(const unsigned int id, const unsigned int qq, QString name, unsigned char gender, QString phone, QString email, QString memo);
	void requestQunSetAdmin(const unsigned int id, const unsigned int qq, const bool isSetAdmin);
	void requestQunTransfer(const unsigned int id, const unsigned int qq);
	void requestModifyQunMembers(const unsigned int id, const std::list<unsigned int> list, const bool isAdd);
	void requestAddBuddy(const unsigned int id, const QString nick, const unsigned short face);
	void requestUpdateQunMessageSettings(const unsigned int id, const signed char type);
	
	void removeMember(const unsigned int id);
protected:
	virtual void closeEvent( QCloseEvent *event);
	virtual void moveEvent (QMoveEvent *event);
private:
	Qun *mQun;
	QTextCodec *codec;
	EvaQunMemberPicker *picker;
	bool m_IsInList;
	
	int numModifications;
	unsigned short category;
	unsigned int m_CardId;
	
	void initTable();
	void initInformation();
	void setWidgetEnabledForInformation(bool enabled);
	
private slots:
	void slotClose();
	void slotLoadMembers();
	void slotUpdateClicked();
	void slotModifyInfo();
	void slotModifyQunCard();
	void slotTableClicked(int row,int col,int button,const QPoint& mousePos);
	
	void slotSetMembersClicked();
	void slotAddToMeClicked();
	void slotDelMembersClicked();
	void slotSetAdminClicked();
	void slotUnsetAdminClicked();
	void slotTransferClicked();
	
	void slotPickerMemberClicked(const unsigned int id, const bool isChecked);
	
	void slotModifyMembers();
	void slotCategoryClicked();
	void slotCategorySelected(const unsigned short );
	
	void slotUpdateMessageSetting();	
};

#endif

