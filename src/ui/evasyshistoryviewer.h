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
#ifndef EVASYSHISTORYVIEWER_H
#define EVASYSHISTORYVIEWER_H

#include "evahistoryuibase.h"
#include "evausersetting.h"
#include <list>
#include <inttypes.h>

class EvaSysHistoryViewer : public EvaHistoryUIBase
{
	Q_OBJECT
public:
	EvaSysHistoryViewer(EvaUserSetting *setting);
	~EvaSysHistoryViewer();
signals:
	void historyDoubleClicked(const unsigned short imType,
				const unsigned char type,
				const unsigned int fromQQ,
				const unsigned int toQQ,
				const QString message,
				const unsigned int qunID,
				const unsigned int commander,
				const unsigned char *code , const unsigned short codeLen,
				const unsigned char *token, const unsigned short tokenLen);
private:	
	void display();
	int page;
	EvaUserSetting *setting; // for load messages only
	std::list<EvaUserSetting::sysMessage> list;
	
private slots:
	void slotPbPrevClick();
	void slotPbNextClick();
	void slotTblDisplayDoubleClick(int row);	
};

#endif

