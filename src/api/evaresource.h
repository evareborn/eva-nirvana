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

#include <QObject>
#include <qfile.h>
#include <qmap.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qmovie.h>
#include <qsize.h>
 
#include <QProcess>

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

	EvaImageResource(const QString& imageRoot);
	~EvaImageResource();
	
	
	 const QString getFacePath() const;
	 const QString getIconPath() ;
	 const QString getSmileyPath() const;
	 const QString getQQShowPath() const;

         const QSize getFaceSize() const;
         void setFaceSize( const QSize& size);

	 int getFaceID(const int fileIndex) const ;
	 int getFaceFileIndex(const int faceId);
	QPixmap *getFace(const int fileIndex, const bool on = true);
	QPixmap *getFaceByID( const unsigned short faceId, const bool on = true);
	QPixmap *getIcon(QString name);
	 const QString getIconFullPath(QString name);
	 const QString getSmiley(const int fileIndex);
	
	const QMovie *getLoginMovie();
	
	 bool loadImage();
	const QString &getImageRootPath() const { return imageRoot; }
	void setImageRootPath( QString &path) { imageRoot = path; }
	
	 QString getThemePath() const { return themePath; }
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
	QMap<QString, QMovie*> smileyList;
	QMap<int, QPixmap> imgOnList;
	QMap<int, QPixmap> imgOffList;
	QMovie *loginMng;
	int faceId[MaxFaceNumber];  // store faceID, index is the fileID
	QString imageRoot;
	QString themePath;
	QSize faceSize;
	
	bool isDownloadingQQShow;
	int downloadID;
	QFile qqshowFile;
	QString qqshowFilename;
	EvaHttp *http;
 
        QPixmap* BAD_ICON;
	
private slots:
	void slotQQShowDone(bool error);

private:
	friend class EvaGlobal;
};

class EvaSoundResource
{
public:
	EvaSoundResource(const QString& soundRoot);
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
 
        static EvaGlobal* getInstance() { return instance; }
        static QString getDirPath();
	
//X 	 bool loadImage();
        //const bool loadFace();
	//const bool loadSound();
	 bool loadEvaSetting();
	
         EvaSetting *getEvaSetting() { return system; }
         EvaServers *getEvaServers() { return servers; }

         const QSize getFaceSize() const;
         void setFaceSize( const QSize& size);
	
//X  	EvaImageResource *getImageResource() { return imgResource;}
	EvaSoundResource *getSoundResource() { return sndResource;}
private:
	void initialize();
	void initImage();
	void initSound();
	void initEvaSetting();
	void initServers();
 
        static EvaGlobal* instance;
	
	QString dirPath;
	EvaSetting *system;
	EvaServers *servers;
//X 	EvaImageResource *imgResource;
	EvaSoundResource *sndResource;
};

#endif
