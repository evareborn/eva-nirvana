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

#ifndef EVASYSMSGMANAGER_H
#define EVASYSMSGMANAGER_H

#include <qobject.h>
#include <qcstring.h>
#include <qpixmap.h>
#include <qfile.h>

class EvaPacketManager;

class EvaSysMsgManager : public QObject
{
	Q_OBJECT
public:
	EvaSysMsgManager( );
	virtual ~EvaSysMsgManager();

	void setPacketManager(EvaPacketManager *packetManager);

signals:
	void requestAddBuddy(const unsigned int id, const QString nick, const unsigned short face);
	void buddyAdded(const unsigned int id, const QString nick, const unsigned short face, const int group);

	void requestDetails(const unsigned int id);
	void requestQunDetails(const unsigned int id);

	void sysMessage();
	void qunListChanged();

private:
	EvaPacketManager *m_PacketManager;

private slots:
	void slotReceivedQQNews(const QString &title, const QString &brief, const QString &url);
	void slotReceivedSysMessage(const unsigned char type, const unsigned int myID, const unsigned int fromID,
				const QString &msg, const bool allowReverse,
				const unsigned char *code, const unsigned int codeLen);

	void slotReceivedVerifyAddingMessageReply(const unsigned int id, const unsigned char reply,
						const unsigned char flag1, const unsigned char flag2);
	void slotQunSysMessage(const unsigned int id, const unsigned short imType,
				const unsigned int ext, const unsigned char qunType,
				const unsigned int sender, const unsigned int commander,
				const QString &msg, const unsigned char *code, const unsigned short codeLen,
				const unsigned char *token, const unsigned short tokenLen);

public slots:
	void showSysMessage();
	/// NOTE: the last arg could be allowReverse(yes:1, no:0) if msgType is not Qun related
	void showSysMessage(const unsigned short msgType, const unsigned char type, const unsigned int from, const unsigned int to, 
				const QString message, const unsigned int internalQunID, const unsigned int commander,
				const unsigned char *code, const unsigned short codeLen,
				const unsigned char *token, const unsigned short tokenLen);
};

#endif
 
