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
#include <qscrollview.h>
#include <qtoolbutton.h>

#include <kpushbutton.h>

#include "evauserinfowidget.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QTabWidget;
class QWidget;
class QPushButton;
class QLineEdit;
class QTextEdit;
class QCheckBox;

class EvaUserUIBase : public QDialog
{
    Q_OBJECT

public:
    EvaUserUIBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
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
    KPushButton* kpbUpdateShow;
    QToolButton* tbtnShop;

    //UserInfo ScrollView 
    QScrollView* svMain;
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
    QTextEdit* teMemoNote;
    QCheckBox* chbAutoUploadMemo;
    QPushButton* pbDownloadMemo;


protected:
    QGridLayout* layout13;
    QSpacerItem* spacer6;
    QGridLayout* layout12;
    QSpacerItem* spacer5;
    QSpacerItem* spacer7;
    QSpacerItem* spacer8;
    
    QGridLayout* TabMemoLayout;
    QSpacerItem* spacer30;
    QSpacerItem* spacer29;
    QVBoxLayout* layout117;
    QSpacerItem* spacer38;
    QHBoxLayout* layout100;
    QVBoxLayout* layout97;
    QVBoxLayout* layout99;
    QHBoxLayout* layout98;
    QVBoxLayout* layout80;
    QVBoxLayout* layout81;
    QVBoxLayout* layout82;
    QVBoxLayout* layout86;
    QHBoxLayout* layout112;
    QVBoxLayout* layout111;
    QSpacerItem* spacer32;
    QHBoxLayout* layout116;
    QSpacerItem* spacer34;
    QSpacerItem* spacer33;


protected slots:
    virtual void languageChange();

};

#endif // EVAUSERUIBASE_H
