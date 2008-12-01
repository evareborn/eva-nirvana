/***************************************************************************
 *   Copyright (C) 2004-2005 by yunfan                                     *
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

#ifndef EVACHATUIBASE_H
#define EVACHATUIBASE_H

#include <qvariant.h>
#include <qwidget.h>
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
class QLabel;
class QLineEdit;
class EvaTextEdit;
class Q3TextEdit;
//X class EvaChatView;
class WebkitChatView;
class QPushButton;
class QSplitter;
class Q3Frame;
//X class EvaFilePanel;

class EvaChatUIBase : public QWidget
{
	Q_OBJECT

public:
	EvaChatUIBase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
	~EvaChatUIBase();
	
	QToolButton* tbBuddy;
	QLabel* lblIP;
	QLineEdit* leIP;
	QLabel* lblSignature;
	WebkitChatView *chatDisplay;
	QWidget *lowerLayoutWidget;
	QToolButton* tbSmiley;
	QToolButton* tbFont;
	QToolButton* tbFile;
	QToolButton* tbB;
	QToolButton* tbU;
	QToolButton* tbI;
	QToolButton* tbAddImage;
	QToolButton* tbScreenShot;
	QToolButton* tbQuickReply;
	QToolButton* tbHideShows;
	QToolButton* tbEnableSound;
	EvaTextEdit* kteInput;
	QPushButton* pbHistory;
	QPushButton* pbClose;
	QPushButton* pbSend;
	QPushButton* pbSendKey;
	Q3Frame *m_ShowFrame;
	QLabel* lblBuddyShow;
	QToolButton* tbShowBuddy;
	QLabel* lblMeShow;
	QToolButton* tbShowMe;
	QSplitter* splitter;

//X 	EvaFilePanel *m_FilePanel;
	Q3Frame *m_FilePanelFrame;

protected:
	//QGridLayout* EvaChatUIBaseLayout;
	Q3HBoxLayout* EvaChatUIBaseLayout;
	Q3VBoxLayout* layout5;
	Q3HBoxLayout* layout4;
	Q3HBoxLayout* layout3;
	QSpacerItem* spcTootBtn1;
	QSpacerItem* spcToolBtn2;
	QSpacerItem* spcToolBtn3;
	Q3HBoxLayout* layout2;
	QSpacerItem* spcBottom;
	Q3VBoxLayout* layout19;
	QSpacerItem* spacer12;
	QSpacerItem* spacer10;
	Q3VBoxLayout* lowerLayout;
	Q3VBoxLayout* m_ShowLayout;
	Q3VBoxLayout* m_FilePanelLayout;

protected slots:
	virtual void languageChange();

};

#endif
