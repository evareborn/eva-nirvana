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

#ifndef EVANOTIFYUIBASE_H
#define EVANOTIFYUIBASE_H
#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class Q3GroupBox;
class QLabel;
class QToolButton;
class Q3WidgetStack;
class Q3TextEdit;
class QPushButton;

class EvaNotifyUIBase : public QWidget
{
    Q_OBJECT
public:
	EvaNotifyUIBase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
	~EvaNotifyUIBase();
	
	Q3GroupBox* gbFrom;
	QLabel* textLabel1;
	QLabel* tlQQ;
	QLabel* textLabel2;
	QLabel* tlNick;
	QLabel* textLabel3;
	QToolButton* tbDetails;
	Q3GroupBox* gbMain;
	Q3WidgetStack* wsMain;
	QWidget* WStackPage;
	QLabel* tlP0Tip;
	QWidget* WStackPage_2;
	QLabel* tlP1Tip;
	Q3TextEdit* teP1Msg;
	QPushButton* pbApprove;
	QPushButton* pbReject;
	QPushButton* pbAdd;
	QPushButton* pbClose;

protected:
	Q3GridLayout* EvaNotifyUIBaseLayout;
	Q3VBoxLayout* layout62;
	Q3GridLayout* gbFromLayout;
	Q3HBoxLayout* layout49;
	QSpacerItem* spacer14;
	QSpacerItem* spacer13;
	Q3GridLayout* gbMainLayout;
	Q3GridLayout* WStackPageLayout;
	Q3GridLayout* WStackPageLayout_2;
	Q3HBoxLayout* layout54;
	Q3HBoxLayout* layout52;
	QSpacerItem* spacer15;

protected slots:
	virtual void languageChange();

};

#endif 
