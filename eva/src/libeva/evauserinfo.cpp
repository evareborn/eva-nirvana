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
#include "evauserinfo.h"
#include "evadefines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <cstdlib>

// important: all string stored are encoded by "GB18030"

ContactInfo::ContactInfo() 
{ 
	infos.reserve(QQ_CONTACT_FIELDS); 
	infos.push_back("-");   // qq number 
	infos.push_back("EVA");   // nick
};

ContactInfo::ContactInfo(const unsigned char *buf, const int len)  
{
	parseData(buf, len);
}

ContactInfo::ContactInfo( const ContactInfo &rhs)
{
	infos = rhs.details();	
}

void ContactInfo::parseData(const unsigned char *buf, const int len)  
{
	int start = 0;
	infos.clear();
	for(int i = 0; i< len; i++){
		if(buf[i]!= DIVIDER) continue;
		char *tmp = (char *)malloc( (i-start+1) *sizeof(char));
		memcpy(tmp, buf+start, i-start);
		tmp[i-start] = 0x00;
		infos.push_back(std::string(tmp));
		start = i+1;
		free(tmp);
	}
	if(infos.size()< (uint)QQ_CONTACT_FIELDS){
		char *tmp = (char *)malloc( (len-start+1) *sizeof(char));
		memcpy(tmp, buf+start, len-start);
		tmp[len-start]=0x00;
		infos.push_back(std::string(tmp));
		free(tmp);
	}
}

bool ContactInfo::operator== ( const ContactInfo &rhs ) const
{
    if( infos.size() != rhs.details().size() ) return false;    
    return (infos==rhs.details());
}

ContactInfo &ContactInfo::operator= ( const ContactInfo &rhs )
{
	if( this == &rhs)   return *this;    
	infos = rhs.details();    
	return *this;
}

/*  ======================================================= */

GetUserInfoPacket::GetUserInfoPacket()
	: OutPacket(QQ_CMD_GET_USER_INFO, true),
	  qqNum(-1)
{
}

GetUserInfoPacket::GetUserInfoPacket(const int id)
	: OutPacket(QQ_CMD_GET_USER_INFO, true),
	  qqNum(id)
{
}

GetUserInfoPacket::GetUserInfoPacket(const GetUserInfoPacket &rhs)
	: OutPacket(rhs)
{
	qqNum = rhs.getUserQQ();
}

GetUserInfoPacket &GetUserInfoPacket::operator=(const GetUserInfoPacket &rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	qqNum = rhs.getUserQQ();
        return *this;
}

int GetUserInfoPacket::putBody(unsigned char *buf)
{
    sprintf((char *)buf, "%d", qqNum);
    return strlen((char *)buf);
}

/*  ======================================================= */

GetUserInfoReplyPacket::GetUserInfoReplyPacket(unsigned char *buf, int len)
	: InPacket( buf, len)
{
}

GetUserInfoReplyPacket::GetUserInfoReplyPacket( const GetUserInfoReplyPacket &rhs)
	: InPacket(rhs)
{
	mContactInfo = rhs.contactInfo();
}

GetUserInfoReplyPacket &GetUserInfoReplyPacket::operator=(const GetUserInfoReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	mContactInfo = rhs.contactInfo();
        return *this;
}

void GetUserInfoReplyPacket::parseBody() 
{
	mContactInfo.parseData(decryptedBuf, bodyLength);
	int j = mContactInfo.details().size();
	if(j < QQ_CONTACT_FIELDS)
		fprintf(stderr, "GetUserInfoReply->parseBody: number of fields wrong\n");
	else if(j > QQ_CONTACT_FIELDS)
		fprintf(stderr, "GetUserInfoReply->parseBody: number of fields might be wrong!\n");
}

/*  ======================================================= */

ModifyInfoPacket::ModifyInfoPacket( ) 
	: OutPacket(QQ_CMD_MODIFY_INFO, true)
{
}

ModifyInfoPacket::ModifyInfoPacket( const ContactInfo & info )
	: OutPacket(QQ_CMD_MODIFY_INFO, true),
	newInfo(info), currentPwd(""), newPwd("")
{
}

ModifyInfoPacket::ModifyInfoPacket( const ModifyInfoPacket & rhs )
	: OutPacket(rhs)
{
	currentPwd = rhs.getPassword();
	newPwd = rhs.getNewPassword();
	newInfo = rhs.getContactInfo();
}

ModifyInfoPacket & ModifyInfoPacket::operator =( const ModifyInfoPacket & rhs )
{
	*((OutPacket*)this) = (OutPacket)rhs;
	currentPwd = rhs.getPassword();
	newPwd = rhs.getNewPassword();
	newInfo = rhs.getContactInfo();	
	return *this;
}

int ModifyInfoPacket::putBody( unsigned char * buf )
{
	int pos=0;	
	if( currentPwd != "" && newPwd != ""){
		memcpy(buf, currentPwd.c_str(), currentPwd.length());
		pos+=currentPwd.length();
		buf[pos++] = DELIMIT;
		memcpy(buf+pos, newPwd.c_str(), newPwd.length());
		pos+=newPwd.length();
	}else
		buf[pos++] = DELIMIT;
	
	buf[pos++] = DELIMIT;
	
	for(int i=1; i<QQ_CONTACT_FIELDS; i++){
		memcpy(buf+pos, newInfo.at(i).c_str(),newInfo.at(i).length());
		pos+=newInfo.at(i).length();
		buf[pos++] = DELIMIT;
	}

	return pos;
}

/*  ======================================================= */

ModifyInfoReplyPacket::ModifyInfoReplyPacket( unsigned char * buf, int len )
	: InPacket(buf, len),
	accepted(false)
{
}

ModifyInfoReplyPacket::ModifyInfoReplyPacket( const ModifyInfoReplyPacket & rhs )
	: InPacket(rhs)
{
	accepted = rhs.isAccepted();
}

ModifyInfoReplyPacket & ModifyInfoReplyPacket::operator =( const ModifyInfoReplyPacket & rhs )
{
	*((InPacket*)this) = (InPacket)rhs;
	accepted = rhs.isAccepted();
	return *this;
}

void ModifyInfoReplyPacket::parseBody( )
{
	char *str = (char *)malloc((bodyLength+1) * sizeof(char));
	memcpy(str, decryptedBuf, bodyLength);
	str[bodyLength]=0x00;
	
	char myQQ[20];
	sprintf(myQQ, "%d", getQQ());
	char *pos = strstr(str, myQQ);
	if( pos != str )
		accepted = false;
	else
		accepted = true;
}

