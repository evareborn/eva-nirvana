/***************************************************************************
 *   Copyright (C) 2005 by yunfan                                          *
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

#include "evafilemanager.h"
#include "evafiledownloader.h"
#include <qevent.h>

EvaFileManager::EvaFileManager(const int myId, QObject *parent)
	: QObject(parent), m_MyId(myId), m_FileAgentToken(NULL), m_IsProxySet(false)
{
	m_ThreadList.setAutoDelete(true);
}

EvaFileManager::~EvaFileManager()
{
	stopAll();
	if(m_FileAgentToken) delete []m_FileAgentToken;
}

void EvaFileManager::setMyBasicInfo(const unsigned char *key, const unsigned char *token, 
				const unsigned int tokenLen)
{
	if(!token) return;
	if(m_FileAgentToken) delete [] m_FileAgentToken;
	m_FileAgentToken = new unsigned char[tokenLen];
	memcpy(m_FileAgentToken, token, tokenLen);
	m_FileAgentTokenLength = tokenLen;

	memcpy(m_FileAgentKey, key, 16);
}

void EvaFileManager::setMyProxyInfo(const QHostAddress addr, const short port, const QCString &param)
{
	m_ProxyServer = addr;
	m_ProxyPort = port;
	m_ProxyAuthParam = param;
	m_IsProxySet = true;
}

const bool EvaFileManager::newSession(const unsigned int id, const unsigned int session, 
				const QValueList<QString> &dirList, 
				const QValueList<QString> &filenameList,
				const QValueList<unsigned int> &sizeList,
				const bool isDownload, const unsigned char transferType)
{
	if(getThread(id, session)) return false;
	EvaFileThread *thread = NULL;
	if(isDownload){
		EvaUdpDownloader *dthread = new EvaUdpDownloader(this, id, dirList, filenameList, sizeList);
		dthread->setFileAgentToken(m_FileAgentToken, m_FileAgentTokenLength);
		dthread->setQQ(m_MyId);
		dthread->setSession(session);
		dthread->setTransferType(transferType);
		thread = dthread;
	} else {
		EvaUdpUploader *uthread = new EvaUdpUploader(this, id, dirList, filenameList);
		uthread->setFileAgentToken(m_FileAgentToken, m_FileAgentTokenLength);
		uthread->setFileAgentKey(m_FileAgentKey);
		uthread->setQQ(m_MyId);
		uthread->setSession(session);
		uthread->setTransferType(transferType);
		thread = uthread;
	}
	if(thread) m_ThreadList.append(thread);
	return true;
}

const bool EvaFileManager::changeToAgent(const unsigned int id, const unsigned int session)
{
	EvaFileThread *thread = getThread(id, session);
	if(!thread) return false;
	thread->stop();
	thread->wait();
	
	EvaAgentThread *newThread = NULL;
	EvaUdpUploader *upthread = dynamic_cast<EvaUdpUploader *>(thread);
	if(upthread){
		EvaAgentUploader *uthread = new EvaAgentUploader(this, 
						upthread->getBuddyQQ(), 
						upthread->getDirList(), 
						upthread->getFileNameList());
		uthread->setFileAgentToken(m_FileAgentToken, m_FileAgentTokenLength);
		uthread->setFileAgentKey(m_FileAgentKey);
		uthread->setQQ(m_MyId);
		uthread->setSession(session);
		uthread->setTransferType(upthread->getTransferType());
		newThread = uthread;
	}else{
		EvaUdpDownloader *downthread = dynamic_cast<EvaUdpDownloader *>(thread);
		if(downthread){
			EvaAgentDownloader *dthread = new EvaAgentDownloader(this, 
								downthread->getBuddyQQ(),
								downthread->getDirList(),
								downthread->getFileNameList(),
								downthread->getSizeList());
			dthread->setFileAgentToken(m_FileAgentToken, m_FileAgentTokenLength);
			dthread->setQQ(m_MyId);
			dthread->setSession(session);
			dthread->setTransferType(downthread->getTransferType());
			newThread = dthread;
		} else 
			return false;
	}
	m_ThreadList.removeRef(thread);
	if(newThread){
		if(m_IsProxySet) newThread->setProxySettings(m_ProxyServer, m_ProxyPort, m_ProxyAuthParam);
		m_ThreadList.append(newThread);
	}
	return true;
}
/*
const bool EvaFileManager::newSession(const int id, const QString &dir, 
				const QString &file, const unsigned int session, const unsigned int size,
				const bool isDirectConnection, const bool usingProxy, const bool isDownload)
{
	if(getThread(id, session)) return false;
	EvaFileThread *thread = NULL;
	if(isDownload){
		if(isDirectConnection){
		} else {
			EvaAgentDownloader *dthread = new EvaAgentDownloader(this, id, dir, file, size);
			dthread->setFileAgentToken(m_FileAgentToken, m_FileAgentTokenLength);
			dthread->setQQ(m_MyId);
			dthread->setSession(session);
			if(usingProxy){
				if(!m_IsProxySet) {
					delete dthread;
					return false;
				}
				dthread->setProxySettings(m_ProxyServer, m_ProxyPort, m_ProxyAuthParam);
			}
			thread = dthread;
		}
	} else {
		if(isDirectConnection){
			EvaUdpUploader *uthread = new EvaUdpUploader(this, id, dir, file);
			uthread->setFileAgentToken(m_FileAgentToken, m_FileAgentTokenLength);
			uthread->setFileAgentKey(m_FileAgentKey);
			uthread->setQQ(m_MyId);
			uthread->setSession(session);
			thread = uthread;
		} else {
			EvaAgentUploader *uthread = new EvaAgentUploader(this, id, dir, file);
			uthread->setFileAgentToken(m_FileAgentToken, m_FileAgentTokenLength);
			uthread->setFileAgentKey(m_FileAgentKey);
			uthread->setQQ(m_MyId);
			uthread->setSession(session);
			if(usingProxy){
				if(!m_IsProxySet) {
					delete uthread;
					return false;
				}
				uthread->setProxySettings(m_ProxyServer, m_ProxyPort, m_ProxyAuthParam);
			}
			thread = uthread;
		}
	}
	if(thread) m_ThreadList.append(thread);
	return true;
}*/

void EvaFileManager::updateIp(const unsigned int id, const unsigned int session, const unsigned int ip)
{
	EvaAgentUploader *thread = dynamic_cast<EvaAgentUploader *>(getThread(id, session));
	if(thread) thread->setBuddyIp(ip);
}

const bool EvaFileManager::startSession(const unsigned int id, const unsigned int session)
{
	EvaFileThread *thread = getThread(id, session);
	if(!thread){
		printf("EvaFileManager::startSession -- no session to run, return now.\n");
		return false;
	}
	if(thread->running()){
		printf("EvaFileManager::startSession -- session already running, return now.\n");
		return false;
	}
	thread->start();
	return true;
}

const QString EvaFileManager::getFileName(const unsigned int id, const unsigned int session, const bool isAbs)
{
	EvaFileThread *thread = getThread(id, session);
	if(!thread) return "";
	QString file = "";
	if(isAbs) file = thread->getDir() + "/";
	file += thread->getFileName();
	return file;
}

const unsigned int EvaFileManager::getFileSize(const unsigned int id, const unsigned int session)
{
	EvaFileThread *thread = getThread(id, session);
	if(!thread) return 0;
	return thread->getFileSize();
}

const unsigned char EvaFileManager::getTransferType(const unsigned int id, const unsigned int session)
{
	EvaFileThread *thread = getThread(id, session);
	if(!thread) return 0;
	return thread->getTransferType();
}

/*
void EvaFileManager::newReceiveThread()
{
}*/

void EvaFileManager::stopThread(const unsigned int id, const unsigned int session)
{
	EvaFileThread * thread = getThread(id, session);
	if(!thread) return;
	thread->stop();
	thread->wait();
	m_ThreadList.removeRef(thread);
}

void EvaFileManager::stopAll()
{
	EvaFileThread *thread;
	for(thread = m_ThreadList.first(); thread; thread = m_ThreadList.next()){
		thread->stop();
		thread->wait();
	}
	m_ThreadList.clear();
}

void EvaFileManager::customEvent(QCustomEvent *e)
{
	printf("EvaFileManager::customEvent \n");
	switch(e->type()){
	case Eva_FileNotifyAgentEvent:{
		EvaFileNotifyAgentEvent *ae = (EvaFileNotifyAgentEvent *)e;
		emit notifyAgentRequest(ae->getBuddyQQ(), ae->getOldSession(), ae->getAgentSession(),
					ae->getAgentIp(), ae->getAgentPort(), ae->getTransferType());
		}
		break;
	case Eva_FileNotifyStatusEvent:{
		EvaFileNotifyStatusEvent *se = (EvaFileNotifyStatusEvent *)e;
		emit notifyTransferStatus(se->getBuddyQQ(), se->getSession(),
				se->getFileSize(), se->getBytesSent(), se->getTimeElapsed());
		}
		break;
	case Eva_FileNotifySessionEvent:{
		EvaFileNotifySessionEvent *se = (EvaFileNotifySessionEvent *)e;
		emit notifyTransferSessionChanged(se->getBuddyQQ(), se->getOldSession(), se->getNewSession());
		}
		break;
	case Eva_FileNotifyNormalEvent:{
		EvaFileNotifyNormalEvent *ne = (EvaFileNotifyNormalEvent *)e;
		emit notifyTransferNormalInfo(ne->getBuddyQQ(), ne->getSession(), ne->getStatus(),
					ne->getDir(), ne->getFileName(), ne->getFileSize(), ne->getTransferType());
		}
		break;
	case Eva_FileNotifyAddressEvent:{
		printf("EvaFileManager::customEvent -- Eva_FileNotifyAddressEvent Got!");
		EvaFileNotifyAddressEvent *ae = (EvaFileNotifyAddressEvent *)e;
		emit notifyAddressRequest(ae->getBuddyQQ(), ae->getSession(), ae->getSynSession(), 
			ae->getIp(), ae->getPort(), ae->getMyIp(), ae->getMyPort());
		}
		break;
	default:
		break;
	}
}

EvaFileThread * EvaFileManager::getThread(const unsigned int id, const unsigned int session)
{
	EvaFileThread * thread;
	for(thread = m_ThreadList.first(); thread; thread = m_ThreadList.next()){
		if( (thread->getBuddyQQ() == id) && (thread->getSession() == session) ){
			return thread;
		}
	}
	printf("EvaFileManager::getThread -- cannot find thread: %d\n", session);
	return NULL;	
}

void EvaFileManager::changeSessionTo(const unsigned int id, const unsigned int oldSession,
				const unsigned int newSession )
{
	EvaFileThread *thread = getThread(id, oldSession);
	if(!thread)	return;
	thread->setSession(newSession);
}

void EvaFileManager::setBuddyAgentKey(const unsigned int id, const unsigned int session, const unsigned char *key)
{
	EvaAgentThread *thread = dynamic_cast<EvaAgentThread *>(getThread(id, session));
	if(thread) thread->setFileAgentKey(key);
}

void EvaFileManager::saveFileTo(const unsigned int id, const unsigned int session, const QString dir)
{
	EvaFileThread *thread = getThread(id, session);
	if(!thread) return;
	if(thread->running()) return;
	thread->setDir(dir);
}

void EvaFileManager::setAgentServer(const unsigned int id, const unsigned int session, 
					const unsigned int ip, const unsigned short port)
{
	EvaAgentThread *thread = dynamic_cast<EvaAgentThread *>(getThread(id, session));
	if(thread) thread->setServerAddress(ip, port);
}

void EvaFileManager::slotFileTransferResume( const unsigned int id, const unsigned int session, const bool isResume )
{
	EvaAgentDownloader *thread = dynamic_cast<EvaAgentDownloader *>(getThread(id, session));
	if(thread) thread->askResumeLastDownload(isResume);
}

const bool EvaFileManager::isSender(const unsigned int id, const unsigned int session, bool *isExisted)
{
	EvaFileThread *thread = getThread(id, session);
	if(!thread){
		*isExisted = false;
		return false;
	}
	*isExisted = true;
	return thread->isSender();
}

