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
#include "evamemo.h"
#include "evautil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>

EvaMemoPacket::EvaMemoPacket( const int id, const unsigned char type )
	:OutPacket( QQ_CMD_MEMO_OP, true ), m_Id( id ), m_Type( type )
{
}

EvaMemoPacket::EvaMemoPacket( const EvaMemoPacket &rhs )
	:OutPacket( rhs )
{
	(*this) = rhs;
}

EvaMemoPacket &EvaMemoPacket::operator=( const EvaMemoPacket & rhs )
{
	*((OutPacket *)this) = (OutPacket)rhs;
	m_Id = rhs.getQQ();
	m_Type = rhs.getType();
	m_Memo = rhs.getMemo();
	return *this;
}

void EvaMemoPacket::setDetails( const MemoItem &memo)
{
	m_Infos.push_back( memo.name );
	m_Infos.push_back( memo.mobile );
	m_Infos.push_back( memo.telephone );
	m_Infos.push_back( memo.address );
	m_Infos.push_back( memo.email );
	m_Infos.push_back( memo.zipcode );
	m_Infos.push_back( memo.note );
}

int EvaMemoPacket::putBody( unsigned char *buf )
{
	int pos = 0;
	int len = 0;
	setDetails(m_Memo);
	switch(m_Type){
	case 0x01:{
		buf[pos++] = QQ_MEMO_UPLOAD; //0x01 is the upload memo option
		buf[pos++] = 0x00; //unknow byte
		pos += EvaUtil::write32( buf+pos, m_Id);//qq number
		buf[pos++] = 0x00; //unknow byte
		
		for(int i=0; i<QQ_MEMO_FIELDS; i++){
			len = m_Infos.at(i).length();
			buf[pos++] = 0xff & len;
			memcpy(buf+pos, m_Infos.at(i).c_str(), len);
			pos += len;
			}
		}
		break;
	case 0x02:{
		buf[pos++] = QQ_MEMO_REMOVE; //0x02 is the remove memo option
		pos += EvaUtil::write32( buf+pos,m_Id);//qq number
		}
		break;
	case 0x03:{
		buf[pos++] = QQ_MEMO_DOWNLOAD; //0x03 is the download memo option
		pos += EvaUtil::write32( buf+pos,m_Id);//qq number
		}
		break;
	}
	return pos;
}

/*  ======================================================= */
EvaMemoReplyPacket::EvaMemoReplyPacket( unsigned char *buf, int len )
	:InPacket( buf, len )
{
}

EvaMemoReplyPacket::EvaMemoReplyPacket( const EvaMemoReplyPacket &rhs )
	:InPacket( rhs )
{
	m_Memo = rhs.getMemo();
}

EvaMemoReplyPacket &EvaMemoReplyPacket::operator=( const EvaMemoReplyPacket &rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	m_Type = rhs.getType();
	m_ReplyCode = rhs.getReplyCode();
	m_Memo = rhs.getMemo();
	m_Id = rhs.getQQ();
	return *this;
}

void EvaMemoReplyPacket::setMemo( const stringList &infos )
{
	m_Memo.name = infos.at(0);
	m_Memo.mobile = infos.at(1);
	m_Memo.telephone = infos.at(2);
	m_Memo.address = infos.at(3);
	m_Memo.email = infos.at(4);
	m_Memo.zipcode = infos.at(5);
	m_Memo.note = infos.at(6);
}

void EvaMemoReplyPacket::parseBody()
{
	int pos = 0;
	int len = 0;
	switch(decryptedBuf[pos++]){
	case 0x01:
		
		m_Type = QQ_MEMO_UPLOAD;
		m_ReplyCode = decryptedBuf[pos];
		break;
	case 0x02:
		m_Type = QQ_MEMO_REMOVE;
		m_ReplyCode = decryptedBuf[pos];
		break;
	case 0x03:{
		m_Type = QQ_MEMO_DOWNLOAD;
		if(bodyLength>1){
			m_Id = EvaUtil::read32(decryptedBuf+pos);//get the qq number
			pos += 5; //included an ignored unknow byte
		
			for( int i=0; i<QQ_MEMO_FIELDS; i++ ){
				len = decryptedBuf[pos++];
				char *str = new char[len+1];
				memcpy(str, decryptedBuf+pos, len); pos += len;
				str[len] = 0x00;
				m_Infos.push_back(std::string(str));
				delete[] str;
				}
			setMemo(m_Infos);
			}
			else{
				printf("no memo on the server,set user qq number to zero\n");
				setQQ(0);//if there is no user memo on the server,then set the qq number zero;
			}
		}
		break;
	}
	
}
