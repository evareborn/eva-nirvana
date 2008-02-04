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
#ifndef LIBEVAINFO_H
#define LIBEVAINFO_H

#include "evapacket.h"
#include <string>
#include <vector>

typedef std::vector<std::string> stringList;
    
class ContactInfo {
public: 
	ContactInfo();
	ContactInfo(const unsigned char *buf, const int len);
	ContactInfo( const ContactInfo &rhs);
	~ContactInfo() {};
	
	void parseData(const unsigned char *buf, const int len);
	
	enum Info_Index{Info_qqID,       Info_nick,     Info_country,  Info_province,  Info_zipcode, 
			Info_address,    Info_telephone, Info_age,      Info_gender,    Info_name, 
			Info_email,      Info_pagerSn,   Info_pagerNum, Info_pagerSp,   Info_pagerBaseNum, 
			Info_pagerType,  Info_occupation,Info_homepage, Info_authType,  Info_unknown1, 
			Info_unknown2,   Info_face,      Info_mobile,   Info_mobileType,Info_intro, 
			Info_city,       Info_unknown3,  Info_unknown4, Info_unknown5,  Info_openHp, 
			Info_openContact,Info_college,   Info_horoscope,Info_zodiac,    Info_blood, 
			Info_qqShow,     Info_unknown6  };
			
	const std::string &at(const Info_Index index) const { return infos[index]; }
	const std::string &at(const int index) const { return infos[index]; }  // note this method will not test if out of index range!
	const stringList &details() const {  return infos; };
	const uint count() const { return infos.size(); }
	
	void setDetails(const stringList &strList) { infos = strList; }
	bool operator== ( const ContactInfo &rhs ) const;
	ContactInfo &operator= ( const ContactInfo &rhs ) ;
private:
	stringList infos; 
	static const unsigned char DIVIDER = 0x1e; 
};

class GetUserInfoPacket : public OutPacket {
public: 
	GetUserInfoPacket();
	GetUserInfoPacket(const int id);
	GetUserInfoPacket(const GetUserInfoPacket &rhs);
	virtual ~GetUserInfoPacket() {}
	
	OutPacket * copy() { return new GetUserInfoPacket(*this);}
	GetUserInfoPacket &operator=(const GetUserInfoPacket &rhs);
	
	void setUserQQ(const int qqNum) { this->qqNum = qqNum; }
	const int getUserQQ() const { return qqNum; }
protected:
	virtual int putBody(unsigned char *buf); 
     
private:
	int qqNum;
};

class GetUserInfoReplyPacket : public InPacket {
public:
	GetUserInfoReplyPacket() {}
	GetUserInfoReplyPacket(unsigned char *buf, int len);
	GetUserInfoReplyPacket( const GetUserInfoReplyPacket &rhs);
	virtual ~GetUserInfoReplyPacket() {}

	InPacket *copy() { return new GetUserInfoReplyPacket(*this);}
	GetUserInfoReplyPacket &operator=(const GetUserInfoReplyPacket &rhs);
	
	const ContactInfo &contactInfo() const { return mContactInfo; }
protected:
	virtual void parseBody();
private:
	ContactInfo mContactInfo;
};

class ModifyInfoPacket : public OutPacket {
public:
	ModifyInfoPacket();
	ModifyInfoPacket(const ContactInfo &info);
	ModifyInfoPacket(const ModifyInfoPacket &rhs);
	virtual ~ModifyInfoPacket() {}
	
	OutPacket *copy() { return new ModifyInfoPacket(*this); }
	ModifyInfoPacket &operator=(const ModifyInfoPacket &rhs);
	
	void setPassword( const std::string &password) { currentPwd = password; };
	void setNewPassword( const std::string &password) { newPwd = password; };
	void setContactInfo( const ContactInfo &info) { newInfo = info; }
	
	const std::string getPassword() const { return currentPwd; }
	const std::string getNewPassword() const { return newPwd; }
	const ContactInfo getContactInfo() const { return newInfo; }
protected:
	virtual int putBody(unsigned char *buf);
private:
	ContactInfo newInfo;
	std::string currentPwd;
	std::string newPwd;
	static const unsigned char DELIMIT = 0x1f;
};

class ModifyInfoReplyPacket : public InPacket {
public:
	ModifyInfoReplyPacket() {};
	ModifyInfoReplyPacket(unsigned char *buf, int len);
	ModifyInfoReplyPacket(const ModifyInfoReplyPacket &rhs);
	virtual ~ModifyInfoReplyPacket() {}
	
	InPacket *copy() { return new ModifyInfoReplyPacket(*this);}
	ModifyInfoReplyPacket &operator=(const ModifyInfoReplyPacket &rhs);	
	
	const bool isAccepted() const { return accepted; }
protected:
	virtual void parseBody();
private:
	bool accepted;
};

#endif
