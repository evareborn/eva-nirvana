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

#include "mainlistfontsettinguibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <ktabwidget.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <kcolorcombo.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <klocale.h>
//#include <qtooltip.h>
//#include <qwhatsthis.h>


MainListFontSettingUIBase::MainListFontSettingUIBase( QWidget* parent, const char* name, WFlags fl )
	: QWidget( parent, name, fl )
{
	if ( !name )
		setName( "MainListFontSettingUIBase" );
	MainListFontSettingUIBaseLayout = new QGridLayout( this, 1, 1, 11, 6, "MainListFontSettingUIBaseLayout"); 
	
	ktabMainListFont = new KTabWidget( this, "ktabMainListFont" );
	ktabMainListFont->setCurrentPage( 0 );
	
	TabPage = new QWidget( ktabMainListFont, "TabPage" );
	TabPageLayout = new QVBoxLayout( TabPage, 11, 9, "TabPageLayout"); 
	
	gbBuddyNick = new QGroupBox( TabPage, "gbBuddyNick" );
	gbBuddyNick->setColumnLayout(0, Qt::Vertical );
	gbBuddyNick->layout()->setSpacing( 6 );
	gbBuddyNick->layout()->setMargin( 11 );
	gbBuddyNickLayout = new QVBoxLayout( gbBuddyNick->layout() );
	gbBuddyNickLayout->setAlignment( Qt::AlignTop );
	
	layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

	lblBuddyNickColor = new QLabel( gbBuddyNick, "lblBuddyNickColor" );
	layout1->addWidget( lblBuddyNickColor );
	
	kcbbBuddyNickColor = new KColorCombo( gbBuddyNick, "kcbbBuddyNickColor" );
	layout1->addWidget( kcbbBuddyNickColor );
	gbBuddyNickLayout->addLayout( layout1 );
	
	layout2 = new QHBoxLayout( 0, 0, 6, "layout2"); 
	
	chbBuddyNickFontB = new QCheckBox( gbBuddyNick, "chbBuddyNickFontB" );
	layout2->addWidget( chbBuddyNickFontB );

	chbBuddyNickFontU = new QCheckBox( gbBuddyNick, "chbBuddyNickFontU" );
	layout2->addWidget( chbBuddyNickFontU );
	
	chbBuddyNickFontI = new QCheckBox( gbBuddyNick, "chbBuddyNickFontI" );
	layout2->addWidget( chbBuddyNickFontI );
	gbBuddyNickLayout->addLayout( layout2 );
	TabPageLayout->addWidget( gbBuddyNick );
	
	gbBuddyFlash = new QGroupBox( TabPage, "gbBuddyFlash" );
	gbBuddyFlash->setColumnLayout(0, Qt::Vertical );
	gbBuddyFlash->layout()->setSpacing( 6 );
	gbBuddyFlash->layout()->setMargin( 11 );
	gbBuddyFlashLayout = new QVBoxLayout( gbBuddyFlash->layout() );
	gbBuddyFlashLayout->setAlignment( Qt::AlignTop );
	
	layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
	
	lblBuddyFlashColor = new QLabel( gbBuddyFlash, "lblBuddyFlashColor" );
	layout3->addWidget( lblBuddyFlashColor );
	
	kcbbBuddyFlashColor = new KColorCombo( gbBuddyFlash, "kcbbBuddyFlashColor" );
	layout3->addWidget( kcbbBuddyFlashColor );
	gbBuddyFlashLayout->addLayout( layout3 );
	
	layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 
	
	chbBuddyFlashFontB = new QCheckBox( gbBuddyFlash, "chbBuddyFlashFontB" );
	layout4->addWidget( chbBuddyFlashFontB );
	
	chbBuddyFlashFontU = new QCheckBox( gbBuddyFlash, "chbBuddyFlashFontU" );
	layout4->addWidget( chbBuddyFlashFontU );

	chbBuddyFlashFontI = new QCheckBox( gbBuddyFlash, "chbBuddyFlashFontI" );
	layout4->addWidget( chbBuddyFlashFontI );
	gbBuddyFlashLayout->addLayout( layout4 );
	TabPageLayout->addWidget( gbBuddyFlash );
	
	gbBuddySignature = new QGroupBox( TabPage, "gbBuddySignature" );
	gbBuddySignature->setColumnLayout(0, Qt::Vertical );
	gbBuddySignature->layout()->setSpacing( 6 );
	gbBuddySignature->layout()->setMargin( 11 );
	gbBuddySignatureLayout = new QVBoxLayout( gbBuddySignature->layout() );
	gbBuddySignatureLayout->setAlignment( Qt::AlignTop );
	
	layout5 = new QHBoxLayout( 0, 0, 6, "layout5"); 
	
	lblBuddySigColor = new QLabel( gbBuddySignature, "lblBuddySigColor" );
	layout5->addWidget( lblBuddySigColor );

	kcbbBuddySigColor = new KColorCombo( gbBuddySignature, "kcbbBuddySigColor" );
	layout5->addWidget( kcbbBuddySigColor );
	gbBuddySignatureLayout->addLayout( layout5 );
	
	layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 
	
	chbBuddySigFontB = new QCheckBox( gbBuddySignature, "chbBuddySigFontB" );
	layout6->addWidget( chbBuddySigFontB );
	
	chbBuddySigFontU = new QCheckBox( gbBuddySignature, "chbBuddySigFontU" );
	layout6->addWidget( chbBuddySigFontU );
	
	chbBuddySigFontI = new QCheckBox( gbBuddySignature, "chbBuddySigFontI" );
	layout6->addWidget( chbBuddySigFontI );
	gbBuddySignatureLayout->addLayout( layout6 );
	TabPageLayout->addWidget( gbBuddySignature );
	
	ktabMainListFont->addTab(TabPage, i18n("Buddy Font"));

	TabPage_2 = new QWidget( ktabMainListFont, "TabPage_2" );
	TabPageLayout_2 = new QVBoxLayout( TabPage_2, 11, 9, "TabPageLayout_2"); 

	layout11 = new QVBoxLayout( 0, 0, 6, "layout11"); 
	
	gbQunName = new QGroupBox( TabPage_2, "gbQunName" );
	gbQunName->setColumnLayout(0, Qt::Vertical );
	gbQunName->layout()->setSpacing( 6 );
	gbQunName->layout()->setMargin( 11 );
	gbQunNameLayout = new QVBoxLayout( gbQunName->layout() );
	gbQunNameLayout->setAlignment( Qt::AlignTop );
	
	layout1_2 = new QHBoxLayout( 0, 0, 6, "layout1_2"); 
	
	lblQunNameColor = new QLabel( gbQunName, "lblQunNameColor" );
	layout1_2->addWidget( lblQunNameColor );
	
	kcbbQunNameColor = new KColorCombo( gbQunName, "kcbbQunNameColor" );
	layout1_2->addWidget( kcbbQunNameColor );
	gbQunNameLayout->addLayout( layout1_2 );
	
	layout2_2 = new QHBoxLayout( 0, 0, 6, "layout2_2"); 
	
	chbQunNameFontB = new QCheckBox( gbQunName, "chbQunNameFontB" );
	layout2_2->addWidget( chbQunNameFontB );
	
	chbQunNameFontU = new QCheckBox( gbQunName, "chbQunNameFontU" );
	layout2_2->addWidget( chbQunNameFontU );
	
	chbQunNameFontI = new QCheckBox( gbQunName, "chbQunNameFontI" );
	layout2_2->addWidget( chbQunNameFontI );
	gbQunNameLayout->addLayout( layout2_2 );
	layout11->addWidget( gbQunName );
	
	gbQunFlash = new QGroupBox( TabPage_2, "gbQunFlash" );
	gbQunFlash->setColumnLayout(0, Qt::Vertical );
	gbQunFlash->layout()->setSpacing( 6 );
	gbQunFlash->layout()->setMargin( 11 );
	gbQunFlashLayout = new QVBoxLayout( gbQunFlash->layout() );
	gbQunFlashLayout->setAlignment( Qt::AlignTop );
	
	layout3_2 = new QHBoxLayout( 0, 0, 6, "layout3_2"); 

	lblQunFlashColor = new QLabel( gbQunFlash, "lblQunFlashColor" );
	layout3_2->addWidget( lblQunFlashColor );
	
	kcbbQunFlashColor = new KColorCombo( gbQunFlash, "kcbbQunFlashColor" );
	layout3_2->addWidget( kcbbQunFlashColor );
	gbQunFlashLayout->addLayout( layout3_2 );
	
	layout4_2 = new QHBoxLayout( 0, 0, 6, "layout4_2"); 
	
	chbQunFlashFontB = new QCheckBox( gbQunFlash, "chbQunFlashFontB" );
	layout4_2->addWidget( chbQunFlashFontB );
	
	chbQunFlashFontU = new QCheckBox( gbQunFlash, "chbQunFlashFontU" );
	layout4_2->addWidget( chbQunFlashFontU );
	
	chbQunFlashFontI = new QCheckBox( gbQunFlash, "chbQunFlashFontI" );
	layout4_2->addWidget( chbQunFlashFontI );
	gbQunFlashLayout->addLayout( layout4_2 );
	layout11->addWidget( gbQunFlash );
	TabPageLayout_2->addLayout( layout11 );
	ktabMainListFont->addTab(TabPage_2, i18n("Qun Font"));
	
	TabPage_3 = new QWidget( ktabMainListFont, "TabPage_3" );
	TabPageLayout_3 = new QVBoxLayout( TabPage_3, 11, 6, "TabPageLayout_3"); 
	
	gbGroupName = new QGroupBox( TabPage_3, "gbGroupName" );
	gbGroupName->setColumnLayout(0, Qt::Vertical );
	gbGroupName->layout()->setSpacing( 6 );
	gbGroupName->layout()->setMargin( 11 );
	gbGroupNameLayout = new QVBoxLayout( gbGroupName->layout() );
	gbGroupNameLayout->setAlignment( Qt::AlignTop );
	
	layout1_3 = new QHBoxLayout( 0, 0, 6, "layout1_3"); 
	
	lblGroupNameColor = new QLabel( gbGroupName, "lblGroupNameColor" );
	layout1_3->addWidget( lblGroupNameColor );
	
	kcbbGroupNameColor = new KColorCombo( gbGroupName, "kcbbGroupNameColor" );
	layout1_3->addWidget( kcbbGroupNameColor );
	gbGroupNameLayout->addLayout( layout1_3 );
	
	layout2_3 = new QHBoxLayout( 0, 0, 6, "layout2_3"); 
	
	chbGroupNameFontB = new QCheckBox( gbGroupName, "chbGroupNameFontB" );
	layout2_3->addWidget( chbGroupNameFontB );
	
	chbGroupNameFontU = new QCheckBox( gbGroupName, "chbGroupNameFontU" );
	layout2_3->addWidget( chbGroupNameFontU );
	
	chbGroupNameFontI = new QCheckBox( gbGroupName, "chbGroupNameFontI" );
	layout2_3->addWidget( chbGroupNameFontI );
	gbGroupNameLayout->addLayout( layout2_3 );
	TabPageLayout_3->addWidget( gbGroupName );
	
	gbGroupFlash = new QGroupBox( TabPage_3, "gbGroupFlash" );
	gbGroupFlash->setColumnLayout(0, Qt::Vertical );
	gbGroupFlash->layout()->setSpacing( 6 );
	gbGroupFlash->layout()->setMargin( 11 );
	gbGroupFlashLayout = new QVBoxLayout( gbGroupFlash->layout() );
	gbGroupFlashLayout->setAlignment( Qt::AlignTop );
	
	layout3_3 = new QHBoxLayout( 0, 0, 6, "layout3_3"); 
	
	lblGroupFlashColor = new QLabel( gbGroupFlash, "lblGroupFlashColor" );
	layout3_3->addWidget( lblGroupFlashColor );
	
	kcbbGroupFlashColor = new KColorCombo( gbGroupFlash, "kcbbGroupFlashColor" );
	layout3_3->addWidget( kcbbGroupFlashColor );
	gbGroupFlashLayout->addLayout( layout3_3 );
	
	layout4_3 = new QHBoxLayout( 0, 0, 6, "layout4_3"); 
	
	chbGroupFlashFontB = new QCheckBox( gbGroupFlash, "chbGroupFlashFontB" );
	layout4_3->addWidget( chbGroupFlashFontB );
	
	chbGroupFlashFontU = new QCheckBox( gbGroupFlash, "chbGroupFlashFontU" );
	layout4_3->addWidget( chbGroupFlashFontU );
	
	chbGroupFlashFontI = new QCheckBox( gbGroupFlash, "chbGroupFlashFontI" );
	layout4_3->addWidget( chbGroupFlashFontI );
	gbGroupFlashLayout->addLayout( layout4_3 );
	TabPageLayout_3->addWidget( gbGroupFlash );
	
	gbGroupOnlineCount = new QGroupBox( TabPage_3, "gbGroupOnlineCount" );
	gbGroupOnlineCount->setColumnLayout(0, Qt::Vertical );
	gbGroupOnlineCount->layout()->setSpacing( 6 );
	gbGroupOnlineCount->layout()->setMargin( 11 );
	gbGroupOnlineCountLayout = new QVBoxLayout( gbGroupOnlineCount->layout() );
	gbGroupOnlineCountLayout->setAlignment( Qt::AlignTop );
	
	layout5_2 = new QHBoxLayout( 0, 0, 6, "layout5_2"); 
	
	lblGroupOnlineColor = new QLabel( gbGroupOnlineCount, "lblGroupOnlineColor" );
	layout5_2->addWidget( lblGroupOnlineColor );
	
	kcbbGroupOnlineColor = new KColorCombo( gbGroupOnlineCount, "kcbbGroupOnlineColor" );
	layout5_2->addWidget( kcbbGroupOnlineColor );
	gbGroupOnlineCountLayout->addLayout( layout5_2 );
	
	layout6_2 = new QHBoxLayout( 0, 0, 6, "layout6_2"); 

	chbGroupOnlineFontB = new QCheckBox( gbGroupOnlineCount, "chbGroupOnlineFontB" );
	layout6_2->addWidget( chbGroupOnlineFontB );
	
	chbGroupOnlineFontU = new QCheckBox( gbGroupOnlineCount, "chbGroupOnlineFontU" );
	layout6_2->addWidget( chbGroupOnlineFontU );
	
	chbGroupOnlineFontI = new QCheckBox( gbGroupOnlineCount, "chbGroupOnlineFontI" );
	layout6_2->addWidget( chbGroupOnlineFontI );
	gbGroupOnlineCountLayout->addLayout( layout6_2 );
	TabPageLayout_3->addWidget( gbGroupOnlineCount );
	ktabMainListFont->addTab(TabPage_3, i18n("Group Font"));

	MainListFontSettingUIBaseLayout->addWidget( ktabMainListFont, 0, 0 );
	languageChange();
	resize( QSize(341, 345).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
MainListFontSettingUIBase::~MainListFontSettingUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MainListFontSettingUIBase::languageChange()
{
	setCaption( QString::null );
	gbBuddyNick->setTitle( i18n("Nickname:" ) );
	lblBuddyNickColor->setText( i18n("nickname color:" ) );
	chbBuddyNickFontB->setText( i18n("bold" ) );
	chbBuddyNickFontU->setText( i18n("underline" ) );
	chbBuddyNickFontI->setText( i18n("italic" ) );
	gbBuddyFlash->setTitle( i18n("Message Flashing:" ) );
	lblBuddyFlashColor->setText( i18n("flashing color:" ) );
	chbBuddyFlashFontB->setText( i18n("bold" ) );
	chbBuddyFlashFontU->setText( i18n("underline" ) );
	chbBuddyFlashFontI->setText( i18n("italic" ) );
	gbBuddySignature->setTitle( i18n("Signature:" ) );
	lblBuddySigColor->setText( i18n("signature color:" ) );
	chbBuddySigFontB->setText( i18n("bold" ) );
	chbBuddySigFontU->setText( i18n("underline" ) );
	chbBuddySigFontI->setText( i18n("italic" ) );
	gbQunName->setTitle( i18n("Name:" ) );
	lblQunNameColor->setText( i18n("name color:" ) );
	chbQunNameFontB->setText( i18n("bold" ) );
	chbQunNameFontU->setText( i18n("underline" ) );
	chbQunNameFontI->setText( i18n("italic" ) );
	gbQunFlash->setTitle( i18n("Message Flashing:" ) );
	lblQunFlashColor->setText( i18n("flashing color:" ) );
	chbQunFlashFontB->setText( i18n("bold" ) );
	chbQunFlashFontU->setText( i18n("underline" ) );
	chbQunFlashFontI->setText( i18n("italic" ) );
	gbGroupName->setTitle( i18n("Name:" ) );
	lblGroupNameColor->setText( i18n("name color:" ) );
	chbGroupNameFontB->setText( i18n("bold" ) );
	chbGroupNameFontU->setText( i18n("underline" ) );
	chbGroupNameFontI->setText( i18n("italic" ) );
	gbGroupFlash->setTitle( i18n("Message Flashing:" ) );
	lblGroupFlashColor->setText( i18n("flashing color:" ) );
	chbGroupFlashFontB->setText( i18n("bold" ) );
	chbGroupFlashFontU->setText( i18n("underline" ) );
	chbGroupFlashFontI->setText( i18n("italic" ) );
	gbGroupOnlineCount->setTitle( i18n("Online statistics:" ) );
	lblGroupOnlineColor->setText( i18n("signature color:" ) );
	chbGroupOnlineFontB->setText( i18n("bold" ) );
	chbGroupOnlineFontU->setText( i18n("underline" ) );
	chbGroupOnlineFontI->setText( i18n("italic" ) );
}

