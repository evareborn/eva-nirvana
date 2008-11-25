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

#ifndef EVA_STATUSBAR_H
#define EVA_STATUSBAR_H

#include <q3frame.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <QLabel>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QLabel;
class QToolButton;

class EvaStatusBar : public Q3Frame
{
public:
	EvaStatusBar( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
	~EvaStatusBar();
	
	QLabel* lblNotification;
	Q3Frame* fraSystem;
	QToolButton* tbSearch;
	QToolButton* tbSysMsg;
	QToolButton* tbSystem;
	Q3Frame* fraStatus;
	QToolButton* tbStatus;
protected:
	Q3GridLayout* frame4Layout;
	Q3HBoxLayout* layout4;
	QSpacerItem* spacer2;
	Q3GridLayout* fraSystemLayout;
	Q3HBoxLayout* layout3;
	Q3GridLayout* fraStatusLayout;
};

#endif //EVA_STATUSBAR_H
