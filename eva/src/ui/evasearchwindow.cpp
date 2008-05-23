/***************************************************************************
 *   Copyright (C) 2005 by yunfan,casper                                  *
 *   yunfan_zg@163.com                                                     *
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


#include "evasearchwindow.h"
#include "evaresource.h"
#include <stdlib.h>
#include <qlabel.h>
#include <qwidgetstack.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qtable.h>
#include <qcheckbox.h>
#include <qtoolbutton.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qimage.h>
#include <qtextcodec.h>
#include <qtabwidget.h>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qrect.h>
#include <kmessagebox.h>
#include <krun.h>
#include <klocale.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <kapp.h>

#include "../evamain.h"
#include "../evaaddingmanager.h"
#include "evaqunlist.h"
#include "qundetailswindow.h"

CityList::CityList( )
{
	isOK = loadList();
}

std::list< CityListElement > CityList::getCityList( unsigned short province )
{
	std::list< CityListElement > results;
	std::map<unsigned short, std::list< CityListElement> >::iterator iter;
	for(iter=list.begin(); iter!=list.end(); ++iter)
		if(province == iter->first){
			results = iter->second;
			break;
		}
	
	return results;
}

const bool CityList::loadList( )
{
	list.clear();
	QString filePath = KGlobal::dirs()->findResource("data", QString::fromLatin1("eva/citylist"));
	QFile file(filePath);    
	if(!file.open(IO_ReadOnly)){
		return false;
	}
	
	QTextStream stream(&file);
	stream.setCodec(QTextCodec::codecForName("GB18030"));
	QString line;
	QStringList lineList;
	
	std::list<CityListElement> tmpList;
	unsigned int tmpProv;
	unsigned int province = 1;
	while(!stream.atEnd()){
		line = stream.readLine().stripWhiteSpace();
			
		lineList = QStringList::split(",", line);
				
		if(lineList.size() != 3)
			continue;
		
		bool ok;
		CityListElement element;
		tmpProv = (unsigned short)(lineList[0].stripWhiteSpace().toInt(&ok));
		if(!ok) continue;
		
		if( tmpProv != province){	
			list[province++] = tmpList;
			tmpList.clear();
		}
		
		element.city = (unsigned short)(lineList[1].stripWhiteSpace().toInt(&ok));
		if(!ok) continue;
		element.name = lineList[2].stripWhiteSpace();
		tmpList.push_back(element);	//store the city informations to list
	}
	list[province] = tmpList;
	file.close();
	return true; 
}
/*----------------------------------------------------------*/
EvaSearchWindow::EvaSearchWindow(EvaImageResource *res, const int onlineUsers, QWidget* parent, 
		const char* name, WFlags fl)
	: EvaSearchUIBase(parent,name, fl),
	m_BSearchType(B_CUSTOM),
	m_BCurrentPage(0),
	m_ShowAllBasicUsers(false),
	m_ACurrentPage(0),
	m_ShowAllAdvancedUsers(false),
	m_OnlineUser(true), m_HasCam(false), m_ProvinceIndex(0), m_CityIndex(0), m_AgeIndex(0), m_GenderIndex(0),
	m_QSearchType(Q_CATEGORY)
{
	if(onlineUsers>0)
		lblOnlineNum->setText(QString::number(onlineUsers));
	else
		lblOnlineNum->setText(i18n("Unknown"));
	
	images = res;
	lblLogo->setPixmap(*images->getIcon("SEARCH_LOGO"));
	adjustInterface();
	//initialise city info
	cityData = new CityList();
	if(!cityData->isLoaded()){
		delete cityData;
		cityData = NULL;
	}
	//initialise qun category list
	loadCategoryList();
	QObject::connect(rbSearchOnline, SIGNAL(clicked()), this, SLOT(slotRbSearchOnlineClicked()));
	QObject::connect(rbCustomSearch, SIGNAL(clicked()), this, SLOT(slotRbCustomSearchClicked()));
	QObject::connect(rbAccuratelySearch, SIGNAL(clicked()), this, SLOT(slotRbAccuratelySearchClicked()));
	
	QObject::connect(rbSearchByCategory, SIGNAL(clicked()), this, SLOT(slotRbSearchByCategoryClicked()));
	QObject::connect(rbSearchAlumni, SIGNAL(clicked()), this, SLOT(slotRbSearchAlumniClicked()));
	QObject::connect(rbSearchFrdCenter, SIGNAL(clicked()), this, SLOT(slotRbSearchFrdCenterClicked()));
	
	QObject::connect(pbSearch, SIGNAL(clicked()), this, SLOT(slotPbSearchClicked()));
	QObject::connect(pbClose, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(pbLastStep, SIGNAL(clicked()), this, SLOT(slotPbLastStepClicked()));
	QObject::connect(twSearchMain, SIGNAL(currentChanged(QWidget*)), this, SLOT( slotCurrentChanged(QWidget*)));
	
	QObject::connect(tbBSNext, SIGNAL( clicked() ), this, SLOT( slotTbBSNextClicked()));
	QObject::connect(tbBSPrev, SIGNAL( clicked() ), this, SLOT( slotTbBSPrevClicked()));
	QObject::connect(tbBSDetails, SIGNAL( clicked() ), this, SLOT( slotTbBSDetailsClicked()));
	QObject::connect(tbBSAll, SIGNAL( clicked() ), this, SLOT( slotTbBSAllClicked()));
	
	QObject::connect(tbBSResult, SIGNAL(clicked(int,int,int,const QPoint&)), this, SLOT(slotBasicUserSelected(int)));
	QObject::connect(tbBSResult, SIGNAL(doubleClicked(int,int,int,const QPoint&)), this, SLOT(slotBasicResultTableDBClicked(int)));
	
	QObject::connect(tbASResult, SIGNAL(clicked(int,int,int,const QPoint&)), this,SLOT(slotAdvancedUserSelected(int)));
	QObject::connect(tbASResult, SIGNAL(doubleClicked(int,int,int,const QPoint&)), this, SLOT(slotAdvancedResultTableDBClicked(int)));
	
	QObject::connect(cbASProvince, SIGNAL(activated(int)), this, SLOT(slotCbASProvinceActive(int)));
	
	QObject::connect(tbASNext, SIGNAL( clicked() ), this, SLOT( slotTbASNextClicked()));
	QObject::connect(tbASPrev, SIGNAL( clicked() ), this, SLOT( slotTbASPrevClicked()));
	QObject::connect(tbASAll, SIGNAL( clicked() ), this, SLOT( slotTbASAllClicked()));
	QObject::connect(tbASDetails, SIGNAL( clicked() ), this, SLOT( slotTbASDetailsClicked()));
	
	QObject::connect(cbCategory1,SIGNAL(activated(int)), SLOT(slotTopListChanged(int)));
	QObject::connect(cbCategory2,SIGNAL(activated(int)), SLOT(slotSecondListChanged(int)));
	
	QObject::connect(tbQSResult, SIGNAL(clicked(int,int,int,const QPoint&)), this, SLOT(slotQunSelected(int)));
	QObject::connect(tbQSResult, SIGNAL(doubleClicked(int,int,int,const QPoint&)), this, SLOT(slotQunResultTableDBClicked(int)));
	QObject::connect(tbQSDetails, SIGNAL( clicked() ), this, SLOT( slotTbQSDetailsClicked()));
}

EvaSearchWindow::~EvaSearchWindow()
{
	if(cityData)
		delete cityData;
	if(categoryData)
		delete categoryData;
	
}
void EvaSearchWindow::adjustInterface()
{
	pbLastStep->setHidden(true);
	rbCustomSearch->setChecked(true);
	rbSearchByCategory->setChecked(true);
	chbOnlineUsers->setChecked(true);
	
	tbBSResult->setLeftMargin(0);
	tbBSResult->setColumnWidth(0,100);//width of qq num column
	tbBSResult->setColumnWidth(1,100);//Nick Name
	tbBSResult->setColumnWidth(2,120);//From 
	
	tbASResult->setLeftMargin(0);
	tbASResult->setColumnWidth(0,90);//qq num
	tbASResult->setColumnWidth(1,75);
	tbASResult->setColumnWidth(2,50);
	tbASResult->setColumnWidth(3,30);
	tbASResult->setColumnWidth(4,60);
	tbASResult->setColumnWidth(5,50);
	tbASResult->setColumnWidth(6,40);
	tbQSResult->setLeftMargin(0);
	

	cbASProvince->insertItem(i18n("any"));
	cbASProvince->insertItem(i18n("Abroad"));
	cbASProvince->insertItem( i18n("Beijing") );
	cbASProvince->insertItem( i18n("Shanghai") );
	cbASProvince->insertItem( i18n("Tianjin") );
	cbASProvince->insertItem( i18n("Chongqing") );
	cbASProvince->insertItem( i18n("Hebei") );
	cbASProvince->insertItem( i18n("Shanxi") );
	cbASProvince->insertItem( i18n("Neimenggu") );
	cbASProvince->insertItem( i18n("Liaoning") );
	cbASProvince->insertItem( i18n("Jilin") );
	cbASProvince->insertItem( i18n("Heilongjiang") );
	cbASProvince->insertItem( i18n("Jiangsu") );
	cbASProvince->insertItem( i18n("Zhejiang") );
	cbASProvince->insertItem( i18n("Anhui") );
	cbASProvince->insertItem( i18n("Fujian") );
	cbASProvince->insertItem( i18n("Jiangxi") );
	cbASProvince->insertItem( i18n("Shandong" ) );
	cbASProvince->insertItem( i18n("Henan") );
	cbASProvince->insertItem( i18n("Hubei") );
	cbASProvince->insertItem( i18n("Hunan") );
	cbASProvince->insertItem( i18n("Guangdong") );
	cbASProvince->insertItem( i18n("Guangxi") );
	cbASProvince->insertItem( i18n("Hainan") );
	cbASProvince->insertItem( i18n("Sichuan") );
	cbASProvince->insertItem( i18n("Guizhou") );
	cbASProvince->insertItem( i18n("Yunnan") );
	cbASProvince->insertItem( i18n("Xizang") );
	cbASProvince->insertItem( i18n("Shaanxi") );
	cbASProvince->insertItem( i18n("Gansu") );
	cbASProvince->insertItem( i18n("Ningxia") );
	cbASProvince->insertItem( i18n("Qinghai") );
	cbASProvince->insertItem( i18n("Xinjiang") );
	cbASProvince->insertItem( i18n("Hong Kong" ) );
	cbASProvince->insertItem( i18n("Macao") );
	cbASProvince->insertItem( i18n("Taiwan") );
	
	cbASCity->insertItem(i18n("any"));
	
}

void EvaSearchWindow::loadCityList(unsigned int province)
{
	if(cityData != NULL){
		std::list< CityListElement > city = cityData->getCityList(province);
		cbASCity->clear();
		std::list< CityListElement >::iterator iter;
		cbASCity->insertItem(i18n("any"));
		for(iter=city.begin(); iter!=city.end(); ++iter){
			cbASCity->insertItem(iter->name);
		}
	}
}

void EvaSearchWindow::loadCategoryList()
{
	categoryData = new QunCategory();
	if(!categoryData->isLoaded()){
		delete categoryData;
		categoryData = NULL;
	}
	std::list< QunCategoryElement > top = categoryData->getSubList(0);
	cbCategory1->clear();
	std::list< QunCategoryElement >::iterator iter;
	int index = 0;
	cbCategory1->insertItem("");
	for(iter=top.begin(); iter!=top.end(); ++iter){
		cbCategory1->insertItem(iter->name);
		topIndexCodeMap[++index] = iter->code;
	}
	cbCategory2->setEnabled(false);
	cbCategory3->setEnabled(false);
	
}


void EvaSearchWindow::slotRbSearchOnlineClicked()
{
	wsBasicCondtion->raiseWidget(0);
	m_BSearchType = B_ONLINE;
	
	lblQQNum->setEnabled(false);
	lblNickName->setEnabled(false);
	//lblEmail->setEnabled(false);
	
	leQQNum->setEnabled(false);
	leNickName->setEnabled(false);
	//leEmail->setEnabled(false);
}

void EvaSearchWindow::slotRbCustomSearchClicked()
{
	wsBasicCondtion->raiseWidget(0);
	m_BSearchType = B_CUSTOM;
	
	lblQQNum->setEnabled(true);
	lblNickName->setEnabled(true);
	//lblEmail->setEnabled(true);
	
	leQQNum->setEnabled(true);
	leNickName->setEnabled(true);
	//leEmail->setEnabled(true);
}

void EvaSearchWindow::slotRbSearchFrdCenterClicked()
{
	wsBasicCondtion->raiseWidget(1);
	m_BSearchType = B_FRDCENTER;
}

void EvaSearchWindow::slotRbSearchByCategoryClicked()
{
	wsQunCondition->raiseWidget(0);
	m_QSearchType = Q_CATEGORY;
}


void EvaSearchWindow::slotRbSearchAlumniClicked()
{
	wsQunCondition->raiseWidget(1);
	m_QSearchType = Q_ALUMNI;
}

void EvaSearchWindow::slotRbAccuratelySearchClicked()
{
	wsQunCondition->raiseWidget(2);
	m_QSearchType = Q_ACCURATE;
}

void EvaSearchWindow::slotPbSearchClicked()
{
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	//Index of TabWidget: 0-basic search, 1-advanced search, 2-qun search
	switch(twSearchMain->currentPageIndex()){
	case 0:
		if(!strcmp(wsBasicSearch->visibleWidget()->name(),"wsBSPage")){ 
			//we are now on the search parma page: click the button means search user.
			switch(m_BSearchType){
			case B_ONLINE:{	
				processBasicSearch(true,"", "", "");
				}
				break;
			case B_CUSTOM:{
				m_Id = leQQNum->text().stripWhiteSpace();
				m_Nick = leNickName->text().stripWhiteSpace();
				//m_Email = leEmail->text().stripWhiteSpace();
				processBasicSearch(false, m_Id, m_Nick, m_Email);
				}
				break;
			case B_FRDCENTER:{
				QStringList args;
				args<<"exec"<< "http://love.qq.com/";
				kapp->kdeinitExec("kfmclient",args);
				}
				break;
			}
		}else{
			//we are now on the search result page, click the button means send a auth request message
			//to selected user. the auth dialog should called here
			if(m_SelectedBasicUser.getQQ() == 0){
				KMessageBox::messageBox((QWidget *) 0,KMessageBox::Information,
				i18n("Please select a user."),
				i18n("Eva - Search/Add"));
				return;
			}

			EvaMain::g_AddingManager->slotAddBuddy(m_SelectedBasicUser.getQQ(),
								codec->toUnicode(m_SelectedBasicUser.getNick().c_str()),
								m_SelectedBasicUser.getFace());
			close();
		}
		break;
		
	case 1:
		if(!strcmp(wsAdvancedSearch->visibleWidget()->name(),"wsASPage")){
			m_OnlineUser = chbOnlineUsers->isChecked();
			m_HasCam = chbHaveCamera->isChecked();
			m_ProvinceIndex = cbASProvince->currentItem();
			m_CityIndex = cbASCity->currentItem();
			m_AgeIndex = cbASAge->currentItem();
			m_GenderIndex = cbASSex->currentItem(); 
			processAdvancedSearch(m_OnlineUser, m_HasCam, m_ProvinceIndex, m_CityIndex, m_AgeIndex, m_GenderIndex);
		}else{
			//we are now on the advanced search result page, click the button means send a auth request//message to selected user. the auth dialog should called here
			if(m_SelectedAdvancedUser.getQQ() == 0){
				KMessageBox::messageBox((QWidget *) 0,KMessageBox::Information,
				i18n("Please select a user."),
				i18n("Eva - Search/Add"));
				return;
			}

			EvaMain::g_AddingManager->slotAddBuddy(m_SelectedAdvancedUser.getQQ(),
								codec->toUnicode(m_SelectedAdvancedUser.getNick().c_str()),
								m_SelectedAdvancedUser.getFace());
			close();
		}
		break;
	case 2:
		if(!strcmp(wsQunSearch->visibleWidget()->name(),"wsQSPage")){
			//the same to above
			switch(m_QSearchType){
				case Q_ALUMNI:{
					QStringList args;
					args<<"exec"<< "http://school.qq.com/";
					kapp->kdeinitExec("kfmclient",args);
					}
					break;
				case Q_CATEGORY:{
					QStringList args;
					args << "exec" << "http://jump.qq.com/clienturl_simp_18";
					kapp->kdeinitExec("kfmclient",args);
					//QString url = "http://group.qq.com/cgi-bin/group_classify?catalog="+QString::number(getCategoryCode()); 
					//KRun::runCommand("konqueror \""+url+"\"");
					}
					break;
				case Q_ACCURATE:{
					m_QunNum = leQunNum->text().stripWhiteSpace();
					processQunSearch(m_QunNum);
					}
					break;
			}
		}else{
			//the same to above
			//we are now on the search result page, click the button means send a auth request message
			//to selected user. the auth dialog should called here
			if(m_SelectedQun.getQunID() == 0){
				KMessageBox::messageBox((QWidget *) 0,KMessageBox::Information,
				i18n("Please select a Qun."),
				i18n("Eva - Search/Add"));
				return;
			}
			//emit requestJoinQun(m_SelectedQun);
			EvaMain::g_AddingManager->slotAddQun(m_SelectedQun);
			close();
		}
		break;
	}
	
}

void EvaSearchWindow::processBasicSearch(const bool isSearchAll, const QString id, const QString nick, const QString email)
{
	tbBSPrev->setEnabled(false);
	tbBSNext->setEnabled(false);
	tbBSDetails->setEnabled(false);
	tbBSAll->setEnabled(false);
	lblBSResultTip->setText(i18n("Eva is searching users for you..."));
	
	m_BCurrentPage = 0;
	m_ShowAllBasicUsers = false;
	m_BasicUserPages.clear();//clear because click here aways means re-search
	m_SelectedBasicUser.setQQ(0);
	tbBSResult->setNumRows(0);
	
	//clear the result
	for(int row=0; row<tbBSResult->numRows(); row++){
		for(int col=0; col<tbBSResult->numCols(); col++){
			tbBSResult->clearCell(row, col);
		}
	}
	
	if(isSearchAll){
		pbSearch->setText(i18n("Add Buddy"));
		pbLastStep->setHidden(false);
		wsBasicSearch->raiseWidget(1);
		emit requestSearchUsers(true, "0", "", "", "", false);//request the first page of online users 
	}
	else{
		if(id.isEmpty() && nick.isEmpty() && email.isEmpty()){
			KMessageBox::messageBox((QWidget *) 0,KMessageBox::Information,
				i18n("Search conditions can not be empty."),
				i18n("Eva - Search/Add"));
			leQQNum->setFocus();
			return;
		}
		
		bool ok;
		m_Id.toInt(&ok);
		if(!ok && !m_Id.isEmpty()){
			KMessageBox::messageBox((QWidget *) 0,KMessageBox::Information,
				i18n("QQ number is incorrect."),
				i18n("Eva - Search/Add"));
			leQQNum->setFocus();
			return;
		}
		
		pbSearch->setText(i18n("Add Buddy"));
		pbLastStep->setHidden(false);
		wsBasicSearch->raiseWidget(1);
		emit requestSearchUsers(false, "0", id, nick, email, true);//request the first page of  precise online users
	}
	
}

void EvaSearchWindow::slotPbLastStepClicked()
{
	//there's no need to find which page we are, because the"last step" button is only on the search result page
	switch(twSearchMain->currentPageIndex()){
	case 0://basic search
		switch(m_BSearchType){
		case B_ONLINE:{	
			pbSearch->setText(i18n("Search"));
			pbLastStep->setHidden(true);
			wsBasicSearch->raiseWidget(0);
			}
			break;
		case B_CUSTOM:{
			pbSearch->setText(i18n("Search"));
			pbLastStep->setHidden(true);
			wsBasicSearch->raiseWidget(0);
			}
			break;
		case B_FRDCENTER:{
			}
			break;
		}
		break;
		
	case 1://advance search
		pbSearch->setText(i18n("Search"));
		pbLastStep->setHidden(true);
		wsAdvancedSearch->raiseWidget(0);
		break;
	case 2://qun search
		switch(m_QSearchType){
		case Q_ALUMNI:{
			}
			break;
		case Q_CATEGORY:{
			}
			break;
		case Q_ACCURATE:{
			pbSearch->setText(i18n("Search"));
			pbLastStep->setHidden(true);
			wsQunSearch->raiseWidget(0);
			}
			break;
		}
		break;
	}
}

void EvaSearchWindow::slotCurrentChanged(QWidget* wCurrentPage)
{
	if(!strcmp(wCurrentPage->name(), "tabBasicSearch")){
		
		if(!strcmp(wsBasicSearch->visibleWidget()->name(),"wsBSPage")){
			pbSearch->setText(i18n("Search"));
			pbLastStep->setHidden(true);	
		}
		else{
			pbSearch->setText(i18n("Add Buddy"));
			pbLastStep->setHidden(false);	
		}
	
		
	}
	if(!strcmp(wCurrentPage->name(), "tabAdvancedSearch")){
		if(!strcmp(wsAdvancedSearch->visibleWidget()->name(),"wsASPage")){
			pbSearch->setText(i18n("Search"));
			pbLastStep->setHidden(true);
		}
		else{
			pbSearch->setText(i18n("Add Buddy"));
			pbLastStep->setHidden(false);	
		}
	}
	if(!strcmp(wCurrentPage->name(), "tabQunSearch")){
		if(!strcmp(wsQunSearch->visibleWidget()->name(),"wsQSPage")){
			pbSearch->setText(i18n("Search"));
			pbLastStep->setHidden(true);
		}
		else{
			pbSearch->setText(i18n("Join Qun"));
			pbLastStep->setHidden(false);	
		}
	}
}

void EvaSearchWindow::slotSearchUsersReady(const bool finished, const std::list<OnlineUser> list)
{
	lblBSResultTip->setText(i18n("the following users are found for you by Eva."));
	
	std::map<int, std::list<OnlineUser> >::iterator iter = m_BasicUserPages.find(m_BCurrentPage);
	if( iter != m_BasicUserPages.end()) return ;
	
	m_BasicUserPages[m_BCurrentPage] = list;
	m_BSearchFinished = finished;
		
	displayBasicUsers();
}

void EvaSearchWindow::displayBasicUsers()
{
	//clear the reuslt table of basic search
	for(int row=0; row<tbBSResult->numRows(); row++){
		for(int col=0; col<tbBSResult->numCols(); col++){
			tbBSResult->clearCell(row, col);
		}
	}
	tbBSResult->setNumRows(0);
	
	int start=0, end=1;
	
	if(m_ShowAllBasicUsers){
		end = m_BasicUserPages.size();
	}else{
		start = m_BCurrentPage;
		end = start+1;
	}
	std::list<OnlineUser>::iterator iter;
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	for(int i=start; i<end; i++){
		for(iter = m_BasicUserPages[i].begin(); iter!=m_BasicUserPages[i].end(); ++iter){
			tbBSResult->setNumRows(tbBSResult->numRows()+1);
			QImage img = images->getFace(images->getFaceFileIndex(iter->getFace()),true)->convertToImage();
			tbBSResult->setPixmap(tbBSResult->numRows()-1, 0, QPixmap(img.smoothScale(16,16)));
			tbBSResult->setText(tbBSResult->numRows()-1, 0, QString::number(iter->getQQ()));
			tbBSResult->setText(tbBSResult->numRows()-1, 1,codec->toUnicode(iter->getNick().c_str()));
			tbBSResult->setText(tbBSResult->numRows()-1, 2,codec->toUnicode(iter->getProvince().c_str()));
		}
	}
	
	tbBSPrev->setEnabled(m_BCurrentPage==0?false:true);
	tbBSNext->setEnabled(m_BSearchFinished?false:true);
	tbBSDetails->setEnabled(true);
	tbBSAll->setEnabled(true);
	
	lblBSResultPage->setText(QString(i18n("Current page %1 ")).arg(m_BCurrentPage+1));
	
}

void EvaSearchWindow::slotTbBSNextClicked()
{
	m_BCurrentPage++;
	m_ShowAllBasicUsers = false;
	if((m_BCurrentPage+1)>m_BasicUserPages.size()){
		
		tbBSNext->setEnabled(false);
		tbBSPrev->setEnabled(false);
		tbBSDetails->setEnabled(false);
		tbBSAll->setEnabled(false);
		
		lblBSResultTip->setText(i18n("Eva is searching users for you..."));
	
		QString pageNo = QString::number(m_BCurrentPage);
		if(m_BSearchType == B_ONLINE){
			emit requestSearchUsers(true, pageNo, "", "", "", false);
		}else{
			//send precise search request
			emit requestSearchUsers(false, pageNo, m_Id, m_Nick, m_Email, false);
		}
	}else{
		displayBasicUsers();
	}
}

void EvaSearchWindow::slotTbBSPrevClicked()
{
	m_BCurrentPage--;
	m_ShowAllBasicUsers = false;
	displayBasicUsers();
}

void EvaSearchWindow::slotTbBSAllClicked()
{
	m_ShowAllBasicUsers = true;
	displayBasicUsers();
}

void EvaSearchWindow::slotBasicUserSelected(int row)
{
	int start = 0, end = 1 ,count = 0;
	if(m_ShowAllBasicUsers){
		end = m_BasicUserPages.size();
	}else{
		start = m_BCurrentPage;
		end = start+1;
	}
	std::list<OnlineUser>::iterator iter;
	for(int i=start; i<end; i++){
		for(iter = m_BasicUserPages[i].begin(); iter!=m_BasicUserPages[i].end(); ++iter){
			if(count == row){
				m_SelectedBasicUser = *iter;
				return;
			}else
				count++;
		}
	}
	
}

void EvaSearchWindow::slotTbBSDetailsClicked()
{
	//if user click the "details" button without any selected item, then show the message box
	if(tbBSResult->numRows()!=0 && m_SelectedBasicUser.getQQ() == 0){
		KMessageBox::messageBox((QWidget *) 0,KMessageBox::Information,
				i18n("Please select a user."),
				i18n("Eva - Search/Add"));
		return;
	}
	emit requestUserInfo(m_SelectedBasicUser.getQQ());
	
}

void EvaSearchWindow::slotBasicResultTableDBClicked(int row)
{
	slotBasicUserSelected(row);
	slotTbBSDetailsClicked();
}


void EvaSearchWindow::processAdvancedSearch(const bool online, const bool cam, const int province, const int city, const int age, const int gender)
{
	
	pbSearch->setText(i18n("Add Buddy"));
	pbLastStep->setHidden(false);
	wsAdvancedSearch->raiseWidget(1);
	tbASPrev->setEnabled(false);
	tbASNext->setEnabled(false);
	tbASDetails->setEnabled(false);
	tbASAll->setEnabled(false);
	lblASResultTip->setText(i18n("Eva is searching users for you..."));
	
	m_ACurrentPage = 0;
	m_ShowAllAdvancedUsers = false;
	m_AdvancedUserPages.clear();//empty it, because there is always re-search
	m_SelectedAdvancedUser.setQQ(0);
	tbASResult->setNumRows(0);
	
	//clear the result table
	for(int row=0; row<tbASResult->numRows(); row++){
		for(int col=0; col<tbASResult->numCols(); col++){
			tbASResult->clearCell(row, col);
		}
	}
	
	//printf("AdvancedSearch sig send: online:%d, cam:%d\n", online?1:0, cam?1:0);
	emit requestAdvancedSearch(0, online, cam, province, city, age, gender);//always send page 1 request, so the page num is 0
}

void EvaSearchWindow::slotAdvancedSearchReady(const int page, const bool finished, const std::list<AdvancedUser> list)
{
	lblASResultTip->setText(i18n("the following users are found for you by Eva."));
	
	std::map<int, std::list<AdvancedUser> >::iterator iter = m_AdvancedUserPages.find(m_ACurrentPage);
	
	if( iter != m_AdvancedUserPages.end()) return ;
	m_ASearchFinished = finished;
	if(!m_ASearchFinished){
		m_ACurrentPage = page;
		m_AdvancedUserPages[m_ACurrentPage] = list;
	}
	else
		m_ACurrentPage--;
	
	displayAdvancedUsers();
}

void EvaSearchWindow::displayAdvancedUsers()
{
	
	//clear the advanced search result table
	for(int row=0; row<tbASResult->numRows(); row++){
		for(int col=0; col<tbBSResult->numCols(); col++){
			tbASResult->clearCell(row, col);
		}
	}
	tbASResult->setNumRows(0);
	
	int start=0, end=1;
	
	if(m_ShowAllAdvancedUsers){
		end = m_AdvancedUserPages.size();
	}else{
		start = m_ACurrentPage;
		end = start+1;
	}
	std::list<AdvancedUser>::iterator iter;
	
	std::list< CityListElement> city;
	std::list< CityListElement >::iterator cityIter;
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	for(int i=start; i<end; i++){
		for(iter = m_AdvancedUserPages[i].begin(); iter!=m_AdvancedUserPages[i].end(); ++iter){
			tbASResult->setNumRows(tbASResult->numRows()+1);
			QImage img = images->getFace(images->getFaceFileIndex(iter->getFace()), iter->isOnline())->convertToImage();
			tbASResult->setPixmap(tbASResult->numRows()-1, 0, QPixmap(img.smoothScale(16,16)));
			tbASResult->setText(tbASResult->numRows()-1, 0, QString::number(iter->getQQ()));
			tbASResult->setText(tbASResult->numRows()-1, 1,codec->toUnicode(iter->getNick().c_str()));
			tbASResult->setText(tbASResult->numRows()-1, 2,cbASSex->text(iter->getGenderIndex()));
			tbASResult->setText(tbASResult->numRows()-1, 3,QString::number(iter->getAge()));
			tbASResult->setText(tbASResult->numRows()-1, 4,cbASProvince->text(iter->getProvinceIndex()));
			//if use selected unlimited item, then the city name can not selected in the combox, because
			//the combox has no items yet, so I decided that all the city name is search in cityData
			if(cityData != NULL){
				city = cityData->getCityList(iter->getProvinceIndex());
				for(cityIter=city.begin(); cityIter!=city.end(); cityIter++){
					if(iter->getCityIndex() == cityIter->city){
						tbASResult->setText(tbASResult->numRows()-1,5,cityIter->name);
						break;
					}	
				}
			}
			tbASResult->setText(tbASResult->numRows()-1, 6,iter->isOnline()?i18n("on"):i18n("off"));
		}
	}
	tbASPrev->setEnabled(m_ACurrentPage==1?false:true);
	tbASNext->setEnabled(m_ACurrentPage==10?false:true);//advanced search can only search 10 pages
	tbASDetails->setEnabled(true);
	tbASAll->setEnabled(true);
	lblASResultPage->setText(QString(i18n("Current page %1 ")).arg(m_ACurrentPage));
	
}

void EvaSearchWindow::slotAdvancedUserSelected(int row)
{
	int start = 0, end = 1 ,count = 0;
	if(m_ShowAllAdvancedUsers){
		end = m_AdvancedUserPages.size();
	}else{
		start = m_ACurrentPage;
		end = start+1;
	}
	std::list<AdvancedUser>::iterator iter;
	for(int i=start; i<end; i++){
		for(iter = m_AdvancedUserPages[i].begin(); iter!=m_AdvancedUserPages[i].end(); ++iter){
			if(count == row){
				m_SelectedAdvancedUser = *iter;
				return;
			}else
				count++;
		}
	}
	
}

void EvaSearchWindow::slotTbASDetailsClicked()
{
	//if user click the "details" button without any selected item, then show the message box
	if(tbASResult->numRows()!=0 && m_SelectedAdvancedUser.getQQ() == 0){
		KMessageBox::messageBox((QWidget *) 0,KMessageBox::Information,
				i18n("Please select a user."),
				i18n("Eva - Search/Add"));
		return;
	}
	emit requestUserInfo(m_SelectedAdvancedUser.getQQ());
	
}

void EvaSearchWindow::slotAdvancedResultTableDBClicked(int row)
{
	slotAdvancedUserSelected(row);
	slotTbASDetailsClicked();
}

void EvaSearchWindow::slotCbASProvinceActive(int index)
{
	loadCityList(index);
	cbASCity->setCurrentItem(0);
}

void EvaSearchWindow::slotTbASNextClicked()
{
	m_ACurrentPage++;
	m_ShowAllAdvancedUsers = false;
	int pageNo = m_ACurrentPage-1;//the request page num of advanced search is start by 0, but the response page num is start by 1
	if(m_ACurrentPage>m_AdvancedUserPages.size()){
		
		tbASNext->setEnabled(false);
		tbASPrev->setEnabled(false);
		tbASDetails->setEnabled(false);
		tbASAll->setEnabled(false);
		
		lblASResultTip->setText(i18n("Eva is searching users for you..."));
	
		emit requestAdvancedSearch(pageNo, m_OnlineUser, m_HasCam, m_ProvinceIndex, m_CityIndex, m_AgeIndex, m_GenderIndex);
		
	}else{
		displayAdvancedUsers();
	}

}

void EvaSearchWindow::slotTbASPrevClicked()
{
	m_ACurrentPage--;
	m_ShowAllAdvancedUsers = false;
	displayAdvancedUsers();
}

void EvaSearchWindow::slotTbASAllClicked()
{
	m_ShowAllAdvancedUsers = true;
	displayAdvancedUsers();
}

void EvaSearchWindow::processQunSearch(const QString &qunNum)
{
	printf("qun search clicked\n");
	if(qunNum.isEmpty()){
		KMessageBox::messageBox((QWidget *) 0,KMessageBox::Information,
                              i18n("Qun Num can not be empty."),
                              i18n("Eva - Search/Add"));
		leQunNum->setFocus();
		return;
	}
	
	bool ok;
	int extQunNum = qunNum.toInt(&ok);
	if(!ok){
		KMessageBox::messageBox((QWidget *) 0,KMessageBox::Information,
                              i18n("Qun Num is incorrect."),
                              i18n("Eva - Search/Add"));
		leQunNum->setFocus();
		return;
	}
	
	m_SelectedQun.setQunID(0);
	
	pbSearch->setText(i18n("Join Qun"));
	pbLastStep->setHidden(false);
	wsQunSearch->raiseWidget(1);
	tbQSPrev->setEnabled(false);
	tbQSNext->setEnabled(false);
	tbQSDetails->setEnabled(false);
	tbQSAll->setEnabled(false);
			
	lblQSResultTip->setText(i18n("Eva is searching Qun for you..."));
	emit requestQunSearch(extQunNum);
}

void EvaSearchWindow::slotQunSearchReady(const std::list<QunInfo> list, QString /*error*/)
{
	//clear qun search result table
	for(int row=0; row<tbQSResult->numRows(); row++){
		for(int col=0; col<tbQSResult->numCols(); col++){
			tbQSResult->clearCell(row, col);
		}
	}
	tbQSResult->setNumRows(0);
	
	std::list<QunInfo>::iterator iter;
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	
	qunList = list;
	if(list.size()){
		for(iter = qunList.begin(); iter!=qunList.end(); ++iter){
			tbQSResult->setNumRows(tbQSResult->numRows()+1);
			QImage img = images->getIcon("QUN")->convertToImage();
			tbQSResult->setPixmap(tbQSResult->numRows()-1, 0, QPixmap(img.smoothScale(16,16)));
			tbQSResult->setText(tbQSResult->numRows()-1, 0, QString::number(iter->getExtID()));
			tbQSResult->setText(tbQSResult->numRows()-1, 1,codec->toUnicode(iter->getName().c_str()));
			tbQSResult->setText(tbQSResult->numRows()-1, 2,QString::number(iter->getCreator()));
		}
	}
		
	tbQSDetails->setEnabled(true);
	
	lblQSResultPage->setText(QString(i18n("Current page 1 ")));
	lblQSResultTip->setText(i18n("the following Qun are found for you by Eva."));
}

void EvaSearchWindow::slotTopListChanged(int index)
{
	std::list< QunCategoryElement > second = categoryData->getSubList(topIndexCodeMap[index]);
	cbCategory2->clear();
	if(!second.size()){
		cbCategory2->setEnabled(false);
		return;
	}else
		cbCategory2->setEnabled(true);
	std::list< QunCategoryElement >::iterator iter;
	int sIndex = 0;
	cbCategory2->insertItem("");
	secondIndexCodeMap.clear();
	for(iter=second.begin(); iter!=second.end(); ++iter){
		cbCategory2->insertItem(iter->name);
		secondIndexCodeMap[++sIndex] = iter->code;
	}
}

void EvaSearchWindow::slotSecondListChanged(int index)
{
	std::list< QunCategoryElement > third = categoryData->getSubList(secondIndexCodeMap[index]);
	cbCategory3->clear();
	if(!third.size()){
		cbCategory3->setEnabled(false);
		return;
	}else
		cbCategory3->setEnabled(true);
	std::list< QunCategoryElement >::iterator iter;
	int tIndex = 0;
	cbCategory3->insertItem("");
	thirdIndexCodeMap.clear();
	for(iter=third.begin(); iter!=third.end(); ++iter){
		cbCategory3->insertItem(iter->name);
		thirdIndexCodeMap[++tIndex] = iter->code;
	}	
}

const unsigned short EvaSearchWindow::getCategoryCode()
{
	unsigned short code = 12; // 12 is "tech union", we set it as default
	
	if(cbCategory3->isEnabled() && cbCategory3->currentItem())
		code = thirdIndexCodeMap[cbCategory3->currentItem()];
	else if(cbCategory2->isEnabled() && cbCategory2->currentItem())
		code = secondIndexCodeMap[cbCategory2->currentItem()];
	else if(cbCategory1->currentItem())
		code = topIndexCodeMap[cbCategory1->currentItem()];
	return code;
}

void EvaSearchWindow::slotQunSelected(int row)
{
	int count = 0;
	std::list<QunInfo>::iterator iter;
	for(iter = qunList.begin(); iter!=qunList.end(); ++iter){
		if(count == row){
			m_SelectedQun = *iter;
			return;
		}else
			count++;
	}
	
}

void EvaSearchWindow::slotQunResultTableDBClicked(int row)
{
	slotQunSelected(row);
	slotTbQSDetailsClicked();
}

void EvaSearchWindow::slotTbQSDetailsClicked()
{
	//if user click the "details" button without any selected item, then show the message box
	if(tbBSResult->numRows()!=0 && m_SelectedQun.getQunID() == 0){
		KMessageBox::messageBox((QWidget *) 0,KMessageBox::Information,
				i18n("Please select a Qun."),
				i18n("Eva - Search/Add"));
		return;
	}
	Qun *qun = new Qun(m_SelectedQun.getQunID());
	qun->setDetails( m_SelectedQun );
	QunDetailsWindow *win = new QunDetailsWindow(qun, false);
	delete qun;

	QRect scr = KApplication::desktop()->screenGeometry();
	win->move(scr.center() - win->rect().center());
	win->show();
}
