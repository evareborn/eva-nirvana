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
 
#ifndef EVACONTACTMANAGER_H
#define EVACONTACTMANAGER_H

#include <list>
#include <string>
	
#include "evafriendlist.h"
#include "evaqtutil.h"


enum Status{
	ESCM_NONE,            //!< no status
	ESCM_SINGLEOP,        //!< single operations
	ESCM_CONTACTLIST,     //!< when \c fetchBuddyList was called
	ESCM_GROUPNAMES,      //!< when \c fetchGroupNames was called
	ESCM_GROUPEDBUDDIES,  //!< when \c fetchGroupedFriends was called
	ESCM_QUNLIST,         //!< when \c fetchQunList was called
	ESCM_QUNDETAILS,      //!< when \c fetchQunDetails was called
	ESCM_QUNMEMBERS,      //!< when \c fetchQunDetails was called
	ESCM_CONTACTDETAIL,   //!< when \c fetchDetail was called
	ESCM_SIGNATURE,       //!< when \c fetchSignature was called
	ESCM_ALLSIGNATURES,   //!< when \c fetchAllSignatures was called
	ESCM_LEVEL,           //!< when \c fetchLevel was called
	ESCM_ALLLEVELS,       //!< when \c fetchAllLevel was called
};

class EvaPacketManager;
class GetFriendListReplyPacket;
class GroupNameOpReplyPacket;
class DownloadGroupFriendReplyPacket;
class QunReplyPacket;
class GetUserInfoReplyPacket;
class SignatureReplyPacket;
class EvaGetLevelReplyPacket;

/**
 * A helper class to get user contact list and
 * Qun list.
 * 
 * 
 */

class EvaContactManager// : public QObject
{
public:
	EvaContactManager();
	virtual ~EvaContactManager();
	
	void setPacketManager(EvaPacketManager *pm);
	
	void fetchContacts();//DONE
	void fetchGroupNames();//DONE
	void fetchGroupedFriends();//DONE
	void fetchQunList();//!< same as above method
	
	void fetchDetail     (const unsigned int id); //DONE, NOTE: this method updates EvaUser's friendlist directly
	void fetchSignature  (const unsigned int id); //DONE
	void fetchAllSignatures();                    //DONE
	void fetchLevel      (const unsigned int id); //DONE
	void fetchAllLevels();                        //DONE, NOTE: this method has no corresponding event type, 
                                                      //            update EvaUser's friendlist directly
	void fetchAddQuestion(const unsigned int id); //DONE
	
	void downloadAll();
	//void fetchDetails    (unsigned int id);
	
	void fetchQunDetails    (const unsigned int id); //DONE
	void fetchQunMembersInfo(const unsigned int id, bool isFirst = true); //DONE, NOTE:this method updates EvaUser's Qun Member directly
	
	//once contacts ready, call this method to get the downloaded list 
	inline const FriendList &getContacts();
	
	inline const QunList &getQunList();
	
	//once group names ready, call this method to get the group names
	inline const std::list<std::string> &getGroupNames();
	
	inline const std::map<unsigned int, int> &getGroupedContacts();
	//inline const std::list<unsigned int> &getQunIDList();
	
	inline const QunInfo &getQunInfo();
	inline const std::map<unsigned int, QunMember> &getQunMembers();

	inline const ContactInfo &getContactInfo();
	inline const bool IsContactsReady();
private:
	bool m_downloadAll;
	bool m_contactsReady;
	Status m_status;
	EvaPacketManager *m_packetManager;
	
	FriendList m_Contacts; //!< used for downloading contact list
	
	std::list<std::string> m_GroupNames; //!< used for downloading group names
	
	std::map<unsigned int, int> m_GroupedContacts; //!< used for downloading grouped contacts(including Qun internal ID)
	//std::list<unsigned int> m_QunIDs;  //!< used for downloading grouped contacts(including Qun internal ID)
	QunList m_QunList; //!< used for recording downloaded Qun relative stuff
	
	QunInfo m_QunInfo; //!< used for downloading Qun information
	std::map<unsigned int, QunMember> m_QunMembers; //!< used for downloading Qun information
	
	int m_QunMemberCount; //!< used for counting Qun members

	ContactInfo m_ContactInfo; //!< used for fetching contact information
	
	void notifyEvent(int eId, const QString &msg = QString::null, EPARAM param = 0);
	
	// we should ensure the word consistency used in Eva
	// later on, we will change FriendList to Contacts or ContactList
	void processGetFriendListReply(const GetFriendListReplyPacket *packet);
	void processDownloadGroupName(const GroupNameOpReplyPacket *packet);
	void processDownloadGroupFriendReply(const DownloadGroupFriendReplyPacket *packet);
	void processQunInfoReply(const QunReplyPacket *packet);
	void processQunMemberReply(const QunReplyPacket *packet);
	void processGetUserInfoReply( const GetUserInfoReplyPacket * packet );
	void processSignatureReply( const SignatureReplyPacket *packet);
	void processGetLevelReply( const EvaGetLevelReplyPacket *packet);
	
	friend class EvaPacketManager;
};

inline const FriendList &EvaContactManager::getContacts() { return m_Contacts; }
inline const std::list<std::string> &EvaContactManager::getGroupNames() { return m_GroupNames; }
inline const std::map<unsigned int, int> &EvaContactManager::getGroupedContacts() { return m_GroupedContacts; }
//inline const std::list<unsigned int> &EvaContactManager::getQunIDList() { return m_QunIDs; }
inline const QunInfo &EvaContactManager::getQunInfo() { return m_QunInfo; }
inline const std::map<unsigned int, QunMember> &EvaContactManager::getQunMembers() { return m_QunMembers; }
inline const ContactInfo &EvaContactManager::getContactInfo() { return m_ContactInfo; }
inline const bool EvaContactManager::IsContactsReady() { return m_contactsReady; }
inline const QunList &EvaContactManager::getQunList() { return m_QunList; }

EvaContactManager *GetContactManager();
		
#endif // EVACONTACTMANAGER_H
