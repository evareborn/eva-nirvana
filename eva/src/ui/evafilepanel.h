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


#ifndef EVAFILEPANEL_H
#define EVAFILEPANEL_H

#include <ktabwidget.h>
#include <kurl.h>

class EvaFileStatusUIBase;

class EvaFilePanel : public KTabWidget
{
	Q_OBJECT
public:
	EvaFilePanel(QWidget *parent = 0, const char *name=0, WFlags f=0);

	void updateStatus(const unsigned int session, const KURL &url, const unsigned int size, 
			const unsigned int bytes, const unsigned int time);
	void closeTab(const unsigned int session);
signals:
	void closeSession(const unsigned int);
private:
	EvaFileStatusUIBase *  newTab(const unsigned int session, const unsigned int size,
					const KURL &url);
private slots:
	void slotCloseTab(const unsigned int, QWidget *);
};

#endif
 
