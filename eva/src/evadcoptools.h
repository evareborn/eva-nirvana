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
 
#ifndef EVA_DCOP_TOOLS_INTERFACE_H
#define EVA_DCOP_TOOLS_INTERFACE_H

#include <dcopobject.h>
#include <qstring.h>

class EvaDCOPToolsInterface : virtual public DCOPObject
{
	K_DCOP
	k_dcop:

	virtual bool stopScript(const QString &name) = 0;
	virtual bool runScript(const QString &name, bool silent = false) = 0;
	virtual QString scriptDir( bool isSystem ) = 0;
	virtual int numScripts() = 0;
	virtual int numRunningScripts() = 0;
};

#endif // EVA_DCOP_TOOLS_INTERFACE_H


