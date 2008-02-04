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
#ifndef QUNDETAILSUI_H
#define QUNDETAILSUI_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QTabWidget;
class QLineEdit;
class QToolButton;
class QGroupBox;
class QTextEdit;
class QButtonGroup;
class QRadioButton;
class QTable;
class QPushButton;
class QComboBox;
class QFrame;

class QunDetailsUI : public QWidget
{
    Q_OBJECT

public:
    QunDetailsUI( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~QunDetailsUI();

    QLabel* lblLogo;
    QTabWidget* tabWMain;
    QWidget* tabDetails;
    QLabel* lblID;
    QLabel* lblCreator;
    QLabel* lblName;
    QLineEdit* leQunID;
    QLineEdit* leCreator;
    QLineEdit* leQunName;
    QLabel* lblQunFace;
    QLabel* lblCategory;
    QToolButton* tbCategory;
    QGroupBox* gbNotice;
    QTextEdit* teNotice;
    QGroupBox* gbDescription;
    QTextEdit* teDescription;
    QButtonGroup* bgAuthen;
    QRadioButton* rbNoAuthenNeed;
    QRadioButton* rbNeedAuthen;
    QRadioButton* rbRejectAnyone;
    QWidget* tabMember;
    QTable* tblMembers;
    QPushButton* pbSetMembers;
    QPushButton* pbAddToMe;
    QPushButton* pbDelMembers;
    QPushButton* pbSetAdmin;
    QPushButton* pbUnsetAdmin;
    QPushButton* pbTransfer;
    QWidget* tabSetting;
    QButtonGroup* bgSettings;
    QRadioButton* rbRecord_Show;
    QRadioButton* rbPopupWindow;
    QRadioButton* rbDisplayNumbers;
    QRadioButton* rbRecord_only;
    QRadioButton* rbRejectMsgs;
    QWidget* TabCard;
    QLabel* lblMyName;
    QLabel* lblMyGender;
    QLabel* lblMyPhone;
    QLabel* lblMyEmail;
    QLineEdit* leMyName;
    QComboBox* cbbMyGender;
    QLineEdit* leMyPhone;
    QLineEdit* leMyEmail;
    QFrame* line1;
    QLabel* lblMyComment;
    QTextEdit* teMyMemo;
    QPushButton* pbUpdate;
    QPushButton* pbClose;

protected:
    QVBoxLayout* QunDetailsUILayout;
    QHBoxLayout* layout18;
    QVBoxLayout* tabDetailsLayout;
    QHBoxLayout* layout17;
    QVBoxLayout* layout1;
    QVBoxLayout* layout2;
    QVBoxLayout* layout25;
    QHBoxLayout* layout5;
    QSpacerItem* spacer1;
    QHBoxLayout* layout24;
    QGridLayout* gbNoticeLayout;
    QGridLayout* gbDescriptionLayout;
    QVBoxLayout* bgAuthenLayout;
    QGridLayout* tabMemberLayout;
    QHBoxLayout* layout17_2;
    QVBoxLayout* layout16;
    QSpacerItem* spacer6;
    QSpacerItem* spacer7;
    QGridLayout* tabSettingLayout;
    QVBoxLayout* bgSettingsLayout;
    QVBoxLayout* TabCardLayout;
    QHBoxLayout* layout19;
    QVBoxLayout* layout11;
    QVBoxLayout* layout18_2;
    QHBoxLayout* layout17_3;
    QSpacerItem* spacer4;
    QHBoxLayout* layout20;
    QSpacerItem* spacer5;
    QHBoxLayout* layout21;
    QVBoxLayout* layout15;
    QSpacerItem* spacer3;
    QHBoxLayout* layout9;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // QUNDETAILSUI_H
