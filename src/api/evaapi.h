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

enum UserStatus {Eva_Online = 10, Eva_Offline = 20, Eva_Leave = 30, Eva_Invisible = 40};

static QString s_ENotify[E_Count] =
{
	QString("Something error"),
	QString("Server connected"),
	QString("Login redirection"),
	QString("Login failed"),
	QString("Password wrong"),
	QString("User logged in"),
	QString("My details ready"),
	QString("File agent key ready"),	
	QString("Login finished"),
	QString("Downloading contact list"),
	QString("Downloading contacts finished"),	
	QString("Downloading group names finished"),
	QString("Received group index of contacts"),
	QString("Downloading group index of contacts finished"),
	QString("Qun information ready"),
	QString("Qun member information ready"),
	QString("Contact signature ready"),
	QString("All contact signatures ready"),
	QString("Contact level ready"),
	QString("Login process finished"),
	""
};

#endif //EVAAPI_H
