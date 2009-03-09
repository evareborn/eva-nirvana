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
#include "evahistoryviewer.h"

#include <qpushbutton.h>
#include <qtable.h>
#include <qcheckbox.h>
#include <qdatetime.h>
#include <qdatetimeedit.h>
#include <klocale.h>

EvaHistoryViewer::EvaHistoryViewer(const int id, const QString &nickname, EvaUserSetting *userSetting, const bool isQunMsg)
	: EvaHistoryUIBase(0,0,Qt::WStyle_Customize | Qt::WStyle_Title | Qt::WStyle_SysMenu | Qt::WStyle_Minimize
				| Qt::WStyle_Maximize | Qt::WStyle_NormalBorder | Qt::WDestructiveClose), 
	qqNum(id), nick(nickname), setting(userSetting), isQun(isQunMsg)
{
	page = 0;
	setCaption(QString(i18n("IM History of %1")).arg(nick));
	QObject::connect( chbSelect, SIGNAL( clicked() ), this, SLOT( slotChbSelectClick() ) );
	QObject::connect( pbShow, SIGNAL( clicked() ), this, SLOT( slotPbShowClick() ) );
	QObject::connect( pbPrev, SIGNAL( clicked() ), this, SLOT( slotPbPrevClick() ) );
	QObject::connect( pbNext, SIGNAL( clicked() ), this, SLOT( slotPbNextClick() ) );
	QObject::connect(tblDisplay, SIGNAL(doubleClicked( int,int, int, const QPoint &)), 
					this, SLOT( slotTblDisplayDoubleClick(int)));
	
	chbSelect->setChecked(false);
	deFrom->setEnabled(false);
	deTo->setEnabled(false);
	pbShow->setEnabled(false);
	pbPrev->setEnabled(false);
	display();
}

EvaHistoryViewer::~EvaHistoryViewer()
{
}

void EvaHistoryViewer::display()
{
	if(chbSelect->isChecked()){
		list = setting->getMessages(qqNum, page, QDateTime(deFrom->date()), QDateTime(deTo->date()), isQun);
	}else{
		list = setting->getMessages(qqNum, page, isQun);
	}
	int numRows = list.size();
	tblDisplay->setNumRows(numRows);
	std::list<EvaUserSetting::chatMessage>::iterator iter;
	int i=0;
	for(iter = list.begin(); iter!=list.end(); iter++){
		tblDisplay->setText(i,0, iter->message);
		tblDisplay->setText(i,1,iter->sNick);		
		tblDisplay->setText(i,2, iter->time.toString("yyyy/MM/dd hh:mm:ss"));	
		tblDisplay->adjustRow(i);	
		//tblDisplay->adjustRow(numRows-i-1);	
		i++;
	}
	tblDisplay->setColumnWidth(0, tblDisplay->width()/2);
	tblDisplay->setColumnWidth(1, tblDisplay->width()/4);
	tblDisplay->setColumnWidth(2, tblDisplay->width()/4);		
}

void EvaHistoryViewer::closeEvent( QCloseEvent * e)
{
	emit windowClosed();
	hide();
	e->ignore();
}

void EvaHistoryViewer::slotChbSelectClick()
{
	bool enabled = (chbSelect->isChecked())?true:false;
	deFrom->setEnabled(enabled);
	deTo->setEnabled(enabled);
	pbShow->setEnabled(enabled);
	page = 0;
}


void EvaHistoryViewer::slotPbShowClick()
{
	display();
}

void EvaHistoryViewer::slotPbPrevClick()
{
	if(page==0)   return;
	pbNext->setEnabled(true);	
	page--;
	display();
	if(page==0) {
		pbPrev->setEnabled(false);
		pbNext->setEnabled(true);
	}
}

void EvaHistoryViewer::slotPbNextClick()
{
	pbPrev->setEnabled(true);
	page++;
	display();
	if(!list.size()){
		pbPrev->setEnabled(true);
		pbNext->setEnabled(false);
	}
}

void EvaHistoryViewer::slotTblDisplayDoubleClick(int row)
{
	std::list<EvaUserSetting::chatMessage>::iterator iter;
	int i= 0;	
	for(iter = list.begin(); iter!=list.end(); iter++){
		if(i==row)	break;
		i++;
	}
	if(iter == list.end()) return;
	emit historyDoubleClicked(iter->sender, iter->sNick, iter->receiver, iter->rNick, iter->type, iter->message, 
				iter->time, iter->fontSize,
				((iter->flag)&0x01), ((iter->flag)&0x02), ((iter->flag)&0x04),
				iter->blue, iter->green, iter->red);
}



