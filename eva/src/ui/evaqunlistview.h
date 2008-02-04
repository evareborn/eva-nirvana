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

#ifndef EVAQUNLISTVIEW_H
#define EVAQUNLISTVIEW_H

#include <qlistview.h>
#include <list>

class QPixmap;
//class EvaToolTip;
class QCustomEvent;
class QPopupMenu;


class EvaQunBuddyItem :  public QObject, public QListViewItem
{
	Q_OBJECT
public:
	EvaQunBuddyItem( QListView *parent, const QString &name, const unsigned int id, const QPixmap *pic, const QPixmap *offPic);
	
	void update(const unsigned int id, const QString &name, const QPixmap *pic, const QPixmap *offPic, const bool isCreator = false);
	void setCreator(bool isCreator);
	void setAdmin(bool isAdmin);
	void setShareHolder();
	
	void setOnline(const bool online);
	const bool isOnline() const { return mIsOnline; }
	const unsigned int &getQQ() const { return qqNum; }
	const QString &getNick() const { return nick; }
	
	virtual QString key( int col, bool ascending) const;
	QString tip(); 
protected:
	virtual void customEvent( QCustomEvent *e);
private:
	unsigned int qqNum;
	QString nick;
	const QPixmap *facePic;
	const QPixmap *faceOffPic;
	bool mIsOnline;
	bool mIsCreator;
	bool mIsAdmin;
};

class EvaQunListView : public QListView 
{
	Q_OBJECT
public:
	EvaQunListView( QWidget *parent=0, const char *name =0, WFlags f= 0);
	~EvaQunListView();
	
	EvaQunBuddyItem * addQunBuddy(const QString &nick, const unsigned int id, const QPixmap *pic, const QPixmap *offpic);

	void updateOnlineMembers(const std::list<unsigned int> &onlines);
	
	void setCreator(int);
signals:
	void requestBuddyInfo(const unsigned int);
	void requestQunCard(const unsigned int);
	void requestQunMembers();
	void requestChat(const unsigned int);
private:
	//EvaToolTip *mToolTip;
	QPopupMenu *popupMenu;
private slots:
	void slotContextMenu(QListViewItem *, const QPoint & , int);
	void slotDetails();
	void slotDoRefreshMembers();
	void slotQunCard();
	void slotBuddyDoubleClick( QListViewItem *item, const QPoint &, int );
};


#endif 
