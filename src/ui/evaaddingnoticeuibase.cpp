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

#include "evaaddingnoticeuibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <klocale.h>

/*
 *  Constructs a EvaAddingNoticeUIBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
EvaAddingNoticeUIBase::EvaAddingNoticeUIBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
	if ( !name )
		setName( "EvaAddingNoticeUIBase" );
	EvaAddingNoticeUIBaseLayout = new QGridLayout( this, 1, 1, 11, 6, "EvaAddingNoticeUIBaseLayout"); 
	
	layout15 = new QVBoxLayout( 0, 0, 6, "layout15"); 
	spacer1 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
	layout15->addItem( spacer1 );
	
	fraMain = new QFrame( this, "fraMain" );
	fraMain->setPaletteBackgroundColor( QColor( 234, 247, 225 ) );
	fraMain->setFrameShape( QFrame::StyledPanel );
	fraMain->setFrameShadow( QFrame::Raised );
	fraMainLayout = new QGridLayout( fraMain, 1, 1, 11, 6, "fraMainLayout"); 
	
	layout10 = new QVBoxLayout( 0, 0, 6, "layout10"); 
	
	layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 
	
	layout4 = new QVBoxLayout( 0, 0, 6, "layout4"); 
	
	lblTitle = new QLabel( fraMain, "lblTitle" );
	layout4->addWidget( lblTitle );
	
	lblMessageTag = new QLabel( fraMain, "lblMessageTag" );
	layout4->addWidget( lblMessageTag );
	layout9->addLayout( layout4 );
	spacer3 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout9->addItem( spacer3 );
	
	tbFace = new QToolButton( fraMain, "tbFace" );
	tbFace->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, tbFace->sizePolicy().hasHeightForWidth() ) );
	tbFace->setUsesBigPixmap( TRUE );
	tbFace->setAutoRaise( TRUE );
	layout9->addWidget( tbFace );
	layout10->addLayout( layout9 );
	
	lblMessage = new QLabel( fraMain, "lblMessage" );
	layout10->addWidget( lblMessage );
	
	fraMainLayout->addLayout( layout10, 0, 0 );
	layout15->addWidget( fraMain );
	
	btngrpActions = new QButtonGroup( this, "btngrpActions" );
	btngrpActions->setFrameShape( QButtonGroup::NoFrame );
	btngrpActions->setColumnLayout(0, Qt::Vertical );
	btngrpActions->layout()->setSpacing( 6 );
	btngrpActions->layout()->setMargin( 0 );
	btngrpActionsLayout = new QGridLayout( btngrpActions->layout() );
	btngrpActionsLayout->setAlignment( Qt::AlignTop );
	
	layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 
	
	rbtnAcceptAdd = new QRadioButton( btngrpActions, "rbtnAcceptAdd" );
	layout14->addWidget( rbtnAcceptAdd );
	
	rbtnAccept = new QRadioButton( btngrpActions, "rbtnAccept" );
	layout14->addWidget( rbtnAccept );
	
	rbtnReject = new QRadioButton( btngrpActions, "rbtnReject" );
	layout14->addWidget( rbtnReject );
	
	btngrpActionsLayout->addLayout( layout14, 0, 0 );
	layout15->addWidget( btngrpActions );
	
	fraRejectContents = new QFrame( this, "fraRejectContents" );
	fraRejectContents->setFrameShape( QFrame::NoFrame );
	fraRejectContents->setFrameShadow( QFrame::Raised );
	fraRejectContentsLayout = new QGridLayout( fraRejectContents, 1, 1, 0, 6, "fraRejectContentsLayout"); 
	
	layout2 = new QVBoxLayout( 0, 0, 6, "layout2"); 
	
	leRejectMessage = new QLineEdit( fraRejectContents, "leRejectMessage" );
	layout2->addWidget( leRejectMessage );
	
	chbRejectForever = new QCheckBox( fraRejectContents, "chbRejectForever" );
	layout2->addWidget( chbRejectForever );
	
	fraRejectContentsLayout->addLayout( layout2, 0, 0 );
	layout15->addWidget( fraRejectContents );
	
	layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
	
	tbtnAuthSettings = new QToolButton( this, "tbtnAuthSettings" );
	tbtnAuthSettings->setPaletteForegroundColor( QColor( 85, 0, 255 ) );
	tbtnAuthSettings->setAutoRaise( TRUE );
	layout3->addWidget( tbtnAuthSettings );
	spacer2 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout3->addItem( spacer2 );
	
	btnOk = new QPushButton( this, "btnOk" );
	layout3->addWidget( btnOk );
	
	btnCancel = new QPushButton( this, "btnCancel" );
	layout3->addWidget( btnCancel );
	layout15->addLayout( layout3 );
	
	EvaAddingNoticeUIBaseLayout->addLayout( layout15, 0, 0 );
	languageChange();
	resize( QSize(337, 272).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaAddingNoticeUIBase::~EvaAddingNoticeUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaAddingNoticeUIBase::languageChange()
{
	setCaption( i18n( "Eva Search/Add Friend" ) );
	lblTitle->setText( i18n( "-" ) );
	//lblMessageTag->setText( i18n( "Message ():" ) );
	tbFace->setText( i18n( "..." ) );
	//lblMessage->setText( i18n( "-" ) );
	btngrpActions->setTitle( QString::null );
	rbtnAcceptAdd->setText( i18n( "Accept and Add" ) );
	rbtnAccept->setText( i18n( "Accept" ) );
	rbtnReject->setText( i18n( "Reject" ) );
	chbRejectForever->setText( i18n( "Reject always" ) );
	tbtnAuthSettings->setText( i18n( "Authentication settings" ) );
	btnOk->setText( i18n( "&OK" ) );
	btnOk->setAccel( QKeySequence( tr( "Alt+O" ) ) );
	btnCancel->setText( i18n( "&Cancel" ) );
	btnCancel->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

