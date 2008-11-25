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
 
#ifndef EVASYSTEMTRAY_H
#define EVASYSTEMTRAY_H

//#include <qsystemtrayicon.h>
#include <q3valuestack.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QPixmap>
#include <Q3PopupMenu>

class EvaImageResource;
class Q3PopupMenu;
class QMouseEvent;
class QTimer;
class QPixmap;

class EvaSystemTray// : public QSystemTray
{
//X 	Q_OBJECT
//X public:
//X 	EvaSystemTray(QWidget* parent = 0, const char* name = 0);
//X 	EvaSystemTray():QSystemTray() {}
//X 	~EvaSystemTray();
//X 	
//X 	void setImagesResource(EvaImageResource *res = 0);
//X 	void setupMenus(QPopupMenu *sys, QPopupMenu *status);
//X 	void reset();
//X public slots:	
//X 	void changeToolTip(const unsigned int id, const QString &nick, const short face);
//X 	void setOnline();
//X 	void setOffline();
//X 	void setLeave();
//X 	void setInvisible();
//X 	void setLoginWaiting();
//X 	
//X 	void newTxtMessage(const unsigned int id, short face = 0);
//X 	void gotTxtMessage(const unsigned int id);
//X 	void newSysMessage();
//X 	void gotSysMessage();
//X 	int getSenderID();
//X signals:
//X 	void requestChat(const unsigned int id);
//X 	void requestQunChat(const unsigned int id);
//X 	void requestSystemMessage();
//X protected:
//X 	virtual void mousePressEvent(QMouseEvent *e);
//X 	virtual void mouseDoubleClickEvent( QMouseEvent *me );
//X private:
//X 	EvaImageResource *images;
//X 	QPopupMenu *sysMenu;
//X 	QPopupMenu *statusMenu;
//X 	QTimer *blinkTimer;
//X 	QTimer *clickTimer;
//X 	QPixmap *statusPix;
//X 	bool isBlinkOn;
//X 	
//X 	QValueStack<int> messageStack; // store user qq id or -1 for system message
//X 	QValueStack<int> iconStack; // store the corresponding face number or -1 for system message icon
//X 	
//X 	void setIconTo(QPixmap *pix);
//X private slots:
//X 	void slotTimeout();
//X 	void slotClickTimeout();
//X 	void minMaxAction();
};

#endif

