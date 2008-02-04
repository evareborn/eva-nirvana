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
#ifndef QUNCREATEUI_H
#define QUNCREATEUI_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QFrame;
class QWidgetStack;
class QLineEdit;
class QToolButton;
class QGroupBox;
class QTextEdit;
class QButtonGroup;
class QRadioButton;
class QTable;
class QPushButton;

class QunCreateUI : public QWidget
{
    Q_OBJECT

public:
    QunCreateUI( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~QunCreateUI();

    QLabel* lblTip;
    QFrame* line1;
    QWidgetStack* wsMain;
    QWidget* wsDetails;
    QLabel* lblName;
    QLineEdit* leQunName;
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
    QWidget* wsMembers;
    QTable* tblMembers;
    QPushButton* pbSetMembers;
    QPushButton* pbDelMembers;
    QPushButton* pbPrev;
    QPushButton* pbNext;
    QPushButton* pbCancel;

protected:
    QVBoxLayout* QunCreateUILayout;
    QGridLayout* wsDetailsLayout;
    QVBoxLayout* layout44;
    QHBoxLayout* layout40;
    QSpacerItem* spacer15;
    QHBoxLayout* layout41;
    QSpacerItem* spacer16;
    QGridLayout* gbNoticeLayout;
    QGridLayout* gbDescriptionLayout;
    QVBoxLayout* bgAuthenLayout;
    QGridLayout* wsMembersLayout;
    QHBoxLayout* layout43;
    QVBoxLayout* layout42;
    QSpacerItem* spacer17;
    QHBoxLayout* layout7;
    QSpacerItem* spacer18;

protected slots:
    virtual void languageChange();

};

#endif // QUNCREATEUI_H
