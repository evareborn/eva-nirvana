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
 /*
	revised by tlmcasper 2005
	tlmcasper@163.com
*/
#include "evasyssettingwindow.h"
#include "evaresource.h"
#include "evausersetting.h"

#include "../evamain.h"
#include "evapacket.h"
#include "evauser.h"
#include "evautil.h"

#include <qcombobox.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qfiledialog.h>
#include <klocale.h>
#include <inttypes.h>

#include <qlistview.h>
#include <qheader.h>
#include <qwidgetstack.h>
#include <qiconview.h>
#include <qradiobutton.h>
#include <qtextcodec.h>
#include <qtimer.h>
#include <qtoolbutton.h>
#include <krun.h>
#include <kpushbutton.h>
#include <kcolorcombo.h>
#include <kmessagebox.h>
#include <kapp.h>

EvaImageResource *EvaSysSettingWindow::faces = NULL;
EvaSysSettingWindow::EvaSysSettingWindow( QStringList &user, EvaImageResource * res, EvaUserSetting * setting , QWidget *parent)
	: EvaSysSettingUIBase(parent, 0, WType_Dialog | WShowModal),
	details(user), selectedFaceIndex(0), isSignatureChanged(false)
{
	userSetting = setting;
	lvSettingItem->setSorting( -1 ); //make the listview do NOT Sort
	lvSettingItem->header()->setHidden( TRUE );//make the listview title hidden
	
	pbFaceDefault->hide();
		
	faces = res; //faces is a pointer of EvaImageResource
	setupFaces();
	
	loadSetting();
	slotCbbAutoNoActivated(0);
	slotCbbQuickNoActivated(0);
	slotAuthChanged();

	QObject::connect(pbFaceDefault, SIGNAL(clicked()), SLOT(slotPbFaceDefaultClicked()));
	
	QObject::connect(pbApply, SIGNAL(clicked()), SLOT(slotPbApplyClicked()));
	QObject::connect(pbCancel, SIGNAL(clicked()), SLOT(close()));
	QObject::connect(pbOk, SIGNAL(clicked()), SLOT(slotPbOKClicked()));
	QObject::connect(sliderFaceSize, SIGNAL(valueChanged(int)), SLOT(slotFaceSizeChanged(int)));
	
	QObject::connect(kkbtnShortKey, SIGNAL(capturedShortcut(const KShortcut &)), SLOT(slotCapturedShortcut(const KShortcut &)));
	
	QObject::connect(pbTheme, SIGNAL(clicked()), SLOT(slotPbThemeClicked()));
	QObject::connect(pbSound, SIGNAL(clicked()), SLOT(slotPbSoundClicked()));
	
	QObject::connect(cbbAutoNo, SIGNAL(activated(int)), SLOT(slotCbbAutoNoActivated(int)));
	QObject::connect(pbAutoNew, SIGNAL(clicked()), SLOT(slotPbAutoNewClicked()));
	QObject::connect(pbAutoDelete, SIGNAL(clicked()), SLOT(slotPbAutoDeleteClicked()));
	QObject::connect(teAutoMessage, SIGNAL(textChanged()), SLOT(slotTeAutoTextChanged()));
	
	QObject::connect(cbbQuickNo, SIGNAL(activated(int)), SLOT(slotCbbQuickNoActivated(int)));
	QObject::connect(pbQuickNew, SIGNAL(clicked()), SLOT(slotPbQuickNewClicked()));
	QObject::connect(pbQuickDelete, SIGNAL(clicked()), SLOT(slotPbQuickDeleteClicked()));
	QObject::connect(teQuickMessage, SIGNAL(textChanged()), SLOT(slotTeQuickTextChanged()));
	
	//connect ListView to the WidgetStack,add by casper
	QObject::connect(lvSettingItem, SIGNAL(clicked(QListViewItem *)), SLOT(slotSettingListItemClicked(QListViewItem *)));
	//click face button then show the IconView
	QObject::connect(pbChangeFace,SIGNAL(clicked()),SLOT(slotPbFaceClicked()));
	//double click the face Icon in the IconView
	QObject::connect( ivFace, SIGNAL( doubleClicked(QIconViewItem*) ), SLOT( FaceChoose(QIconViewItem*) ) );
	//if the text of signature changed
	QObject::connect( teSignature, SIGNAL(textChanged()), SLOT(slotSignatureChanged()));
	
	//QQShow
	QObject::connect( tbtnShop, SIGNAL( clicked() ),  SLOT( slotShopClicked() ) );
	QObject::connect( tbtnAlbum, SIGNAL( clicked() ),  SLOT( slotAlbumClicked() ) );
	QObject::connect( tbtnHome, SIGNAL( clicked() ),  SLOT( slotHomeClicked() ) );
	QObject::connect( kpbUpdateShow, SIGNAL( clicked() ),  SLOT( slotUpdateQQShow() ) );
	
	//the following connection check if anything of Setting Item changed ,then make apply enable;
	QObject::connect( leNickName, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( leRealName, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( leAge, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( leSchool, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( leHomePage, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( teSignature, SIGNAL(textChanged()), SLOT( slotUserInfoChanged() ) );
	QObject::connect( teAboutMe, SIGNAL(textChanged()), SLOT( slotUserInfoChanged() ) );
	QObject::connect( ivFace, SIGNAL( doubleClicked(QIconViewItem*) ), SLOT( slotUserInfoChanged() ) );
	QObject::connect( cbSex, SIGNAL( activated( int )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( cbOccupation, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( cbZodiac, SIGNAL( activated( int )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( cbHoroscape, SIGNAL( activated( int )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( cbBlood, SIGNAL( activated( int )), SLOT( slotUserInfoChanged() ) );
	
	QObject::connect( cbArea, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( cbProvince, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( cbCity, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( leZipCode, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( leEmail, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( leAddress, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( lePhone, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( leMobile, SIGNAL( textChanged( const QString & )), SLOT( slotUserInfoChanged() ) );
	QObject::connect( rbPublic, SIGNAL( clicked() ), SLOT( slotUserInfoChanged() ) );
	QObject::connect( rbProtect, SIGNAL( clicked() ), SLOT( slotUserInfoChanged() ) );
	QObject::connect( rbPrivate, SIGNAL( clicked() ), SLOT( slotUserInfoChanged() ) );
	
	QObject::connect( pbChangePassword, SIGNAL(clicked()), SLOT(slotChangePasswordClicked()));
	QObject::connect( pbProtectPassword, SIGNAL(clicked()), SLOT(slotProtectPasswordClicked()));
	QObject::connect( rbAuthNoNeed, SIGNAL( clicked() ), SLOT( slotAuthChanged() ) );
	QObject::connect( rbAuthNeed, SIGNAL( clicked() ), SLOT( slotAuthChanged() ) );
	QObject::connect( rbAuthReject, SIGNAL( clicked() ), SLOT( slotAuthChanged() ) );
	QObject::connect( rbAuthQuest, SIGNAL( clicked() ), SLOT( slotAuthChanged() ) );
	QObject::connect( cbbQuestion, SIGNAL( textChanged( const QString & ) ), SLOT( slotAuthChanged() ) );
	QObject::connect( leAnswer, SIGNAL( textChanged( const QString & ) ), SLOT( slotAuthChanged() ) );
	
	
	QObject::connect( chbOnlineTip, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( chbMessage, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( chbShowOnlineUsers, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( chbSystemBroadcast, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( chbSystemNews, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( chbSound, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( chbSendKey, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( chbShowNickSmiley, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( chbShowSigSeperate, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( sbPageSize, SIGNAL( valueChanged( int ) ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( sbIdleTime, SIGNAL( valueChanged( int ) ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( sliderFaceSize, SIGNAL( valueChanged( int )  ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( kkbtnShortKey, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	
	QObject::connect( leThemePath, SIGNAL( textChanged( const QString & )), SLOT( slotSysSettingChanged() ) );
	QObject::connect( leSoundPath, SIGNAL( textChanged( const QString & )), SLOT( slotSysSettingChanged() ) );
	
	QObject::connect( chbAutoReply, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( pbAutoNew, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( pbAutoDelete, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( pbQuickNew, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( pbQuickDelete, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );

	QObject::connect( wspMainFont->kcbbBuddyNickColor, SIGNAL( activated(int) ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbBuddyNickFontB, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbBuddyNickFontU, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbBuddyNickFontI, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->kcbbBuddyFlashColor, SIGNAL( activated(int) ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbBuddyFlashFontB, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbBuddyFlashFontU, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbBuddyFlashFontI, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->kcbbBuddySigColor, SIGNAL( activated(int) ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbBuddySigFontB, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbBuddySigFontU, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbBuddySigFontI, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );

	QObject::connect( wspMainFont->kcbbQunNameColor, SIGNAL( activated(int) ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbQunNameFontB, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbQunNameFontU, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbQunNameFontI, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->kcbbQunFlashColor, SIGNAL( activated(int) ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbQunFlashFontB, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbQunFlashFontU, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbQunFlashFontI, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );

	QObject::connect( wspMainFont->kcbbGroupNameColor, SIGNAL( activated(int) ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbGroupNameFontB, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbGroupNameFontU, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbGroupNameFontI, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->kcbbGroupFlashColor, SIGNAL( activated(int) ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbGroupFlashFontB, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbGroupFlashFontU, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbGroupFlashFontI, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->kcbbGroupOnlineColor, SIGNAL( activated(int) ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbGroupOnlineFontB, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbGroupOnlineFontU, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );
	QObject::connect( wspMainFont->chbGroupOnlineFontI, SIGNAL( clicked() ), SLOT( slotSysSettingChanged() ) );

	
	//setup Icon of QQShow 
	if(faces){
		QPixmap *icon = faces->getIcon("QQ_SHOP");
		if(icon)
			tbtnShop->setIconSet(QIconSet(*icon));
	} 
	if(faces){
		QPixmap *icon = faces->getIcon("QQ_ALBUM");
		if(icon)
			tbtnAlbum->setIconSet(QIconSet(*icon));
	}
	if(faces){
		QPixmap *icon = faces->getIcon("QQ_HOME");
		if(icon)
			tbtnHome->setIconSet(QIconSet(*icon));
	}
	//show user info in User Info Window
	UpdateData(true); 
	if(details.size()!=37) {
		QTimer::singleShot(500, this, SLOT(slotFirstUpdate()));
	}
	
	pbApply->setEnabled( FALSE );
	isUserInfoChanged = FALSE;
	isSysSettingChanged = FALSE;
	isFaceSizeChanged = FALSE;
	isSignatureChanged = false;
	m_IsAuthQuestionChanged = false;
}

EvaSysSettingWindow::~ EvaSysSettingWindow( )
{
}

void EvaSysSettingWindow::saveSetting( )
{
	userSetting->setShowBudyOnlineNotifyEnabled(chbOnlineTip->isChecked());
	userSetting->setShowMessageTipEnabled(chbMessage->isChecked());
	userSetting->setShowOnlineEnabled(chbShowOnlineUsers->isChecked());
	userSetting->setShowSystemBroadcastEnabled(chbSystemBroadcast->isChecked());
	userSetting->setShowSystemNewsEnabled(chbSystemNews->isChecked());
	userSetting->setSoundEnabled(chbSound->isChecked());
	userSetting->setShowSmileyInNickName( chbShowNickSmiley->isChecked());
	userSetting->setShowSignatureInSeperateLine( chbShowSigSeperate->isChecked());
	userSetting->setSendKeyEnterEnabled(chbSendKey->isChecked());
	
	userSetting->setFaceSize(QSize(sliderFaceSize->value(), sliderFaceSize->value()));
	userSetting->setPageSize(sbPageSize->value());
	
	userSetting->setMessageShortcut(kkbtnShortKey->shortcut());

	userSetting->setBuddyNameColor( wspMainFont->kcbbBuddyNickColor->color() );
	userSetting->setBuddyNameBold( wspMainFont->chbBuddyNickFontB->isChecked() );
	userSetting->setBuddyNameUnderline( wspMainFont->chbBuddyNickFontU->isChecked()  );
	userSetting->setBuddyNameItalic( wspMainFont->chbBuddyNickFontI->isChecked()  );
	userSetting->setBuddyFlashColor( wspMainFont->kcbbBuddyFlashColor->color()  );
	userSetting->setBuddyFlashBold( wspMainFont->chbBuddyFlashFontB->isChecked()  );
	userSetting->setBuddyFlashUnderline( wspMainFont->chbBuddyFlashFontU->isChecked()  );
	userSetting->setBuddyFlashItalic( wspMainFont->chbBuddyFlashFontI->isChecked()  );
	userSetting->setBuddySigColor( wspMainFont->kcbbBuddySigColor->color()  );
	userSetting->setBuddySigBold( wspMainFont->chbBuddySigFontB->isChecked()   );
	userSetting->setBuddySigUnderline( wspMainFont->chbBuddySigFontU->isChecked()   );
	userSetting->setBuddySigItalic( wspMainFont->chbBuddySigFontI->isChecked()   );

	userSetting->setQunNameColor( wspMainFont->kcbbQunNameColor->color()  );
	userSetting->setQunNameBold( wspMainFont->chbQunNameFontB->isChecked()   );
	userSetting->setQunNameUnderline( wspMainFont->chbQunNameFontU->isChecked()   );
	userSetting->setQunNameItalic( wspMainFont->chbQunNameFontI->isChecked()   );
	userSetting->setQunFlashColor( wspMainFont->kcbbQunFlashColor->color()  );
	userSetting->setQunFlashBold( wspMainFont->chbQunFlashFontB->isChecked()   );
	userSetting->setQunFlashUnderline( wspMainFont->chbQunFlashFontU->isChecked()   );
	userSetting->setQunFlashItalic( wspMainFont->chbQunFlashFontI->isChecked()   );

	userSetting->setGroupNameColor( wspMainFont->kcbbGroupNameColor->color()  );
	userSetting->setGroupNameBold( wspMainFont->chbGroupNameFontB->isChecked()  );
	userSetting->setGroupNameUnderline( wspMainFont->chbGroupNameFontU->isChecked()  );
	userSetting->setGroupNameItalic( wspMainFont->chbGroupNameFontI->isChecked() );
	userSetting->setGroupFlashColor( wspMainFont->kcbbGroupFlashColor->color()  );
	userSetting->setGroupFlashBold( wspMainFont->chbGroupFlashFontB->isChecked()  );
	userSetting->setGroupFlashUnderline( wspMainFont->chbGroupFlashFontU->isChecked()  );
	userSetting->setGroupFlashItalic( wspMainFont->chbGroupFlashFontI->isChecked()  );
	userSetting->setGroupCountColor( wspMainFont->kcbbGroupOnlineColor->color()  );
	userSetting->setGroupOnlineCountBold( wspMainFont->chbGroupOnlineFontB->isChecked()  );
	userSetting->setGroupOnlineCountUnderline( wspMainFont->chbGroupOnlineFontU->isChecked()  );
	userSetting->setGroupOnlineCountItalic( wspMainFont->chbGroupOnlineFontI->isChecked()  );

	
	userSetting->setThemeDir(leThemePath->text());
	userSetting->setSoundDir(leSoundPath->text());
	//userSetting->setShowOnlineEnabled(false);
	
	userSetting->setAutoReplyList(autoList);
	userSetting->setQuickReplyList(quickList);
	userSetting->setIdleMaxTime(sbIdleTime->value());
	
	userSetting->saveSettings();
	emit settingChanged();
}

void EvaSysSettingWindow::loadSetting( const bool isLoadDefault)
{	
	chbOnlineTip->setChecked(userSetting->isShowBudyOnlineNotifyEnabled());
	chbMessage->setChecked(userSetting->isShowMessageTipEnabled());
	chbShowOnlineUsers->setChecked(userSetting->isShowOnlineEnabled());
	chbSystemBroadcast->setChecked(userSetting->isShowSystemBroadcastEnabled());
	chbSystemNews->setChecked(userSetting->isShowSystemNewsEnabled());
	chbSound->setChecked(userSetting->isSoundEnabled());
	chbShowNickSmiley->setChecked(userSetting->isShowSmileyInNickName());
	chbShowSigSeperate->setChecked(userSetting->isShowSignatureInSeperateLine());
	chbSendKey->setChecked(userSetting->isSendKeyEnterEnabled());
		
	sliderFaceSize->setValue( userSetting->getFaceSize().width() ); // assume width == height
	slotFaceSizeChanged(userSetting->getFaceSize().width());
	sbPageSize->setValue(userSetting->getPageSize());
	sbIdleTime->setValue(userSetting->getIdleMaxTime());
	
	chbAutoReply->setChecked(userSetting->isAutoReplyEnabled());
	
	leThemePath->setText(userSetting->getThemeDir());
	leSoundPath->setText(userSetting->getSoundDir());
	
	kkbtnShortKey->setShortcut(userSetting->getMessageShortcut(), false);

	wspMainFont->kcbbBuddyNickColor->setColor(userSetting->getBuddyNameColor());
	wspMainFont->chbBuddyNickFontB->setChecked(userSetting->isBuddyNameBold());
	wspMainFont->chbBuddyNickFontU->setChecked(userSetting->isBuddyNameUnderline());
	wspMainFont->chbBuddyNickFontI->setChecked(userSetting->isBuddyNameItalic());

	wspMainFont->kcbbBuddyFlashColor->setColor(userSetting->getBuddyFlashColor());
	wspMainFont->chbBuddyFlashFontB->setChecked(userSetting->isBuddyFlashBold());
	wspMainFont->chbBuddyFlashFontU->setChecked(userSetting->isBuddyFlashUnderline());
	wspMainFont->chbBuddyFlashFontI->setChecked(userSetting->isBuddyFlashItalic());

	wspMainFont->kcbbBuddySigColor->setColor(userSetting->getBuddySigColor());
	wspMainFont->chbBuddySigFontB->setChecked(userSetting->isBuddySigBold());
	wspMainFont->chbBuddySigFontU->setChecked(userSetting->isBuddySigUnderline());
	wspMainFont->chbBuddySigFontI->setChecked(userSetting->isBuddySigItalic());

	wspMainFont->kcbbQunNameColor->setColor(userSetting->getQunNameColor());
	wspMainFont->chbQunNameFontB->setChecked(userSetting->isQunNameBold());
	wspMainFont->chbQunNameFontU->setChecked(userSetting->isQunNameUnderline());
	wspMainFont->chbQunNameFontI->setChecked(userSetting->isQunNameItalic());

	wspMainFont->kcbbQunFlashColor->setColor(userSetting->getQunFlashColor());
	wspMainFont->chbQunFlashFontB->setChecked(userSetting->isQunFlashBold());
	wspMainFont->chbQunFlashFontU->setChecked(userSetting->isQunFlashUnderline());
	wspMainFont->chbQunFlashFontI->setChecked(userSetting->isQunFlashItalic());

	wspMainFont->kcbbGroupNameColor->setColor(userSetting->getGroupNameColor());
	wspMainFont->chbGroupNameFontB->setChecked(userSetting->isGroupNameBold());
	wspMainFont->chbGroupNameFontU->setChecked(userSetting->isGroupNameUnderline());
	wspMainFont->chbGroupNameFontI->setChecked(userSetting->isGroupNameItalic());

	wspMainFont->kcbbGroupFlashColor->setColor(userSetting->getGroupFlashColor());
	wspMainFont->chbGroupFlashFontB->setChecked(userSetting->isGroupFlashBold());
	wspMainFont->chbGroupFlashFontU->setChecked(userSetting->isGroupFlashUnderline());
	wspMainFont->chbGroupFlashFontI->setChecked(userSetting->isGroupFlashItalic());

	wspMainFont->kcbbGroupOnlineColor->setColor(userSetting->getGroupCountColor());
	wspMainFont->chbGroupOnlineFontB->setChecked(userSetting->isGroupOnlineCountBold());
	wspMainFont->chbGroupOnlineFontU->setChecked(userSetting->isGroupOnlineCountUnderline());
	wspMainFont->chbGroupOnlineFontI->setChecked(userSetting->isGroupOnlineCountItalic());
	
	if(!isLoadDefault){
		autoList = userSetting->getAutoReplyList();
		cbbAutoNo->clear();
		for(uint i=0; i<autoList.size(); i++)
			cbbAutoNo->insertItem(QString::number(i+1));
		slotCbbAutoNoActivated(0);
				
		quickList = userSetting->getQuickReplyList();
		cbbQuickNo->clear();
		for(uint i=0; i<quickList.size(); i++)
			cbbQuickNo->insertItem(QString::number(i+1));
		slotCbbQuickNoActivated(0);
	}
}


void EvaSysSettingWindow::slotPbFaceDefaultClicked( )
{
	userSetting->loadDefaultSettings();
	loadSetting(false);
}


void EvaSysSettingWindow::slotPbApplyClicked( )
{
	
	if( isUserInfoChanged ){
		bool ok;
		unsigned int id = details[0].toUInt(&ok, 10);
		if(!ok) return;
		
		if(m_IsAuthQuestionChanged){
			if(cbbQuestion->currentText().stripWhiteSpace() == ""){
				KMessageBox::information(this, i18n( "Question cannot be empty."), i18n( "Modify Personal Details")); 
				cbbQuestion->setFocus();
				return;
			}

			if(leAnswer->text().stripWhiteSpace() == ""){
				KMessageBox::information(this, i18n( "Answer cannot be empty.."), i18n( "Modify Personal Details")); 
				leAnswer->setFocus();
				return;
			}
			requestMyUpdateQuestion(cbbQuestion->currentText(), leAnswer->text());
			return;
		}
		
		if(UpdateData(false)){
			if(isSignatureChanged){
				if(teSignature->text().local8Bit().size() > 100){
					KMessageBox::information(this, i18n("Signature is too long!"), i18n("Modify Settings"));
					teSignature->setFocus();
					return;
				}
			}
			//emit requestModify(details, leOldPwd->text(), leNewPwd->text());
			emit requestModify(details, "", "");
			if(isSignatureChanged){
				QString contents = teSignature->text();
				if(contents.isEmpty()){
					emit requestDeleteSignature();
				}else{
					emit requestUpdateSignature(contents);
				}
			}
		
			//if( chbChangePwd->isChecked() )
			//	chbChangePwd->setChecked( FALSE );
			slotChangePwdClicked();
	
		}else
			emit requestUpdate(id);
	}
	if( isSysSettingChanged ){
		saveSetting( );
	}

	if( isFaceSizeChanged ) {
		emit faceSizeChanged();
	}
	
	isUserInfoChanged = FALSE;
	isSysSettingChanged = FALSE;
	isFaceSizeChanged = FALSE;
	pbApply->setEnabled( FALSE );
	
}

void EvaSysSettingWindow::slotPbOKClicked( )
{
	slotPbApplyClicked();
	close();
}

void EvaSysSettingWindow::slotFaceSizeChanged( int value )
{
	QString txt = i18n("large" ) + " (" + QString::number(value) + ")";
	lblFaceBig->setText( txt );

	isFaceSizeChanged = TRUE;
	
}

void EvaSysSettingWindow::slotCapturedShortcut(const KShortcut &key)
{
	kkbtnShortKey->setShortcut(key, false);
}

void EvaSysSettingWindow::slotPbThemeClicked( )
{
	QFileDialog *fd = new QFileDialog(this, i18n("Choose Theme Path  - Eva"), true);
	fd->setMode(QFileDialog::DirectoryOnly);
	fd->setFilter("Theme (eva.theme)");
	QString path;
	if(fd->exec() == QDialog::Accepted)
		path = fd->selectedFile();
	else
		return;
		
	//path = path.left(path.length() - strlen("/eva.theme"));
	
	leThemePath->setText(path);
}

void EvaSysSettingWindow::slotPbSoundClicked( )
{
	QFileDialog *fd = new QFileDialog(this, i18n("Choose Sound Path  - Eva"), true);
	fd->setMode(QFileDialog::DirectoryOnly);
	fd->setFilter("all (*)");
	QString path;
	if(fd->exec() == QDialog::Accepted)
		path = fd->selectedFile();
	else
		return;
	
	leSoundPath->setText(path);
}

void EvaSysSettingWindow::slotCbbAutoNoActivated( int index)
{
	if((uint)index>=autoList.size()) return;
	std::list<QString>::iterator iter = autoList.begin();
	for(int i=0; i<index; i++)
		iter++;
	teAutoMessage->setText(*iter);
}

void EvaSysSettingWindow::slotPbAutoNewClicked( )
{	
	cbbAutoNo->insertItem(QString::number(cbbAutoNo->count()+1));
	cbbAutoNo->setCurrentItem(cbbAutoNo->count()-1);
	autoList.push_back(teAutoMessage->text());
	teAutoMessage->selectAll(true);
}

void EvaSysSettingWindow::slotPbAutoDeleteClicked( )
{
	QString txt = teAutoMessage->text();
	int index = cbbAutoNo->currentItem();
	std::list<QString>::iterator iter = autoList.begin();
	for(int i=0; i<index; i++)
		iter++;
	autoList.erase(iter);
	cbbAutoNo->removeItem(cbbAutoNo->count()-1);
	slotCbbAutoNoActivated(cbbAutoNo->currentItem());
}

void EvaSysSettingWindow::slotTeAutoTextChanged()
{
	int index = cbbAutoNo->currentItem();
	QString txt = teAutoMessage->text();
	if((uint)index > autoList.size()) return;
	
	std::list<QString>::iterator iter = autoList.begin();
	for(int i=0; i<index; i++)
		iter++;
	*iter = txt;
}

void EvaSysSettingWindow::slotCbbQuickNoActivated( int index)
{
	if((uint)index>=quickList.size()) return;
	std::list<QString>::iterator iter = quickList.begin();
	for(int i=0; i<index; i++)
		iter++;
	teQuickMessage->setText(*iter);
}

void EvaSysSettingWindow::slotPbQuickNewClicked( )
{
	cbbQuickNo->insertItem(QString::number(cbbQuickNo->count()+1));
	cbbQuickNo->setCurrentItem(cbbQuickNo->count()-1);
	quickList.push_back(teQuickMessage->text());
	teQuickMessage->selectAll(true);
}

void EvaSysSettingWindow::slotPbQuickDeleteClicked( )
{
	int index = cbbQuickNo->currentItem();
	if((uint)index > quickList.size()) return;
	std::list<QString>::iterator iter = quickList.begin();
	for(int i=0; i<index; i++)
		iter++;
	quickList.erase(iter);

	cbbQuickNo->removeItem(cbbQuickNo->count()-1);
	slotCbbQuickNoActivated(cbbQuickNo->currentItem());
}

void EvaSysSettingWindow::slotTeQuickTextChanged()
{
	int index = cbbQuickNo->currentItem();
	QString txt = teQuickMessage->text();
	if((uint)index >= quickList.size()) return;
	
	std::list<QString>::iterator iter = quickList.begin();
	for(int i=0; i<index; i++){
		iter++;
	}
	*iter = txt;
}

void EvaSysSettingWindow::slotSettingListItemClicked(QListViewItem* item)
{ 
	if(!item)
		return;
	
	EvaSettingListItem* listItem = dynamic_cast<EvaSettingListItem *>(item);
	//printf( "Clicked Item Index:%d\n",listItem->itemIndex);
	int index = listItem->itemIndex;
	switch(index){
	case 0:						//User Setting
		if( item->isOpen() ){
			item->setPixmap(0, *faces->getIcon("SETTING_CLOSE")); 
			item->setOpen( FALSE );
		}else{
			item->setPixmap(0, *faces->getIcon("SETTING_OPEN")); 
			item->setOpen( TRUE );
		}
		pbFaceDefault->hide();
		break;
	case 1:						//User Info
		wsSetting->raiseWidget(0);
		pbFaceDefault->hide();
		break;
	case 2:						//QQ Show
		wsSetting->raiseWidget(1);
		pbFaceDefault->hide();
		break;
	case 3:						//Contact
		wsSetting->raiseWidget(2);
		pbFaceDefault->hide();
		break;
	case 4:						//Security
		wsSetting->raiseWidget(3);
		pbFaceDefault->hide();
		break;
	case 5:						//Sys Setting
		if( item->isOpen() ){
			item->setPixmap(0, *faces->getIcon("SYS_CLOSE"));
			item->setOpen( FALSE );
		}else{
			item->setPixmap(0, *faces->getIcon("SYS_OPEN"));
			item->setOpen( TRUE );
		}
		
		break;
	case 6:						//Basic Setting
		wsSetting->raiseWidget(4);
		pbFaceDefault->show();
		break;
	case 7:						//Resource
		wsSetting->raiseWidget(5);
		pbFaceDefault->show();
		break;
	case 8:						//Reply
		wsSetting->raiseWidget(6);
		pbFaceDefault->show();
		break;	
	case 9:						// font setting
		wsSetting->raiseWidget(7);
		pbFaceDefault->show();
		break;
	default:
		break;
	}
	//qDebug( item->itemPos() );*/
	
}

void EvaSysSettingWindow::setupFaces()
{
	if(!faces) return;
	ivFace->clear();
	//printf("setupFaces run !\n");
	for(int i=1; i< MaxFaceNumber; i++){
		QIconViewItem *item =  new QIconViewItem( ivFace, QString::null );
		item->setPixmap(*(faces->getFace(i)));
		item->setKey( QString::number(i));  // set face file index as key
	}
}

void EvaSysSettingWindow::slotPbFaceClicked()
{
	
	if(ivFace->isVisible()) 
		ivFace->hide();
	else
		ivFace->show();
}

void EvaSysSettingWindow::FaceChoose( QIconViewItem * item )
{
	if(!faces) return;
	selectedFaceIndex = faces->getFaceID(item->key().toInt());
	//printf("select:%d, %d\n", selectedFaceIndex, item->key().toInt());
	lblFace->setPixmap(*(item->pixmap()));
	ivFace->hide();
}

bool EvaSysSettingWindow::UpdateData(bool toShow)
{
	leNumber->setText( details[0] );
	if(details.size()!=37) return false;

	bool OK;
	unsigned int id = details[0].toUInt(&OK);
	if(!OK) return false;
	slotBuddyQQShowReady(id);
	
	int level = 0;
	QString signature = "";
	level = EvaMain::user->getLevel();
	signature = QTextCodec::codecForName("GB18030")->toUnicode(EvaMain::user->getSignature().c_str());
	
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
	lblLevelDisplay->setText("(" + QString::number(level) + ")" + strLevel);
	lblLevelDisplay->adjustSize();
	leNumber->setReadOnly(true);
	if (toShow)
	{
		leNumber->setText( details[0] );			
		leNickName->setText( details[1]);
		leAge->setText( details[7] );
		cbSex->setCurrentText( details[8] );
		cbArea->setCurrentText( details[2]);
		cbProvince->setCurrentText( details[3] ); 
		cbCity->setCurrentText( details[25] );
		
		bool ok;
		int faceId = details[21].toInt(&ok, 10);
		if(ok){
			selectedFaceIndex = faceId;
			QPixmap *pix = faces->getUserHeadPixmap(id);
			if(!pix) pix = faces->getFace(faces->getFaceFileIndex(faceId));
			if(pix) lblFace->setPixmap( *pix );
		}else{
			selectedFaceIndex = 0;
			printf("face id converting failed. \"%s\"\n", details[21].local8Bit().data());
		}

		leEmail->setText( details[10] );
		leAddress->setText( details[5] );
		leZipCode->setText( details[4] );
        	lePhone->setText( details[6] );
		leMobile->setText( details[22]);
		
		leRealName->setText( details[9]);
        	leSchool->setText( details[31] );
		cbOccupation->setCurrentText( details[16] );
		leHomePage->setText( details[17] );
		cbZodiac->setCurrentItem( details[33].toInt() );
		cbBlood->setCurrentItem(details[34].toInt());
		cbHoroscape->setCurrentItem( details[32].toInt() );
		teAboutMe->setText( details[24] );
		teSignature->setText(signature);
		
		switch ( details[30].toInt() )
		{
			case 0: rbPublic->setChecked( true );
				break;
			case 1: rbProtect->setChecked( true );
				break;
			case 2: rbPrivate->setChecked( true );
				break;
			default:break;
		}
		QString authCode = details[18];
		if ( authCode == "0" ){
			rbAuthNoNeed->setChecked(true);
		}else if(authCode == "1"){
			rbAuthNeed->setChecked(true);
		}else if(authCode == "2") {
			rbAuthReject->setChecked(true);
		} else if(authCode == "3"){
			rbAuthQuest->setChecked(true);
			printf("signal emitted\n");
			emit requestMyAuthQuestionSetting();
		}
	}
	else
	{
// 		if(chbChangePwd->isChecked()){
// 			if(leOldPwd->text().stripWhiteSpace() == ""){
// 				KMessageBox::information(this, i18n( "enter your current password please."), i18n( "Modify Personal Details")); 
// 				leOldPwd->setFocus();
// 				return false;
// 			}
// 				
// 			if(leNewPwd->text() != leNewPwdConfirm->text()){
// 				KMessageBox::information(this, i18n( "new password mismatch."), i18n( "Modify Personal Details"));
// 				leNewPwdConfirm->setFocus(); 
// 				return false;
// 			}
// 			if(leNewPwd->text().stripWhiteSpace() == ""){
// 				KMessageBox::information(this, i18n( "Modify Personal Details"), i18n( "new password cannot be empty.")); 
// 				leNewPwd->setFocus();
// 				return false;
// 			}
// 		}
		details.clear();
		details.append(leNumber->text( ));
		details.append(leNickName->text( ));
		details.append(cbArea->currentText(  ));
		details.append(cbProvince->currentText( ));
		details.append(leZipCode->text( ));
		
		
		details.append(leAddress->text(  ));
        	details.append(lePhone->text(  ));
		details.append(leAge->text( ));
		details.append(cbSex->currentText(  )); 
		details.append(leRealName->text(  ));
		
		details.append(leEmail->text( ));
		details.append(""); // pagerSn
		details.append(""); // pagerNum
		details.append(""); // pagerSP
		details.append(""); // pagerBaseNum
		
		details.append(""); // pagerType
		details.append(cbOccupation->currentText( ));
		details.append(leHomePage->text(  ));
		if ( rbAuthNoNeed->isChecked() ){
			details.append("0");
		}else if ( rbAuthNeed->isChecked()){
			details.append("1");
		} else if( rbAuthReject->isChecked()){
			details.append("2");
		} else if (rbAuthQuest->isChecked()){
			details.append("3");
			///TODO: we should update question setting if changed
		} else 
			details.append("2");

		details.append(""); // unknown1
		
		details.append(""); // unknown2
		details.append( QString::number(selectedFaceIndex) );printf("face index: %d", selectedFaceIndex);
		details.append(leMobile->text( ));
		details.append(""); // mobile type
		details.append(teAboutMe->text(  ));
			
		details.append(cbCity->currentText(  ));
		details.append(""); // unknown3
		details.append(""); // unknown4
		details.append(""); // unknown5
		details.append(""); // open hp
		
		if ( rbPublic->isChecked() ){
			details.append("0");
		}else if ( rbProtect->isChecked()){
			details.append("1");
		} else{
			details.append("2");
		}
        	details.append(leSchool->text( ));
		details.append(QString::number(cbHoroscape->currentItem(  )));
		details.append(QString::number(cbZodiac->currentItem(  )));
		details.append(QString::number(cbBlood->currentItem()));
		
		details.append("0"); // qqshow
		details.append("0"); // unknown6
	}
	
	return true;
}

void EvaSysSettingWindow::slotFirstUpdate()
{
	bool ok;
	unsigned int id = details[0].toUInt(&ok, 10);
	if(!ok) return;
	printf("ask for details\n");
	emit requestUpdate(id);
}

void EvaSysSettingWindow::slotDetailsUpdated(QStringList list)
{
	if(details[0] != list[0] ) 
		return;
	details = list;
	UpdateData(true);

	isUserInfoChanged = FALSE;
	isSysSettingChanged = FALSE;
	pbApply->setEnabled( FALSE );
}

void EvaSysSettingWindow::slotSignatureChanged( )
{
	isSignatureChanged = true;
}

void EvaSysSettingWindow::slotSignatureReply(const bool ok)
{
	if(ok) printf("Signature modify successed !\n");
	else printf("Signature modify failed !\n");

}

void EvaSysSettingWindow::slotUpdateResult(bool ok, bool isSignature)
{
	if(ok)
		KMessageBox::information(this, i18n( "modify settings successfully."), i18n( "Modify Settings"));
	else
		KMessageBox::information(this, i18n( "failed to modify settings."), i18n( "Modify Settings"));
	if(isSignature && ok)
		isSignatureChanged=false;
}


void EvaSysSettingWindow::slotLinkClicked( const QString & url)
{
	QStringList args;
	args<<"exec"<< url;
	kapp->kdeinitExec("kfmclient",args);
	//KRun::runCommand("konqueror \""+url+"\"");
}

void EvaSysSettingWindow::slotShopClicked( )
{
	QString url = "http://jump.qq.com/clienturl_simp_17?clientuin=" + QString::number(EvaMain::user->getQQ());
	url+="&clientkey=";
	url+=getClientKeyString();
	slotLinkClicked(url);
}

void EvaSysSettingWindow::slotAlbumClicked( )
{
	QString url = "http://ptlogin.qq.com/qqshowalbum?clientuin=" + QString::number(EvaMain::user->getQQ());
	url+="&clientkey=";
	url+=getClientKeyString();
	slotLinkClicked(url);
}

void EvaSysSettingWindow::slotHomeClicked( )
{
	QString url = "http://jump.qq.com/clienturl_42?clientuin=" + QString::number(EvaMain::user->getQQ());
	url+="&clientkey=";
	url+=getClientKeyString();
	slotLinkClicked(url);
}

const QString EvaSysSettingWindow::getClientKeyString()
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

void EvaSysSettingWindow::slotUpdateQQShow()
{
	bool ok;
	int id = details[0].toInt(&ok, 10);
	if(ok)
		emit requestQQShow(id);
}

void EvaSysSettingWindow::slotBuddyQQShowReady(const unsigned int id)
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

void EvaSysSettingWindow:: slotChangePwdClicked()
{
// 	if( chbChangePwd->isChecked() ){
// 		leOldPwd->setEnabled( TRUE );
// 		leNewPwd->setEnabled( TRUE );
// 		leNewPwdConfirm->setEnabled( TRUE );
// 		
// 	}
// 	else{
// 		leOldPwd->setEnabled( FALSE );
// 		leNewPwd->setEnabled( FALSE );
// 		leNewPwdConfirm->setEnabled( FALSE );
// 		
// 	}
}

void EvaSysSettingWindow::slotUserInfoChanged()
{
	isUserInfoChanged = TRUE;
	pbApply->setEnabled( TRUE );
}
void EvaSysSettingWindow:: slotSysSettingChanged()
{
	isSysSettingChanged = TRUE;
	pbApply->setEnabled( TRUE );
}

void EvaSysSettingWindow::closeEvent( QCloseEvent * e )
{	
	e->accept();
	deleteLater();
}

void EvaSysSettingWindow::slotAuthChanged( )
{
	if(rbAuthQuest->isChecked()){
		fraQuestion->setEnabled(true);
		m_IsAuthQuestionChanged = true;
	} else{
		fraQuestion->setEnabled(false);
		m_IsAuthQuestionChanged = true;
	}
		
	slotUserInfoChanged();
}

void EvaSysSettingWindow::slotReceivedMyAuthSettings( const unsigned char auth,
					const QString& question,
					const QString & answer )
{
	if(auth != AUHT_TYPE_QUESTION_GET) return;
	fraQuestion->setEnabled( true );
	cbbQuestion->setCurrentText( question);
	leAnswer->setText( answer );
}

void EvaSysSettingWindow::slotUpdateAuthSettingResult( const unsigned char auth, const unsigned char reply )
{
	if(auth != AUTH_TYPE_QUESTION_SET) return;
	if(reply != 0x00){
		KMessageBox::information(0, QString(i18n("Update authentication question failed!")),
			i18n("Modify Personal Details"));
		return;
	}
	m_IsAuthQuestionChanged = false;
	// update all details now
	slotPbApplyClicked();
}

void EvaSysSettingWindow::slotChangePasswordClicked( )
{
	QString url = "http://jump.qq.com/clienturl_173?clientuin=" + QString::number(EvaMain::user->getQQ());
	url+= "&clientkey=";
	url+= getClientKeyString();
	url+= "&ADUIN=";
	url+= QString::number(EvaMain::user->getQQ());
	url+= "&ADSESSION=1171468766&ADTAG=CLIENT.QQ.1595_Setting_PasswordTab.0";
	slotLinkClicked(url);
}

void EvaSysSettingWindow::slotProtectPasswordClicked( )
{
	QString url = "http://service.qq.com/psw/psw_id_index.htm?ADUIN=" + QString::number(EvaMain::user->getQQ());
	url+= "&ADSESSION=1171468766&ADTAG=CLIENT.QQ.1595_Setting_PasswordTab.0";
	slotLinkClicked(url);
}
	


