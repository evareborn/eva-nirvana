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
#ifndef EVASETTING_H
#define EVASETTING_H

#include <qobject.h>
#include <qptrlist.h>

/**
Eva system setting, saved in directory ".eva" under user's $HOME directory
 
@author yunfan
*/

typedef struct LoginRecord{
Q_UINT32 id;
Q_UINT8  *md5Pwd; // note: the password is always 16 bits long. if isRecorded is false, password is 16 '0' chars
Q_UINT8  flag;  //   bit 0 to 2 reserved, bit 3: UDP, bit 4: TCP, bit 5: HTTP Proxy, bit 6: isHidden, bit 7: isRecorded( 0 for false, 117for true)
Q_UINT32 proxy;
Q_UINT16 port;
QString  proxyUserName;
QCString base64param; 
} LoginRecord;

class EvaSetting : public QObject
{
	Q_OBJECT
public:

	EvaSetting();
	~EvaSetting();
	// append a new login setting record
	bool saveSetting(const int id, 
					 const char * md5Pwd, 
					 const bool recorded, 
					 const bool hidden, 
					 const int type, 
					 const Q_UINT32 server, 
					 const Q_UINT16 port, 
					 const QString username, 
					 const QCString base64Param); 	
	// load all login records
	bool loadSetting();
	int getLastUserIndex(); // the last user id
	void setFilename(QString &name);
	const bool isRememberChecked(const int id);
	const bool isHiddenChecked(const int id);
	
	const char *getPassword(const int id);
	QPtrList<LoginRecord> getRecordedList() { return userList; }
	
	const int getConnectType(const int id); // 0: udp, 1: tcp,  2: http proxy
	const int getServer(const int id);
	const short getPort(const int id);
	const QString getProxyUsername(const int id);
	const QCString getProxyParam(const int id);
	
	const QString defaultDownloadDir() { return m_DownloadDir; }
	
	LoginRecord m_currLoginSetting;
	
signals:
	void exceptionEvent(QString);	
private:
	int findUser(const int id); // if id exists, return index of userList, otherwise return -1
	static QString filename;
	int lastIndex;
	QPtrList<LoginRecord> userList;
	QString m_DownloadDir;
};

#endif
