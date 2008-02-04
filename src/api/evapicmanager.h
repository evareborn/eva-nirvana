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
 
#ifndef EVAPICMANAGER_H
#define EVAPICMANAGER_H

#include <qobject.h>
#include <qptrlist.h>
#include "evahtmlparser.h"

class QCustomEvent;
class EvaUser;
class EvaNetwork;
class EvaPicOutPacket;
class EvaPicInPacket;
class QTextCodec;
class QTimer;

class EvaPicManager : public QObject {
	Q_OBJECT
public:
	EvaPicManager(EvaUser *u, bool useProxy = false);
	virtual ~EvaPicManager();
	
	void setProxyServer(const QString ip = "", const short port = 0, QCString proxyParam = "");
	void append(EvaPicOutPacket *packet);
	void stop();
signals:
	void pictureReady(const unsigned int id, const QString fileName, const QString tmpFileName);
	void pictureSent(const unsigned int id, const unsigned int sessionID, const unsigned int ip, const unsigned short port);
	void sendErrorMessage(const unsigned int id, const QString msg);
protected:
	virtual void customEvent( QCustomEvent *e);
private:
	typedef struct FileItem{
	QString filename;
	int length;
	int offset;
	unsigned short lastPacketSeq;
	unsigned char *buf;
	} FileItem;
	
	EvaUser *user;
	EvaNetwork *connecter;
	QString proxyIP;
	short proxyPort;
	bool usingProxy;
	QCString proxyAuthParam;
	int sendCount;
	
	QTextCodec *codec;
	
	bool isBusy;
	typedef struct Session {
	unsigned int qunID;
	std::list<CustomizedPic> list;
	} Session;
	
	std::list<Session> downloadList;
	std::list<CustomizedPic> picList;
	int qunID;
	CustomizedPic currentPic;
	int currentIndex;
	
	FileItem currentFile;
	
	typedef struct SessionOut{
	unsigned int qunID;
	std::list< OutCustomizedPic> list;
	} OutSession;
	
	OutCustomizedPic currentOutPic;
	std::list<OutSession> sendList;
	std::list<OutCustomizedPic> outList;
	
	unsigned short expectedSequence;
	unsigned int sessionID;
	unsigned int sendIP;
	unsigned short sendPort;
	
	void doRequestPic(CustomizedPic pic);
	void doRequestData(CustomizedPic pic, const bool isReply);
	
	void doProcessEvent();
	void initConnection(const int ip, const short port);
	
	void sendPacket(EvaPicOutPacket *packet);
	void parseInData(const EvaPicInPacket *in);
	
	unsigned char buf[65535];
	unsigned int bufLength;
	bool isSend;
	bool isAppending, isRemoving;
	
	QPtrList<EvaPicOutPacket> outPool;
	QTimer *timer;
	void removePacket(const int hashCode ); // remove packet which needs acknowlegement
	void clearManager();
private slots:
	void slotReady();
	void slotDataComming(int);
	void slotException(int);
	void packetMonitor();
	
	void slotProcessBuffer();
	
	void processRequestAgentReply(const EvaPicInPacket *in);
	void processRequestFaceReply(const EvaPicInPacket *in);
	void processTransferReply(const EvaPicInPacket *in);
	void processRequestStartReply(const EvaPicInPacket *in);
	
	void doSaveFile();
	void doRequestNextPic();
	
	void doProcessOutEvent();
	void doRequestStart();
	void doSendFileInfo();
	void doRequestAgent();
	void doSendNextFragment();
};

#endif

