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

#include "loginuibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <klocale.h>

LoginUIBase::LoginUIBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "frmLogin" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setSizeGripEnabled( FALSE );

    cancelBtn = new QPushButton( this, "cancelBtn" );
    cancelBtn->setGeometry( QRect( 210, 170, 70, 23 ) );

    tbNetSetup = new QToolButton( this, "tbNetSetup" );
    tbNetSetup->setGeometry( QRect( 9, 170, 51, 23 ) );
    tbNetSetup->setCursor( QCursor( 13 ) );
    tbNetSetup->setToggleButton( TRUE );
    tbNetSetup->setAutoRepeat( TRUE );
    tbNetSetup->setAutoRaise( TRUE );

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setGeometry( QRect( 21, 51, 250, 115 ) );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QGridLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 
    spacer3 = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout14->addItem( spacer3 );

    layout9 = new QVBoxLayout( 0, 0, 6, "layout9"); 

    layout8 = new QHBoxLayout( 0, 0, 6, "layout8"); 

    layout7 = new QVBoxLayout( 0, 0, 6, "layout7"); 

    textLabel1 = new QLabel( groupBox1, "textLabel1" );
    layout7->addWidget( textLabel1 );

    textLabel2 = new QLabel( groupBox1, "textLabel2" );
    layout7->addWidget( textLabel2 );
    layout8->addLayout( layout7 );

    layout6 = new QVBoxLayout( 0, 0, 6, "layout6"); 

    cbQQ = new QComboBox( FALSE, groupBox1, "cbQQ" );
    cbQQ->setEditable( TRUE );
    layout6->addWidget( cbQQ );

    lePwd = new QLineEdit( groupBox1, "lePwd" );
    lePwd->setEchoMode( QLineEdit::Password );
    layout6->addWidget( lePwd );
    layout8->addLayout( layout6 );
    layout9->addLayout( layout8 );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 
    spacer1 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer1 );

    chbRecordPwd = new QCheckBox( groupBox1, "chbRecordPwd" );
    layout1->addWidget( chbRecordPwd );

    chbLoginMode = new QCheckBox( groupBox1, "chbLoginMode" );
    layout1->addWidget( chbLoginMode );
    layout9->addLayout( layout1 );
    layout14->addLayout( layout9 );
    spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout14->addItem( spacer2 );

    groupBox1Layout->addLayout( layout14, 0, 0 );

    line1 = new QFrame( this, "line1" );
    line1->setGeometry( QRect( 31, 204, 200, 16 ) );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );

    lblType = new QLabel( this, "lblType" );
    lblType->setGeometry(QRect(20, 220, 106, 23));

    cbbLoginType = new QComboBox( FALSE, this, "cbbLoginType" );
    cbbLoginType->setGeometry(QRect(130, 220, 100, 23));

    loginBtn = new QPushButton( this, "loginBtn" );
    loginBtn->setGeometry( QRect( 127, 170, 61, 23 ) );
    loginBtn->setDefault(true);

    fraNetSetup = new QFrame( this, "fraNetSetup" );
    fraNetSetup->setGeometry( QRect( 1, 252, 290, 76 ) );
    fraNetSetup->setFrameShape( QFrame::StyledPanel );
    fraNetSetup->setFrameShadow( QFrame::Raised );
    fraNetSetupLayout = new QGridLayout( fraNetSetup, 1, 1, 11, 6, "fraNetSetupLayout"); 

    layout13 = new QVBoxLayout( 0, 0, 6, "layout13"); 

    layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

    tlProxyServer = new QLabel( fraNetSetup, "tlProxyServer" );
    layout12->addWidget( tlProxyServer );

    leIP = new QLineEdit( fraNetSetup, "leIP" );
    leIP->setMaximumSize( QSize( 130, 32767 ) );
    layout12->addWidget( leIP );

    tlProxyPort = new QLabel( fraNetSetup, "tlProxyPort" );
    tlProxyPort->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, tlProxyPort->sizePolicy().hasHeightForWidth() ) );
    layout12->addWidget( tlProxyPort );

    lePort = new QLineEdit( fraNetSetup, "lePort" );
    lePort->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, lePort->sizePolicy().hasHeightForWidth() ) );
    lePort->setMaximumSize( QSize( 35, 32767 ) );
    layout12->addWidget( lePort );
    layout13->addLayout( layout12 );

    layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

    tlUserName = new QLabel( fraNetSetup, "tlUserName" );
    tlUserName->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, tlUserName->sizePolicy().hasHeightForWidth() ) );
    layout11->addWidget( tlUserName );

    leUserName = new QLineEdit( fraNetSetup, "leUserName" );
    leUserName->setMaximumSize( QSize( 75, 32767 ) );
    layout11->addWidget( leUserName );

    tlPassword = new QLabel( fraNetSetup, "tlPassword" );
    tlPassword->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, tlPassword->sizePolicy().hasHeightForWidth() ) );
    layout11->addWidget( tlPassword );

    lePassword = new QLineEdit( fraNetSetup, "lePassword" );
    lePassword->setMaximumSize( QSize( 75, 32767 ) );
    lePassword->setEchoMode( QLineEdit::Password );
    layout11->addWidget( lePassword );
    layout13->addLayout( layout11 );

    fraNetSetupLayout->addLayout( layout13, 0, 0 );

    lblLogo = new QLabel( this, "lblLogo" );
    lblLogo->setGeometry( QRect( 0, 0, 292, 47 ) );
    lblLogo->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, lblLogo->sizePolicy().hasHeightForWidth() ) );
    lblLogo->setMaximumSize( QSize( 345, 32767 ) );
    lblLogo->setScaledContents( TRUE );
    languageChange();
    resize( QSize(292, 333).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( loginBtn, SIGNAL( clicked() ), this, SLOT( loginClickSlot() ) );
    connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( cancelClickSlot() ) );
    //connect( tbNetSetup, SIGNAL( clicked() ), this, SLOT( tbNetSetupClicked() ) );
}

LoginUIBase::~LoginUIBase()
{
}

void LoginUIBase::languageChange()
{
    setCaption( i18n("Eva  - An IM Client" ) );
    cancelBtn->setText( i18n("Cancel" ) );
    tbNetSetup->setText( i18n("Network" ) );
    groupBox1->setTitle( i18n("User Login" ) );
    textLabel1->setText( i18n("Account:" ) );
    textLabel2->setText( i18n("Password:" ) );
    chbRecordPwd->setText( i18n("remember" ) );
    chbLoginMode->setText( i18n("Invisible" ) );
    lblType->setText( i18n("Connection Type:" ) );
    cbbLoginType->clear();
    cbbLoginType->insertItem( i18n("UDP" ) );
    cbbLoginType->insertItem( i18n("TCP" ) );
    cbbLoginType->insertItem( i18n("HTTP Proxy" ) );
    loginBtn->setText( i18n("Login" ) );
    tlProxyServer->setText( i18n("Proxy IP:" ) );
    tlProxyPort->setText( i18n("Port:" ) );
    tlUserName->setText( i18n("Username:" ) );
    tlPassword->setText( i18n("Password:" ) );
}

void LoginUIBase::loginClickSlot()
{
    qWarning( "LoginUIBase::loginClickSlot(): Not implemented yet" );
}

void LoginUIBase::cancelClickSlot()
{
    qWarning( "LoginUIBase::cancelClickSlot(): Not implemented yet" );
}


