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

#include "evaaddqunuibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qframe.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qtooltip.h>

#include <klocale.h>

/*
 *  Constructs a EvaAddQunUIBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
EvaAddQunUIBase::EvaAddQunUIBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "EvaAddQunUIBase" );
    EvaAddQunUIBaseLayout = new QGridLayout( this, 1, 1, 10, 6, "EvaAddQunUIBaseLayout"); 

    layout21 = new QVBoxLayout( 0, 0, 6, "layout21"); 

    layout15 = new QHBoxLayout( 0, 0, 6, "layout15"); 

    tbQunDetails = new QToolButton( this, "tbQunDetails" );
    tbQunDetails->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, tbQunDetails->sizePolicy().hasHeightForWidth() ) );
    tbQunDetails->setUsesBigPixmap( TRUE );
    tbQunDetails->setAutoRaise( TRUE );
    layout15->addWidget( tbQunDetails );

    layout1 = new QVBoxLayout( 0, 0, 6, "layout1"); 

    lblQunIDTag = new QLabel( this, "lblQunIDTag" );
    layout1->addWidget( lblQunIDTag );

    lblCreatorTag = new QLabel( this, "lblCreatorTag" );
    layout1->addWidget( lblCreatorTag );
    layout15->addLayout( layout1 );

    layout2 = new QVBoxLayout( 0, 0, 6, "layout2"); 

    lblQunExtID = new QLabel( this, "lblQunExtID" );
    layout2->addWidget( lblQunExtID );

    lblCreator = new QLabel( this, "lblCreator" );
    layout2->addWidget( lblCreator );
    layout15->addLayout( layout2 );

    layout14 = new QVBoxLayout( 0, 0, 6, "layout14"); 

    layout13 = new QHBoxLayout( 0, 0, 6, "layout13"); 

    lblQunNameTag = new QLabel( this, "lblQunNameTag" );
    layout13->addWidget( lblQunNameTag );

    lblQunName = new QLabel( this, "lblQunName" );
    layout13->addWidget( lblQunName );
    spacer3 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout13->addItem( spacer3 );
    layout14->addLayout( layout13 );

    layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 

    tbCreatorDetails = new QToolButton( this, "tbCreatorDetails" );
    tbCreatorDetails->setPaletteForegroundColor( QColor( 41, 90, 197 ) );
    tbCreatorDetails->setAutoRaise( TRUE );
    layout4->addWidget( tbCreatorDetails );
    spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout4->addItem( spacer1 );
    layout14->addLayout( layout4 );
    layout15->addLayout( layout14 );
    layout21->addLayout( layout15 );

    fraAuthContents = new QFrame( this, "fraAuthContents" );
    fraAuthContents->setPaletteBackgroundColor( QColor( 234, 247, 255 ) );
    fraAuthContents->setFrameShape( QFrame::StyledPanel );
    fraAuthContents->setFrameShadow( QFrame::Raised );
    fraAuthContentsLayout = new QVBoxLayout( fraAuthContents, 11, 6, "fraAuthContentsLayout"); 

    layout20 = new QVBoxLayout( 0, 0, 6, "layout20"); 

    lblDescription = new QLabel( fraAuthContents, "lblDescription" );
    lblDescription->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
    layout20->addWidget( lblDescription );

    fraAuthGraphic = new QFrame( fraAuthContents, "fraAuthGraphic" );
    fraAuthGraphic->setFrameShape( QFrame::StyledPanel );
    fraAuthGraphic->setFrameShadow( QFrame::Raised );
    fraAuthGraphicLayout = new QVBoxLayout( fraAuthGraphic, 11, 6, "fraAuthGraphicLayout"); 

    layout19 = new QVBoxLayout( 0, 0, 6, "layout19"); 

    layout18 = new QHBoxLayout( 0, 0, 6, "layout18"); 

    lblCode = new QLabel( fraAuthGraphic, "lblCode" );
    layout18->addWidget( lblCode );

    leCode = new QLineEdit( fraAuthGraphic, "leCode" );
    layout18->addWidget( leCode );
    spacer4 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout18->addItem( spacer4 );
    layout19->addLayout( layout18 );

    layout17 = new QHBoxLayout( 0, 0, 6, "layout17"); 

    lblTip = new QLabel( fraAuthGraphic, "lblTip" );
    layout17->addWidget( lblTip );

    lblGraphic = new QLabel( fraAuthGraphic, "lblGraphic" );
    layout17->addWidget( lblGraphic );

    tbRefresh = new QToolButton( fraAuthGraphic, "tbRefresh" );
    tbRefresh->setPaletteForegroundColor( QColor( 41, 90, 197 ) );
    tbRefresh->setAutoRaise( TRUE );
    layout17->addWidget( tbRefresh );
    spacer5 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout17->addItem( spacer5 );
    layout19->addLayout( layout17 );
    fraAuthGraphicLayout->addLayout( layout19 );
    layout20->addWidget( fraAuthGraphic );
    fraAuthContentsLayout->addLayout( layout20 );
    layout21->addWidget( fraAuthContents );

    gbMessage = new QGroupBox( this, "gbMessage" );
    gbMessage->setColumnLayout(0, Qt::Vertical );
    gbMessage->layout()->setSpacing( 6 );
    gbMessage->layout()->setMargin( 11 );
    gbMessageLayout = new QVBoxLayout( gbMessage->layout() );
    gbMessageLayout->setAlignment( Qt::AlignTop );

    leMessage = new QLineEdit( gbMessage, "leMessage" );
    gbMessageLayout->addWidget( leMessage );
    layout21->addWidget( gbMessage );

    layout11 = new QHBoxLayout( 0, 10, 6, "layout11"); 
    spacer2 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout11->addItem( spacer2 );

    btnSend = new QPushButton( this, "btnSend" );
    layout11->addWidget( btnSend );

    btnCancel = new QPushButton( this, "btnCancel" );
    layout11->addWidget( btnCancel );
    layout21->addLayout( layout11 );

    EvaAddQunUIBaseLayout->addLayout( layout21, 0, 0 );
    languageChange();
    resize( QSize(357, 334).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaAddQunUIBase::~EvaAddQunUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaAddQunUIBase::languageChange()
{
    setCaption( i18n( "Eva Search/Add Friend" ) );
    ///tbQunDetails->setText( tr( "..." ) );
    lblQunIDTag->setText( i18n( "Qun ID:" ) );
    lblCreatorTag->setText( i18n( "Creator:" ) );
    ///lblQunExtID->setText( tr( "ext id" ) );
    ///lblCreator->setText( tr( "creator" ) );
    lblQunNameTag->setText( i18n( "Qun Name:" ) );
    ///lblQunName->setText( tr( "name" ) );
    tbCreatorDetails->setText( i18n( "Creator Details" ) );
    ///lblDescription->setText( tr( "description" ) );
    lblCode->setText( i18n( "Auth Code:" ) );
    lblTip->setText( i18n( "Please enter the code" ) );
    ///lblGraphic->setText( tr( "--" ) );
    tbRefresh->setText( i18n( "Refresh" ) );
    gbMessage->setTitle( i18n( "Authentication needed, please leave a message" ) );
    btnSend->setText( i18n( "&Send" ) );
    btnSend->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    btnCancel->setText( i18n( "&Cancel" ) );
    btnCancel->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

