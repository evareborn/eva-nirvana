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

#include "evaqunchatuibase.h"
#include "evatextedit.h"
#include "evachatview.h"
#include "evaqunlistview.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qframe.h>
#include <qlabel.h>
#include <qheader.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qsplitter.h>
#include <qvaluelist.h>
#include <klocale.h>

#include <kapplication.h>

/*
 *  Constructs a EvaQunChatUIBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
EvaQunChatUIBase::EvaQunChatUIBase( QWidget* parent, const char* name, WFlags fl )
	: QWidget( parent, name, fl )
{
	if ( !name )
		setName( "EvaQunChatUIBase" );
	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 3, 3, sizePolicy().hasHeightForWidth() ) );
	setSizeIncrement( QSize( 2, 2 ) );
	EvaQunChatUIBaseLayout = new QGridLayout( this, 1, 1, 2, 2, "EvaQunChatUIBaseLayout"); 
	EvaQunChatUIBaseLayout->setResizeMode( QLayout::FreeResize );
	
	layout16 = new QHBoxLayout( 0, 0, 6, "layout16"); 
	
	layout15 = new QVBoxLayout( 0, 0, 2, "layout15");
	
	layout6 = new QHBoxLayout( 0, 0, 6, "layout6"); 
	
	tbQunDetails = new QToolButton( this, "tbQunDetails" );
	tbQunDetails->setUsesTextLabel( TRUE );
	tbQunDetails->setAutoRaise( TRUE );
	tbQunDetails->setTextPosition( QToolButton::BesideIcon );
	layout6->addWidget( tbQunDetails );
	spacer4 = new QSpacerItem( 81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout6->addItem( spacer4 );
	layout15->addLayout( layout6 );

	splitter = new QSplitter( this, "splitter" );
	splitter->setOrientation( QSplitter::Vertical );

	chatDisplay = new EvaChatView(splitter, "chatDisplay");
	chatDisplay->view()->setHScrollBarMode(QScrollView::AlwaysOff);
	//chatDisplay->view()->setMinimumSize(QSize( 0, 170 ) );
	//layout15->addWidget( chatDisplay->view() );

	
	lowerLayoutWidget = new QWidget(splitter, "lower");

	lowerLayout = new QVBoxLayout(lowerLayoutWidget, 0, 3, "lowerLayout");
	layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 
	
	tbSmiley = new QToolButton( lowerLayoutWidget, "tbSmiley" );
	tbSmiley->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbSmiley->sizePolicy().hasHeightForWidth() ) );
	tbSmiley->setMinimumSize( QSize( 24, 24 ) );
	tbSmiley->setMaximumSize( QSize( 24, 24 ) );
	tbSmiley->setAutoRaise( TRUE );
	layout14->addWidget( tbSmiley );
	
	tbFont = new QToolButton( lowerLayoutWidget, "tbFont" );
	tbFont->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbFont->sizePolicy().hasHeightForWidth() ) );
	tbFont->setMinimumSize( QSize( 24, 24 ) );
	tbFont->setMaximumSize( QSize( 24, 24 ) );
	tbFont->setAutoRaise( TRUE );
	layout14->addWidget( tbFont );
	
	spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
	layout14->addItem( spacer2 );
	
	tbB = new QToolButton( lowerLayoutWidget, "tbB" );
	tbB->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbB->sizePolicy().hasHeightForWidth() ) );
	tbB->setMinimumSize( QSize( 24, 24 ) );
	tbB->setMaximumSize( QSize( 24, 24 ) );
	tbB->setToggleButton( TRUE );
	tbB->setAutoRaise( TRUE );
	layout14->addWidget( tbB );
	
	tbU = new QToolButton( lowerLayoutWidget, "tbU" );
	tbU->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbU->sizePolicy().hasHeightForWidth() ) );
	tbU->setMinimumSize( QSize( 24, 24 ) );
	tbU->setMaximumSize( QSize( 24, 24 ) );
	tbU->setToggleButton( TRUE );
	tbU->setAutoRaise( TRUE );
	layout14->addWidget( tbU );
	
	tbI = new QToolButton( lowerLayoutWidget, "tbI" );
	tbI->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbI->sizePolicy().hasHeightForWidth() ) );
	tbI->setMinimumSize( QSize( 24, 24 ) );
	tbI->setMaximumSize( QSize( 24, 24 ) );
	tbI->setToggleButton( TRUE );
	tbI->setAutoRaise( TRUE );
	layout14->addWidget( tbI );
	spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
	layout14->addItem( spacer3 );
	
	tbImageFile = new QToolButton( lowerLayoutWidget, "tbImageFile" );
	tbImageFile->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbImageFile->sizePolicy().hasHeightForWidth() ) );
	tbImageFile->setMinimumSize( QSize( 24, 24 ) );
	tbImageFile->setMaximumSize( QSize( 24, 24 ) );
	tbImageFile->setAutoRaise( TRUE );
	layout14->addWidget( tbImageFile );
	
	tbScreenShot = new QToolButton( lowerLayoutWidget, "tbScreenShot" );
	tbScreenShot->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbScreenShot->sizePolicy().hasHeightForWidth() ) );
	tbScreenShot->setMinimumSize( QSize( 24, 24 ) );
	tbScreenShot->setMaximumSize( QSize( 24, 24 ) );
	tbScreenShot->setAutoRaise( TRUE );
	layout14->addWidget( tbScreenShot );
	
	tbQuickReply = new QToolButton( lowerLayoutWidget, "tbQuickReply" );
	tbQuickReply->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbQuickReply->sizePolicy().hasHeightForWidth() ) );
	tbQuickReply->setMinimumSize( QSize( 24, 24 ) );
	tbQuickReply->setMaximumSize( QSize( 24, 24 ) );
	tbQuickReply->setAutoRaise( TRUE );
	layout14->addWidget( tbQuickReply );
	spacer5 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout14->addItem( spacer5 );

	lowerLayout->addLayout(layout14);
	//layout15->addLayout( layout14 );

	teInput = new EvaTextEdit( lowerLayoutWidget, "kteInput" );
	teInput->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)1, 0, 0, teInput->sizePolicy().hasHeightForWidth() ) );
	teInput->setMinimumSize( QSize( 0, 10 ) );
	//teInput->setMaximumSize( QSize( 32767, 160 ) );
	teInput->setHScrollBarMode( KTextEdit::AlwaysOff );
	teInput->setTextFormat(Qt::RichText);
	lowerLayout->addWidget(teInput);

	//layout15->addWidget( teInput );
	layout15->addWidget(splitter);
		
	layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
	
	pbHistory = new QPushButton( this, "pbHistory" );
	layout3->addWidget( pbHistory );
	spacer1 = new QSpacerItem( 81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout3->addItem( spacer1 );
	
	pbClose = new QPushButton( this, "pbClose" );
	layout3->addWidget( pbClose );
	
	pbSend = new QPushButton( this, "pbSend" );
	layout3->addWidget( pbSend );
	
	pbSendKey = new QPushButton( this, "pbSendKey" );
	pbSendKey->setMaximumSize( QSize( 20, 32767 ) );
	layout3->addWidget( pbSendKey );
	layout15->addLayout( layout3 );
	layout16->addLayout( layout15 );
	
	memberSplitter = new QSplitter( this, "memberSplitter" );
	memberSplitter->setOrientation( QSplitter::Vertical );

	
	frmNotice = new QFrame( memberSplitter, "frmNoice" ); /// --------------
	//frmNotice->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, frmNotice->sizePolicy().hasHeightForWidth() ) );
	frmNotice->setMaximumSize( QSize( 140, 32767 ) );
	frmNotice->setFrameShape( QFrame::LineEditPanel );
	frmNotice->setFrameShadow( QFrame::Raised );
	frmNoticeLayout = new QGridLayout( frmNotice, 1, 1, 2, 2, "frmNoiceLayout"); 
	
	layout4 = new QVBoxLayout( 0, 0, 0, "layout4"); 
	
	tbtnNotice = new QToolButton( frmNotice, "tbtnNotice" );
	tbtnNotice->setEnabled( TRUE );
	tbtnNotice->setMaximumSize( QSize( 32767, 24 ) );
	tbtnNotice->setUsesTextLabel( TRUE );
	tbtnNotice->setAutoRaise( TRUE );
	tbtnNotice->setTextPosition( QToolButton::BesideIcon );
	layout4->addWidget( tbtnNotice );
	
	//kslblNotice = new KSqueezedTextLabel( frmNotice, "kslblNotice" );
//	teNotice = new QLabel(frmNotice, "teNotice");
	teNotice = new QTextEdit(frmNotice, "teNotice");
	//kslblNotice->setMaximumSize( QSize( 32767, 32767 ) );
	teNotice->setMaximumSize( QSize( 136, 32767 ) );
	teNotice->setPaletteBackgroundColor( QColor( 255, 253, 211 ) );
	teNotice->setFrameShape( QLabel::LineEditPanel );
	teNotice->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );
	teNotice->setVScrollBarMode(QScrollView::AlwaysOff);
	teNotice->setReadOnly(true);
	//layout4->addWidget( kslblNotice );
	layout4->addWidget( teNotice );
	
	frmNoticeLayout->addLayout( layout4, 0, 0 );

	frmMembers = new QFrame(memberSplitter, "frmMembers");
	frmMemberLayout = new QVBoxLayout( frmMembers, 1, 1, "memberLayout"); 
	
	lblMembers = new QLabel( frmMembers, "lblMembers" );
	lblMembers->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, lblMembers->sizePolicy().hasHeightForWidth() ) );
	lblMembers->setMaximumSize( QSize( 140, 32767 ) );
	frmMemberLayout->addWidget( lblMembers );
	
	memberList = new EvaQunListView( frmMembers, "memberList" );
	memberList->addColumn(  "" );
	memberList->addColumn( "" );
	memberList->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, memberList->sizePolicy().hasHeightForWidth() ) );
	memberList->setMaximumSize( QSize( 140, 32767 ) );
	memberList->setAllColumnsShowFocus(true);
	//memberList->setHScrollBarMode( QListView::AlwaysOff );
	//memberList->setSelectionMode( QListView::Extended );
	memberList->header()->hide();
	memberList->setSorting(0);
	memberList->setRootIsDecorated( false );
	memberList->setTreeStepSize( 0 );   
	memberList->setDragAutoScroll(true);
	frmMemberLayout->addWidget(memberList);

	layout12 = new QVBoxLayout( 0, 4, 4, "layout12"); /// --------------	
	layout12->addWidget( memberSplitter );
	layout16->addLayout( layout12 );
	
	EvaQunChatUIBaseLayout->addLayout( layout16, 0, 0 );

	QValueList<int> list;
	list.append(150);
	list.append(80);
	splitter->setSizes(list);

	languageChange();
	resize( QSize(470, 422).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
	QRect scr = KApplication::desktop()->screenGeometry();
	move(scr.center()-rect().center());
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaQunChatUIBase::~EvaQunChatUIBase()
{
	// no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaQunChatUIBase::languageChange()
{
	setCaption( i18n( "Qun" ) );
	tbQunDetails->setText( i18n( "..." ) );
	tbSmiley->setText( i18n( "..." ) );
	QToolTip::add( tbSmiley, i18n( "add a smiley icon" ) );
	tbFont->setText( i18n( "..." ) );
	QToolTip::add( tbFont, i18n( "choose font size and color" ) );
	tbB->setText( i18n( "..." ) );
	tbB->setAccel( QKeySequence( tr( "Alt+B" ) ) );
	QToolTip::add( tbB, i18n( "bold" ) );
	tbU->setText( i18n( "..." ) );
	tbU->setAccel( QKeySequence( tr( "Alt+U" ) ) );
	QToolTip::add( tbU, i18n( "underline" ) );
	tbI->setText( i18n( "..." ) );
	tbI->setAccel( QKeySequence( tr( "Alt+I" ) ) );
	QToolTip::add( tbI, i18n( "italic" ) );
	tbImageFile->setText(i18n("..."));
	tbImageFile->setAccel( QKeySequence( tr( "Alt+A" ) ) );
	QToolTip::add( tbImageFile, i18n( "add a custom smiley (file size less than 50K)" ) );
	tbScreenShot->setText(i18n("..."));
	tbScreenShot->setAccel( QKeySequence( tr( "Alt+T" ) ) );
	QToolTip::add( tbScreenShot, i18n( "add a screen shot ( less than 700 X 600 )" ) );
	tbQuickReply->setText( i18n( "..." ) );
	tbQuickReply->setAccel( QKeySequence( tr( "Alt+R" ) ) );
	QToolTip::add( tbQuickReply, i18n( "select an quick-reply message" ) );
	pbHistory->setText( i18n( "History" ) );
	pbHistory->setAccel( QKeySequence( tr( "Alt+H" ) ) );
	QToolTip::add( pbHistory, i18n( "view IM history(Alt+H)" ) );
	pbClose->setText( i18n( "Close" ) );
	pbClose->setAccel( QKeySequence( tr( "Alt+X" ) ) );
	QToolTip::add( pbClose, i18n( "close this window(Alt+X)" ) );
	pbSend->setText( i18n( "&Send" ) );
	pbSend->setAccel( QKeySequence( i18n( "Alt+S" ) ) );
	QToolTip::add( pbSend, i18n( "send message" ) );
	pbSendKey->setText( QString::null );
	QToolTip::add( pbSendKey, i18n( "change send key" ) );
	tbtnNotice->setTextLabel( i18n( "Notice" ) );
	QToolTip::add( tbtnNotice, i18n( "Click to open/close qun notice" ) );
	//kslblNotice->setText( i18n( "Notice Here" ) );
	lblMembers->setText( i18n( "Members" ) );
	//listView1->header()->setLabel( 0, i18n( "Column 1" ) );
	//listView1->header()->setLabel( 1, i18n( "Column 2" ) );
	memberList->clear();
	QListViewItem * item = new QListViewItem( memberList, 0 );
	item->setText( 0, i18n( "New Item" ) );

}

