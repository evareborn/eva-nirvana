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
class QPushButton;
class QToolButton;
class Q3GroupBox;
class QLabel;
class QComboBox;
class QLineEdit;
class QCheckBox;
class Q3Frame;

class LoginUIBase : public QDialog
{
    Q_OBJECT

public:
    LoginUIBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~LoginUIBase();

    QPushButton* cancelBtn;
    QToolButton* tbNetSetup;
    Q3GroupBox* groupBox1;
    QLabel* textLabel1;
    QLabel* textLabel2;
    QComboBox* cbQQ;
    QLineEdit* lePwd;
    QCheckBox* chbRecordPwd;
    QCheckBox* chbLoginMode;
    Q3Frame* line1;
    QLabel* lblType;
    QComboBox* cbbLoginType;
    QPushButton* loginBtn;
    Q3Frame* fraNetSetup;
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
    Q3GridLayout* groupBox1Layout;
    Q3HBoxLayout* layout14;
    QSpacerItem* spacer3;
    QSpacerItem* spacer2;
    Q3VBoxLayout* layout9;
    Q3HBoxLayout* layout8;
    Q3VBoxLayout* layout7;
    Q3VBoxLayout* layout6;
    Q3HBoxLayout* layout1;
    QSpacerItem* spacer1;
    Q3GridLayout* fraNetSetupLayout;
    Q3VBoxLayout* layout13;
    Q3HBoxLayout* layout12;
    Q3HBoxLayout* layout11;

protected slots:
    virtual void languageChange();

};

#endif // FRMLOGIN_H
