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

#ifndef EVAADDUIBASE_H
#define EVAADDUIBASE_H

#include <qvariant.h>
#include <qwidget.h>


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QFrame;
class QToolButton;
class QComboBox;
class QLineEdit;
class QCheckBox;
class QPushButton;

class EvaAddUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaAddUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EvaAddUIBase();

    QLabel* lblTip;
    QFrame* frame3;
    QLabel* lblTopic;
    QToolButton* tbtnFace;
    QLabel* lblGroup;
    QComboBox* cbbGroups;
    QFrame* fraVeriGraphic;
    QLabel* lblVeriCode;
    QLineEdit* leCode;
    QLabel* lblCodeGraphic;
    QToolButton* btnRefreshCode;
    QFrame* fraQuestion;
    QLabel* lblQuestion;
    QLabel* lblAnswer;
    QLabel* lblQuesContents;
    QLineEdit* leAnswer;
    QLineEdit* leMessage;
    QCheckBox* chbReverse;
    QPushButton* btnOk;
    QPushButton* btnCancel;

protected:
    QVBoxLayout* EvaAddUIBaseLayout;
    QVBoxLayout* frame3Layout;
    QHBoxLayout* layout17;
    QHBoxLayout* layout13;
    QSpacerItem* spacer1;
    QGridLayout* fraVeriGraphicLayout;
    QHBoxLayout* layout14;
    QHBoxLayout* fraQuestionLayout;
    QVBoxLayout* layout12;
    QVBoxLayout* layout13_2;
    QHBoxLayout* layout13_3;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};
#endif // EVAADDUIBASE_H
