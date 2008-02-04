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

#include "evachatuibase.h"
#include "evatextedit.h"
#include "evachatview.h"
#include "evafilepanel.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qsplitter.h>
#include <qvaluelist.h>
#include <qframe.h>
#include <klocale.h>

#include <kapplication.h>

EvaChatUIBase::EvaChatUIBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
	if ( !name )
		setName( "EvaChatUIBase" );
	EvaChatUIBaseLayout = new QHBoxLayout( this, 0, 0, "EvaChatUIBaseLayout");

	splitter = new QSplitter( this, "splitter" );
	splitter->setOrientation( QSplitter::Vertical );
	QWidget *upperLayoutWidget = new QWidget(splitter, "upper");

	layout5 = new QVBoxLayout( upperLayoutWidget, 0, 3, "layout5");

	layout4 = new QHBoxLayout( 0, 3, 6, "layout4"); 
	
	tbBuddy = new QToolButton( upperLayoutWidget, "tbBuddy" );
	tbBuddy->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbBuddy->sizePolicy().hasHeightForWidth() ) );
	tbBuddy->setMinimumSize( QSize( 20, 24 ) );
	tbBuddy->setMaximumSize( QSize( 300, 24 ) );
	tbBuddy->setUsesTextLabel( TRUE );
	tbBuddy->setAutoRaise( TRUE );
	tbBuddy->setTextPosition( QToolButton::BesideIcon );
	layout4->addWidget( tbBuddy );
	
	lblIP = new QLabel( upperLayoutWidget, "lblIP" );
	lblIP->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, lblIP->sizePolicy().hasHeightForWidth() ) );
	lblIP->setMinimumSize( QSize( 15, 0 ) );
	lblIP->setMaximumSize( QSize( 15, 32767 ) );
	layout4->addWidget( lblIP );
	
	leIP = new QLineEdit( upperLayoutWidget, "leIP" );
	leIP->setPaletteBackgroundColor( QColor( 238, 238, 230 ) );
	leIP->setFrameShape( QLineEdit::NoFrame );
	leIP->setReadOnly( TRUE );
	layout4->addWidget( leIP );
	layout5->addLayout( layout4 );
	
	lblSignature = new QLabel( upperLayoutWidget, "lblSignature");
	lblSignature->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, (QSizePolicy::SizeType)0, 0, 0, lblSignature->sizePolicy().hasHeightForWidth() ) );
	lblSignature->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop | QLabel::AlignLeft ) );
	layout5->addWidget(lblSignature);
	
	chatDisplay = new EvaChatView(upperLayoutWidget, "chatDisplay");
	chatDisplay->view()->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred, 0, 3, chatDisplay->view()->sizePolicy().hasHeightForWidth() ) );
	chatDisplay->view()->setHScrollBarMode(QScrollView::AlwaysOff);
	layout5->addWidget( chatDisplay->view() );
	
	
	lowerLayoutWidget = new QWidget(splitter, "lower");
	lowerLayout = new QVBoxLayout(lowerLayoutWidget, 0, 3, "toolLayout");

	layout3 = new QHBoxLayout( 0, 0, 0, "layout3"); 
	
	tbSmiley = new QToolButton( lowerLayoutWidget, "tbSmiley" );
	tbSmiley->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbSmiley->sizePolicy().hasHeightForWidth() ) );
	tbSmiley->setMinimumSize( QSize( 24, 24 ) );
	tbSmiley->setMaximumSize( QSize( 24, 24 ) );
	tbSmiley->setAutoRaise( TRUE );
	layout3->addWidget( tbSmiley );
	
	tbFont = new QToolButton( lowerLayoutWidget, "tbFont" );
	tbFont->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbFont->sizePolicy().hasHeightForWidth() ) );
	tbFont->setMinimumSize( QSize( 24, 24 ) );
	tbFont->setMaximumSize( QSize( 24, 24 ) );
	tbFont->setAutoRaise( TRUE );
	layout3->addWidget( tbFont );

	tbFile = new QToolButton( lowerLayoutWidget, "tbFile" );
	tbFile->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbFile->sizePolicy().hasHeightForWidth() ) );
	tbFile->setMinimumSize( QSize( 24, 24 ) );
	tbFile->setMaximumSize( QSize( 24, 24 ) );
	tbFile->setAutoRaise( TRUE );
	layout3->addWidget( tbFile );

	spcTootBtn1 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
	layout3->addItem( spcTootBtn1 );
	
	tbB = new QToolButton( lowerLayoutWidget, "tbB" );
	tbB->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbB->sizePolicy().hasHeightForWidth() ) );
	tbB->setMinimumSize( QSize( 24, 24 ) );
	tbB->setMaximumSize( QSize( 24, 24 ) );
	tbB->setToggleButton( TRUE );
	tbB->setAutoRaise( TRUE );
	layout3->addWidget( tbB );
	
	tbU = new QToolButton( lowerLayoutWidget, "tbU" );
	tbU->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbU->sizePolicy().hasHeightForWidth() ) );
	tbU->setMinimumSize( QSize( 24, 24 ) );
	tbU->setMaximumSize( QSize( 24, 24 ) );
	tbU->setToggleButton( TRUE );
	tbU->setUsesTextLabel( FALSE );
	tbU->setAutoRaise( TRUE );
	layout3->addWidget( tbU );
	
	tbI = new QToolButton( lowerLayoutWidget, "tbI" );
	tbI->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbI->sizePolicy().hasHeightForWidth() ) );
	tbI->setMinimumSize( QSize( 24, 24 ) );
	tbI->setMaximumSize( QSize( 24, 24 ) );
	tbI->setToggleButton( TRUE );
	tbI->setAutoRaise( TRUE );
	layout3->addWidget( tbI );
	spcToolBtn2 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
	layout3->addItem( spcToolBtn2 );

	tbAddImage = new QToolButton( lowerLayoutWidget, "tbAddImage");
	tbAddImage->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbAddImage->sizePolicy().hasHeightForWidth() ) );
	tbAddImage->setMinimumSize( QSize( 24, 24 ) );
	tbAddImage->setMaximumSize( QSize( 24, 24 ) );
	tbAddImage->setAutoRaise( TRUE );
	layout3->addWidget( tbAddImage );

	tbScreenShot = new QToolButton( lowerLayoutWidget, "tbScreenShot" );
	tbScreenShot->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbScreenShot->sizePolicy().hasHeightForWidth() ) );
	tbScreenShot->setMinimumSize( QSize( 24, 24 ) );
	tbScreenShot->setMaximumSize( QSize( 24, 24 ) );
	tbScreenShot->setAutoRaise( TRUE );
	layout3->addWidget( tbScreenShot );

	tbQuickReply = new QToolButton( lowerLayoutWidget, "tbQuickReply" );
	tbQuickReply->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbQuickReply->sizePolicy().hasHeightForWidth() ) );
	tbQuickReply->setMinimumSize( QSize( 24, 24 ) );
	tbQuickReply->setMaximumSize( QSize( 24, 24 ) );
	tbQuickReply->setAutoRaise( TRUE );
	layout3->addWidget( tbQuickReply );
	spcToolBtn3 = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout3->addItem( spcToolBtn3 );
	
	tbHideShows = new QToolButton( lowerLayoutWidget, "tbHideShows" );
	tbHideShows->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbHideShows->sizePolicy().hasHeightForWidth() ) );
	tbHideShows->setMinimumSize( QSize( 24, 24 ) );
	tbHideShows->setMaximumSize( QSize( 24, 24 ) );
	tbHideShows->setAutoRaise( TRUE );
	layout3->addWidget( tbHideShows );

	lowerLayout->addLayout(layout3);
	
	kteInput = new EvaTextEdit( lowerLayoutWidget, "kteInput" );
	kteInput->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)2, (QSizePolicy::SizeType)1, 0, 2, kteInput->sizePolicy().hasHeightForWidth() ) );
	kteInput->setMinimumSize( QSize( 0, 10 ) );
	kteInput->setHScrollBarMode( KTextEdit::AlwaysOff );
	kteInput->setTextFormat(Qt::RichText);
	lowerLayout->addWidget( kteInput );
	
	layout2 = new QHBoxLayout( 0, 4, 6, "layout2"); 
	
	pbHistory = new QPushButton( lowerLayoutWidget, "pbHistory" );
	layout2->addWidget( pbHistory );
	spcBottom = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( spcBottom );
	
	pbClose = new QPushButton( lowerLayoutWidget, "pbClose" );
	layout2->addWidget( pbClose );
	
	pbSend = new QPushButton( lowerLayoutWidget, "pbSend" );
	layout2->addWidget( pbSend );
	
	pbSendKey = new QPushButton( lowerLayoutWidget, "pbSendKey" );
	pbSendKey->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pbSendKey->sizePolicy().hasHeightForWidth() ) );
	pbSendKey->setMaximumSize( QSize( 15, 32767 ) );
	layout2->addWidget( pbSendKey );
	lowerLayout->addLayout(layout2);
	
	EvaChatUIBaseLayout->addWidget(splitter);
	
	layout19 = new QVBoxLayout( 0, 10, 6, "layout19");

	spacer12 = new QSpacerItem( 20, 3, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout19->addItem( spacer12 );
	
	m_FilePanelFrame = new QFrame(this, "m_FilePanelFrame");
	m_FilePanelFrame->setMaximumSize(QSize(115, 200));
	m_FilePanelFrame->setFrameShape(QFrame::LineEditPanel);
	m_FilePanelLayout = new QVBoxLayout( m_FilePanelFrame, 2, 2, "m_FilePanelLayout"); // parent, margin, space
		
	m_FilePanel = new EvaFilePanel(m_FilePanelFrame, "m_FilePanel");
	m_FilePanelLayout->addWidget(m_FilePanel);

	layout19->addWidget(m_FilePanelFrame);

	m_ShowFrame = new QFrame(this, "m_ShowFrame");
	m_ShowFrame->setFrameShape(QFrame::LineEditPanel);
	m_ShowLayout = new QVBoxLayout( m_ShowFrame, 10, 1, "m_ShowLayout"); // parent, margin, space

	lblBuddyShow = new QLabel( m_ShowFrame, "lblBuddyShow" );
	lblBuddyShow->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, lblBuddyShow->sizePolicy().hasHeightForWidth() ) );
	lblBuddyShow->setAlignment( int( QLabel::AlignVCenter ) );
	m_ShowLayout->addWidget( lblBuddyShow );
	
	tbShowBuddy = new QToolButton( m_ShowFrame, "tbShowBuddy" );
	tbShowBuddy->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, tbShowBuddy->sizePolicy().hasHeightForWidth() ) );
	tbShowBuddy->setMinimumSize( QSize( 95, 120 ) );
	tbShowBuddy->setMaximumSize( QSize( 95, 120 ) );
	tbShowBuddy->setAutoRepeat( false );
	tbShowBuddy->setUsesBigPixmap( true );
	m_ShowLayout->addWidget( tbShowBuddy );
	
	lblMeShow = new QLabel( m_ShowFrame, "lblMeShow" );
	lblMeShow->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, lblMeShow->sizePolicy().hasHeightForWidth() ) );
	lblMeShow->setAlignment( int( QLabel::AlignVCenter ) );
	//layout19->addWidget( lblMeShow );
	m_ShowLayout->addWidget( lblMeShow );
	
	tbShowMe = new QToolButton( m_ShowFrame, "tbShowMe" );
	tbShowMe->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, tbShowMe->sizePolicy().hasHeightForWidth() ) );
	tbShowMe->setMinimumSize( QSize( 95, 120 ) );
	tbShowMe->setMaximumSize( QSize( 95, 120 ) );
	tbShowMe->setUsesBigPixmap( true );
	tbShowMe->setAutoRaise( false );
	//layout19->addWidget( tbShowMe );
	m_ShowLayout->addWidget(tbShowMe);
	
	layout19->addWidget(m_ShowFrame);

	spacer10 = new QSpacerItem( 20, 3, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout19->addItem( spacer10 );

	//EvaChatUIBaseLayout->addLayout(layout19);

	m_FilePanelFrame->hide();
	m_ShowFrame->hide();

	QValueList<int> list;
	list.append(150);
	list.append(120);
	//list.append(100);
	splitter->setSizes(list);

	languageChange();
	resize( QSize(320, 367).expandedTo(minimumSizeHint()) );
	//resize( QSize(450, 422).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
	QRect scr = KApplication::desktop()->screenGeometry();
	move(scr.center()-rect().center());
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaChatUIBase::~EvaChatUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaChatUIBase::languageChange()
{
	setCaption( i18n( "Chat" ) );
	tbBuddy->setText( i18n( "..." ) );
	QToolTip::add( tbBuddy, i18n( "click for details" ) );
	lblIP->setText( i18n( "IP:" ) );
	tbSmiley->setText( i18n( "..." ) );
	tbSmiley->setAccel( QKeySequence( tr( "Alt+M" ) ) );
	QToolTip::add( tbSmiley, i18n( "add a smiley icon" ) );
	tbFont->setText( i18n( "..." ) );
	tbFont->setAccel( QKeySequence( tr( "Alt+F" ) ) );
	QToolTip::add( tbFont, i18n( "choose font size and color" ) );
	tbFile->setText( i18n( "..." ) );
	tbFile->setAccel( QKeySequence( tr( "Alt+L" ) ) );
	QToolTip::add( tbFile, i18n( "send file to friend" ) );

	tbB->setText( i18n( "B" ) );
	tbB->setAccel( QKeySequence( tr( "Alt+B" ) ) );
	QToolTip::add( tbB, i18n( "bold" ) );
	tbU->setText( i18n( "U" ) );
	tbU->setAccel( QKeySequence( tr( "Alt+U" ) ) );
	tbU->setTextLabel( QString::null );
	QToolTip::add( tbU, i18n( "underline" ) );
	tbI->setText( i18n( "I" ) );
	tbI->setAccel( QKeySequence( tr( "Alt+I" ) ) );
	QToolTip::add( tbI, i18n( "italic" ) );
	tbAddImage->setText("...");
	tbAddImage->setAccel( QKeySequence( tr( "Alt+A" ) ) );
	tbAddImage->setText( i18n( "..." ) );
	QToolTip::add( tbAddImage, i18n( "send image files" ) );
	tbScreenShot->setText(i18n("..."));
	tbScreenShot->setAccel( QKeySequence( tr( "Alt+T" ) ) );
	QToolTip::add( tbScreenShot, i18n( "add a screen shot" ) );
	tbQuickReply->setAccel( QKeySequence( tr( "Alt+R" ) ) );
	QToolTip::add( tbQuickReply, i18n( "select an quick-reply message" ) );
	
	
	tbHideShows->setText( i18n( "..." ) );
	tbHideShows->setAccel( QKeySequence( tr( "Alt+P" ) ) );
	QToolTip::add( tbHideShows, i18n( "hide/display QQ show panel" ) );
	
	pbHistory->setText( i18n( "&History" ) );
	pbHistory->setAccel( QKeySequence( tr( "Alt+H" ) ) );
	QToolTip::add( pbHistory, i18n( "view IM history(Alt+H)" ) );
	pbClose->setText( i18n( "Close" ) );
	pbClose->setAccel( QKeySequence( tr( "Alt+X" ) ) );
	QToolTip::add( pbClose, i18n( "close this window(Alt+X)" ) );
	pbSend->setText( i18n( "&Send" ) );
	pbSend->setAccel( QKeySequence( tr( "Alt+S" ) ) );
	QToolTip::add( pbSend, i18n( "send message" ) );
	pbSendKey->setText( QString::null );
	pbSendKey->setAccel( QKeySequence( QString::null ) );
	QToolTip::add( pbSendKey, i18n( "change send key" ) );
	lblBuddyShow->setText( i18n( "Buddy" ) );
	tbShowBuddy->setText( i18n( "..." ) );
	QToolTip::add( tbShowBuddy, i18n( "Click to update" ) );
	lblMeShow->setText( i18n( "Me" ) );
	tbShowMe->setText( i18n( "..." ) );
	tbShowMe->setAccel( QKeySequence( QString::null ) );
	QToolTip::add( tbShowMe, i18n( "Click to update" ) );
}

