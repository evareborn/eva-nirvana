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

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QToolButton;
class QButtonGroup;
class QRadioButton;
class QLineEdit;
class QCheckBox;
class QPushButton;

class EvaAddingNoticeUIBase : public QWidget
{
    Q_OBJECT

public:
    EvaAddingNoticeUIBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EvaAddingNoticeUIBase();

    QFrame* fraMain;
    QLabel* lblTitle;
    QLabel* lblMessageTag;
    QToolButton* tbFace;
    QLabel* lblMessage;
    QButtonGroup* btngrpActions;
    QRadioButton* rbtnAcceptAdd;
    QRadioButton* rbtnAccept;
    QRadioButton* rbtnReject;
    QFrame* fraRejectContents;
    QLineEdit* leRejectMessage;
    QCheckBox* chbRejectForever;
    QToolButton* tbtnAuthSettings;
    QPushButton* btnOk;
    QPushButton* btnCancel;

protected:
    QGridLayout* EvaAddingNoticeUIBaseLayout;
    QVBoxLayout* layout15;
    QSpacerItem* spacer1;
    QGridLayout* fraMainLayout;
    QVBoxLayout* layout10;
    QHBoxLayout* layout9;
    QSpacerItem* spacer3;
    QVBoxLayout* layout4;
    QGridLayout* btngrpActionsLayout;
    QHBoxLayout* layout14;
    QGridLayout* fraRejectContentsLayout;
    QVBoxLayout* layout2;
    QHBoxLayout* layout3;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // EVAADDINGNOTICEUIBASE_H
