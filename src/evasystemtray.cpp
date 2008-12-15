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
#include "evaguimain.h"
#include "evamainwindow.h"
#include "evahtmlparser.h"
#include "defines.h"
//X #include <kpopupmenu.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qevent.h>
#include <qtimer.h>
#include <qtooltip.h>
#include <qcursor.h>
#include <qapplication.h>
#include <QLabel>
#include <QMenu>
//X #include <kaction.h>
//X #include <klocale.h>

EvaSystemTray::EvaSystemTray(QWidget* parent, const char* /*name*/) 
	: QSystemTrayIcon(parent), 
	images(NULL), sysMenu(NULL), statusMenu(NULL), isBlinkOn(false)
{
	blinkTimer = new QTimer(this);
	QObject::connect(blinkTimer, SIGNAL(timeout()), SLOT(slotTimeout()));

	clickTimer = new QTimer(this);
	QObject::connect(clickTimer, SIGNAL(timeout()), SLOT(slotClickTimeout()));
        QObject::connect( this, SIGNAL( activated(QSystemTrayIcon::ActivationReason) ), SLOT( slotActivited(QSystemTrayIcon::ActivationReason) ) );
        QObject::connect( this, SIGNAL( messageClicked() ), SLOT( popMessageOrMainWin() ) );
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

void EvaSystemTray::setupMenus(QMenu *sys, QMenu *status)
{
	sysMenu = sys;
	statusMenu = status;
        setContextMenu( sysMenu );
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
 
void EvaSystemTray::showMessageTip( const int sender, const QString& nick, const QString& message ) {
    showMessage( QString( "%1 (%2) sayed:" ).arg( nick ).arg( sender ) , message );
}

void EvaSystemTray::changeToolTip(const unsigned int id, const QString &nick, const short face)
{
	if(!images) return;
	QString picName = "";
	if(EvaMain::getInstance()->getUHManager()){
		picName = EvaMain::getInstance()->getUHManager()->getFileName(id);
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
        setToolTip( tip );
//X 	QToolTip::remove(this);
//X 	QToolTip::add(this, tip);
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
//X 	QLabel::setMovie((images->getLoginMovie()));
//X 	setMovie(images->getLoginMovie());
}

void EvaSystemTray::newTxtMessage(const unsigned int id, const QString& nick, const QString& message)
{
	if(messageStack.contains( id)) return; // if exists, do nothing
        showMessageTip( id, nick, message );
	messageStack.push_back(id);
	iconStack.push_back(0);
	if(!blinkTimer->isActive()){
		//statusPix =  *QLabel::pixmap();
		blinkTimer->start(300, false);
	}
}

void EvaSystemTray::newQunMessage(const unsigned int id, const QString& nick, const QString& message)
{
	if(messageStack.contains( id )) return; // if exists, do nothing
        showMessageTip( id, nick, message );
	messageStack.push_back(id);
	iconStack.push_back(-2);
        printf( "id: %d, msgs: %d, icons: %d\n", id, messageStack.size(), iconStack.size() );
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

void EvaSystemTray::newSysMessage(const QString& message)
{
        showMessage( "system message:" , message );
	messageStack.push_back(-1);
	iconStack.push_back(-1);
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
	
	QList<int>::iterator iter = messageStack.find(-1);
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
	if(me->button() == Qt::MidButton){
		mouseDoubleClickEvent(me);
		me->accept();
		return;
	}
	
	if(me->button() == Qt::RightButton && sysMenu){
		sysMenu->popup(me->globalPos());
		if(clickTimer->isActive()) clickTimer->stop();
		me->accept();
		return;
	}
		
	if(me->button() == Qt::LeftButton && !clickTimer->isActive())
		clickTimer->start(QApplication::doubleClickInterval(), false);
	
	me->accept();
}
 
void EvaSystemTray::slotActivited( QSystemTrayIcon::ActivationReason reason ) {
    if ( reason == Trigger ) {
        popMessageOrMainWin();
    }
}
 
void EvaSystemTray::popMessageOrMainWin() {
        printf( "msgs: %d, icons: %d\n", messageStack.size(), iconStack.size() );
	if(!messageStack.isEmpty() && !iconStack.isEmpty() ){
		int id = messageStack.last();
                messageStack.pop_back( );
		int face = iconStack.last();
                iconStack.pop_back();
                printf( "id: %d, msgs: %d, icons: %d\n", id, messageStack.size(), iconStack.size() );
		if(id == -1 && face == -1){
			emit requestSystemMessage();
			return;
		}
		
		if(id != -1 && face == -2){
			emit requestQunChat(id);
		}
		
		if(id != -1 && face != -1 && face != -2){
			emit requestChat(id);
		}
		return;
	}else{
		if(messageStack.isEmpty() && statusMenu){
			minMaxAction();
			return;
		}
	}
}


void EvaSystemTray::mouseDoubleClickEvent( QMouseEvent *me )
{
	if(clickTimer->isActive())
		clickTimer->stop();
	if(!messageStack.isEmpty() && !iconStack.isEmpty() && (me->button() == Qt::LeftButton || me->button() == Qt::MidButton)){
		int id = messageStack.last();
                messageStack.pop_back();
		int face = iconStack.last();
                iconStack.pop_back();
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
		if(messageStack.isEmpty() && (me->button() == Qt::LeftButton || me->button() == Qt::MidButton) && statusMenu){
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
    EvaMainWindow* mainWin = EvaMain::g_mainWin;
    if ( mainWin ) {
        mainWin->toggleVisible();
    }
//X 	QAction* action = QSystemTrayIcon::actionCollection()->action("minimizeRestore");
//X 	if(action)
//X 		action->activate();
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
//X 		QLabel::setPixmap(QPixmap());
		isBlinkOn = false;
	}else{
		int id = messageStack.last();
		int face = iconStack.last();
		
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
//X 	QMovie *mov = getMovie();
//X 	if(mov) mov->pause();
	
//X 	QImage img(pix->convertToImage().smoothScale(16, 16));
	QPixmap img(pix->convertToImage().smoothScale(16, 16));
//X 	QLabel::setPixmap(QPixmap(img));
        setIcon( QIcon( img ) );
	if(timerRunning)
		blinkTimer->start(300, false);
}

int EvaSystemTray::getSenderID( )
{
	if(messageStack.isEmpty()) return -1000;
	if(iconStack.isEmpty()) return -1001;
	
	int id = messageStack.last();
        messageStack.pop_back( );
	int face = iconStack.last();
        iconStack.pop_back( );
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

