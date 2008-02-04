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

#include "quncategorypicker.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qtextcodec.h>
#include <kglobal.h>
#include <kstandarddirs.h>

QunCategory::QunCategory( )
{
	isOK = loadList();
}

std::list< QunCategoryElement > QunCategory::getSubList( unsigned short code )
{
	std::list< QunCategoryElement > results;
	std::map<unsigned short, QunCategoryElement >::iterator iter;
	for(iter=list.begin(); iter!=list.end(); ++iter){
		if(code == iter->second.parent){
			QunCategoryElement element;
			element.code = iter->second.code;
			element.parent = iter->second.parent;
			element.name = iter->second.name;
			results.push_back(element);
		}
	}
	return results;
}

const QString QunCategory::getDescription(const unsigned short code)
{
	if(!code || code>list.size()) return "";
	QString ret;
	ret = list[code].name;
	unsigned short parentCode = list[code].parent;
	if(!parentCode) return ret;
	do{
		ret = list[parentCode].name + "-" + ret;
		parentCode = list[parentCode].parent;
	}while(parentCode);
	return ret;
}

const bool QunCategory::loadList( )
{
	list.clear();
	QString filePath = KGlobal::dirs()->findResource("data", QString::fromLatin1("eva/qun-category"));
	
	QFile file(filePath);    
	if(!file.open(IO_ReadOnly)){
		return false;
	}
	
	QTextStream stream(&file);
	stream.setCodec(QTextCodec::codecForName("GB18030"));
	QString line;
	QStringList lineList;
	
	while(!stream.atEnd()){
		line = stream.readLine().stripWhiteSpace();
			
		lineList = QStringList::split(",", line);
		
		if(lineList.size() != 3)
			continue;
		
		bool ok;
		QunCategoryElement element;
		element.code = (unsigned short)(lineList[0].stripWhiteSpace().toInt(&ok));
		if(!ok) continue;
		
		element.parent = (unsigned short)(lineList[1].stripWhiteSpace().toInt(&ok));
		if(!ok) continue;
		
		element.name = lineList[2].stripWhiteSpace();
		list[element.code]=element;
	}
	file.close();
	return true; 
}

QunCategoryPicker::QunCategoryPicker(QWidget* parent, const char* name, WFlags fl)
	: QunCategoryUI(parent, name, fl), categoryData(NULL)
{
	loadCategoryList();
	QObject::connect(cbbTop,SIGNAL(activated(int)), SLOT(slotTopListChanged(int)));
	QObject::connect(cbbSecond,SIGNAL(activated(int)), SLOT(slotSecondListChanged(int)));
	QObject::connect(cbbThird,SIGNAL(activated(int)), SLOT(slotThirdListChanged(int)));
	QObject::connect(pbCancel,SIGNAL(clicked()), SLOT(slotCancelClicked()));
	QObject::connect(pbOK,SIGNAL(clicked()), SLOT(slotOKClicked()));
}

QunCategoryPicker::~QunCategoryPicker()
{
	if(categoryData)
		delete categoryData;
}

void QunCategoryPicker::loadCategoryList()
{
	categoryData = new QunCategory();
	if(!categoryData->isLoaded()){
		delete categoryData;
		categoryData = NULL;
	}
	std::list< QunCategoryElement > top = categoryData->getSubList(0);
	cbbTop->clear();
	std::list< QunCategoryElement >::iterator iter;
	int index = 0;
	cbbTop->insertItem("");
	for(iter=top.begin(); iter!=top.end(); ++iter){
		cbbTop->insertItem(iter->name);
		topIndexCodeMap[++index] = iter->code;
	}
	cbbSecond->setEnabled(false);
	cbbThird->setEnabled(false);
}

void QunCategoryPicker::slotTopListChanged(int index)
{
	std::list< QunCategoryElement > second = categoryData->getSubList(topIndexCodeMap[index]);
	cbbSecond->clear();
	if(!second.size()){
		cbbSecond->setEnabled(false);
		return;
	}else
		cbbSecond->setEnabled(true);
	std::list< QunCategoryElement >::iterator iter;
	int sIndex = 0;
	cbbSecond->insertItem("");
	secondIndexCodeMap.clear();
	for(iter=second.begin(); iter!=second.end(); ++iter){
		cbbSecond->insertItem(iter->name);
		secondIndexCodeMap[++sIndex] = iter->code;
	}
}

void QunCategoryPicker::slotSecondListChanged(int index)
{
	std::list< QunCategoryElement > third = categoryData->getSubList(secondIndexCodeMap[index]);
	cbbThird->clear();
	if(!third.size()){
		cbbThird->setEnabled(false);
		return;
	}else
		cbbThird->setEnabled(true);
	std::list< QunCategoryElement >::iterator iter;
	int tIndex = 0;
	cbbThird->insertItem("");
	thirdIndexCodeMap.clear();
	for(iter=third.begin(); iter!=third.end(); ++iter){
		cbbThird->insertItem(iter->name);
		thirdIndexCodeMap[++tIndex] = iter->code;
	}	
}

void QunCategoryPicker::slotThirdListChanged(int)
{
// nothing to do
}

void QunCategoryPicker::slotCancelClicked()
{
	close();
}

const unsigned short QunCategoryPicker::getCategoryCode()
{
	unsigned short code = 12; // 12 is "tech union", we set it as default
	
	if(cbbThird->isEnabled() && cbbThird->currentItem())
		code = thirdIndexCodeMap[cbbThird->currentItem()];
	else if(cbbSecond->isEnabled() && cbbSecond->currentItem())
		code = secondIndexCodeMap[cbbSecond->currentItem()];
	else if(cbbTop->currentItem())
		code = topIndexCodeMap[cbbTop->currentItem()];
	return code;
}

void QunCategoryPicker::slotOKClicked()
{
	unsigned short code = getCategoryCode();
	emit selectCategoryCode(code);
	slotCancelClicked();
}
