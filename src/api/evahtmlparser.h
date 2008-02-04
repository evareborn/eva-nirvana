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
  
#ifndef EVAHTMLPARSER_H
#define EVAHTMLPARSER_H

#include "evautil.h"
#include <list>
#include <qobject.h>

typedef struct CustomizedPic{
	unsigned char type;  // should be 33, 34, 36, 37, ..     0 means error
	QString  fileName;
	QString  shortCutName;
	bool     isExisted;
	int      occurredIndex;
	//Q_UINT8  sessionID[4];
	Q_UINT32 sessionID;
	Q_UINT32 ip;
	Q_UINT16 port;
	Q_UINT8  fileAgentKey[16];
	QString  tmpFileName;
} CustomizedPic;

typedef struct OutCustomizedPic{
	unsigned int imageLength;
	unsigned char md5[16];
	QString fileName;
} OutCustomizedPic;

class EvaHtmlParser 
{
public:
	EvaHtmlParser(){}
	~EvaHtmlParser(){};
	std::list<CustomizedPic> convertToHtml(QString &txt, bool isURLOn = true, bool isAbsImgPath = false,
						bool useRealFileName = false);
	const int convertToPlainTxt(QString &html, QString &sendFileNameBase);
	void convertToPlainTxt(QString &html, const unsigned int agentSessionID, 
					const unsigned int agentIP, const unsigned short agentPort );
	std::list<QString> getCustomImages(const QString html);
	void parseToAbsPath(QString &html, const QString absPath);
	void setAbsImagePath(const QString &path,const QString &cachePath = QString::null) 
				{ absImagePath = path; absCustomCachesPath = cachePath;}
	const QString getAbsImagePath() const { return absImagePath; }
private:
	QString absImagePath;
	QString absCustomCachesPath;
	static unsigned int tmpNum;
	
	std::list< CustomizedPic > picList;
	std::list<CustomizedPic> convertCustomizedPictures(QString &text, bool useRealFileName = false);
	QString processPic32( const QString &src, CustomizedPic * args);
	QString processPic33( const QString &src, CustomizedPic *args);
	QString processPic34( const QString &src );
	QString processPic36( const QString &src, CustomizedPic *args);
	QString processPic37( const QString &src );
	
	QString generateSendFormat(QString &src, const unsigned int agentSessionID, const unsigned int agentIP, const unsigned short agentPort );
	QString generateSendFormat32(const QString sendFileNameBase, const QString &fileName, const int imgNo);
	//EvaUtil util;
};

#endif
