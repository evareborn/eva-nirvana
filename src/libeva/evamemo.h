/***************************************************************************
 *   Copyright (C) 2005 by casper                                          *
 *   tlmcasper@163.com                                                     *
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
#ifndef LIBEVAMEMO_H
#define LIBEVAMEMO_H

#include "evapacket.h" 
#include "evadefines.h"
#include <string>
#include <vector>

typedef std::vector<std::string> stringList;

typedef struct MemoItem{
	std::string address;
	std::string telephone;
	std::string email;
	std::string mobile;
	std::string name;
	std::string note;
	std::string zipcode;
}MemoItem;

class EvaMemoPacket : public OutPacket
{
public:
	EvaMemoPacket(){}
	EvaMemoPacket( const int id, const unsigned char type);
	EvaMemoPacket( const EvaMemoPacket &rhs );
	virtual ~EvaMemoPacket(){};
 
	EvaMemoPacket &operator=( const EvaMemoPacket &rhs );
 
	void setType( const unsigned char type ) { m_Type = type; }
	void setMemo( const MemoItem &memo ) { m_Memo = memo; }
	void setQQ( const int qq ) { m_Id = qq; } 
	
	const unsigned char  getType() const { return m_Type; }
	const MemoItem getMemo() const { return m_Memo; }
	const int getQQ() const { return m_Id; }
	
	void setDetails( const MemoItem &memo);
	void setDetails( const stringList infos ) { m_Infos = infos; }
	const stringList &getDetails() const { return m_Infos; }
 
protected:
	virtual int putBody( unsigned char *buf );
private:
	int m_Id; 
	unsigned char m_Type;
	MemoItem m_Memo;
	stringList m_Infos;
};

class EvaMemoReplyPacket : public InPacket
{
public:
	EvaMemoReplyPacket(){}
	EvaMemoReplyPacket( unsigned char *buf, int len );
	EvaMemoReplyPacket( const EvaMemoReplyPacket &rhs );
	virtual ~EvaMemoReplyPacket(){}
	
	EvaMemoReplyPacket &operator=( const EvaMemoReplyPacket &rhs );
	
	unsigned char getType() const { return m_Type; }
	const unsigned char getReplyCode() const { return m_ReplyCode; }
	const MemoItem &getMemo() const { return m_Memo; }
	const int getQQ() const { return m_Id; }
	const stringList &getDetails() const { return m_Infos; }
	
	void setType( const unsigned char type ) { m_Type = type; }
	void setReplyCode( const unsigned char replyCode ) { m_ReplyCode = replyCode; }
	void setQQ( const int qq ) { m_Id = qq; }
	void setMemo( const MemoItem &memo ) { m_Memo = memo; }
	void setMemo( const stringList &infos);
	void setDetails( const stringList &infos ) { m_Infos = infos; }
	
	const bool isChangeAccepted() const { return m_ReplyCode == 0x00; }
	
protected:
	virtual void parseBody();
private:
	unsigned char m_Type;
	unsigned char m_ReplyCode;
	MemoItem m_Memo;
	int m_Id;
	stringList m_Infos;
};

#endif
