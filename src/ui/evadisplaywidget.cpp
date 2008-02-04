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
#include "evadisplaywidget.h"

#include <qlabel.h>
#include <qlayout.h>
#include <qwidgetstack.h>
#include <kprogress.h>
#include <klocale.h>

#include "evaapi.h"
#include "evatabwidget.h"

EvaLoginPageFrame::EvaLoginPageFrame(QWidget* parent, 
					const char* name, 
					WFlags fl)
	: QFrame(parent, name, fl)
{
	layout1 = new QVBoxLayout( this, 20, 6, "layout1"); 
	spacer1 = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout1->addItem( spacer1 );

	lblDesc = new QLabel( this, "lblDesc" );
	lblDesc->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
	layout1->addWidget( lblDesc );

	kpgProgress = new KProgress( this, "kpgProgress" );
	kpgProgress->setTotalSteps(E_LoginProcessDone + 1);
	layout1->addWidget( kpgProgress );
	spacer2 = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout1->addItem( spacer2 );

	//resize( QSize(693, 670).expandedTo(minimumSizeHint()) );
	//clearWState( WState_Polished );
}

EvaLoginPageFrame::~EvaLoginPageFrame()
{
}

void EvaLoginPageFrame::setTotalSteps(int max)
{
	kpgProgress->setTotalSteps(max);
}

void EvaLoginPageFrame::update(int value, const QString &msg)
{
	lblDesc->setText(msg);
	kpgProgress->setProgress(value);
	adjustSize();
}

///******************************************************///

EvaDisplayWidget::EvaDisplayWidget(QWidget* parent, const char* name, WFlags fl)
	: QWidgetStack(parent, name, fl)
{
	wLogin = new QWidget(this);	
	layoutLogin = new QGridLayout( wLogin, 1, 1, 30, 6, "wspageLoginLayout");
	
	loginPage = new EvaLoginPageFrame(wLogin, "loginPageFrame");
	layoutLogin->addWidget(loginPage, 0, 0);
	addWidget(wLogin, 0);
	
	//wspageLoginLayout = new QGridLayout( loginPage, 1, 1, 30, 6, "wspageLoginLayout");
	//addWidget(wLogin, 0);
	wTab = new QWidget(this);
	layoutTab = new QGridLayout( wTab, 1, 1, 3, 3, "wspageLoginLayout");
	
	tab = new EvaTabWidget(wTab, "tabWidget");
	layoutTab->addWidget(tab, 0, 0);
	addWidget(wTab, 1);
	
	raiseWidget(0);
}

EvaDisplayWidget::~EvaDisplayWidget()
{
}

void EvaDisplayWidget::showInfoFrame(bool showInfo)
{
	if(showInfo)
		loginPage->update(0, i18n("Start logging in ..."));
	raiseWidget(showInfo?0:1);
}
