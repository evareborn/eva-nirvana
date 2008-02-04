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

#ifndef EVACHATVIEW_H
#define EVACHATVIEW_H

#include <khtmlview.h>
#include <khtml_part.h>
#include <kurl.h>
#include <qdatetime.h>
#include <qcolor.h>

class KPopupMenu;
class MenuPrivateData;
//class KAction;

class EvaChatView : public KHTMLPart
{
	Q_OBJECT
public:
	EvaChatView(QWidget* parent = 0, const char* name = 0);
	virtual ~EvaChatView();
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
	
	KPopupMenu *menu;
	MenuPrivateData *d;
	KAction *copyAction;
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
	void slotLinkClicked( const KURL & urls, const KParts::URLArgs &);
	void copy();
	void searchProvider();
	void openSelection();
	void slotCopyLinkLocation();
	void slotCopyImage();
	void slotSaveImageAs();
	void slotSaveAsCustomSmiley();
};
#endif

