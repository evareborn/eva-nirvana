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

//X  
//X EvaSystemTray::EvaSystemTray(QWidget* parent, const char* name) 
//X 	: KSystemTray(parent, name), 
//X 	images(NULL), sysMenu(NULL), statusMenu(NULL), isBlinkOn(false)
//X {
//X 	blinkTimer = new QTimer(this);
//X 	QObject::connect(blinkTimer, SIGNAL(timeout()), SLOT(slotTimeout()));
//X 
//X 	clickTimer = new QTimer(this);
//X 	QObject::connect(clickTimer, SIGNAL(timeout()), SLOT(slotClickTimeout()));
//X }
//X 
//X EvaSystemTray::~EvaSystemTray()
//X {
//X 	if(blinkTimer->isActive())
//X 		blinkTimer->stop();
//X 	delete blinkTimer;
//X 	if(clickTimer->isActive())
//X 		clickTimer->stop();
//X 	delete clickTimer;
//X }
//X 
//X void EvaSystemTray::setImagesResource(EvaImageResource *res) 
//X { 
//X 	if(!res) return;
//X 	images = res;
//X 	setIconTo(images->getIcon("OFFLINE"));
//X }
//X 
//X void EvaSystemTray::setupMenus(KPopupMenu *sys, KPopupMenu *status)
//X {
//X 	sysMenu = sys;
//X 	statusMenu = status;
//X }
//X 
//X void EvaSystemTray::reset()
//X {
//X 	if(blinkTimer->isActive())
//X 		blinkTimer->stop();
//X 	if(clickTimer->isActive())
//X 		clickTimer->stop();
//X 	messageStack.clear();
//X 	iconStack.clear();
//X 	setOffline();
//X }
//X 
//X void EvaSystemTray::changeToolTip(const unsigned int id, const QString &nick, const short face)
//X {
//X 	if(!images) return;
//X 	QString picName = "";
//X 	if(EvaMain::uhManager){
//X 		picName = EvaMain::uhManager->getFileName(id);
//X 	}
//X 	if(picName.isEmpty()){
//X 		picName = images->getFacePath() +
//X 			"/" + QString::number(images->getFaceFileIndex(face)) + ".png";
//X 	}
//X 	EvaHtmlParser parser;
//X 	parser.setAbsImagePath(images->getSmileyPath());
//X 	QString htmlNick = nick;
//X 	parser.convertToHtml( htmlNick,  false, true);
//X 	QString tip = "<qt><table><tr><td><img src =\"" + picName + "\"></td><td><nobr><font color=blue><b>EVA</b> </font> - " +
//X 			i18n("IM Client")+ "</nobr><br>" + htmlNick +
//X 			"<br>" + QString::number(id) + "</td></tr></table></qt>";
//X 	QToolTip::remove(this);
//X 	QToolTip::add(this, tip);
//X }
//X 
//X void EvaSystemTray::setOnline()
//X {
//X 	if(!images) return;
//X 	statusPix = images->getIcon("ONLINE");
//X 	setIconTo(statusPix);
//X }
//X 
//X void EvaSystemTray::setOffline()
//X {
//X 	if(!images) return;
//X 	statusPix = images->getIcon("OFFLINE");
//X 	if(blinkTimer->isActive())
//X 		blinkTimer->stop();
//X 	setIconTo(statusPix);
//X }
//X 
//X void EvaSystemTray::setLeave()
//X {
//X 	if(!images) return;
//X 	statusPix = images->getIcon("LEAVE");
//X 	setIconTo(statusPix);
//X }
//X 
//X void EvaSystemTray::setInvisible()
//X {
//X 	if(!images) return;
//X 	statusPix = images->getIcon("INVISIBLE");
//X 	setIconTo(statusPix);
//X }
//X 
//X void EvaSystemTray::setLoginWaiting()
//X {
//X 	if(!images) return;
//X 	if(blinkTimer->isActive())
//X 		blinkTimer->stop();
//X 	QLabel::setMovie(*(images->getLoginMovie()));
//X }
//X 
//X void EvaSystemTray::newTxtMessage(const unsigned int id, short face)
//X {
//X 	if(messageStack.findIndex(id) != -1) return; // if exists, do nothing
//X 	messageStack.push(id);
//X 	iconStack.push(face);
//X 	if(!blinkTimer->isActive()){
//X 		//statusPix =  *QLabel::pixmap();
//X 		blinkTimer->start(300, false);
//X 	}
//X }
//X 
//X void EvaSystemTray::gotTxtMessage(const unsigned int id)
//X {
//X 	int index = messageStack.findIndex(id);
//X 	if( index == -1) return; // if not exist, just return
//X 	messageStack.remove(id);
//X 	int face = iconStack[index];
//X 	iconStack.remove(face);
//X 	if(messageStack.isEmpty() && blinkTimer->isActive()){
//X 		blinkTimer->stop();
//X 		setIconTo(statusPix);
//X 	}
//X }
//X 
//X void EvaSystemTray::newSysMessage()
//X {
//X 	messageStack.push(-1);
//X 	iconStack.push(-1);
//X 	if(!blinkTimer->isActive()){
//X 		//statusPix =  *QLabel::pixmap();
//X 		blinkTimer->start(300, false);
//X 	}
//X }
//X 
//X void EvaSystemTray::gotSysMessage()
//X {
//X 	int index = messageStack.findIndex(-1);
//X 	if( index == -1) return; // if not exist, just return
//X 	if(iconStack[index] != -1) return; // if not a sys msg icon, return
//X 	
//X 	QValueStack<int>::iterator iter = messageStack.find(-1);
//X 	messageStack.remove(iter);
//X 	iter = iconStack.find(-1);
//X 	iconStack.remove(iter);
//X 	
//X 	if(messageStack.isEmpty() && blinkTimer->isActive()){
//X 		blinkTimer->stop();
//X 		setIconTo(statusPix);
//X 	}	
//X }
//X 
//X void EvaSystemTray::mousePressEvent(QMouseEvent *me)
//X {
//X 	if(me->button() == MidButton){
//X 		mouseDoubleClickEvent(me);
//X 		me->accept();
//X 		return;
//X 	}
//X 	
//X 	if(me->button() == RightButton && sysMenu){
//X 		sysMenu->popup(me->globalPos());
//X 		if(clickTimer->isActive()) clickTimer->stop();
//X 		me->accept();
//X 		return;
//X 	}
//X 		
//X 	if(me->button() == LeftButton && !clickTimer->isActive())
//X 		clickTimer->start(QApplication::doubleClickInterval(), false);
//X 	
//X 	me->accept();
//X }
//X 
//X void EvaSystemTray::mouseDoubleClickEvent( QMouseEvent *me )
//X {
//X 	if(clickTimer->isActive())
//X 		clickTimer->stop();
//X 	if(!messageStack.isEmpty() && !iconStack.isEmpty() && (me->button() == LeftButton || me->button() == MidButton)){
//X 		int id = messageStack.pop();
//X 		int face = iconStack.pop();
//X 		if(id == -1 && face == -1){
//X 			me->accept();
//X 			emit requestSystemMessage();
//X 			return;
//X 		}
//X 		
//X 		if(id != -1 && face == -2){
//X 			emit requestQunChat(id);
//X 		}
//X 		
//X 		if(id != -1 && face != -1 && face != -2){
//X 			emit requestChat(id);
//X 		}
//X 		me->accept();
//X 		return;
//X 	}else{
//X 		if(messageStack.isEmpty() && (me->button() == LeftButton || me->button() == MidButton) && statusMenu){
//X 			minMaxAction();
//X 			me->accept();
//X 			return;
//X 		}
//X 	}
//X }
//X 
//X void EvaSystemTray::slotClickTimeout( )
//X {
//X 	if(clickTimer->isActive()){
//X 		statusMenu->popup(QCursor::pos());
//X 	}
//X 	clickTimer->stop();
//X }
//X 
//X void EvaSystemTray::minMaxAction( )
//X {
//X 	KAction* action = KSystemTray::actionCollection()->action("minimizeRestore");
//X 	if(action)
//X 		action->activate();
//X }
//X 
//X void EvaSystemTray::slotTimeout()
//X {
//X 	if(messageStack.isEmpty() || iconStack.isEmpty()){
//X 		if(blinkTimer->isActive())
//X 			blinkTimer->stop();
//X 		messageStack.clear();
//X 		iconStack.clear();
//X 		setIconTo(statusPix);
//X 		return;
//X 	}
//X 	if(isBlinkOn){
//X 		QLabel::setPixmap(QPixmap());
//X 		isBlinkOn = false;
//X 	}else{
//X 		int id = messageStack.top();
//X 		int face = iconStack.top();
//X 		
//X 		if(id == -1 && face == -1){
//X 			if(images)
//X 				setIconTo(images->getIcon("SYSTEM_MSG"));
//X 		}
//X 		
//X 		if(id != -1 && face == -2){
//X 			if(images)
//X 				setIconTo(images->getIcon("QUN"));
//X 		}
//X 		
//X 		if(id != -1 && face != -1 && face != -2){
//X 			if(images){
//X 				QPixmap *pic = images->getUserHeadPixmap(id);
//X 				if(!pic)
//X 					pic = images->getFace(images->getFaceFileIndex(face));
//X 				setIconTo(pic);
//X 			}
//X 		}
//X 		isBlinkOn = true;
//X 	}
//X }
//X 
//X void EvaSystemTray::setIconTo( QPixmap * pix )
//X {
//X 	if(!pix) return;
//X 	bool timerRunning = false;
//X 	if(blinkTimer->isActive()){
//X 		blinkTimer->stop();
//X 		timerRunning = true;
//X 	}
//X 	QMovie *mov = movie();
//X 	if(mov) mov->pause();
//X 	
//X 	QImage img(pix->convertToImage().smoothScale(16, 16));
//X 	QLabel::setPixmap(QPixmap(img));
//X 	if(timerRunning)
//X 		blinkTimer->start(300, false);
//X }
//X 
//X int EvaSystemTray::getSenderID( )
//X {
//X 	if(messageStack.isEmpty()) return -1000;
//X 	if(iconStack.isEmpty()) return -1001;
//X 	
//X 	int id = messageStack.pop();
//X 	int face = iconStack.pop();
//X 	if(id == -1 && face == -1){
//X 		//emit requestSystemMessage();
//X 		return -1; // system message
//X 	}
//X 	
//X 	if(id != -1 && face == -2){
//X 		return id; // qun message
//X 	}
//X 	
//X 	if(id != -1 && face != -1 && face != -2){
//X 		return id; // normal buddy message
//X 	}
//X 	return -1000;
//X }
//X 
