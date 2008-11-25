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

#ifndef EVAHISTORYUIBASE_H
#define EVAHISTORYUIBASE_H

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QCheckBox;
class QLabel;
class Q3DateEdit;
class QPushButton;
class Q3Table;

class EvaHistoryUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaHistoryUIBase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~EvaHistoryUIBase();

    QCheckBox* chbSelect;
    QLabel* lblFrom;
    Q3DateEdit* deFrom;
    QLabel* lblTo;
    Q3DateEdit* deTo;
    QPushButton* pbShow;
    Q3Table* tblDisplay;
    QPushButton* pbPrev;
    QPushButton* pbNext;

protected:
    Q3GridLayout* EvaHistoryUIBaseLayout;
    Q3VBoxLayout* layout1;
    Q3HBoxLayout* layout2;
    Q3HBoxLayout* layout3;
    QSpacerItem* spacer1;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif 
