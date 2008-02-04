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
#include "evasystemtray.h"

#include "evaresource.h"
#include "evauhmanager.h"
#include "evamain.h"
#include "evahtmlparser.h"
#include <kpopupmenu.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qevent.h>
#include <qtimer.h>
#include <qtooltip.h>
#include <qcursor.h>
#include <qapplication.h>
#include <kaction.h>
#include <klocale.h>

 
EvaSystemTray::EvaSystemTray(QWidget* parent, const char* name) 
	: KSystemTray(parent, name), 
	images(NULL), sysMenu(NULL), statusMenu(NULL), isBlinkOn(false)
{
	blinkTimer = new QTimer(this);
	QObject::connect(blinkTimer, SIGNAL(timeout()), SLOT(slotTimeout()));

	clickTimer = new QTimer(this);
	QObject::connect(clickTimer, SIGNAL(timeout()), SLOT(slotClickTimeout()));
}

EvaSystemTray::~EvaSystemTray()
{
	if(blinkTimer->isActive())
		blinkTimer->stop();
	delete blinkTimer;
	if(clickTimer->isActive())
		clickTimer->stop();
	delete clickTimer;
}

void EvaSystemTray::setImagesResource(EvaImageResource *res) 
{ 
	if(!res) return;
	images = res;
	setIconTo(images->getIcon("OFFLINE"));
}

void EvaSystemTray::setupMenus(KPopupMenu *sys, KPopupMenu *status)
{
	sysMenu = sys;
	statusMenu = status;
}

void EvaSystemTray::reset()
{
	if(blinkTimer->isActive())
		blinkTimer->stop();
	if(clickTimer->isActive())
		clickTimer->stop();
	messageStack.clear();
	iconStack.clear();
	setOffline();
}

void EvaSystemTray::changeToolTip(const unsigned int id, const QString &nick, const short face)
{
	if(!images) return;
	QString picName = "";
	if(EvaMain::uhManager){
		picName = EvaMain::uhManager->getFileName(id);
	}
	if(picName.isEmpty()){
		picName = images->getFacePath() +
			"/" + QString::number(images->getFaceFileIndex(face)) + ".png";
	}
	EvaHtmlParser parser;
	parser.setAbsImagePath(images->getSmileyPath());
	QString htmlNick = nick;
	parser.convertToHtml( htmlNick,  false, true);
	QString tip = "<qt><table><tr><td><img src =\"" + picName + "\"></td><td><nobr><font color=blue><b>EVA</b> </font> - " +
			i18n("IM Client")+ "</nobr><br>" + htmlNick +
			"<br>" + QString::number(id) + "</td></tr></table></qt>";
	QToolTip::remove(this);
	QToolTip::add(this, tip);
}

void EvaSystemTray::setOnline()
{
	if(!images) return;
	statusPix = images->getIcon("ONLINE");
	setIconTo(statusPix);
}

void EvaSystemTray::setOffline()
{
	if(!images) return;
	statusPix = images->getIcon("OFFLINE");
	if(blinkTimer->isActive())
		blinkTimer->stop();
	setIconTo(statusPix);
}

void EvaSystemTray::setLeave()
{
	if(!images) return;
	statusPix = images->getIcon("LEAVE");
	setIconTo(statusPix);
}

void EvaSystemTray::setInvisible()
{
	if(!images) return;
	statusPix = images->getIcon("INVISIBLE");
	setIconTo(statusPix);
}

void EvaSystemTray::setLoginWaiting()
{
	if(!images) return;
	if(blinkTimer->isActive())
		blinkTimer->stop();
	QLabel::setMovie(*(images->getLoginMovie()));
}

void EvaSystemTray::newTxtMessage(const unsigned int id, short face)
{
	if(messageStack.findIndex(id) != -1) return; // if exists, do nothing
	messageStack.push(id);
	iconStack.push(face);
	if(!blinkTimer->isActive()){
		//statusPix =  *QLabel::pixmap();
		blinkTimer->start(300, false);
	}
}

void EvaSystemTray::gotTxtMessage(const unsigned int id)
{
	int index = messageStack.findIndex(id);
	if( index == -1) return; // if not exist, just return
	messageStack.remove(id);
	int face = iconStack[index];
	iconStack.remove(face);
	if(messageStack.isEmpty() && blinkTimer->isActive()){
		blinkTimer->stop();
		setIconTo(statusPix);
	}
}

void EvaSystemTray::newSysMessage()
{
	messageStack.push(-1);
	iconStack.push(-1);
	if(!blinkTimer->isActive()){
		//statusPix =  *QLabel::pixmap();
		blinkTimer->start(300, false);
	}
}

void EvaSystemTray::gotSysMessage()
{
	int index = messageStack.findIndex(-1);
	if( index == -1) return; // if not exist, just return
	if(iconStack[index] != -1) return; // if not a sys msg icon, return
	
	QValueStack<int>::iterator iter = messageStack.find(-1);
	messageStack.remove(iter);
	iter = iconStack.find(-1);
	iconStack.remove(iter);
	
	if(messageStack.isEmpty() && blinkTimer->isActive()){
		blinkTimer->stop();
		setIconTo(statusPix);
	}	
}

void EvaSystemTray::mousePressEvent(QMouseEvent *me)
{
	if(me->button() == MidButton){
		mouseDoubleClickEvent(me);
		me->accept();
		return;
	}
	
	if(me->button() == RightButton && sysMenu){
		sysMenu->popup(me->globalPos());
		if(clickTimer->isActive()) clickTimer->stop();
		me->accept();
		return;
	}
		
	if(me->button() == LeftButton && !clickTimer->isActive())
		clickTimer->start(QApplication::doubleClickInterval(), false);
	
	me->accept();
}

void EvaSystemTray::mouseDoubleClickEvent( QMouseEvent *me )
{
	if(clickTimer->isActive())
		clickTimer->stop();
	if(!messageStack.isEmpty() && !iconStack.isEmpty() && (me->button() == LeftButton || me->button() == MidButton)){
		int id = messageStack.pop();
		int face = iconStack.pop();
		if(id == -1 && face == -1){
			me->accept();
			emit requestSystemMessage();
			return;
		}
		
		if(id != -1 && face == -2){
			emit requestQunChat(id);
		}
		
		if(id != -1 && face != -1 && face != -2){
			emit requestChat(id);
		}
		me->accept();
		return;
	}else{
		if(messageStack.isEmpty() && (me->button() == LeftButton || me->button() == MidButton) && statusMenu){
			minMaxAction();
			me->accept();
			return;
		}
	}
}

void EvaSystemTray::slotClickTimeout( )
{
	if(clickTimer->isActive()){
		statusMenu->popup(QCursor::pos());
	}
	clickTimer->stop();
}

void EvaSystemTray::minMaxAction( )
{
	KAction* action = KSystemTray::actionCollection()->action("minimizeRestore");
	if(action)
		action->activate();
}

void EvaSystemTray::slotTimeout()
{
	if(messageStack.isEmpty() || iconStack.isEmpty()){
		if(blinkTimer->isActive())
			blinkTimer->stop();
		messageStack.clear();
		iconStack.clear();
		setIconTo(statusPix);
		return;
	}
	if(isBlinkOn){
		QLabel::setPixmap(QPixmap());
		isBlinkOn = false;
	}else{
		int id = messageStack.top();
		int face = iconStack.top();
		
		if(id == -1 && face == -1){
			if(images)
				setIconTo(images->getIcon("SYSTEM_MSG"));
		}
		
		if(id != -1 && face == -2){
			if(images)
				setIconTo(images->getIcon("QUN"));
		}
		
		if(id != -1 && face != -1 && face != -2){
			if(images){
				QPixmap *pic = images->getUserHeadPixmap(id);
				if(!pic)
					pic = images->getFace(images->getFaceFileIndex(face));
				setIconTo(pic);
			}
		}
		isBlinkOn = true;
	}
}

void EvaSystemTray::setIconTo( QPixmap * pix )
{
	if(!pix) return;
	bool timerRunning = false;
	if(blinkTimer->isActive()){
		blinkTimer->stop();
		timerRunning = true;
	}
	QMovie *mov = movie();
	if(mov) mov->pause();
	
	QImage img(pix->convertToImage().smoothScale(16, 16));
	QLabel::setPixmap(QPixmap(img));
	if(timerRunning)
		blinkTimer->start(300, false);
}

int EvaSystemTray::getSenderID( )
{
	if(messageStack.isEmpty()) return -1000;
	if(iconStack.isEmpty()) return -1001;
	
	int id = messageStack.pop();
	int face = iconStack.pop();
	if(id == -1 && face == -1){
		//emit requestSystemMessage();
		return -1; // system message
	}
	
	if(id != -1 && face == -2){
		return id; // qun message
	}
	
	if(id != -1 && face != -1 && face != -2){
		return id; // normal buddy message
	}
	return -1000;
}

