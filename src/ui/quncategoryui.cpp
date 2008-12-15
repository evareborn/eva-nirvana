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
#include "quncategoryui.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <q3frame.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
 
#include "evaguimain.h"
#include "defines.h"

//X #include <klocale.h>

/*
 *  Constructs a QunCategoryUI as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QunCategoryUI::QunCategoryUI( QWidget* parent, const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "QunCategoryUI" );
    QunCategoryUILayout = new Q3GridLayout( this, 1, 1, 11, 6, "QunCategoryUILayout"); 

    layout5 = new Q3VBoxLayout( 0, 0, 6, "layout5"); 

    frame3 = new Q3Frame( this, "frame3" );
    frame3->setFrameShape( Q3Frame::StyledPanel );
    frame3->setFrameShadow( Q3Frame::Raised );
    frame3Layout = new Q3GridLayout( frame3, 1, 1, 11, 6, "frame3Layout"); 

    layout3 = new Q3VBoxLayout( 0, 0, 6, "layout3"); 

    cbbTop = new QComboBox( FALSE, frame3, "cbbTop" );
    layout3->addWidget( cbbTop );

    cbbSecond = new QComboBox( FALSE, frame3, "cbbSecond" );
    layout3->addWidget( cbbSecond );

    cbbThird = new QComboBox( FALSE, frame3, "cbbThird" );
    layout3->addWidget( cbbThird );

    frame3Layout->addLayout( layout3, 0, 0 );
    layout5->addWidget( frame3 );

    layout4 = new Q3HBoxLayout( 0, 0, 6, "layout4"); 
    spacer2 = new QSpacerItem( 38, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout4->addItem( spacer2 );

    pbCancel = new QPushButton( this, "pbCancel" );
    layout4->addWidget( pbCancel );

    pbOK = new QPushButton( this, "pbOK" );
    layout4->addWidget( pbOK );
    layout5->addLayout( layout4 );

    QunCategoryUILayout->addLayout( layout5, 0, 0 );
    languageChange();
    resize( QSize(232, 179).expandedTo(minimumSizeHint()) );
//X     clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
QunCategoryUI::~QunCategoryUI()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QunCategoryUI::languageChange()
{
    setCaption( i18n( "Select Qun Category" ) );
    pbCancel->setText( i18n( "&Cancel" ) );
    pbCancel->setAccel( QKeySequence( i18n( "Alt+C" ) ) );
    pbOK->setText( i18n( "&OK" ) );
    pbOK->setAccel( QKeySequence( i18n( "Alt+O" ) ) );
}

