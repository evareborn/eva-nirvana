/***************************************************************************
 *   Copyright (C) 2004 by zsyddl                                          *
 *   morrowren@sina.com                                                     *
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
/*
	revised by yunfan  2005
	           tlmcasper 2005
	yunfan_zg@163.com
	tlmcasper@163.com
*/

#include "evadetailswindow.h"

#include "evaresource.h"
#include "../evamain.h"
#include "evapacket.h"
#include "evauser.h"
#include "evautil.h"
#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qiconview.h>
#include <qradiobutton.h>
#include <qtextedit.h>
#include <qpixmap.h>
#include <qgroupbox.h>
#include <qmessagebox.h>
#include <qtimer.h>
#include <qstringlist.h>
#include <qtoolbutton.h>
#include <qtextcodec.h>
#include <kpushbutton.h>
#include <krun.h>
#include <klocale.h>
#include <kapplication.h>
#include <qtabwidget.h>
#include <qcheckbox.h>

//#include "../usermsg.ui.h"
/*
 *  Constructs a EvaDetailsWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
 
EvaImageResource *EvaDetailsWindow::faces = NULL;
EvaDetailsWindow::EvaDetailsWindow(  QStringList &user, QWidget* parent, const char* name, bool modal, WFlags fl)
	: EvaUserUIBase( parent, name, modal, fl ),
	details( user ),selectedFaceIndex( 0 )
{
  
        
	QObject::connect( pbUpdate, SIGNAL( clicked() ), this, SLOT( slotUpdateClick() ) );	//Click the update button
	QObject::connect( pbClose, SIGNAL( clicked() ), this, SLOT( close() ) );	//Click the close button
	
	QObject::connect( tbtnShop, SIGNAL( clicked() ), this, SLOT( slotShopClicked() ) );
	QObject::connect( tbtnAlbum, SIGNAL( clicked() ), this, SLOT( slotAlbumClicked() ) );
	
	QObject::connect( kpbUpdateShow, SIGNAL( clicked() ), this, SLOT( slotUpdateQQShow() ) );
	QObject::connect( twTabMain, SIGNAL(currentChanged(QWidget*)), this, SLOT( slotCurrentChanged(QWidget*)));
	
	QObject::connect( pbDownloadMemo, SIGNAL( clicked() ), this, SLOT( slotDownloadMemo() ));
	//QObject::connect( tabWidget, SIGNAL(currentChanged(QWidget*)), this, SLOT(currentChanged(QWidget*)) );
	
	m_IsModifyMemo = false;
	codec = QTextCodec::codecForName("GB18030");
	
	bool ok;
	id = details[0].toUInt(&ok, 10);
	if(!ok) return;
	frd = (EvaMain::user->getFriendList()).getFriend(id); //get the user qq of this details window
	
	setCaption( i18n( "View User Info") );//set the title of this dialog

	qwUserInfo->lblBasicInfo->setPixmap(*faces->getIcon("DETAILS_BASIC_INFO"));
	qwUserInfo->lblDetails->setPixmap(*faces->getIcon("DETAILS_DETAILS"));
	qwUserInfo->lblDescription->setPixmap(*faces->getIcon("DETAILS_DESCRIPTION"));
	
	chbAutoUploadMemo->setChecked(true);
	if(!frd){ // He/She is not in your buddy list
		twTabMain->setTabEnabled(twTabMain->page(2), false);
	}
	if(faces){
		QPixmap *icon = faces->getIcon("QQ_SHOP");
		if(icon)
			tbtnShop->setIconSet(QIconSet(*icon));// add Icon to QQShow shop button
	} 
	if(faces){
		QPixmap *icon = faces->getIcon("QQ_ALBUM");
		if(icon)
			tbtnAlbum->setIconSet(QIconSet(*icon));
	}
	
	UpdateData(true);
	if(details.size()!=37) {
		QTimer::singleShot(500, this, SLOT(slotFirstUpdate()));
	}
	
	
}

EvaDetailsWindow::~EvaDetailsWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void EvaDetailsWindow::slotFirstUpdate()
{
	emit requestUpdate(id);
}

//slot of the update button click envent
void EvaDetailsWindow::slotUpdateClick()
{
	if(!frd) return;
	if( m_IsMemoPage ){		//if the user is in Memo page and the id is set
		//if the auto uplad checkbutton selected
		m_Memo.name = codec->fromUnicode(leMemoName->text()).data();
		m_Memo.mobile = codec->fromUnicode(leMemoMobile->text()).data();
		m_Memo.telephone = codec->fromUnicode(leMemoTelephone->text()).data();
		m_Memo.address = codec->fromUnicode(leMemoAddress->text()).data();
		m_Memo.email = codec->fromUnicode(leMemoEmail->text()).data();
		m_Memo.zipcode = codec->fromUnicode(leMemoZipCode->text()).data();
		m_Memo.note = codec->fromUnicode(teMemoNote->text()).data();
		if(chbAutoUploadMemo->isChecked()){
			emit memoChanged(frd->getQQ(), m_Memo);
			emit requestUploadMemo(frd->getQQ(), m_Memo);
		}
		else{
			//save memo to local file
			
			emit memoChanged(frd->getQQ(), m_Memo);
			QMessageBox::information(this, i18n( "Store Memo"), i18n( "Store memo successfully"));
		}	
		
	}
	else{
		emit requestUpdate(id);
	}
}

void EvaDetailsWindow::slotDetailsUpdated( QStringList list)
{
	if(details[0] != list[0] ) return;
	details = list;
	UpdateData(true);
}	
	
const bool EvaDetailsWindow::UpdateData(const bool toShow)
{
	qwUserInfo->leNumber->setText( details[0] );
	
	if(details.size()!=37) return false;
	
	slotBuddyQQShowReady(id);
	
	int level = 0;
	QString signature = "";
	if(id == EvaMain::user->getQQ() ){
		level = EvaMain::user->getLevel();
		signature = codec->toUnicode(EvaMain::user->getSignature().c_str());
	}else{
		if(frd){
			level = frd->getLevel();
			signature = codec->toUnicode(frd->getSignature().c_str());
			m_Memo = frd->getMemo();
			slotUpdateMemo(m_Memo);
		}
	}
	int suns, moons, stars;
	EvaUtil::calcSuns(level, &suns, &moons, &stars);
	QString strSun =  "<img src=\"" + faces->getIconFullPath("TIME_SUN") + "\"></img>";
	QString strMoon = "<img src=\"" + faces->getIconFullPath("TIME_MOON") + "\"></img>";
	QString strStar = "<img src=\"" + faces->getIconFullPath("TIME_STAR") + "\"></img>";
	QString strLevel;
	for(int i=0; i<suns; i++){
		strLevel += strSun;
	}
	for(int i=0; i<moons; i++){
		strLevel += strMoon;
	}
	for(int i=0; i<stars; i++){
		strLevel += strStar;
	}
	qwUserInfo->lblLevelDisplay->setText("(" + QString::number(level) + ")" + strLevel);
	qwUserInfo->lblLevelDisplay->adjustSize();
	//lE_Num->setReadOnly(true);
	if (toShow)
	{
		qwUserInfo->leNumber->setText( details[0] );			
		qwUserInfo->leNickName->setText( details[1]);
		qwUserInfo->leAge->setText( details[7] );
		qwUserInfo->leSex->setText( details[8] );
		qwUserInfo->leArea->setText( details[2] );
		qwUserInfo->leProvince->setText( details[3] ); 
		qwUserInfo->leCity->setText( details[25] );
		
		bool ok;
		int faceId = details[21].toInt(&ok, 10);
		if(ok){
			selectedFaceIndex = faceId;
			QPixmap *pix = faces->getUserHeadPixmap(id);
			if(!pix) pix = faces->getFace(faces->getFaceFileIndex(faceId));
			if(pix) qwUserInfo->lblFace->setPixmap( *pix );
		}else
			selectedFaceIndex = 0;

		qwUserInfo->leEmail->setText( details[10] );
		qwUserInfo->leAddress->setText( details[5] );
		qwUserInfo->leZipCode->setText( details[4] );
        	qwUserInfo->lePhone->setText( details[6] );
		qwUserInfo->leMobile->setText( details[22]);
		
		qwUserInfo->leRealName->setText( details[9]);
        	qwUserInfo->leSchool->setText( details[31] );
		qwUserInfo->leOccupation->setText( details[16] );
		qwUserInfo->leHomePage->setText( details[17] );
		qwUserInfo->teAboutMe->setText( details[24] );
		qwUserInfo->teSignature->setText(signature);
		
		
		QStringList ZodiacList( QString::null );
		QStringList BloodList( QString::null );
		QStringList HoroscapeList( QString::null );
		
		
		ZodiacList += ( i18n( "Rat" ) );
		ZodiacList += ( i18n( "Ox" ) );
		ZodiacList += ( i18n("Tiger" ) );
		ZodiacList += ( i18n("Rabbit" ) );
		ZodiacList += ( i18n("Dragon" ) );
		ZodiacList += ( i18n("Snake" ) );
		ZodiacList += ( i18n("Horse" ) );
		ZodiacList += ( i18n("Sheep" ) );
		ZodiacList += ( i18n("Monkey" ) );
		ZodiacList += ( i18n("Rooster" ) );
		ZodiacList += ( i18n("Dog" ) );
		ZodiacList += ( i18n("Pig" ) );
		
		BloodList += ( i18n("A" ) );
		BloodList += ( i18n("B" ) );
		BloodList += ( i18n("O" ) );
		BloodList += ( i18n("AB" ) );
		BloodList += ( i18n("Other" ) );
		
		HoroscapeList += ( i18n("Aquarius" ) );
		HoroscapeList += ( i18n("Pisces" ) );
		HoroscapeList += ( i18n("Aries" ) );
		HoroscapeList += ( i18n("Taurus" ) );
		HoroscapeList += ( i18n("Gemini" ) );
		HoroscapeList += ( i18n("Cancer" ) );
		HoroscapeList += ( i18n("Leo" ) );
		HoroscapeList += ( i18n("Virgo" ) );
		HoroscapeList += ( i18n("Libra" ) );
		HoroscapeList += ( i18n("Scoripio" ) );
		HoroscapeList += ( i18n("Sagittarius" ) );
		HoroscapeList += ( i18n("Capricorn" ) );

		qwUserInfo->leZodiac->setText( ZodiacList[ details[33].toInt()<0 ? 0: details[33].toInt()  ] ); 
		qwUserInfo->leBlood->setText( BloodList[ details[34].toInt() <0 ? 0: details[34].toInt() ] );
		qwUserInfo->leHoroscape->setText( HoroscapeList[ details[32].toInt() <0 ? 0: details[32].toInt() ] );
	}
	return true;
}

void EvaDetailsWindow::slotLinkClicked( const QString & url )
{
	QStringList args;
	args<<"exec"<< url;
	//kapp->kdeinitExec("kfmclient",args);
	KRun::runCommand("konqueror \""+url+"\"");
}

void EvaDetailsWindow::slotShopClicked( )
{
	QString url = "http://jump.qq.com/clienturl_simp_17?clientuin=" + QString::number(EvaMain::user->getQQ());
	url+="&clientkey=";
	url+=getClientKeyString();
	slotLinkClicked(url);
}

void EvaDetailsWindow::slotAlbumClicked( )
{
	QString url = "http://ptlogin.qq.com/qqshowalbum?clientuin=" + QString::number(EvaMain::user->getQQ());
	url+="&clientkey=";
	url+=getClientKeyString();
	slotLinkClicked(url);
}

const QString EvaDetailsWindow::getClientKeyString()
{
	QString key="";
	int len = Packet::getClientKeyLength();
	for(int i=0; i<len; i++){
		QString tmp = QString::number((unsigned char)(Packet::getClientKey()[i]), 16);
		if(tmp.length()==1)
			tmp = "0" + tmp;
		key+=tmp;
	}
	return key.upper();
}

void EvaDetailsWindow::slotUpdateQQShow()
{
	emit requestQQShow(id);
}

void EvaDetailsWindow::slotBuddyQQShowReady(const unsigned int id)
{
	bool ok;
	unsigned int qq = details[0].toUInt(&ok, 10);
	if(!ok) return;
	if(id != qq) return;
	QPixmap *pix = faces->getQQShow(id);
	if(pix){
		lblQQShow->setPixmap( *pix);
	}
}

void EvaDetailsWindow::slotCurrentChanged( QWidget *wCurrentPage)
{
	if(!strcmp(wCurrentPage->name(), "TabMemo")){
		m_IsMemoPage = true;
		if(!frd) return;
		slotUpdateMemo( frd->getMemo() );
		pbUpdate->setText( i18n("Modify") );
	}
	else{
		m_IsMemoPage = false;
		pbUpdate->setText( i18n("Update"));
	}
}

void EvaDetailsWindow::slotDownloadMemo()
{
	if(!frd) return;
	emit requestDownloadMemo(frd->getQQ());
}

void EvaDetailsWindow::slotUpdateMemo(const MemoItem &memo)
{
	leMemoName->setText( codec->toUnicode(memo.name.c_str()) );
	leMemoMobile->setText( codec->toUnicode(memo.mobile.c_str()) );
	leMemoTelephone->setText( codec->toUnicode(memo.telephone.c_str()) );
	leMemoAddress->setText( codec->toUnicode(memo.address.c_str()) );
	leMemoEmail->setText( codec->toUnicode(memo.email.c_str()) );
	leMemoZipCode->setText( codec->toUnicode(memo.zipcode.c_str()) );
	teMemoNote->setText( codec->toUnicode(memo.note.c_str()) );
}

void EvaDetailsWindow::slotUploadMemoReply(const bool flag)
{
	if(flag)
		QMessageBox::information(this, i18n( "Upload Memo"), i18n( "upload memo successfully"));
	else
		QMessageBox::information(this, i18n( "Upload Memo"), i18n( "failed to upload memo."));
	
}

void EvaDetailsWindow::slotNoMemoFound()
{
	QMessageBox::information(this, i18n( "Downlad Memo"), i18n( "no memo found on the server"));
}

void EvaDetailsWindow::closeEvent( QCloseEvent * /*e*/ )
{
	deleteLater();
}

