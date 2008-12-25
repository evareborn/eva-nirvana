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

#include "evaapi.h"
#include "evanetworkpolicy.h"
#include "evagraphicverifycode.h"

class EvaUser;
class EvaConnecter;
class EvaFileManager;
class EvaLoginManager;
class EvaAddingManager;
class EvaPacketManager;
class EvaContactManager;
 
 
/**
 * Session for each single QQ connection. This class only offers a simple
 * interface for the basic im features of QQ protocl. Optional functions
 * should be implemented in separated classes with the first arugment of
 * contructor as a pointer to active session instance.
 * 
 */

class EvaSession : public QObject {
    Q_OBJECT

    public:
 
        /**
         * id: QQ account id.
         * password: QQ account password
         * policy: we use network policies to represent connection strategies such as
         * UDP/TCP selecting, Proxy setups. The side effect is that we can have a 
         * shorter contructor argument list.
         */

        EvaSession( const unsigned int id, const std::string& password, const EvaNetworkPolicy& policy );
        EvaSession( const unsigned int id, const char *md5Password, const EvaNetworkPolicy& policy );
 
        virtual ~EvaSession();
 
        /**
         * Member asscess to the internal objects of an eva session.
         * This interface was supplied for compact purpose. We should
         * consider comment them out after the whole programme was
         * refactored properly.
         */

        EvaUser* getUser();
        EvaConnecter* getConnecter();
        EvaLoginManager* getLoginManager();
        EvaPacketManager* getPacketManager();
        EvaContactManager* getContactManager();
 
        /**
         * Methods to get the basic stuff and do status switch.
         */

        unsigned int getQQ() const;
        bool isLoggedIn() const;
        void login();
        void logout();
 
        void online();
        void offline();
        void invisible();
        void leave();

        UserStatus getStatus() const;
 
        /**
         * We call loginManager methods to get the infomation. Considering of keep
         * loginManage inaccessible, maybe we sould use a class to represent the
         * network info for a compact interface.
         */

        unsigned int getLoginWanIp() const;
 
    signals:
 
        /**
         * Re-yeild the EvaLoginManager signal as the login progresses.
         */

        void error( EvaError );
        void statusChanged( UserStatus );
        void friendStatusChanged( unsigned int );
        void loginProcessUpdate( EvaLoginProcess );

    private:
        void free();
 
        EvaUser* user;

        EvaContactManager *contactManager;

        EvaLoginManager *loginManager;

        EvaNetworkPolicy policy;

	// class to connect to QQ server
	EvaConnecter *connecter;

	// packetize QQ messages
	EvaPacketManager *packetManager;

};

#endif // API_EVASESSION_H_
