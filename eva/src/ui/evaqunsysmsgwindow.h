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

#ifndef EVAQUNSYSMSGWINDOW_H 
#define EVAQUNSYSMSGWINDOW_H

#include "evaqunsysmsguibase.h"

#include "evapacketmanager.h"
#include <qmemarray.h>

class EvaQunSysMsgWindow : public EvaQunSysMsgUIBase
{
	Q_OBJECT
public:
	EvaQunSysMsgWindow(EvaPacketManager *packetManager);

	void setMessage(const unsigned short msgType, const unsigned char type, const unsigned int sender, const unsigned int extQunID,
					const QString message, const unsigned int internalQunID, const unsigned int commander);

	void setCode(const unsigned char* code, const unsigned short len);
	void setToken(const unsigned char* token, const unsigned short len);
signals:
	void requestDetails(const unsigned int id);
	void requestQunDetails(const unsigned int id);

private:
	EvaPacketManager *m_PacketManager;

	unsigned int m_QunID;
	unsigned int m_QunExtID;
	unsigned int m_Sender;
	unsigned int m_Commander;
	unsigned short m_MsgType;
	unsigned char m_QunType;
	QString m_Message;
	QByteArray m_Code;
	QByteArray m_Token;

	virtual void closeEvent( QCloseEvent *e);
private slots:
	void slotQQClicked();
	void slotQunClicked();
	void slotOkClicked();
	void slotRejectClicked();
	void slotJoinQunAuthReply(const unsigned int id, const unsigned char reply);
};


#endif
