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

#include "evaadduibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qframe.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <klocale.h>

/*
 *  Constructs a EvaAddUIBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
EvaAddUIBase::EvaAddUIBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "EvaAddUIBase" );
    EvaAddUIBaseLayout = new QVBoxLayout( this, 11, 6, "EvaAddUIBaseLayout"); 

    lblTip = new QLabel( this, "lblTip" );
    lblTip->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, lblTip->sizePolicy().hasHeightForWidth() ) );
    EvaAddUIBaseLayout->addWidget( lblTip );

    frame3 = new QFrame( this, "frame3" );
    frame3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, frame3->sizePolicy().hasHeightForWidth() ) );
    frame3->setPaletteBackgroundColor( QColor( 234, 247, 255 ) );
    frame3->setFrameShape( QFrame::StyledPanel );
    frame3->setFrameShadow( QFrame::Raised );
    frame3Layout = new QVBoxLayout( frame3, 11, 6, "frame3Layout"); 

    layout17 = new QHBoxLayout( 0, 0, 6, "layout17"); 

    lblTopic = new QLabel( frame3, "lblTopic" );
    lblTopic->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)1, 0, 0, lblTopic->sizePolicy().hasHeightForWidth() ) );
    layout17->addWidget( lblTopic );

    tbtnFace = new QToolButton( frame3, "tbtnFace" );
    tbtnFace->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, tbtnFace->sizePolicy().hasHeightForWidth() ) );
    tbtnFace->setUsesBigPixmap( TRUE );
    tbtnFace->setAutoRaise( TRUE );
    layout17->addWidget( tbtnFace );
    frame3Layout->addLayout( layout17 );

    layout13 = new QHBoxLayout( 0, 0, 6, "layout13"); 

    lblGroup = new QLabel( frame3, "lblGroup" );
    layout13->addWidget( lblGroup );

    cbbGroups = new QComboBox( FALSE, frame3, "cbbGroups" );
    layout13->addWidget( cbbGroups );
    spacer1 = new QSpacerItem( 177, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout13->addItem( spacer1 );
    frame3Layout->addLayout( layout13 );

    fraVeriGraphic = new QFrame( frame3, "fraVeriGraphic" );
    fraVeriGraphic->setFrameShape( QFrame::NoFrame );
    fraVeriGraphic->setFrameShadow( QFrame::Plain );
    fraVeriGraphicLayout = new QGridLayout( fraVeriGraphic, 1, 1, 0, 0, "fraVeriGraphicLayout"); 

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 

    lblVeriCode = new QLabel( fraVeriGraphic, "lblVeriCode" );
    layout14->addWidget( lblVeriCode );

    leCode = new QLineEdit( fraVeriGraphic, "leCode" );
    layout14->addWidget( leCode );

    lblCodeGraphic = new QLabel( fraVeriGraphic, "lblCodeGraphic" );
    layout14->addWidget( lblCodeGraphic );

    btnRefreshCode = new QToolButton( fraVeriGraphic, "btnRefreshCode" );
    btnRefreshCode->setAutoRaise( TRUE );
    layout14->addWidget( btnRefreshCode );

    fraVeriGraphicLayout->addLayout( layout14, 0, 0 );
    frame3Layout->addWidget( fraVeriGraphic );

    fraQuestion = new QFrame( frame3, "fraQuestion" );
    fraQuestion->setFrameShape( QFrame::NoFrame );
    fraQuestion->setFrameShadow( QFrame::Plain );
    fraQuestionLayout = new QHBoxLayout( fraQuestion, 0, 0, "fraQuestionLayout"); 

    layout12 = new QVBoxLayout( 0, 0, 6, "layout12"); 

    lblQuestion = new QLabel( fraQuestion, "lblQuestion" );
    layout12->addWidget( lblQuestion );

    lblAnswer = new QLabel( fraQuestion, "lblAnswer" );
    lblAnswer->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, lblAnswer->sizePolicy().hasHeightForWidth() ) );
    layout12->addWidget( lblAnswer );
    fraQuestionLayout->addLayout( layout12 );

    layout13_2 = new QVBoxLayout( 0, 0, 6, "layout13_2"); 

    lblQuesContents = new QLabel( fraQuestion, "lblQuesContents" );
    lblQuesContents->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)4, 0, 0, lblQuesContents->sizePolicy().hasHeightForWidth() ) );
    lblQuesContents->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignLeft ) );
    layout13_2->addWidget( lblQuesContents );

    leAnswer = new QLineEdit( fraQuestion, "leAnswer" );
    layout13_2->addWidget( leAnswer );
    fraQuestionLayout->addLayout( layout13_2 );
    frame3Layout->addWidget( fraQuestion );

    leMessage = new QLineEdit( frame3, "leMessage" );
    leMessage->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, leMessage->sizePolicy().hasHeightForWidth() ) );
    leMessage->setMaxLength( 255 );
    leMessage->setAlignment( int( QLineEdit::AlignAuto ) );
    frame3Layout->addWidget( leMessage );
    EvaAddUIBaseLayout->addWidget( frame3 );

    layout13_3 = new QHBoxLayout( 0, 10, 6, "layout13_3"); 

    chbReverse = new QCheckBox( this, "chbReverse" );
    layout13_3->addWidget( chbReverse );
    spacer2 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout13_3->addItem( spacer2 );

    btnOk = new QPushButton( this, "btnOk" );
    layout13_3->addWidget( btnOk );

    btnCancel = new QPushButton( this, "btnCancel" );
    layout13_3->addWidget( btnCancel );
    EvaAddUIBaseLayout->addLayout( layout13_3 );
    languageChange();
    resize( QSize(430, 280).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaAddUIBase::~EvaAddUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaAddUIBase::languageChange()
{
    setCaption( i18n( "Eva Search/Add Friend" ) );
    //lblTip->setText( i18n( "-" ) );
    //lblTopic->setText( i18n( "-" ) );
    //tbtnFace->setText( i18n( "..." ) );
    lblGroup->setText( i18n( "Select Group:" ) );
    lblVeriCode->setText( i18n( "Verify Code:" ) );
    lblQuestion->setText( i18n( "Personal Question:" ) );
    lblAnswer->setText( i18n( "Your Answer:" ) );
    //lblCodeGraphic->setText( i18n( "-" ) );
    btnRefreshCode->setText( i18n( "Refresh" ) );
    //lblQuesContents->setText( i18n( "-" ) );
    chbReverse->setText( i18n( "allow reverse adding" ) );
    btnOk->setText( i18n( "&OK" ) );
    btnOk->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    btnCancel->setText( i18n( "&Cancel" ) );
    btnCancel->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

