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

#include "api/evasession.h"
#include "evauser.h"
#include "evaconnecter.h"
#include "evafilemanager.h"
#include "evaloginmanager.h"
#include "evaaddingmanager.h"
#include "evapacketmanager.h"
#include "evacontactmanager.h"
 
EvaSession::EvaSession( unsigned int qq, char const* password, const EvaNetworkPolicy& policy )
    : policy( policy )
{
    user = new EvaUser( qq, password );
    connecter = new EvaConnecter(policy);

    packetManager = new EvaPacketManager(this, user, connecter);
    contactManager = new EvaContactManager( this, connecter, packetManager );
    loginManager = new EvaLoginManager( this, connecter, contactManager, packetManager );

    loginManager->setStatus( Eva_Offline );
}
 
EvaSession::~EvaSession()
{
    delete user;

//X     fileManager->stopAll();
//X     delete fileManager;
    free();
}
 
void EvaSession::free()
{
    delete connecter;
    delete loginManager;
    delete contactManager;
    delete packetManager;
    connecter = NULL;
    packetManager = NULL;
    contactManager = NULL;
    loginManager = NULL;

}

EvaUser* EvaSession::getUser()
{
    return user;
}
 
EvaLoginManager* EvaSession::getLoginManager()
{ 
    return loginManager;
}

EvaContactManager* EvaSession::getContactManager()
{ 
    return contactManager;
}

EvaConnecter* EvaSession::getConnecter()
{ 
    return connecter;
}

EvaPacketManager* EvaSession::getPacketManager()
{
    return packetManager;
}
 
void EvaSession::login()
{
    loginManager->login();
}
 
void EvaSession::logout()
{
    loginManager->logout();
}
 
void EvaSession::online()
{
    loginManager->online();
}
 
void EvaSession::invisible()
{
    loginManager->invisible();
}
 
void EvaSession::leave()
{
    loginManager->leave();
}
 
void EvaSession::offline()
{
    loginManager->offline();
}

unsigned int EvaSession::getQQ() const
{
    return user->getQQ();
}

UserStatus EvaSession::getStatus() const
{ 
    return loginManager->getStatus();
}

void EvaSession::setLoginWanIp(const unsigned int ip)
{ 
    loginIp = ip;
}
void EvaSession::setLoginWanPort(const unsigned short port)
{ 
    loginPort = port;
}

void EvaSession::setLoginLanIp(const unsigned int ip) 
{ 
    lanIp = ip;
}
 
void EvaSession::setLoginLanPort(const unsigned short port)
{ 
    lanPort = port;
}

void EvaSession::setLastLoginIp(const unsigned int ip)
{ 
    lastLoginIp = ip;
}

void EvaSession::setLastLoginTime(const unsigned int time)
{ 
    lastLoginTime = time;
}

unsigned int EvaSession::getLoginWanIp() const
{ 
    return loginIp;
}

unsigned short EvaSession::getLoginWanPort() const
{ 
    return loginPort;
}

unsigned int EvaSession::getLoginLanIp() const
{ 
    return lanIp;
}

unsigned short EvaSession::getLoginLanPort() const
{ 
    return lanPort;
}

unsigned int EvaSession::getLastLoginIp() const
{ 
    return lastLoginIp;
}

unsigned int EvaSession::getLastLoginTime() const
{ 
    return lastLoginTime;
}
void EvaSession::addLoginVerifyInfo( const GraphicVerifyCode & info )
{
	codeList.push_back(info);
}

GraphicVerifyCode EvaSession::getLoginVerifyInfo( )
{
	GraphicVerifyCode code;
	if(codeList.empty()) return code;
	return codeList.front();
}


GraphicVerifyCode EvaSession::getNextLoginVerifyInfo( )
{
	GraphicVerifyCode code;
	if(codeList.empty()) return code;
	code = codeList.front();
	codeList.pop_front();
	return code;
}
 
bool EvaSession::isLoggedIn() const
{
    return loginManager->isLoggedIn();
}

void EvaSession::clearAllVerifyCodes( )
{
	if(codeList.size())
		codeList.clear();
}


int EvaSession::getNumVerifyCodes() const
{ 
    return codeList.size();
}

bool EvaSession::isLoginNeedVerify() const
{ 
    return (codeList.size() != 0);
}

// vim: sw=4 sts=4 et tw=100 
