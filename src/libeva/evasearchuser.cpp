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
#include "evasearchuser.h"



#define   DIVIDER      0x1F
#define   NULL_FIELD   0x2D
#define   PERCENT      0x25

OnlineUser::OnlineUser()
{
	qqNum = -1;
	nick = "";
	province = "";
	face = 0;
}

OnlineUser & OnlineUser::operator =( const OnlineUser & rhs )
{
	qqNum = rhs.getQQ();
	nick = rhs.getNick();
	face = rhs.getFace();
	return *this;
}

int OnlineUser::readData( unsigned char * buf )
{
	int i = 0;
	int start = 0;
	int offset=0;
	char tmp[256];
	while(1){
		if(offset>60) break;
		if(buf[offset]!=0x1e && buf[offset]!=0x1f)
			offset++;
		else{
			memset(tmp, 0, 256);
			memcpy(tmp, buf+start, offset-start);
			tmp[offset]=0x00;
			if(i==0)
				qqNum = atoi(tmp);
			if(i==1)
				nick = tmp;
			if(i==2)
				province = tmp;
			if(i==3){
				face = atoi(tmp);
				break;
			}
			i++;
			start = ++offset;
		}
	}        
	// the last 2 bytes is 0x1e and 0x1f
	return offset+2;
}

/*----------------------------------------------------------------------------------------------*/

SearchUserPacket::SearchUserPacket( )
	:OutPacket(QQ_CMD_SEARCH_USER, true),
	searchType(QQ_SEARCH_ALL),
	page("0"),
	qqStr(""), nick(""), email(""),
	matchEntireString(false)
{
}

SearchUserPacket::SearchUserPacket( SearchUserPacket & rhs )
	: OutPacket(rhs)
{
	page = rhs.getPage();
	searchType = rhs.getSearchType();
	qqStr = rhs.getQQ();
	nick = rhs.getNick();
	email = rhs.getEmail();
	matchEntireString = rhs.getMatchEntireString();
}

void SearchUserPacket::setPage( const int p )
{
	char tmp[255];
	sprintf(tmp,"%d", p);
	page = tmp;
}

void SearchUserPacket::setQQ( const int qqNum )
{
	char tmp[255];
	sprintf(tmp,"%d", qqNum);
	qqStr = tmp;	
}

SearchUserPacket & SearchUserPacket::operator =( const SearchUserPacket & rhs )
{
	*((OutPacket *)this) = (OutPacket)rhs;
	page = rhs.getPage();
	searchType = rhs.getSearchType();
	qqStr = rhs.getQQ();
	nick = rhs.getNick();
	email = rhs.getEmail();
	matchEntireString = rhs.getMatchEntireString();	
	return *this;
}

int SearchUserPacket::putBody( unsigned char * buf )
{
	int offset = 0;	     

	buf[offset++] = searchType;
	buf[offset++] = DIVIDER;
	if(searchType == QQ_SEARCH_ALL) {
		memcpy(buf+offset, page.c_str(), page.length());
		offset+=page.length();			
	} else if(searchType == QQ_SEARCH_NICK || searchType == QQ_SEARCH_QQ) {
						
		if( searchType == QQ_SEARCH_QQ && qqStr.length()) {
			memcpy(buf+offset, qqStr.c_str(), qqStr.length());
			offset+=qqStr.length();
		} else{ 
			buf[offset++] = NULL_FIELD;
		}
		
		buf[offset++] = DIVIDER;
		
		if(searchType == QQ_SEARCH_NICK && nick.length()) {
			memcpy(buf+offset, nick.c_str(),  nick.length());
			offset+= nick.length();
			if(!matchEntireString)
				buf[offset++] = PERCENT;
		} else {
			buf[offset++] = NULL_FIELD;
		}
		buf[offset++] = DIVIDER;
		
		if(!email.length())
			buf[offset++] = NULL_FIELD;
		else {
			memcpy(buf+offset, email.c_str(), email.length());
			offset+=email.length();
			if(!matchEntireString)
				buf[offset++] = PERCENT;
		}
		buf[offset++] = DIVIDER;	
	
		memcpy(buf+offset, page.c_str(), page.length());
		offset+=page.length();		
		
		buf[offset++] = 0x00;
	}
	return offset;
}


/*----------------------------------------------------------------------------------------------*/


SearchUserReplyPacket::SearchUserReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len),
	finished(false)
{
}

SearchUserReplyPacket::SearchUserReplyPacket( const SearchUserReplyPacket & rhs )
	: InPacket(rhs)
{
	finished = rhs.isFinished();
	users = rhs.getUsers();
}

SearchUserReplyPacket & SearchUserReplyPacket::operator =( const SearchUserReplyPacket & rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	finished = rhs.isFinished();
	users = rhs.getUsers();	
	return *this;
}

void SearchUserReplyPacket::parseBody( )
{
	if( bodyLength == 0 || decryptedBuf[0] == NULL_FIELD && decryptedBuf[1] == QQ_SEARCH_ALL) {
		finished = true;
		return;
	}
	finished = false;
	int offset=0;
	users.clear();
	offset += 4;// 4 unknown bytes, 00 01 00 00
	if(  decryptedBuf[offset] == QQ_SEARCH_ALL && decryptedBuf[offset + 1] == DIVIDER) {
		offset += 2;
	}else{
		OnlineUser user;
		offset += user.readData(decryptedBuf + offset);
		users.push_back(user);
		// still some useless bytes left, don't care about
               return;
	}
	while(offset<bodyLength) {
		OnlineUser onlineUser;
		offset+=onlineUser.readData(decryptedBuf + offset);
		users.push_back(onlineUser);
	}
}


