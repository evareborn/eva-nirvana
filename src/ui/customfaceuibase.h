 /***************************************************************************
 *   Copyright (C) 2006 by yunfan                                          *
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

#ifndef CUSTOMFACEUIBASE_H
#define CUSTOMFACEUIBASE_H

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <QLabel>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QTabWidget;
class QToolButton;
class QCheckBox;
class QLabel;

class CustomFaceUIBase : public QWidget
{
    Q_OBJECT

public:
    CustomFaceUIBase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~CustomFaceUIBase();

    QTabWidget* twSelector;
    QToolButton* tbAdd;
    QCheckBox* chbUseShortcut;
    QLabel* lblPage;
    QToolButton* tbPrev;
    QToolButton* tbNext;

protected:
    Q3GridLayout* CustomFaceUIBaseLayout;
    Q3VBoxLayout* layout2;
    Q3HBoxLayout* layout1;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // CUSTOMFACEUIBASE_H
