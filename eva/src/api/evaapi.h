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

#ifndef EVAAPI_H
#define EVAAPI_H

#include "evaconnecter.h"
#include "evahtmlparser.h"
#include "evanetwork.h"
#include "evapacketmanager.h"
#include "evaresource.h"
#include "evasetting.h"
#include "evasocket.h"
#include "evauser.h"
#include "evausersetting.h"
#include "evaservers.h"
#include "qmdcodec.h"
#include "evaqtutil.h"
#include "evapicmanager.h"
#include "evauhmanager.h"
#include "regiongrabber.h"
#include "filetrans/evafilemanager.h"
#include <qtextcodec.h>
#include <klocale.h>

enum EVA_NOTIFY
{
	E_Err = 0,
	E_SvrConnected,
	E_SvrRedirect,
	E_SvrLoginFailed,
	E_PwWrong,
	E_LoggedIn,
	E_MyInfo,
	E_KeyFileAgent,
	E_LoginFinished,
	E_ContactsDownloading,
	E_ContactsDone,
	E_GroupNameDownloadDone,
	E_GroupContactsReceived,
	E_GroupContactsDone,
	E_QunInfoFinished,
	E_QunMemberFinished,
	E_LoginProcessDone,
	E_ContactDetailDone,
	E_SignatureDone,
	E_AllSignatureDone,
	E_LevelDone,
	E_Count
};

static QString s_ENotify[E_Count] =
{
	I18N_NOOP("Something error"),
	I18N_NOOP("Server connected"),
	I18N_NOOP("Login redirection"),
	I18N_NOOP("Login failed"),
	I18N_NOOP("Password wrong"),
	I18N_NOOP("User logged in"),
	I18N_NOOP("My details ready"),
	I18N_NOOP("File agent key ready"),	
	I18N_NOOP("Login finished"),
	I18N_NOOP("Downloading contact list"),
	I18N_NOOP("Downloading contacts finished"),	
	I18N_NOOP("Downloading group names finished"),
	I18N_NOOP("Received group index of contacts"),
	I18N_NOOP("Downloading group index of contacts finished"),
	I18N_NOOP("Qun information ready"),
	I18N_NOOP("Qun member information ready"),
	I18N_NOOP("Contact signature ready"),
	I18N_NOOP("All contact signatures ready"),
	I18N_NOOP("Contact level ready"),
	I18N_NOOP("Login process finished"),
	""
};

#endif //EVAAPI_H
