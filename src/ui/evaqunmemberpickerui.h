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

#ifndef EVAQUNMEMBERPICKERUI_H
#define EVAQUNMEMBERPICKERUI_H

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3Frame>
#include <QLabel>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class Q3Frame;
class QLabel;
class Q3ListView;

class EvaQunMemberPickerUI : public QWidget
{
    Q_OBJECT

public:
	EvaQunMemberPickerUI( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
	~EvaQunMemberPickerUI();
	
	Q3Frame* fraMain;
	Q3Frame* line1;
	Q3Frame* line2;
	QLabel* lblTitle;
	Q3ListView* lvBuddyList;

protected:
	Q3GridLayout* EvaQunMemberPickerUILayout;
	Q3GridLayout* fraMainLayout;
	Q3VBoxLayout* layout1;

protected slots:
    virtual void languageChange();

};

#endif // EVAQUNMEMBERPICKERUI_H
