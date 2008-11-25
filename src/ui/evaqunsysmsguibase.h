/***************************************************************************
 *   Copyright (C) 2004 by yunfan                                          *
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

#ifndef EVAQUNSYSMSGUIBASE_H
#define EVAQUNSYSMSGUIBASE_H

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3Frame>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class Q3Frame;
class QToolButton;
class QLabel;
class Q3ButtonGroup;
class QRadioButton;
class QLineEdit;
class QCheckBox;
class QPushButton;

class EvaQunSysMsgUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaQunSysMsgUIBase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~EvaQunSysMsgUIBase();

    Q3Frame* fraMain;
    QToolButton* tbQQ;
    QLabel* lblAction;
    QToolButton* tbQun;
    QLabel* lblMessage;
    Q3ButtonGroup* bgActions;
    QRadioButton* rbtnAccept;
    QRadioButton* rbtnReject;
    Q3Frame* fraMessage;
    QLineEdit* leMessage;
    QCheckBox* chbRejectForever;
    QPushButton* btnOk;
    QPushButton* btnClose;

protected:
    Q3GridLayout* EvaQunSysMsgUIBaseLayout;
    Q3VBoxLayout* layout12;
    Q3VBoxLayout* fraMainLayout;
    Q3VBoxLayout* layout6;
    Q3HBoxLayout* layout5;
    QSpacerItem* spacer3;
    Q3VBoxLayout* bgActionsLayout;
    Q3HBoxLayout* layout1;
    QSpacerItem* spacer1;
    Q3VBoxLayout* fraMessageLayout;
    Q3HBoxLayout* layout10;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // EVAQUNSYSMSGUIBASE_H
