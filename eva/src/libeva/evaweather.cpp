/*

libevaX: Protocol Extensions for libeva library written by yunfan

Copyright 2006 Studio KUMA, Written by Stark Wong(starkwong@hotmail.com).
Portions of code converted from jql(Java QQ Library) written by Luma.

                        protocol updated by yunfan, 26th, Jan, 2007

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "evaweather.h"
#include "evadefines.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

WeatherOpPacket::WeatherOpPacket( )
: OutPacket(),
subCommand(QQ_SUB_CMD_GET_WEATHER)
{
}

WeatherOpPacket::WeatherOpPacket( const unsigned int ip )
: OutPacket(QQ_CMD_WEATHER, true),
subCommand(QQ_SUB_CMD_GET_WEATHER), ip(ip)
{
}

WeatherOpPacket::WeatherOpPacket(const WeatherOpPacket &rhs)
: OutPacket(rhs)
{
	subCommand=rhs.getSubCommand();
	ip=rhs.getIP();
}

WeatherOpPacket & WeatherOpPacket::operator =( const WeatherOpPacket & rhs )
{
	*( (OutPacket *)this) = (OutPacket)rhs;
	subCommand=rhs.getSubCommand();
	ip=rhs.getIP();
	return *this;
}

int WeatherOpPacket::putBody( unsigned char * buf )
{
	int pos=0;
	int ip=this->ip;//htonl(this->ip);
	buf[pos++]=subCommand;
	memmove((char*)buf+pos,&ip,4);
	pos+=4;
	buf[pos++]=0;
	buf[pos++]=0; // one more 0x00 than before(2006standard)
	return pos;
}

/** ============================= */
Weather &Weather::operator=(const Weather &rhs) {
	time=rhs.getTime();
	//int year, month, day;
	shortDesc=rhs.getShortDesc();
	wind=rhs.getWind();
	lowTemperature=rhs.getLowTemperature();
	highTemperature=rhs.getHighTemperature();
	hint=rhs.getHint();
	return *this;
}

int Weather::parseBean(unsigned char* buf) {
	int pos=0, len;
	char* pszTemp;

	time = htonl(*(int*)buf);
	pos+=4;

	len=buf[pos++];
	pszTemp=new char[len+1];
	strncpy(pszTemp,(char*)buf+pos,len);
	pszTemp[len]=0;
	shortDesc=pszTemp;
	delete[] pszTemp;
	pos+=len;

	len=buf[pos++];
	pszTemp=new char[len+1];
	strncpy(pszTemp,(char*)buf+pos,len);
	pszTemp[len]=0;
	wind=pszTemp;
	delete[] pszTemp;
	pos+=len;

	lowTemperature = htons(*(short*)(buf+pos));
	pos+=2;
	highTemperature = htons(*(short*)(buf+pos));
	pos+=2;

	pos++;

	len=buf[pos++];
	pszTemp=new char[len+1];
	strncpy(pszTemp,(char*)buf+pos,len);
	pszTemp[len]=0;
	hint=pszTemp;
	delete[] pszTemp;
	pos+=len;
	return pos;
}

/************************************************************************/
/* WeatherOpReplyPacket                                                 */
/************************************************************************/

WeatherOpReplyPacket::WeatherOpReplyPacket(unsigned char* buf, int len)
: InPacket(buf, len)
{
}

WeatherOpReplyPacket::WeatherOpReplyPacket( const WeatherOpReplyPacket &rhs)
: InPacket(rhs)
{
	subCommand=rhs.getSubCommand();
	replyCode=rhs.getReplyCode();
	province=rhs.getProvince();
	city=rhs.getCity();
	weathers=rhs.getWeathers();
}

WeatherOpReplyPacket::~WeatherOpReplyPacket() {
	//while (weathers.size()) {
		/*Weather* w=weathers.front();
		delete w;
		weathers.pop_front();*/
	//}
}

WeatherOpReplyPacket &WeatherOpReplyPacket::operator=(const WeatherOpReplyPacket &rhs)
{
	*((InPacket *)this) = (InPacket)rhs;
	subCommand=rhs.getSubCommand();
	replyCode=rhs.getReplyCode();
	province=rhs.getProvince();
	city=rhs.getCity();
	weathers=rhs.getWeathers();
	return *this;
}

void WeatherOpReplyPacket::parseBody()
{
	//tests.push_back(1);
	//return;

	int pos=0;
	int len;
	char* pszTemp;

	/*
	FILE* fp=fopen("C:\\weather.txt","wb");
	fwrite(decryptedBuf,this->bodyLength,1,fp);
	fclose(fp);
	*/

	subCommand = decryptedBuf[pos++];
	replyCode = decryptedBuf[pos++];

	len = decryptedBuf[pos++];
	if(len == 0)
		return;		

	pszTemp=new char[len+1];
	strncpy(pszTemp,(char*)decryptedBuf+pos,len);
	pszTemp[len]=0;
	province = pszTemp;
	delete[] pszTemp;
	pos+=len;

	len = decryptedBuf[pos++];

	pszTemp=new char[len+1];
	strncpy(pszTemp,(char*)decryptedBuf+pos,len);
	pszTemp[len]=0;
	city = pszTemp;
	delete[] pszTemp;
	pos+=len;

	pos+=2;  // unknown 2 bytes

	len = decryptedBuf[pos++];
	if(len > 0) {
		pos+=len; // ignore the second city information
	}
	unsigned char count = decryptedBuf[pos++];

	while(count-- > 0) {
		Weather* weather=new Weather();
		pos+=weather->parseBean(decryptedBuf+pos);
		weathers.push_back(*weather);
		//tests.push_back(1);
		delete weather;
	}
	// unknown 2 bytes, ignore
}
 
