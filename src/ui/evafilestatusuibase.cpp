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

#include "evafilestatusuibase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <kprogress.h>
#include <kpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <klocale.h>
#include <qtimer.h>


EvaFileStatusUIBase::EvaFileStatusUIBase( const unsigned int session, const unsigned int size, 
					QWidget* parent, const char* name, WFlags fl )
	: QWidget( parent, name, fl ), m_StartPosition(0), m_Session(session), m_IsFinished(false)
{
	if ( !name )
	setName( "EvaFileStatusUIBase" );
	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
	setMaximumSize( QSize( 110, 200 ) );
	EvaFileStatusUIBaseLayout = new QGridLayout( this, 1, 1, 2, 2, "EvaFileStatusUIBaseLayout"); 
	
	layout10 = new QVBoxLayout( 0, 0, 0, "layout10");
	
	layout6 = new QHBoxLayout( 0, 0, 6, "layout6");
	
	lblIcon = new QLabel( this, "lblIcon" );
	lblIcon->setMinimumSize( QSize( 32, 32 ) );
	lblIcon->setMaximumSize( QSize( 32, 32 ) );
	lblIcon->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	layout6->addWidget( lblIcon );
	
	lblFileName = new QLabel( this, "lblFileName" );
	lblFileName->setMaximumSize( QSize( 70, 32 ) );
	QFont lblFileName_font( lblFileName->font() );
	lblFileName_font.setPointSize( 8 );
	lblFileName->setFont( lblFileName_font ); 
	lblFileName->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignLeft ) );
	layout6->addWidget( lblFileName );
	layout10->addLayout( layout6 );
	
	layout1 = new QHBoxLayout( 0, 2, 2, "layout1"); 
	
	kpgFileStatus = new KProgress( this, "kpgFileStatus" );
	kpgFileStatus->setMaximumSize( QSize( 75, 15 ) );
	QFont kpgFileStatus_font(  kpgFileStatus->font() );
	kpgFileStatus_font.setPointSize( 8 );
	kpgFileStatus->setFont( kpgFileStatus_font ); 
	kpgFileStatus->setFrameShape( KProgress::NoFrame );
	kpgFileStatus->setIndicatorFollowsStyle( TRUE );
	kpgFileStatus->setTotalSteps(size);
	layout1->addWidget( kpgFileStatus );
	
	kpbtnClose = new KPushButton( this, "kpbtnClose" );
	kpbtnClose->setMaximumSize( QSize( 15, 15 ) );
	kpbtnClose->setFocusPolicy( KPushButton::NoFocus );
	layout1->addWidget( kpbtnClose );
	layout10->addLayout( layout1 );
	
	layout9 = new QVBoxLayout( 0, 0, 0, "layout9"); 
	
	lblReceived = new QLabel( this, "lblReceived" );
	lblReceived->setMaximumSize( QSize( 100, 20 ) );
	QFont lblReceived_font(lblReceived->font());
	lblReceived_font.setPointSize( 8 );
	lblReceived->setFont(lblReceived_font);
	layout9->addWidget( lblReceived );
	
	lblSpeed = new QLabel( this, "lblSpeed" );
	lblSpeed->setMaximumSize( QSize( 100, 20 ) );
	QFont lblSpeed_font( lblSpeed->font() );
	lblSpeed_font.setPointSize( 8 );
	lblSpeed->setFont( lblSpeed_font ); 
	layout9->addWidget( lblSpeed );
	layout10->addLayout( layout9 );
	
	EvaFileStatusUIBaseLayout->addLayout( layout10, 0, 0 );
	languageChange();
	resize( QSize(100, 107).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
	QObject::connect(kpbtnClose, SIGNAL(clicked()), SLOT(slotCloseClicked()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaFileStatusUIBase::~EvaFileStatusUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

void EvaFileStatusUIBase::setMimePixmap(const QPixmap &pixmap)
{
	lblIcon->setPixmap(pixmap);
}

void EvaFileStatusUIBase::setFileName(const QString &fileName)
{
	lblFileName->setText(fileName);
	QToolTip::add(lblFileName, fileName);
}

void EvaFileStatusUIBase::updateStatus(const unsigned int size, const unsigned int bytes, 
					const unsigned int time)
{
	kpgFileStatus->setProgress(bytes);
	QString sizetip = i18n("Size: ") + strFileSize(bytes, 1) + "/" + strFileSize(size, 1);
	lblReceived->setText( sizetip );
	QToolTip::remove(lblReceived);
	QToolTip::add(lblReceived, sizetip );
	float speed = time?(((float)bytes - (float) m_StartPosition)/((float)time)):0;
	QString speedtip = i18n("Speed: ") + strFileSize(speed, 2) + "/S";
	lblSpeed->setText( speedtip );
	QToolTip::remove(lblSpeed);
	QToolTip::add(lblSpeed, speedtip);
	if(bytes == size ) {
		m_IsFinished = true;
		QTimer::singleShot(100, this, SLOT(slotCloseClicked()));
	}
}

const QString EvaFileStatusUIBase::strFileSize(const float size, int precise)
{
	QString fileSize = (size<0x400)?(QString::number(size) + "B"):
			((size<0x100000)?(QString::number(size/0x400, 'f', precise) + "KB") :
			((size<0x40000000)?(QString::number(size/0x100000, 'f', precise) + "MB"):
			(QString::number(size/0x40000000, 'f', precise) + "GB") ) );
	return fileSize;
}

void EvaFileStatusUIBase::languageChange()
{
	setCaption( i18n( "FileStatus" ) );
	lblIcon->setText( i18n( "Icon" ) );
	lblFileName->setText( i18n( "FileName" ) );
	kpbtnClose->setText( i18n( "x" ) );
	lblReceived->setText( i18n( "Size:" ) );
	lblSpeed->setText( i18n( "Speed:" ) );
}

void EvaFileStatusUIBase::slotCloseClicked()
{
	emit closeRequest(m_Session, this);
}
