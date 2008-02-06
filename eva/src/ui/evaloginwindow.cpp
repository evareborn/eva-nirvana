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
#include "evaloginwindow.h"

#include "evasetting.h"
#include "evautil.h"
#include "qmdcodec.h"

#include <qpixmap.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qhostaddress.h>
#include <klocale.h>

EvaLoginWindow::EvaLoginWindow(QWidget* parent, const char* name, bool modal, WFlags fl)
    : LoginUIBase(parent,name, modal,fl), qqNum(0), port(0)
{
	QObject::connect(tbNetSetup,SIGNAL(toggled(bool)),SLOT(showNetSetup(bool)));
	QObject::connect(cbQQ,SIGNAL(activated(int)),SLOT(slotSelectChanged(int)));
	QObject::connect(lePwd,SIGNAL(textChanged(const QString &)),SLOT(slotPasswordChanged(const QString &)));
	QObject::connect(cbbLoginType,SIGNAL(activated(int)),SLOT(slotLoginTypeChanged(int)));
	QObject::connect(leUserName,SIGNAL(textChanged(const QString &)),SLOT(slotProxyUserChanged(const QString &)));
	QObject::connect(lePassword,SIGNAL(textChanged(const QString &)),SLOT(slotProxyPasswordChanged(const QString &)));
	
	md5Pwd[0] = 0x00;
	setting=NULL;
	showNetSetup(false);
	proxyPwdChanged = false;
	pwdChanged = false;
	slotLoginTypeChanged(0);
}

EvaLoginWindow::~EvaLoginWindow()
{
}

void EvaLoginWindow::setLogo(const QPixmap *logo)
{
	if(!logo) return;
	lblLogo->setPixmap(*logo);
}

void EvaLoginWindow::setEvaSetting(EvaSetting *s)
{
	setting = s;
	iniRecords( );
}

const int EvaLoginWindow::getQQ() const
{
	return qqNum;
}

const QString EvaLoginWindow::getPassword() const
{
	return lePwd->text();
}

const bool EvaLoginWindow::isHiddenLoginMode() const
{
	return chbLoginMode->isOn();
}

const bool EvaLoginWindow::isRememberChecked() const
{
	return chbRecordPwd->isOn();
}

const char * EvaLoginWindow::getMd5Password() const
{	
	return md5Pwd;
}

const EvaLoginWindow::Type EvaLoginWindow::getConnectionType() const
{
	int index = cbbLoginType->currentItem();
	if(index == 0) return UDP;
	if(index == 1) return TCP;
	if(index == 2) return HTTP_Proxy;
	return UDP;
}

const QString EvaLoginWindow::getProxyIP() const
{
	return leIP->text();
}

const int EvaLoginWindow::getProxyPort() const
{
	return port;
}

const QString EvaLoginWindow::getProxyUserName() const
{
	return leUserName->text();
}

const QString EvaLoginWindow::getProxyPassword() const
{
	return lePassword->text();
}

const QCString EvaLoginWindow::getProxyParam()
{
	return proxyParam;
}

void EvaLoginWindow::cancelClickSlot()
{
	emit doCancel();
}

void EvaLoginWindow::loginClickSlot()
{
	bool qqok, portok;
	qqNum = cbQQ->currentText().toInt(&qqok, 10);
	if(!qqok){
		QMessageBox::information(this, i18n( "Eva Login"), 
			i18n( "Sorry, account is not an integer number."));
		cbQQ->setFocus();
		return;
	}
	if(getConnectionType() == HTTP_Proxy){
		port = lePort->text().toInt(&portok, 10);
		if(!portok){
			QMessageBox::information(this, i18n( "Eva Login"), 
				i18n( "Sorry, proxy server port is not an integer number."));
			lePort->setFocus();
			return;
		}
	}
	proxyPwdChanged = false;
	pwdChanged = false;
	emit doLogin();
}

void EvaLoginWindow::showNetSetup(bool on)
{
	if(on){
		setMinimumHeight (330);
		setMaximumHeight (330);
	}else{
		setMinimumHeight (205);
		setMaximumHeight (205);
	}
}

void EvaLoginWindow::iniRecords( )
{
	proxyPwdChanged = false;
	pwdChanged = false;
	if(!setting) return;
	if(!setting->loadSetting())
		return;
	cbQQ->clear();
	for(uint i=0; i<setting->getRecordedList().count();i++){
		int id = setting->getRecordedList().at(i)->id;
		cbQQ->insertItem(QString::number(id));
	}
	cbQQ->setCurrentItem(setting->getLastUserIndex());

	currentIndex = setting->getLastUserIndex();
	setupOtherSetings(currentIndex);
}


void EvaLoginWindow::slotIDChanged( const QString &)
{
}


void EvaLoginWindow::slotSelectChanged( int index)
{
	if((uint)(index) >= setting->getRecordedList().count()) return;
	currentIndex = index;	
 	setupOtherSetings( currentIndex);
	proxyPwdChanged = false;
	pwdChanged = false;
}


void EvaLoginWindow::slotPasswordChanged( const QString &newPwd)
{
	if(newPwd.isEmpty()) return;
	int pwdLen = strlen(newPwd.ascii());
	char *pwd = new char[pwdLen+1];
	memcpy(pwd, newPwd.ascii(), pwdLen);
	pwd[pwdLen]=0x00;
	memcpy(md5Pwd, EvaUtil::doMd5Md5(pwd, pwdLen), 16);	
	delete pwd;
}

void EvaLoginWindow::slotProxyUserChanged(const QString &)
{
	updateProxyLoginParam();
	lePassword->setText("");
}

void EvaLoginWindow::slotProxyPasswordChanged(const QString &)
{
	if(proxyPwdChanged){
		proxyPwdChanged = false;
		return;
	}
	updateProxyLoginParam();
}

void EvaLoginWindow::updateProxyLoginParam()
{
	QCString para = (leUserName->text() + ':' + lePassword->text()).local8Bit();
	proxyParam = QCodecs::base64Encode(para);
}

void EvaLoginWindow::setupOtherSetings( int index )
{
	if(!setting) return;
	if((uint)(index) >= setting->getRecordedList().count()) return;
		
	QString sid = cbQQ->text(index);
	bool ok;
	int id = sid.toInt(&ok);
	if(!ok) return;
		
	if(setting->isHiddenChecked(id))
		chbLoginMode->setChecked(true);
	else
		chbLoginMode->setChecked(false);

	cbQQ->setFocus();		
	
	if(setting->isRememberChecked(id)){ 
		chbRecordPwd->setChecked(true);
		lePwd->setText("@@@@@@@@");
		memcpy(md5Pwd, setting->getPassword(id), 16);
		pwdChanged = true;
	}else{
		chbRecordPwd->setChecked(false);
		lePwd->setText("");
		md5Pwd[0]=0;
		
		cbbLoginType->setCurrentItem(0);		
		leIP->setText("");
		lePort->setText("");
		leUserName->setText("");
		lePassword->setText("");
		proxyParam="";
		return;
	}
	
	switch(setting->getConnectType(id)){
	case 0:
		cbbLoginType->setCurrentItem(0);
		break;
	case 1:
		cbbLoginType->setCurrentItem(1);
		break;
	case 2:
		cbbLoginType->setCurrentItem(2);
		break;
	default:
		cbbLoginType->setCurrentItem(0);
		
	}
	
	if(setting->getConnectType(id) == 2){
		leIP->setText(QHostAddress(setting->getServer(id)).toString());
		lePort->setText(QString::number(setting->getPort(id)) );
		leUserName->setText(setting->getProxyUsername(id));		
		lePassword->setText("@@@@@@@@");
		proxyParam = setting->getProxyParam(id);
		proxyPwdChanged = true;
	}
	
	//lePwd->setFocus(); // finally, set focus in the password item
	cbQQ->setFocus();  // set focus in the QQ number item
}

void EvaLoginWindow::slotLoginTypeChanged( int index)
{
	switch(index){
	case 0:
	case 1:
		fraNetSetup->setEnabled(false);
		break;
	case 2:
		fraNetSetup->setEnabled(true);
		break;
	default:
		fraNetSetup->setEnabled(false);
	}
}


