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

#ifndef EVACHATWINDOW_H
#define EVACHATWINDOW_H

#include "evachatuibase.h"
#include "../api/filetrans/evafiledownloader.h"
#include <qdatetime.h>
#include <qcolor.h>
#include <kurl.h>
#include <qvaluelist.h>
#include <qmap.h>
#include <list>

class EvaImageResource;
class QQFriend;
class CustomFaceSelector;
class QTextCodec;
class QKeyEvent;
class QCloseEvent;
class EvaFontSelecter;
class QPopupMenu;
class RegionGrabber;

class EvaChatWindow : public EvaChatUIBase
{
	Q_OBJECT
public:
	EvaChatWindow(QQFriend * frd, QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
	~EvaChatWindow();
	static void setupImages(EvaImageResource *res);
	static void setQuickReplyMessages(const std::list<QString> &list);
	void setQuickReplyMenu();
	const unsigned int getBuddyQQ();
	void showMessages();

	static int myQQ;
	static QString myName;
	static bool isSentByEnter;

	void openSendFileDialog();
	void appendText(const QString &txt);
	
	void addToolButton(QString &scriptName, QString buttonName, QString &pixmap, QString &tip);
  void removeToolButton(QString &scriptName, QString buttonName);
public slots:
	void graphicChanged();
	void slotReceivedMessage(unsigned int sender, bool isNormal, QString message, QDateTime time, const char size=9, 
			const bool u=false, const bool i=false, const bool b=false, 
			const char blue=0, const char green=0, const char red=0);
	void slotAddMessage(unsigned int sender, QString sNick, unsigned int receiver, QString rNick, bool isNormal, QString message, 
			QDateTime time, const char size=9, 
			const bool u=false, const bool i=false, const bool b=false, 
			const char blue=0, const char green=0, const char red=0);
	void slotSendResult(bool ok);
	void slotBuddyQQShowReady(const unsigned int id);
	void slotMyQQShowReady();

	void slotFileClick();
	void slotReceivedFileRequest(const unsigned int session, const QString &file, const int size,
					const unsigned char transferType);
	void slotReceivedFileAccepted(const unsigned int session, const bool isAccepted,
					const unsigned char transferType);
	void slotSessionChangedTo(const unsigned int oldSession, const unsigned int newSession);
	void slotFileNotifyNormalInfo(const unsigned int, EvaFileStatus,
					const QString, const QString, const unsigned int,
					const unsigned char);
	void slotFileStatusNotification(const unsigned int session, const KURL &url, const unsigned int size,
				const unsigned int bytes, const unsigned int time);

	void slotAddImageToInputEdit( const QString & destFile);
signals:
	void requestDetails(const unsigned int );
	void sendMessage(const unsigned int , const bool, QString &, const char , 
			const bool, const bool , const bool , 
			const char , const char , const char );
	void requestHistory(const unsigned int );
	void requestBuddyQQShow(const unsigned int);
	void requestMyQQShow();

	void fileTransferSend(const unsigned int, const unsigned int, const QValueList<QString>,
				const QValueList<unsigned int>,
				const unsigned char transferType);
	// last string is the dest path to save the file
	void fileTransferAccept(const unsigned int, const unsigned int, const QString,
				const unsigned char transferType);
	void fileTransferCancel(const unsigned int, const unsigned int);
	void fileTransferResume(const unsigned int, const unsigned int, const bool);
private:
	void initObjects();
	void initInformation();
	void initConnection();

	
	void displaySendingMessage();
	
	CustomFaceSelector *smileyPopup;
	static EvaImageResource *images;
	
	static std::list<QString> quickList;
	QPopupMenu *quickMenu;
	
	EvaFontSelecter *fontSelecter;
	QPopupMenu *sendKey;
	
	QQFriend *buddy;
	QTextCodec *codec;

	QDateTime sendtime;

	int m_NumImages;
	RegionGrabber *grabber;

	QMap<QString, QToolButton*> m_btnMap; // button name, button widget
	QMap<QString, QString> m_scriptMap;   // button name, script name
	
	QMap<unsigned int, QString> m_FileList;
	QMap<unsigned int, unsigned int> m_FileNoList;
	const bool addToFileList(const unsigned int session, const QString filename);
	QString getFileName(const unsigned int session);
	const unsigned int getSession(const unsigned int showSession);
	void removeFromFileList(const unsigned int session);

	void closeEvent(QCloseEvent *e);	
private slots:
	void slotSmileySelected(int);
	
	void slotTbBuddyClick();
	
	void slotSmileyClick();
	void slotFontClick();
	void slotQuickReplyClick();
	void slotQuickReplyActivated(int id);
	void slotHideShowsClick();
	
	void slotHistoryClick();
	void slotSendKeyClick();
	void slotSend();
	
	void slotTbShowBuddyClick();
	void slotTbShowMeClick();
	
	void slotInputKeyPress(QKeyEvent *e);

	void slotFontChanged(QColor color, int size)	;

	void setEnterSend();
	void setCtrlEnterSend();
	void slotPbCloseClick();

	void slotFileTransferAcceptClicked(const unsigned int);
	void slotFileTransferSaveAsClicked(const unsigned int);
	void slotFileTransferCancelClicked(const unsigned int);
	void slotFileTransferResume(const unsigned int, const bool);

	void slotFilePanelCloseClicked(const unsigned int);

	void slotAddImageClick();

	void slotScreenShotClick();
	void slotRegionGrabbed( const QPixmap & );
	
	void slotCustomBtnClick();
};

#endif
