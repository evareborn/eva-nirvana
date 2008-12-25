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

#ifndef EVA_LOGIN_MANAGER_H 
#define EVA_LOGIN_MANAGER_H

#include <QObject>
#include <QHostAddress>
 
#include "evaapi.h"
#include "evagraphicverifycode.h"

class EvaUser;
class EvaConnecter;
class EvaContactManager;
class EvaPacketManager;
class ContactInfo;
class EvaLoginVeriWindow;

/**
 * \class EvaLoginManager
 * \brief Login process handler
 * 
 * This class handles the whole login process which includes
 * 	1. selecting login server(including login verification),
 * 	2. retrieving login token,
 * 	3. login,
 * 	4. get user information,
 * 
 * if it is the first time that user try to login Tencent server,
 * the following steps are also handled:
 * 	5. get user friend list
 * 	6. get user-defined group name
 * 	7. get friend group index(including Qun)
 * 	8. get all Quns' information(Details, members)
 * 
 * \note all other packets received during login process will be
 * 	pended until this class finish its processing
 *
 *
 */

class EvaLoginManager : public QObject
{
    Q_OBJECT
    public:
        EvaLoginManager(EvaUser* user, EvaConnecter* connecter, EvaContactManager* contactManager, EvaPacketManager* packetManager);
        void setPacketManager(EvaPacketManager *pm);

        void login();
        void logout();

        void online();
        void offline();
        void leave();
        void invisible();

        bool isLoggedIn() const;

        UserStatus getStatus() const;
        void setStatus(const UserStatus status);

        static char getStatusCode(const UserStatus status) ;
 
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
 
    signals:
 
        void loginProcessUpdate( EvaLoginProcess );


    private slots:
 
        void slotDoLogin();
        void fileAgentInfoReady();

    private:

        EvaUser* user;
        EvaConnecter* connecter;
        EvaContactManager* contactManager;
        EvaPacketManager *packetManager;

        enum LoginStatus{
            EStart,
            ELoginToken,
            ELogin,
            EUserInfo,
            EFileAgentKey,
        };

        LoginStatus loginStatus;

        UserStatus status;
//X         void notifyEvent(const int eId, const QString &msg = QString::null);

        unsigned int loginIp;
        unsigned short loginPort;
        unsigned int lastLoginIp;
        unsigned int lastLoginTime;

        unsigned int lanIp;
        unsigned short lanPort;

        std::list<GraphicVerifyCode> codeList;

        bool m_isLoggedIn;

        void serverBusy();

        void loginVerification();
        void verifyPassed();
        void loginOK();
        void wrongPassword(QString);
        void loginNeedRedirect(const unsigned int fromIp, const unsigned int ip, const short port);
        void myInfoReady(const ContactInfo info);

        EvaLoginVeriWindow *m_veriWin;

        friend class EvaPacketManager;
};

//X EvaLoginManager *GetLoginManager();
#endif //EVA_LOGIN_MANAGER_H

