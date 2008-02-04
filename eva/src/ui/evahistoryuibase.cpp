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
#include "evahistoryuibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qdatetimeedit.h>
#include <qtable.h>
#include <qlayout.h>
#include <qimage.h>
#include <qpixmap.h>
#include <klocale.h>

EvaHistoryUIBase::EvaHistoryUIBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
	if ( !name )
		setName( "EvaHistoryUIBase" );
	EvaHistoryUIBaseLayout = new QGridLayout( this, 1, 1, 0, 0, "EvaHistoryUIBaseLayout"); 
	
	layout1 = new QVBoxLayout( 0, 0, 6, "layout1"); 
	
	layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 
	
	chbSelect = new QCheckBox( this, "chbSelect" );
	layout2->addWidget( chbSelect );
	
	lblFrom = new QLabel( this, "lblFrom" );
	lblFrom->setMaximumSize( QSize( 30, 32767 ) );
	layout2->addWidget( lblFrom );
	
	deFrom = new QDateEdit( this, "deFrom" );
	deFrom->setOrder(QDateEdit::YMD);
	deFrom->setDate(QDate::currentDate());
	//deFrom->setMaximumSize( QSize( 90, 32767 ) );
	layout2->addWidget( deFrom );
	
	lblTo = new QLabel( this, "lblTo" );
	lblTo->setMaximumSize( QSize( 20, 32767 ) );
	layout2->addWidget( lblTo );
	
	deTo = new QDateEdit( this, "deTo" );
	deTo->setOrder(QDateEdit::YMD);
	deTo->setDate(QDate::currentDate());
	//deTo->setMaximumSize( QSize( 90, 32767 ) );
	layout2->addWidget( deTo );
	spacer1 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( spacer1 );
	
	pbShow = new QPushButton( this, "pbShow" );
	layout2->addWidget( pbShow );
	layout1->addLayout( layout2 );
	
	tblDisplay = new QTable( this, "tblDisplay" );
	QHeader *vheader = tblDisplay->verticalHeader();
	vheader->hide();
	tblDisplay->setLeftMargin(0);
	tblDisplay->setNumRows( 0 );
	tblDisplay->setNumCols( 3 );
	//tblDisplay->setColumnWidth(0, 10);
	//tblDisplay->setColumnWidth(1, 5);
	//tblDisplay->setColumnWidth(2, 5);
	tblDisplay->setSelectionMode( QTable::SingleRow );
	tblDisplay->setFocusStyle( QTable::FollowStyle );
	tblDisplay->setReadOnly( true );
	layout1->addWidget( tblDisplay );
	
	layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
	spacer2 = new QSpacerItem( 121, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout3->addItem( spacer2 );
	
	pbPrev = new QPushButton( this, "pbPrev" );
	layout3->addWidget( pbPrev );
	
	pbNext = new QPushButton( this, "pbNext" );
	layout3->addWidget( pbNext );
	layout1->addLayout( layout3 );
	
	EvaHistoryUIBaseLayout->addLayout( layout1, 0, 0 );
	languageChange();
	resize( QSize(472, 219).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
}

EvaHistoryUIBase::~EvaHistoryUIBase()
{
}

void EvaHistoryUIBase::languageChange()
{
	setCaption( i18n( "View IM Histroy" ) );
	tblDisplay->horizontalHeader()->setLabel( 0, i18n( "Contents" ) );
	tblDisplay->horizontalHeader()->setLabel( 1, i18n( "Nickname" ) );
	tblDisplay->horizontalHeader()->setLabel( 2, i18n( "Time" ) );
	chbSelect->setText( i18n( "specify period:" ) );
	lblFrom->setText( i18n( "from" ) );
	lblTo->setText( i18n( "to" ) );
	pbShow->setText( i18n( "&Show" ) );
	pbShow->setAccel( QKeySequence( tr( "Alt+S" ) ) );
	pbPrev->setText( i18n( "&Prev" ) );
	pbPrev->setAccel( QKeySequence( tr( "Alt+P" ) ) );
	pbNext->setText( i18n( "&Next" ) );
	pbNext->setAccel( QKeySequence( tr( "Alt+N" ) ) );
}

