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
 
#include "evaqunlist.h"

 


TmpQun::TmpQun( )
	: qunID(0), parentID(0)
{
}

TmpQun::TmpQun( const unsigned int pid )
	: qunID(0), parentID(pid)
{
}

TmpQun &TmpQun::operator=(const TmpQun &rhs)
{
	qunID = rhs.getQunID();
	parentID = rhs.getParentID();
	name = rhs.getName();
	memberList = rhs.getMemberQQList();
	return *this;
}

void TmpQun::addMember( const unsigned int qqNum )
{
	memberList.push_back(qqNum);
}

const bool TmpQun::removeMember( const unsigned int qqNum )
{
	std::list<unsigned int>::iterator iter;
	for(iter=memberList.begin(); iter!=memberList.end(); ++iter){
		if( *iter == qqNum){
			memberList.erase(iter);
			return true;
		}
	}
	return false;
}




/*************************************************************************************************************************************/


Qun::Qun( const unsigned int id )
	: qunID(id), cardName(""), cardGender(0), cardPhone(""), cardEmail(""), cardMemo(""),
	m_RealNamesVersion(0), msgType(Notify), m_ChatFontSize(9), m_ChatFontColor(0)
{
	m_memberStartCount = 0;
	m_bFirstRefresh = true;
}

Qun::Qun(const Qun &rhs)
{
	*this = rhs;
}

Qun &Qun::operator=(const Qun &rhs)
{
	qunID = rhs.getQunID();
	info = rhs.getDetails();
	//numberList = rhs.getQQList();
	memberList = rhs.getMembers();
	memberArgs = rhs.getMemberArgs();
	tmpQunList = rhs.getTmpQunList();
	cardName = rhs.getCardName();
	cardGender = rhs.getCardGender();
	cardPhone = rhs.getCardPhone();
	cardEmail = rhs.getCardEmail();
	cardMemo = rhs.getCardMemo();
	m_RealNamesVersion = rhs.getRealNamesVersion();
	msgType = rhs.getMessageType();

	m_ChatFontSize = rhs.getChatFontSize();
	m_ChatFontColor = rhs.getChatFontColor();

	return *this;
}

void Qun::setMembers(const std::list<FriendItem> &l)
{
	std::list<FriendItem> list = l;
	std::list<FriendItem>::iterator iter;
	std::list<FriendItem>::iterator iter1;
	for(iter = list.begin(); iter!= list.end(); ++iter){
		for(iter1 = memberList.begin(); iter1!= memberList.end(); ++iter1){
			if(iter1->getQQ() == iter->getQQ()){
 				break;
			}
		}
		if(iter1 != memberList.end()){ // already in our list
			iter->setQunRealName(iter1->getQunRealName());
			iter->setQunGroupIndex(iter1->getQunGroupIndex());
			iter->setQunAdminValue(iter1->getQunAdminValue());
			(*iter1) = (*iter);
		}else{ // new guy, just add it
			memberList.push_back(*iter);
		}
	}
}

void Qun::updateOnlineMembers(const std::list<unsigned int> &list)
{
	std::list<unsigned int> onlist = list;
	std::list<FriendItem>::iterator iter;
	for(iter = memberList.begin(); iter!= memberList.end(); ++iter){
		iter->setOnline(false);
		std::list<unsigned int>::iterator iter1;
		for(iter1 = onlist.begin(); iter1 != onlist.end(); ++iter1){
			if(iter->getQQ() == *iter1)
				iter->setOnline(true);
		}
	}
}

const FriendItem *Qun::getMemberDetails( const unsigned int qqNum ) 
{
	std::list<FriendItem>::iterator iter;
	for(iter = memberList.begin(); iter!= memberList.end(); ++iter){
		if(iter->getQQ() == qqNum)
			return &(*iter);
	}
	return NULL;
}

FriendItem *Qun::memberDetails(const unsigned int qqNum)
{
	std::list<FriendItem>::iterator iter;
	for(iter = memberList.begin(); iter!= memberList.end(); ++iter){
		if(iter->getQQ() == qqNum)
			return &(*iter);
	}
	return NULL;
}

void Qun::setMember( const FriendItem & fi )
{
	FriendItem item = fi;
	std::list<FriendItem>::iterator iter;
	for(iter = memberList.begin(); iter!= memberList.end(); ++iter){
		if(iter->getQQ() == item.getQQ()){
			item.setQunGroupIndex(iter->getQunGroupIndex());
			item.setQunAdminValue(iter->getQunAdminValue());
			(*iter) = item;
			break;
		}
	}
	if(iter == memberList.end()){
		std::map<unsigned int, QunMember>::iterator it = memberArgs.find(item.getQQ());
		if( it != memberArgs.end()){
			item.setQunGroupIndex(it->second.groupIndex);
 			item.setQunAdminValue(it->second.admin);
		}
		memberList.push_back(item);
	}
}

void Qun::addTmpQun( const TmpQun & q )
{
	tmpQunList.push_back(q);
}

void Qun::setMemberArgs(const std::map<unsigned int, QunMember> &list)
{
	memberArgs = list;
	m_RealNamesVersion = 0;

	std::map<unsigned int, QunMember>::iterator it;
	std::list<FriendItem>::iterator iter;

	std::map<unsigned int, bool> toRemove; //record those members who are kicked out from this Qun
	for(iter = memberList.begin(); iter != memberList.end(); ++ iter){
		toRemove[iter->getQQ()] = true;
	}
	// try every member in memberArgs, check if already in
	// in our memberList. if it is we'll use the old one
	// otherwise use the memberArgs one
	for(it = memberArgs.begin(); it!=memberArgs.end(); ++it){
		for(iter = memberList.begin(); iter != memberList.end(); ++ iter){
			if(iter->getQQ() == it->first){
				toRemove.erase(iter->getQQ());
				iter->setQunGroupIndex( it->second.groupIndex);
				iter->setQunAdminValue( it->second.admin);
				break;
			}
		}
		if(iter != memberList.end()) continue; // we have deleted it
		
		FriendItem item(it->first);
		item.setQunGroupIndex( it->second.groupIndex);
		item.setQunAdminValue( it->second.admin);
		memberList.push_back(item);
	}
	
	// remove those NOT existed members
	std::map<unsigned int, bool>::iterator remvIt;
	iter = memberList.begin();
	while(iter != memberList.end()){
		remvIt = toRemove.find(iter->getQQ());
		if(remvIt != toRemove.end()){
			memberList.erase(iter);
			toRemove.erase(remvIt);
			iter = memberList.begin();
		} else
			iter++;
	}
}

std::list<unsigned int> Qun::getNextMembers()
{
	std::list<unsigned int> list;
	unsigned int i = 0;
	if(m_memberStartCount == memberList.size() ) return list;
	std::list<FriendItem>::iterator iter;
	for(iter = memberList.begin(); iter!= memberList.end(); ++iter){
		if(i >= m_memberStartCount){
			if(i > m_memberStartCount + 30) break;
			list.push_back(iter->getQQ());
		}
		i++;
	}
	m_memberStartCount += list.size();
	return list;
}

const  bool Qun::isAdmin( const unsigned int id)
{
	std::list<FriendItem>::iterator iter;
	for(iter = memberList.begin(); iter!= memberList.end(); ++iter){
		if(iter->getQQ() == id){
			if(iter->getQunAdminValue() & QUN_TYPE_ADMIN)
				return true;
			break;
		}
	}	
	return false;
}

const bool Qun::hasMember(const unsigned int id)
{
	std::list<FriendItem>::iterator iter;
	for(iter = memberList.begin(); iter!= memberList.end(); ++iter){
		if(iter->getQQ() == id){
			return true;
		}
	}	
	return false;
}

/*************************************************************************************************************************************/

QunList &QunList::operator=(const QunList &rhs)
{
	qunList = rhs.getQunList();
	m_iter = qunList.begin();
	return *this;
}

void QunList::add( const Qun & q )
{
	Qun *qun = getQun(q.getQunID());
	if(!qun)
		qunList.push_back(q);
}

bool QunList::remove( const unsigned int id )
{
	std::list<Qun>::iterator iter;
	for(iter = qunList.begin(); iter!= qunList.end(); ++iter){
		if(iter->getQunID() == id){
			qunList.erase(iter);
			return true;
		}
	}
	return false;
}

void QunList::update( const Qun & q )
{
	if(remove(q.getQunID()))
		add(q);
}

Qun *QunList::getQun( const unsigned int id )
{
	std::list<Qun>::iterator iter;
	for(iter = qunList.begin(); iter!=qunList.end(); ++iter){
		if(iter->getQunID() == id){
			return &(*iter);
		}
	}
	return NULL;
}

Qun * QunList::getQunByExtID( const unsigned int ext )
{
	std::list<Qun>::iterator iter;
	for(iter = qunList.begin(); iter!=qunList.end(); ++iter){
		if(iter->getExtQunID() == ext){
			return &(*iter);
		}
	}
	return NULL;
}

void QunList::setDetails( const QunInfo & info )
{
	std::list<Qun>::iterator iter;
	for(iter = qunList.begin(); iter!= qunList.end(); ++iter){
		if(iter->getQunID() == info.getQunID()){
			iter->setDetails(info);
			return;
		}
	}
}

void QunList::setMember( const unsigned int id, const FriendItem & fi )
{
	std::list<Qun>::iterator iter;
	for(iter = qunList.begin(); iter!= qunList.end(); ++iter){
		if(iter->getQunID() == id){
			iter->setMember(fi);
			return;
		}
	}	
}

void QunList::setMembers( const unsigned int id, const std::list< FriendItem > & list )
{
	std::list<Qun>::iterator iter;
	for(iter = qunList.begin(); iter!= qunList.end(); ++iter){
		if(iter->getQunID() == id){
			iter->setMembers(list);
			return;
		}
	}
}

void QunList::setOnlineMembers(const unsigned int id, const std::list<unsigned int> &list)
{
	std::list<Qun>::iterator iter;
	for(iter = qunList.begin(); iter!= qunList.end(); ++iter){
		if(iter->getQunID() == id){
			iter->updateOnlineMembers(list);
			return;
		}
	}
}

// void QunList::setQQList(const unsigned int id, const std::list<unsigned int> &list)
// {
// 	std::list<Qun>::iterator iter;
// 	for(iter = qunList.begin(); iter!= qunList.end(); ++iter){
// 		if(iter->getQunID() == id){
// 			iter->setQQList(list);
// 			return;
// 		}
// 	}
// }

void QunList::setMemberArgs(const unsigned int id, const std::map<unsigned int, QunMember> &list)
{
	std::list<Qun>::iterator iter;
	for(iter = qunList.begin(); iter!= qunList.end(); ++iter){
		if(iter->getQunID() == id){
			iter->setMemberArgs(list);
			return;
		}
	}
}

void QunList::setMyQunCardInfo( const unsigned int id, const std::string name, const unsigned char gender, 
			const std::string phone, const std::string email, const std::string memo )
{
	std::list<Qun>::iterator iter;
	for(iter = qunList.begin(); iter!= qunList.end(); ++iter){
		if(iter->getQunID() == id){
			iter->setCardName(name);
			iter->setCardGender(gender);
			iter->setCardPhone(phone);
			iter->setCardEmail(email);
			iter->setCardMemo(memo);
			return;
		}
	}	
}

Qun * QunList::first( )
{
	m_iter = qunList.begin();
	if(m_iter == qunList.end())
		return NULL;
	return &(*m_iter);
}

Qun * QunList::next( )
{
	m_iter++;
	if(m_iter == qunList.end())
		return NULL;
	return &(*m_iter);
}

const unsigned int QunList::getQunExtID( const unsigned int id )
{
	Qun *q = getQun(id);
	if(q)
		return q->getExtQunID();
	return 0;
}
