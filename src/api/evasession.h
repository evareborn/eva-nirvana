/**
 * Copyright (C)2008 George Ang (gnap.an AT gmail.com)
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */ 

#ifndef API_EVASESSION_H_
#define API_EVASESSION_H_
 
#include <QObject>
#include <QHostAddress>

#include "evanetworkpolicy.h"
#include "evagraphicverifycode.h"

class EvaUser;
class EvaNetwork;
class EvaConnecter;
class EvaFileManager;
class EvaLoginManager;
class EvaAddingManager;
class EvaPacketManager;
class EvaContactManager;
 
enum UserStatus {Eva_Online = 10, Eva_Offline = 20, Eva_Leave = 30, Eva_Invisible = 40};
 
/**
 * Holds one session per one QQ account connection, all non-graphic stuff
 * would be moved here.
 */

class EvaSession : public QObject {
    Q_OBJECT

    public:
        EvaSession( const unsigned int id, const std::string& password, const EvaNetworkPolicy& policy );
        EvaSession( const unsigned int id, const char *md5Password, const EvaNetworkPolicy& policy );
 
        virtual ~EvaSession();
 
        EvaUser* getUser();
        EvaConnecter* getConnecter();
        EvaFileManager* getFileManager();
        EvaLoginManager* getLoginManager();
        EvaPacketManager* getPacketManager();
        EvaContactManager* getContactManager();

        unsigned int getQQ() const;
        bool isLoggedIn() const;
        void login();
        void logout();

        void setStatus(const UserStatus status);
        UserStatus getStatus() const;

        void setLoginWanIp(const unsigned int ip);
        void setLoginWanPort(const unsigned short port);
        void setLoginLanIp(const unsigned int ip);
        void setLoginLanPort(const unsigned short port);
        void setLastLoginIp(const unsigned int ip);
        void setLastLoginTime(const unsigned int time);

        unsigned int getLoginWanIp() const;
        unsigned short getLoginWanPort() const;
        unsigned int getLoginLanIp() const;
        unsigned short getLoginLanPort() const;
        unsigned int getLastLoginIp() const;
        unsigned int getLastLoginTime() const;

        bool isLoginNeedVerify() const;
        void addLoginVerifyInfo(const GraphicVerifyCode &info);
        GraphicVerifyCode getLoginVerifyInfo();
        GraphicVerifyCode getNextLoginVerifyInfo();
        int getNumVerifyCodes() const;
        void clearAllVerifyCodes();

        static char getStatusCode(const UserStatus status) ;

    private:
 
        EvaUser* user;

	// transferring files manager
	EvaFileManager *fileManager;

        EvaContactManager *contactManager;

        EvaLoginManager *loginManager;

        EvaNetworkPolicy policy;
	// class to connect to QQ server
	EvaConnecter *connecter;

	// packetize QQ messages
	EvaPacketManager *packetManager;

        UserStatus status;

        unsigned int loginIp;
        unsigned short loginPort;
        unsigned int lastLoginIp;
        unsigned int lastLoginTime;

        unsigned int lanIp;
        unsigned short lanPort;

        std::list<GraphicVerifyCode> codeList;

};

#endif // API_EVASESSION_H_
