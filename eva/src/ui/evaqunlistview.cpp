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
#include "evaqunlistview.h"
#include "evamain.h"
#include "evaresource.h"
#include "evaqtutil.h"
//#include "evalistview.h"
#include "evahtmlparser.h"
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qsize.h>
#include <klocale.h>



EvaQunListView::EvaQunListView( QWidget * parent, const char * name, WFlags f )
	: QListView(parent, name, f)
{
	//mToolTip = new EvaToolTip(this);
	
	popupMenu = new QPopupMenu(0, "QunPopup");
	popupMenu->insertItem(QIconSet(*(EvaMain::images->getIcon("QUN_CARD"))), i18n( "Qun Card" ), this, SLOT( slotQunCard()));
	popupMenu->insertItem(QIconSet(*(EvaMain::images->getIcon("DETAILS"))), i18n( "Details"), this, SLOT(slotDetails()), -1, 1);
	popupMenu->insertSeparator(-1);
	popupMenu->insertItem(QIconSet(*(EvaMain::images->getIcon("REFRESH_BUDDIES"))), i18n("Refresh Qun Members"), this,SLOT(slotDoRefreshMembers()));
	
	QObject::connect(this, SIGNAL(contextMenuRequested(QListViewItem *, const QPoint & , int)), this, 
		SLOT(slotContextMenu(QListViewItem *, const QPoint & , int)));
		
	QObject::connect(this, SIGNAL(doubleClicked(QListViewItem *, const QPoint & , int)),
									 SLOT(slotBuddyDoubleClick(QListViewItem *, const QPoint & , int)));
}

EvaQunListView::~EvaQunListView()
{
	delete popupMenu;
}

EvaQunBuddyItem * EvaQunListView::addQunBuddy(const QString &nick, const unsigned int id, const QPixmap *pic, const QPixmap *offpic)
{
	return new EvaQunBuddyItem(this, nick, id, pic, offpic);
}

void EvaQunListView::updateOnlineMembers(const std::list<unsigned int> &onlines)
{
	std::list<unsigned int> l = onlines;
	EvaQunBuddyItem *buddy = dynamic_cast<EvaQunBuddyItem *>(firstChild());
	std::list<unsigned int>::iterator iter;
	bool found = false;
	while(buddy){
		for(iter=l.begin(); iter!=l.end(); ++iter){
			if( buddy->getQQ() == *iter ) {
				if(!buddy->isOnline())
					buddy->setOnline(true);
				found = true;
			} 
		}
		if( (!found) && buddy->isOnline()){
			buddy->setOnline(false);
		}
		found = false;
		buddy = dynamic_cast<EvaQunBuddyItem *>(buddy->nextSibling());
	}
	sort();
}

void EvaQunListView::slotContextMenu(QListViewItem *item, const QPoint &p , int col)
{
	if(col!=1) return;
	EvaQunBuddyItem *q = dynamic_cast<EvaQunBuddyItem *>(item);
	if(q)
		q->setSelected(true);
	
	popupMenu->popup(p);
}

void EvaQunListView::slotDetails()
{
	EvaQunBuddyItem *item = dynamic_cast<EvaQunBuddyItem *>(selectedItem());
	if(!item) return;
	emit requestBuddyInfo(item->getQQ());
}

void EvaQunListView::slotDoRefreshMembers()
{
	EvaQunBuddyItem *item = dynamic_cast<EvaQunBuddyItem *>(selectedItem());
	if(!item) return;
	emit requestQunMembers();
}


void EvaQunListView::slotQunCard( )
{
	EvaQunBuddyItem *item = dynamic_cast<EvaQunBuddyItem *>(selectedItem());
	if(!item) return;
	emit requestQunCard(item->getQQ());
}

void EvaQunListView::slotBuddyDoubleClick( QListViewItem * item, const QPoint &, int )
{
	EvaQunBuddyItem *b = dynamic_cast<EvaQunBuddyItem *>(item);
	if(b){
		emit requestChat(b->getQQ());
	}
}

/************************************************************************************************************************************************/


EvaQunBuddyItem::EvaQunBuddyItem( QListView *parent, const QString &name, const unsigned int id, const QPixmap *pic, const QPixmap *offPic)
	: QListViewItem(parent,"", name)
{	
	qqNum = id;
	nick = name;
	QString display = nick + "(" + QString::number(qqNum) + ")";
	setText(1, display);
	
	facePic = pic;
	faceOffPic = offPic;
	mIsOnline = false;
	mIsCreator = false;
	mIsAdmin = false;
	EvaMain::helper->setCategory(EvaHelper::ScaleImage,this);
	EvaMain::helper->setScaleArgs(*faceOffPic, QSize(16, 16));
	EvaMain::helper->run();
}

void EvaQunBuddyItem::update(const unsigned int id, const QString &name, const QPixmap *pic, const QPixmap *offPic, const bool isCreator)
{
	nick = name;	
	facePic = pic;
	faceOffPic = offPic;
	QString display = name + "(" + QString::number(id) + ")";
	setText( 1, display);
	if(isCreator) setCreator(true);
	if(!pic) return;
}

void EvaQunBuddyItem::setOnline(const bool online)
{
	mIsOnline = online;
	EvaMain::helper->setCategory(EvaHelper::ScaleImage,this);
	if(mIsOnline){
		EvaMain::helper->setScaleArgs(*facePic, QSize(16, 16));
	}else
		EvaMain::helper->setScaleArgs(*faceOffPic, QSize(16, 16));
	EvaMain::helper->run();
		
}

void EvaQunBuddyItem::setCreator(bool isCreator)
{
	mIsCreator = isCreator;
	if(mIsCreator){
		QPixmap *p = EvaMain::images->getIcon("QUN_CREATOR");
		if(p)
			setPixmap(0, *p);
	}
}

void EvaQunBuddyItem::setAdmin(bool isAdmin)
{
	mIsAdmin = isAdmin;
	if(mIsAdmin){
		QPixmap *p = EvaMain::images->getIcon("QUN_ADMIN");
		if(p)
			setPixmap(0, *p);
	}	
}

void EvaQunBuddyItem::setShareHolder()
{
	QPixmap *p = EvaMain::images->getIcon("QUN_SHAREHOLDER");
	if(p)
		setPixmap(0, *p);
}

QString EvaQunBuddyItem::key( int, bool) const
{
	QString prefix;
	if(mIsOnline)
		prefix ="0";
	else if( mIsCreator)
			prefix ="1";
		else if(mIsAdmin)
				prefix = "2";
			else
				prefix = "3";

	return prefix + nick;
}

QString EvaQunBuddyItem::tip()
{
	QString htmlName = nick;
	EvaHtmlParser parser;
	parser.setAbsImagePath(EvaMain::images->getSmileyPath());
	parser.convertToHtml(htmlName, false, true);
	QString tip = "<table width = 150><tr><td><b><font color = blue>" + 
			i18n("Nickname:") +" </font></b>"+ htmlName +"<br><b><font color = blue>"+
			i18n("QQ") +": </font></b>"+ QString::number(qqNum) + "</td></tr></table>";
	return tip;
}

void EvaQunBuddyItem::customEvent( QCustomEvent *e)
{
	if(e->type() == EvaScaleImageEvent){
		EvaScaleEvent *se = (EvaScaleEvent *)e;
		QPixmap p = se->pixmap();
		setPixmap(1, p);
	}
}

