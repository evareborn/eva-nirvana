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

#ifndef LIBEVALEVEL_H
#define LIBEVALEVEL_H

#include "evapacket.h" 
#include <list>

class EvaGetLevelPacket : public OutPacket {
public:
	EvaGetLevelPacket() {}
	EvaGetLevelPacket(const std::list<unsigned int> &l);
	EvaGetLevelPacket(const EvaGetLevelPacket &rhs);
	virtual ~EvaGetLevelPacket() {};
	
	EvaGetLevelPacket &operator=(const EvaGetLevelPacket &rhs);
	
	const std::list<unsigned int> &getList() const { return friends; }
	void setList(const std::list<unsigned int> &l) { friends = l; }
	
protected:
	virtual int putBody(unsigned char *buf);
private:
	std::list<unsigned int> friends;
};

typedef struct LevelItem{
	unsigned int qqNum;
	unsigned int onlineTime; // note: the time is presented in seconds
	unsigned short level;
	unsigned short timeRemainder; // note: the time is presented in hours
} LevelUserItem;

class EvaGetLevelReplyPacket: public InPacket 
{
public: 
	EvaGetLevelReplyPacket() {}
	EvaGetLevelReplyPacket(unsigned char *buf, int len);
	EvaGetLevelReplyPacket(const EvaGetLevelReplyPacket &rhs);
	virtual ~EvaGetLevelReplyPacket(){};
 
	const unsigned char getReplyCode() const { return replyCode; }
	const std::list<LevelUserItem> &getLevelList() const { return friends; }
	
	EvaGetLevelReplyPacket &operator=(const EvaGetLevelReplyPacket &rhs);
protected:
	virtual void parseBody() ;
private:
	unsigned char replyCode;
	std::list<LevelUserItem> friends;
};

#endif

