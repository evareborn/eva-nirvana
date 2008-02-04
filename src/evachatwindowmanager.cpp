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
#include "evachatwindowmanager.h"
#include "evaresource.h"
#include "evafriendlist.h"
#include "evausersetting.h"
#include "evachatwindow.h"
#include "evaqunchatwindow.h"
#include "evaqunlist.h"
#include "evamain.h"
#include "evauser.h"
#include "evaresource.h"
#include "evaqtutil.h"
#include "evahtmlparser.h"
#include "evascriptmanager.h"

#include <qmessagebox.h>
#include <qapplication.h>
#include <kwin.h>
#include <klocale.h>
		
EvaChatWindowManager::EvaChatWindowManager(EvaImageResource *img, EvaUserSetting *setting, QString myNickname, const int id)
{
	EvaChatWindow::setupImages(img);
	EvaChatWindow::myQQ = id;
	EvaChatWindow::myName = myNickname;
	
	EvaQunChatWindow::setupImages(img);
	EvaQunChatWindow::myQQ = id;
	EvaQunChatWindow::myName = myNickname;
	
	userSetting = setting;
	windowList.setAutoDelete(true);
	messageQueue.setAutoDelete(true);
	
	qunWindowList.setAutoDelete(true);
	qunMessageQueue.setAutoDelete(true);
	
}

EvaChatWindowManager::~EvaChatWindowManager()
{
	//closeAllWindows();
}

void EvaChatWindowManager::setMyName(QString myName, const unsigned int id)
{
	EvaChatWindow::myQQ = id;
	EvaChatWindow::myName = myName;
	EvaQunChatWindow::myQQ = id;
	EvaQunChatWindow::myName = myName;
}

bool EvaChatWindowManager::isChatWindowExisted(const unsigned int id)
{
	EvaChatWindow *win = getWindow(id);
	if(win){
		if(win->isHidden()){
			return false;
		}else{
			return true;
		}
	}else{
		return false;
	}	
}
	
EvaChatWindow * EvaChatWindowManager::openChatWindow(QQFriend * frd, const bool isSendingFile)
{
	if(!frd) return NULL;
	EvaChatWindow *win = getWindow(frd->getQQ());
	if(win){
		if(win->isHidden()){
			windowList.removeRef(win);
			win = newWindow(frd);
			//win->show();
		}else{
			KWin::forceActiveWindow(win->winId());
			return win;
		}
	}else{
		win = newWindow(frd);
		//win->show();
	}
	
	Message *msg;
	for(uint i = 0; i<messageQueue.count(); i++){
		msg = messageQueue.at(i);
		if(msg->sender == frd->getQQ()){
			win->slotReceivedMessage(msg->sender, msg->isNormal, msg->message, msg->time, msg->size,
						msg->u, msg->i, msg->b, msg->blue, msg->green, msg->red);
			messageQueue.remove(i--);
		}
	}
	win->show();
	win->showMessages();
	if(isSendingFile)
		win->openSendFileDialog();
	return win;
}

void EvaChatWindowManager::closeAllWindows()
{
	EvaChatWindow *win;
	for(uint i=0; i<windowList.count();i++){
		win = windowList.at(i);
		if(win){
			if(win->isVisible()){
				win->close();
			}
			delete win;
			windowList.remove(i--);
		}
	}
	//m_btnList.clear();
}

std::list<int> EvaChatWindowManager::getMessages()
{
	std::list<int> list;
	for(uint i=0; i<messageQueue.count();i++){
		list.push_back(messageQueue.at(i)->sender);
	}
	return list;
}

void EvaChatWindowManager::graphicChanged()
{
	EvaChatWindow *win;
	for(uint i=0; i<windowList.count();i++){
		win = windowList.at(i);
		if(win->isVisible()){
			win->close();
			windowList.remove(i--);
			continue;
		}
		win->graphicChanged();
	}	
}

void EvaChatWindowManager::slotReceivedMessage(unsigned int sender, bool isNormal, QString message, QDateTime time, const char size, 
		const bool u, const bool i, const bool b, 
		const char blue, const char green, const char red)
{	
	// notify script
	GetScriptManager()->notifyMessage( false, sender, time, message);
	
	EvaChatWindow *win = getWindow(sender);
	if(win && win->isVisible()){
		win->slotReceivedMessage(sender, isNormal, message, time, size, u, i, b, blue, green, red);
		win->showMessages();
		if(!win->isActiveWindow()){
			KWin::demandAttention(win->winId(), true);
		}
		return;
	}
	
	Message *msg = new Message;
	msg->sender = sender;
	msg->isNormal = isNormal;
	msg->message = message;
	msg->time = time;
	msg->size = size;
	msg->u = u;
	msg->i = i;
	msg->b = b;
	msg->blue = blue;
	msg->green = green;
	msg->red = red;
	
	messageQueue.append(msg);
}

void EvaChatWindowManager::slotSendResult(unsigned int id, bool ok)
{
	EvaChatWindow *win = getWindow(id);
	if(win && win->isVisible()) {
		win->slotSendResult(ok);
	}else{
		if(!ok)
			QMessageBox::information(0, i18n("Send Message"), QString(i18n("Send message to \"%1\" failed.")).arg(id));
	}
}

void EvaChatWindowManager::slotBuddyQQShowReady(const unsigned int id)
{
	EvaChatWindow *win = getWindow(id);
	if(win && win->isVisible())
		win->slotBuddyQQShowReady(id);	
}

void EvaChatWindowManager::slotMyQQShowReady()
{
	for(uint i=0; i<windowList.count();i++){
		if(windowList.at(i)->isVisible())
			windowList.at(i)->slotMyQQShowReady();
	}	
}

EvaChatWindow *EvaChatWindowManager::newWindow(QQFriend *frd)
{
	EvaChatWindow::setQuickReplyMessages(userSetting->getQuickReplyList());
	EvaChatWindow::isSentByEnter = userSetting->isSendKeyEnterEnabled();
	EvaChatWindow *win = new EvaChatWindow(frd);
	addBtnsToBuddyWin(win);
	windowList.append(win);
	QObject::connect(win, SIGNAL(requestDetails(const unsigned int)), this, SIGNAL(requestDetails(const unsigned int)));
	QObject::connect(win, SIGNAL(sendMessage(const unsigned int , const bool, QString &, const char , 
		const bool , const bool , const bool , 
		const char, const char , const char)), this, 
		SIGNAL(sendMessage(const unsigned int , const bool, QString &, const char, 
		const bool , const bool , const bool , 
		const char , const char, const char)));	
	QObject::connect(win, SIGNAL(requestHistory(const unsigned int )), this, SIGNAL(requestHistory(const unsigned int )));
	QObject::connect(win, SIGNAL(requestBuddyQQShow(const unsigned int)), this, SIGNAL(requestBuddyQQShow(const unsigned int)));
	QObject::connect(win, SIGNAL(requestMyQQShow()), this, SIGNAL(requestMyQQShow()));

	QObject::connect(win, SIGNAL(fileTransferSend(const unsigned int, const unsigned int, const QValueList<QString>,
							const QValueList<unsigned int>, const unsigned char)),
				SIGNAL(fileTransferSend(const unsigned int, const unsigned int, const QValueList<QString>,
							const QValueList<unsigned int>, const unsigned char)));
	QObject::connect(win, SIGNAL(fileTransferAccept(const unsigned int, const unsigned int, const QString,
							const unsigned char )),
				SIGNAL(fileTransferAccept(const unsigned int, const unsigned int, const QString,
							const unsigned char )));
	QObject::connect(win, SIGNAL(fileTransferCancel(const unsigned int, const unsigned int)),
				SIGNAL(fileTransferCancel(const unsigned int, const unsigned int)));
	QObject::connect(win, SIGNAL(fileTransferResume(const unsigned int, const unsigned int, const bool)),
				SIGNAL(fileTransferResume(const unsigned int, const unsigned int, const bool)));

	return win;
}

EvaChatWindow *EvaChatWindowManager::getWindow(const unsigned int id)
{
	for(uint i=0; i<windowList.count();i++){
		if(windowList.at(i)->getBuddyQQ() == id)
			return windowList.at(i);
	}
	return NULL;
}

void EvaChatWindowManager::slotReceivedQunMessage( unsigned int qunID, unsigned int senderQQ, QString message, QDateTime time, const char size, 
						const bool u, const bool i, const bool b, 
						const char blue, const char green, const char red )
{
	// notify script
	int extId = EvaMain::user->getQunList()->getQunExtID(qunID);
	
	GetScriptManager()->notifyMessage( true, extId, time, message);
	EvaQunChatWindow *win = getQunWindow(qunID);
	if(win && win->isVisible()){
		win->slotReceivedMessage(qunID, senderQQ, message, time, size, u, i, b, blue, green, red);
		win->showMessages();
		if(!win->isActiveWindow()){
			KWin::demandAttention(win->winId(), true);
		}
		return;
	}
	
	QunMessage *msg = new QunMessage;
	msg->qunID = qunID;
	msg->sender = senderQQ;
	msg->message = message;
	msg->time = time;
	msg->size = size;
	msg->u = u;
	msg->i = i;
	msg->b = b;
	msg->blue = blue;
	msg->green = green;
	msg->red = red;
	
	qunMessageQueue.append(msg);
	
	EvaHtmlParser parser;
	QString cachesPath = EvaMain::user->getSetting()->getPictureCacheDir();
	parser.setAbsImagePath(EvaMain::images->getSmileyPath(), cachesPath);
	std::list<CustomizedPic> picList = parser.convertToHtml(message, true, true);
	if(picList.size()){
		EvaAskForCustomizedPicEvent *event = new EvaAskForCustomizedPicEvent();
		event->setPicList(picList);
		event->setQunID(qunID);
		QApplication::postEvent((QObject *)EvaMain::picManager, event);
	}
}

void EvaChatWindowManager::slotQunSendResult( unsigned int id, bool ok ,QString msg)
{
	EvaQunChatWindow *win = getQunWindow(id);
	if(win && win->isVisible()) {
		win->slotSendResult(ok);
	}else{
		if(!ok)
			QMessageBox::information(0, i18n("Send Message Failed"), msg);
	}
}

void EvaChatWindowManager::slotQunJoinEvent(const unsigned int qunID,
					const short imType,
					const unsigned int sender,
					const unsigned int commander)
{
	EvaQunChatWindow *win = getQunWindow(qunID);
	if(win ) {
		QString info;
		switch(imType){
			case QQ_RECV_IM_APPROVE_JOIN_QUN:
				info = QString(i18n("%1 has joined this Qun approved by %2."))
								.arg(sender).arg(commander);
				break;
			case QQ_RECV_IM_ADDED_TO_QUN:
				info = QString(i18n("%1 has joined this Qun added by %2."))
								.arg(sender).arg(commander);
				break;
			case QQ_RECV_IM_DELETED_FROM_QUN:
				info = QString(i18n("%1 has quit this Qun.")
								.arg(sender));
				break;
			default:
				info = QString(i18n("%1 has done some unknown action on %2."))
								.arg(commander).arg(sender);
		}
		
		win->slotAddInformation(info);
	}
}

EvaQunChatWindow * EvaChatWindowManager::getQunWindow( const unsigned int id )
{
	for(uint i=0; i<qunWindowList.count();i++){
		if(qunWindowList.at(i)->getQunID() == id)
			return qunWindowList.at(i);
	}
	return NULL;
}

EvaQunChatWindow * EvaChatWindowManager::newQunWindow( Qun * qun )
{
	EvaQunChatWindow::setQuickReplyMessages(userSetting->getQuickReplyList());
	EvaQunChatWindow::isSentByEnter = userSetting->isSendKeyEnterEnabled();
	EvaQunChatWindow *win = new EvaQunChatWindow(qun);
	addBtnsToQinWin(win);
	qunWindowList.append(win);
	QObject::connect(win, SIGNAL(requestQunDetails(const unsigned int)), this, SIGNAL(requestQunDetails(const unsigned int)));
	QObject::connect(win, SIGNAL(sendQunMessage(const unsigned int, QString &, const char , 
			const bool, const bool, const bool, 
			const char, const char, const char)), this, 
		SIGNAL(sendQunMessage(const unsigned int, QString &, const char , 
			const bool, const bool, const bool, 
			const char, const char, const char)));	
	QObject::connect(win, SIGNAL(requestHistory(const unsigned int )), this, SIGNAL(requestQunHistory(const unsigned int )));
	QObject::connect(win, SIGNAL(requestDetails(const unsigned int)), this, SIGNAL(requestDetails(const unsigned int )));
	QObject::connect(win, SIGNAL(requestQunOnlineList(const unsigned int)), this, SIGNAL(requestQunOnlineList(const unsigned int)));
	QObject::connect(win, SIGNAL(requestQunCard(const unsigned int, const unsigned int)),
			this, SIGNAL(requestQunCard(const unsigned int, const unsigned int)));
	//QObject::connect(win, SIGNAL(requestQunMembers(const unsigned int)), this, SIGNAL(requestQunMembers(const unsigned int)));
	QObject::connect(win, SIGNAL(requestQunRealNames(const unsigned int)), this, SIGNAL(requestQunRealNames(const unsigned int)));

	return win;
}

bool EvaChatWindowManager::isQunChatWindowExisted( const unsigned int id )
{
	EvaQunChatWindow *win = getQunWindow(id);
	if(win){
		if(win->isHidden()){
			return false;
		}else{
			return true;
		}
	}else{
		return false;
	}
}

void EvaChatWindowManager::openQunChatWindow( Qun * qun )
{
	if(!qun) return;
	EvaQunChatWindow *win = getQunWindow(qun->getQunID());
	if(win){
		if(win->isHidden()){
			qunWindowList.removeRef(win);
			win = newQunWindow(qun);
			//win->show();
		}else{
			KWin::forceActiveWindow(win->winId());
			return;
		}
	}else{
		win = newQunWindow(qun);
		//win->show();
	}
	
	QunMessage *msg;
	for(uint i = 0; i<qunMessageQueue.count(); i++){
		msg = qunMessageQueue.at(i);
		if(msg->qunID == qun->getQunID()){
			win->slotReceivedMessage(msg->qunID, msg->sender, msg->message, msg->time, msg->size,
						msg->u, msg->i, msg->b, msg->blue, msg->green, msg->red);
			qunMessageQueue.remove(i--);
		}
	}
	win->show();
	win->showMessages();
}

void EvaChatWindowManager::slotQunOnlineMembers(const unsigned int id)
{
	EvaQunChatWindow *win = getQunWindow(id);
	if(win)
		win->slotUpdateOnlineMembers();
}

void EvaChatWindowManager::slotQunPicReady( const unsigned int id, const QString fileName , const QString tmpFileName)
{
	EvaQunChatWindow *win = getQunWindow(id);
	if(win)
		win->slotPictureReady(fileName, tmpFileName);
}

void EvaChatWindowManager::slotQunPicSent( const int unsigned id, const unsigned int sessionID, const unsigned int ip, const unsigned short port)
{
	EvaQunChatWindow *win = getQunWindow(id);
	if(win)
		win->slotSendImageDone(sessionID, ip, port);
}

void EvaChatWindowManager::slotSendImageError( const unsigned int id, const QString message )
{
	EvaQunChatWindow *win = getQunWindow(id);
	if(win)
		win->sendImageError(message);
}

void EvaChatWindowManager::slotQunMemberInfoReady( const unsigned int id)
{
	EvaQunChatWindow *win = getQunWindow(id);
	if(win)
		win->slotDisplayMembers();
}

// if Qun is not in user's list, just close the chat window
void EvaChatWindowManager::slotQunListUpdated()
{
	int id = 0;
	EvaQunChatWindow *win = 0;
	for(uint i=0; i<qunWindowList.count();i++){
		win = qunWindowList.at(i);
		id = win->getQunID();
		if( ! (EvaMain::user->getQunList()->getQun(id) ) ){
			win->hide();
			qunWindowList.removeRef(win);
		}
	}
}

void EvaChatWindowManager::slotReceivedFileRequest(QQFriend * frd, const unsigned int session,
						const QString fileName, const int size,
						const unsigned char transferType )
{
	EvaChatWindow *win = openChatWindow(frd);
	if(win) win->slotReceivedFileRequest(session, fileName, size, transferType);
}

void EvaChatWindowManager::slotReceivedFileAccepted( const unsigned int id, const unsigned int session, 
					const bool isAccepted, const unsigned char transferType )
{
	EvaChatWindow *win = getWindow(id);
	if(win) win->slotReceivedFileAccepted(session, isAccepted, transferType);
}

void EvaChatWindowManager::slotChangeFileSessionTo(const unsigned int id, const unsigned int oldSession, const unsigned int newSession )
{
	EvaChatWindow *win = getWindow(id);
	if(win) win->slotSessionChangedTo(oldSession, newSession);
}

void EvaChatWindowManager::slotFileNotifyNormalInfo( const unsigned int id, const unsigned int session, 
			EvaFileStatus status, const QString dir, const QString filename, const unsigned int size,
			const unsigned char transferType)
{
	EvaChatWindow *win = getWindow(id);
	if(win) win->slotFileNotifyNormalInfo(session, status, dir, filename, size, transferType);
}

void EvaChatWindowManager::slotFileStatusNotification( const unsigned int id, const unsigned int session, const KURL & url, 
				const unsigned int size, const unsigned int bytes, const unsigned int time )
{
	EvaChatWindow *win = getWindow(id);
	if(win) win->slotFileStatusNotification(session, url, size, bytes, time);
}

void EvaChatWindowManager::addButton( QString &scriptName, QString &buttonName, QString &image, QString &tip)
{
	CustomButton btn;
	btn.script = scriptName;
	btn.button = buttonName;
	btn.tip    = tip;
	btn.image  = image;
	std::list<CustomButton>::iterator it;
	for(it = m_btnList.begin(); it != m_btnList.end(); it++){
		if(it->script == scriptName &&
			 it->button == buttonName)
			return;
	}
	addBuddyWindowButton(btn);
	addQunWindowButton(btn);
	m_btnList.push_back(btn);
}

void EvaChatWindowManager::removeButton( QString &scriptName, const QString &buttonName )
{
	std::list<CustomButton>::iterator it;
	for(it = m_btnList.begin(); it != m_btnList.end(); it++){
		if(it->script == scriptName &&
				 (it->button == buttonName || buttonName.isEmpty()) ){
			removeBuddyWindowButton(*it);
			removeQunWindowButton(*it);
			m_btnList.erase(it);
			return;
		}
	}
}

void EvaChatWindowManager::addBuddyWindowButton( CustomButton & btn )
{
	for(uint i=0; i<windowList.count();i++){
		if(windowList.at(i))
			windowList.at(i)->addToolButton(btn.script, btn.button, btn.image, btn.tip);
	}
}

void EvaChatWindowManager::addQunWindowButton( CustomButton & btn )
{
	for(uint i=0; i<qunWindowList.count();i++){
		if(qunWindowList.at(i))
			qunWindowList.at(i)->addToolButton(btn.script, btn.button, btn.image, btn.tip);
	}
}

void EvaChatWindowManager::removeBuddyWindowButton( CustomButton & btn )
{
	for(uint i=0; i<windowList.count();i++){
		if(windowList.at(i))
			windowList.at(i)->removeToolButton(btn.script, btn.button);
	}
}

void EvaChatWindowManager::removeQunWindowButton( CustomButton & btn )
{
	for(uint i=0; i<qunWindowList.count();i++){
		if(qunWindowList.at(i))
			qunWindowList.at(i)->removeToolButton(btn.script, btn.button);
	}
}

void EvaChatWindowManager::addBtnsToBuddyWin( EvaChatWindow * win )
{
	std::list<CustomButton>::iterator it;
	for(it = m_btnList.begin(); it != m_btnList.end(); it++){
		win->addToolButton( it->script, it->button, it->image, it->tip);
	}
}

void EvaChatWindowManager::addBtnsToQinWin( EvaQunChatWindow * win )
{
	std::list<CustomButton>::iterator it;
	for(it = m_btnList.begin(); it != m_btnList.end(); it++){
		win->addToolButton( it->script, it->button, it->image, it->tip);
	}
}
