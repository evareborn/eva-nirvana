/***************************************************************************
 *   Copyright (C) 2004 by yunfan                                          *
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
#include "evaservers.h"
#include <stdlib.h>      // rand() function
#include <qdns.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qdatetime.h>   // seed for rand()
#include <qtimer.h>
#include <qdir.h>

#include <kdebug.h>
#include <kconfig.h>

#include "evamain.h"
#include "evauser.h"
#include "evausersetting.h"

EvaServers::EvaServers(QString &dataRoot):
	gotIP(false),
	isLoaded(false),
	m_StopDns(false),
	m_Timeout(0),
	m_CurrAddrIndex(0),
	m_bIsFirst(true)
{
	filename = dataRoot + "/servers";
	QTime t = QTime::currentTime();
	srand( t.hour()*12+t.minute()*60+t.second()*60 );
	isLoaded = this->loadServers();
	//dns = NULL;
}


EvaServers::~EvaServers()
{
	//delete dns;
	delete m_Timeout;
}

void EvaServers::fetchAddress( bool isUdp )
{
	int num = 0;
	if(m_bIsFirst){
		m_bIsFirst = false;
		KConfig* config = new KConfig( (QDir::homeDirPath() + "/.eva/eva.cfg") );
		config->setGroup("General");
		QString type = config->readEntry("Server Type");
		if(!type.isEmpty()){
			QHostAddress addr(config->readEntry("Server IP"));
			if(!addr.isNull()) {
				if( (type == "UDP" && isUdp) ||
						(type == "TCP" && !isUdp)){
					emit isReady(addr);
					return;
				}
			}
		}
		delete config;
	}
				
	if(isUdp){
		num = UDPServers.count();
		fetchType = UDP;
	} else{
		num = TCPServers.count();
		fetchType = TCP;
	}
	if(num == 0 ){
		defaultAddress();
		return;
	}
	//int index = rand() % num;

	int maxItems = isUdp?UDPServers.count():TCPServers.count();
	if(m_CurrAddrIndex>maxItems) m_CurrAddrIndex = 0;
	serverItem addr;
	if(isUdp)
		addr = UDPServers[m_CurrAddrIndex++];
	else
		addr = TCPServers[m_CurrAddrIndex++];
	
	if(addr.type == Addr_IP){
		emit isReady(QHostAddress(addr.addr.latin1())); // this way, Red hat 9 might work properly
		return;
	}
	
	// the address should be a URL now, so we try to get the IP
	QDns * dns =  new QDns(addr.addr, QDns::A);
	QObject::connect(dns, SIGNAL(resultsReady()), this, SLOT(getResultsSlot()));


	m_Timeout = new QTimer(this, "dns timer");
	QObject::connect(m_Timeout, SIGNAL(timeout()), SLOT(slotTimeout()));
	m_Timeout->start(30000, true);
}

bool EvaServers::loadServers( )
{
	QFile file(filename);    
	if(!file.open(IO_ReadOnly)){
		return false;
	}

	QTextStream stream(&file);
	QString line;
	QStringList lineList;
	int nextType = 0; 
	while(!stream.atEnd()){
		line = stream.readLine().stripWhiteSpace();

		if(line == "UDP"){
			nextType = UDP;
			continue;
		}else if(line == "TCP"){
				nextType = TCP;
				continue;
			} else if(line == "" ){
				continue;
			}

        	lineList = QStringList::split(":", line);

        	if(lineList.size() != 2)
			continue;

		lineList[0].stripWhiteSpace();

		serverItem *item = new serverItem();
		if(lineList[0]=="URL"){
			item->type = Addr_URL;
		}else if(lineList[0]=="IP"){
			item->type = Addr_IP;
		}else
			continue;

		item->addr = lineList[1].stripWhiteSpace();
		
		if(nextType == UDP){
			UDPServers.append(*item);
		}
		
		if(nextType == TCP){
			TCPServers.append(*item);
		}
	}

	file.close();
	return true;
}

void EvaServers::defaultAddress()
{    
    if(fetchType == TCP){
        emit isReady(QHostAddress("218.17.209.23"));
    }else{
        emit isReady(QHostAddress("219.133.60.28")); //218.17.209.20
    }    
}

void EvaServers::getResultsSlot( )
{
	QDns *dns = (QDns *)(QObject::sender());
	if(dns == 0 ){
        	defaultAddress();
        	return;
	}
	QValueList<QHostAddress> list = dns->addresses();
	if(list.count() == 0 ){
		defaultAddress();
		return;
	}
	
	QHostAddress addr = list[0];
	kdDebug() << "[DNS reply] " << dns->label() << " ---> " << addr.toString() << endl;
	emit isReady(addr);
}

void EvaServers::stopDns( )
{
	m_StopDns = true;
}

void EvaServers::slotTimeout( )
{
	if(m_StopDns) return;
	kdDebug() << "[DNS timeout] Tencent Server DNS resovling timeout, use default IP instead.";
	defaultAddress();
}

