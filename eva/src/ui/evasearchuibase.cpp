/***************************************************************************
 *   Copyright (C) 2006 by casper                                          *
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



#include "evasearchuibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qwidgetstack.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qtable.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <klocale.h>

/*
 *  Constructs a EvaSearchUIBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
EvaSearchUIBase::EvaSearchUIBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "EvaSearchUIBase" );
    setEnabled( TRUE );
    setMinimumSize( QSize( 535, 360 ) );
    setMaximumSize( QSize( 535, 360 ) );
    EvaSearchUIBaseLayout = new QGridLayout( this, 1, 1, 11, 6, "EvaSearchUIBaseLayout"); 
    EvaSearchUIBaseLayout->setResizeMode( QLayout::Minimum );

    layout127 = new QGridLayout( 0, 1, 1, 0, 6, "layout127"); 

    twSearchMain = new QTabWidget( this, "twSearchMain" );
    twSearchMain->setMinimumSize( QSize( 0, 305 ) );
    twSearchMain->setMaximumSize( QSize( 32767, 305 ) );
    twSearchMain->setMargin( 0 );

    tabBasicSearch = new QWidget( twSearchMain, "tabBasicSearch" );
    tabBasicSearchLayout = new QGridLayout( tabBasicSearch, 1, 1, 11, 6, "tabBasicSearchLayout"); 

    wsBasicSearch = new QWidgetStack( tabBasicSearch, "wsBasicSearch" );
    wsBasicSearch->setMargin( 0 );

    wsBSPage = new QWidget( wsBasicSearch, "wsBSPage" );
    wsBSPageLayout = new QGridLayout( wsBSPage, 1, 1, 11, 6, "wsBSPageLayout"); 

    layout135 = new QVBoxLayout( 0, 0, 6, "layout135"); 

    layout105 = new QVBoxLayout( 0, 3, 5, "layout105"); 

    lblSearchOptionTip = new QLabel( wsBSPage, "lblSearchOptionTip" );
    layout105->addWidget( lblSearchOptionTip );

    layout100 = new QHBoxLayout( 0, 3, 6, "layout100"); 

    bgBSType = new QButtonGroup( wsBSPage, "bgBSType" );
    bgBSType->setMaximumSize( QSize( 32767, 94 ) );
    bgBSType->setFrameShape( QButtonGroup::NoFrame );
    bgBSType->setRadioButtonExclusive( TRUE );
    bgBSType->setProperty( "selectedId", -1 );
    bgBSType->setColumnLayout(0, Qt::Vertical );
    bgBSType->layout()->setSpacing( 0 );
    bgBSType->layout()->setMargin( 0 );
    bgBSTypeLayout = new QGridLayout( bgBSType->layout() );
    bgBSTypeLayout->setAlignment( Qt::AlignTop );

    rbCustomSearch = new QRadioButton( bgBSType, "rbCustomSearch" );

    bgBSTypeLayout->addWidget( rbCustomSearch, 1, 0 );

    rbSearchFrdCenter = new QRadioButton( bgBSType, "rbSearchFrdCenter" );

    bgBSTypeLayout->addWidget( rbSearchFrdCenter, 2, 0 );

    rbSearchOnline = new QRadioButton( bgBSType, "rbSearchOnline" );

    bgBSTypeLayout->addWidget( rbSearchOnline, 0, 0 );
    layout100->addWidget( bgBSType );
    spacer54 = new QSpacerItem( 81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout100->addItem( spacer54 );
    layout105->addLayout( layout100 );
    layout135->addLayout( layout105 );

    wsBasicCondtion = new QWidgetStack( wsBSPage, "wsBasicCondtion" );
    wsBasicCondtion->setMargin( -9 );

    wsCustomSearchCondition = new QWidget( wsBasicCondtion, "wsCustomSearchCondition" );
    wsCustomSearchConditionLayout = new QGridLayout( wsCustomSearchCondition, 1, 1, 11, 6, "wsCustomSearchConditionLayout"); 

    gbCustomCondition = new QGroupBox( wsCustomSearchCondition, "gbCustomCondition" );
    gbCustomCondition->setMargin( 0 );
    gbCustomCondition->setColumnLayout(0, Qt::Vertical );
    gbCustomCondition->layout()->setSpacing( 6 );
    gbCustomCondition->layout()->setMargin( 11 );
    gbCustomConditionLayout = new QGridLayout( gbCustomCondition->layout() );
    gbCustomConditionLayout->setAlignment( Qt::AlignTop );

    layout204_2 = new QHBoxLayout( 0, 0, 6, "layout204_2"); 

    layout188_2 = new QVBoxLayout( 0, 0, 6, "layout188_2"); 

    lblQQNum = new QLabel( gbCustomCondition, "lblQQNum" );
    layout188_2->addWidget( lblQQNum );

    lblNickName = new QLabel( gbCustomCondition, "lblNickName" );
    layout188_2->addWidget( lblNickName );

    layout204_2->addLayout( layout188_2 );

    layout203_2 = new QVBoxLayout( 0, 0, 6, "layout203_2"); 

    leQQNum = new QLineEdit( gbCustomCondition, "leQQNum" );
    leQQNum->setMinimumSize( QSize( 150, 0 ) );
    leQQNum->setFrameShape( QLineEdit::Box );
    leQQNum->setFrameShadow( QLineEdit::Plain );
    leQQNum->setLineWidth( 1 );
    layout203_2->addWidget( leQQNum );

    leNickName = new QLineEdit( gbCustomCondition, "leNickName" );
    leNickName->setFrameShape( QLineEdit::Box );
    leNickName->setFrameShadow( QLineEdit::Plain );
    leNickName->setLineWidth( 1 );
    layout203_2->addWidget( leNickName );

    layout204_2->addLayout( layout203_2 );

    gbCustomConditionLayout->addLayout( layout204_2, 0, 0 );
    spacer119_2 = new QSpacerItem( 70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    gbCustomConditionLayout->addItem( spacer119_2, 0, 1 );

    wsCustomSearchConditionLayout->addWidget( gbCustomCondition, 0, 0 );
    wsBasicCondtion->addWidget( wsCustomSearchCondition, 0 );

    wsFrdCenterSearchCondition = new QWidget( wsBasicCondtion, "wsFrdCenterSearchCondition" );
    wsFrdCenterSearchConditionLayout = new QGridLayout( wsFrdCenterSearchCondition, 1, 1, 11, 6, "wsFrdCenterSearchConditionLayout"); 

    gbFrdCenterCondition = new QGroupBox( wsFrdCenterSearchCondition, "gbFrdCenterCondition" );
    gbFrdCenterCondition->setMargin( 0 );
    gbFrdCenterCondition->setColumnLayout(0, Qt::Vertical );
    gbFrdCenterCondition->layout()->setSpacing( 2 );
    gbFrdCenterCondition->layout()->setMargin( 8 );
    gbFrdCenterConditionLayout = new QGridLayout( gbFrdCenterCondition->layout() );
    gbFrdCenterConditionLayout->setAlignment( Qt::AlignTop );

    layout46 = new QVBoxLayout( 0, 0, 6, "layout46"); 
    spacer27 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout46->addItem( spacer27 );

    lblQQFrdTip = new QLabel( gbFrdCenterCondition, "lblQQFrdTip" );
    lblQQFrdTip->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout46->addWidget( lblQQFrdTip );
    spacer28 = new QSpacerItem( 20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout46->addItem( spacer28 );

    gbFrdCenterConditionLayout->addLayout( layout46, 0, 0 );

    wsFrdCenterSearchConditionLayout->addWidget( gbFrdCenterCondition, 0, 0 );
    wsBasicCondtion->addWidget( wsFrdCenterSearchCondition, 1 );
    layout135->addWidget( wsBasicCondtion );

    layout316_2 = new QHBoxLayout( 0, 0, 6, "layout316_2"); 

    lblOnlineNumbers = new QLabel( wsBSPage, "lblOnlineNumbers" );
    layout316_2->addWidget( lblOnlineNumbers );

    lblOnlineNum = new QLabel( wsBSPage, "lblOnlineNum" );
    layout316_2->addWidget( lblOnlineNum );
    spacer111 = new QSpacerItem( 183, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout316_2->addItem( spacer111 );
    layout135->addLayout( layout316_2 );

    wsBSPageLayout->addLayout( layout135, 0, 0 );
    wsBasicSearch->addWidget( wsBSPage, 0 );

    wsBSResultPage = new QWidget( wsBasicSearch, "wsBSResultPage" );
    wsBSResultPageLayout = new QGridLayout( wsBSResultPage, 1, 1, 11, 6, "wsBSResultPageLayout"); 

    layout47 = new QVBoxLayout( 0, 0, 6, "layout47"); 

    layout96 = new QHBoxLayout( 0, 0, 6, "layout96"); 

    lblBSResultTip = new QLabel( wsBSResultPage, "lblBSResultTip" );
    layout96->addWidget( lblBSResultTip );
    spacer46 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout96->addItem( spacer46 );
    layout47->addLayout( layout96 );

    tbBSResult = new QTable( wsBSResultPage, "tbBSResult" );
    tbBSResult->setNumCols( tbBSResult->numCols() + 1 );
    tbBSResult->horizontalHeader()->setLabel( tbBSResult->numCols() - 1, i18n( "Account" ) );
    tbBSResult->setNumCols( tbBSResult->numCols() + 1 );
    tbBSResult->horizontalHeader()->setLabel( tbBSResult->numCols() - 1, i18n( "Nick Name" ) );
    tbBSResult->setNumCols( tbBSResult->numCols() + 1 );
    tbBSResult->horizontalHeader()->setLabel( tbBSResult->numCols() - 1, i18n( "From" ) );
    tbBSResult->setFrameShape( QTable::Box );
    tbBSResult->setFrameShadow( QTable::Plain );
    tbBSResult->setLineWidth( 1 );
    tbBSResult->setHScrollBarMode( QTable::Auto );
    tbBSResult->setNumRows( 0 );
    tbBSResult->setNumCols( 3 );
    tbBSResult->setShowGrid( TRUE );
    tbBSResult->setReadOnly( TRUE );
    tbBSResult->setSelectionMode( QTable::SingleRow );
    tbBSResult->setFocusStyle( QTable::FollowStyle );
    layout47->addWidget( tbBSResult );

    layout46_2 = new QHBoxLayout( 0, 0, 6, "layout46_2"); 

    lblBSResultPage = new QLabel( wsBSResultPage, "lblBSResultPage" );
    layout46_2->addWidget( lblBSResultPage );
    spacer47 = new QSpacerItem( 59, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout46_2->addItem( spacer47 );

    tbBSAll = new QToolButton( wsBSResultPage, "tbBSAll" );
    tbBSAll->setCursor( QCursor( 13 ) );
    tbBSAll->setAutoRaise( TRUE );
    layout46_2->addWidget( tbBSAll );

    tbBSDetails = new QToolButton( wsBSResultPage, "tbBSDetails" );
    tbBSDetails->setCursor( QCursor( 13 ) );
    tbBSDetails->setAutoRaise( TRUE );
    layout46_2->addWidget( tbBSDetails );

    tbBSPrev = new QToolButton( wsBSResultPage, "tbBSPrev" );
    tbBSPrev->setCursor( QCursor( 13 ) );
    tbBSPrev->setAutoRaise( TRUE );
    layout46_2->addWidget( tbBSPrev );

    tbBSNext = new QToolButton( wsBSResultPage, "tbBSNext" );
    tbBSNext->setCursor( QCursor( 13 ) );
    tbBSNext->setAutoRaise( TRUE );
    layout46_2->addWidget( tbBSNext );
    layout47->addLayout( layout46_2 );

    wsBSResultPageLayout->addLayout( layout47, 0, 0 );
    wsBasicSearch->addWidget( wsBSResultPage, 1 );

    tabBasicSearchLayout->addWidget( wsBasicSearch, 0, 0 );
    twSearchMain->insertTab( tabBasicSearch, QString::fromLatin1("") );

    tabAdvancedSearch = new QWidget( twSearchMain, "tabAdvancedSearch" );
    tabAdvancedSearchLayout = new QGridLayout( tabAdvancedSearch, 1, 1, 11, 6, "tabAdvancedSearchLayout"); 

    wsAdvancedSearch = new QWidgetStack( tabAdvancedSearch, "wsAdvancedSearch" );

    wsASPage = new QWidget( wsAdvancedSearch, "wsASPage" );
    wsASPageLayout = new QGridLayout( wsASPage, 1, 1, 11, 6, "wsASPageLayout"); 

    layout91 = new QVBoxLayout( 0, 0, 6, "layout91"); 

    layout371 = new QVBoxLayout( 0, 0, 6, "layout371"); 

    lblASTip = new QLabel( wsASPage, "lblASTip" );
    layout371->addWidget( lblASTip );

    layout370 = new QHBoxLayout( 0, 0, 6, "layout370"); 
    spacer156 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout370->addItem( spacer156 );

    layout369 = new QHBoxLayout( 0, 0, 6, "layout369"); 

    chbOnlineUsers = new QCheckBox( wsASPage, "chbOnlineUsers" );
    layout369->addWidget( chbOnlineUsers );
    spacer155 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout369->addItem( spacer155 );

    chbHaveCamera = new QCheckBox( wsASPage, "chbHaveCamera" );
    layout369->addWidget( chbHaveCamera );
    layout370->addLayout( layout369 );
    spacer157 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout370->addItem( spacer157 );
    layout371->addLayout( layout370 );
    layout91->addLayout( layout371 );

    gbBasicCondition = new QGroupBox( wsASPage, "gbBasicCondition" );
    gbBasicCondition->setColumnLayout(0, Qt::Vertical );
    gbBasicCondition->layout()->setSpacing( 6 );
    gbBasicCondition->layout()->setMargin( 11 );
    gbBasicConditionLayout = new QGridLayout( gbBasicCondition->layout() );
    gbBasicConditionLayout->setAlignment( Qt::AlignTop );

    layout376 = new QHBoxLayout( 0, 0, 6, "layout376"); 

    layout372 = new QVBoxLayout( 0, 0, 6, "layout372"); 

    lblASProvince = new QLabel( gbBasicCondition, "lblASProvince" );
    layout372->addWidget( lblASProvince );

    lblASCity = new QLabel( gbBasicCondition, "lblASCity" );
    layout372->addWidget( lblASCity );

    lblASAge = new QLabel( gbBasicCondition, "lblASAge" );
    layout372->addWidget( lblASAge );

    lblASGender = new QLabel( gbBasicCondition, "lblASGender" );
    layout372->addWidget( lblASGender );
    layout376->addLayout( layout372 );

    layout375 = new QVBoxLayout( 0, 0, 6, "layout375"); 

    cbASProvince = new QComboBox( FALSE, gbBasicCondition, "cbASProvince" );
    layout375->addWidget( cbASProvince );

    cbASCity = new QComboBox( FALSE, gbBasicCondition, "cbASCity" );
    layout375->addWidget( cbASCity );

    cbASAge = new QComboBox( FALSE, gbBasicCondition, "cbASAge" );
    layout375->addWidget( cbASAge );

    layout373 = new QHBoxLayout( 0, 0, 6, "layout373"); 

    cbASSex = new QComboBox( FALSE, gbBasicCondition, "cbASSex" );
    cbASSex->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, cbASSex->sizePolicy().hasHeightForWidth() ) );
    layout373->addWidget( cbASSex );
    spacer158 = new QSpacerItem( 51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout373->addItem( spacer158 );
    layout375->addLayout( layout373 );
    layout376->addLayout( layout375 );

    gbBasicConditionLayout->addLayout( layout376, 0, 0 );
    spacer159 = new QSpacerItem( 101, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    gbBasicConditionLayout->addItem( spacer159, 0, 1 );
    layout91->addWidget( gbBasicCondition );
    spacer40_2 = new QSpacerItem( 20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout91->addItem( spacer40_2 );

    wsASPageLayout->addLayout( layout91, 0, 0 );
    wsAdvancedSearch->addWidget( wsASPage, 0 );

    wsASResultPage = new QWidget( wsAdvancedSearch, "wsASResultPage" );
    wsASResultPageLayout = new QVBoxLayout( wsASResultPage, 11, 6, "wsASResultPageLayout"); 

    layout47_2 = new QVBoxLayout( 0, 0, 6, "layout47_2"); 

    layout96_2 = new QHBoxLayout( 0, 0, 6, "layout96_2"); 

    lblASResultTip = new QLabel( wsASResultPage, "lblASResultTip" );
    layout96_2->addWidget( lblASResultTip );
    spacer48 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout96_2->addItem( spacer48 );
    layout47_2->addLayout( layout96_2 );

    tbASResult = new QTable( wsASResultPage, "tbASResult" );
    tbASResult->setNumCols( tbASResult->numCols() + 1 );
    tbASResult->horizontalHeader()->setLabel( tbASResult->numCols() - 1, i18n( "Account" ) );
    tbASResult->setNumCols( tbASResult->numCols() + 1 );
    tbASResult->horizontalHeader()->setLabel( tbASResult->numCols() - 1, i18n( "Nick Name" ) );
    tbASResult->setNumCols( tbASResult->numCols() + 1 );
    tbASResult->horizontalHeader()->setLabel( tbASResult->numCols() - 1, i18n( "Gender" ) );
    tbASResult->setNumCols( tbASResult->numCols() + 1 );
    tbASResult->horizontalHeader()->setLabel( tbASResult->numCols() - 1, i18n( "Age" ) );
    tbASResult->setNumCols( tbASResult->numCols() + 1 );
    tbASResult->horizontalHeader()->setLabel( tbASResult->numCols() - 1, i18n( "Province" ) );
    tbASResult->setNumCols( tbASResult->numCols() + 1 );
    tbASResult->horizontalHeader()->setLabel( tbASResult->numCols() - 1, i18n( "City" ) );
    tbASResult->setNumCols( tbASResult->numCols() + 1 );
    tbASResult->horizontalHeader()->setLabel( tbASResult->numCols() - 1, i18n( "Status" ) );
    tbASResult->setFrameShape( QTable::Box );
    tbASResult->setFrameShadow( QTable::Plain );
    tbASResult->setLineWidth( 1 );
    tbASResult->setHScrollBarMode( QTable::Auto );
    tbASResult->setNumRows( 0 );
    tbASResult->setNumCols( 7 );
    tbASResult->setShowGrid( TRUE );
    tbASResult->setReadOnly( TRUE );
    tbASResult->setSelectionMode( QTable::SingleRow );
    tbASResult->setFocusStyle( QTable::FollowStyle );
    layout47_2->addWidget( tbASResult );

    layout46_2_2 = new QHBoxLayout( 0, 0, 6, "layout46_2_2"); 

    lblASResultPage = new QLabel( wsASResultPage, "lblASResultPage" );
    layout46_2_2->addWidget( lblASResultPage );
    spacer50 = new QSpacerItem( 59, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout46_2_2->addItem( spacer50 );

    tbASAll = new QToolButton( wsASResultPage, "tbASAll" );
    tbASAll->setCursor( QCursor( 13 ) );
    tbASAll->setAutoRaise( TRUE );
    layout46_2_2->addWidget( tbASAll );

    tbASDetails = new QToolButton( wsASResultPage, "tbASDetails" );
    tbASDetails->setCursor( QCursor( 13 ) );
    tbASDetails->setAutoRaise( TRUE );
    layout46_2_2->addWidget( tbASDetails );

    tbASPrev = new QToolButton( wsASResultPage, "tbASPrev" );
    tbASPrev->setCursor( QCursor( 13 ) );
    tbASPrev->setAutoRaise( TRUE );
    layout46_2_2->addWidget( tbASPrev );

    tbASNext = new QToolButton( wsASResultPage, "tbASNext" );
    tbASNext->setCursor( QCursor( 13 ) );
    tbASNext->setAutoRaise( TRUE );
    layout46_2_2->addWidget( tbASNext );
    layout47_2->addLayout( layout46_2_2 );
    wsASResultPageLayout->addLayout( layout47_2 );
    wsAdvancedSearch->addWidget( wsASResultPage, 1 );

    tabAdvancedSearchLayout->addWidget( wsAdvancedSearch, 0, 0 );
    twSearchMain->insertTab( tabAdvancedSearch, QString::fromLatin1("") );

    tabQunSearch = new QWidget( twSearchMain, "tabQunSearch" );
    tabQunSearchLayout = new QGridLayout( tabQunSearch, 1, 1, 11, 6, "tabQunSearchLayout"); 

    wsQunSearch = new QWidgetStack( tabQunSearch, "wsQunSearch" );

    wsQSPage = new QWidget( wsQunSearch, "wsQSPage" );
    wsQSPageLayout = new QGridLayout( wsQSPage, 1, 1, 11, 6, "wsQSPageLayout"); 

    layout203 = new QVBoxLayout( 0, 0, 6, "layout203"); 

    layout92 = new QHBoxLayout( 0, 0, 0, "layout92"); 
    spacer173_2 = new QSpacerItem( 40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout92->addItem( spacer173_2 );

    layout400_2 = new QVBoxLayout( 0, 0, 0, "layout400_2"); 

    lblQSTip = new QLabel( wsQSPage, "lblQSTip" );
    layout400_2->addWidget( lblQSTip );

    layout399_2 = new QHBoxLayout( 0, 0, 6, "layout399_2"); 

    bgQSType = new QButtonGroup( wsQSPage, "bgQSType" );
    bgQSType->setFrameShape( QButtonGroup::NoFrame );
    bgQSType->setFrameShadow( QButtonGroup::Plain );
    bgQSType->setColumnLayout(0, Qt::Vertical );
    bgQSType->layout()->setSpacing( 2 );
    bgQSType->layout()->setMargin( 6 );
    bgQSTypeLayout = new QGridLayout( bgQSType->layout() );
    bgQSTypeLayout->setAlignment( Qt::AlignTop );

    rbSearchAlumni = new QRadioButton( bgQSType, "rbSearchAlumni" );

    bgQSTypeLayout->addWidget( rbSearchAlumni, 0, 0 );

    rbAccuratelySearch = new QRadioButton( bgQSType, "rbAccuratelySearch" );

    bgQSTypeLayout->addWidget( rbAccuratelySearch, 2, 0 );

    rbSearchByCategory = new QRadioButton( bgQSType, "rbSearchByCategory" );

    bgQSTypeLayout->addWidget( rbSearchByCategory, 1, 0 );
    layout399_2->addWidget( bgQSType );
    spacer172_2 = new QSpacerItem( 81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout399_2->addItem( spacer172_2 );
    layout400_2->addLayout( layout399_2 );
    layout92->addLayout( layout400_2 );
    spacer174_2 = new QSpacerItem( 51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout92->addItem( spacer174_2 );
    layout203->addLayout( layout92 );

    gbQCondition = new QGroupBox( wsQSPage, "gbQCondition" );
    gbQCondition->setColumnLayout(0, Qt::Vertical );
    gbQCondition->layout()->setSpacing( 0 );
    gbQCondition->layout()->setMargin( 8 );
    gbQConditionLayout = new QGridLayout( gbQCondition->layout() );
    gbQConditionLayout->setAlignment( Qt::AlignTop );

    wsQunCondition = new QWidgetStack( gbQCondition, "wsQunCondition" );

    wsCategoryConditon = new QWidget( wsQunCondition, "wsCategoryConditon" );
    wsCategoryConditonLayout = new QGridLayout( wsCategoryConditon, 1, 1, 11, 6, "wsCategoryConditonLayout"); 

    layout402_2 = new QVBoxLayout( 0, 0, 6, "layout402_2"); 

    layout388_2 = new QHBoxLayout( 0, 0, 6, "layout388_2"); 

    lblCategory = new QLabel( wsCategoryConditon, "lblCategory" );
    layout388_2->addWidget( lblCategory );

    cbCategory1 = new QComboBox( FALSE, wsCategoryConditon, "cbCategory1" );
    cbCategory1->setMinimumSize( QSize( 100, 0 ) );
    layout388_2->addWidget( cbCategory1 );
    layout402_2->addLayout( layout388_2 );

    layout394_2 = new QHBoxLayout( 0, 0, 6, "layout394_2"); 
    spacer168_2 = new QSpacerItem( 51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout394_2->addItem( spacer168_2 );

    layout390_2 = new QVBoxLayout( 0, 0, 6, "layout390_2"); 

    cbCategory2 = new QComboBox( FALSE, wsCategoryConditon, "cbCategory2" );
    cbCategory2->setMinimumSize( QSize( 100, 0 ) );
    layout390_2->addWidget( cbCategory2 );

    cbCategory3 = new QComboBox( FALSE, wsCategoryConditon, "cbCategory3" );
    layout390_2->addWidget( cbCategory3 );
    layout394_2->addLayout( layout390_2 );
    layout402_2->addLayout( layout394_2 );

    wsCategoryConditonLayout->addLayout( layout402_2, 0, 1 );
    spacer148 = new QSpacerItem( 50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    wsCategoryConditonLayout->addItem( spacer148, 0, 0 );
    spacer149 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    wsCategoryConditonLayout->addItem( spacer149, 0, 2 );
    wsQunCondition->addWidget( wsCategoryConditon, 0 );

    wsAlumniCondition = new QWidget( wsQunCondition, "wsAlumniCondition" );
    wsAlumniConditionLayout = new QGridLayout( wsAlumniCondition, 1, 1, 11, 6, "wsAlumniConditionLayout"); 

    lblAlumniTip = new QLabel( wsAlumniCondition, "lblAlumniTip" );
    lblAlumniTip->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    wsAlumniConditionLayout->addWidget( lblAlumniTip, 0, 0 );
    wsQunCondition->addWidget( wsAlumniCondition, 1 );

    wsAccurateCondition = new QWidget( wsQunCondition, "wsAccurateCondition" );
    wsAccurateConditionLayout = new QGridLayout( wsAccurateCondition, 1, 1, 11, 6, "wsAccurateConditionLayout"); 

    layout306 = new QHBoxLayout( 0, 0, 6, "layout306"); 

    lblQunNum = new QLabel( wsAccurateCondition, "lblQunNum" );
    layout306->addWidget( lblQunNum );

    leQunNum = new QLineEdit( wsAccurateCondition, "leQunNum" );
    leQunNum->setFrameShape( QLineEdit::Box );
    leQunNum->setFrameShadow( QLineEdit::Plain );
    leQunNum->setLineWidth( 1 );
    layout306->addWidget( leQunNum );

    wsAccurateConditionLayout->addLayout( layout306, 0, 0 );
    spacer151 = new QSpacerItem( 111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    wsAccurateConditionLayout->addItem( spacer151, 0, 1 );
    spacer152 = new QSpacerItem( 20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding );
    wsAccurateConditionLayout->addItem( spacer152, 1, 0 );
    wsQunCondition->addWidget( wsAccurateCondition, 2 );

    gbQConditionLayout->addWidget( wsQunCondition, 0, 0 );
    layout203->addWidget( gbQCondition );

    wsQSPageLayout->addLayout( layout203, 0, 0 );
    wsQunSearch->addWidget( wsQSPage, 0 );

    wsQSResultPage = new QWidget( wsQunSearch, "wsQSResultPage" );
    wsQSResultPageLayout = new QGridLayout( wsQSResultPage, 1, 1, 11, 6, "wsQSResultPageLayout"); 

    layout49 = new QVBoxLayout( 0, 0, 6, "layout49"); 

    layout96_3 = new QHBoxLayout( 0, 0, 6, "layout96_3"); 

    lblQSResultTip = new QLabel( wsQSResultPage, "lblQSResultTip" );
    layout96_3->addWidget( lblQSResultTip );
    spacer46_2 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout96_3->addItem( spacer46_2 );
    layout49->addLayout( layout96_3 );

    tbQSResult = new QTable( wsQSResultPage, "tbQSResult" );
    tbQSResult->setNumCols( tbQSResult->numCols() + 1 );
    tbQSResult->horizontalHeader()->setLabel( tbQSResult->numCols() - 1, i18n( "Qun Num" ) );
    tbQSResult->setNumCols( tbQSResult->numCols() + 1 );
    tbQSResult->horizontalHeader()->setLabel( tbQSResult->numCols() - 1, i18n( "Qun Name" ) );
    tbQSResult->setNumCols( tbQSResult->numCols() + 1 );
    tbQSResult->horizontalHeader()->setLabel( tbQSResult->numCols() - 1, i18n( "Creator" ) );
    tbQSResult->setFrameShape( QTable::Box );
    tbQSResult->setFrameShadow( QTable::Plain );
    tbQSResult->setLineWidth( 1 );
    tbQSResult->setHScrollBarMode( QTable::Auto );
    tbQSResult->setNumRows( 0 );
    tbQSResult->setNumCols( 3 );
    tbQSResult->setShowGrid( TRUE );
    tbQSResult->setReadOnly( TRUE );
    tbQSResult->setSelectionMode( QTable::SingleRow );
    tbQSResult->setFocusStyle( QTable::FollowStyle );
    layout49->addWidget( tbQSResult );

    layout46_3 = new QHBoxLayout( 0, 0, 6, "layout46_3"); 

    lblQSResultPage = new QLabel( wsQSResultPage, "lblQSResultPage" );
    layout46_3->addWidget( lblQSResultPage );
    spacer51 = new QSpacerItem( 59, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout46_3->addItem( spacer51 );

    tbQSAll = new QToolButton( wsQSResultPage, "tbQSAll" );
    tbQSAll->setCursor( QCursor( 13 ) );
    tbQSAll->setAutoRaise( TRUE );
    layout46_3->addWidget( tbQSAll );

    tbQSDetails = new QToolButton( wsQSResultPage, "tbQSDetails" );
    tbQSDetails->setCursor( QCursor( 13 ) );
    tbQSDetails->setAutoRaise( TRUE );
    layout46_3->addWidget( tbQSDetails );

    tbQSPrev = new QToolButton( wsQSResultPage, "tbQSPrev" );
    tbQSPrev->setCursor( QCursor( 13 ) );
    tbQSPrev->setAutoRaise( TRUE );
    layout46_3->addWidget( tbQSPrev );

    tbQSNext = new QToolButton( wsQSResultPage, "tbQSNext" );
    tbQSNext->setCursor( QCursor( 13 ) );
    tbQSNext->setAutoRaise( TRUE );
    layout46_3->addWidget( tbQSNext );
    layout49->addLayout( layout46_3 );

    wsQSResultPageLayout->addLayout( layout49, 0, 0 );
    wsQunSearch->addWidget( wsQSResultPage, 1 );

    tabQunSearchLayout->addWidget( wsQunSearch, 0, 0 );
    twSearchMain->insertTab( tabQunSearch, QString::fromLatin1("") );

    layout127->addWidget( twSearchMain, 0, 1 );

    layout195 = new QHBoxLayout( 0, 0, 6, "layout195"); 
    spacer38 = new QSpacerItem( 140, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout195->addItem( spacer38 );

    pbLastStep = new QPushButton( this, "pbLastStep" );
    layout195->addWidget( pbLastStep );

    pbSearch = new QPushButton( this, "pbSearch" );
    layout195->addWidget( pbSearch );

    pbClose = new QPushButton( this, "pbClose" );
    layout195->addWidget( pbClose );
    spacer40 = new QSpacerItem( 21, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout195->addItem( spacer40 );

    layout127->addMultiCellLayout( layout195, 1, 1, 0, 1 );

    lblLogo = new QLabel( this, "lblLogo" );
    lblLogo->setMinimumSize( QSize( 120, 300 ) );
    lblLogo->setMaximumSize( QSize( 120, 300 ) );

    layout127->addWidget( lblLogo, 0, 0 );

    EvaSearchUIBaseLayout->addLayout( layout127, 0, 0 );
    languageChange();
    resize( QSize(535, 360).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaSearchUIBase::~EvaSearchUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaSearchUIBase::languageChange()
{
    setCaption( i18n( "Search - Eva" ) );
    lblSearchOptionTip->setText( i18n( "Please select one of the following options:" ) );
    bgBSType->setTitle( QString::null );
    rbCustomSearch->setText( i18n( "accurate search" ) );
    rbSearchFrdCenter->setText( i18n( "search in QQ friend center" ) );
    rbSearchOnline->setText( i18n( "search online users" ) );
    gbCustomCondition->setTitle( i18n( "accurate conditions" ) );
    lblQQNum->setText( i18n( "QQ Num:" ) );
    lblNickName->setText( i18n( "Nick Name:" ) );
    //lblEmail->setText( i18n( "--Email:" ) );
    gbFrdCenterCondition->setTitle( i18n( "accurate conditions" ) );
    lblQQFrdTip->setText( i18n( "click the search button to go to the QQ friend homepage for Searching" ) );
    lblOnlineNumbers->setText( i18n( "number of online users :" ) );
    lblOnlineNum->setText( i18n( "unknown" ) );
    lblBSResultTip->setText( i18n( "the following users is found for you by Eva." ) );
    tbBSResult->horizontalHeader()->setLabel( 0, i18n( "QQ Num" ) );
    tbBSResult->horizontalHeader()->setLabel( 1, i18n( "Nick Name" ) );
    tbBSResult->horizontalHeader()->setLabel( 2, i18n( "From" ) );
    lblBSResultPage->setText( QString::null );
    tbBSAll->setText( i18n( "All pages" ) );
    tbBSDetails->setText( i18n( "Details" ) );
    tbBSPrev->setText( i18n( "Prev" ) );
    tbBSNext->setText( i18n( "Next" ) );
    twSearchMain->changeTab( tabBasicSearch, i18n( "&Basic Search" ) );
    lblASTip->setText( i18n( "you may set one condition or more to search friend here" ) );
    chbOnlineUsers->setText( i18n( "online users" ) );
    chbHaveCamera->setText( i18n( "has camera" ) );
    gbBasicCondition->setTitle( i18n( "basic conditions" ) );
    lblASProvince->setText( i18n( "Province" ) );
    lblASCity->setText( i18n( "City" ) );
    lblASAge->setText( i18n( "Age" ) );
    lblASGender->setText( i18n( "Gender" ) );
    cbASAge->clear();
    cbASAge->insertItem( i18n( "any" ) );
    cbASAge->insertItem( i18n( "0-15" ) );
    cbASAge->insertItem( i18n( "16-22" ) );
    cbASAge->insertItem( i18n( "23-30" ) );
    cbASAge->insertItem( i18n( "31-40" ) );
    cbASAge->insertItem( i18n( "above 40" ) );
    cbASSex->clear();
    cbASSex->insertItem( i18n( "any" ) );
    cbASSex->insertItem( i18n( "male" ) );
    cbASSex->insertItem( i18n( "female" ) );
    lblASResultTip->setText( i18n( "the following users is found for you by Eva." ) );
    tbASResult->horizontalHeader()->setLabel( 0, i18n( "QQ Num" ) );
    tbASResult->horizontalHeader()->setLabel( 1, i18n( "Nick Name" ) );
    tbASResult->horizontalHeader()->setLabel( 2, i18n( "Gender" ) );
    tbASResult->horizontalHeader()->setLabel( 3, i18n( "Age" ) );
    tbASResult->horizontalHeader()->setLabel( 4, i18n( "Province" ) );
    tbASResult->horizontalHeader()->setLabel( 5, i18n( "City" ) );
    tbASResult->horizontalHeader()->setLabel( 6, i18n( "Status" ) );
    lblASResultPage->setText( QString::null );
    tbASAll->setText( i18n( "All pages" ) );
    tbASDetails->setText( i18n( "Details" ) );
    tbASPrev->setText( i18n( "Prev" ) );
    tbASNext->setText( i18n( "Next" ) );
    twSearchMain->changeTab( tabAdvancedSearch, i18n( "&Advanced Search" ) );
    lblQSTip->setText( i18n( "Please select one of the following Qun options:" ) );
    bgQSType->setTitle( QString::null );
    rbSearchAlumni->setText( i18n( "search alumni" ) );
    rbAccuratelySearch->setText( i18n( "accurate search" ) );
    rbSearchByCategory->setText( i18n( "search by category" ) );
    gbQCondition->setTitle( i18n( "search conditions" ) );
    lblCategory->setText( i18n( "category" ) );
    lblAlumniTip->setText( i18n( "click the search button to go to the Alumni homepage for Searching" ) );
    lblQunNum->setText( i18n( "Qun Num" ) );
    lblQSResultTip->setText( QString::null );
    tbQSResult->horizontalHeader()->setLabel( 0, i18n( "Qun Num" ) );
    tbQSResult->horizontalHeader()->setLabel( 1, i18n( "Qun Name" ) );
    tbQSResult->horizontalHeader()->setLabel( 2, i18n( "Creator" ) );
    lblQSResultPage->setText( QString::null );
    tbQSAll->setText( i18n( "All pages" ) );
    tbQSDetails->setText( i18n( "Details" ) );
    tbQSPrev->setText( i18n( "Prev" ) );
    tbQSNext->setText( i18n( "Next" ) );
    twSearchMain->changeTab( tabQunSearch, i18n( "&Qun Search" ) );
    pbLastStep->setText( i18n( "Last Step" ) );
    pbSearch->setText( i18n( "Sea&rch" ) );
    pbClose->setText( i18n( "&Close" ) );
    lblLogo->setText( QString::null );
}

