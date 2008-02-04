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
#include "evanotifyuibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qwidgetstack.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <klocale.h>

EvaNotifyUIBase::EvaNotifyUIBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
	if ( !name )
		setName( "EvaNotifyUIBase" );
	EvaNotifyUIBaseLayout = new QGridLayout( this, 1, 1, 1, 0, "EvaNotifyUIBaseLayout"); 
	
	layout62 = new QVBoxLayout( 0, 0, 6, "layout62"); 
	
	gbFrom = new QGroupBox( this, "gbFrom" );
	gbFrom->setMaximumSize( QSize( 32767, 60 ) );
	gbFrom->setFrameShape( QGroupBox::GroupBoxPanel );
	gbFrom->setFrameShadow( QGroupBox::Sunken );
	gbFrom->setColumnLayout(0, Qt::Vertical );
	gbFrom->layout()->setSpacing( 0 );
	gbFrom->layout()->setMargin( 5 );
	gbFromLayout = new QGridLayout( gbFrom->layout() );
	gbFromLayout->setAlignment( Qt::AlignTop );
	
	layout49 = new QHBoxLayout( 0, 0, 0, "layout49"); 
	
	textLabel1 = new QLabel( gbFrom, "textLabel1" );
	layout49->addWidget( textLabel1 );
	
	tlQQ = new QLabel( gbFrom, "tlQQ" );
	tlQQ->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, tlQQ->sizePolicy().hasHeightForWidth() ) );
	tlQQ->setMaximumSize( QSize( 90, 25 ) );
	tlQQ->setPaletteBackgroundColor( QColor( 200, 238, 196 ) );
	tlQQ->setFrameShape( QLabel::Panel );
	tlQQ->setFrameShadow( QLabel::Sunken );
	layout49->addWidget( tlQQ );
	spacer14 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout49->addItem( spacer14 );
	
	textLabel2 = new QLabel( gbFrom, "textLabel2" );
	layout49->addWidget( textLabel2 );
	
	tlNick = new QLabel( gbFrom, "tlNick" );
	tlNick->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, tlNick->sizePolicy().hasHeightForWidth() ) );
	tlNick->setMaximumSize( QSize( 90, 25 ) );
	tlNick->setPaletteBackgroundColor( QColor( 200, 238, 196 ) );
	tlNick->setFrameShape( QLabel::Panel );
	tlNick->setFrameShadow( QLabel::Sunken );
	layout49->addWidget( tlNick );
	spacer13 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout49->addItem( spacer13 );
	
	textLabel3 = new QLabel( gbFrom, "textLabel3" );
	textLabel3->setMaximumSize( QSize( 50, 32767 ) );
	textLabel3->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
	layout49->addWidget( textLabel3 );
	
	tbDetails = new QToolButton( gbFrom, "tbDetails" );
	tbDetails->setUsesBigPixmap( TRUE );
	tbDetails->setAutoRaise( TRUE );
	layout49->addWidget( tbDetails );
	
	gbFromLayout->addLayout( layout49, 0, 0 );
	layout62->addWidget( gbFrom );
	
	gbMain = new QGroupBox( this, "gbMain" );
	gbMain->setColumnLayout(0, Qt::Vertical );
	gbMain->layout()->setSpacing( 3 );
	gbMain->layout()->setMargin( 5 );
	gbMainLayout = new QGridLayout( gbMain->layout() );
	gbMainLayout->setAlignment( Qt::AlignTop );
	
	wsMain = new QWidgetStack( gbMain, "wsMain" );
	
	WStackPage = new QWidget( wsMain, "WStackPage" );
	WStackPageLayout = new QGridLayout( WStackPage, 1, 1, 11, 6, "WStackPageLayout"); 
	
	tlP0Tip = new QLabel( WStackPage, "tlP0Tip" );
	tlP0Tip->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
	
	WStackPageLayout->addWidget( tlP0Tip, 0, 0 );
	wsMain->addWidget( WStackPage, 0 );
	
	WStackPage_2 = new QWidget( wsMain, "WStackPage_2" );
	WStackPageLayout_2 = new QGridLayout( WStackPage_2, 1, 1, 11, 6, "WStackPageLayout_2"); 
	
	layout54 = new QHBoxLayout( 0, 0, 6, "layout54"); 
	
	tlP1Tip = new QLabel( WStackPage_2, "tlP1Tip" );
	tlP1Tip->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
	layout54->addWidget( tlP1Tip );
	
	teP1Msg = new QTextEdit( WStackPage_2, "teP1Msg" );
	teP1Msg->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, teP1Msg->sizePolicy().hasHeightForWidth() ) );
	teP1Msg->setMaximumSize( QSize( 150, 60 ) );
	teP1Msg->setTextFormat( QTextEdit::PlainText );
	layout54->addWidget( teP1Msg );
	
	WStackPageLayout_2->addLayout( layout54, 0, 0 );
	wsMain->addWidget( WStackPage_2, 1 );
	
	gbMainLayout->addWidget( wsMain, 0, 0 );
	layout62->addWidget( gbMain );
	
	layout52 = new QHBoxLayout( 0, 0, 6, "layout52"); 
	
	pbApprove = new QPushButton( this, "pbApprove" );
	pbApprove->setMaximumSize( QSize( 70, 32767 ) );
	layout52->addWidget( pbApprove );
	
	pbReject = new QPushButton( this, "pbReject" );
	pbReject->setMaximumSize( QSize( 70, 32767 ) );
	layout52->addWidget( pbReject );
	spacer15 = new QSpacerItem( 60, 20, QSizePolicy::Preferred, QSizePolicy::Minimum );
	layout52->addItem( spacer15 );
	
	pbAdd = new QPushButton( this, "pbAdd" );
	pbAdd->setMaximumSize( QSize( 70, 32767 ) );
	layout52->addWidget( pbAdd );
	
	pbClose = new QPushButton( this, "pbClose" );
	pbClose->setMaximumSize( QSize( 70, 32767 ) );
	layout52->addWidget( pbClose );
	layout62->addLayout( layout52 );
	
	EvaNotifyUIBaseLayout->addLayout( layout62, 0, 0 );
	languageChange();
	resize( QSize(372, 249).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
	
}


EvaNotifyUIBase::~EvaNotifyUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

void EvaNotifyUIBase::languageChange()
{
	setCaption( i18n( "System Message" ) );
	gbFrom->setTitle( i18n( "From" ) );
	textLabel1->setText( i18n( "QQ:" ) );
	tlQQ->setText( "12345" );
	textLabel2->setText( i18n( "Nick:" ) );
	tlNick->setText( i18n( "nickname" ) );
	textLabel3->setText( i18n( "Click for Details" ) );
	tbDetails->setText( QString::null );
	gbMain->setTitle( i18n( "Contents:" ) );
	tlP0Tip->setText( i18n( "message" ) );
	tlP1Tip->setText( i18n( "tips" ) );
	pbApprove->setText( i18n( "&Approve" ) );
	pbApprove->setAccel( QKeySequence( tr( "Alt+A" ) ) );
	pbReject->setText( i18n( "&Reject" ) );
	pbReject->setAccel( QKeySequence( tr( "Alt+R" ) ) );
	pbAdd->setText( i18n( "A&dd" ) );
	pbAdd->setAccel( QKeySequence( tr( "Alt+D" ) ) );
	pbClose->setText( i18n( "&Close" ) );
	pbClose->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}


