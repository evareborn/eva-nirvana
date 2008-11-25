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

#include "evasysbroadcastuibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <q3frame.h>
#include <qlabel.h>
//X #include <kurllabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
 
#include "evamain.h"

//X #include <klocale.h>
//X #include <kurllabel.h>
//X #include <kglobalsettings.h>

/*
 *  Constructs a EvaSysBroadcastUIBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
EvaSysBroadcastUIBase::EvaSysBroadcastUIBase( QWidget* parent, const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "EvaSysBroadcastUIBase" );
    EvaSysBroadcastUIBaseLayout = new Q3GridLayout( this, 1, 1, 11, 6, "EvaSysBroadcastUIBaseLayout"); 

    layout7 = new Q3VBoxLayout( 0, 0, 6, "layout7"); 

    fraContents = new Q3Frame( this, "fraContents" );
    fraContents->setPaletteBackgroundColor( QColor( 234, 247, 225 ) );
    fraContents->setFrameShape( Q3Frame::StyledPanel );
    fraContents->setFrameShadow( Q3Frame::Raised );
    fraContentsLayout = new Q3GridLayout( fraContents, 1, 1, 11, 6, "fraContentsLayout"); 

    layout6 = new Q3VBoxLayout( 0, 0, 6, "layout6"); 

    layout5 = new Q3HBoxLayout( 0, 0, 6, "layout5"); 

    lblTitle = new QLabel( fraContents, "lblTitle" );
    layout5->addWidget( lblTitle );
    spacer2 = new QSpacerItem( 111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout5->addItem( spacer2 );
    layout6->addLayout( layout5 );

    lblContents = new QLabel( fraContents, "lblContents" );
    lblContents->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, lblContents->sizePolicy().hasHeightForWidth() ) );
//X     QFont lblContents_font = KGlobalSettings::fixedFont();
//X     lblContents->setFont( lblContents_font ); 
    lblContents->setFrameShape( QLabel::NoFrame );
    lblContents->setTextFormat( Qt::PlainText );
    lblContents->setAlignment( Qt::WordBreak | Qt::AlignTop | Qt::AlignLeft );
    layout6->addWidget( lblContents );

    qlblUrl = new QLabel( fraContents, "kurllblUrl" );
    qlblUrl->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, qlblUrl->sizePolicy().hasHeightForWidth() ) );
    qlblUrl->setFrameShape( QLabel::NoFrame );
    layout6->addWidget( qlblUrl );

    fraContentsLayout->addLayout( layout6, 0, 0 );
    layout7->addWidget( fraContents );

    layout3 = new Q3HBoxLayout( 0, 0, 6, "layout3"); 
    spacer1 = new QSpacerItem( 101, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer1 );

    btnDetails = new QPushButton( this, "btnDetails" );
    layout3->addWidget( btnDetails );

    btnClose = new QPushButton( this, "btnClose" );
    layout3->addWidget( btnClose );
    layout7->addLayout( layout3 );

    EvaSysBroadcastUIBaseLayout->addLayout( layout7, 0, 0 );
    languageChange();
    resize( QSize(400, 248).expandedTo(minimumSizeHint()) );
//X     clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaSysBroadcastUIBase::~EvaSysBroadcastUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaSysBroadcastUIBase::languageChange()
{
    setCaption( i18n( "System message" ) );
    lblTitle->setText( i18n( "System Broadcast:" ) );
    //lblContents->setText( tr( "-" ) );
    //kurllblUrl->setText( tr( "-" ) );
//X     qlblUrl->setProperty( "url", QVariant::null );
    btnDetails->setText( i18n( "&More Details" ) );
    btnDetails->setAccel( QKeySequence( tr( "Alt+M" ) ) );
    btnClose->setText( i18n( "&Close" ) );
    btnClose->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

