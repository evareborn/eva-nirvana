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
#include "evasetting.h"
#include <stdlib.h>
#include <cstring>
#include <qapplication.h>
#include <qfile.h>
#include <qdir.h>
#include <qdatastream.h>
#include <qtextcodec.h>
#include <qtextstream.h>

#define DEFAULT_DOWNLOAD_DIR         "Downloads"

QString EvaSetting::filename = "eva.sys";

// helper function for reading xdg user dirs: it is required in order to take 
// care of locale stuff
void readXdgUserDirs(QString *downloads)
{
	QFile f( QDir::homeDirPath() + "/.config/user-dirs.dirs" );

	if (!f.open(IO_ReadOnly))
		return;

	// set the codec for the current locale
	QTextStream s(&f);
	s.setCodec( QTextCodec::codecForLocale() );

	QString line = s.readLine();
	while (!line.isNull())
	{
		if (line.startsWith("XDG_DOWNLOAD_DIR="))
		{
			*downloads = line.remove("XDG_DOWNLOAD_DIR=").remove("\"").replace("$HOME", QDir::homeDirPath());
			break;
		}

		line = s.readLine();
	}
	f.close();
}

EvaSetting::EvaSetting()
{
	userList.setAutoDelete(true);
	QString home = QDir::homeDirPath();
	QDir d;
	if (!d.exists(home + "/.eva")){
		if(!d.cd(home)){
			printf("EvaSetting::constructor -- can't enter user's home directory");
			return;
		}
		if(!d.mkdir(QString(".eva"),false)){
			printf("EvaSetting::constructor -- can't create Eva directory");
			return;
		}		
	}
	m_DownloadDir = home + "/" + DEFAULT_DOWNLOAD_DIR;
	readXdgUserDirs(&m_DownloadDir);
	if (!d.exists(m_DownloadDir)){
		if(!d.cd(home)){
			printf("EvaSetting::constructor -- can't enter user's home directory");
			return;
		}
		if(!d.mkdir(QString(DEFAULT_DOWNLOAD_DIR),false)){
			printf("EvaSetting::constructor -- can't create default download directory");
			return;
		}		
	}
}


EvaSetting::~EvaSetting()
{}

bool EvaSetting::saveSetting(const int id, const char * md5Pwd, const bool recorded, const bool hidden , 
			const int type, const Q_UINT32 server, const Q_UINT16 port, const QString username, const QCString base64Param)
{
	QString home = QDir::homeDirPath();
	
	QDir d;
	QString fullPath = home + "/.eva";
	if (!d.exists(fullPath)){
		if(!d.cd(home)){
			emit exceptionEvent(QString("can't enter user's home directory"));
			return false;		
		}
		if(!d.mkdir(QString(".eva"),false)){
			emit exceptionEvent(QString("can't create Eva directory"));
			return false;		
		}		
	}
	QString fullName = fullPath + "/" + filename;
	loadSetting();
	//if id exists, update information, otherwise add a new record
	int userIndex = findUser(id);
	Q_UINT8 flag = 0x00;
	if(recorded) flag|=0x01;
	if(hidden) flag |= 0x02;
	switch(type){
	case 0: 
		flag |= 0x10;  // udp;
		break;
	case 1: 
		flag |= 0x08;  // tcp;
		break;
	case 2: 
		flag |= 0x04;  // http proxy;
		break;
	}	
	
	QString s_username = " ";
	QCString s_param = " ";
	if( !username.isEmpty() && username.stripWhiteSpace() != "") s_username = username;
	if( !base64Param.isEmpty() && base64Param.stripWhiteSpace() != "") s_param = base64Param;
	if( userIndex == -1){
		//new record
		LoginRecord *record = new LoginRecord();
		
		record->id = (uint)id;
		
		char *pwd = (char *)malloc(16 * sizeof(char));
		memcpy(pwd, md5Pwd, 16);
		//FIXME memory leak here!!!!
		record->md5Pwd = (Q_UINT8 *)pwd;
		
		record->flag = flag;
		record->proxy = server;
		record->port = port;
		record->proxyUserName = s_username;
		record->base64param = s_param;
		
		userList.append(record);
		userIndex = userList.count()-1;
	}else{
		// update information
		memcpy(userList.at(userIndex)->md5Pwd, md5Pwd, 16);
		userList.at(userIndex)->flag = flag;
		userList.at(userIndex)->proxy = server;
		userList.at(userIndex)->port = port;
		
		userList.at(userIndex)->proxyUserName = s_username;
		userList.at(userIndex)->base64param = s_param;
	}
	QFile file(fullName);
	if(!file.open(IO_WriteOnly)){
		//QString msg = qApp->translate("QFile",file.errorString());
		emit exceptionEvent(fullName);
		return false;
	}
  
	QDataStream stream(&file);
	// save the lastest user's id
	stream<<(Q_UINT32)(userIndex);
	// saving now
	for(uint i=0; i<userList.count(); i++){
		LoginRecord *r= userList.at(i);
		stream<<r->id;
		stream.writeRawBytes((char *)(r->md5Pwd), 16);
		stream<<r->flag;
		stream<<r->proxy;
		stream<<r->port;
		Q_UINT8 len = strlen(r->proxyUserName.ascii());
		stream<<len;
		//stream<<r->proxyUserName;
		stream.writeRawBytes(r->proxyUserName.ascii(), len);
		len = strlen(r->base64param.data());
		stream<<len;
		stream.writeRawBytes(r->base64param.data(), len);
	}
	file.flush();
	file.close();
	return true;
}

bool EvaSetting::loadSetting( )
{
	userList.clear();
	QString fullPath = QDir::homeDirPath() + QString("/.eva/") + filename;
	
	QFile file(fullPath);
	if(!file.open(IO_ReadOnly)){
		return false;
	}
	Q_UINT32 qq;
	char *pwd;
	Q_UINT8 flag;
	Q_UINT32 server;
	Q_UINT16 port;
	QString proxyUserName;
	
	Q_UINT8 len;
	char *strTmp;
    
	QDataStream stream(&file);
	stream>>(Q_UINT32&)lastIndex;
	while(!stream.atEnd()){
		stream>>qq;
		pwd = (char *)malloc(16 * sizeof(char));
		stream.readRawBytes(pwd, 16);
		stream>>flag;
		stream>>server;
		stream>>port;
		stream>>len;
		strTmp = new char[len + 1];
		stream.readRawBytes(strTmp, len);
		strTmp[len] = 0x00;
		proxyUserName = strTmp;
		delete strTmp;
		//stream>>proxyUserName;
		stream>>len;
		strTmp = new char[len+1];
		stream.readRawBytes(strTmp, len);
		strTmp[len]=0x00;
		//stream>>param;printf("load param: %s\n", param.data());
		
		LoginRecord *record = new LoginRecord();
		record->id = qq;
		record->md5Pwd = (Q_UINT8 *)pwd;
		record->flag = flag;	
		record->proxy = server;
		record->port = port;
		record->proxyUserName = proxyUserName;
		record->base64param = strTmp;
		userList.append(record);
		delete strTmp;
	}
	file.close();
	
	return true;
}

// return index of the id user
int EvaSetting::findUser(const int id )
{
	for(uint i=0; i<userList.count(); i++){
		if(userList.at(i)->id==(uint)id)
			return i;
	}
	return -1;
}

int EvaSetting::getLastUserIndex( )
{
	return lastIndex;
}

const bool EvaSetting::isRememberChecked(const int id)
{
	int index = findUser(id);
	if(index!=-1){
		return (userList.at(index)->flag & 0x01);
	}
	return false;
}

const bool EvaSetting::isHiddenChecked(const int id)
{
	int index = findUser(id);
	if(index!=-1){
		return (userList.at(index)->flag & 0x02);
	}
	return false;
}

const QString EvaSetting::getProxyUsername(const int id)
{
	int index = findUser(id);
	if(index!=-1){
		return userList.at(index)->proxyUserName;
	}
	return "";	
}

const char *EvaSetting::getPassword(const int id)
{
	int index = findUser(id);
	if(index!=-1){
		return (char*)(userList.at(index)->md5Pwd);
	}
	return "";
}

const int EvaSetting::getConnectType(const int id) // 0: udp, 1: tcp,  2: http proxy
{
	int index = findUser(id);
	int type = 0;
	if(index!=-1){
		Q_UINT8 flag = userList.at(index)->flag;
		if(flag & 0x10)
			type = 0;
		else if(flag & 0x08)
			type = 1;
		else if(flag & 0x04)
			type = 2;
	}
	return type;
}

const int EvaSetting::getServer(const int id)
{
	int index = findUser(id);
	if(index!=-1){
		return userList.at(index)->proxy;
	}
	return 0;	
}

const short EvaSetting::getPort(const int id)
{
	int index = findUser(id);
	if(index!=-1){
		return userList.at(index)->port;
	}
	return 0;
}

const QCString EvaSetting::getProxyParam(const int id)
{
	int index = findUser(id);
	if(index!=-1){
		return userList.at(index)->base64param.stripWhiteSpace();
	}
	return "";
}



