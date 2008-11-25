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
class Q3Frame;
class Q3WidgetStack;
class QLineEdit;
class QToolButton;
class Q3GroupBox;
class Q3TextEdit;
class Q3ButtonGroup;
class QRadioButton;
class Q3Table;
class QPushButton;

class QunCreateUI : public QWidget
{
    Q_OBJECT

public:
    QunCreateUI( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~QunCreateUI();

    QLabel* lblTip;
    Q3Frame* line1;
    Q3WidgetStack* wsMain;
    QWidget* wsDetails;
    QLabel* lblName;
    QLineEdit* leQunName;
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
    QWidget* wsMembers;
    Q3Table* tblMembers;
    QPushButton* pbSetMembers;
    QPushButton* pbDelMembers;
    QPushButton* pbPrev;
    QPushButton* pbNext;
    QPushButton* pbCancel;

protected:
    Q3VBoxLayout* QunCreateUILayout;
    Q3GridLayout* wsDetailsLayout;
    Q3VBoxLayout* layout44;
    Q3HBoxLayout* layout40;
    QSpacerItem* spacer15;
    Q3HBoxLayout* layout41;
    QSpacerItem* spacer16;
    Q3GridLayout* gbNoticeLayout;
    Q3GridLayout* gbDescriptionLayout;
    Q3VBoxLayout* bgAuthenLayout;
    Q3GridLayout* wsMembersLayout;
    Q3HBoxLayout* layout43;
    Q3VBoxLayout* layout42;
    QSpacerItem* spacer17;
    Q3HBoxLayout* layout7;
    QSpacerItem* spacer18;

protected slots:
    virtual void languageChange();

};

#endif // QUNCREATEUI_H
