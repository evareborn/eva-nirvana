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

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QToolButton;
class QLineEdit;
class QFrame;
class QPushButton;

class EvaLoginVeriUIBase : public QDialog
{
    Q_OBJECT

public:
    EvaLoginVeriUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EvaLoginVeriUIBase();

    QLabel* lblTopTip;
    QLabel* lblGraphicCode;
    QLabel* lblVeriCode;
    QLabel* lblGraphic;
    QToolButton* tbnChangeGraphic;
    QLineEdit* leCode;
    QFrame* line1;
    QPushButton* btnOK;
    QPushButton* btnCancel;

protected:
    QGridLayout* EvaLoginVeriUIBaseLayout;
    QVBoxLayout* layout8;
    QHBoxLayout* layout1;
    QSpacerItem* spacer1;
    QHBoxLayout* layout5;
    QVBoxLayout* layout2;
    QVBoxLayout* layout4;
    QHBoxLayout* layout3;
    QHBoxLayout* layout6;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // EVALOGINVERIUIBASE_H
