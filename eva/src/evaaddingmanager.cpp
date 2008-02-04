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

#include "evaaddingmanager.h"

#include <string>

#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>

#include <qrect.h>
#include <qurl.h>
#include <qtextcodec.h>
#include <qpixmap.h>

#include <kmessagebox.h>
#include <klocale.h>
#include <kdebug.h>
#include <kapp.h>

#include "evamain.h"
#include "evauser.h"
#include "evaresource.h"
#include "evasetting.h"
#include "evausersetting.h"
#include "evaaddingwindow.h"
#include "evapacketmanager.h"
#include "evasocket.h" // EvaHttp
#include "evaaddqunwindow.h"


#define AUTH_TYPE_INITIAL      0xff
#define AUTH_GRAPHIC_FILE      "AUTH_GRAPHIC.JPG"

EvaAddingManager::EvaAddingManager( )
	: m_IsAddingQun(false)
{
	m_ID = 0;
	m_Qun = QunInfo();
	m_AddingDialog = new EvaAddingWindow();
	QRect scr = KApplication::desktop()->screenGeometry();
	m_AddingDialog->move(scr.center() - m_AddingDialog->rect().center());

	//TODO: we should setup a info manager to handle all details request including Qun
	QObject::connect(m_AddingDialog, SIGNAL(requestDetails(const unsigned int)), SIGNAL(requestDetails(const unsigned int)));
	QObject::connect(m_AddingDialog, SIGNAL(refreshGraphic()), SLOT(requestGraphic()));
	QObject::connect(m_AddingDialog, SIGNAL(requestAdd()), SLOT(slotFinalAdd()));
	QObject::connect(m_AddingDialog, SIGNAL(requestCancel()), SLOT(addingFinished()));
	QObject::connect(m_AddingDialog, SIGNAL(groupSelected(const int)), SLOT(slotAccepAndAdd(const int)));

	m_AddingQunDialog = new EvaAddingQunWindow;
	m_AddingQunDialog->move(scr.center() - m_AddingQunDialog->rect().center());

	//TODO: we should setup a info manager to handle all details request including Qun
	QObject::connect(m_AddingQunDialog, SIGNAL(requestDetails(const unsigned int)), SIGNAL(requestDetails(const unsigned int)));
	QObject::connect(m_AddingQunDialog, SIGNAL(refreshGraphic()), SLOT(requestGraphic()));
	QObject::connect(m_AddingQunDialog, SIGNAL(requestAdd()), SLOT(slotFinalAdd()));
	QObject::connect(m_AddingQunDialog, SIGNAL(requestCancel()), SLOT(addingFinished()));

	m_Http = new EvaHttp();
	QObject::connect(m_Http, SIGNAL(requestFinished(bool)), SLOT(slotGraphicDone(bool)));
}

EvaAddingManager::~EvaAddingManager( )
{
	if(m_AddingDialog){
		delete m_AddingDialog;
	}
	if(m_AddingQunDialog){
		delete m_AddingQunDialog;
	}
	delete m_Http;
}

void EvaAddingManager::setPacketManager( EvaPacketManager * packetManager )
{
	m_PacketManager = packetManager;
	if(!packetManager){
		kdDebug() << "[EvaAddingManager] "<< " NULL pointer for packet manager" << endl;
		return;
	}
	m_ID = 0;
	m_Qun = QunInfo();
	// connect adding buddy signals
	QObject::connect(m_PacketManager, SIGNAL(addBuddyExReply(const unsigned int, const unsigned char, const unsigned char)),
			SLOT(slotAddBuddyExReply(const unsigned int, const unsigned char, const unsigned char)));

	QObject::connect(m_PacketManager, SIGNAL(requestAuthInfoReply(const unsigned char, const unsigned char,
				const unsigned char *, const int)),
			SLOT(slotAuthInfoReply(const unsigned char, const unsigned char,
				const unsigned char *, const int)));

	QObject::connect(m_PacketManager, SIGNAL(requestQuestionReply(const unsigned char, const unsigned char,
				const unsigned char *, const int)),
			SLOT(slotRequestQuestionReply(const unsigned char, const unsigned char,
				const unsigned char *, const int)));

	QObject::connect(m_PacketManager, SIGNAL(addFriendAuthExReply(const unsigned int, const unsigned char, const bool)),
			SLOT(slotAddFriendAuthExReply(const unsigned int, const unsigned char, const bool)) );

	// connect Qun joinings signals
	QObject::connect(m_PacketManager, SIGNAL(joinQunReply(const unsigned int, const unsigned char, const QString &)),
			SLOT(slotJoinQunReply(const unsigned int, const unsigned char, const QString &)));
	QObject::connect(m_PacketManager, SIGNAL(joinQunAuthReply(const unsigned int, const unsigned char)),
			SLOT(slotJoinQunAuthReply(const unsigned int, const unsigned char)));
	
//TODO: we should setup a info manager to handle all details request including Qun
	// connect packet manager with adding window
	//QObject::connect(m_AddingDialog, SIGNAL(requestDetails(const unsigned int)), m_PacketManager, SLOT(doGetUserInfo(const unsigned int)));

	// connect packet manager with add qun window
// 	QObject::connect(m_AddingQunDialog, SIGNAL(requestDetails(const unsigned int)),
// 			m_PacketManager, SLOT(doGetUserInfo(const unsigned int)));
// 	QObject::connect(m_AddingQunDialog, SIGNAL(requestQunDetails(const unsigned int)),
// 			m_PacketManager, SLOT(doRequestQunInfo(const unsigned int)));
	
	kdDebug() << "[EvaAddingManager] "<< " connected with packet manager" << endl;
}

void EvaAddingManager::slotAddBuddy(const unsigned int id, const QString nick, const unsigned short face)
{
	if(m_ID) {
		KMessageBox::information(0, i18n("Adding friend/Qun in progress\nonly adding one at a time is allowed, please try later."),
			i18n("Eva Search/Add Friend"));
		return;
	}
	m_IsAddingQun = false;

	m_ID = id;
	m_Nick = nick;
	m_Face = face;
	m_AuthType = AUTH_TYPE_INITIAL; // initial case
	m_IsGraphic = false;
	m_IsInLastProcess = false;

	m_AddingDialog->setBuddy(id, nick, face);
	//m_AddingDialog->setAuthInfo(1);
	m_AddingDialog->show();
	if(m_PacketManager){
		m_PacketManager->doAddBuddyEx(id);
	}
}

void EvaAddingManager::slotAddBuddy(const unsigned int id)
{
	slotAddBuddy(id, QString::number(id), 0);
}

void EvaAddingManager::addingFinished( )
{
	printf("AddingFinished\n");
	m_ID = 0;
	m_IsGraphic = false;
	m_IsGraphicVerified = false;
	m_IsQuestVerified = false;
	m_IsInLastProcess = false;
	if(m_IsAddingQun){
		m_AddingQunDialog->close();
		m_Qun = QunInfo();
	} else {
		m_AddingDialog->close();
	}
}

void EvaAddingManager::slotAddBuddyExReply( const unsigned int id, const unsigned char reply, const unsigned char auth )
{
	if(id !=  m_ID) {
		KMessageBox::information(0, QString(i18n("You are adding %1, cannot processing %2 request.")).arg(m_ID).arg(id),
			i18n("Eva Search/Add Friend"));
		return;
	}
	switch(reply){
		case QQ_ADD_FRIEND_EX_ADDING_POSSIBLE://the user can be added to your buddy list
			break;
		case QQ_ADD_FRIEND_EX_ALREADY_IN_LIST://the user is already in your buddy list
			addingFinished();
			return;
			break; // never get on this line :)
		default:
			addingFinished();
			break;
	}

	m_AuthType = auth;
	switch(m_AuthType){
		case QQ_AUTH_NO_AUTH:
			printf("slotAddBuddyExReply: anyone\n");
		case QQ_AUTH_NEED_AUTH:
			requestAuthInfo();
			printf("slotAddBuddyExReply: need auth\n");
			break;
		case QQ_AUTH_NO_ADD:
			rejectedAdding();
			printf("slotAddBuddyExReply: need rejected\n");
			break;
		case QQ_AUTH_HAS_QUESTION:
			requestQuestion();
			printf("slotAddBuddyExReply: need question\n");
			break;
		default:
			break;
	}
}

void EvaAddingManager::requestAuthInfo( const bool hasGraphic )
{
	unsigned id;
	QString code;
	if(m_IsAddingQun){
		id = m_Qun.getExtID();
		code = m_AddingQunDialog->leCode->text();
		printf("code entered: %s\n", code.local8Bit().data());
	} else {
		id = m_ID;
		code = m_AddingDialog->leCode->text();
	}

	if(hasGraphic){
		m_PacketManager->doRequestAuthInfo(id, AUTH_INFO_CMD_CODE, code, m_Session, m_IsAddingQun);
	} else {
		m_PacketManager->doRequestAuthInfo(id, AUTH_INFO_CMD_INFO, "", "", m_IsAddingQun);
	}
}

void EvaAddingManager::slotAuthInfoReply( const unsigned char cmd, const unsigned char reply, const unsigned char * code, const int len )
{
	if(cmd == AUTH_INFO_CMD_INFO){
		printf(" auth info cmd info reply\n");
	}
	if(cmd == AUTH_INFO_CMD_CODE){
		printf(" auth info cmd code reply\n");
		if(reply == 0x01){
			KMessageBox::information(0, QString(i18n("Incorrect verify code, try again please!")),
				i18n("Eva Search/Add Friend"));
			m_AddingDialog->leCode->setFocus();
			return;
		}
	}
	QByteArray array;
	array.setRawData( (const char *)(code), len);
	m_AuthInfo.duplicate( array);
	printf("len: %2x, auth len: %2x\n", len, m_AuthInfo.size());
	switch(reply){
		case AUTH_INFO_TYPE_CODE:
			if(m_IsInLastProcess){
				m_IsGraphicVerified = true;
				printf("last auth code(graphic) got\n");
				slotFinalAdd();
			}else{
				m_IsGraphic = false;
				authInfoReady();
			}
			break;
		case AUTH_INFO_TYPE_GRAPHIC:
			if(m_IsInLastProcess){
				m_IsGraphicVerified = true;
				printf("last auth code(graphic) got\n");
				slotFinalAdd();
			}else{
				m_IsGraphic = true;
				printf("graphic: %s\n", m_AuthInfo.data());
				requestGraphic();
			}
			break;
		default:
			KMessageBox::information(0, QString(i18n("Unkown auth reply type(%1), adding failed.")).arg(reply),
				i18n("Eva Search/Add Friend"));
			addingFinished();
			return;
	}
}

void EvaAddingManager::rejectedAdding( )
{
	m_AddingDialog->AddingRejected();
}

void EvaAddingManager::requestQuestion( const bool isQuestion, const QString &answer)
{
	m_IsQuestVerified = false;
	m_PacketManager->doRequestAuthQuestion(m_ID, isQuestion, answer); // ask for the question
}

void EvaAddingManager::slotRequestQuestionReply( const unsigned char auth, const unsigned char reply, const unsigned char * code, const int len )
{
	QByteArray array;
	array.setRawData( (const char *)(code), len);
	m_QuestInfo.duplicate( array);

	if(auth == AUTH_TYPE_QUESTION_REQUEST){
		if(reply == 0x01){
			kdDebug() << "[EvaAddingManager] user question set as empty"<< endl;
			///FIXME: if user doesn't set the answer(or an empty answer,
			///  we will get here, so how will we do?
		}
		requestAuthInfo();
	}
	if(auth == AUTH_TYPE_QUESTION_ANSWER){
		if(reply == 0x01){
			KMessageBox::information(0, QString(i18n("Incorrect answer, try again please!")),
				i18n("Eva Search/Add Friend"));
			m_AddingDialog->leAnswer->setFocus();
			return;
		}
		m_IsQuestVerified = true;
		slotFinalAdd();
	}
}

void EvaAddingManager::authInfoReady( )
{
	printf("auth info ready\n");
	if(m_AuthType == AUTH_TYPE_INITIAL){
		addingFinished();
		return;
	}
	printf("we will try\n");
	if(m_IsGraphic){
		printf("update graphic!!!!\n");
		QString graphic = EvaMain::user->getSetting()->getPictureCacheDir() + "/" + AUTH_GRAPHIC_FILE;
		QPixmap pix(graphic);
		if(m_IsAddingQun)
			m_AddingQunDialog->updateGraphic(pix);
		else
			m_AddingDialog->updateGraphic(pix);
	}

	if(m_IsAddingQun){
		printf("heehe\n");
		switch(m_AuthType){
			case QQ_QUN_NO_AUTH:{
				///FIXME: never seen such condition fired. Even the setting is allowing
				/// joining for anyone, the server still return Need Auth. strange
				m_AddingQunDialog->AddingNoAuthReady();
				}
				break;
			case QQ_QUN_JOIN_NEED_AUTH:
				m_AddingQunDialog->AddingNeedAuth();
				break;
			case QQ_QUN_JOIN_DENIED:
				m_AddingQunDialog->AddingRejected();
				break;
			default:
				printf("QQ_QUN_CMD_JOIN_QUN -- unknown join reply type:%2x\n", m_AuthType);
		}
	} else {
		switch(m_AuthType){
			case QQ_AUTH_NO_AUTH:
				m_AddingDialog->AddingNoAuthReady();
				// do nothing but waiting for user clicking
				break;
			case QQ_AUTH_NEED_AUTH:
				m_AddingDialog->AddingNeedAuth();
				break;
			case QQ_AUTH_NO_ADD:
				rejectedAdding();
				break;
			case QQ_AUTH_HAS_QUESTION:{
				QTextCodec *codec = QTextCodec::codecForName("GB18030");
				m_AddingDialog->setQuestion(codec->toUnicode(m_QuestInfo.data()));
				}
				break;
			default:
				break;
		}
	}
}

void EvaAddingManager::requestGraphic( )
{
	m_IsGraphicVerified = false;

	QString path = EvaMain::user->getSetting()->getPictureCacheDir();
	if(path == "") return;
	EvaUser *user = EvaMain::user;
	if(!user) return;
	
	m_GraphicFile.setName(path +"/" + AUTH_GRAPHIC_FILE );
	if(m_GraphicFile.exists()){
		m_GraphicFile.remove();
	}
	
	if ( !m_GraphicFile.open( IO_WriteOnly ) ) {
		kdDebug() << "[EvaAddingManager] cannot create the image" << endl;
		return;
	}

	EvaSetting *sysSetting = EvaMain::global->getEvaSetting();
	if(sysSetting->getConnectType(user->getQQ()) == 2){
		m_Http->setProxyServer( QHostAddress(sysSetting->getServer( user->getQQ())).toString(), 
					sysSetting->getPort( user->getQQ() ));
		m_Http->setBase64AuthParam( sysSetting->getProxyParam( user->getQQ()));
	}

	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	QString url = codec->toUnicode(m_AuthInfo.data());
	m_Http->get( url, &m_GraphicFile  );
}

void EvaAddingManager::slotGraphicDone( bool error )
{
	if(m_GraphicFile.isOpen()){
		m_GraphicFile.close();
	}
	if(error){
		kdDebug() << "[EvaAddingManager] get auth graphic file error" << endl;
		addingFinished();
		return;
	}
	printf("graphic done!\n");
	m_Session = m_Http->getHeader().getCookie("getqqsession");

//	if(m_AuthType == QQ_AUTH_NEED_AUTH)
	authInfoReady();
}

/// now should do some checking when user click add button
void EvaAddingManager::slotFinalAdd( )
{
	if(m_AuthType == AUTH_TYPE_INITIAL){
		addingFinished();
		return;
	}

	m_IsInLastProcess = true;

	if(m_IsAddingQun){
		switch(m_AuthType){
			case QQ_QUN_NO_AUTH:
				//authAddEx();
				//break;
			case QQ_QUN_JOIN_NEED_AUTH:
				if(m_IsGraphic){
					if(m_IsGraphicVerified){
						qunAuthAdd();
					}else{
						requestAuthInfo(true);
					}
				} else
					qunAuthAdd();
				break;
			case QQ_QUN_JOIN_DENIED:
				addingFinished();
				break;
			default:
				break;
		}
	} else {
		switch(m_AuthType){
			case QQ_AUTH_NO_AUTH:
				//authAddEx();
				//break;
			case QQ_AUTH_NEED_AUTH:
				if(m_IsGraphic){
					if(m_IsGraphicVerified){
						authAddEx();
					}else{
						requestAuthInfo(true);
					}
				} else
					authAddEx();
				break;
			case QQ_AUTH_NO_ADD:
				rejectedAdding();
				break;
			case QQ_AUTH_HAS_QUESTION:{
				if(m_IsGraphic){
					if(m_IsGraphicVerified){ // we got the graphic code
						if(m_IsQuestVerified) // we got the question code
							authAddEx();
						else 
							requestQuestion(false, m_AddingDialog->leAnswer->text());
						
					}else{
						requestAuthInfo(true);
					}
				} else {
					// don't need graphic verification, only verifying question needed
					if(m_IsQuestVerified) // we got the question code
						authAddEx();
					else 
						requestQuestion(false, m_AddingDialog->leAnswer->text());
					}
					
				}
				break;
			default:
				break;
		}
	}
}

void EvaAddingManager::authAddEx( )
{
	unsigned char auth;
	switch(m_AuthType){
		case QQ_AUTH_NO_AUTH:
			auth = ADDING_AUTH_TYPE_ANYONE;
			break;
		case QQ_AUTH_NEED_AUTH:
			auth = ADDING_AUTH_TYPE_AUTH;
			break;
		case QQ_AUTH_NO_ADD
			:auth = 0x02;// impossible to get here
			break;
		case QQ_AUTH_HAS_QUESTION:
			auth = ADDING_AUTH_TYPE_COMPOUND;
			break;
		default:
			break;
	}
	// you do want to add the buddy, so make sure she/he is
	// not in the rejectforever list
	EvaMain::user->getSetting()->removeFromRejectForever(m_ID);
	EvaMain::user->getSetting()->saveSettings();

	m_PacketManager->doAddBuddyAuthEx(m_ID, auth, m_AddingDialog->cbbGroups->currentItem(),
				(const unsigned char *)(m_AuthInfo.data()), m_AuthInfo.size(),
				(const unsigned char *)(m_QuestInfo.data()), m_QuestInfo.size(),
				m_AddingDialog->leMessage->text(), m_AddingDialog->chbReverse->isChecked());
}

void EvaAddingManager::slotAddFriendAuthExReply( const unsigned int id, const unsigned char auth, const bool ok )
{
	// NOTE: id might not be the same to m_ID, as EvaSysMsgManager uses:
	//        acceptAddingRequest  and
	//        rejectAddingRequest 
	//        methods
	if(ok ){
		if( m_AuthType == QQ_AUTH_NO_AUTH || m_AuthType == QQ_AUTH_HAS_QUESTION){
			QTextCodec *codec = QTextCodec::codecForName("GB18030");
			QQFriend frd(m_ID, m_Face);
			frd.setNick( std::string(codec->fromUnicode(m_Nick).data()));
			frd.setGroupIndex( m_AddingDialog->cbbGroups->currentItem());
			EvaMain::user->getFriendList().addFriend(frd);
			emit buddyAdded(m_ID, m_Nick, m_Face, m_AddingDialog->cbbGroups->currentItem());
		} 
		if( m_AuthType == QQ_AUTH_NEED_AUTH){
			printf("save buddy to the cache file\n");
			EvaMain::user->getSetting()->saveToBeAddedBuddy(
					BuddyInfoCacheItem(m_ID, m_Nick, m_Face, m_AddingDialog->cbbGroups->currentItem()));
		}
	}

	// let eva system notification window knows the results
	switch(auth){
		case ADDING_AUTH_TYPE_ACCEPT:
		case ADDING_AUTH_TYPE_REJECT:
		case ADDING_AUTH_TYPE_ACCEPT_ADD:
			emit addAuthActionReply(id, auth, ok);
			break;
		default:
			if(!ok)
				KMessageBox::information(0, QString(i18n("Adding %1(%2) failed.")).arg(m_Nick).arg(m_ID),
							i18n("Eva Search/Add Friend"));
			addingFinished();
			break;
	}
}

void EvaAddingManager::acceptAndAdd( const unsigned int id, const QString &nick, const unsigned short face )
{
	m_ID = id;
	m_Nick = nick;
	m_Face = face;
	m_AuthType = AUTH_TYPE_INITIAL; // initial case
	m_IsGraphic = false;
	m_IsInLastProcess = false;

	m_AddingDialog->setBuddy(id, nick, face, true);
	
	m_AddingDialog->show();
	
}

void EvaAddingManager::slotAccepAndAdd( const int index)
{
	m_PacketManager->doAddBuddyAuthEx(m_ID, ADDING_AUTH_TYPE_ACCEPT_ADD);
}

void EvaAddingManager::slotAddQun( const QunInfo & info )
{
	if(m_Qun.getQunID() || m_ID ) {
		KMessageBox::information(0, i18n("Adding friend/Qun in progress\nonly adding one at a time is allowed, please try later."),
			i18n("Eva Search/Add Friend"));
		return;
	}
	m_IsAddingQun = true;
	m_Qun = info;
	
	m_AuthType = AUTH_TYPE_INITIAL; // initial case
	m_IsGraphic = false;
	m_IsInLastProcess = false;

	m_AddingQunDialog->setQun(m_Qun);
	m_AddingQunDialog->show();
	if(m_PacketManager){
		m_PacketManager->doJoinQun(m_Qun.getQunID());
	}
}

void EvaAddingManager::slotJoinQunReply( const unsigned int id, const unsigned char authType, const QString & message )
{
	m_AuthType = authType;
	switch(m_AuthType){
	case QQ_QUN_NO_AUTH:{
		printf("qun no auth\n");
		///FIXME: never seen such condition fired. Even the setting is allowing
		/// joining for anyone, the server still return Need Auth. strange
		m_AddingQunDialog->AddingNoAuthReady();
		}
		break;
	case QQ_QUN_JOIN_NEED_AUTH:
		printf("qun need auth\n");
		/// should retrieving information
		requestAuthInfo();
		break;
	case QQ_QUN_JOIN_DENIED:
		printf("qun denied\n");
		m_AddingQunDialog->AddingRejected();
		break;
	default:
		printf("QQ_QUN_CMD_JOIN_QUN -- unknown join reply type:%2x\n", authType);
		printf("\t--msg:%s\n",message.local8Bit().data());
	}
}

void EvaAddingManager::qunAuthAdd( )
{
	printf("m_AuthInfo.size: %d\n", m_AuthInfo.size());
	m_PacketManager->doJoinQunAuth(m_Qun.getQunID(), (const unsigned char *)(m_AuthInfo.data()), m_AuthInfo.size(),
					m_AddingQunDialog->leMessage->text());
}

void EvaAddingManager::slotJoinQunAuthReply(const unsigned int id, const unsigned char reply)
{
// 	if(id != m_Qun.getQunID()){
// 		kdDebug() << "[EvaAddingManager] received join qun auth reply("
// 			<< id << "), but different from we are adding(" << m_Qun.getQunID() << ")" << endl;
// 		addingFinished();
// 	}
	if(reply == 0x01){	
		KMessageBox::information(0, i18n("Qun operation failed."),
			i18n("Eva Search/Add Friend"));
	}
	emit joinQunAuthReply(id, reply);
	addingFinished();
}

