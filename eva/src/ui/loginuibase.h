/***************************************************************************
 *   Copyright (C) 2005 by yunfan                                          *
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

#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QToolButton;
class QGroupBox;
class QLabel;
class QComboBox;
class QLineEdit;
class QCheckBox;
class QFrame;

class LoginUIBase : public QDialog
{
    Q_OBJECT

public:
    LoginUIBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~LoginUIBase();

    QPushButton* cancelBtn;
    QToolButton* tbNetSetup;
    QGroupBox* groupBox1;
    QLabel* textLabel1;
    QLabel* textLabel2;
    QComboBox* cbQQ;
    QLineEdit* lePwd;
    QCheckBox* chbRecordPwd;
    QCheckBox* chbLoginMode;
    QFrame* line1;
    QLabel* lblType;
    QComboBox* cbbLoginType;
    QPushButton* loginBtn;
    QFrame* fraNetSetup;
    QLabel* tlProxyServer;
    QLineEdit* leIP;
    QLabel* tlProxyPort;
    QLineEdit* lePort;
    QLabel* tlUserName;
    QLineEdit* leUserName;
    QLabel* tlPassword;
    QLineEdit* lePassword;
    QLabel* lblLogo;

public slots:
    virtual void loginClickSlot();
    virtual void cancelClickSlot();

protected:
    QGridLayout* groupBox1Layout;
    QHBoxLayout* layout14;
    QSpacerItem* spacer3;
    QSpacerItem* spacer2;
    QVBoxLayout* layout9;
    QHBoxLayout* layout8;
    QVBoxLayout* layout7;
    QVBoxLayout* layout6;
    QHBoxLayout* layout1;
    QSpacerItem* spacer1;
    QGridLayout* fraNetSetupLayout;
    QVBoxLayout* layout13;
    QHBoxLayout* layout12;
    QHBoxLayout* layout11;

protected slots:
    virtual void languageChange();

};

#endif // FRMLOGIN_H
