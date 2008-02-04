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

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QCheckBox;
class QLabel;
class QDateEdit;
class QPushButton;
class QTable;

class EvaHistoryUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaHistoryUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EvaHistoryUIBase();

    QCheckBox* chbSelect;
    QLabel* lblFrom;
    QDateEdit* deFrom;
    QLabel* lblTo;
    QDateEdit* deTo;
    QPushButton* pbShow;
    QTable* tblDisplay;
    QPushButton* pbPrev;
    QPushButton* pbNext;

protected:
    QGridLayout* EvaHistoryUIBaseLayout;
    QVBoxLayout* layout1;
    QHBoxLayout* layout2;
    QHBoxLayout* layout3;
    QSpacerItem* spacer1;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif 
