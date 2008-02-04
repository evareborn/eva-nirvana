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

#include "evasysbroadcastwindow.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qstringlist.h>

#include <qdesktopwidget.h>

#include <kapp.h>
#include <kurllabel.h>
#include <kapplication.h>
#include <klocale.h>

EvaSysBroadcastWindow::EvaSysBroadcastWindow( )
	: EvaSysBroadcastUIBase()
{
	QObject::connect(btnDetails, SIGNAL(clicked()), SLOT(slotOpenURL()));
	QObject::connect(btnClose, SIGNAL(clicked()), SLOT(close()));
	QObject::connect(kurllblUrl, SIGNAL(leftClickedURL()), SLOT(slotOpenURL()));

	//adjustSize();
	//resize( QSize(315, 157).expandedTo(minimumSizeHint()) );
}

void EvaSysBroadcastWindow::slotOpenURL()
{
	QStringList args;
	args<<"exec"<< m_URL;
	kapp->kdeinitExec("kfmclient",args);

	close();
}

void EvaSysBroadcastWindow::closeEvent( QCloseEvent *)
{
	deleteLater();
}

void EvaSysBroadcastWindow::setMessage( const QString & msg )
{
	int index = msg.findRev( '\x0a');
	m_Contents = msg.left( index );
	m_URL = msg.right( msg.length() - index - 1 - 2);

	lblContents->setText(m_Contents);
	kurllblUrl->setText( m_URL );
	kurllblUrl->setProperty( "url", m_URL );
	adjustSize();

	moveToRightBottom( );
}

void EvaSysBroadcastWindow::setNews( const QString & title, const QString & brief, const QString & url )
{
	setCaption(i18n("System News"));
	lblTitle->setText( title);
	lblContents->setText(brief);
	m_URL = url;

	/// if we show the url link, the layout will be a bit wide,
	/// so, just hide it. user can still click the "more dettails"
	/// button anyway
	kurllblUrl->setHidden( true);
	//kurllblUrl->setText( m_URL );
	//kurllblUrl->setProperty( "url", m_URL );
	adjustSize();

	moveToRightBottom( );
}

void EvaSysBroadcastWindow::moveToRightBottom( )
{
	QDesktopWidget *desktop = KApplication::desktop();
	QRect scr = desktop->screenGeometry();
	setGeometry(scr.right()- width(), scr.bottom() - height(), width(), height());	
}

