/***************************************************************************
 *   Copyright (C) 2005 by casper                                          *
 *   tlmcasper@163.com                                                     *
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

#ifndef EVAUSERUIBASE_H
#define EVAUSERUIBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <q3scrollview.h>
#include <qtoolbutton.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

//X #include <kpushbutton.h>

#include "evauserinfowidget.h"

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QTabWidget;
class QWidget;
class QPushButton;
class QLineEdit;
class Q3TextEdit;
class QCheckBox;

class EvaUserUIBase : public QDialog
{
    Q_OBJECT

public:
    EvaUserUIBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~EvaUserUIBase();

    QPushButton* pbUpdate;
    QPushButton* pbClose;
    QLabel* lblText;
    QTabWidget* twTabMain;
    
    QWidget* tabQQShow;
   // QLabel* lblQQShowBar;   
    QLabel* lblQQShow;
    //QToolButton* tbtnHome;
    QToolButton* tbtnAlbum;
    QPushButton* kpbUpdateShow;
    QToolButton* tbtnShop;

    //UserInfo ScrollView 
    Q3ScrollView* svMain;
    EvaUserInfoWidget* qwUserInfo;
    
    //Memo
    QWidget* TabMemo;
    QLabel* lblMemoName;
    QLabel* lblMemoMobile;
    QLabel* lblMemoAddress;
    QLabel* lblMemoEmail;
    QLineEdit* leMemoName;
    QLineEdit* leMemoMobile;
    QLabel* lblMemoTelephone;
    QLabel* lblMemoZipCode;
    QLineEdit* leMemoTelephone;
    QLineEdit* leMemoZipCode;
    QLineEdit* leMemoAddress;
    QLineEdit* leMemoEmail;
    QLabel* lblMemoNote;
    Q3TextEdit* teMemoNote;
    QCheckBox* chbAutoUploadMemo;
    QPushButton* pbDownloadMemo;


protected:
    Q3GridLayout* layout13;
    QSpacerItem* spacer6;
    Q3GridLayout* layout12;
    QSpacerItem* spacer5;
    QSpacerItem* spacer7;
    QSpacerItem* spacer8;
    
    Q3GridLayout* TabMemoLayout;
    QSpacerItem* spacer30;
    QSpacerItem* spacer29;
    Q3VBoxLayout* layout117;
    QSpacerItem* spacer38;
    Q3HBoxLayout* layout100;
    Q3VBoxLayout* layout97;
    Q3VBoxLayout* layout99;
    Q3HBoxLayout* layout98;
    Q3VBoxLayout* layout80;
    Q3VBoxLayout* layout81;
    Q3VBoxLayout* layout82;
    Q3VBoxLayout* layout86;
    Q3HBoxLayout* layout112;
    Q3VBoxLayout* layout111;
    QSpacerItem* spacer32;
    Q3HBoxLayout* layout116;
    QSpacerItem* spacer34;
    QSpacerItem* spacer33;


protected slots:
    virtual void languageChange();

};

#endif // EVAUSERUIBASE_H
