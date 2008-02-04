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
#ifndef EVASERVERS_H
#define EVASERVERS_H

#include <qobject.h>
#include <qvaluelist.h>
#include <qhostaddress.h>



class QTimer;

/**
This class loads all Tencent's tcp and udp servers, and return a random server IP address
This class also involves some DNS operations.

@author yunfan
*/

class EvaServers : public QObject
{
	Q_OBJECT
public:
	EvaServers(QString &dataRoot);
	
	virtual ~EvaServers();
	/** used to resolving the IP address for Tencent server url */
	void fetchAddress(bool isUdp);
	/** when the server has been verified by command 0x0091, main routine should
	*    call this function to stop the processing for the rest URLs in the list
	*/
	void stopDns();
	void resetDefaultIP() { m_bIsFirst = true; }
signals:
	/** emitted when IP is resovled or the default IP if timeout occurred */
	void isReady(QHostAddress ip);
private:
	typedef struct serverItem{
		int type;   
		QString addr;
	} serverItem;
	
	enum { UDP, TCP, Addr_URL, Addr_IP};
	
	QString filename;
	bool gotIP;
	bool isLoaded;
	int fetchType;
	bool m_StopDns;
	QTimer *m_Timeout;
	int m_CurrAddrIndex;
	QValueList<serverItem> TCPServers;
	QValueList<serverItem> UDPServers;
	
	bool m_bIsFirst;
	
	/** load the server list file */
	bool loadServers();
	/** emit the default server IP for the specific type, UDP or TCP*/
	void defaultAddress();
private slots:
	void getResultsSlot();
	void slotTimeout();
};

#endif
