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
#include "qundetailsui.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qgroupbox.h>
#include <qtextedit.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qtable.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <klocale.h>

/*
 *  Constructs a QunDetailsUI as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QunDetailsUI::QunDetailsUI( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "QunDetailsUI" );
    QunDetailsUILayout = new QVBoxLayout( this, 11, 6, "QunDetailsUILayout"); 

    layout18 = new QHBoxLayout( 0, 0, 1, "layout18"); 

    lblLogo = new QLabel( this, "lblLogo" );
    lblLogo->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, lblLogo->sizePolicy().hasHeightForWidth() ) );
    lblLogo->setMinimumSize( QSize( 120, 0 ) );
    lblLogo->setMaximumSize( QSize( 120, 32767 ) );
    layout18->addWidget( lblLogo );

    tabWMain = new QTabWidget( this, "tabWMain" );
    tabWMain->setTabShape( QTabWidget::Triangular );

    tabDetails = new QWidget( tabWMain, "tabDetails" );
    tabDetailsLayout = new QVBoxLayout( tabDetails, 11, 6, "tabDetailsLayout"); 

    layout17 = new QHBoxLayout( 0, 0, 6, "layout17"); 

    layout1 = new QVBoxLayout( 0, 0, 6, "layout1"); 

    lblID = new QLabel( tabDetails, "lblID" );
    layout1->addWidget( lblID );

    lblCreator = new QLabel( tabDetails, "lblCreator" );
    layout1->addWidget( lblCreator );

    lblName = new QLabel( tabDetails, "lblName" );
    layout1->addWidget( lblName );
    layout17->addLayout( layout1 );

    layout2 = new QVBoxLayout( 0, 0, 6, "layout2"); 

    leQunID = new QLineEdit( tabDetails, "leQunID" );
    leQunID->setReadOnly( TRUE );
    layout2->addWidget( leQunID );

    leCreator = new QLineEdit( tabDetails, "leCreator" );
    leCreator->setReadOnly( TRUE );
    layout2->addWidget( leCreator );

    leQunName = new QLineEdit( tabDetails, "leQunName" );
    layout2->addWidget( leQunName );
    layout17->addLayout( layout2 );

    layout25 = new QVBoxLayout( 0, 0, 6, "layout25"); 

    layout5 = new QHBoxLayout( 0, 0, 6, "layout5"); 
    spacer1 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout5->addItem( spacer1 );

    lblQunFace = new QLabel( tabDetails, "lblQunFace" );
    lblQunFace->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, lblQunFace->sizePolicy().hasHeightForWidth() ) );
    lblQunFace->setMinimumSize( QSize( 60, 60 ) );
    lblQunFace->setMaximumSize( QSize( 60, 60 ) );
    lblQunFace->setAlignment( int( QLabel::AlignCenter ) );
    layout5->addWidget( lblQunFace );
    layout25->addLayout( layout5 );

    layout24 = new QHBoxLayout( 0, 0, 6, "layout24"); 

    lblCategory = new QLabel( tabDetails, "lblCategory" );
    lblCategory->setMaximumSize( QSize( 50, 32767 ) );
    layout24->addWidget( lblCategory );

    tbCategory = new QToolButton( tabDetails, "tbCategory" );
    layout24->addWidget( tbCategory );
    layout25->addLayout( layout24 );
    layout17->addLayout( layout25 );
    tabDetailsLayout->addLayout( layout17 );

    gbNotice = new QGroupBox( tabDetails, "gbNotice" );
    gbNotice->setColumnLayout(0, Qt::Vertical );
    gbNotice->layout()->setSpacing( 6 );
    gbNotice->layout()->setMargin( 11 );
    gbNoticeLayout = new QGridLayout( gbNotice->layout() );
    gbNoticeLayout->setAlignment( Qt::AlignTop );

    teNotice = new QTextEdit( gbNotice, "teNotice" );
    teNotice->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, teNotice->sizePolicy().hasHeightForWidth() ) );
    teNotice->setMaximumSize( QSize( 32767, 50 ) );

    gbNoticeLayout->addWidget( teNotice, 0, 0 );
    tabDetailsLayout->addWidget( gbNotice );

    gbDescription = new QGroupBox( tabDetails, "gbDescription" );
    gbDescription->setColumnLayout(0, Qt::Vertical );
    gbDescription->layout()->setSpacing( 6 );
    gbDescription->layout()->setMargin( 11 );
    gbDescriptionLayout = new QGridLayout( gbDescription->layout() );
    gbDescriptionLayout->setAlignment( Qt::AlignTop );

    teDescription = new QTextEdit( gbDescription, "teDescription" );
    teDescription->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, teDescription->sizePolicy().hasHeightForWidth() ) );
    teDescription->setMaximumSize( QSize( 32767, 60 ) );

    gbDescriptionLayout->addWidget( teDescription, 0, 0 );
    tabDetailsLayout->addWidget( gbDescription );

    bgAuthen = new QButtonGroup( tabDetails, "bgAuthen" );
    bgAuthen->setExclusive( TRUE );
    bgAuthen->setColumnLayout(0, Qt::Vertical );
    bgAuthen->layout()->setSpacing( 6 );
    bgAuthen->layout()->setMargin( 11 );
    bgAuthenLayout = new QVBoxLayout( bgAuthen->layout() );
    bgAuthenLayout->setAlignment( Qt::AlignTop );

    rbNoAuthenNeed = new QRadioButton( bgAuthen, "rbNoAuthenNeed" );
    bgAuthenLayout->addWidget( rbNoAuthenNeed );

    rbNeedAuthen = new QRadioButton( bgAuthen, "rbNeedAuthen" );
    bgAuthenLayout->addWidget( rbNeedAuthen );

    rbRejectAnyone = new QRadioButton( bgAuthen, "rbRejectAnyone" );
    bgAuthenLayout->addWidget( rbRejectAnyone );
    tabDetailsLayout->addWidget( bgAuthen );
    tabWMain->insertTab( tabDetails, QString("") );

    tabMember = new QWidget( tabWMain, "tabMember" );
    tabMemberLayout = new QGridLayout( tabMember, 1, 1, 11, 6, "tabMemberLayout"); 

    layout17_2 = new QHBoxLayout( 0, 0, 6, "layout17_2"); 

    tblMembers = new QTable( tabMember, "tblMembers" );
    tblMembers->setNumCols( tblMembers->numCols() + 1 );
    tblMembers->horizontalHeader()->setLabel( tblMembers->numCols() - 1, i18n( "QQ" ) );
    tblMembers->setNumCols( tblMembers->numCols() + 1 );
    tblMembers->horizontalHeader()->setLabel( tblMembers->numCols() - 1, i18n( "Nick" ) );
    tblMembers->setNumCols( tblMembers->numCols() + 1 );
    tblMembers->horizontalHeader()->setLabel( tblMembers->numCols() - 1, i18n( "Gender" ) );
    tblMembers->setMinimumSize( QSize( 250, 0 ) );
    tblMembers->setHScrollBarMode( QTable::AlwaysOff );
    tblMembers->setNumRows( 3 );
    tblMembers->setNumCols( 4 );
    layout17_2->addWidget( tblMembers );

    layout16 = new QVBoxLayout( 0, 0, 6, "layout16"); 

    pbSetMembers = new QPushButton( tabMember, "pbSetMembers" );
    layout16->addWidget( pbSetMembers );

    pbAddToMe = new QPushButton( tabMember, "pbAddToMe" );
    layout16->addWidget( pbAddToMe );

    pbDelMembers = new QPushButton( tabMember, "pbDelMembers" );
    layout16->addWidget( pbDelMembers );
    spacer6 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout16->addItem( spacer6 );

    pbSetAdmin = new QPushButton( tabMember, "pbSetAdmin" );
    layout16->addWidget( pbSetAdmin );

    pbUnsetAdmin = new QPushButton( tabMember, "pbUnsetAdmin" );
    layout16->addWidget( pbUnsetAdmin );

    pbTransfer = new QPushButton( tabMember, "pbTransfer" );
    layout16->addWidget( pbTransfer );
    spacer7 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout16->addItem( spacer7 );
    layout17_2->addLayout( layout16 );

    tabMemberLayout->addLayout( layout17_2, 0, 0 );
    tabWMain->insertTab( tabMember, QString("") );

    tabSetting = new QWidget( tabWMain, "tabSetting" );
    tabSettingLayout = new QGridLayout( tabSetting, 1, 1, 11, 6, "tabSettingLayout"); 

    bgSettings = new QButtonGroup( tabSetting, "bgSettings" );
    bgSettings->setExclusive( TRUE );
    bgSettings->setColumnLayout(0, Qt::Vertical );
    bgSettings->layout()->setSpacing( 6 );
    bgSettings->layout()->setMargin( 11 );
    bgSettingsLayout = new QVBoxLayout( bgSettings->layout() );
    bgSettingsLayout->setAlignment( Qt::AlignTop );

    rbRecord_Show = new QRadioButton( bgSettings, "rbRecord_Show" );
    bgSettingsLayout->addWidget( rbRecord_Show );

    rbPopupWindow = new QRadioButton( bgSettings, "rbPopupWindow" );
    bgSettingsLayout->addWidget( rbPopupWindow );

    rbDisplayNumbers = new QRadioButton( bgSettings, "rbDisplayNumbers" );
    bgSettingsLayout->addWidget( rbDisplayNumbers );

    rbRecord_only = new QRadioButton( bgSettings, "rbRecord_only" );
    bgSettingsLayout->addWidget( rbRecord_only );

    rbRejectMsgs = new QRadioButton( bgSettings, "rbRejectMsgs" );
    bgSettingsLayout->addWidget( rbRejectMsgs );

    tabSettingLayout->addWidget( bgSettings, 0, 0 );
    tabWMain->insertTab( tabSetting, QString("") );

    TabCard = new QWidget( tabWMain, "TabCard" );
    TabCardLayout = new QVBoxLayout( TabCard, 11, 6, "TabCardLayout"); 

    layout19 = new QHBoxLayout( 0, 0, 6, "layout19"); 

    layout11 = new QVBoxLayout( 0, 0, 6, "layout11"); 

    lblMyName = new QLabel( TabCard, "lblMyName" );
    layout11->addWidget( lblMyName );

    lblMyGender = new QLabel( TabCard, "lblMyGender" );
    layout11->addWidget( lblMyGender );

    lblMyPhone = new QLabel( TabCard, "lblMyPhone" );
    layout11->addWidget( lblMyPhone );

    lblMyEmail = new QLabel( TabCard, "lblMyEmail" );
    layout11->addWidget( lblMyEmail );
    layout19->addLayout( layout11 );

    layout18_2 = new QVBoxLayout( 0, 0, 6, "layout18_2"); 

    leMyName = new QLineEdit( TabCard, "leMyName" );
    layout18_2->addWidget( leMyName );

    layout17_3 = new QHBoxLayout( 0, 0, 6, "layout17_3"); 

    cbbMyGender = new QComboBox( FALSE, TabCard, "cbbMyGender" );
    layout17_3->addWidget( cbbMyGender );
    spacer4 = new QSpacerItem( 61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout17_3->addItem( spacer4 );
    layout18_2->addLayout( layout17_3 );

    leMyPhone = new QLineEdit( TabCard, "leMyPhone" );
    layout18_2->addWidget( leMyPhone );

    leMyEmail = new QLineEdit( TabCard, "leMyEmail" );
    layout18_2->addWidget( leMyEmail );
    layout19->addLayout( layout18_2 );
    TabCardLayout->addLayout( layout19 );

    layout20 = new QHBoxLayout( 0, 0, 6, "layout20"); 

    line1 = new QFrame( TabCard, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );
    layout20->addWidget( line1 );
    spacer5 = new QSpacerItem( 20, 20, QSizePolicy::Preferred, QSizePolicy::Minimum );
    layout20->addItem( spacer5 );
    TabCardLayout->addLayout( layout20 );

    layout21 = new QHBoxLayout( 0, 0, 6, "layout21"); 

    layout15 = new QVBoxLayout( 0, 0, 6, "layout15"); 

    lblMyComment = new QLabel( TabCard, "lblMyComment" );
    layout15->addWidget( lblMyComment );
    spacer3 = new QSpacerItem( 20, 151, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout15->addItem( spacer3 );
    layout21->addLayout( layout15 );

    teMyMemo = new QTextEdit( TabCard, "teMyMemo" );
    layout21->addWidget( teMyMemo );
    TabCardLayout->addLayout( layout21 );
    tabWMain->insertTab( TabCard, QString("") );
    layout18->addWidget( tabWMain );
    QunDetailsUILayout->addLayout( layout18 );

    layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 
    spacer2 = new QSpacerItem( 91, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout9->addItem( spacer2 );

    pbUpdate = new QPushButton( this, "pbUpdate" );
    layout9->addWidget( pbUpdate );

    pbClose = new QPushButton( this, "pbClose" );
    layout9->addWidget( pbClose );
    QunDetailsUILayout->addLayout( layout9 );
    languageChange();
    resize( QSize(535, 488).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
QunDetailsUI::~QunDetailsUI()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QunDetailsUI::languageChange()
{
    setCaption( i18n( "Qun" ) );
    lblLogo->setText( i18n( "Qun Details" ) );
    lblID->setText( i18n( "Qun ID" ) );
    lblCreator->setText( i18n( "Creator" ) );
    lblName->setText( i18n( "Name" ) );
    lblQunFace->setText( i18n( "Qun Face" ) );
    lblCategory->setText( i18n( "Category" ) );
    tbCategory->setText( QString::null );
    gbNotice->setTitle( i18n( "Qun Notice ( member only)" ) );
    gbDescription->setTitle( i18n( "Qun Description" ) );
    bgAuthen->setTitle( i18n( "Authentication" ) );
    rbNoAuthenNeed->setText( i18n( "No authentication needed." ) );
    rbNeedAuthen->setText( i18n( "Need authentication" ) );
    rbRejectAnyone->setText( i18n( "Reject any request" ) );
    tabWMain->changeTab( tabDetails, i18n( "Details" ) );
    tblMembers->horizontalHeader()->setLabel( 0, i18n( "QQ" ) );
    tblMembers->horizontalHeader()->setLabel( 1, i18n( "Nick" ) );
    tblMembers->horizontalHeader()->setLabel( 2, i18n( "Gender" ) );
    pbSetMembers->setText( i18n( "&Members >>" ) );
    pbSetMembers->setAccel( QKeySequence( i18n( "Alt+M" ) ) );
    pbAddToMe->setText( i18n( "&Add to Me" ) );
    pbAddToMe->setAccel( QKeySequence( i18n( "Alt+A" ) ) );
    pbDelMembers->setText( i18n( "&Del Members" ) );
    pbDelMembers->setAccel( QKeySequence( i18n( "Alt+D" ) ) );
    pbSetAdmin->setText( i18n( "&Set Admin" ) );
    pbSetAdmin->setAccel( QKeySequence( i18n( "Alt+S" ) ) );
    pbUnsetAdmin->setText( i18n( "&Unset Admin" ) );
    pbUnsetAdmin->setAccel( QKeySequence( i18n( "Alt+U" ) ) );
    pbTransfer->setText( i18n( "&Transfer" ) );
    pbTransfer->setAccel( QKeySequence( i18n( "Alt+T" ) ) );
    tabWMain->changeTab( tabMember, i18n( "Members Setting" ) );
    bgSettings->setTitle( i18n( "when Qun message arrives" ) );
    rbRecord_Show->setText( i18n( "record and notify in system tray" ) );
    rbPopupWindow->setText( i18n( "popup chatting window" ) );
    rbDisplayNumbers->setText( i18n( "don't notify in system tray" ) );
    rbRecord_only->setText( i18n( "record only" ) );
    rbRejectMsgs->setText( i18n( "reject all messages from this Qun" ) );
    tabWMain->changeTab( tabSetting, i18n( "Message Setting" ) );
    lblMyName->setText( i18n( "Real Name" ) );
    lblMyGender->setText( i18n( "Gender" ) );
    lblMyPhone->setText( i18n( "Telephone" ) );
    lblMyEmail->setText( i18n( "Email" ) );
    cbbMyGender->clear();
    cbbMyGender->insertItem( i18n( "Male" ) );
    cbbMyGender->insertItem( i18n( "Female" ) );
    cbbMyGender->insertItem( i18n( "-" ) );
    lblMyComment->setText( i18n( "Memo" ) );
    tabWMain->changeTab( TabCard, i18n( "Qun Card" ) );
    pbUpdate->setText( i18n( "&Update" ) );
    pbUpdate->setAccel( QKeySequence( i18n( "Alt+U" ) ) );
    pbClose->setText( i18n( "&Close" ) );
    pbClose->setAccel( QKeySequence( i18n( "Alt+C" ) ) );
}

