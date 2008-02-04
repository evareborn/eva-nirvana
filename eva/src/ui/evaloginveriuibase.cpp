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

#include "evaloginveriuibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qlineedit.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <klocale.h>

/*
 *  Constructs a EvaLoginVeriUIBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
EvaLoginVeriUIBase::EvaLoginVeriUIBase( QWidget* parent, const char* name, WFlags fl )
    : QDialog( parent, name, false, fl )
{
    if ( !name )
	setName( "EvaLoginVeriUIBase" );
    EvaLoginVeriUIBaseLayout = new QGridLayout( this, 1, 1, 11, 6, "EvaLoginVeriUIBaseLayout"); 

    layout8 = new QVBoxLayout( 0, 0, 6, "layout8"); 

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

    lblTopTip = new QLabel( this, "lblTopTip" );
    layout1->addWidget( lblTopTip );
    spacer1 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer1 );
    layout8->addLayout( layout1 );

    layout5 = new QHBoxLayout( 0, 8, 10, "layout5"); 

    layout2 = new QVBoxLayout( 0, 0, 6, "layout2"); 

    lblGraphicCode = new QLabel( this, "lblGraphicCode" );
    layout2->addWidget( lblGraphicCode );

    lblVeriCode = new QLabel( this, "lblVeriCode" );
    layout2->addWidget( lblVeriCode );
    layout5->addLayout( layout2 );

    layout4 = new QVBoxLayout( 0, 0, 6, "layout4"); 

    layout3 = new QHBoxLayout( 0, 4, 6, "layout3"); 

    lblGraphic = new QLabel( this, "lblGraphic" );
    lblGraphic->setPaletteForegroundColor( QColor( 112, 162, 255 ) );
    lblGraphic->setFrameShape( QLabel::Box );
    layout3->addWidget( lblGraphic );

    tbnChangeGraphic = new QToolButton( this, "tbnChangeGraphic" );
    tbnChangeGraphic->setAutoRaise( TRUE );
    layout3->addWidget( tbnChangeGraphic );
    layout4->addLayout( layout3 );

    leCode = new QLineEdit( this, "leCode" );
    layout4->addWidget( leCode );
    layout5->addLayout( layout4 );
    layout8->addLayout( layout5 );

    line1 = new QFrame( this, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setMargin( 0 );
    line1->setFrameShape( QFrame::HLine );
    layout8->addWidget( line1 );

    layout6 = new QHBoxLayout( 0, 10, 6, "layout6"); 
    spacer2 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer2 );

    btnOK = new QPushButton( this, "btnOK" );
    btnOK->setDefault( true);
    layout6->addWidget( btnOK );

    btnCancel = new QPushButton( this, "btnCancel" );
    layout6->addWidget( btnCancel );
    layout8->addLayout( layout6 );

    EvaLoginVeriUIBaseLayout->addLayout( layout8, 0, 0 );
    languageChange();
    resize( QSize(320, 182).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaLoginVeriUIBase::~EvaLoginVeriUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaLoginVeriUIBase::languageChange()
{
    setCaption( i18n( "Please input the verification contents" ) );
    lblTopTip->setText( i18n( "Please input the contents shown below:" ) );
    lblGraphicCode->setText( i18n( "Verification Graphic:" ) );
    lblVeriCode->setText( i18n( "Verification Code:" ) );
    lblGraphic->setText( "pic"  );
    tbnChangeGraphic->setText( i18n( "Change image" ) );
    btnOK->setText( i18n( "&OK" ) );
    btnOK->setAccel( QKeySequence( "Alt+O"  ) );
    btnCancel->setText( i18n( "&Cancel" ) );
    btnCancel->setAccel( QKeySequence(  "Alt+C" ) );
}

