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
 
#ifndef QUNCATEGORYPICKER_H
#define QUNCATEGORYPICKER_H

#include "quncategoryui.h"
#include <map>
#include <list>
typedef struct QunCategoryElement{
unsigned short code;
unsigned short parent;
QString name;
} QunCategoryElement;


class QunCategory
{
public:
	QunCategory();	
	const bool isLoaded() { return isOK; }
	std::list<QunCategoryElement> getSubList(unsigned short code);
	const QString getDescription(const unsigned short code);
private:
	const bool loadList();
	bool isOK;
	std::map<unsigned short, QunCategoryElement> list;
};

class QunCategoryPicker : public QunCategoryUI
{
	Q_OBJECT
public:
	QunCategoryPicker(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~QunCategoryPicker();
	const unsigned short getCategoryCode();
signals:
	void selectCategoryCode(const unsigned short);
private:
	void loadCategoryList();
	
	//unsigned short mCategory;
	QunCategory *categoryData;
	std::map<int, unsigned short> topIndexCodeMap;
	std::map<int, unsigned short> secondIndexCodeMap;
	std::map<int, unsigned short> thirdIndexCodeMap;
private slots:
	void slotTopListChanged(int index);
	void slotSecondListChanged(int index);
	void slotThirdListChanged(int index);
	
	void slotCancelClicked();
	void slotOKClicked();
};

#endif

