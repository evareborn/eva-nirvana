/***************************************************************************
 *   Copyright (C) 2004 by yunfan                                          *
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

#include "evaqunsysmsguibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>

#include <klocale.h>

/*
 *  Constructs a EvaQunSysMsgUIBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
EvaQunSysMsgUIBase::EvaQunSysMsgUIBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "EvaQunSysMsgUIBase" );
    EvaQunSysMsgUIBaseLayout = new QGridLayout( this, 1, 1, 17, 6, "EvaQunSysMsgUIBaseLayout"); 

    layout12 = new QVBoxLayout( 0, 0, 6, "layout12"); 

    fraMain = new QFrame( this, "fraMain" );
    fraMain->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, fraMain->sizePolicy().hasHeightForWidth() ) );
    fraMain->setPaletteBackgroundColor( QColor( 234, 247, 255 ) );
    fraMain->setFrameShape( QFrame::StyledPanel );
    fraMain->setFrameShadow( QFrame::Raised );
    fraMainLayout = new QVBoxLayout( fraMain, 11, 6, "fraMainLayout"); 

    layout6 = new QVBoxLayout( 0, 0, 6, "layout6"); 

    layout5 = new QHBoxLayout( 0, 0, 6, "layout5"); 

    tbQQ = new QToolButton( fraMain, "tbQQ" );
    tbQQ->setPaletteBackgroundColor( QColor( 234, 247, 255 ) );
    tbQQ->setAutoRaise( TRUE );
    layout5->addWidget( tbQQ );

    lblAction = new QLabel( fraMain, "lblAction" );
    layout5->addWidget( lblAction );

    tbQun = new QToolButton( fraMain, "tbQun" );
    tbQun->setPaletteBackgroundColor( QColor( 234, 247, 255 ) );
    tbQun->setAutoRaise( TRUE );
    layout5->addWidget( tbQun );
    spacer3 = new QSpacerItem( 81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout5->addItem( spacer3 );
    layout6->addLayout( layout5 );

    lblMessage = new QLabel( fraMain, "lblMessage" );
    lblMessage->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, lblMessage->sizePolicy().hasHeightForWidth() ) );
    lblMessage->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );
    layout6->addWidget( lblMessage );
    fraMainLayout->addLayout( layout6 );
    layout12->addWidget( fraMain );

    bgActions = new QButtonGroup( this, "bgActions" );
    bgActions->setFrameShape( QButtonGroup::NoFrame );
    bgActions->setColumnLayout(0, Qt::Vertical );
    bgActions->layout()->setSpacing( 6 );
    bgActions->layout()->setMargin( 0 );
    bgActionsLayout = new QVBoxLayout( bgActions->layout() );
    bgActionsLayout->setAlignment( Qt::AlignTop );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

    rbtnAccept = new QRadioButton( bgActions, "rbtnAccept" );
    layout1->addWidget( rbtnAccept );

    rbtnReject = new QRadioButton( bgActions, "rbtnReject" );
    layout1->addWidget( rbtnReject );
    spacer1 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer1 );
    bgActionsLayout->addLayout( layout1 );
    layout12->addWidget( bgActions );

    fraMessage = new QFrame( this, "fraMessage" );
    fraMessage->setFrameShape( QFrame::NoFrame );
    fraMessage->setFrameShadow( QFrame::Raised );
    fraMessageLayout = new QVBoxLayout( fraMessage, 0, 6, "fraMessageLayout"); 

    leMessage = new QLineEdit( fraMessage, "leMessage" );
    fraMessageLayout->addWidget( leMessage );
    layout12->addWidget( fraMessage );

    layout10 = new QHBoxLayout( 0, 0, 6, "layout10"); 

    chbRejectForever = new QCheckBox( this, "chbRejectForever" );
    layout10->addWidget( chbRejectForever );
    spacer2 = new QSpacerItem( 122, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout10->addItem( spacer2 );

    btnOk = new QPushButton( this, "btnOk" );
    btnOk->setDefault( TRUE );
    layout10->addWidget( btnOk );

    btnClose = new QPushButton( this, "btnClose" );
    layout10->addWidget( btnClose );
    layout12->addLayout( layout10 );

    EvaQunSysMsgUIBaseLayout->addLayout( layout12, 0, 0 );
    languageChange();
    resize( QSize(411, 210).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaQunSysMsgUIBase::~EvaQunSysMsgUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaQunSysMsgUIBase::languageChange()
{
    setCaption( i18n( "Qun System Message" ) );
    //tbQQ->setText( tr( "..." ) );
    //lblAction->setText( tr( "request to join Qun" ) );
    //tbQun->setText( tr( "..." ) );
    //lblMessage->setText( tr( "-" ) );
    bgActions->setTitle( QString::null );
    rbtnAccept->setText( i18n( "Accept" ) );
    rbtnReject->setText( i18n( "Reject" ) );
    chbRejectForever->setText( i18n( "Reject always" ) );
    btnOk->setText( i18n( "&OK" ) );
    btnOk->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    btnClose->setText( i18n( "&Close" ) );
    btnClose->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

