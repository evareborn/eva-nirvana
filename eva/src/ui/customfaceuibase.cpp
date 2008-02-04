 /***************************************************************************
 *   Copyright (C) 2006 by yunfan                                          *
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

#include "customfaceuibase.h"

#include <qvariant.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <klocale.h>


CustomFaceUIBase::CustomFaceUIBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "CustomFaceUIBase" );
    setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    CustomFaceUIBaseLayout = new QGridLayout( this, 1, 1, 1, 0, "CustomFaceUIBaseLayout"); 

    layout2 = new QVBoxLayout( 0, 0, 0, "layout2"); 

    twSelector = new QTabWidget( this, "twSelector" );
    twSelector->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );

    layout2->addWidget( twSelector );

    layout1 = new QHBoxLayout( 0, 5, 6, "layout1"); 

    tbAdd = new QToolButton( this, "tbAdd" );
    tbAdd->setBackgroundMode( QToolButton::PaletteBackground );
    tbAdd->setPaletteForegroundColor( QColor( 4, 74, 155 ) );
    tbAdd->setAutoRaise( TRUE );
    layout1->addWidget( tbAdd );

    chbUseShortcut = new QCheckBox( this, "chbUseShortcut" );
    chbUseShortcut->setPaletteForegroundColor( QColor( 4, 74, 155 ) );
    layout1->addWidget( chbUseShortcut );
    spacer1 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer1 );

    lblPage = new QLabel( this, "lblPage" );
    lblPage->setPaletteForegroundColor( QColor( 4, 74, 155 ) );
    layout1->addWidget( lblPage );

    tbPrev = new QToolButton( this, "tbPrev" );
    tbPrev->setPaletteForegroundColor( QColor( 4, 74, 155 ) );
    layout1->addWidget( tbPrev );

    tbNext = new QToolButton( this, "tbNext" );
    tbNext->setPaletteForegroundColor( QColor( 4, 74, 155 ) );
    layout1->addWidget( tbNext );
    layout2->addLayout( layout1 );

    CustomFaceUIBaseLayout->addLayout( layout2, 0, 0 );
    languageChange();
    resize( QSize(384, 265).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

CustomFaceUIBase::~CustomFaceUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

void CustomFaceUIBase::languageChange()
{
	setCaption( i18n( "CustomFaceUIBase" ) );
	tbAdd->setText( i18n( "Add smiley" ) );
	QToolTip::add( tbAdd, i18n( "add Custom Smiley" ) );
	chbUseShortcut->setText( i18n( "&Use Shortcut" ) );
	chbUseShortcut->setAccel( QKeySequence( i18n( "Alt+U" ) ) );
	lblPage->setText( i18n( "0/0" ) );
	tbPrev->setText( i18n( "<<" ) );
	QToolTip::add( tbPrev, i18n( "previous page" ) );
	tbNext->setText( i18n( ">>" ) );
	QToolTip::add( tbNext, i18n( "next page" ) );
}

