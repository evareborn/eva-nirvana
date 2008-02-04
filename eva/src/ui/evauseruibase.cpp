/***************************************************************************
 *   Copyright (C) 2005 by casper                                          *
 *   tlmcasper@163.com                                                     *
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
 
#include "evauseruibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <klocale.h>

/*
 *  Constructs a EvaUserUIBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
EvaUserUIBase::EvaUserUIBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "EvaUserUIBase" );
    setMinimumSize( QSize( 512, 360 ) );
    setMaximumSize( QSize( 512, 360 ) );

    pbUpdate = new QPushButton( this, "pbUpdate" );
    pbUpdate->setGeometry( QRect( 330, 330, 69, 24 ) );

    pbClose = new QPushButton( this, "pbClose" );
    pbClose->setGeometry( QRect( 430, 330, 69, 24 ) );

    lblText = new QLabel( this, "lblText" );
    lblText->setGeometry( QRect( 10, 330, 200, 19 ) );

    twTabMain = new QTabWidget( this, "twTabMain" );
    twTabMain->setGeometry( QRect( 5, 5, 501, 320 ) );
    twTabMain->setMargin( 10 );

    svMain = new QScrollView( twTabMain, "svMain" );
    svMain->setStaticBackground ( TRUE );
    svMain->setHScrollBarMode(QScrollView::AlwaysOff);
    svMain->setFrameShape(QFrame::NoFrame);
    
    qwUserInfo = new EvaUserInfoWidget( svMain, "qwUserInfo");
    svMain->addChild( qwUserInfo );
    
    twTabMain->insertTab( svMain, QString("") );
   
    //QQShow Widget For TabWidget
    tabQQShow = new QWidget( twTabMain, "tabQQShow" );
    ///////////////////////////////////////////////////////////////////////
    //lblQQShowBar = new QLabel( tabQQShow, "lblQQShowBar" );
    //lblQQShowBar->setGeometry( QRect( 0, 10, 480, 16 ) );
    
    QWidget* privateLayoutWidget = new QWidget( tabQQShow, "layout13" );
    privateLayoutWidget->setGeometry( QRect( 10, -15, 350, 290 ) );
    layout13 = new QGridLayout( privateLayoutWidget, 1, 1, 11, 6, "layout13"); 

    lblQQShow = new QLabel( privateLayoutWidget, "lblQQShow" );
    lblQQShow->setMinimumSize( QSize( 147, 233 ) );
    lblQQShow->setMaximumSize( QSize( 147, 233 ) );
    lblQQShow->setFrameShape( QLabel::Box );
    lblQQShow->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    lblQQShow->setScaledContents( FALSE );
    lblQQShow->setAlignment( int( QLabel::AlignCenter ) );

    layout13->addWidget( lblQQShow, 0, 0 );
    spacer6 = new QSpacerItem( 23, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout13->addItem( spacer6, 0, 1 );

    layout12 = new QGridLayout( 0, 1, 1, 0, 6, "layout12"); 
    spacer5 = new QSpacerItem( 20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout12->addItem( spacer5, 4, 0 );
    spacer7 = new QSpacerItem( 20, 42, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout12->addItem( spacer7, 6, 0 );
    spacer8 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout12->addItem( spacer8, 0, 0 );

    tbtnAlbum = new QToolButton( privateLayoutWidget, "tbtnAlbum" );
    tbtnAlbum->setPaletteForegroundColor( QColor( 0, 0, 255 ) );
    tbtnAlbum->setCursor( QCursor( 13 ) );
    tbtnAlbum->setAutoRaise( TRUE );
    tbtnAlbum->setUsesTextLabel( TRUE );
    tbtnAlbum->setTextPosition( QToolButton::BesideIcon );

    layout12->addWidget( tbtnAlbum, 2, 0 );

    kpbUpdateShow = new KPushButton( privateLayoutWidget, "kpbUpdateShow" );

    layout12->addWidget( kpbUpdateShow, 5, 0 );

    tbtnShop = new QToolButton( privateLayoutWidget, "tbtnShop" );
    tbtnShop->setPaletteForegroundColor( QColor( 0, 0, 255 ) );
    tbtnShop->setCursor( QCursor( 13 ) );
    tbtnShop->setAutoRaise( TRUE );
    tbtnShop->setUsesTextLabel( TRUE );
    tbtnShop->setTextPosition( QToolButton::BesideIcon );


    layout12->addWidget( tbtnShop, 1, 0 );

    layout13->addLayout( layout12, 0, 2 );
    twTabMain->insertTab( tabQQShow, QString("") );
    ///////////////////////////////////////////////////////////////////////
    //Memo Widget
    TabMemo = new QWidget( twTabMain, "TabMemo" );
    TabMemoLayout = new QGridLayout( TabMemo, 1, 1, 11, 6, "TabMemoLayout"); 
    spacer30 = new QSpacerItem( 20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding );
    TabMemoLayout->addItem( spacer30, 1, 0 );
    spacer29 = new QSpacerItem( 91, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    TabMemoLayout->addItem( spacer29, 0, 1 );

    layout117 = new QVBoxLayout( 0, 0, 6, "layout117"); 

    layout100 = new QHBoxLayout( 0, 0, 6, "layout100"); 

    layout97 = new QVBoxLayout( 0, 0, 6, "layout97"); 

    lblMemoName = new QLabel( TabMemo, "lblMemoName" );
    layout97->addWidget( lblMemoName );

    lblMemoMobile = new QLabel( TabMemo, "lblMemoMobile" );
    layout97->addWidget( lblMemoMobile );

    lblMemoAddress = new QLabel( TabMemo, "lblMemoAddress" );
    layout97->addWidget( lblMemoAddress );

    lblMemoEmail = new QLabel( TabMemo, "lblMemoEmail" );
    layout97->addWidget( lblMemoEmail );
    layout100->addLayout( layout97 );

    layout99 = new QVBoxLayout( 0, 0, 6, "layout99"); 

    layout98 = new QHBoxLayout( 0, 0, 6, "layout98"); 

    layout80 = new QVBoxLayout( 0, 0, 6, "layout80"); 

    leMemoName = new QLineEdit( TabMemo, "leMemoName" );
    leMemoName->setFrameShape( QLineEdit::Box );
    leMemoName->setFrameShadow( QLineEdit::Plain );
    leMemoName->setLineWidth( 1 );
    layout80->addWidget( leMemoName );

    leMemoMobile = new QLineEdit( TabMemo, "leMemoMobile" );
    leMemoMobile->setFrameShape( QLineEdit::Box );
    leMemoMobile->setFrameShadow( QLineEdit::Plain );
    leMemoMobile->setLineWidth( 1 );
    layout80->addWidget( leMemoMobile );
    layout98->addLayout( layout80 );

    layout81 = new QVBoxLayout( 0, 0, 6, "layout81"); 

    lblMemoTelephone = new QLabel( TabMemo, "lblMemoTelephone" );
    layout81->addWidget( lblMemoTelephone );

    lblMemoZipCode = new QLabel( TabMemo, "lblMemoZipCode" );
    layout81->addWidget( lblMemoZipCode );
    layout98->addLayout( layout81 );

    layout82 = new QVBoxLayout( 0, 0, 6, "layout82"); 

    leMemoTelephone = new QLineEdit( TabMemo, "leMemoTelephone" );
    leMemoTelephone->setFrameShape( QLineEdit::Box );
    leMemoTelephone->setFrameShadow( QLineEdit::Plain );
    leMemoTelephone->setLineWidth( 1 );
    layout82->addWidget( leMemoTelephone );

    leMemoZipCode = new QLineEdit( TabMemo, "leMemoZipCode" );
    leMemoZipCode->setFrameShape( QLineEdit::Box );
    leMemoZipCode->setFrameShadow( QLineEdit::Plain );
    leMemoZipCode->setLineWidth( 1 );
    layout82->addWidget( leMemoZipCode );
    layout98->addLayout( layout82 );
    layout99->addLayout( layout98 );

    layout86 = new QVBoxLayout( 0, 0, 6, "layout86"); 

    leMemoAddress = new QLineEdit( TabMemo, "leMemoAddress" );
    leMemoAddress->setFrameShape( QLineEdit::Box );
    leMemoAddress->setFrameShadow( QLineEdit::Plain );
    leMemoAddress->setLineWidth( 1 );
    layout86->addWidget( leMemoAddress );

    leMemoEmail = new QLineEdit( TabMemo, "leMemoEmail" );
    leMemoEmail->setFrameShape( QLineEdit::Box );
    leMemoEmail->setFrameShadow( QLineEdit::Plain );
    leMemoEmail->setLineWidth( 1 );
    layout86->addWidget( leMemoEmail );
    layout99->addLayout( layout86 );
    layout100->addLayout( layout99 );
    layout117->addLayout( layout100 );

    layout112 = new QHBoxLayout( 0, 0, 6, "layout112"); 

    layout111 = new QVBoxLayout( 0, 0, 6, "layout111"); 

    lblMemoNote = new QLabel( TabMemo, "lblMemoNote" );
    layout111->addWidget( lblMemoNote );
    spacer32 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout111->addItem( spacer32 );
    layout112->addLayout( layout111 );

    teMemoNote = new QTextEdit( TabMemo, "teMemoNote" );
    teMemoNote->setMinimumSize( QSize( 0, 50 ) );
    teMemoNote->setMaximumSize( QSize( 32767, 50 ) );
    teMemoNote->setFrameShape( QTextEdit::Box );
    teMemoNote->setFrameShadow( QTextEdit::Plain );
    teMemoNote->setLineWidth( 1 );
    layout112->addWidget( teMemoNote );
    layout117->addLayout( layout112 );
    spacer38 = new QSpacerItem( 20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout117->addItem( spacer38 );

    layout116 = new QHBoxLayout( 0, 0, 6, "layout116"); 
    spacer34 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout116->addItem( spacer34 );

    chbAutoUploadMemo = new QCheckBox( TabMemo, "chbAutoUploadMemo" );
    layout116->addWidget( chbAutoUploadMemo );
    spacer33 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout116->addItem( spacer33 );

    pbDownloadMemo = new QPushButton( TabMemo, "pbDownloadMemo" );
    layout116->addWidget( pbDownloadMemo );
    layout117->addLayout( layout116 );

    TabMemoLayout->addLayout( layout117, 0, 0 );
    twTabMain->insertTab( TabMemo, QString("") );

    //////////////////////////////////////////////////////////////////////
    
    languageChange();
    resize( QSize(512, 345).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaUserUIBase::~EvaUserUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaUserUIBase::languageChange()
{
    setCaption( i18n( "User Info" ) );
    pbUpdate->setText( i18n( "Update" ) );
    pbClose->setText( i18n( "Close" ) );
    lblText->setText( "<p align=\"center\"><font color=\"#00557f\"><b>" + i18n("Enjoy Eva, Enjoy Open Source!") + "</b></font></p>" );
    
    //lblQQShowBar->setText( i18n( " QQ Show Bar" ) );
    lblQQShow->setText( i18n( "QQ Show" ) );
    //tbtnHome->setTextLabel( i18n( "QQ home" ) );
    tbtnAlbum->setTextLabel( i18n( "check QQ Show album" ) );
    kpbUpdateShow->setText( i18n( "Update QQ Show" ) );
    tbtnShop->setTextLabel( i18n( "go to QQ Show shop" ) );

    twTabMain->changeTab( svMain, i18n( "User Info" ) );
    twTabMain->changeTab( tabQQShow, i18n( "QQShow" ) );
    
    lblMemoName->setText( i18n( "Name:" ) );
    lblMemoMobile->setText( i18n( "Mobile:" ) );
    lblMemoAddress->setText( i18n( "Address:" ) );
    lblMemoEmail->setText( i18n( "Email:" ) );
    lblMemoTelephone->setText( i18n( "Telephone:" ) );
    lblMemoZipCode->setText( i18n( "Zip Code:" ) );
    lblMemoNote->setText( i18n( "Note:" ) );
    chbAutoUploadMemo->setText( i18n( "Auto Upload Memo" ) );
    pbDownloadMemo->setText( i18n( "Download Memo" ) );
    twTabMain->changeTab( TabMemo, i18n( "Memo" ) );

}

