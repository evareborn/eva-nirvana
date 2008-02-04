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

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QLabel;
class QToolButton;
class QWidgetStack;
class QTextEdit;
class QPushButton;

class EvaNotifyUIBase : public QWidget
{
    Q_OBJECT
public:
	EvaNotifyUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~EvaNotifyUIBase();
	
	QGroupBox* gbFrom;
	QLabel* textLabel1;
	QLabel* tlQQ;
	QLabel* textLabel2;
	QLabel* tlNick;
	QLabel* textLabel3;
	QToolButton* tbDetails;
	QGroupBox* gbMain;
	QWidgetStack* wsMain;
	QWidget* WStackPage;
	QLabel* tlP0Tip;
	QWidget* WStackPage_2;
	QLabel* tlP1Tip;
	QTextEdit* teP1Msg;
	QPushButton* pbApprove;
	QPushButton* pbReject;
	QPushButton* pbAdd;
	QPushButton* pbClose;

protected:
	QGridLayout* EvaNotifyUIBaseLayout;
	QVBoxLayout* layout62;
	QGridLayout* gbFromLayout;
	QHBoxLayout* layout49;
	QSpacerItem* spacer14;
	QSpacerItem* spacer13;
	QGridLayout* gbMainLayout;
	QGridLayout* WStackPageLayout;
	QGridLayout* WStackPageLayout_2;
	QHBoxLayout* layout54;
	QHBoxLayout* layout52;
	QSpacerItem* spacer15;

protected slots:
	virtual void languageChange();

};

#endif 
