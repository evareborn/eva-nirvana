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

#ifndef MAINLISTFONTSETTINGUIBASE_H
#define MAINLISTFONTSETTINGUIBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class KTabWidget;
class QGroupBox;
class QLabel;
class KColorCombo;
class QCheckBox;

class MainListFontSettingUIBase : public QWidget
{
	Q_OBJECT

public:
	MainListFontSettingUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~MainListFontSettingUIBase();
	
	KTabWidget* ktabMainListFont;
	QWidget* TabPage;
	QGroupBox* gbBuddyNick;
	QLabel* lblBuddyNickColor;
	KColorCombo* kcbbBuddyNickColor;
	QCheckBox* chbBuddyNickFontB;
	QCheckBox* chbBuddyNickFontU;
	QCheckBox* chbBuddyNickFontI;
	QGroupBox* gbBuddyFlash;
	QLabel* lblBuddyFlashColor;
	KColorCombo* kcbbBuddyFlashColor;
	QCheckBox* chbBuddyFlashFontB;
	QCheckBox* chbBuddyFlashFontU;
	QCheckBox* chbBuddyFlashFontI;
	QGroupBox* gbBuddySignature;
	QLabel* lblBuddySigColor;
	KColorCombo* kcbbBuddySigColor;
	QCheckBox* chbBuddySigFontB;
	QCheckBox* chbBuddySigFontU;
	QCheckBox* chbBuddySigFontI;
	QWidget* TabPage_2;
	QGroupBox* gbQunName;
	QLabel* lblQunNameColor;
	KColorCombo* kcbbQunNameColor;
	QCheckBox* chbQunNameFontB;
	QCheckBox* chbQunNameFontU;
	QCheckBox* chbQunNameFontI;
	QGroupBox* gbQunFlash;
	QLabel* lblQunFlashColor;
	KColorCombo* kcbbQunFlashColor;
	QCheckBox* chbQunFlashFontB;
	QCheckBox* chbQunFlashFontU;
	QCheckBox* chbQunFlashFontI;
	QWidget* TabPage_3;
	QGroupBox* gbGroupName;
	QLabel* lblGroupNameColor;
	KColorCombo* kcbbGroupNameColor;
	QCheckBox* chbGroupNameFontB;
	QCheckBox* chbGroupNameFontU;
	QCheckBox* chbGroupNameFontI;
	QGroupBox* gbGroupFlash;
	QLabel* lblGroupFlashColor;
	KColorCombo* kcbbGroupFlashColor;
	QCheckBox* chbGroupFlashFontB;
	QCheckBox* chbGroupFlashFontU;
	QCheckBox* chbGroupFlashFontI;
	QGroupBox* gbGroupOnlineCount;
	QLabel* lblGroupOnlineColor;
	KColorCombo* kcbbGroupOnlineColor;
	QCheckBox* chbGroupOnlineFontB;
	QCheckBox* chbGroupOnlineFontU;
	QCheckBox* chbGroupOnlineFontI;

protected:
	QGridLayout* MainListFontSettingUIBaseLayout;
	QVBoxLayout* TabPageLayout;
	QVBoxLayout* gbBuddyNickLayout;
	QHBoxLayout* layout1;
	QHBoxLayout* layout2;
	QVBoxLayout* gbBuddyFlashLayout;
	QHBoxLayout* layout3;
	QHBoxLayout* layout4;
	QVBoxLayout* gbBuddySignatureLayout;
	QHBoxLayout* layout5;
	QHBoxLayout* layout6;
	QVBoxLayout* TabPageLayout_2;
	QVBoxLayout* layout11;
	QVBoxLayout* gbQunNameLayout;
	QHBoxLayout* layout1_2;
	QHBoxLayout* layout2_2;
	QVBoxLayout* gbQunFlashLayout;
	QHBoxLayout* layout3_2;
	QHBoxLayout* layout4_2;
	QVBoxLayout* TabPageLayout_3;
	QVBoxLayout* gbGroupNameLayout;
	QHBoxLayout* layout1_3;
	QHBoxLayout* layout2_3;
	QVBoxLayout* gbGroupFlashLayout;
	QHBoxLayout* layout3_3;
	QHBoxLayout* layout4_3;
	QVBoxLayout* gbGroupOnlineCountLayout;
	QHBoxLayout* layout5_2;
	QHBoxLayout* layout6_2;

protected slots:
	virtual void languageChange();

};

#endif // MAINLISTFONTSETTINGUIBASE_H
