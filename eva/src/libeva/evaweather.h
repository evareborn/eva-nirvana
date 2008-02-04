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

Usage: Add this file to be included in libeva.h
*/

#ifndef LIBEVAWEATHER_H
#define LIBEVAWEATHER_H

#include <string>
#include <list>
using namespace std;
#include "evapacket.h"

/************************************************************************/
/* WeatherOpPacket: Request for weather information                     */
/************************************************************************/
class WeatherOpPacket : public OutPacket
{
public:
	WeatherOpPacket();
	WeatherOpPacket(const unsigned int ip);
	WeatherOpPacket(const WeatherOpPacket &rhs);
	virtual ~WeatherOpPacket() {};

	OutPacket *copy(){return new WeatherOpPacket(*this);}
	WeatherOpPacket &operator=( const WeatherOpPacket &rhs );

	void setSubCommand(const unsigned char subCommand) { this->subCommand=subCommand; }
	const unsigned char getSubCommand() const { return subCommand; }
	void setIP(const unsigned int ip) { this->ip=ip; }
	const unsigned int getIP() const { return ip; }
protected:
	virtual int putBody(unsigned char* buf);
private:
	unsigned char subCommand;
	unsigned int ip;
};

/************************************************************************/
/* Weather: Container for each day's weather forecast                   */
/************************************************************************/
class Weather {
public:
	Weather() {}
	virtual ~Weather() {};
	Weather &operator=(const Weather &rhs);
	Weather *copy() { Weather* w=new Weather(); *w=*this; return w;}

	const int getTime() const { return time; }
	const string getShortDesc() const { return shortDesc; }
	const string getWind() const { return wind; }
	const int getLowTemperature() const { return lowTemperature; }
	const int getHighTemperature() const { return highTemperature; }
	const string getHint() const { return hint; }
	virtual int parseBean(unsigned char* buf);
private:
	int time;
	string shortDesc;
	string wind;
	int lowTemperature, highTemperature;
	string hint;
};

/************************************************************************/
/* WeatherOpReplyPacket: Weather information reply                      */
/************************************************************************/
class WeatherOpReplyPacket : public InPacket
{
public:
	WeatherOpReplyPacket() {}
	WeatherOpReplyPacket(unsigned char* buf, int len);
	WeatherOpReplyPacket(const WeatherOpReplyPacket &rhs);	
	virtual ~WeatherOpReplyPacket();

	InPacket *copy() { return new WeatherOpReplyPacket(*this); }
	WeatherOpReplyPacket &operator=(const WeatherOpReplyPacket &rhs);

	const unsigned char getSubCommand() const { return subCommand; }
	const unsigned char getReplyCode() const { return replyCode; }	
	const string getProvince() const { return province; }
	const string getCity() const { return city; }
	list<Weather> getWeathers() const { return weathers; }
protected:
	virtual void parseBody();
private:		
	unsigned char subCommand;
	unsigned char replyCode;
	string province;
	string city;
	std::list<Weather> weathers;
	//std::list<int> tests;
};

#endif // LIBEVAWEATHER_H 
