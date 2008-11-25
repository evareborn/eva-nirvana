/***************************************************************************
 *   Copyright (C) 2007 by yunfan                                          *
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

#ifndef EVALOGINVERIUIBASE_H
#define EVALOGINVERIUIBASE_H

#include <qvariant.h>
#include <qdialog.h>
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
class QLabel;
class QToolButton;
class QLineEdit;
class Q3Frame;
class QPushButton;

class EvaLoginVeriUIBase : public QDialog
{
    Q_OBJECT

public:
    EvaLoginVeriUIBase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~EvaLoginVeriUIBase();

    QLabel* lblTopTip;
    QLabel* lblGraphicCode;
    QLabel* lblVeriCode;
    QLabel* lblGraphic;
    QToolButton* tbnChangeGraphic;
    QLineEdit* leCode;
    Q3Frame* line1;
    QPushButton* btnOK;
    QPushButton* btnCancel;

protected:
    Q3GridLayout* EvaLoginVeriUIBaseLayout;
    Q3VBoxLayout* layout8;
    Q3HBoxLayout* layout1;
    QSpacerItem* spacer1;
    Q3HBoxLayout* layout5;
    Q3VBoxLayout* layout2;
    Q3VBoxLayout* layout4;
    Q3HBoxLayout* layout3;
    Q3HBoxLayout* layout6;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // EVALOGINVERIUIBASE_H
