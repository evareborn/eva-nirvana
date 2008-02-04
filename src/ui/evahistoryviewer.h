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
#ifndef EVAHISTORYVIEWER_H
#define EVAHISTORYVIEWER_H 

#include "evahistoryuibase.h"
#include "evausersetting.h"

#include <list>

class EvaHistoryViewer : public EvaHistoryUIBase
{
	Q_OBJECT
public:
	EvaHistoryViewer(const int id, const QString &nickname, EvaUserSetting *, const bool isQunMsg = false);
	~EvaHistoryViewer();
signals:
	void historyDoubleClicked(unsigned int sender, QString sNick, unsigned int receiver, QString rNick, bool isNormal,
					 QString message, QDateTime time, const char size, 
					const bool u, const bool i, const bool b, 
					const char blue, const char green, const char red);
private:	
	void display();
	int page;
	int qqNum;
	QString nick;
	EvaUserSetting *setting; // for load messages only
	bool isQun;
	std::list<EvaUserSetting::chatMessage> list;
	
private slots:
	void slotChbSelectClick();
	void slotPbShowClick();
	void slotPbPrevClick();
	void slotPbNextClick();
	void slotTblDisplayDoubleClick(int row);

};

#endif

