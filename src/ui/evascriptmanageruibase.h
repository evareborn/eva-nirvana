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

#ifndef EVASCRIPTMANAGERUIBASE_H
#define EVASCRIPTMANAGERUIBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QFrame;
class KListView;
class QListViewItem;

class EvaScriptManagerUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaScriptManagerUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EvaScriptManagerUIBase();

    QPushButton* btnInstall;
    QPushButton* btnUninstall;
    QFrame* line1;
    QPushButton* btnRun;
    QPushButton* btnStop;
    QPushButton* btnConfig;
    QPushButton* btnAbout;
    KListView* listView;

protected:
    QGridLayout* EvaScriptManagerUIBaseLayout;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // EVASCRIPTMANAGERUIBASE_H
