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
class QLabel;
class QTabWidget;
class QLineEdit;
class QToolButton;
class Q3GroupBox;
class Q3TextEdit;
class Q3ButtonGroup;
class QRadioButton;
class Q3Table;
class QPushButton;
class QComboBox;
class Q3Frame;

class QunDetailsUI : public QWidget
{
    Q_OBJECT

public:
    QunDetailsUI( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
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
    Q3GroupBox* gbNotice;
    Q3TextEdit* teNotice;
    Q3GroupBox* gbDescription;
    Q3TextEdit* teDescription;
    Q3ButtonGroup* bgAuthen;
    QRadioButton* rbNoAuthenNeed;
    QRadioButton* rbNeedAuthen;
    QRadioButton* rbRejectAnyone;
    QWidget* tabMember;
    Q3Table* tblMembers;
    QPushButton* pbSetMembers;
    QPushButton* pbAddToMe;
    QPushButton* pbDelMembers;
    QPushButton* pbSetAdmin;
    QPushButton* pbUnsetAdmin;
    QPushButton* pbTransfer;
    QWidget* tabSetting;
    Q3ButtonGroup* bgSettings;
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
    Q3Frame* line1;
    QLabel* lblMyComment;
    Q3TextEdit* teMyMemo;
    QPushButton* pbUpdate;
    QPushButton* pbClose;

protected:
    Q3VBoxLayout* QunDetailsUILayout;
    Q3HBoxLayout* layout18;
    Q3VBoxLayout* tabDetailsLayout;
    Q3HBoxLayout* layout17;
    Q3VBoxLayout* layout1;
    Q3VBoxLayout* layout2;
    Q3VBoxLayout* layout25;
    Q3HBoxLayout* layout5;
    QSpacerItem* spacer1;
    Q3HBoxLayout* layout24;
    Q3GridLayout* gbNoticeLayout;
    Q3GridLayout* gbDescriptionLayout;
    Q3VBoxLayout* bgAuthenLayout;
    Q3GridLayout* tabMemberLayout;
    Q3HBoxLayout* layout17_2;
    Q3VBoxLayout* layout16;
    QSpacerItem* spacer6;
    QSpacerItem* spacer7;
    Q3GridLayout* tabSettingLayout;
    Q3VBoxLayout* bgSettingsLayout;
    Q3VBoxLayout* TabCardLayout;
    Q3HBoxLayout* layout19;
    Q3VBoxLayout* layout11;
    Q3VBoxLayout* layout18_2;
    Q3HBoxLayout* layout17_3;
    QSpacerItem* spacer4;
    Q3HBoxLayout* layout20;
    QSpacerItem* spacer5;
    Q3HBoxLayout* layout21;
    Q3VBoxLayout* layout15;
    QSpacerItem* spacer3;
    Q3HBoxLayout* layout9;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // QUNDETAILSUI_H
