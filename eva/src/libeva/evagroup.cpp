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
 
#include "evagroup.h"
#include "evadefines.h"
#include <stdio.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <cstring>

GroupNameOpPacket::GroupNameOpPacket( )
	: OutPacket(QQ_CMD_GROUP_NAME_OP, true),
	type(QQ_UPLOAD_GROUP_NAME)
{
} 

GroupNameOpPacket::GroupNameOpPacket(const char cmdType)
	: OutPacket(QQ_CMD_GROUP_NAME_OP, true),
	type(cmdType)
{
}

GroupNameOpPacket::GroupNameOpPacket(const GroupNameOpPacket &rhs)
	: OutPacket(rhs)
{
	groups = rhs.getGroups();
	type = rhs.getType();
}

GroupNameOpPacket &GroupNameOpPacket::operator=(const GroupNameOpPacket &rhs)
{
	*((OutPacket*)this) = (OutPacket)rhs;
	groups = rhs.getGroups();
	type = rhs.getType();
	return *this;
}

int GroupNameOpPacket::putBody( unsigned char * buf )
{
    int pos = 0;
    buf[pos++] = type;
    if(type == QQ_UPLOAD_GROUP_NAME) {
            unsigned char groupIndex = 0x00;
	    std::list<std::string>::iterator iter = groups.begin();
	    for(++iter; iter != groups.end(); ++iter){  // we should ignore the first group name(default fixed string)
                    buf[pos++]=(unsigned char)(++groupIndex);
                    if(iter->length() > (uint)QQ_MAX_GROUP_NAME_BYTE){
                            memcpy(buf+pos, iter->c_str(), QQ_MAX_GROUP_NAME_BYTE);					
                            pos+=QQ_MAX_GROUP_NAME_BYTE;
                    }else {
                            memcpy(buf+pos, iter->c_str(), iter->length());
                            pos+=iter->length();
                            int j = QQ_MAX_GROUP_NAME_BYTE - iter->length();
                            while(j-- > 0)
                                    buf[pos++]=0x00;
                    }
            }
    } else {
	//buf[pos++]=0x02;
	buf[pos++] = 0x01;
	memset(buf+pos, 0, 4);
	pos+=4;
    }
    return pos;
}

/* =========================================================== */

GroupNameOpReplyPacket::GroupNameOpReplyPacket(unsigned char* buf, int len)
	: InPacket(buf, len),
	type(QQ_DOWNLOAD_GROUP_NAME)
{
}

GroupNameOpReplyPacket::GroupNameOpReplyPacket(const GroupNameOpReplyPacket &rhs)
	: InPacket(rhs)
{
	type = rhs.getType();
	groupNames = rhs.getGroupNames();
}

GroupNameOpReplyPacket &GroupNameOpReplyPacket::operator=( const GroupNameOpReplyPacket &rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	type = rhs.getType();
	groupNames = rhs.getGroupNames();
	return *this;	
}

void GroupNameOpReplyPacket::parseBody()
{
	groupNames.clear();	
	type = decryptedBuf[0];
	if(type == QQ_DOWNLOAD_GROUP_NAME) {
		int offset=7;
		while(bodyLength>offset) {
			std::string name((char*)(decryptedBuf+offset));
			groupNames.push_back(name);
			offset += 17;
		}
	}
}

const bool GroupNameOpReplyPacket::isDownloadReply() const 
{ 
	return type == QQ_DOWNLOAD_GROUP_NAME; 
} 

/* =========================================================== */

DownloadFriendEntry::DownloadFriendEntry(const char *buf)
{
	readData(buf);
}

DownloadFriendEntry::DownloadFriendEntry(const DownloadFriendEntry &rhs)
{
	*this = rhs;
}

DownloadFriendEntry &DownloadFriendEntry::operator=(const DownloadFriendEntry &rhs)
{
	qqNum = rhs.getQQ();
	type = rhs.getType();
	groupID = rhs.getGroupID();
	return *this;	
}

const int DownloadFriendEntry::readData(const char *buf)
{
	int tmp4;
	memcpy(&tmp4, buf, 4);  
	qqNum = ntohl(tmp4);
	type = buf[4];
	groupID = ( buf[5] >> 2) & 0x3f;  
	return 6;
}

const bool DownloadFriendEntry::isQun() const
{
	return type == QQ_ID_IS_QUN;
}

/* =========================================================== */

DownloadGroupFriendPacket::DownloadGroupFriendPacket()
	: OutPacket(QQ_CMD_DOWNLOAD_GROUP_FRIEND, true),
	startID(0)
{
}

DownloadGroupFriendPacket::DownloadGroupFriendPacket(const int start)
	: OutPacket(QQ_CMD_DOWNLOAD_GROUP_FRIEND, true),
	startID(start)
{
}

DownloadGroupFriendPacket::DownloadGroupFriendPacket(const DownloadGroupFriendPacket &rhs)
	: OutPacket(rhs)
{
	startID = rhs.getStartID();
}

DownloadGroupFriendPacket &DownloadGroupFriendPacket::operator=(const DownloadGroupFriendPacket &rhs)
{
	*( (OutPacket *)this) = (OutPacket)rhs;
	startID = rhs.getStartID();
	return *this;
}

int DownloadGroupFriendPacket::putBody(unsigned char* buf)
{
	buf[0]=0x01;
	buf[1]=0x02;
	memset(buf+2, 0, 4); 
	int tmp4 = htonl(startID);
	memcpy(buf+6, &tmp4, 4);
	return 10;
}

/* =========================================================== */

DownloadGroupFriendReplyPacket::DownloadGroupFriendReplyPacket(unsigned char* buf, int len)
	: InPacket(buf, len)
{	
}

DownloadGroupFriendReplyPacket::DownloadGroupFriendReplyPacket( const DownloadGroupFriendReplyPacket &rhs)
	: InPacket(rhs)
{
	nextStartID = rhs.getNextStartID();
	friends = rhs.getGroupedFriends();
}

DownloadGroupFriendReplyPacket &DownloadGroupFriendReplyPacket::operator=( const DownloadGroupFriendReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	nextStartID = rhs.getNextStartID();
	friends = rhs.getGroupedFriends();
	return *this;
}

void DownloadGroupFriendReplyPacket::parseBody()
{
// 	printf("DownloadGroupFriendReplyPacket::parseBody \n");
// 	for(int i=0; i<bodyLength+6; i++){
// 		if(!(i%8)) printf("\n%d: ",i);
// 		char t = decryptedBuf[i];
// 		printf("%2x ", (uint8_t)t);
// 	}
// 	printf("\n----====== DownloadGroupFriendReplyPacket Finished ======----\n\n");
	friends.clear();
	int offset = 6;
	int tmp4;
	memcpy(&tmp4, decryptedBuf +offset, 4);
	offset+=4;
	nextStartID = ntohl(tmp4);
	
	while(bodyLength>offset) {
		DownloadFriendEntry f((char *)(decryptedBuf+offset));
		friends.push_back(f);	
		offset+=f.numberOfBytesRead();
	}
}

/* =========================================================== */

UploadGroupFriendPacket::UploadGroupFriendPacket()
	: OutPacket(QQ_CMD_UPLOAD_GROUP_FRIEND, true)
{
}

UploadGroupFriendPacket::UploadGroupFriendPacket(const UploadGroupFriendPacket &rhs)
	: OutPacket(rhs)
{
	privateList = rhs.getGroupedFriends();
}

UploadGroupFriendPacket::~UploadGroupFriendPacket()
{
}

UploadGroupFriendPacket &UploadGroupFriendPacket::operator=( const UploadGroupFriendPacket &rhs)
{
	*( (OutPacket *)this) = (OutPacket)rhs;
	privateList = rhs.getGroupedFriends();
	return *this;
}

int UploadGroupFriendPacket::putBody(unsigned char* buf)
{
	std::map<unsigned int, unsigned int>::iterator iter;
	unsigned int tmp4;
	int pos = 0;
	for(iter =privateList.begin(); iter != privateList.end(); ++iter){
		tmp4 = htonl(iter->first);
		memcpy(buf+pos, &tmp4, 4);
		pos+=4;
		buf[pos++] = (unsigned char)(iter->second);
	}
	return pos;
}

/* =========================================================== */

UploadGroupFriendReplyPacket::UploadGroupFriendReplyPacket(unsigned char* buf, int len)
	: InPacket(buf, len)
{
}

UploadGroupFriendReplyPacket::UploadGroupFriendReplyPacket(const UploadGroupFriendReplyPacket &rhs)
	: InPacket(rhs)
{
	replyCode = rhs.getReplyCode();
}

UploadGroupFriendReplyPacket::~UploadGroupFriendReplyPacket()
{
}

UploadGroupFriendReplyPacket &UploadGroupFriendReplyPacket::operator=(const UploadGroupFriendReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	replyCode = rhs.getReplyCode();
	return *this;
}

void UploadGroupFriendReplyPacket::parseBody()
{
	replyCode = decryptedBuf[0];
}








