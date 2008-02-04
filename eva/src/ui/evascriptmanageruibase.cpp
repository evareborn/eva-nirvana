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

#include "evascriptmanageruibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qheader.h>
#include <klistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <klistview.h>

/*
 *  Constructs a EvaScriptManagerUIBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
EvaScriptManagerUIBase::EvaScriptManagerUIBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "EvaScriptManagerUIBase" );
    EvaScriptManagerUIBaseLayout = new QGridLayout( this, 1, 1, 0, 6, "EvaScriptManagerUIBaseLayout"); 

    btnInstall = new QPushButton( this, "btnInstall" );

    EvaScriptManagerUIBaseLayout->addWidget( btnInstall, 0, 1 );

    btnUninstall = new QPushButton( this, "btnUninstall" );

    EvaScriptManagerUIBaseLayout->addWidget( btnUninstall, 1, 1 );

    line1 = new QFrame( this, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );

    EvaScriptManagerUIBaseLayout->addWidget( line1, 2, 1 );

    btnRun = new QPushButton( this, "btnRun" );

    EvaScriptManagerUIBaseLayout->addWidget( btnRun, 3, 1 );

    btnStop = new QPushButton( this, "btnStop" );

    EvaScriptManagerUIBaseLayout->addWidget( btnStop, 4, 1 );

    btnConfig = new QPushButton( this, "btnConfig" );

    EvaScriptManagerUIBaseLayout->addWidget( btnConfig, 5, 1 );

    btnAbout = new QPushButton( this, "btnAbout" );

    EvaScriptManagerUIBaseLayout->addWidget( btnAbout, 6, 1 );
    spacer1 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    EvaScriptManagerUIBaseLayout->addItem( spacer1, 7, 1 );

    listView = new KListView( this, "listView" );
    listView->addColumn( tr( "Scripts" ) );

    EvaScriptManagerUIBaseLayout->addMultiCellWidget( listView, 0, 7, 0, 0 );
    languageChange();
    resize( QSize(350, 308).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
    setTabOrder( listView, btnInstall );
    setTabOrder( btnInstall, btnUninstall );
    setTabOrder( btnUninstall, btnRun );
    setTabOrder( btnRun, btnStop );
    setTabOrder( btnStop, btnConfig );
    setTabOrder( btnConfig, btnAbout );
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaScriptManagerUIBase::~EvaScriptManagerUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaScriptManagerUIBase::languageChange()
{
    setCaption( tr( "Script Manager" ) );
    btnInstall->setText( tr( "&Install Script" ) );
    btnInstall->setAccel( QKeySequence( tr( "Alt+I" ) ) );
    btnUninstall->setText( tr( "&Uninstall Script" ) );
    btnUninstall->setAccel( QKeySequence( tr( "Alt+U" ) ) );
    btnRun->setText( tr( "&Run" ) );
    btnRun->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    btnStop->setText( tr( "&Stop" ) );
    btnStop->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    btnConfig->setText( tr( "&Configure" ) );
    btnConfig->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    btnAbout->setText( tr( "&About" ) );
    btnAbout->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    listView->header()->setLabel( 0, tr( "Scripts" ) );
    QToolTip::add( listView, tr( "These scripts are currently known to Eva." ) );
}

