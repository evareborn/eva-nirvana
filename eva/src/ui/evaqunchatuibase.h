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
#include <qtextedit.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QToolButton;
class QPushButton;
class QFrame;
class KSqueezedTextLabel;
class QLabel;
class EvaTextEdit;
class EvaChatView;
class QListView;
class EvaQunListView;
class QSplitter;

class EvaQunChatUIBase : public QWidget
{
	Q_OBJECT

public:
	EvaQunChatUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~EvaQunChatUIBase();
	
	QToolButton* tbQunDetails;
	EvaChatView* chatDisplay;
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
	QFrame* frmNotice;
	QToolButton* tbtnNotice;
	QTextEdit *teNotice;
	QFrame *frmMembers;
	QLabel* lblMembers;
	EvaQunListView* memberList;
	QSplitter *memberSplitter;
	QSplitter *splitter;

protected:
	QGridLayout* EvaQunChatUIBaseLayout;
	QHBoxLayout* layout16;
	QVBoxLayout* layout15;
	QHBoxLayout* layout6;
	QSpacerItem* spacer4;
	QHBoxLayout* layout14;
	QSpacerItem* spacer2;
	QSpacerItem* spacer3;
	QSpacerItem* spacer5;
	QHBoxLayout* layout3;
	QSpacerItem* spacer1;
	QVBoxLayout* layout12;
	QGridLayout* frmNoticeLayout;
	QVBoxLayout* frmMemberLayout;
	QVBoxLayout* layout4;
	QVBoxLayout* lowerLayout;

protected slots:
	virtual void languageChange();

};

#endif // EVAQUNCHATUIBASE_H
