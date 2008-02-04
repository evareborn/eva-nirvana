/***************************************************************************
 *   Copyright (C) 2004-2005 by yunfan                                     *
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
 
#ifndef EVAUHMANAGER_H
#define EVAUHMANAGER_H

#include <qobject.h>
#include <qthread.h>
#include <qvaluelist.h>
#include <qhostaddress.h>
#include <qdatetime.h>
#include <qmap.h>
#include <qimage.h>
#include <qevent.h>
#include <qsize.h>
#include <list>
#include "evaqtutil.h"

class QSocketDevice;
class QDns;
class EvaUHPacket;
class EvaUHFile;
class EvaUHProfile;

class EvaUHManager : public QObject, public QThread {
	Q_OBJECT
public:
	EvaUHManager(QObject *receiver, const QString &dir); // set the user head directory
	~EvaUHManager();
	// the method will post UH event if profile loaded
	void initiate(QSize size = QSize(16, 16));
	void setQQList(const std::list<unsigned int> list) { mUHList = list; }
	virtual void run();
	void stop();
	QString getFileName(const unsigned int id, bool isGrayscale = false); 
	QImage *getUHImage(const unsigned int id, bool isGrayscale = false); 
	QMap<unsigned int, QImage> getOnList() { return imageOnList; }
	QMap<unsigned int, QImage> getOffList() { return imageOffList; }
private:
	QObject *mReceiver;
	enum COMMAND {No_CMD, All_Info, Buddy_Info, Buddy_File, All_Done};
	COMMAND cmdSent;
	QDateTime timeSent;
	int m_retryCount;
	unsigned int AllInfoGotCounter;
	QString mUHDir;
	QMap<unsigned int, QImage> imageOnList;
	QMap<unsigned int, QImage> imageOffList;
	
	bool mAskForStop;
	char mBuffer[65535];  // used as internal buffer
	int bytesRead;
	std::list<unsigned int> mUHList;
	QSocketDevice *mSocket;
	
	EvaUHProfile *mProfileManager;
	EvaUHFile *mCurrentFile;
	
	QDns *mDns;
	QValueList<QHostAddress> mHostAddresses;
	void doDnsRequest();
	
	void send(EvaUHPacket *packet);
	void doAllInfoRequest();
	bool doInfoRequest();
	void doTransferRequest(const unsigned int id, const unsigned int sid,
				const unsigned  int start, const unsigned  int end);
	
	void processComingData();
	void processAllInfoReply();
	void processBuddyInfoReply();
	void processBuddyFileReply();
	
	void checkTimeout();
	void cleanUp();
private slots:
	void slotDnsReady();
};

#endif 
