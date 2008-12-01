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

 
#ifndef UI_SIMPLECHATVIEW_H_
#define UI_SIMPLECHATVIEW_H_
 
#include <qdatetime.h>
#include <q3textedit.h>
//Added by qt3to4:
#include <QMenu>
 
class QAction;
 
class SimpleChatView : public Q3TextEdit {
    Q_OBJECT
    public:
        SimpleChatView( QWidget*parent = 0, const char* name = 0 );
        virtual ~SimpleChatView();

	void append(QString & nick, QDateTime time, QColor nameColor, bool isNormal, 
			QColor msgColor, Q_UINT8 size, 
			bool underline, bool italic, bool bold, QString contents );
	void updatePicture( const QString filename , const QString tmpFileName);
	void showInfomation(const QString &info);
	void showFileNotification(const QString &who, const QString &filename, const int size, 
				const unsigned int session, const bool isSend = false);
	void askResumeMode(const QString filename, const unsigned int session);
	void showContents();
 
    private slots:

	void slotScrollToBottom();

    private:
	QString wrapFontAttributes(QColor color, Q_UINT8 size, 
					bool underline, bool italic, bool bold, QString contents);
	QString wrapNickName(QString &nick, QDateTime time, QColor color, bool isNormal);
	
	void updateContents(const QString &contents);

	QMenu *menu;
	QAction *copyAction;
	QString buffer;
	bool m_isScrollAtBottom;

};

#endif // UI_SIMPLECHATVIEW_H_
