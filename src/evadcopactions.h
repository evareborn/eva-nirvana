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
 
#ifndef EVA_DCOP_ACTIONS_INTERFACE_H
#define EVA_DCOP_ACTIONS_INTERFACE_H

#include <dcopobject.h>

class EvaDCOPActionsInterface : virtual public DCOPObject
{
	K_DCOP
	k_dcop:

	virtual void changeToOnline() = 0;
	virtual void changeToOffline() = 0;
	virtual void changeToLeave() = 0;
	virtual void changeToInvisible() = 0;
	virtual void changeNick(QString nick) = 0;
	virtual void changeSignature( QString contents) = 0;
	virtual void sendToContact(unsigned int id, QString msg) = 0;
	virtual void sendToQun(unsigned int ext, QString) = 0;
	virtual void textReady(unsigned int id, QString text, bool isQun) = 0;
	virtual void imageReady(unsigned int id, QString path, bool isQun) = 0;
	virtual void openAddFriendDialog(unsigned int id) = 0;
};

#endif // EVA_DCOP_ACTIONS_INTERFACE_H


 
