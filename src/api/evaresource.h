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

#ifndef EVARESOURCE_H 
#define EVARESOURCE_H

#include <qobject.h>
#include <qfile.h>
#include <qmap.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qmovie.h>
#include <qsize.h>

#ifndef MaxFaceNumber
//#define MaxFaceNumber 117
#define MaxFaceNumber 134
#endif

class EvaSetting;
class EvaServers;
class EvaHttp;
class EvaImageResource : public QObject
{
	Q_OBJECT
public:
	EvaImageResource();
	~EvaImageResource();
	
	
	const QString getFacePath() const;
	const QString getIconPath() ;
	const QString getSmileyPath() const;
	const QString getQQShowPath() const;
	
	const int getFaceID(const int fileIndex) const ;
	const int getFaceFileIndex(const int faceId);
	QPixmap *getFace(const int fileIndex, const bool on = true);
	QPixmap *getFaceByID( const unsigned short faceId, const bool on = true);
	QPixmap *getIcon(QString name);
	const QString getIconFullPath(QString name);
	const QString getSmiley(const int fileIndex);
	
	const QMovie *getLoginMovie();
	
	const bool loadImage();
	const QString &getImageRootPath() const { return imageRoot; }
	void setImageRootPath( QString &path) { imageRoot = path; }
	
	const QString getThemePath() const { return themePath; }
	void setThemePath( const QString &path) { themePath = path; }

	QPixmap *getQQShow(const unsigned int id);
	void setUserHeadImage(QMap<unsigned int, QImage> imageOnList, QMap<unsigned int, QImage> imageOffList);
	void addUserHeadImage(const unsigned int id, QImage imgOn, QImage imgOff);
	QPixmap *getUserHeadPixmap(const unsigned int id, bool isGrayscale = false);
signals:
	void qqShowReady(const unsigned int);
public slots:	
	void requestQQShow(const unsigned int id);	
private:
	bool loadFace(const QSize &size);
	bool loadIcon();
	bool loadSmiley();
	QMap<QString, QPixmap> faceList;
	QMap<QString, QPixmap> iconList;
	QMap<QString, QString> iconFileNameMap;
	QMap<QString, QMovie> smileyList;
	QMap<int, QPixmap> imgOnList;
	QMap<int, QPixmap> imgOffList;
	QMovie loginMng;
	int faceId[MaxFaceNumber];  // store faceID, index is the fileID
	QString imageRoot;
	QString themePath;
	QSize faceSize;
	
	bool isDownloadingQQShow;
	int downloadID;
	QFile qqshowFile;
	QString qqshowFilename;
	EvaHttp *http;
	
private slots:
	void slotQQShowDone(bool error);

private:
	friend class EvaGlobal;
};

class EvaSoundResource
{
public:
	EvaSoundResource();
	~EvaSoundResource(){}
	//const bool loadSound(){ return false; }
	void playNewMessage();
	void playSysMessage();
	void playOnlineSound();
	void setSoundDir(const QString &path) { soundRoot = path; }
	const QString &getSoundDir() const { return soundRoot; }
private:
	void playSound(const QString &filename);
	QString soundRoot;
};

class EvaGlobal
{
public:
	EvaGlobal();
	virtual ~EvaGlobal();
	
	static QString &getDirPath();
	const bool loadImage();
        //const bool loadFace();
	//const bool loadSound();
	const bool loadEvaSetting();
	
	EvaImageResource *getImageResource() { return imgResource;}
	EvaSoundResource *getSoundResource() { return sndResource;}
	EvaSetting *getEvaSetting() { return system; }
	EvaServers *getEvaServers() { return servers; }
	
	const QSize &getFaceSize() const;
	void setFaceSize( const QSize size);
private:
	void initialize();
	void initImage();
	void initSound();
	void initEvaSetting();
	void initServers();
	
	static QString dirPath;
	EvaSetting *system;
	EvaServers *servers;
	EvaImageResource *imgResource;
	EvaSoundResource *sndResource;
	QSize faceSize;
};

#endif
