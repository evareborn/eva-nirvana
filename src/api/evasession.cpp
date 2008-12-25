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
    loginManager = new EvaLoginManager( user, connecter, contactManager, packetManager );
 
    connect( loginManager, SIGNAL( loginProcessUpdate( EvaLoginProcess ) ), SIGNAL( loginProcessUpdate( EvaLoginProcess ) ) );
    connect( packetManager, SIGNAL( error( EvaError ) ), SIGNAL( error( EvaError ) ) );
    connect( packetManager, SIGNAL( statusChanged( UserStatus ) ), SIGNAL( statusChanged( UserStatus ) ) );
    connect( packetManager, SIGNAL( friendStatusChanged( unsigned int ) ), SIGNAL( friendStatusChanged( unsigned int ) ) );
}
 
EvaSession::~EvaSession()
{
    delete user;

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
 
bool EvaSession::isLoggedIn() const
{
    return loginManager->isLoggedIn();
}
 
unsigned int EvaSession::getLoginWanIp() const
{
    return loginManager->getLoginWanIp();
}

// vim: sw=4 sts=4 et tw=100 
