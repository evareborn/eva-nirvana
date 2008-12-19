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
#include "evasession.h"
 
#include <q3listview.h>
#include <qimage.h>
#include <qtextcodec.h>
#include <q3header.h>
//Added by qt3to4:
#include <QPixmap>
#include <string>

#include "../evaguimain.h"
#include "evauser.h"
#include "evafriendlist.h"
#include "evafriend.h"
#include "evaresource.h"
#include "evaqunlist.h"
#include "evaqtutil.h"

EvaQunMemberPicker::EvaQunMemberPicker(QWidget * parent, const char * name, Qt::WFlags fl, Qun *qun )
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
	QObject::connect(lvBuddyList, SIGNAL(clicked(Q3ListViewItem *)), SLOT(slotClicked(Q3ListViewItem *)));
	//lvBuddyList->setTreeStepSize( 0 );   
}

void EvaQunMemberPicker::updateBuddyListView()
{
	//if(!mQun) return;
	lvBuddyList->clear();
	
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	std::list<std::string> names = EvaMain::session->getUser()->getGroupNames();
	std::list<std::string>::iterator groupIter;
	int i=0;
	for(groupIter = names.begin(); groupIter!= names.end(); ++groupIter){
		QString g = codec->toUnicode(groupIter->c_str());
		Q3CheckListItem *item = new Q3CheckListItem(lvBuddyList, g, Q3CheckListItem::CheckBox);
		item->setState(Q3CheckListItem::Off);
		groups[i++] = item;
	}
	
	//QunInfo info = mQun->getDetails();
	
	std::map<unsigned int, QQFriend>::iterator iter;
	std::map<unsigned int, QQFriend> list = (EvaMain::session->getUser()->getFriendList()).getAllFriendsMap();
	for(iter = list.begin(); iter != list.end(); ++iter){
	
		int id = iter->second.getQQ();
		QString nick = EvaTextFilter::filter(codec->toUnicode(iter->second.getNick().c_str()));
		
		int groupIndex = iter->second.getGroupIndex();
		Q3CheckListItem *group = groups[groupIndex];
		if(!group) continue;
		
		short faceID = (iter->second.getFace())/3 + 1;
		if(faceID<1) faceID = 1;
		Q3CheckListItem *item = new Q3CheckListItem(group, nick + "(" + QString::number(id) + ")", Q3CheckListItem::CheckBox);

		QPixmap *pic = EvaMain::images->getFace(faceID, true);
		if(pic) {
			QImage img(pic->convertToImage().smoothScale(16, 16));
			item->setPixmap(0, QPixmap(img));
		}else{
			fprintf(stderr, "EvaQunMemberPicker::updateBuddyListView (id:%d, faceID:%d, %s) --  NULL QPixmap pointer, ignored!\n", id, faceID, nick.ascii());
		}
		if(mQun && mQun->hasMember(id))
			item->setState(Q3CheckListItem::On);
		else
			item->setState(Q3CheckListItem::Off);
			
		buddyList[id] = item;
	}
}

void EvaQunMemberPicker::slotClicked(Q3ListViewItem *item)
{
	if(!item) return;
	Q3CheckListItem *chkItem = dynamic_cast<Q3CheckListItem *>(item);
	if(!chkItem) return;
	unsigned int id;
	QString txt;
	QString strID;
	bool ok;
	QString nick;
	QPixmap face;
	
	Q3CheckListItem *child = dynamic_cast<Q3CheckListItem *>(chkItem->firstChild());
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
		child = dynamic_cast<Q3CheckListItem *>(child->nextSibling());
	}
}

void EvaQunMemberPicker::slotSetMemberOff(const unsigned int id)
{
	std::map<int, Q3CheckListItem *>::iterator iter;
	iter = buddyList.find(id);
	if(iter == buddyList.end()) return;
	iter->second->setOn(false);
}



