/***************************************************************************
 *   Copyright (C) 2004-2005 by yunfan                                     *
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
#ifndef LIBEVAUTIL_H
#define LIBEVAUTIL_H

#include <string>
#include "md5.h"
#define KEY_LENGTH 16
#define QQ_SMILEY_AMOUNT 135

/** Visual Studio .NET 2002/2003 Additional Definitions */
#ifdef WIN32
#pragma warning(disable: 4309 4800)
typedef unsigned int uint;
typedef unsigned char uint8_t;
#include <malloc.h>
#endif

#define READ16(buf)  (ntohs(*((unsigned short *)(buf)) ))
#define READ32(buf)  (ntohl(*((unsigned int *)(buf)) ))
#define WRITE16(buf,v) (*((unsigned short *)(buf))  = htons(v))
#define WRITE32(buf,v) (*((unsigned int *)(buf))  = htonl(v))

// this class provides some utilities, like md5 function and parse text smiley into ready to send smiley code.

class EvaUtil{
public:
	EvaUtil();
	// the following 2 functions alway return 16 bytes due to the protocol used by QQ
	static char *doMd5(char *in, int len);
	static char *doMd5Md5(char *in, int len);
	
	static std::string smileyToText(const char smileyCode);     // get text description of the smiley code
	static char textToSmiley(const std::string &textTag);      // get smiley code of the text description
	static int textToFileIndex(const std::string &textTag);  // get the file name in integer of the smiley text description	
	static std::string fileIndexToText( const int fileIndex);
	static std::string convertToSend(const std::string &text);
	static std::string convertToSend(const std::string &text, bool *hasImage);
	static void initMap();
	
	static std::string customSmileyToText(const char *buf, int *smileyLength, const char *uuid);
	
	static void calcSuns(const unsigned short level, int *suns, int *moons, int *stars);
        typedef struct textMap{
		//char gb[5];             // chinese charactor not supported, cuz only few people use them
		char en[16];
		char py[16];
	} textMap;

	static int write16(unsigned char *buf, const unsigned short value); // return number of bytes written
	static int write32(unsigned char *buf, const unsigned int value);   // return number of bytes writen
	static unsigned short read16(const unsigned char *buf); // return the short value
	static unsigned int read32(const unsigned char *buf);  // return the int value
private:	
	static char md5Buf[KEY_LENGTH];
	static textMap map[QQ_SMILEY_AMOUNT];
	static const char smileyMap[QQ_SMILEY_AMOUNT];
	static int smileyToFileIndex( const char smileyCode);
	
};

#endif
 
