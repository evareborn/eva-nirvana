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
 #ifndef EVAADVANCEDSEARCH_H
 #define EVAADVANCEDSEARCH_H
 
 #include "evapacket.h"
 #include "evadefines.h"
 #include <string>
 #include <list>
 
 class AdvancedUser{
 public:
 	AdvancedUser();
	~AdvancedUser(){}
	
	const int getQQ() const { return m_QQNum; }
	const int getGenderIndex() const { return m_GenderIndex; }
	const int getAge() const { return m_Age; }
	const bool isOnline() const { return (m_Online == 0x01)? true:false; }
	const std::string getNick() const { return m_Nick; }
	const int getProvinceIndex() const { return m_ProvinceIndex; }
	const int getCityIndex() const { return m_CityIndex; }
	const int getFace() const { return m_Face; }
	
	void setQQ( const int id ) { m_QQNum = id; }
	void setGenderIndex( const int genderIndex ) { m_GenderIndex = genderIndex; }
	void setAge( const int age ) { m_Age = age; }
	void setOnlineStatus( const unsigned char onlineStatus ) { m_Online = onlineStatus; }
	void setNick( const std::string nick ) { m_Nick = nick; }
	void setProvinceIndex( const int province ) { m_ProvinceIndex = province; }
	void setCityIndex( const int city ) { m_CityIndex = city; }
	void setFace( const int face ) { m_Face = face; }  
	
	int readData( unsigned char *buf);
	AdvancedUser &operator=( const AdvancedUser &rhs );
private:
	int m_QQNum;
	int m_GenderIndex;
	int m_Age;
	unsigned char m_Online;
	std::string m_Nick;
	int m_ProvinceIndex;
	int m_CityIndex;
	int m_Face;
};
/*--------------------------------------------------*/
class EvaAdvancedSearchPacket:public OutPacket
{
public:
	EvaAdvancedSearchPacket();
	EvaAdvancedSearchPacket( EvaAdvancedSearchPacket &rhs );
	virtual ~EvaAdvancedSearchPacket(){}
	
	const bool getSearchOnline() const { return m_SearchOnline; }
	const bool getHasCam() const { return m_HasCam; }
	const int getPage() const { return m_Page; }
	const int getAgeIndex() const { return m_AgeIndex; }
	const int getGenderIndex() const { return m_GenderIndex; }
	const int getProvinceIndex() const { return m_ProvinceIndex; }
	const int getCityIndex() const { return m_CityIndex; }
	
	void setSearchOnline( const bool online ) { m_SearchOnline = online; }
	void setHasCam( const bool hasCam ) { m_HasCam = hasCam; }
	void setPage( const int page ) { m_Page = page; }
	void setAgeIndex( const int ageIndex ) { m_AgeIndex = ageIndex; }
	void setGenderIndex( const int genderIndex ) { m_GenderIndex = genderIndex; }
	void setProvinceIndex( const int provinceIndex) { m_ProvinceIndex = provinceIndex; }
	void setCityIndex( const int cityIndex ) { m_CityIndex = cityIndex; }
	
	EvaAdvancedSearchPacket &operator=( const EvaAdvancedSearchPacket& rhs);
protected:
	virtual int putBody( unsigned char *buf );
private:
	bool m_SearchOnline;
	bool m_HasCam;
	int m_Page;
	int m_AgeIndex;
	int m_GenderIndex;
	int m_ProvinceIndex;
	int m_CityIndex;
	
};

/*--------------------------------------------------*/
class EvaAdvancedSearchReplyPacket:public InPacket
{
public:
	EvaAdvancedSearchReplyPacket(){}
	EvaAdvancedSearchReplyPacket(unsigned char* buf, int len);
	EvaAdvancedSearchReplyPacket(const EvaAdvancedSearchReplyPacket &rhs); 
	virtual ~EvaAdvancedSearchReplyPacket(){}
	
	const unsigned char getReplyCode() const { return m_ReplyCode; }
	const int getPage() const { return m_Page; }
	const std::list<AdvancedUser> getUsers() const { return m_Users; }
	const bool isFinished() const { return m_Finished; }
	
	void setReplyCode( const unsigned char replyCode ) { m_ReplyCode = replyCode; }
	void setPage( const int page ) { m_Page = page; }
	void setUsers( const std::list<AdvancedUser> users ) { m_Users = users; }
	void setFinished( const bool finished ) { m_Finished = finished; }
	
	EvaAdvancedSearchReplyPacket &operator=( const EvaAdvancedSearchReplyPacket& rhs );
protected:
	virtual void parseBody();
private:
	unsigned char m_ReplyCode;
	int m_Page;
	std::list<AdvancedUser> m_Users;
	bool m_Finished;
};

#endif
