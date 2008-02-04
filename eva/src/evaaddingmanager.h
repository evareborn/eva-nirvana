/***************************************************************************
 *   Copyright (C) 2007 by yunfan                                          *
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

#ifndef EVAADDINGMANAGER_H
#define EVAADDINGMANAGER_H

#include <qobject.h>
#include <qcstring.h>
#include <qpixmap.h>
#include <qfile.h>
#include "evaqun.h"

class EvaPacketManager;
class EvaAddingWindow;
class EvaAddingQunWindow;
class EvaHttp;

/**
  This class is in charge of adding friends, all related messages will be processed within 
  this class. Also,

 */

class EvaAddingManager : public QObject
{
	Q_OBJECT
public:
	EvaAddingManager( );
	virtual ~EvaAddingManager();

	void setPacketManager(EvaPacketManager *packetManager);

signals:
	void buddyAdded(const unsigned int id, const QString nick, const unsigned short face, const int group);

	void requestDetails(const unsigned int id);

	void addAuthActionReply(const unsigned int id, const unsigned char auth, const bool ok);
	void joinQunAuthReply(const unsigned int id, const unsigned char reply);
private:
	EvaPacketManager *m_PacketManager;
	EvaAddingWindow *m_AddingDialog;
	EvaAddingQunWindow *m_AddingQunDialog;

	bool m_IsAddingQun;

	unsigned int m_ID;
	QString m_Nick;
	unsigned short m_Face;
	unsigned char m_AuthType; // initial case is 0xff

	QunInfo m_Qun;

	QByteArray m_AuthInfo; // could be verify code or url to ge the graphic
	bool m_IsGraphic;
	QPixmap m_Graphic;
	EvaHttp *m_Http;
	QFile m_GraphicFile;
	QString m_Session;   // session from Set-Cookie value
	bool m_IsGraphicVerified;

	QByteArray m_QuestInfo; // the value returned if answer is correct
	bool m_IsQuestVerified;

	bool m_IsInLastProcess; // true after user click to add

	// reuqest auth information(0x00AE)
	void requestAuthInfo(const bool hasGraphic = false);

	// adding rejected
	void rejectedAdding();

	// buddy has verify question for adding
	void requestQuestion(const bool isQuestion = true, const QString &answer="");

	void authInfoReady();

	void authAddEx();

	void qunAuthAdd();
public slots:
	void slotAddBuddy(const unsigned int id, const QString nick, const unsigned short face);
	void slotAddBuddy(const unsigned int id);

	void acceptAndAdd(const unsigned int id, const QString &nick, const unsigned short face);

	// Qun
	void slotAddQun(const QunInfo &info);
private slots:
	void slotAddBuddyExReply(const unsigned int id, const unsigned char reply, const unsigned char auth);
	void slotAuthInfoReply(const unsigned char cmd, const unsigned char reply,
				const unsigned char *code, const int len);
	void slotRequestQuestionReply(const unsigned char auth, const unsigned char reply,
				const unsigned char *code, const int len);

	void slotAddFriendAuthExReply(const unsigned int id, const unsigned char auth, const bool ok);

	void requestGraphic();	
	void slotGraphicDone(bool error);

	void slotFinalAdd();

	void addingFinished();

	void slotAccepAndAdd(const int);

	void slotJoinQunReply(const unsigned int id, const unsigned char authType, const QString &message);
	void slotJoinQunAuthReply(const unsigned int id, const unsigned char reply);
};

#endif

 
