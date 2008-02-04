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

#include "evafilepanel.h"
#include "evafilestatusuibase.h"
#include <qlabel.h>
#include <kpushbutton.h>
#include <kprogress.h>
#include <kmimetype.h>

EvaFilePanel::EvaFilePanel(QWidget *parent, const char *name, WFlags f)
	: KTabWidget(parent, name, f)
{
	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0,
				 sizePolicy().hasHeightForWidth() ) );
	setMaximumSize( QSize( 112, 200 ) );
	setTabShape(QTabWidget::Triangular);
	setTabPosition(QTabWidget::Bottom);
}

void EvaFilePanel::updateStatus(const unsigned int session, const KURL &url, const unsigned int size,
				const unsigned int bytes, const unsigned int time)
{
	EvaFileStatusUIBase *item = NULL;
	for(int i=0; i<count(); i++){
		item = dynamic_cast<EvaFileStatusUIBase *>(page(i));
		if(!item) continue;
		if(item->getSession() == session){
			item->updateStatus(size, bytes, time);
			break;
		} else
			item = NULL;
	}
	if(!item){
		item = newTab(session, size, url);
		item->setStartPosition( bytes );
		item->updateStatus(size, bytes, time);
		showPage(item);
	}
}

EvaFileStatusUIBase * EvaFilePanel::newTab(const unsigned int session, const unsigned int size,
					const KURL &url)
{
	EvaFileStatusUIBase *item = new EvaFileStatusUIBase(session, size, this);
	item->setMimePixmap(KMimeType::pixmapForURL(url, 0, KIcon::Desktop, KIcon::SizeMedium));
	item->setFileName(url.fileName(false)); // should be no trailing slash
	addTab(item, QString::number(count()+1));
	QObject::connect(item, SIGNAL(closeRequest(const unsigned int, QWidget *)), 
				SLOT(slotCloseTab(const unsigned int, QWidget *)));
	return item;
}

void EvaFilePanel::slotCloseTab( const unsigned int session, QWidget * widget)
{
	removePage(widget);
	EvaFileStatusUIBase *item = dynamic_cast<EvaFileStatusUIBase *>(widget);
	if(!item) return;
	if(item->isFinished()){
		delete widget;
		return;
	}
	delete widget;

	for(int i=0; i<count(); i++){
		item = dynamic_cast<EvaFileStatusUIBase *>(page(i));
		if(!item) continue;
		setTabLabel(item, QString::number( i+1));
	}
	emit closeSession(session);
}

void EvaFilePanel::closeTab( const unsigned int session )
{
	EvaFileStatusUIBase *item = NULL;
	for(int i=0; i<count(); i++){
		item = dynamic_cast<EvaFileStatusUIBase *>(page(i));
		if(!item) continue;
		if(item->getSession() == session){
			removePage(item);
			delete item;
			break;
		} 
	}
}

