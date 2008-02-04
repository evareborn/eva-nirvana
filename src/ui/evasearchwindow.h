/***************************************************************************
 *   Copyright (C) 2005 by yunfan,casper                                   *
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

#ifndef EVASEARCHWINDOW_H
#define EVASEARCHWINDOW_H

#include "quncategorypicker.h"
#include "evasearchuibase.h"
#include "evasearchuser.h"
#include "evaadvancedsearch.h"
#include "evaqun.h"
#include <list>
#include <map>
#include <string>
#include <qvaluestack.h>

class EvaImageResource;

typedef struct CityListElement{
	unsigned short city;
	QString name;
} CityListElement;

class CityList
{
public:
	CityList();	
	const bool isLoaded() { return isOK; }
	std::list<CityListElement> getCityList(unsigned short province);
private:
	const bool loadList();
	bool isOK;
	std::map<unsigned short, std::list< CityListElement > > list;
};


class EvaSearchWindow : public EvaSearchUIBase
{
	Q_OBJECT

public:
	EvaSearchWindow(EvaImageResource *res = NULL, const int onlineUsers = 0, QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
	~EvaSearchWindow();

public slots:
	void slotSearchUsersReady(const bool, const std::list<OnlineUser>);
	void slotAdvancedSearchReady(const int, const bool, const std::list<AdvancedUser>);
	void slotQunSearchReady(const std::list<QunInfo>, QString);
	const short unsigned int getCategoryCode();
signals:
	//void requestBuddyAuthStatus(const int, const short, const QString&);
	//void requestAddBuddy(const unsigned int, const QString, const unsigned short); // id, nick, faceid, to be removed
	void requestSearchUsers(const bool, const QString, const QString, const QString, const QString, const bool);
	void requestUserInfo(const unsigned int);
	void requestAdvancedSearch(const int, const bool, const bool, const int, const int, const int, const int); 
	void requestQunSearch(const unsigned int);
private:	
	EvaImageResource *images;
	
	CityList *cityData;
	QunCategory *categoryData;
	std::list<QunInfo> qunList;
	
	std::map<int, unsigned short> topIndexCodeMap;
	std::map<int, unsigned short> secondIndexCodeMap;
	std::map<int, unsigned short> thirdIndexCodeMap;
		
	enum BSearchType{ B_ONLINE, B_CUSTOM, B_FRDCENTER };
	enum QSearchType{ Q_ALUMNI, Q_CATEGORY, Q_ACCURATE };//B = Basic, A = Advanced, Q = Qun
	
	BSearchType m_BSearchType;	//basic search type
	std::map<int, std::list<OnlineUser> > m_BasicUserPages;  // <page num:base search user list>
	unsigned int m_BCurrentPage;	//current page of basic search
	bool m_BSearchFinished;		//if the basic search finished
	bool  m_ShowAllBasicUsers;	//if show all the users we found by basic search
	OnlineUser m_SelectedBasicUser;	//seclected user in basic search result list
	QString m_Id, m_Nick, m_Email;	//basic search parma: QQ num, nick name and email
	std::map<int, std::list<AdvancedUser> > m_AdvancedUserPages;// <page num:advanced search user iist>
	unsigned int m_ACurrentPage;	//current page of advanced search
	bool m_ASearchFinished;		//if the advanced search finished
	bool m_ShowAllAdvancedUsers;	//if show all the users we found by advanced search 
	AdvancedUser m_SelectedAdvancedUser;//selected user in advanced search result list
	bool m_OnlineUser;		//advanced search parma
	bool m_HasCam;
	int m_ProvinceIndex;
	int m_CityIndex;
	int m_AgeIndex;
	int m_GenderIndex;
	
	QSearchType m_QSearchType;	//qun search type 
	unsigned int m_QCurrentPage;	//current page of qun search
	QString m_QunNum;		//qun num
	QunInfo m_SelectedQun;


	void adjustInterface();
	void displayBasicUsers();
	void displayAdvancedUsers();
	
	void processBasicSearch(const bool, const QString, const QString, const QString);
	void processAdvancedSearch(const bool, const bool, const int, const int, const int, const int);
	void processQunSearch(const QString&);

private slots:
	void slotRbSearchOnlineClicked();
	void slotRbCustomSearchClicked();
	void slotRbSearchFrdCenterClicked();
	
	void slotRbSearchAlumniClicked();
	void slotRbSearchByCategoryClicked();
	void slotRbAccuratelySearchClicked();
	
	void slotPbSearchClicked();
	void slotPbLastStepClicked();
	
	void slotTbBSNextClicked();
	void slotTbBSPrevClicked();
	void slotTbBSAllClicked();
	void slotTbBSDetailsClicked();
	
	void slotBasicUserSelected(int);
	void slotBasicResultTableDBClicked(int);
	void slotCurrentChanged(QWidget*);
	
	
	
	void slotAdvancedResultTableDBClicked(int);
	void slotAdvancedUserSelected(int);
	
	void slotCbASProvinceActive(int);
	void slotTbASNextClicked();
	void slotTbASPrevClicked();
	void slotTbASAllClicked();
	void slotTbASDetailsClicked();
	
	void loadCityList(unsigned int);
	void loadCategoryList();
	
	void slotTopListChanged(int);
	void slotSecondListChanged(int);
	
	void slotQunSelected(int);
	void slotQunResultTableDBClicked(int);
	void slotTbQSDetailsClicked();

	
};

#endif
