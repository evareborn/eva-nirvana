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

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QToolButton;
class QLabel;
class QButtonGroup;
class QRadioButton;
class QLineEdit;
class QCheckBox;
class QPushButton;

class EvaQunSysMsgUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaQunSysMsgUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EvaQunSysMsgUIBase();

    QFrame* fraMain;
    QToolButton* tbQQ;
    QLabel* lblAction;
    QToolButton* tbQun;
    QLabel* lblMessage;
    QButtonGroup* bgActions;
    QRadioButton* rbtnAccept;
    QRadioButton* rbtnReject;
    QFrame* fraMessage;
    QLineEdit* leMessage;
    QCheckBox* chbRejectForever;
    QPushButton* btnOk;
    QPushButton* btnClose;

protected:
    QGridLayout* EvaQunSysMsgUIBaseLayout;
    QVBoxLayout* layout12;
    QVBoxLayout* fraMainLayout;
    QVBoxLayout* layout6;
    QHBoxLayout* layout5;
    QSpacerItem* spacer3;
    QVBoxLayout* bgActionsLayout;
    QHBoxLayout* layout1;
    QSpacerItem* spacer1;
    QVBoxLayout* fraMessageLayout;
    QHBoxLayout* layout10;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // EVAQUNSYSMSGUIBASE_H
