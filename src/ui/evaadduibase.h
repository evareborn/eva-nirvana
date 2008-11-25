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
class Q3Frame;
class QToolButton;
class QComboBox;
class QLineEdit;
class QCheckBox;
class QPushButton;

class EvaAddUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaAddUIBase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~EvaAddUIBase();

    QLabel* lblTip;
    Q3Frame* frame3;
    QLabel* lblTopic;
    QToolButton* tbtnFace;
    QLabel* lblGroup;
    QComboBox* cbbGroups;
    Q3Frame* fraVeriGraphic;
    QLabel* lblVeriCode;
    QLineEdit* leCode;
    QLabel* lblCodeGraphic;
    QToolButton* btnRefreshCode;
    Q3Frame* fraQuestion;
    QLabel* lblQuestion;
    QLabel* lblAnswer;
    QLabel* lblQuesContents;
    QLineEdit* leAnswer;
    QLineEdit* leMessage;
    QCheckBox* chbReverse;
    QPushButton* btnOk;
    QPushButton* btnCancel;

protected:
    Q3VBoxLayout* EvaAddUIBaseLayout;
    Q3VBoxLayout* frame3Layout;
    Q3HBoxLayout* layout17;
    Q3HBoxLayout* layout13;
    QSpacerItem* spacer1;
    Q3GridLayout* fraVeriGraphicLayout;
    Q3HBoxLayout* layout14;
    Q3HBoxLayout* fraQuestionLayout;
    Q3VBoxLayout* layout12;
    Q3VBoxLayout* layout13_2;
    Q3HBoxLayout* layout13_3;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};
#endif // EVAADDUIBASE_H
