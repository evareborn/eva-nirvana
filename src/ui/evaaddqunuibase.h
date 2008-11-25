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

#ifndef EVAADDQUNUIBASE_H
#define EVAADDQUNUIBASE_H

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
class QToolButton;
class QLabel;
class Q3Frame;
class QLineEdit;
class Q3GroupBox;
class QPushButton;

class EvaAddQunUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaAddQunUIBase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~EvaAddQunUIBase();

    QToolButton* tbQunDetails;
    QLabel* lblQunIDTag;
    QLabel* lblCreatorTag;
    QLabel* lblQunExtID;
    QLabel* lblCreator;
    QLabel* lblQunNameTag;
    QLabel* lblQunName;
    QToolButton* tbCreatorDetails;
    Q3Frame* fraAuthContents;
    QLabel* lblDescription;
    Q3Frame* fraAuthGraphic;
    QLabel* lblCode;
    QLineEdit* leCode;
    QLabel* lblTip;
    QLabel* lblGraphic;
    QToolButton* tbRefresh;
    Q3GroupBox* gbMessage;
    QLineEdit* leMessage;
    QPushButton* btnSend;
    QPushButton* btnCancel;

protected:
    Q3GridLayout* EvaAddQunUIBaseLayout;
    Q3VBoxLayout* layout21;
    Q3HBoxLayout* layout15;
    Q3VBoxLayout* layout1;
    Q3VBoxLayout* layout2;
    Q3VBoxLayout* layout14;
    Q3HBoxLayout* layout13;
    QSpacerItem* spacer3;
    Q3HBoxLayout* layout4;
    QSpacerItem* spacer1;
    Q3VBoxLayout* fraAuthContentsLayout;
    Q3VBoxLayout* layout20;
    Q3VBoxLayout* fraAuthGraphicLayout;
    Q3VBoxLayout* layout19;
    Q3HBoxLayout* layout18;
    QSpacerItem* spacer4;
    Q3HBoxLayout* layout17;
    QSpacerItem* spacer5;
    Q3VBoxLayout* gbMessageLayout;
    Q3HBoxLayout* layout11;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // EVAADDQUNUIBASE_H
