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

    packetManager = new EvaPacketManager(user, connecter, this);
    loginManager = new EvaLoginManager( this, connecter, packetManager );
    contactManager = new EvaContactManager( this, packetManager );

    fileManager = new EvaFileManager(qq, this);
    status = Eva_Offline;
    printf( "EvaSession::EvaSession()\n" );
}
 
EvaSession::~EvaSession()
{
    delete user;

    fileManager->stopAll();
    delete fileManager;
    delete connecter;
    delete loginManager;
    delete contactManager;
    delete packetManager;
}
char EvaSession::getStatusCode(const UserStatus status) 
{
	char statusCode;
	switch(status){
	case Eva_Online:
		statusCode = QQ_FRIEND_STATUS_ONLINE;
		break;
	case Eva_Offline:
		statusCode = QQ_FRIEND_STATUS_OFFLINE;
		break;
	case Eva_Invisible:
		statusCode = QQ_FRIEND_STATUS_INVISIBLE;
		break;
	case Eva_Leave:
		statusCode = QQ_FRIEND_STATUS_LEAVE;
		break;
	default:
		statusCode = QQ_FRIEND_STATUS_OFFLINE;
	}
	return statusCode;
}


EvaUser* EvaSession::getUser()
{
    return user;
}
 
EvaFileManager* EvaSession::getFileManager()
{
    return fileManager;
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
    packetManager->doLogout();
    loginManager->setLoggedOut();
    setStatus( Eva_Offline );
}

unsigned int EvaSession::getQQ() const
{
    return user->getQQ();
}
void EvaSession::setStatus(const UserStatus status)
{ 
    this->status = status;
}

UserStatus EvaSession::getStatus() const
{ 
    return status;
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
