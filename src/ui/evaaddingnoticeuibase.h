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

#ifndef EVAADDINGNOTICEUIBASE_H
#define EVAADDINGNOTICEUIBASE_H

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
class Q3Frame;
class QLabel;
class QToolButton;
class Q3ButtonGroup;
class QRadioButton;
class QLineEdit;
class QCheckBox;
class QPushButton;

class EvaAddingNoticeUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaAddingNoticeUIBase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~EvaAddingNoticeUIBase();

    Q3Frame* fraMain;
    QLabel* lblTitle;
    QLabel* lblMessageTag;
    QToolButton* tbFace;
    QLabel* lblMessage;
    Q3ButtonGroup* btngrpActions;
    QRadioButton* rbtnAcceptAdd;
    QRadioButton* rbtnAccept;
    QRadioButton* rbtnReject;
    Q3Frame* fraRejectContents;
    QLineEdit* leRejectMessage;
    QCheckBox* chbRejectForever;
    QToolButton* tbtnAuthSettings;
    QPushButton* btnOk;
    QPushButton* btnCancel;

protected:
    Q3GridLayout* EvaAddingNoticeUIBaseLayout;
    Q3VBoxLayout* layout15;
    QSpacerItem* spacer1;
    Q3GridLayout* fraMainLayout;
    Q3VBoxLayout* layout10;
    Q3HBoxLayout* layout9;
    QSpacerItem* spacer3;
    Q3VBoxLayout* layout4;
    Q3GridLayout* btngrpActionsLayout;
    Q3HBoxLayout* layout14;
    Q3GridLayout* fraRejectContentsLayout;
    Q3VBoxLayout* layout2;
    Q3HBoxLayout* layout3;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // EVAADDINGNOTICEUIBASE_H
