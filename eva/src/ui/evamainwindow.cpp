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

#include <stdlib.h>

#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qheader.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qevent.h>
#include <qcursor.h>

#include <klocale.h>
#include <kpopupmenu.h>
#include <khtmlview.h>
#include <khtml_part.h>
#include <kdebug.h>

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
#include "evascriptwidget.h"

MainWindowTip::MainWindowTip(QWidget *parent)
	: QToolTip(parent) 
{
}

void MainWindowTip::maybeTip(const QPoint &pos)
{
	if(!parentWidget()->inherits("EvaMainWindow"))
		return;
	QRect r( ((EvaMainWindow *)parentWidget())->tipRect(pos) );
	if(!r.isValid()) 
		return;
	tip(r, ((EvaMainWindow *)parentWidget())->myInfoTip());
}

EvaMainWindow::EvaMainWindow(QWidget* parent, const char* name, WFlags fl)
	: DCOPObject("View")
		, EvaMainUIBase(parent, name, fl)
	, sysMenu(NULL), statusMenu(NULL)
	, pixOnline(NULL), pixOffline(NULL), pixLeave(NULL), pixInvisible(NULL)
{
	qqNum = 0;
	myTip = new MainWindowTip(this);

	pixOnline = EvaMain::images->getIcon("ONLINE");
	pixOffline = EvaMain::images->getIcon("OFFLINE");
	pixLeave = EvaMain::images->getIcon("LEAVE");
	pixInvisible = EvaMain::images->getIcon("INVISIBLE");

	statusBar->tbSearch->setIconSet(QIconSet(*EvaMain::images->getIcon("FIND")));
	statusBar->tbSysMsg->setIconSet(QIconSet(*EvaMain::images->getIcon("SYSTEM_MSG")));
	statusBar->tbSystem->setIconSet(QIconSet(*EvaMain::images->getIcon("SYSTEM_OPTIONS")));

	m_buddyLv = 0;
	m_qunLv = 0;
	m_recentLv = 0;

	m_buddyTabKey = 0;
	m_qunTabKey = 1;
	m_recentTabKey = 2;
// 	loadContacts();
// 	loadQuns();
// 	loadRecentContacts();

	QObject::connect(statusBar->tbSearch, SIGNAL(clicked()), this, SLOT(slotSearch()));
	QObject::connect(statusBar->tbSysMsg, SIGNAL(clicked()), this, SLOT(slotSystemMessages()));
	QObject::connect(statusBar->tbSystem, SIGNAL(clicked()), this, SLOT(slotSystemClicked()));
	QObject::connect(statusBar->tbStatus, SIGNAL(clicked()), this, SLOT(slotStatusClicked()));
	QObject::connect(tbMyFace, SIGNAL(clicked()), this, SLOT(slotTbMyFace()));

	offline();
}

EvaMainWindow::~EvaMainWindow()
{
	m_customTabs.clear();
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

	tbMyFace->setIconSet( QIconSet(*pix));


	loadContacts();
	loadQuns();
	loadRecentContacts();
}

void EvaMainWindow::slotUpdateBuddyStat()
{
	int all = EvaMain::user->getFriendList().numberOfFriends();
	int onlines = EvaMain::user->getFriendList().numberOfOnlines();
	tlQQ->setText(" ( "+ QString::number(onlines) + "/" + QString::number(all) + ")");
}

void EvaMainWindow::setSystemMenu( KPopupMenu *sys)
{
	if(sys){
		sysMenu = sys;
		statusBar->tbSystem->setPopup(sys);
	}
}

void EvaMainWindow::setStatusMenu( KPopupMenu *status)
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
	EvaUser *user = EvaMain::user;
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
		
	if(m_buddyLv)
		m_buddyLv->friendStatusChanged(id);
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
	if(pixOnline) statusBar->tbStatus->setIconSet(QIconSet(*pixOnline));
	//statusBar->tbStatus->setText(i18n( "Online"));
}

void EvaMainWindow::offline()
{
	if(pixOffline) statusBar->tbStatus->setIconSet(QIconSet(*pixOffline));
	//pbStatus->setText(i18n( "Offline"));
}

void EvaMainWindow::leave()
{
	if(pixLeave) statusBar->tbStatus->setIconSet(QIconSet(*pixLeave));
	//pbStatus->setText(i18n( "Leave"));
}

void EvaMainWindow::invisible()
{
	if(pixInvisible) statusBar->tbStatus->setIconSet(QIconSet(*pixInvisible));
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
	//const QQFriend *frd = (EvaMain::user->getFriendList()).getFriend(qqNum); 
// 	if(!EvaMain::user->loginManager()->isCommandFinished(QQ_CMD_GET_USER_INFO))
// 		return "Eva";
	if(!GetLoginManager()->isLoggedIn())
		return "Eva";
	QString tip = "<qt>";
	//EvaIPSeeker ipAddr(EvaGlobal::getDirPath().latin1());
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	QString nickName = codec->toUnicode(EvaMain::user->getDetails().at(ContactInfo::Info_nick).c_str());
	//QString addr = codec->toUnicode(ipAddr.getIPLocation(frd->getIP()).c_str());
	//if(addr.length()<4) addr = "0.0.0.0";
	
	QString htmlName = nickName;
	EvaHtmlParser parser;
	parser.setAbsImagePath(EvaMain::images->getSmileyPath());
	parser.convertToHtml(htmlName, false, true);
	
	QString signature = codec->toUnicode(EvaMain::user->getSignature().c_str());
	if(signature.length() > 40)
		signature = signature.left(37) + "...";
	if(!signature.isEmpty())
		signature = "[" + signature + "]";
	
	QString facePath = "<img src=\"" + EvaMain::images->getFacePath() + "/" +
			QString::number(EvaMain::images->getFaceFileIndex(atoi(EvaMain::user->getDetails().at(ContactInfo::Info_face).c_str()))) + 
			".png\"></img>";
			
	if(EvaMain::user->hasUserHead() && EvaMain::uhManager){
		QString uhFileName = EvaMain::uhManager->getFileName(EvaMain::user->getQQ());
		if(!uhFileName.isEmpty())
			facePath = "<img src=\"" + uhFileName + "\"></img>";
	}
	
	int suns, moons, stars;
	EvaUtil::calcSuns(EvaMain::user->getLevel(), &suns, &moons, &stars);
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
	
	tip += "<table width = 260><tr><td width=60 align = center valign = middle>" + facePath + 
		"</td><td align = left valign = middle><b><font color = blue>"+
		i18n("QQ") +": </font></b>"+ QString::number(EvaMain::user->getQQ()) +"<br><b><font color = blue>"+
		i18n("Nickname:") + " </font></b>"+htmlName +"<br>" + signature + "<br><b><font color = blue>"+ 
		i18n("Level") +": </font></b>"+ level + "  (" + QString::number(EvaMain::user->getLevel()) +")<br><b><font color = blue>"+
		i18n("Online Time") +": </font></b>"+ QString::number(EvaMain::user->getOnlineTime()/3600)+
									" Hours<br><b><font color = blue>"+
		i18n("Level Up") +": </font></b>"+ QString::number(EvaMain::user->getHoursToLevelUp())+
							" Hours<br><b><font color = blue>" + "</td></tr></table>"; 
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

void EvaMainWindow::moveEvent( QMoveEvent */*event*/ )
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
		mainDisplay->tab->changeTabTo( key );
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

	m_buddyLv->loadContacts();

	if(m_buddyLv) m_buddyLv->setColumnWidth(0, width());
	mainDisplay->tab->changeTabTo( m_buddyTabKey );

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
	bool result = false;
	if(mainDisplay && mainDisplay->tab ){
		result = mainDisplay->tab->removeTab( id);
		mainDisplay->tab->changeTabTo(m_recentTabKey);
		m_customTabs.remove(id);
		m_tabScriptMap.remove(id);
	}
	
	return result;
}

void EvaMainWindow::updateContacts( )
{
	if(m_buddyLv) m_buddyLv->loadContacts();
}

void EvaMainWindow::updateQuns( )
{
	if(m_qunLv) m_qunLv->loadAllQuns();
}

void EvaMainWindow::updateRecentContacts( )
{
	if(m_recentLv) m_recentLv->loadRecentContacts();
}

void EvaMainWindow::showInfoFrame( bool showInfo )
{
	if(mainDisplay){
		mainDisplay->showInfoFrame(showInfo);
		mainDisplay->loginPage->adjustSize();
	}
}

void EvaMainWindow::UpdateLoginInfo( int value, const QString & msg )
{
	if(mainDisplay){
		mainDisplay->loginPage->update(value, msg);
		mainDisplay->loginPage->adjustSize();
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
int EvaMainWindow::addTab( QString scriptName, QString name, QString image, QString contents )
{
	EvaScriptWidget *view = new EvaScriptWidget(mainDisplay->tab, name);
	view->setJScriptEnabled( true);
	view->setJavaEnabled( true);
	view->setMetaRefreshEnabled( true);
	view->setPluginsEnabled( true);
// 	QObject::connect(view->browserExtension(), 
// 									 SIGNAL(openURLRequest(const KURL &, const KParts::URLArgs &)),
// 										view,
// 									SLOT(openURL( const KURL &)));
	QPixmap p(image);
	int id = mainDisplay->tab->addTab(name, p, name, view->view());
	m_customTabs[id] = view;
	m_tabScriptMap[id] = scriptName;
	view->begin();
	view->write(contents);
	view->end();
// 	QString script = "var browser=navigator.appName\n var b_version=navigator.appVersion\n var version=parseFloat(b_version)\n document.write(\"Browser name: \"+ browser)\n document.write(\"<br />\")\n document.write(\"Browser version: \"+ version)\n";
// 	bool result = (view->executeScript(DOM::Node(), script)).toBool();
// 	if(result) printf("script true\n");
// 	else printf("script false\n");
	return id;
}

bool EvaMainWindow::updateTab( int id, QString contents )
{
	//KHTMLView *view = (KHTMLPart *)mainDisplay->tab->getWidget( id);
	QMap<int, EvaScriptWidget *>::Iterator it = m_customTabs.find( id);
	if(it == m_customTabs.end()) return false;
	EvaScriptWidget *view = it.data();
	if(view){
		view->begin();
		view->write(contents);
		view->end();
		return true;
	}
	return false;
}

bool EvaMainWindow::isTabExisted( int id )
{
	if(mainDisplay->tab->getWidget( id))
		return true;
	else
		return false;
}

void EvaMainWindow::bringToFront( int id )
{
	//mainDisplay->tab->changeTabTo(id);
	ShowTab(id);
}

void EvaMainWindow::openChatWindow( unsigned int id, bool isQun )
{
	if(g_eva){
		if(isQun)
			g_eva->slotRequestQunChat(id);
		else
			g_eva->slotRequestChat( id);
	}
}

void EvaMainWindow::addButton( QString scriptName, QString buttonName, QString image, QString tip )
{
	EvaMain::g_ChatWindowManager->addButton(scriptName, buttonName, image, tip);
}

void EvaMainWindow::removeButton( QString scriptName, QString buttonName )
{
	EvaMain::g_ChatWindowManager->removeButton(scriptName, buttonName);
}

void EvaMainWindow::removeButtons(QString scriptName)
{
	EvaMain::g_ChatWindowManager->removeButton(scriptName);
}

void EvaMainWindow::updateStatusBar( QString message )
{
	statusBar->lblNotification->setText(message);
}

void EvaMainWindow::removeTabs( QString scriptName )
{
	QMap<int, QString>::Iterator it = m_tabScriptMap.begin();
	for(it = m_tabScriptMap.begin(); it != m_tabScriptMap.end(); it++){
		if(it.data() == scriptName)
			removeTab(it.key());
	}
	while(it != m_tabScriptMap.end()){
		if(it.data() == scriptName){
			removeTab(it.key());
			it = m_tabScriptMap.begin();
		}
	}
}

void EvaMainWindow::openUrl( int id, QString _url )
{
	if(_url.isEmpty()) return;
	EvaScriptWidget *part = getCustomTab(id);
	
	if(part){
		KURL url = KURL::fromPathOrURL(_url);
		part->openURL(url);
	}
}

EvaScriptWidget * EvaMainWindow::getCustomTab( int id )
{
	QMap<int, EvaScriptWidget *>::Iterator it = m_customTabs.find( id);
	if(it == m_customTabs.end()) return NULL;
	return it.data();
}

