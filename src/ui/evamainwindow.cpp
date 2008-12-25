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
#include "evamainwindow.h"
#include "defines.h"
#include "evasession.h"

#include <stdlib.h>

#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <q3header.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpixmap.h>
#include <q3listview.h>
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qevent.h>
#include <qcursor.h>
//Added by qt3to4:
#include <QMoveEvent>
#include <QResizeEvent>
#include <QMenu>

//X #include <klocale.h>
//X #include <kpopupmenu.h>
//X #include <khtmlview.h>
//X #include <khtml_part.h>
//X #include <kdebug.h>

#include "evafriendlist.h"
#include "evauhmanager.h"
#include "evauser.h"
#include "evaresource.h"
#include "evahtmlparser.h"
#include "evatabwidget.h"
#include "evacontactlistview.h"
#include "evaloginmanager.h"
#include "evadisplaywidget.h"
#include "evastatusbar.h"
#include "evachatwindowmanager.h"
//X #include "evascriptwidget.h"
//X 
//X MainWindowTip::MainWindowTip(QWidget *parent)
//X 	: QToolTip(parent) 
//X {
//X }
//X 
//X void MainWindowTip::maybeTip(const QPoint &pos)
//X {
//X 	if(!parentWidget()->inherits("EvaMainWindow"))
//X 		return;
//X 	QRect r( ((EvaMainWindow *)parentWidget())->tipRect(pos) );
//X 	if(!r.isValid()) 
//X 		return;
//X 	tip(r, ((EvaMainWindow *)parentWidget())->myInfoTip());
//X }

EvaMainWindow::EvaMainWindow(QWidget* parent, const char* name, Qt::WFlags fl )
//X 	: DCOPObject("View")
        : EvaMainUIBase(parent, name, fl)
	, sysMenu(NULL), statusMenu(NULL)
	, pixOnline(NULL), pixOffline(NULL), pixLeave(NULL), pixInvisible(NULL)
{
	qqNum = 0;
//X 	myTip = new MainWindowTip(this);

	pixOnline = EvaMain::images->getIcon("ONLINE");
	pixOffline = EvaMain::images->getIcon("OFFLINE");
	pixLeave = EvaMain::images->getIcon("LEAVE");
	pixInvisible = EvaMain::images->getIcon("INVISIBLE");

	statusBar->tbSearch->setIconSet(QIcon(*EvaMain::images->getIcon("FIND")));
	statusBar->tbSysMsg->setIconSet(QIcon(*EvaMain::images->getIcon("SYSTEM_MSG")));
	statusBar->tbSystem->setIconSet(QIcon(*EvaMain::images->getIcon("SYSTEM_OPTIONS")));

	m_buddyLv = 0;
	m_qunLv = 0;
	m_recentLv = 0;

	m_buddyTabKey = 0;
	m_qunTabKey = 1;
	m_recentTabKey = 2;
 	loadContacts();
 	loadQuns();
 	loadRecentContacts();

	QObject::connect(statusBar->tbSearch, SIGNAL(clicked()), this, SLOT(slotSearch()));
	QObject::connect(statusBar->tbSysMsg, SIGNAL(clicked()), this, SLOT(slotSystemMessages()));
	QObject::connect(statusBar->tbSystem, SIGNAL(clicked()), this, SLOT(slotSystemClicked()));
	QObject::connect(statusBar->tbStatus, SIGNAL(clicked()), this, SLOT(slotStatusClicked()));
	QObject::connect(tbMyFace, SIGNAL(clicked()), this, SLOT(slotTbMyFace()));

	offline();
}
 


EvaMainWindow::~EvaMainWindow()
{
//X 	m_customTabs.clear();
}

void EvaMainWindow::toggleVisible() {
    setVisible( !isVisible() );
}
void EvaMainWindow::setMainInfo(const unsigned int id, const QString &nick, QPixmap *pix)
{
	qqNum = id;
	QString name = nick;
	if(strlen(nick.ascii()) > 12 )
		name = nick.left(9) + "...";
	EvaHtmlParser parser;
	parser.setAbsImagePath(EvaMain::images->getSmileyPath());
	parser.convertToHtml(name, false, true);
	tlNick->setText("<qt><nobr>"+name+"</nobr></qt>");
	slotUpdateBuddyStat();

	if(pix)
		tbMyFace->setIconSet( QIcon(*pix));
        setToolTip( myInfoTip() );
//
//
//	loadContacts();
//	loadQuns();
//	loadRecentContacts();
}

void EvaMainWindow::slotUpdateBuddyStat()
{
	int all = EvaMain::session->getUser()->getFriendList().numberOfFriends();
	int onlines = EvaMain::session->getUser()->getFriendList().numberOfOnlines();
	tlQQ->setText(" ( "+ QString::number(onlines) + "/" + QString::number(all) + ")");
}

void EvaMainWindow::setSystemMenu( QMenu *sys)
{
	if(sys){
		sysMenu = sys;
		statusBar->tbSystem->setPopup(sys);
	}
}

void EvaMainWindow::setStatusMenu( QMenu *status)
{
	if(status){
		statusMenu = status;
		statusBar->tbStatus->setPopup(status);
	}
}

void EvaMainWindow::changeGroupTo(const unsigned int id, const int index)
{
	if(m_buddyLv)
		m_buddyLv->changeGroupTo(id, index);
	
}

void EvaMainWindow::updateBuddy(const unsigned int id)
{
	/*
	EvaUser *user = EvaMain::session->getUser();
	if(user && id == user->getQQ()){
		//QString myNick = codec->toUnicode(user->getDetails().at(ContactInfo::Info_nick).c_str());
		QString myNick = GB2Unicode(user->getDetails().at(ContactInfo::Info_nick).c_str());
		if(myNick.isNull()) myNick = "";
		setCaption(myNick + " - Eva");
		int myFaceId = atoi(user->getDetails().at(ContactInfo::Info_face).c_str());
		QPixmap *face = EvaMain::images->getFaceByID(myFaceId);
		if(user->hasUserHead()){
			QPixmap *uhPic = EvaMain::images->getUserHeadPixmap(user->getQQ()); // color pixmap
			if(uhPic) face = uhPic;
		}
		setIcon(*face);
		setMainInfo(user->getQQ(), myNick, face?face:EvaMain::images->getFace(0));
	}
*/		
	if(m_buddyLv)
		m_buddyLv->friendStatusChanged(id);
}

void EvaMainWindow::updateMyInfo()
{
	EvaUser *user = EvaMain::session->getUser();
	if(user){
		QString myNick = GB2Unicode(user->getDetails().at(ContactInfo::Info_nick).c_str());
		if(myNick.isNull()) myNick = "";
		setCaption(myNick + " - Eva");
		QString faceIdStr = user->getDetails().at(ContactInfo::Info_face).c_str();
		if( faceIdStr.isNull() ) faceIdStr = "0";
                printf( "EvaMainWindow::updateMYInfo() bad faceIdStr!!\n" );
		int myFaceId = faceIdStr.toInt();
		QPixmap *face = EvaMain::images->getFaceByID(myFaceId);
		if(user->hasUserHead()){
			QPixmap *uhPic = EvaMain::images->getUserHeadPixmap(user->getQQ()); // color pixmap
			if(uhPic) face = uhPic;
		}
		setIcon(*face);
		setMainInfo(user->getQQ(), myNick, face?face:EvaMain::images->getFace(0));
	}
}


void EvaMainWindow::addQun(const unsigned int id)
{
	if(m_qunLv)
		m_qunLv->addQun(id);
}

void EvaMainWindow::updateQun(const unsigned int id)
{
	if(m_qunLv)
		m_qunLv->updateQun(id);
}
	
void EvaMainWindow::clearList()
{
// 	removeTab(m_buddyTabKey);
// 	removeTab(m_qunTabKey);
// 	removeTab(m_recentTabKey);
// 
// 	m_buddyLv = 0;
// 	m_buddyTabKey = 0;
// 	m_qunLv = 0;
// 	m_qunTabKey = 1;
// 	m_recentLv = 0;
// 	m_recentTabKey = 2;

	if(m_buddyLv) m_buddyLv->clear();
	if(m_qunLv) m_qunLv->clear();
	if(m_recentLv) m_recentLv->clear();
}

void EvaMainWindow::friendStatusChanged( unsigned int id )
{
	const QQFriend *frd = (EvaMain::session->getUser()->getFriendList()).getFriend(id);
	if(frd){
		switch(frd->getStatus()){
		case QQ_FRIEND_STATUS_ONLINE:
			changeToOnline(id);
//X 			if(settings->isShowBudyOnlineNotifyEnabled()){
//X                                 tray->showMessageTip( id, codec->toUnicode(frd->getNick().c_str()), i18n( "I am online." ) );
//X 				EvaTipWindow *tip = new EvaTipWindow(images, codec->toUnicode(frd->getNick().c_str()), 
//X 									id, frd->getFace(), i18n("I am online."));
//X 				connect(tip, SIGNAL(requestChat(const unsigned int)), SLOT(slotRequestChat(const unsigned int)));
//X 				tip->show();
//X 				if(settings->isSoundEnabled())
//X 					global->getSoundResource()->playOnlineSound();
//X 			}
			break;
		case QQ_FRIEND_STATUS_OFFLINE:
			changeToOffline(id);
			break;
		case QQ_FRIEND_STATUS_LEAVE:
			changeToLeave(id);
//X 			if(EvaMain::settings->isShowBudyOnlineNotifyEnabled()){
//X                                 tray->showMessageTip( id, codec->toUnicode(frd->getNick().c_str()), i18n( "I am busy ..." ) );
//X 				EvaTipWindow *tip = new EvaTipWindow(images, codec->toUnicode(frd->getNick().c_str()), 
//X 									id, frd->getFace(), i18n("I am busy ..."));
//X 				connect(tip, SIGNAL(requestChat(const unsigned int)), SLOT(slotRequestChat(const unsigned int)));
//X 				tip->show();
//X 			}
			break;
		case QQ_FRIEND_STATUS_INVISIBLE:
			changeToInvisible(id);
			break;
		}
	}else{
		printf("buddy %d not in list\n", id);
	}
}

void EvaMainWindow::changeToOnline(unsigned int id)
{
	if(m_buddyLv)
		m_buddyLv->friendStatusChanged(id);
	slotUpdateBuddyStat();
}

void EvaMainWindow::changeToOffline(unsigned int id)
{
	if(m_buddyLv)
		m_buddyLv->friendStatusChanged(id);
	slotUpdateBuddyStat();
}

void EvaMainWindow::changeToLeave(unsigned int id)
{
	if(m_buddyLv)
		m_buddyLv->friendStatusChanged(id);
	slotUpdateBuddyStat();
}

void EvaMainWindow::changeToInvisible(unsigned int id)
{
	if(m_buddyLv)
		m_buddyLv->friendStatusChanged(id);
	slotUpdateBuddyStat();
}

void EvaMainWindow::newMessage(unsigned int id)
{
	if(m_buddyLv)
		m_buddyLv->newMessage(id);
	if(m_recentLv)
		m_recentLv->newMessage( id);
}

void EvaMainWindow::gotMessage(unsigned int id)
{
	if(m_buddyLv)
		m_buddyLv->getMessage(id);
	if(m_recentLv)
		m_recentLv->getMessage( id);
}
void EvaMainWindow::newQunMessage(unsigned int id)
{
	if(m_qunLv)
		m_qunLv->newMessage( id);
	if(m_recentLv)
		m_recentLv->newQunMessage( id);
}

void EvaMainWindow::gotQunMessage(unsigned int id)
{
	if(m_qunLv)
		m_qunLv->getMessage( id);
	if(m_recentLv)
		m_recentLv->getQunMessage( id);
}

void EvaMainWindow::deleteBuddy(unsigned int id)
{
	if(m_buddyLv)
		m_buddyLv->deleteBuddy(id);
}

void EvaMainWindow::online()
{
	if(pixOnline) statusBar->tbStatus->setIconSet(QIcon(*pixOnline));
	//statusBar->tbStatus->setText(i18n( "Online"));
}

void EvaMainWindow::offline()
{
	if(pixOffline) statusBar->tbStatus->setIconSet(QIcon(*pixOffline));
	//pbStatus->setText(i18n( "Offline"));
}

void EvaMainWindow::leave()
{
	if(pixLeave) statusBar->tbStatus->setIconSet(QIcon(*pixLeave));
	//pbStatus->setText(i18n( "Leave"));
}

void EvaMainWindow::invisible()
{
	if(pixInvisible) statusBar->tbStatus->setIconSet(QIcon(*pixInvisible));
	//pbStatus->setText(i18n( "Invisible"));
}

void EvaMainWindow::slotTbMyFace()
{
	if(qqNum)
		emit requestDetails(qqNum);
}

void EvaMainWindow::slotSearch()
{
	emit requestSearch();
}

void EvaMainWindow::slotSystemMessages()
{
	emit requestSystemMessages();
}

void EvaMainWindow::clearQunList( )
{
	if(m_qunLv) m_qunLv->clear();
}

QRect EvaMainWindow::tipRect( const QPoint & /*pos*/ )
{
	// at the moment we only have one button which has tooltip, so just return the rect of the button
	return tbMyFace->rect();
}

QString EvaMainWindow::myInfoTip( )
{
	//const QQFriend *frd = (EvaMain::session->getUser()->getFriendList()).getFriend(qqNum); 
// 	if(!EvaMain::session->getUser()->loginManager()->isCommandFinished(QQ_CMD_GET_USER_INFO))
// 		return "Eva";
	if(!EvaMain::session->getLoginManager()->isLoggedIn())
		return "Eva";
	QString tip = "<qt>";
	//EvaIPSeeker ipAddr(EvaGlobal::getDirPath().latin1());
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	QString nickName = codec->toUnicode(EvaMain::session->getUser()->getDetails().at(ContactInfo::Info_nick).c_str());
	//QString addr = codec->toUnicode(ipAddr.getIPLocation(frd->getIP()).c_str());
	//if(addr.length()<4) addr = "0.0.0.0";
	
	QString htmlName = nickName;
	EvaHtmlParser parser;
	parser.setAbsImagePath(EvaMain::images->getSmileyPath());
	parser.convertToHtml(htmlName, false, true);
	
	QString signature = codec->toUnicode(EvaMain::session->getUser()->getSignature().c_str());
	if(signature.length() > 40)
		signature = signature.left(37) + "...";
	if(!signature.isEmpty())
		signature = "[" + signature + "]";
	
	QString facePath = "<img src=\"" + EvaMain::images->getFacePath() + "/" +
			QString::number(EvaMain::images->getFaceFileIndex(atoi(EvaMain::session->getUser()->getDetails().at(ContactInfo::Info_face).c_str()))) + 
			".png\"></img>";
			
	if(EvaMain::session->getUser()->hasUserHead() && EvaMain::getInstance()->getUHManager()){
		QString uhFileName = EvaMain::getInstance()->getUHManager()->getFileName(EvaMain::session->getUser()->getQQ());
		if(!uhFileName.isEmpty())
			facePath = "<img src=\"" + uhFileName + "\"></img>";
	}
	
	int suns, moons, stars;
	EvaUtil::calcSuns(EvaMain::session->getUser()->getLevel(), &suns, &moons, &stars);
	QString strSun =  "<img src=\"" + EvaMain::images->getIconFullPath("TIME_SUN") + "\"></img>";
	QString strMoon = "<img src=\"" + EvaMain::images->getIconFullPath("TIME_MOON") + "\"></img>";
	QString strStar = "<img src=\"" + EvaMain::images->getIconFullPath("TIME_STAR") + "\"></img>";
	QString level;
	for(int i=0; i<suns; i++){
		level += strSun;
	}
	for(int i=0; i<moons; i++){
		level += strMoon;
	}
	for(int i=0; i<stars; i++){
		level += strStar;
	}
	int seconds = EvaMain::session->getUser()->getOnlineTime();
	tip += "<table width = 260><tr><td width=60 align = center valign = middle>" + facePath + 
		"</td><td align = left valign = middle><b><font color = blue>"+
		i18n("QQ") +": </font></b>"+ QString::number(EvaMain::session->getUser()->getQQ()) +"<br><b><font color = blue>"+
		i18n("Nickname:") + " </font></b>"+htmlName +"<br>" + signature + "<br><b><font color = blue>"+ 
		i18n("Level") +": </font></b>"+ level + "  (" + QString::number(EvaMain::session->getUser()->getLevel()) +")<br><b><font color = blue>"+
		i18n("Online Time") +": </font></b>"+ QString::number(seconds/3600)+
			i18n(" Hours ")+QString::number((seconds%3600)/60)+i18n(" min ")+"<br><b><font color = blue>";
	//	i18n("Level Up") +": </font></b>"+ QString::number(EvaMain::session->getUser()->getHoursToLevelUp())+
//							" Hours<br><b><font color = blue>" + "</td></tr></table>"; 
	tip += "</qt>";
	return tip;
}

void EvaMainWindow::resizeEvent( QResizeEvent * event )
{
	QSize s = event->size();
	if(m_buddyLv) m_buddyLv->setColumnWidth(0, s.width());
	if(m_qunLv) m_qunLv->setColumnWidth(0, s.width());
	if(m_recentLv) m_recentLv->setColumnWidth(0, s.width());
}

void EvaMainWindow::moveEvent( QMoveEvent * /*event*/ )
{
        setPosAndSize();
}

void EvaMainWindow::setPosAndSize()
{
        nowPosition = pos();
        nowSize = size();
}

void EvaMainWindow::ShowTab(int key)
{
	if(mainDisplay && mainDisplay->tab)
//X 		mainDisplay->tab->changeTabTo( key );
		mainDisplay->tab->setCurrentIndex( key );
}

void EvaMainWindow::loadContacts( )
{
// 	EvaContactListView *w = (EvaContactListView *)(tab->wsView->widget(m_buddyTabKey));
// 	if(w){ // w should be same as  m_buddyLv
// 		tab->wsView->removeWidget(w);
// 		delete w;
// 	}
	mainDisplay->tab->removeTab( m_buddyTabKey );
	m_buddyLv = new EvaContactListView(mainDisplay->tab, "lv");
	QString name = i18n("Buddy List");
	//QPixmap *p= EvaMain::images->getIcon("ONLINE");
	QPixmap *p= EvaMain::images->getIcon("CONTACT");
	m_buddyTabKey = mainDisplay->tab->addTab(name, *p, name, m_buddyLv);
//X 	m_buddyTabKey = mainDisplay->tab->addTab(m_buddyLv, QIcon( *p ), QString::null);

	m_buddyLv->loadContacts();

	if(m_buddyLv) m_buddyLv->setColumnWidth(0, width());
	mainDisplay->tab->setCurrentIndex( m_buddyTabKey );

	QObject::connect(m_buddyLv, SIGNAL(groupDeleted(const int)), this, SIGNAL(groupDeleted(const int)));
	//QObject::connect(m_buddyLv, SIGNAL(groupAdded(QString, int)), this, SIGNAL(groupAdded(QString, int)));
	QObject::connect(m_buddyLv, SIGNAL(groupChanged(const unsigned int, int)), this, SIGNAL(groupChanged(const unsigned int, int)));
	QObject::connect(m_buddyLv, SIGNAL(groupRenamed(QString, int)), this, SIGNAL(groupRenamed(QString, int)));

	QObject::connect(m_buddyLv, SIGNAL(deleteMeFrom(const unsigned int )), this, SIGNAL(deleteMeFrom(const unsigned int)));
	QObject::connect(m_buddyLv, SIGNAL(requestDelete(const unsigned int)), this, SIGNAL(requestDelete(const unsigned int)));

	QObject::connect(m_buddyLv, SIGNAL(requestChat(const unsigned int)), this, SIGNAL(requestChat(const unsigned int)));
	QObject::connect(m_buddyLv, SIGNAL(requestSendFile(const unsigned int)), this, SIGNAL(requestSendFile(const unsigned int)));
	QObject::connect(m_buddyLv, SIGNAL(requestLevel(const unsigned int)), this, SIGNAL(requestLevel(const unsigned int)));
	QObject::connect(m_buddyLv, SIGNAL(requestDetails(const unsigned int)), this, SIGNAL(requestDetails(const unsigned int)));
	QObject::connect(m_buddyLv, SIGNAL(requestHistory(const unsigned int)), this, SIGNAL(requestHistory(const unsigned int)));
	QObject::connect(m_buddyLv, SIGNAL(requestModifyMemo(const unsigned int)), this, SIGNAL(requestModifyMemo(const unsigned int)));
	
}

void EvaMainWindow::addBuddy( const unsigned int id )
{
	m_buddyLv->buddyAdded(id);
}

void EvaMainWindow::loadQuns( )
{
// 	EvaQunsListView *w = (EvaQunsListView *)(tab->wsView->widget(m_qunTabKey));
// 	if(w){ // w should be same as  m_buddyLv
// 		tab->wsView->removeWidget(w);
// 		delete w;
// 	}
	mainDisplay->tab->removeTab( m_qunTabKey );
	m_qunLv = new EvaQunsListView(mainDisplay->tab, "qunLv");
	QString name = i18n("Qun List");
	//QPixmap *p= EvaMain::images->getIcon("QUN");
	QPixmap *p= EvaMain::images->getIcon("QUNS");
	m_qunTabKey = mainDisplay->tab->addTab(name, *p, name, m_qunLv);
//X 	m_qunTabKey = mainDisplay->tab->addTab(m_qunLv, QIcon( *p ), "");
	m_qunLv->loadAllQuns();

	m_qunLv->setColumnWidth(0, width());


	QObject::connect(m_qunLv, SIGNAL(requestQunChat(const unsigned int)), this, SIGNAL(requestQunChat(const unsigned int)));
	QObject::connect(m_qunLv, SIGNAL(requestQunDetails(const unsigned int)), this, SIGNAL(requestQunDetails(const unsigned int)));
	QObject::connect(m_qunLv, SIGNAL(requestQunExit(const unsigned int)), this, SIGNAL(requestQunExit(const unsigned int)));
	QObject::connect(m_qunLv, SIGNAL(requestQunCreate()), this, SIGNAL(requestQunCreate()));
	QObject::connect(m_qunLv, SIGNAL(requestQunHistory(const unsigned int)), this, SIGNAL(requestQunHistory(const unsigned int)));
}

void EvaMainWindow::loadRecentContacts( )
{
// 	EvaRecentContactsListView *w = (EvaRecentContactsListView *)(tab->wsView->widget(m_recentTabKey));
// 	if(w){
// 		tab->wsView->removeWidget(w);
// 		delete w;
// 	}
	mainDisplay->tab->removeTab( m_recentTabKey );
	m_recentLv = new EvaRecentContactsListView(mainDisplay->tab, "recentLv");
	QString name = i18n("Recent Contacts List");
	QPixmap *p= EvaMain::images->getIcon("MSG");
	m_recentTabKey = mainDisplay->tab->addTab(name, *p, name, m_recentLv);
//X 	m_recentTabKey = mainDisplay->tab->addTab(m_recentLv, QIcon( *p ), "");

	m_recentLv->loadRecentContacts();

	m_recentLv->setColumnWidth(0, width());

	if(m_qunLv){
		QObject::connect(m_qunLv, SIGNAL(requestQunChat(const unsigned int)), m_recentLv, SLOT(getQunMessage(const unsigned int)));
		QObject::connect(m_recentLv, SIGNAL(requestQunChat(const unsigned int)), m_qunLv, SLOT(getMessage(const unsigned int)));
	}

	if(m_buddyLv){
		QObject::connect(m_buddyLv, SIGNAL(requestChat(const unsigned int)), m_recentLv, SLOT(getMessage(const unsigned int)));
		QObject::connect(m_recentLv, SIGNAL(requestChat(const unsigned int)), m_buddyLv, SLOT(getMessage(const unsigned int)));
	}

	QObject::connect(m_recentLv, SIGNAL(requestDelete(const unsigned int)), this, SIGNAL(requestDelete(const unsigned int)));

	QObject::connect(m_recentLv, SIGNAL(requestChat(const unsigned int)), this, SIGNAL(requestChat(const unsigned int)));
	QObject::connect(m_recentLv, SIGNAL(requestSendFile(const unsigned int)), this, SIGNAL(requestSendFile(const unsigned int)));
	QObject::connect(m_recentLv, SIGNAL(requestLevel(const unsigned int)), this, SIGNAL(requestLevel(const unsigned int)));
	QObject::connect(m_recentLv, SIGNAL(requestDetails(const unsigned int)), this, SIGNAL(requestDetails(const unsigned int)));
	QObject::connect(m_recentLv, SIGNAL(requestHistory(const unsigned int)), this, SIGNAL(requestHistory(const unsigned int)));
	QObject::connect(m_recentLv, SIGNAL(requestModifyMemo(const unsigned int)), this, SIGNAL(requestModifyMemo(const unsigned int)));

	QObject::connect(m_recentLv, SIGNAL(requestQunChat(const unsigned int)), this, SIGNAL(requestQunChat(const unsigned int)));
	QObject::connect(m_recentLv, SIGNAL(requestQunDetails(const unsigned int)), this, SIGNAL(requestQunDetails(const unsigned int)));
	QObject::connect(m_recentLv, SIGNAL(requestQunExit(const unsigned int)), this, SIGNAL(requestQunExit(const unsigned int)));
	QObject::connect(m_recentLv, SIGNAL(requestQunCreate()), this, SIGNAL(requestQunCreate()));
	QObject::connect(m_recentLv, SIGNAL(requestQunHistory(const unsigned int)), this, SIGNAL(requestQunHistory(const unsigned int)));

}

void EvaMainWindow::addBuddyToRecentList( const unsigned int id, bool isMyWord )
{
	if(m_recentLv)
		m_recentLv->newMessage(id, isMyWord);
}

void EvaMainWindow::addQunToRecentList( const unsigned int id, bool isMyWord )
{
	if(m_recentLv)
		m_recentLv->newQunMessage(id, isMyWord);
}

bool EvaMainWindow::removeTab( int id )
{
//X 	bool result = false;
	if(mainDisplay && mainDisplay->tab ){
//X 		result = mainDisplay->tab->removeTab( id);
		mainDisplay->tab->removeTab( id);
		mainDisplay->tab->setCurrentIndex(m_recentTabKey);
//X 		m_customTabs.remove(id);
//X 		m_tabScriptMap.remove(id);
	}
	
//X 	return result;
        return true;
}

void EvaMainWindow::updateContacts( )
{
	if(m_buddyLv) m_buddyLv->updateContacts();
}

void EvaMainWindow::updateQuns( )
{
	if(m_qunLv) m_qunLv->loadAllQuns();
}

void EvaMainWindow::updateRecentContacts( )
{
	if(m_recentLv) m_recentLv->loadRecentContacts();
}

void EvaMainWindow::slotFaceSizeChanged()
{
	if(m_buddyLv) m_buddyLv->slotFaceSizeChanged();
	if(m_recentLv) m_recentLv->loadRecentContacts();
}


void EvaMainWindow::showInfoFrame( bool showInfo )
{
	if(mainDisplay){
		mainDisplay->showInfoFrame(showInfo);
//X 		mainDisplay->loginPage->adjustSize();
	}
}

void EvaMainWindow::UpdateLoginInfo( int /*value*/, const QString & /*msg*/ )
{
	if(mainDisplay){
//X 		mainDisplay->loginPage->update(value, msg);
//X 		mainDisplay->loginPage->adjustSize();
	}
}

void EvaMainWindow::slotSystemClicked( )
{
	sysMenu->popup(QCursor::pos());
}

void EvaMainWindow::slotStatusClicked( )
{
	statusMenu->popup(QCursor::pos());
}

//DCOP functions
//X int EvaMainWindow::addTab( QString scriptName, QString name, QString image, QString contents )
//X {
//X 	EvaScriptWidget *view = new EvaScriptWidget(mainDisplay->tab, name);
//X 	view->setJScriptEnabled( true);
//X 	view->setJavaEnabled( true);
//X 	view->setMetaRefreshEnabled( true);
//X 	view->setPluginsEnabled( true);
// 	QObject::connect(view->browserExtension(), 
// 									 SIGNAL(openURLRequest(const KURL &, const KParts::URLArgs &)),
// 										view,
// 									SLOT(openURL( const KURL &)));
//X 	QPixmap p(image);
//X 	int id = mainDisplay->tab->addTab(name, p, name, view->view());
//X 	m_customTabs[id] = view;
//X 	m_tabScriptMap[id] = scriptName;
//X 	view->begin();
//X 	view->write(contents);
//X 	view->end();
// 	QString script = "var browser=navigator.appName\n var b_version=navigator.appVersion\n var version=parseFloat(b_version)\n document.write(\"Browser name: \"+ browser)\n document.write(\"<br />\")\n document.write(\"Browser version: \"+ version)\n";
// 	bool result = (view->executeScript(DOM::Node(), script)).toBool();
// 	if(result) printf("script true\n");
// 	else printf("script false\n");
//X 	return id;
//X }
//X 
//X bool EvaMainWindow::updateTab( int id, QString contents )
//X {
	//KHTMLView *view = (KHTMLPart *)mainDisplay->tab->getWidget( id);
//X 	QMap<int, EvaScriptWidget *>::Iterator it = m_customTabs.find( id);
//X 	if(it == m_customTabs.end()) return false;
//X 	EvaScriptWidget *view = it.data();
//X 	if(view){
//X 		view->begin();
//X 		view->write(contents);
//X 		view->end();
//X 		return true;
//X 	}
//X 	return false;
//X }
//X 
//X bool EvaMainWindow::isTabExisted( int id )
//X {
//X 	if(mainDisplay->tab->getWidget( id))
//X 		return true;
//X 	else
//X 		return false;
//X }

//X void EvaMainWindow::bringToFront( int id )
//X {
//X 	//mainDisplay->tab->changeTabTo(id);
//X 	ShowTab(id);
//X }
//X 
//X void EvaMainWindow::openChatWindow( unsigned int id, bool isQun )
//X {
//X 	if(g_eva){
//X 		if(isQun)
//X 			g_eva->slotRequestQunChat(id);
//X 		else
//X 			g_eva->slotRequestChat( id);
//X 	}
//X }
//X 
//X void EvaMainWindow::addButton( QString scriptName, QString buttonName, QString image, QString tip )
//X {
//X 	EvaMain::g_ChatWindowManager->addButton(scriptName, buttonName, image, tip);
//X }
//X 
//X void EvaMainWindow::removeButton( QString scriptName, QString buttonName )
//X {
//X 	EvaMain::g_ChatWindowManager->removeButton(scriptName, buttonName);
//X }
//X 
//X void EvaMainWindow::removeButtons(QString scriptName)
//X {
//X 	EvaMain::g_ChatWindowManager->removeButton(scriptName);
//X }
//X 
//X void EvaMainWindow::updateStatusBar( QString message )
//X {
//X 	statusBar->lblNotification->setText(message);
//X }
//X 
//X void EvaMainWindow::removeTabs( QString scriptName )
//X {
//X 	QMap<int, QString>::Iterator it = m_tabScriptMap.begin();
//X 	for(it = m_tabScriptMap.begin(); it != m_tabScriptMap.end(); it++){
//X 		if(it.data() == scriptName)
//X 			removeTab(it.key());
//X 	}
//X 	while(it != m_tabScriptMap.end()){
//X 		if(it.data() == scriptName){
//X 			removeTab(it.key());
//X 			it = m_tabScriptMap.begin();
//X 		}
//X 	}
//X }
//X 
//X void EvaMainWindow::openUrl( int id, QString _url )
//X {
//X 	if(_url.isEmpty()) return;
//X 	EvaScriptWidget *part = getCustomTab(id);
//X 	
//X 	if(part){
//X 		KURL url = KURL::fromPathOrURL(_url);
//X 		part->openURL(url);
//X 	}
//X }
//X 
//X EvaScriptWidget * EvaMainWindow::getCustomTab( int id )
//X {
//X 	QMap<int, EvaScriptWidget *>::Iterator it = m_customTabs.find( id);
//X 	if(it == m_customTabs.end()) return NULL;
//X 	return it.data();
//X }
//X 
