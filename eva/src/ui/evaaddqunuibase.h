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

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QToolButton;
class QLabel;
class QFrame;
class QLineEdit;
class QGroupBox;
class QPushButton;

class EvaAddQunUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaAddQunUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EvaAddQunUIBase();

    QToolButton* tbQunDetails;
    QLabel* lblQunIDTag;
    QLabel* lblCreatorTag;
    QLabel* lblQunExtID;
    QLabel* lblCreator;
    QLabel* lblQunNameTag;
    QLabel* lblQunName;
    QToolButton* tbCreatorDetails;
    QFrame* fraAuthContents;
    QLabel* lblDescription;
    QFrame* fraAuthGraphic;
    QLabel* lblCode;
    QLineEdit* leCode;
    QLabel* lblTip;
    QLabel* lblGraphic;
    QToolButton* tbRefresh;
    QGroupBox* gbMessage;
    QLineEdit* leMessage;
    QPushButton* btnSend;
    QPushButton* btnCancel;

protected:
    QGridLayout* EvaAddQunUIBaseLayout;
    QVBoxLayout* layout21;
    QHBoxLayout* layout15;
    QVBoxLayout* layout1;
    QVBoxLayout* layout2;
    QVBoxLayout* layout14;
    QHBoxLayout* layout13;
    QSpacerItem* spacer3;
    QHBoxLayout* layout4;
    QSpacerItem* spacer1;
    QVBoxLayout* fraAuthContentsLayout;
    QVBoxLayout* layout20;
    QVBoxLayout* fraAuthGraphicLayout;
    QVBoxLayout* layout19;
    QHBoxLayout* layout18;
    QSpacerItem* spacer4;
    QHBoxLayout* layout17;
    QSpacerItem* spacer5;
    QVBoxLayout* gbMessageLayout;
    QHBoxLayout* layout11;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // EVAADDQUNUIBASE_H
