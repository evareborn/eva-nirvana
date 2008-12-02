/**
 * Copyright (C)2008 George Ang (gnap.an AT gmail.com)
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */ 

#ifndef WEBKITCHATVIEW_H_
#define WEBKITCHATVIEW_H_
 
#include <QWebView>
#include <QDateTime>
 
class QMenu;

class WebkitChatView : public QWebView
{
	Q_OBJECT
public:
	WebkitChatView(QWidget* parent = 0, const char* name = 0);
	virtual ~WebkitChatView();
	void append(QString & nick, QDateTime time, QColor nameColor, bool isNormal, 
			QColor msgColor, Q_UINT8 size, 
			bool underline, bool italic, bool bold, QString contents );
	void updatePicture( const QString filename , const QString tmpFileName);
	void showInfomation(const QString &info);
	void showFileNotification(const QString &who, const QString &filename, const int size, 
				const unsigned int session, const bool isSend = false);
	void askResumeMode(const QString filename, const unsigned int session);
	void showContents();
signals:
	void saveAsCustomSmiley(QString ); // full name with absolute path
	void fileTransferAcceptRequest(const unsigned int session);
	void fileTransferSaveAsRequest(const unsigned int session);
	void fileTransferCancelRequest(const unsigned int session);
	void fileTransferResume(const unsigned int session, const bool isResume);
protected:
	virtual void startDrag();
private:
		
	QString wrapFontAttributes(QColor color, Q_UINT8 size, 
					bool underline, bool italic, bool bold, QString contents);
	QString wrapNickName(QString &nick, QDateTime time, QColor color, bool isNormal);
	
	QMenu *menu;
//X 	MenuPrivateData *d;
//X 	KAction *copyAction;
	QString buffer;
	bool m_isScrollAtBottom;
	void updateContents(const QString &contents);
	static const QString protocolAccept;
	static const QString protocolSaveAs;
	static const QString protocolCancel;
	static const QString protocolResume;
	static const QString protocolNewOne;
private slots:
	void slotScrollToBottom();

	void slotPopupMenu(const QString &url, const QPoint &point);

	void slotSelectionChanged();
//X 	void slotLinkClicked( const KURL & urls, const KParts::URLArgs &);
	void copy();
	void searchProvider();
	void openSelection();
	void slotCopyLinkLocation();
	void slotCopyImage();
	void slotSaveImageAs();
	void slotSaveAsCustomSmiley();
};

#endif // WEBKITCHATVIEW_H_
