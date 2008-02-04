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
 #include "evaadvancedsearch.h"
 #include "evautil.h"
 
AdvancedUser::AdvancedUser()
	:m_QQNum(0),
	m_GenderIndex(0),
	m_Age(0),
	m_Online(0),
	m_Nick(""),
	m_ProvinceIndex(0),
	m_CityIndex(0),
	m_Face(0)
	
{
}

AdvancedUser& AdvancedUser::operator=( const AdvancedUser& rhs )
{
	m_QQNum = rhs.getQQ();
	m_GenderIndex = rhs.getGenderIndex();
	m_Age = rhs.getAge();
	m_Online = rhs.isOnline() ? 0x01:0x00;
	m_Nick = rhs.getNick();
	m_ProvinceIndex = rhs.getProvinceIndex();
	m_CityIndex = rhs.getCityIndex();
	m_Face = rhs.getFace();
	
	return *this;
}

int AdvancedUser::readData( unsigned char *buf )
{
	int pos = 0;
	int len = 0;
	m_QQNum = EvaUtil::read32(buf);//4字节的QQ号码
	pos += 4;
	m_GenderIndex = buf[pos++] & 0xff; //1字节性别的下拉索引
	m_Age = EvaUtil::read16(buf + pos);//2字节的年龄
	pos += 2;
	m_Online = buf[pos++];//1字节的在线标志，0x00表示离线，0x01表示在线
	len = buf[pos++] & 0xff;//1字节昵称的长度
	char *str = new char[len+1];
	memcpy(str, buf+pos, len);
	str[len] = 0x00;
	pos += len;
	m_Nick = std::string(str);//昵称
	delete[] str;
	m_ProvinceIndex = EvaUtil::read16(buf + pos);//2字节省份索引
	pos += 2;
	m_CityIndex = EvaUtil::read16(buf + pos);//2字节城市索引
	pos += 2;
	m_Face = EvaUtil::read16(buf + pos);//2字节头像索引
	pos += 2;
	
	return pos;
}

/*--------------------------------------------------*/
EvaAdvancedSearchPacket::EvaAdvancedSearchPacket()
	:OutPacket(QQ_CMD_ADVANCED_SEARCH, true),
	m_SearchOnline(true),
	m_HasCam(false),
	m_Page(0),
	m_AgeIndex(0),
	m_GenderIndex(0),
	m_ProvinceIndex(0),
	m_CityIndex(0)
{
}

EvaAdvancedSearchPacket::EvaAdvancedSearchPacket(EvaAdvancedSearchPacket& rhs)
	:OutPacket(rhs)
{
	m_SearchOnline = rhs.getSearchOnline();
	m_HasCam = rhs.getHasCam();
	m_Page = rhs.getPage();
	m_AgeIndex = rhs.getAgeIndex();
	m_GenderIndex = rhs.getGenderIndex();
	m_ProvinceIndex = rhs.getProvinceIndex();
	m_CityIndex = rhs.getCityIndex();
}

EvaAdvancedSearchPacket& EvaAdvancedSearchPacket::operator =( const EvaAdvancedSearchPacket& rhs)
{
	*((OutPacket *)this) = (OutPacket)rhs;
	m_SearchOnline = rhs.getSearchOnline();
	m_HasCam = rhs.getHasCam();
	m_Page = rhs.getPage();
	m_AgeIndex = rhs.getAgeIndex();
	m_GenderIndex = rhs.getGenderIndex();
	m_ProvinceIndex = rhs.getProvinceIndex();
	m_CityIndex = rhs.getCityIndex();
	
	return *this;
}

int EvaAdvancedSearchPacket::putBody(unsigned char *buf)
{
	int pos = 0;
	buf[pos++] = 0x01; // probably a sub-command
	pos += EvaUtil::write16(buf+pos, m_Page);	//2字节页数，从0开始
	buf[pos++] = m_SearchOnline ? 0x01:0x00;// 1字节在线与否，0x01表示在线，0x00表示不在线
	buf[pos++] = m_HasCam ? 0x01:0x00;	//1字节是否有摄像头，0x01表示有，0x00表示无
	buf[pos++] = m_AgeIndex & 0xff;		//1字节年龄，表示在下拉框中的索引
	buf[pos++] = m_GenderIndex & 0xff;	//1字节性别，表示在下拉框中的索引
	pos += EvaUtil::write16(buf+pos, m_ProvinceIndex);	//2字节省份，表示下拉框中的索引
	pos += EvaUtil::write16(buf+pos, m_CityIndex);		//2字节城市，表示下拉框中的索引

	return pos;
	
}
/*--------------------------------------------------*/

EvaAdvancedSearchReplyPacket::EvaAdvancedSearchReplyPacket(unsigned char* buf, int len)
	:InPacket(buf, len),
	m_ReplyCode(0),
	m_Page(0),
	m_Finished(false)
{
}

EvaAdvancedSearchReplyPacket::EvaAdvancedSearchReplyPacket(const EvaAdvancedSearchReplyPacket& rhs)
	:InPacket(rhs)
{
	m_ReplyCode = rhs.getReplyCode();
	m_Page = rhs.getPage();
	m_Users = rhs.getUsers();
	m_Finished = rhs.isFinished();
}

EvaAdvancedSearchReplyPacket& EvaAdvancedSearchReplyPacket::operator=( const EvaAdvancedSearchReplyPacket& rhs )
{
	*((InPacket *)this) = (InPacket)rhs;
	m_ReplyCode = rhs.getReplyCode();
	m_Page = rhs.getPage();
	m_Users = rhs.getUsers();
	m_Finished = rhs.isFinished();
	
	return *this;
}

void EvaAdvancedSearchReplyPacket::parseBody()
{
	int pos =0;
	pos++; // first byte is always 0x01, could be a sub-command(reply for the sub-command)
	m_ReplyCode = decryptedBuf[pos++];//1字节回复码，0x00表示还有数据，0x01表示没有更多数据了，当为0x01时，后面没有内容了
	
	if( m_ReplyCode == 0x00 ){
		m_Page = EvaUtil::read16(decryptedBuf + pos);//2字节页号，如果页号后面没有内容了，那也说明是搜索结束了
		pos += 2;
		while(pos < bodyLength){
			AdvancedUser au;
			pos += au.readData(decryptedBuf + pos);//读取一个AdvancedUser结构
			m_Users.push_back(au);
			pos++; // here is a seperator 0x00
		}
		m_Finished = m_Users.empty();
	}else{
		m_Finished = true;
	}
}
