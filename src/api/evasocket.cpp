/***************************************************************************
 *   Copyright (C) 2004-2005 by yunfan                                     *
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
#include "evasocket.h"
#include "qmdcodec.h"
//Added by qt3to4:
#include <Q3TextStream>
#include <Q3Dns>
#include <Q3Url>
#include <QList>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#include <sys/mman.h>
#endif
#include <stdlib.h>
#include <qsocketnotifier.h>
#include <qmutex.h>
#include <qtimer.h>
#include <qstringlist.h>
#include <qiodevice.h>
 
#include <QUdpSocket>
#include <QTcpSocket>
//X #include <kdebug.h>



/*!
	\class EvaSocket evasocket.h
	\brief The EvaSocket class provides a UDP or TCP connection
	
	It provides a very simple non-blocking socket connection.
	socketWriteNotifier is only used once to notify that the connection is ready
 	to write, after isReady() emited, socketWriteNotifier is disabled unless 
	setHost() is called.
 */
EvaSocket::EvaSocket(const  QHostAddress &host, const short port, const Type type)
    : connectSocket( NULL )
//X 	: socketReadNotifier(NULL), socketWriteNotifier(NULL)
{
	connectionType = type;
	receivedLength = 0;
	receivedBuffer = NULL;
 
        setHost( host, port );

//X 	connectionStatus = None;
//X         server = host;
//X 	serverPort = port;
//X 	if(connectionType == UDP){
//X //X 		connectSocket = new Q3SocketDevice(Q3SocketDevice::Datagram); 
//X 		connectSocket = new QUdpSocket(); 
//X 	}else{
//X //X 		connectSocket = new Q3SocketDevice(Q3SocketDevice::Stream);
//X 		connectSocket = new QTcpSocket();
//X 		connectSocket->setBlocking(false);
//X //X         	socketWriteNotifier =  new QSocketNotifier(connectSocket->socket(),
//X //X 							QSocketNotifier::Write,0,"writeNotifier");
//X //X 		QObject::connect(socketWriteNotifier,SIGNAL(activated(int)),SLOT(slotWriteReady(int)));
//X //X 		socketWriteNotifier->setEnabled(false);
//X 	}
//X //X 	socketReadNotifier = new QSocketNotifier(connectSocket->socket(),
//X //X 						QSocketNotifier::Read,0,"readNotifier");
//X 	connectionStatus = Init;
//X         QObject::connect( connectSocket, SIGNAL( readReady() ), SLOT( slotReceiveReady( int ) ) );
//X //X 	QObject::connect(socketReadNotifier,SIGNAL(activated(int)),SLOT(slotReceiveReady(int)));
//X //X 	socketReadNotifier->setEnabled(false);
}

EvaSocket::~EvaSocket()
{
	delete connectSocket;
//X 	if(socketReadNotifier) {
//X 		socketReadNotifier->setEnabled(false);
//X 		delete socketReadNotifier;
//X 	}
//X 	if(socketWriteNotifier) {
//X 		socketWriteNotifier->setEnabled(false);
//X 		delete socketWriteNotifier;
//X 	}
}

const QHostAddress EvaSocket::getSocketAddress( )
{
	if(connectSocket) return connectSocket->localAddress();
	return QHostAddress();
}

unsigned short EvaSocket::getSocketPort( )
{
	if(connectSocket) return connectSocket->localPort();
	return 0;
}

void EvaSocket::setHost(const QHostAddress &address, const short port)
{
	server = address;
	serverPort = port;  
	connectionStatus = None;
	if(connectSocket && connectSocket->isValid()){
		delete connectSocket;
//X 		if(socketReadNotifier) {
//X 			socketReadNotifier->setEnabled(false);
//X 			delete socketReadNotifier;
//X 		}
//X 		if(socketWriteNotifier) {
//X 			socketWriteNotifier->setEnabled(false);
//X 			delete socketWriteNotifier;
//X 		}
        }

        if(connectionType == UDP){
		connectSocket = new QUdpSocket(); 
//X                 connectSocket = new Q3SocketDevice(Q3SocketDevice::Datagram); 
        }else{
		connectSocket = new QTcpSocket();
//X 		connectSocket->setBlocking(false);
//X                 connectSocket = new Q3SocketDevice(Q3SocketDevice::Stream); 
//X                 connectSocket->setBlocking(false);
//X 			socketWriteNotifier =  new QSocketNotifier(connectSocket->socket(),
//X 								QSocketNotifier::Write,0,"writeNotifier");
//X 			QObject::connect(socketWriteNotifier,SIGNAL(activated(int)),SLOT(slotWriteReady(int)));
//X 			socketWriteNotifier->setEnabled(false);
        }
//X 		socketReadNotifier = new QSocketNotifier(connectSocket->socket(),
//X 							QSocketNotifier::Read,0,"SocketNotifier");
//X 		QObject::connect(socketReadNotifier,SIGNAL(activated(int)),SLOT(slotReceiveReady(int)));
//X 		if(connectionType == TCP)
//X 			socketReadNotifier->setEnabled(false);
	connectionStatus = Init;
        QObject::connect( connectSocket, SIGNAL( readyRead() ), SLOT( slotReceiveReady() ) );
        QObject::connect( connectSocket, SIGNAL( error(QAbstractSocket::SocketError) ), SLOT( slotError(QAbstractSocket::SocketError) ) );
}

void EvaSocket::closeConnection()
{
	if(connectSocket->isOpen()) connectSocket->close();
	connectionStatus = None;
	receivedLength = 0;
}

void EvaSocket::startConnecting()
{
	if(connectionStatus != Init) {
		emit exceptionEvent(connectionStatus);
		return;
	}
	connectionStatus = Connecting;
        connectSocket->connectToHost(server, serverPort);
	if(connectionType == TCP){
//X 		if(!connectSocket->connectToHost(server, serverPort)){
//X 			fprintf(stderr,"connecting server failed\nError type: ");
//X 			connectionStatus = Failed;
//X 			switch(connectSocket->error()){
//X 				case Q3SocketDevice::NoError:
//X 					fprintf(stderr,"NoError\n");
//X 					break;
//X 				case Q3SocketDevice::AlreadyBound:
//X 					fprintf(stderr,"AlreadyBound\n");
//X 					break; 
//X 				case Q3SocketDevice::Inaccessible:
//X 					fprintf(stderr,"Inaccessible\n");
//X 					break;
//X 				case Q3SocketDevice::NoResources:
//X 					fprintf(stderr,"NoResources\n");
//X 					break;
//X 				case Q3SocketDevice::InternalError:
//X 					fprintf(stderr,"InternalError\n");
//X 					break;
//X 				case Q3SocketDevice::Impossible:
//X 					fprintf(stderr,"Impossible\n");
//X 					break;
//X 				case Q3SocketDevice::NoFiles:
//X 					fprintf(stderr,"NoFiles\n");
//X 					break;
//X 				case Q3SocketDevice::ConnectionRefused:
//X 					fprintf(stderr,"ConnectionRefused\n");
//X 					break;
//X 				case Q3SocketDevice::NetworkFailure:
//X 					fprintf(stderr,"NetworkFailure\n");
//X 					break;
//X 				case Q3SocketDevice::UnknownError:
//X 					fprintf(stderr,"UnknownError\n");
//X 					break;
//X 				default:
//X 					printf("not listed error\n");
//X 			}
//X 			emit exceptionEvent(connectionStatus);
//X 			return;
//X 		}
	}
        emit isReady();
//X 	if(socketReadNotifier) socketReadNotifier->setEnabled(true);
//X 	if(connectionType == TCP && socketWriteNotifier) {
//X 		socketWriteNotifier->setEnabled(true);
//X 	}else{
		connectionStatus = Ready;
//X 	}
}

bool EvaSocket::write(const char *buf, const int len)
{
	if(connectionStatus != Ready || !buf ) return false;
	if(!connectSocket->isValid()){
//X 		if(connectionType == TCP && socketReadNotifier && socketWriteNotifier){
//X 			socketReadNotifier->setEnabled(false);
//X 			socketWriteNotifier->setEnabled(false);
//X 		}
		emit exceptionEvent(Failed);
		return false;
	}
	QMutex mutex;
	mutex.lock();
	int BytesSent = 0;
//X 	if(socketWriteNotifier) socketWriteNotifier->setEnabled(false);
	if(connectionType == UDP){
//X 		BytesSent =connectSocket->writeBlock(buf, len, server, serverPort);
		BytesSent =connectSocket->write(buf, len);
	}else{
		int bytes = 0;
		int times = 0;
		while(BytesSent < len){
			bytes =connectSocket->write(buf + BytesSent, len - BytesSent);
			if(bytes == -1) {
				printf("EvaSocket::write retry :%d\n", times);
				if(!connectSocket->error()){
					if(times>20){
						fprintf(stderr, "EvaSocket::write -- error : retried %d times\n", times);
						mutex.unlock();
						return false;
					}
					#ifdef _WIN32
					Sleep(10000);
					#else
					usleep(10000);
					#endif
					//qApp->processEvents();
					times++;
					continue;
				}else
					break;
			}
			BytesSent += bytes;
		}
	}
	mutex.unlock();
	if(len != BytesSent){
		printf("EvaSocket::write -- error code: %d\n", connectSocket->error());
		return false;
	}
	return true;
}

bool EvaSocket::read(char *buf, int len)
{
	if(connectionStatus != Ready || receivedLength != len || !buf ){
		printf("EvaSocket::read -- receivedLength: %d, len: %d\n", receivedLength, len);
		return false;
	}
	memcpy(buf, receivedBuffer, receivedLength);
//X 	if(socketReadNotifier) socketReadNotifier->setEnabled(true);
	return true;
}

void EvaSocket::setWriteNotifierEnabled( bool /*enabled*/ )
{
//X 	if(socketWriteNotifier) socketWriteNotifier->setEnabled(enabled);
}
 
void EvaSocket::slotError(QAbstractSocket::SocketError) {
    printf( "EvaSocket error: %s\n", connectSocket->errorString().toStdString().c_str() );
}

void EvaSocket::slotWriteReady(int /*socket */)
{
//X 	if(socketWriteNotifier) socketWriteNotifier->setEnabled(false);
	if(connectionStatus == Connecting){
		connectionStatus = Ready;
		emit isReady();
	} else{
		emit writeReady();
	}
}

void EvaSocket::slotReceiveReady()
{
//X 	if( (socketReadNotifier->type() != QSocketNotifier::Read) || (!connectSocket->isValid()) ){
//X 		socketReadNotifier->setEnabled(false);
//X 		printf("EvaSocket::slotReceiveReady -- socket not valid or notifier not set to Read \n");
//X 		emit exceptionEvent(Failed);
//X 		return;
//X 	}
		
	int ByteCount = 0;
	ByteCount = connectSocket->bytesAvailable();
	
	if(receivedBuffer!=NULL) delete receivedBuffer;
	receivedBuffer = new char[ByteCount * 2];  
	
	receivedLength = connectSocket->read(receivedBuffer,ByteCount*2);
	if(!receivedLength){
		printf("EvaSocket::slotReceiveReady -- connection closed due to ZERO byte\n");
//X 		socketReadNotifier->setEnabled(false);
		emit exceptionEvent(Failed);
		return;
	}
	
	if(receivedLength == -1){
		printf("EvaSocket::slotReceiveReady -- read return -1\n");
		emit exceptionEvent(Failed);
		return;
	}
//X 	if(socketReadNotifier) socketReadNotifier->setEnabled(false);
	emit receivedData(receivedLength);
	if(receivedLength != ByteCount)
		printf("EvaSocket::slotReceiveReady -- bytesAvailable() might not be accurate.\n");
}


/* =========================================================== */
EvaHttpProxy::EvaHttpProxy(const QHostAddress &proxyHost, const short proxyPort, const QString username, const QString password)
	: EvaSocket(proxyHost, proxyPort, EvaSocket::TCP),
	status(Proxy_None),
	destinationAddress(""),
	base64AuthParam(""),
	readBuffer(NULL)
{
	if(username!=QString::null && password!= QString::null){
		setAuthParameter(username, password);
	}
	QObject::connect(this, SIGNAL(isReady()), SLOT(tcpReady()));
	QObject::connect(this, SIGNAL(writeReady()), SLOT(slotWriteReady()));
	QObject::connect(this, SIGNAL(receivedData(int)), SLOT(parseData(int)));
	QObject::connect(this, SIGNAL(exceptionEvent(int)), SIGNAL(socketException(int)));
}

void EvaHttpProxy::setDestinationServer(const QString &server, short port) // server could be IP or URL
{
	destinationAddress = server + ':' + QString::number(port);// qq http proxy server port: 443
	status = Proxy_None;
}

void EvaHttpProxy::setAuthParameter(const QString &username, const QString &password)
{
	QByteArray para = (username + ':' + password).local8Bit();
	base64AuthParam = QCodecs::base64Encode(para);
	status = Proxy_None;
}

bool EvaHttpProxy::doInitConnecting()
{
	if(getStatus() != EvaSocket::Ready) return false;
	if(destinationAddress == "") return false;
	sentBuffer = "CONNECT " + destinationAddress.local8Bit() + " HTTP/1.1\r\n" +
		"Accept: */*\r\n" + "Content-Type: text/html\r\nProxy-Connection: Keep-Alive\r\n" +
		"Content-length: 0\r\n\r\n";
	status = Proxy_Connecting;
	return write(sentBuffer.data(), sentBuffer.length());
}

bool EvaHttpProxy::doAuthConnecting()
{
	if(getStatus() != EvaSocket::Ready) return false;
	if(destinationAddress == "") return false;
	if(base64AuthParam == "") return false;
	
	sentBuffer = "CONNECT " + destinationAddress.local8Bit() + " HTTP/1.1\r\n" +
		"Proxy-Authorization: Basic " + base64AuthParam + "\r\n" +
		"Accept: */*\r\nContent-Type: text/html\r\nProxy-Connection: Keep-Alive\r\n" +
		"Content-length: 0\r\n\r\n";
	status = Proxy_Connecting;
	return write(sentBuffer.data(), sentBuffer.length());
}

void EvaHttpProxy::tcpReady()
{
	printf("EvaHttpProxy::tcpReady -- TCP connection ready\n");
	if(destinationAddress == "") {
		emit proxyEvent(Proxy_TCP_Ready);
		return;
	}
	
	if(base64AuthParam != "")
		doAuthConnecting();
	else 
		doInitConnecting();
}

void EvaHttpProxy::slotWriteReady()
{
	if ( status == Proxy_Ready )
		emit proxyWriteReady();
}

void EvaHttpProxy::parseData(int len)
{	
	if(readBuffer!=NULL) free(readBuffer);
	readBuffer = (char *)malloc((len+1) * sizeof(char));
	if(!read(readBuffer, len)){ 
		emit proxyEvent(Proxy_Read_Error);
		return;
	}
	readBuffer[len]=0x00;
	QString replyBuffer(readBuffer);
	if(replyBuffer.startsWith("HTTP/1.")){ // this is for RedHat 9, the old Qt dosen't support QString::startsWith(const QString &str, bool cs) const
		int replyCode = replyBuffer.mid(9, 3).toInt();
		fprintf(stderr, "Proxy Server Reply Code: %d\n",replyCode);
		switch(replyCode){
		case 200:
			status = Proxy_Ready;
			emit proxyEvent(Proxy_Ready);
			break;
		case 407:  //Proxy Authentication Required
			status = Proxy_Need_Auth;
			emit proxyEvent(Proxy_Need_Auth);
			break;
		case 501:  // "Not Support"
		case 502:  // "Proxy Error"
		default:
			status = Proxy_Error;
			emit proxyEvent(Proxy_Error);
			break;
		}
		return;
	}
	if(status == Proxy_Ready)
		dataArrived(len);
}



/** ============================================================== **/



HttpHeader::HttpHeader()
	: m_HeaderLen(0),
	m_ContentLen(0),
	m_Username(""),
	m_Password(""),
	m_Base64AuthParam("")
{
}

HttpHeader::HttpHeader(const QByteArray &data)
	: m_HeaderLen(0),
	m_ContentLen(0),
	m_Username(""),
	m_Password(""),
	m_Base64AuthParam("")
{
	parseHeader(data);
}

bool HttpHeader::parseHeader(const QByteArray &data)
{
	QByteArray buf(data);
	if (buf.left(strlen(HTTP_VERSION)) != HTTP_VERSION){
		//kdDebug() << "[HttpHeader] Not a HTTP command return, but might be data packet" << endl;
		return false;
	}
	int index = buf.find(HTTP_HEADER_END);
	if(index == -1) return false;


	m_HeaderLen = index + 1 + 3;// index + 1 + strlen("\r\n\r\n")
	/* NOTE:we use QString here for easy parsing */
	QString header = buf.left(index + 1);
	QStringList lines = QStringList::split(HTTP_NEW_LINE, header);
	QStringList::Iterator lineIt = lines.begin();
	/* first line should be the return code */
	QString ret = *(lineIt++);
	QStringList items = QStringList::split(" ", ret);
	if(items.count() != 3){
//X 		kdDebug() << "[HttpHeader] Http header unknown: " << ret << endl;
		return false;
	}
	QStringList::Iterator itemIt = items.begin();
	setMetaData(HTTP_REPLY_CODE, *(++itemIt));
	
	for( ; lineIt != lines.end(); ++lineIt){
		ret = *lineIt;
		QStringList items = QStringList::split(": ", ret);
		if(items.count() == 2){
			if(items[0] == HTTP_SET_COOKIE){
				QStringList cookies = QStringList::split(":", items[1]);
				m_Cookies[cookies[0]] = cookies[1];
			} else {
				setMetaData(items[0], items[1]);
				}
		} else {
//X 			kdDebug() << "[HttpHeader] line contains different parts" << ret << endl;
		}
	}
	return true;
}

const QByteArray HttpHeader::toCString()
{
	return "GET /forum/ HTTP/1.1\r\nHost: www.myswear.net\r\nUser-Agent: Eva 0.4.2\r\nAccept: */*\r\nConnection: Keep-Alive\r\n\r\n";
}

QByteArray HttpHeader::getProxyConnectHeader( const QString &destHost, const unsigned short port, const bool needAuth)
{
	QByteArray buf;
	Q3TextStream stream(buf, QIODevice::WriteOnly);
	stream << HTTP_CONNECT << " " << (destHost + ":" + QString::number(port)) << " " << HTTP_VERSION << HTTP_NEW_LINE;
	if(needAuth){
		stream << HTTP_PROXY_BASIC;
		
		if(m_Base64AuthParam.isEmpty()){
			stream << QCodecs::base64Encode( (m_Username + ":" + m_Password).local8Bit()).data();
		} else
			stream << m_Base64AuthParam.data();
		
		stream << HTTP_NEW_LINE;
	}
	stream << HTTP_ACCEPT_ALL << HTTP_NEW_LINE;
	stream << HTTP_CONTENT_TYPE << ": text/html" << HTTP_NEW_LINE;
	stream << HTTP_PROXY_CONNECTION << ":" << HTTP_KEEP_ALIVE << HTTP_NEW_LINE;
	stream << HTTP_CONTENT_LENGTH << ": 0" << HTTP_NEW_LINE;
	stream << HTTP_NEW_LINE;
	return buf;
}

QByteArray HttpHeader::getCmdGetHeader(const bool useProxy, const bool needAuth)
{
	QString buf;
	Q3TextStream stream(buf, QIODevice::WriteOnly);
	stream << HTTP_CMD_GET << " ";
	if(useProxy){
		stream << "http://" << getMetaData(HTTP_HOST);
	}
	stream << getMetaData(HTTP_CMD_GET) << " " << HTTP_VERSION << HTTP_NEW_LINE;
	
	stream << HTTP_HOST << ": " << getMetaData(HTTP_HOST) << HTTP_NEW_LINE;
	if(needAuth){
		stream << HTTP_PROXY_BASIC;
		
		if(m_Base64AuthParam.isEmpty()){
			stream << QCodecs::base64Encode( (m_Username + ":" + m_Password).local8Bit()).data();
		} else
			stream << m_Base64AuthParam.data();

		stream << HTTP_NEW_LINE;
	}
	stream << HTTP_USER_AGENT << ": " << HTTP_DEFAULT_AGENT << HTTP_NEW_LINE;
	stream << HTTP_ACCEPT_ALL << HTTP_NEW_LINE;
	stream << HTTP_ACCEPT_LANGUAGE << ": " << HTTP_ACCEPT_DEFAULT_LANG << HTTP_NEW_LINE;
	stream << HTTP_ACCEPT_ENCODING << ": " <<  HTTP_ACCEPT_DEFAULT_ENCODE << HTTP_NEW_LINE;
	stream << HTTP_ACCEPT_CHARSET << ": " <<  HTTP_ACCEPT_DEFAULT_CHARSET << HTTP_NEW_LINE;
	stream << HTTP_KEEP_ALIVE << ": 300" << HTTP_NEW_LINE;
	
	if(useProxy)
		stream << HTTP_PROXY_CONNECTION << ": " << QString(HTTP_KEEP_ALIVE).lower() << HTTP_NEW_LINE;
	else
		stream << HTTP_CONNECTION << ": " << QString(HTTP_KEEP_ALIVE).lower() << HTTP_NEW_LINE;
		
	if(m_Cookies.count()){
		stream << HTTP_COOKIE << ": ";
		QMap<QString, QString>::Iterator it;
		for( it = m_Cookies.begin(); it != m_Cookies.end(); ++it){
			if(it != m_Cookies.begin()){
				stream << "; ";
			}
			stream << it.key() << "=" << it.data();
		}
		stream << HTTP_NEW_LINE;
	}
	stream << HTTP_NEW_LINE;
	return buf.latin1();
}

void HttpHeader::setCookie(const QString &name, const QString &value)
{
	m_Cookies[name] = value;
}

void HttpHeader::setGetURI(const QString &uri)
{
	setMetaData(HTTP_CMD_GET, uri);
}

void HttpHeader::setHost(const QString &host)
{
	setMetaData(HTTP_HOST, host);
}

void HttpHeader::setAuthInfo(const QString &user, const QString &password)
{
	m_Username = user;
	m_Password = password;
}

void HttpHeader::setBase64AuthParam( const QByteArray & param )
{
	m_Base64AuthParam = param;
}

const QString HttpHeader::getReplyCode() const
{
	return getMetaData(HTTP_REPLY_CODE);
}

const QMap<QString, QString> &HttpHeader::getCookies() const
{
	return m_Cookies;
}

const QString HttpHeader::getCookie(const QString &name) const
{
	return m_Cookies[name];
}

unsigned int HttpHeader::getContentLength() 
{
	QString strLen = getMetaData(HTTP_CONTENT_LENGTH);
	bool ok = false;
	int len = strLen.toUInt(&ok);
	if(ok) return len;
	else
		return 0;
}

unsigned int HttpHeader::getHeaderLength() const
{
	return m_HeaderLen;
}

unsigned int HttpHeader::getContentsOffset() const
{
	return m_HeaderLen;
}

void HttpHeader::setMetaData(const QString &field, const QString &value)
{
	m_Fields[field.lower()] = value;
}

const QString HttpHeader::getMetaData(const QString &field) const
{
	return m_Fields[field.lower()];
}



/** ============================================================== **/



EvaHttp::EvaHttp( const QString & host, const unsigned short port )
	: EvaSocket( QHostAddress( host ), port , EvaSocket::TCP),
	m_UseProxy(false),
	m_NeedAuth(false),
	m_IsProxyReady(false),
	m_ProxyHost(""),
	m_ProxyPort(8080),
	m_Host(host),
	m_Port(port),
	m_IODevice(0),
	m_BytesReceived(0)
{
	m_Header.setHost(m_Host);

	QObject::connect(this, SIGNAL(isReady()), SLOT(tcpReady()));
	QObject::connect(this, SIGNAL(writeReady()), SLOT(slotWriteReady()));
	QObject::connect(this, SIGNAL(receivedData(int)), SLOT(parseData(int)));

}

void EvaHttp::setHost(const QString &host, const unsigned short port)
{
	m_Host = host;
	m_Port = port;
	m_Header.setHost(m_Host);
}

void EvaHttp::setProxyServer( const QString & host, unsigned short port )
{
	if(host.isEmpty()) return;
	m_IsProxyReady = false;
	m_UseProxy = true;
	m_ProxyHost = host;
	m_ProxyPort = port;
}

void EvaHttp::setProxyAuthInfo( const QString & username, const QString & password )
{
	if(username.isEmpty() || password.isEmpty()) return;
	//kdDebug() << "[EvaHttp] u:" << username << ", p" << password << endl;
	m_IsProxyReady = false;
	m_NeedAuth = true;
	m_Header.setAuthInfo(username, password);
}

void EvaHttp::setBase64AuthParam( const QByteArray & param )
{
	if(param.isEmpty()) return;
	m_IsProxyReady = false;
	m_NeedAuth = true;
	m_Header.setBase64AuthParam( param);
}

void EvaHttp::get( const QString & path, QIODevice * to )
{
	m_BytesReceived = 0;
	m_IODevice = to;
	if(path.startsWith("/")){
		m_Header.setGetURI(path);
	} else{
		Q3Url url(path);
		if(!url.isValid()){
//X 			kdDebug() << "[EvaHttp] uri of get is malformed" << endl;
			emit requestFinished(true);
			return;
		} else {
			//kdDebug() << "[EvaHttp] set host " << url.host() << endl;
			QString p;
			if(path.startsWith("http://"))
				p = path.right( path.length() - 7); //get rid of "http://"
			setHost( url.host());
			QString rest = p.right( p.length() - p.find("/"));
			m_Header.setGetURI(rest);
		}
	}
	if(!m_UseProxy)
		startDnsLookup(m_Host);
	else{
		EvaSocket::setHost(QHostAddress(m_ProxyHost), m_ProxyPort);
		startConnecting();
	}
}

void EvaHttp::tcpReady( )
{
	QByteArray toSend;
	if(m_UseProxy){
		toSend = m_Header.getCmdGetHeader(true, m_NeedAuth);
		//kdDebug() << "[EvaHttp] ("<< toSend.length() << ") connecting to proxy:\r\n" << endl;
		
	}else{
		toSend = m_Header.getCmdGetHeader();
		//kdDebug() << ("[EvaHttp] sending GET command:\r\n" + toSend) << endl;
	}
	write(toSend.data(), toSend.length());
}

void EvaHttp::slotWriteReady( )
{
}

void EvaHttp::parseData( int len )
{
	//kdDebug() << "[EvaHttp] data comes (" << len << ")" << endl;
	
	char *readBuffer = new char[len];
	if(!read(readBuffer, len)){
//X 		kdDebug() << "[EvaHttp] read data error" << endl;
		closeConnection();
		emit requestFinished(true);
		return;
	}
	if(m_BytesReceived && (m_BytesReceived >= m_Header.getContentLength())) return;

	QByteArray buf;
	buf.setRawData(readBuffer, len);
	//kdDebug() << "[EvaHttp] raw data:\n" << buf << endl;
	if(m_Header.parseHeader(buf)){
		bool ok = false;
		int replyCode = m_Header.getReplyCode().toInt(&ok);
		if(!ok){
//X 			kdDebug() << "[EvaHttp] Http reply code error" << endl;
			closeConnection();
			emit requestFinished(true);
			return;
		}
		switch(replyCode){
		case 200:
			if(m_UseProxy && !m_IsProxyReady){
				m_IsProxyReady = true;
				QByteArray toSend = m_Header.getCmdGetHeader();
				write(toSend.data(), toSend.length());
			}else{ 
				// do nothing, just wait for the contents
			}
			break;
		case 407:  //Proxy Authentication Required
//X 			kdDebug() << "[EvaHttp] Http reply code 407 (Proxy Authentication Required)" << endl;
			closeConnection();
			emit requestFinished(true);
			break;
		case 501:  // "Not Support"
		case 502:  // "Proxy Error"
		default:
//X 			kdDebug() << "[EvaHttp] Http reply code error" << endl;
			closeConnection();
			emit requestFinished(true);
			break;
		}
		//kdDebug() << "[EvaHttp] Received:\n" << buf.data() << endl;
		//kdDebug() << "[EvaHttp] " << len << " bytes, reply:"<< replyCode <<",HeaderLen:"<< m_Header.getHeaderLength() 
		//		<< ", contentsLength:"<<m_Header.getContentLength() << endl;
		if(len > (int)(m_Header.getHeaderLength())){
			m_BytesReceived = len - m_Header.getHeaderLength();
			int ret = m_IODevice->write(buf.data() + m_Header.getContentsOffset(), m_BytesReceived);
			if(ret == -1 || ret !=  m_BytesReceived ){
//X 				kdDebug() << "[EvaHttp] write error" << endl;
				closeConnection();
				emit requestFinished(true);
				return;
			}
			if(m_BytesReceived == m_Header.getContentLength())
				emit requestFinished(false); // now we done!!!
		}
	}else{
		int bytesToWrite = len;
		if( (m_Header.getContentLength() - m_BytesReceived) < len){
			bytesToWrite = m_Header.getContentLength() - m_BytesReceived;
			m_BytesReceived = m_Header.getContentLength();
		}else
			m_BytesReceived += len;

		if(!m_IODevice){
			emit readReady();
		}else{
			int ret = m_IODevice->write(buf.data(), bytesToWrite);
			if(ret == -1){
//X 				kdDebug() << "[EvaHttp] write error" << endl;
				closeConnection();
				emit requestFinished(true);
				return;
			}

			if(m_BytesReceived == m_Header.getContentLength()){
				//kdDebug() << "[EvaHttp] request Done!" << endl;
				emit requestFinished(false); // now we done!!!
			}
		}
	}
}

void EvaHttp::startDnsLookup( const QString & host )
{
	//kdDebug() << "{EvaHttp} looking for IP of " << host << endl;
	// host should be a URL string
	Q3Dns * dns =  new Q3Dns(host, Q3Dns::A);
	QObject::connect(dns, SIGNAL(resultsReady()), this, SLOT(getResultsSlot()));
}

void EvaHttp::getResultsSlot( )
{
	Q3Dns *dns = (Q3Dns *)(QObject::sender());
	if(dns == 0 ){
//X         	kdDebug() << "[EvaHttp] Dns lookup error" << endl;
		emit requestFinished(true);
        	return;
	}
	QList<QHostAddress> list = dns->addresses();
	if(list.count() == 0 ){
//X         	kdDebug() << "[EvaHttp] Dns lookup error - no address" << endl;
		emit requestFinished(true);
        	return;
	}
	
	QHostAddress addr = list[0];
	EvaSocket::setHost(addr, m_Port);
	//kdDebug() << "{EvaHttp] DNS ready" << endl;	
	startConnecting();
}








