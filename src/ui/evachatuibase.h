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

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QToolButton;
class QLabel;
class QLineEdit;
class EvaTextEdit;
class EvaChatView;
class QPushButton;
class QSplitter;
class QFrame;
class EvaFilePanel;

class EvaChatUIBase : public QWidget
{
	Q_OBJECT

public:
	EvaChatUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~EvaChatUIBase();
	
	QToolButton* tbBuddy;
	QLabel* lblIP;
	QLineEdit* leIP;
	QLabel* lblSignature;
	EvaChatView *chatDisplay;
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
	EvaTextEdit* kteInput;
	QPushButton* pbHistory;
	QPushButton* pbClose;
	QPushButton* pbSend;
	QPushButton* pbSendKey;
	QFrame *m_ShowFrame;
	QLabel* lblBuddyShow;
	QToolButton* tbShowBuddy;
	QLabel* lblMeShow;
	QToolButton* tbShowMe;
	QSplitter* splitter;

	EvaFilePanel *m_FilePanel;
	QFrame *m_FilePanelFrame;

protected:
	//QGridLayout* EvaChatUIBaseLayout;
	QHBoxLayout* EvaChatUIBaseLayout;
	QVBoxLayout* layout5;
	QHBoxLayout* layout4;
	QHBoxLayout* layout3;
	QSpacerItem* spcTootBtn1;
	QSpacerItem* spcToolBtn2;
	QSpacerItem* spcToolBtn3;
	QHBoxLayout* layout2;
	QSpacerItem* spcBottom;
	QVBoxLayout* layout19;
	QSpacerItem* spacer12;
	QSpacerItem* spacer10;
	QVBoxLayout* lowerLayout;
	QVBoxLayout* m_ShowLayout;
	QVBoxLayout* m_FilePanelLayout;

protected slots:
	virtual void languageChange();

};

#endif
