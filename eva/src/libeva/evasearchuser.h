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
#ifndef EVASEARCHUSER_H
#define EVASEARCHUSER_H

#include "evapacket.h"
#include "evadefines.h"
#ifndef WIN32
#include <inttypes.h>
#endif
#include <string>
#include <list>
#include <cstdlib>
/**
@author yunfan
*/

class OnlineUser{
public:
	OnlineUser();
	~OnlineUser() {}
	
	const int getQQ() const { if (qqNum!=0) return qqNum;else return atoi(nick.c_str()); }
	void setQQ(const int id){ qqNum = id; }
	const std::string getNick() const { if (qqNum!=0) return nick;else return province;}
	const short getFace() const { return face; }
	const std::string getProvince() const { if (qqNum!=0) return province;else return "";}
	int readData(unsigned char * buf);
	OnlineUser &operator=(const OnlineUser &rhs);
private:
	int qqNum;
	std::string nick;
	std::string province;
	short face;
};

class SearchUserPacket : public OutPacket
{
public:
	SearchUserPacket();
	SearchUserPacket(SearchUserPacket &rhs);
	virtual ~SearchUserPacket() {};
	
	
	void setPage(const int p);
	void setPage(const std::string p) { page = p; }
	const int getIntPage() const { return atoi(page.c_str()); }
	const std::string getPage() const { return page; }
	void setSearchType(const uint8_t type) { searchType = type; }
	const uint8_t getSearchType() const { return searchType; }
	void setMatchEntireString(const bool match) {  matchEntireString = match; }
	const bool getMatchEntireString() const { return matchEntireString; }
	void setNick( const std::string &nickname ) { nick = nickname; }
	const std::string getNick() const { return nick; }
	void setQQ(const int qqNum);
	void setQQ(const std::string &id) { qqStr = id; }
	const int getIntQQ() const { return atoi(qqStr.c_str()); }
	const std::string getQQ() const { return qqStr; }
	void setEmail(const std::string &mail) { email = mail; }
	const std::string getEmail() const { return email; }
	
	SearchUserPacket &operator=(const SearchUserPacket &rhs);
	OutPacket *copy(){ return new SearchUserPacket(*this);}	
protected:		
	virtual int putBody(unsigned char* buf);
private:
	uint8_t searchType;
	std::string page;
	std::string qqStr;
	std::string nick;
	std::string email;
	bool matchEntireString;
};

class SearchUserReplyPacket : public InPacket
{
public:
	SearchUserReplyPacket() {}
	SearchUserReplyPacket(unsigned char* buf, int len);
	SearchUserReplyPacket(const SearchUserReplyPacket &rhs); 
	~SearchUserReplyPacket() {};
		
	const std::list<OnlineUser> getUsers() const { return users; }
	const bool isFinished() const { return finished; }
	//void setFinished(const bool f) { finished = f; }
	
	SearchUserReplyPacket &operator=(const SearchUserReplyPacket &rhs);
protected:		
	virtual void parseBody();
private:
	std::list<OnlineUser> users;
	bool finished;
};

#endif
