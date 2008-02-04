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
 
#include "evaqunmemberpicker.h"
 
#include <qlistview.h>
#include <qimage.h>
#include <qtextcodec.h>
#include <qheader.h>
#include <string>

#include "../evamain.h"
#include "evauser.h"
#include "evafriendlist.h"
#include "evafriend.h"
#include "evaresource.h"
#include "evaqunlist.h"
#include "evaqtutil.h"

EvaQunMemberPicker::EvaQunMemberPicker(QWidget * parent, const char * name, WFlags fl, Qun *qun )
	: EvaQunMemberPickerUI( parent, name, fl )
{
	mQun = qun;
	initListView();
	updateBuddyListView();
}

void EvaQunMemberPicker::initListView()
{
	lvBuddyList->header()->hide();
	lvBuddyList->setRootIsDecorated( true);
	QObject::connect(lvBuddyList, SIGNAL(clicked(QListViewItem *)), SLOT(slotClicked(QListViewItem *)));
	//lvBuddyList->setTreeStepSize( 0 );   
}

void EvaQunMemberPicker::updateBuddyListView()
{
	//if(!mQun) return;
	lvBuddyList->clear();
	
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	std::list<std::string> names = EvaMain::user->getGroupNames();
	std::list<std::string>::iterator groupIter;
	int i=0;
	for(groupIter = names.begin(); groupIter!= names.end(); ++groupIter){
		QString g = codec->toUnicode(groupIter->c_str());
		QCheckListItem *item = new QCheckListItem(lvBuddyList, g, QCheckListItem::CheckBox);
		item->setState(QCheckListItem::Off);
		groups[i++] = item;
	}
	
	//QunInfo info = mQun->getDetails();
	
	std::map<unsigned int, QQFriend>::iterator iter;
	std::map<unsigned int, QQFriend> list = (EvaMain::user->getFriendList()).getAllFriendsMap();
	for(iter = list.begin(); iter != list.end(); ++iter){
	
		int id = iter->second.getQQ();
		QString nick = EvaTextFilter::filter(codec->toUnicode(iter->second.getNick().c_str()));
		
		int groupIndex = iter->second.getGroupIndex();
		QCheckListItem *group = groups[groupIndex];
		if(!group) continue;
		
		short faceID = (iter->second.getFace())/3 + 1;
		if(faceID<1) faceID = 1;
		QCheckListItem *item = new QCheckListItem(group, nick + "(" + QString::number(id) + ")", QCheckListItem::CheckBox);

		QPixmap *pic = EvaMain::images->getFace(faceID, true);
		if(pic) {
			QImage img(pic->convertToImage().smoothScale(16, 16));
			item->setPixmap(0, QPixmap(img));
		}else{
			fprintf(stderr, "EvaQunMemberPicker::updateBuddyListView (id:%d, faceID:%d, %s) --  NULL QPixmap pointer, ignored!\n", id, faceID, nick.ascii());
		}
		if(mQun && mQun->hasMember(id))
			item->setState(QCheckListItem::On);
		else
			item->setState(QCheckListItem::Off);
			
		buddyList[id] = item;
	}
}

void EvaQunMemberPicker::slotClicked(QListViewItem *item)
{
	if(!item) return;
	QCheckListItem *chkItem = dynamic_cast<QCheckListItem *>(item);
	if(!chkItem) return;
	unsigned int id;
	QString txt;
	QString strID;
	bool ok;
	QString nick;
	QPixmap face;
	
	QCheckListItem *child = dynamic_cast<QCheckListItem *>(chkItem->firstChild());
	if(!child){
		txt = chkItem->text(0);
		int index = txt.findRev("(");
		nick = txt.left( index );
		strID = txt.mid(index+1, txt.length() - index - 1 - 1);
		id = strID.toUInt(&ok);
		if(!ok) return;
		
		face = *(chkItem->pixmap(0));
		
		emit memberClicked(id, chkItem->isOn());
	}
	while(child){
		child->setOn(chkItem->isOn());
		
		txt = child->text(0);
		int index = txt.findRev("(");
		nick = txt.left( index );
		strID = txt.mid(index+1, txt.length() - index - 1 - 1);
		id = strID.toUInt(&ok);
		if(!ok) return;
		
		face = *(child->pixmap(0));
		
		emit memberClicked(id, child->isOn());
		child = dynamic_cast<QCheckListItem *>(child->nextSibling());
	}
}

void EvaQunMemberPicker::slotSetMemberOff(const unsigned int id)
{
	std::map<int, QCheckListItem *>::iterator iter;
	iter = buddyList.find(id);
	if(iter == buddyList.end()) return;
	iter->second->setOn(false);
}



