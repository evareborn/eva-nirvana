/***************************************************************************
 *   Copyright (C) 2004 by yunfan                                          *
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
#ifndef EVAPACKETMANAGER_H 
#define EVAPACKETMANAGER_H

#include "libeva.h"
#include "evauser.h"
#include <map>
#include <list>
#include <qobject.h>
#include <qdatetime.h>
#include <qstringlist.h>
#include <qvaluelist.h>


// passing a connected EvaConnecter in, otherwise whenever sending a packet will cause an exception
class QTimer;
class EvaConnecter;
class QTextCodec;
class EvaLoginManager;
class EvaContactManager;

typedef struct {
	short total;                       //used to show missing part
	std::map<short,QString> content;
} pcMsg;

class EvaPacketManager : public QObject {
	Q_OBJECT
public:
	EvaPacketManager(EvaUser *user, EvaConnecter *connecter);
	~EvaPacketManager();
	
	EvaUser *getUser() { return user; }
	EvaConnecter *getConnecter() { return connecter; }
	
	QStringList convertDetails(const ContactInfo &info);
	
	const int getNumOnlineUsers() const { return numOnlineUsers; }
public slots:
	void doRequestLoginToken();
	void doRequestLoginTokenEx( const QString &code = "");
	void doLogin();
	void doLogout();
	void doGetUserInfo(const unsigned int id);
	void doModifyDetails(QStringList info, QString oldPwd = QString::null, QString newPwd = QString::null);
	
	void doChangeStatus(EvaUser::UserStatus newStatus);
	void doRequestFileAgentKey();
	void doGetOnlineFriends();
	void doGetAllFriends();
	
	void doGetAllLevels();
	void requestFriendLevel(const unsigned int id);
	
	void doSendMessage(const unsigned int receiver, const bool isNormal, QString &message, const char fontSize = 9, 
			const bool u = false, const bool i = false, const bool b = false, 
			const char blue = 0, const char green = 0, const char red = 0);
			
	void doGetGroups();
	void doUploadGroups();
	
	void doDeleteBuddy(const unsigned int id);
	
	void doAddBuddy(const unsigned int id);
	void doAddAuthBuddy(const unsigned int id, const QString &message);
	void doApproveBuddy(const unsigned int id);
	void doRejectBuddy(const unsigned int id, const QString &message);

	void doSearchUsers(const bool isSearchAll = true, const QString page = "0", const QString id ="", 
			const QString nick = "", const QString email = "", const bool match = false);
	
	void doDeleteMeFrom(const unsigned int id);
	
	void doRequestQunInfo(const unsigned int id);
	void doRequestQunMemberInfo(const unsigned int id, const std::list<unsigned int> &l);
	void doRequestQunOnlineMembers(const unsigned int id);
	
	void doSendQunMessage(const unsigned int receiver, QString &message, const char fontSize = 9, 
			const bool u = false, const bool i = false, const bool b = false, 
			const char blue = 0, const char green = 0, const char red = 0);
	void doSearchQun(const unsigned int extID);
	void doJoinQun(const unsigned int qunID);
	void doJoinQunAuth(const unsigned int qunID , const unsigned char *code, const unsigned short len,  const QString &msg);
	
	void doApproveQun(const unsigned int qq, const unsigned int id,
			const unsigned char *token, const unsigned short len);
	void doRejectQun(const unsigned int qq, const unsigned int id, const QString & msg,
			const unsigned char *token, const unsigned short len);
	
	void doModifyQunInfo(const unsigned int id, unsigned char auth, unsigned short cate, QString name, QString notice, QString description);
	void doModifyQunCard(const unsigned int id,  const unsigned int qqID, QString name, unsigned char gender, QString phone, QString email, QString memo);
	void doQunSetAdmin(const unsigned int id, const unsigned int qq, const bool isSetAdmin);
	void doQunTransfer(const unsigned int id, const unsigned int qq);
	void doModifyQunMembers(const unsigned int id, const std::list<unsigned int>members, const bool isAdd);
	void doRequestQunCard( const unsigned int id,  const unsigned int qq);
	void doQunExit(const unsigned int id);
	void doQunCreate(const QString &name, const unsigned short category, const unsigned char auth, const QString &notice, 
			const QString &description, const std::list<unsigned int> &members);
	void doRequestQunRealNames(const unsigned int id);
	
	void doRequestExtraInfo();
	void doModifySignature(const QString signature); // only for user use only
	void doDeleteSignature();   // only for user use only
	bool doRequestSignature( const unsigned int start = 0, const bool isSingleContact = false);  // for all your buddies
	void doUploadMemo( const unsigned int id, const MemoItem &memo );
	void doRemoveMemo( const unsigned int id);
	void doDownloadMemo( const unsigned int id);
	
	//void lastLoginStep();

	void doSendFileUdpRequest(const unsigned int id, const QString fileName, const unsigned int fileSize,
				const short session, const unsigned char transferType = QQ_TRANSFER_FILE);
	void doAcceptFileRequest(const unsigned int id, const short session, const unsigned int wanIp,
				const unsigned char transferType = QQ_TRANSFER_FILE);
	void doCancelFileRequest(const unsigned int id, const short session, 
				const unsigned char transferType = QQ_TRANSFER_FILE);
	void doNotifyAgentTransfer(const unsigned int id, const unsigned int oldSession, const unsigned int agentSession,
				const unsigned int ip, const unsigned short port,
				const unsigned char transferType = QQ_TRANSFER_FILE);
	void doNotifyIpEx(const unsigned int id, const bool isSender,
			const unsigned int session,const unsigned char transferType,
			const unsigned int wanIp1, const unsigned short wanPort1,
			const unsigned int wanIp2, const unsigned short wanPort2,
			const unsigned int wanIp3, const unsigned short wanPort3,
			const unsigned int lanIp1, const unsigned short lanPort1,
			const unsigned int lanIp2, const unsigned short lanPort2,
			const unsigned int lanIp3, const unsigned short lanPort3,
			const unsigned int syncIp, const unsigned short syncPort,
			const unsigned int syncSession);
	void doAdvancedSearch(const int page = 0, const bool onlineUser = true, const bool hasCam = false, 
				const int province = 0,const int city = 0, const int age = 0, const int gender = 0);
	void doAddBuddyEx(const unsigned int id);
	void doRequestAuthInfo(const unsigned int id, const unsigned char cmd = AUTH_INFO_CMD_INFO,
				const QString &veri="", const QString &session="", const bool isQun = false);
	void doRequestAuthQuestion(const unsigned int id, const bool isQuestion = true, const QString &answer = "");
	void doSetMyAuthQuestion(const QString &quest = "", const QString &answer = "");
	void doRequestMyAuthQuestionSetting();

	void doAddBuddyAuthEx(const unsigned int id, const unsigned char authStatus, const unsigned int destGroup = 0,
					const unsigned char *code = 0, const unsigned short len = 0,
					const unsigned char *qcode = 0, const unsigned short qlen = 0,
					const QString &msg = "", const bool allowReverse = false);
	void doVerifyAddingMessage(const unsigned int id, const unsigned char * code, const unsigned int len);
	void doGetWeatherForecast(const unsigned int ip);

	//new methods used with evacontactmanager
	void doGetContacts(int pos = QQ_FRIEND_LIST_POSITION_START);
signals:
	void networkException(int);
	void packetException(int);
	void serverBusy();
	
	void logoutOK(); 
	void onlineReady();
	void offlineReady();
	void invisibleReady();
	void leaveReady();
	void kickedOut(const QString);
	
	void friendStatusChanged(unsigned int id);
	void friendListReady();
	//void myInfoReady();
	void userInfoReady(QStringList info);
	void extraInfoReady();
	void modifyInfo(bool);
	
	void sentMessageResult(unsigned int ,bool);
	void txtMessage(unsigned int sender, bool isNormal, QString message, QDateTime time, const char fontSize, 
			const bool u, const bool i, const bool b, 
			const char blue, const char green, const char red);
	void sysMessage(int type, QString message);
	
	//void wrongPassword(QString);
	
	//void friendGroupsReady();
	void friendGroupsUploadReady(bool);
		
	void deleteBuddyReady(unsigned int, bool);
	
	void addingBuddy(int);
	void addBuddyReady();
	void addBuddyRejected();
	void addBuddyNeedAuth();
	void addAnonymous(const unsigned int,const unsigned short);

	void addBuddySentToServer(bool);
	
	void searchUsersReady(const bool finished, const std::list<OnlineUser> list);
	void deleteMeReady(bool);
	
	void friendSignatureChanged(const unsigned int, const QDateTime, const QString);
	
	void qunSentMessageResult(unsigned int, bool, QString);
	void qunTxtMessage(unsigned int qunID, unsigned int senderQQ, QString message, QDateTime time, const char fontSize, 
			const bool u, const bool i, const bool b, 
			const char blue, const char green, const char red);
	void qunInfomationReady(const unsigned int, const bool, QString);
	void qunMemberInfoReady(const unsigned int);
	void qunOnlineListReady(const unsigned int);
	void qunSearchReady(const std::list<QunInfo>, QString);
	
	//void qunJoinReplyOK(const unsigned int);
	//void qunJoinReplyNeedAuth(const unsigned int);
	//void qunJoinReplyDenied(const unsigned int);
	void joinQunReply(const unsigned int id, const unsigned char authType, const QString &message);
	// note that this signal only means the server received your request correctly, nothing else
	void joinQunAuthReply(const unsigned int id, const unsigned char reply);

	void qunSystemMessageRequest(const unsigned int, QString);
	void qunJoinEvent(const unsigned int, const short, const unsigned int, const unsigned int); // qunid, im type, sender, commander
	void qunRequestUpdateDisplay();
	void qunModifyInfoReply(const unsigned int, const bool, QString);
	void qunModifyQunCardReply(const unsigned int, const bool, const unsigned int, QString);
	void qunSetAdminReply(const unsigned int , const bool , const unsigned int, const bool , QString);
	void qunTransferReply(const unsigned int , const bool, const unsigned int, QString);
	void qunModifyQunMembersReply(const unsigned int, const bool, QString);
	void qunRequestQunCardReply(const unsigned int, const bool, const unsigned int, QString, const unsigned char, QString, QString, QString, QString);
	void qunExitReply(const unsigned int, const bool, QString);
	void qunCreateDone(const unsigned int);
	void qunCreateFailed(QString);
	void qunActiveQunFailed(QString);
	
	void signatureModifyReply(const bool);
	void signatureDeleteReply(const bool);

	void memoUploadReply( const bool );
	void memoDownloadReply( const MemoItem& );
	void memoNoMemoFound();

	void receivedFileRequest(const unsigned int id, const unsigned int session, const QString fileName, 
				const int size, const unsigned char transferType);
	void receivedFileAccepted(const unsigned int id, const unsigned int session, const unsigned int ip,
				const bool isAccepted, const unsigned char transferType);
	void receivedFileAgentInfo(const unsigned int id, const unsigned int oldSession, const unsigned int newSession, 
				const unsigned int ip, const unsigned short port, const unsigned char *key);
	void receivedFileNotifyIpEx(const unsigned int id, const bool isSender, 
				const unsigned int session,const unsigned char transferType,
				const unsigned int wanIp1, const unsigned short wanPort1,
				const unsigned int wanIp2, const unsigned short wanPort2,
				const unsigned int wanIp3, const unsigned short wanPort3,
				const unsigned int lanIp1, const unsigned short lanPort1,
				const unsigned int lanIp2, const unsigned short lanPort2,
				const unsigned int lanIp3, const unsigned short lanPort3,
				const unsigned int syncIp, const unsigned short syncPort,
				const unsigned int syncSession);
	void advancedSearchReady(const int page, const bool finished, const std::list<AdvancedUser> list);
	void addBuddyExReply(const unsigned int id, const unsigned char reply, const unsigned char auth);
	void requestAuthInfoReply(const unsigned char cmd, const unsigned char replyCode,
				const unsigned char *code, const int codeLen);

	void receivedMyAuthSettings(const unsigned char auth, const QString& question, const QString& anwser);
	void updateAuthSettingResult(const unsigned char auth, const unsigned char replyCode);
	void requestQuestionReply(const unsigned char auth, const unsigned char replyCode,
				const unsigned char *code, const int codeLen);

	void addFriendAuthExReply(const unsigned int id, const unsigned char reply, const bool ok);
	void receivedSysMessage(const unsigned char type, const unsigned int myID, const unsigned int fromID,
				const QString &msg, const bool allowReverse,
				const unsigned char *code, const unsigned int codeLen);
	void receivedVerifyAddingMessageReply(const unsigned int id, const unsigned char reply,
						const unsigned char flag1, const unsigned char flag2);

	void qunSysMessage(const unsigned int id, const unsigned short imType, const unsigned int ext,
			const unsigned char qunType, const unsigned int sender, const unsigned int commander,
			const QString &msg, const unsigned char *code, const unsigned short codeLen,
			const unsigned char *token, const unsigned short tokenLen);
	void receivedQQNews(const QString &title, const QString &brief, const QString &url);
private:

	QTextCodec *codec;
	EvaUser *user;
	EvaConnecter *connecter;
	QTimer *timer; // keep online and get online friends
	std::map<short, int>sendRemoveCacheList; // packet sequence(key), qq id
	int numOnlineUsers;
	int addingBuddyQQ;
	int deletingBuddyQQ;
	int processQunID;
	bool qunMemberInfoFinished;
	ContactInfo changingDetails;
	QValueList<int>receivedPacketList;
	QValueList<int>receivedCacheList;
	
	std::map<short, pcMsg> pcMsgCache;

	// key(replied sequence), next packet
	std::map<int, SendTextIMPacket *> longMsgPacketQueue;
	std::map<int, QunSendIMExPacket *> longMsgQunPacketQueue;

	
	void redirectTo(const unsigned int ip, const short port);
//	void doGetOnlineFriends();
	void parsePacket(InPacket *packet);
	
	void saveBuddyQQ(const unsigned int id, const short seq);
	void removeSequence(const short seq);
	const int getSavedBuddyQQ(const short seq);
	
	void doGetGroupedFriends(int start = 0);
	
	void processRequestLoginTokenReply(const InPacket *in);
	void processRequestLoginTokenExReply(const InPacket *in);
	void processLoginReply(const InPacket *in);
	void processGetUserInfoReply(const InPacket *in);
	void processChangeStatusReply(const InPacket *in);
	void processGetFriendListReply(const InPacket *in);
	void processSendIMReply(const InPacket *in);
	void processGetOnlineFriendReply(const InPacket *in);
	void processReceiveIM(const InPacket *in);
	void processKeepAliveReply(const InPacket *in);
	void processFriendChangeStatus(const InPacket *in);
	void processGroupNameOp(const InPacket *in);
	void processDownloadGroupFriendReply(const InPacket *in);
	void processUploadGroupFriendReply(const InPacket *in);
	void processDeleteFriendReply(const InPacket *in);
	void processAddFriendReply(const InPacket *in);
	void processAddFriendAuthReply(const InPacket *in);
	void processSystemNotification(const InPacket *in);
	void processModifyInfoReply(const InPacket *in);
	void processSearchUserReply(const InPacket *in);
	void processDeleteMeReply(const InPacket *in);
	void processQunReply(const InPacket *in);
	void processGetLevelReply(const InPacket *in);
	void processRequestKeyReply(const InPacket *in);
	void processRequestExtraInfoReply(const InPacket *in);
	void processSignatureReply(const InPacket *in);
	void processMemoReply( const InPacket *in );
	void processAdvancedSearchReply(const InPacket *in);
	void processAddFriendExReply(const InPacket *in);
	void processAddFrendAuthInfoReply(const InPacket *in);
	void processAddFriendAuthExReply(const InPacket *in);
	void processAddFriendAuthQuestionReply(const InPacket *in);
	void processVerifyAddingMsgReply(const InPacket *in);
	void processWeatherOpReply(const InPacket *in);

private slots:
	void slotConnectReady();
	void newPacketSlot();
	void networkExceptionSlot(int);
	void packetExceptionSlot(int);
	void timerSlot();
	void slotClientNotReady();
	
	friend class EvaLoginManager;
	friend class EvaContactManager;
};

#endif 
