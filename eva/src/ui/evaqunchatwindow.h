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

#ifndef EVAQUNCHATWINDOW_H
#define EVAQUNCHATWINDOW_H

#include "evaqunchatuibase.h"
#include "evahtmlparser.h"
#include <qdatetime.h>
#include <qcolor.h>
#include <list> 

class EvaImageResource;
//class QQFriend;
class Qun;
class CustomFaceSelector;
class QTextCodec;
class QKeyEvent;
class EvaFontSelecter;
class QPopupMenu;
class QTimer;
class QShowEvent;
class QCloseEvent;
class QHideEvent;
class RegionGrabber;

class EvaQunChatWindow : public EvaQunChatUIBase
{
	Q_OBJECT
public:
	EvaQunChatWindow(Qun * qun, QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
	~EvaQunChatWindow();
	static void setupImages(EvaImageResource *res);
	static void setQuickReplyMessages(const std::list<QString> &list);
	void setQuickReplyMenu();
	const unsigned int getQunID();
	void showMessages();	
	void appendText(const QString &txt);

	static unsigned int myQQ;
	static QString myName;
	static bool isSentByEnter;
	
	void addToolButton( QString &scriptName, QString buttonName, QString &pixmap, QString &tip );
	void removeToolButton( QString &scriptName, QString buttonName );
public slots:
	void graphicChanged();
	void slotReceivedMessage(unsigned int qunID, unsigned int senderQQ, QString message, QDateTime time, const char size, 
			const bool u, const bool i, const bool b, 
			const char blue, const char green, const char red);
	void slotAddMessage(unsigned int sender, QString sNick, unsigned int receiver, QString rNick, bool isNormal, QString message, 
			QDateTime time, const char size=9, 
			const bool u=false, const bool i=false, const bool b=false, 
			const char blue=0, const char green=0, const char red=0);
	void slotAddInformation(const QString &info);
	void slotSendResult(bool ok);
	//void slotBuddyQQShowReady(const int id);
	//void slotMyQQShowReady();
	void slotDisplayMembers();
	void slotUpdateOnlineMembers();
	///////////////void slotUpdateOnlineMembers();
	void slotPictureReady(const QString filename, const QString tmpFileName);
	void sendImageError(const QString message);
	void slotSendImageDone(const unsigned int sessionID, const unsigned int ip, const unsigned short port);

	void slotAddImageToInputEdit(const QString &);
signals:
	void requestQunDetails(const unsigned int );
	void sendQunMessage(const unsigned int, QString &, const char , 
			const bool, const bool, const bool, 
			const char, const char, const char);
	void requestHistory(const unsigned int );
	//void requestBuddyQQShow(const int);
	//void requestMyQQShow();
	void requestQunOnlineList(const unsigned int);
	
	void requestDetails(const unsigned int);
	void requestQunCard(const unsigned int, const unsigned int);
	void requestQunRealNames(const unsigned int);
	void reqeustChat(const unsigned int);
protected:
	virtual void showEvent( QShowEvent *e);
	virtual void closeEvent( QCloseEvent *e);
	virtual void hideEvent( QHideEvent *e);
private:
	void initObjects();
	void initInformation();
	void initConnection();

	
	void displaySendingMessage();
	QString getSenderName(const unsigned int qqNum);
	
	const std::list<OutCustomizedPic> getSendFiles(const std::list<QString> &fileList);
	
	CustomFaceSelector *smileyPopup;
	static EvaImageResource *images;
// 	static QString htmlHeader;
// 	static QString htmlTail;
	
	static std::list<QString> quickList;
	
	EvaFontSelecter *fontSelecter;
	QPopupMenu *sendKey;
	QPopupMenu *quickMenu;
	
	Qun *mQun;
	QTextCodec *codec;

	QDateTime sendtime;
	
	QTimer *timer;
	
	QString sendingImageMsg;
	
	RegionGrabber *grabber;
	//std::list<QString> outPicList;
	unsigned int sessionID;
	unsigned int ip;
	unsigned short port;
	
	QMap<QString, QToolButton*> m_btnMap; // button name, button widget
	QMap<QString, QString> m_scriptMap;   // button name, script name
private slots:
	void slotSmileySelected(int);
	void slotManageCustomSmileys();
// 	void slotMailClick(const QString &, const QString &);
// 	void slotURLClick(const QString &);
	
	//void slotTbBuddyClick();
	void slotTbQunDetailsClick();
	
	void slotSmileyClick();
	void slotFontClick();
	void slotImageFileClick();
	void slotScreenShotClick();
	void slotQuickReplyClick();
	void slotQuickReplyActivated(int id);
	
	void slotHistoryClick();
	void slotSendKeyClick();
	void slotSend();
	
	void slotNoticeClick();
	//void slotTbShowBuddyClick();
	//void slotTbShowMeClick();
	
	void slotInputKeyPress(QKeyEvent *e);
	
	void slotFontChanged(QColor, int);
	
	void setEnterSend();
	void setCtrlEnterSend();
	void slotPbCloseClick();
	
	void slotTimeout();
	
	void slotRequestQunMembers();
	
	
	void slotRegionGrabbed( const QPixmap & );
	
	void slotRequestQunCard(const unsigned int id);
	void slotRequestQunRealNames();

	void slotSaveAsCustomSmiley(QString);
	void slotAddCustomSmileyReady( bool );
	void slotCustomSmileyChanged();
	
	void slotCustomBtnClick();	
	void slotRequestChat(const unsigned int);
};

#endif
