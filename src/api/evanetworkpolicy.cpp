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

#include "api/evanetworkpolicy.h"

EvaNetworkPolicy::EvaNetworkPolicy( ConnectionType type )
    : type( type )
{}
 
EvaNetworkPolicy::EvaNetworkPolicy( ConnectionType type, QString ip, short port )
    : ip( ip ), port( port ), type( type )
{}
 
ConnectionType EvaNetworkPolicy::getConnectionType() const
{
    return type;
}
 
void EvaNetworkPolicy::setConnectionType( const ConnectionType& type )
{
    this->type = type;
}

QString EvaNetworkPolicy::getProxyIP() const
{
	return ip;
}
 
void EvaNetworkPolicy::setProxyIP( const QString& ip )
{
    this->ip = ip;
}

short EvaNetworkPolicy::getProxyPort() const
{
	return port;
}
 
void EvaNetworkPolicy::setProxyPort( short port )
{
    this->port= port;
}

QString EvaNetworkPolicy::getProxyUsername() const
{
	return proxyUsername;
}
 
void EvaNetworkPolicy::setProxyUsername( const QString& username )
{
    proxyUsername = username;
}

QString EvaNetworkPolicy::getProxyPassword() const
{
	return proxyPassword;
}
 
void EvaNetworkPolicy::setProxyPassword( const QString& password )
{
    proxyPassword = password;
}

QByteArray EvaNetworkPolicy::getProxyParam() const
{
	return proxyParam;
}

void EvaNetworkPolicy::setProxyParam( const QByteArray& param )
{
    proxyParam = param;
}
// vim: sw=4 sts=4 et tw=100 
