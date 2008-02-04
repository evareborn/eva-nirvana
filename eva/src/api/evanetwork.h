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
#ifndef EVANETWORK_H
#define EVANETWORK_H 


#include <qobject.h>
#include <qhostaddress.h>

class EvaSocket;
class EvaHttpProxy;

class EvaNetwork : public QObject{
	Q_OBJECT
public:
	enum Type { UDP, TCP, HTTP_Proxy};
	enum Event { Init, Connecting, Ready, Failed, None, BytesReadWrong, 
			Proxy_None, Proxy_TCP_Ready, Proxy_Connecting, Proxy_Ready, 
				Proxy_Need_Auth, Proxy_Read_Error, Proxy_Error  };
	EvaNetwork(const QHostAddress &host, const short port, const Type type = UDP);
	~EvaNetwork();
	
	void setServer(const QHostAddress &address, const short port);
	const QHostAddress &getHostAddress() const;  // if it's Http Proxy, return the proxy's address
	const short getHostPort() const;
	
	void setDestinationServer(const QString &server, const short port); // for Http Proxy only;
        void setAuthParameter(const QString &username, const QString &password);
	void setAuthParameter(const QCString &param);
	void newURLRequest();
	void connect();
	
	bool read(char *buf, int len);
	bool write(const char *buf, const int len);
	void setWriteNotifierEnabled(bool enabled);

	void close();
	const Type connectionType() { return type; }
	const QHostAddress getSocketIp();
	const unsigned int getSocketPort();
signals:
	void isReady();
	void dataComming(int);
	void exceptionEvent(int); // all in enum type Event;
	void writeReady();
private:
	EvaSocket *socket;
	Type type;
private slots:
	void processProxyEvent(int);
};


#endif

