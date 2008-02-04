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
#ifndef EVACONNECTER_H 
#define EVACONNECTER_H

#include <qobject.h>
#include <qptrlist.h>
#include <qhostaddress.h>
#include <qstring.h>
#include "evapacket.h"
#include "evanetwork.h"

class QTimer;
class InPacket;

class EvaConnecter : public QObject {
	Q_OBJECT
public:
	EvaConnecter(EvaNetwork *network);
	~EvaConnecter();
	
	void append(OutPacket *out);
	InPacket *getInPacket();
	const unsigned int numOfOutPackets() const { return outPool.count(); }
	const unsigned int numOfInPackets() const { return inPool.count(); }
	void redirectTo(const int ip, const short port);
	void connect();
        void stop();
	bool isConnectionReady() const { return connectionReady; }
	
        const EvaNetwork::Type getConnectionType() const { return connecter->connectionType(); }
	const QHostAddress getSocketIp();
	const unsigned int getSocketPort();
	const QHostAddress &getHostAddress() const { return connecter->getHostAddress();}  // if it's Http Proxy, return the proxy's address
	const short getHostPort() const { return connecter->getHostPort(); }
public slots:
	void slotClientReady();

signals:
	void isReady();
	void networkException(int);
	void packetException(int);
	void sendMessage(unsigned int, bool);
	void sendQunMessage(unsigned int, bool, QString);
	void newPacket();
	void clientNotReady();
private:
	bool connectionReady;
	bool m_IsDetecting;
	bool isClientSetup;
	QPtrList<InPacket> inPool;
	QPtrList<OutPacket> outPool;
	QTimer *timer;
	bool isConnected;
	EvaNetwork *connecter;
	
	unsigned char buffer[65535];
	unsigned short packetLength;
	unsigned int bufLength;
	
	void sendOut( OutPacket *out);
	void removePacket(const int hashCode ); // remove packet which needs acknowlegement
	void removeOutRequests(const short cmd);
	void startDetecting();
	void processDetectReply(InPacket *in);
private slots:
	void isReadySlot();
	void processPacket(char *data, int len);
	void dataCommingSlot(int len);
	void packetMonitor();
	void clearAllPools();
	void slotNetworkException(int num);
};

#endif
