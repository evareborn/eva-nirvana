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
 
#ifndef EVA_DCOP_CONTACTS_INTERFACE_H
#define EVA_DCOP_CONTACTS_INTERFACE_H

#include <dcopobject.h>
#include <qstring.h>
#include <qstringlist.h>

class EvaDCOPContactsInterface : virtual public DCOPObject
{
	K_DCOP
	k_dcop:

	virtual int numFriends() = 0;
	virtual QStringList friends() = 0;
	virtual bool hasFriend(unsigned int id) = 0;
	virtual QString nickOf(unsigned int id) = 0;
	virtual QString faceOf(unsigned int id, bool isOff) = 0;
	virtual int genderOf(unsigned int id) = 0;
	virtual int levelOf(unsigned int id) = 0;
	virtual QString signatureOf(unsigned int) = 0;
	virtual int numGroups() = 0;
	virtual QString groupName(int index) = 0;
	virtual int group(unsigned int id) = 0;
	virtual int numQuns() = 0;
	virtual QStringList Quns() = 0;
	virtual QString QunName(unsigned int ext) = 0;
	virtual QString QunNotice(unsigned int ext) = 0;
	virtual QString QunDescription(unsigned int ext) = 0;
	virtual int numQunMembers(unsigned int ext) = 0;
	virtual QStringList QunMembers(unsigned int ext) = 0;
	virtual QString QunMemberNick(unsigned int ext, unsigned int id) = 0;
	virtual QString QunMemberFace(unsigned int ext, unsigned int id, bool isOff) = 0;
	virtual unsigned int myQQ() = 0;
	virtual int onlineStatus(unsigned int id) = 0;
	virtual QString currentLoginIP() = 0;
	virtual QString lastLoginIP() = 0;
	virtual QString lastLoginTime() = 0; // format in yyyy-MM-dd hh:mm:ss
};

#endif // EVA_DCOP_CONTACTS_INTERFACE_H


