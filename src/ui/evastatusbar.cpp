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
#include "evastatusbar.h"

#include <qlabel.h>
#include <qtoolbutton.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3Frame>
#include <Q3GridLayout>
#include <Q3HBoxLayout>

EvaStatusBar::EvaStatusBar( QWidget* parent, const char* name, Qt::WFlags fl )
    : Q3Frame( parent, name, fl )
{
	if ( !name )
		setName( "Eva Status Bar" );

	setFrameShape( Q3Frame::NoFrame );
	setFrameShadow( Q3Frame::Raised );
	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );

	frame4Layout = new Q3GridLayout( this, 1, 1, 1, 1, "frame4Layout");

	layout4 = new Q3HBoxLayout( 0, 1, 1, "layout4"); 
	
	lblNotification = new QLabel( this, "lblNotification" );
	lblNotification->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, lblNotification->sizePolicy().hasHeightForWidth() ) );
	layout4->addWidget( lblNotification );

	fraSystem = new Q3Frame( this, "fraSystem" );
	fraSystem->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, fraSystem->sizePolicy().hasHeightForWidth() ) );
	fraSystem->setFrameShape( Q3Frame::NoFrame );
	fraSystem->setFrameShadow( Q3Frame::Raised );
	fraSystemLayout = new Q3GridLayout( fraSystem, 1, 1, 2, 2, "fraSystemLayout"); 

	layout3 = new Q3HBoxLayout( 0, 0, 6, "layout3");

	tbSearch = new QToolButton( fraSystem, "tbSearch" );
	tbSearch->setMinimumSize( QSize( 24, 24 ) );
	tbSearch->setMaximumSize( QSize( 24, 24 ) );
	tbSearch->setAutoRaise( TRUE );
	layout3->addWidget( tbSearch );
	
	tbSysMsg = new QToolButton( fraSystem, "tbSysMsg" );
	tbSysMsg->setMinimumSize( QSize( 24, 24 ) );
	tbSysMsg->setMaximumSize( QSize( 24, 24 ) );
	tbSysMsg->setAutoRaise( TRUE );
	layout3->addWidget( tbSysMsg );
	
	tbSystem = new QToolButton( fraSystem, "tbSystem" );
	tbSystem->setMinimumSize( QSize( 24, 24 ) );
	tbSystem->setMaximumSize( QSize( 24, 24 ) );
	//tbSystem->setPopupDelay(0);
	tbSystem->setAutoRaise( TRUE );
	layout3->addWidget( tbSystem );

	fraSystemLayout->addLayout( layout3, 0, 0 );
	layout4->addWidget( fraSystem );
	spacer2 = new QSpacerItem( 21, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
	layout4->addItem( spacer2 );

	fraStatus = new Q3Frame( this, "fraStatus" );
	fraStatus->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, fraStatus->sizePolicy().hasHeightForWidth() ) );
	fraStatus->setFrameShape( Q3Frame::NoFrame );
	fraStatus->setFrameShadow( Q3Frame::Raised );
	fraStatusLayout = new Q3GridLayout( fraStatus, 1, 1, 2, 2, "fraStatusLayout"); 
	
	tbStatus = new QToolButton( fraStatus, "tbStatus" );
	tbStatus->setMinimumSize( QSize( 24, 24 ) );
	tbStatus->setMaximumSize( QSize( 24, 24 ) );
	//tbStatus->setPopupDelay(0);
	tbStatus->setAutoRaise( TRUE );
	
	fraStatusLayout->addWidget( tbStatus, 0, 0 );
	layout4->addWidget( fraStatus );

	frame4Layout->addLayout( layout4, 0, 0 );
	resize( QSize(600, 30).expandedTo(minimumSizeHint()) );
//X 	clearWState( WState_Polished );
}

EvaStatusBar::~EvaStatusBar()
{
}

