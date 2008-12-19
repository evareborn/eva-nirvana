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

#ifndef API_EVANETWORKPOLICY_H_
#define API_EVANETWORKPOLICY_H_
 
#include <QString>

 
enum ConnectionType { CONN_UDP, CONN_TCP, HTTP_Proxy};

class EvaNetworkPolicy {
    public:
       EvaNetworkPolicy() {}
       EvaNetworkPolicy( ConnectionType type );
       EvaNetworkPolicy( ConnectionType type, QString ip, short port);

       ConnectionType getConnectionType() const; 
       void setConnectionType( const ConnectionType& type );

       QString getProxyIP() const;
       void setProxyIP( const QString& proxyIP );
       short getProxyPort() const;
       void setProxyPort( short proxyPort );
       QString getProxyUsername() const;
       void setProxyUsername( const QString& username );
       QString getProxyPassword() const;
       void setProxyPassword(const QString& password );
       QByteArray getProxyParam() const;
       void setProxyParam( const QByteArray& param );
 
    protected:

       QString ip;
       short port;
       ConnectionType type;
 
       QString proxyUsername;
       QString proxyPassword;
       QByteArray proxyParam;
};

#endif // API_EVANETWORKPOLICY_H_
