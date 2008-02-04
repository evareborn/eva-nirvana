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
#ifndef QUNCATEGORYUI_H
#define QUNCATEGORYUI_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QComboBox;
class QPushButton;

class QunCategoryUI : public QWidget
{
    Q_OBJECT

public:
    QunCategoryUI( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~QunCategoryUI();

    QFrame* frame3;
    QComboBox* cbbTop;
    QComboBox* cbbSecond;
    QComboBox* cbbThird;
    QPushButton* pbCancel;
    QPushButton* pbOK;

protected:
    QGridLayout* QunCategoryUILayout;
    QVBoxLayout* layout5;
    QGridLayout* frame3Layout;
    QVBoxLayout* layout3;
    QHBoxLayout* layout4;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // QUNCATEGORYUI_H
