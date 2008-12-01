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

#ifndef EVAQUNCHATUIBASE_H
#define EVAQUNCHATUIBASE_H

#include <qvariant.h>
#include <qwidget.h>
#include <q3textedit.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>
#include <Q3Frame>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QToolButton;
class QPushButton;
class Q3Frame;
class KSqueezedTextLabel;
class QLabel;
class EvaTextEdit;
class WebkitChatView;
class Q3ListView;
class EvaQunListView;
class QSplitter;

class EvaQunChatUIBase : public QWidget
{
	Q_OBJECT

public:
	EvaQunChatUIBase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
	~EvaQunChatUIBase();
	
	QToolButton* tbQunDetails;
	WebkitChatView* chatDisplay;
	QWidget *lowerLayoutWidget;
	QToolButton* tbSmiley;
	QToolButton* tbFont;
	QToolButton* tbB;
	QToolButton* tbU;
	QToolButton* tbI;
	QToolButton* tbImageFile;
	QToolButton* tbScreenShot;
	QToolButton* tbQuickReply;
	QToolButton* tbEnableSound;
	EvaTextEdit* teInput;
	QPushButton* pbHistory;
	QPushButton* pbClose;
	QPushButton* pbSend;
	QPushButton* pbSendKey;
	Q3Frame* frmNotice;
	QToolButton* tbtnNotice;
	Q3TextEdit *teNotice;
	Q3Frame *frmMembers;
	QLabel* lblMembers;
	EvaQunListView* memberList;
	QSplitter *memberSplitter;
	QSplitter *splitter;

protected:
	Q3GridLayout* EvaQunChatUIBaseLayout;
	Q3HBoxLayout* layout16;
	Q3VBoxLayout* layout15;
	Q3HBoxLayout* layout6;
	QSpacerItem* spacer4;
	Q3HBoxLayout* layout14;
	QSpacerItem* spacer2;
	QSpacerItem* spacer3;
	QSpacerItem* spacer5;
	Q3HBoxLayout* layout3;
	QSpacerItem* spacer1;
	Q3VBoxLayout* layout12;
	Q3GridLayout* frmNoticeLayout;
	Q3VBoxLayout* frmMemberLayout;
	Q3VBoxLayout* layout4;
	Q3VBoxLayout* lowerLayout;

protected slots:
	virtual void languageChange();

};

#endif // EVAQUNCHATUIBASE_H
