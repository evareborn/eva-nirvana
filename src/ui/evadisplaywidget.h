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
#ifndef EVADISPLAYWIDGET_H
#define EVADISPLAYWIDGET_H

#include <qwidget.h>
#include <q3frame.h>
#include <q3widgetstack.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <QLabel>
#include <QTabWidget>
#include "evamainwindow.h"

class QLabel;
class Q3ProgressBar;
class QSpacerItem;
class Q3VBoxLayout;
class Q3GridLayout;
class Q3WidgetStack;
class EvaTabWidget;

class EvaLoginPageFrame : public Q3Frame
{
	Q_OBJECT
public:
	EvaLoginPageFrame(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);
	virtual ~EvaLoginPageFrame();

	void setTotalSteps(int max);
	void update(int value, const QString &msg);
private:
	QLabel* lblDesc;
	Q3ProgressBar* qpbProgress;
	Q3VBoxLayout* layout1;
	QSpacerItem* spacer1;
	QSpacerItem* spacer2;
};

class EvaDisplayWidget : public Q3WidgetStack
{
	Q_OBJECT
public:
	EvaDisplayWidget(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);
	virtual ~EvaDisplayWidget();

	/** 
	 * show login information page or contacts page
	 *
	 * @param showInfo display information page if true, false otherwise
	 */
	void showInfoFrame(bool showInfo = false);

private:
	//QGridLayout* wspageLoginLayout;
//X 	Q3GridLayout * layoutLogin;
//X 	QWidget     * wLogin;
//X 	Q3GridLayout * layoutTab;
//X 	QWidget     * wTab;
//X 	EvaLoginPageFrame * loginPage;
	EvaTabWidget      *tab;
//X         QTabWidget      *tab;
	//QWidgetStack      * wsMainDisplay;
	
	friend class EvaMainWindow;
};

#endif //EVADISPLAYWIDGET_H

