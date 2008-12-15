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

#include "evatipuibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <q3frame.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
 
#include "evaguimain.h"
#include "defines.h"

//X #include <kwin.h>
//X #include <klocale.h>

EvaTipUIBase::EvaTipUIBase( QWidget* parent, const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "EvaTipUIBase" );
    EvaTipUIBaseLayout = new Q3GridLayout( this, 1, 1, 0, 0, "EvaTipUIBaseLayout"); 

    fraMain = new Q3Frame( this, "fraMain" );
    fraMain->setFrameShape( Q3Frame::StyledPanel );
    fraMain->setFrameShadow( Q3Frame::Sunken );
    fraMain->setLineWidth( 3 );
    fraMain->setMargin( 0 );
    fraMain->setMidLineWidth( 0 );
    fraMain->setCursor( QCursor( Qt::PointingHandCursor ) );
    fraMainLayout = new Q3GridLayout( fraMain, 1, 1, 0, 0, "fraMainLayout"); 

    layout2 = new Q3HBoxLayout( 0, 0, 0, "layout2"); 

    lblPixmap = new QLabel( fraMain, "lblPixmap" );
    lblPixmap->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, lblPixmap->sizePolicy().hasHeightForWidth() ) );
    lblPixmap->setMinimumSize( QSize( 81, 121 ) );
    lblPixmap->setMaximumSize( QSize( 81, 121 ) );
    lblPixmap->setFrameShape( QLabel::NoFrame );
    lblPixmap->setFrameShadow( QLabel::Plain );
    lblPixmap->setAlignment( int( Qt::AlignCenter ) );
    lblPixmap->setCursor( QCursor( Qt::PointingHandCursor ) );
    layout2->addWidget( lblPixmap );

    layout1 = new Q3VBoxLayout( 0, 6, 0, "layout1"); 

    lblNick = new QLabel( fraMain, "lblNick" );
    lblNick->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)1, 0, 0, lblNick->sizePolicy().hasHeightForWidth() ) );
    lblNick->setMinimumSize( QSize( 120, 30 ) );
    lblNick->setMaximumSize( QSize( 120, 30 ) );
    lblNick->setCursor( QCursor( Qt::PointingHandCursor ) );
    layout1->addWidget( lblNick );

    lblMessage = new QLabel( fraMain, "lblMessage" );
    lblMessage->setAlignment( Qt::WordBreak | Qt::AlignTop );
    lblMessage->setMinimumSize( QSize( 120, 81 ) );
    lblMessage->setMaximumSize( QSize( 120, 81 ) );
    lblMessage->setCursor( QCursor( Qt::PointingHandCursor ) );
    layout1->addWidget( lblMessage );
    layout2->addLayout( layout1 );

    fraMainLayout->addLayout( layout2, 0, 0 );

    EvaTipUIBaseLayout->addWidget( fraMain, 0, 0 );
    languageChange();
    resize( QSize(227, 135).expandedTo(minimumSizeHint()) );
//X     clearWState( WState_Polished );
}


EvaTipUIBase::~EvaTipUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

void EvaTipUIBase::languageChange()
{
    setCaption( i18n( "Tip - Eva" ) );
    lblPixmap->setText( i18n( "Pix" ) );
    lblNick->setText( i18n( "nickname" ) );
    lblMessage->setText( i18n( "message" ) );
}

