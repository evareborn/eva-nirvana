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
 
#ifndef EVAQTUTIL_H 
#define EVAQTUTIL_H

#include "evafriendlist.h"
#include "evaqunlist.h"
#include "evahtmlparser.h"

#include <string>
#include <qobject.h>
#include <qthread.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qsize.h>
#include <qevent.h>
#include <qmutex.h>

#define EvaLoadGroupedUsersEvent  65535
#define EvaLoadQunUsersEvent      65534
#define EvaLoadSysEvent           65533
#define EvaScaleImageEvent        65532
#define EvaRequestCustomizedPicEvent 65531
#define EvaPictureReadyEvent      65530
#define EvaSendPictureReadyEvent  65529
#define EvaUserHeadReadyEvent     65528
#define EvaEventNotify            65527

const Q_UINT32 profileVersion = 0x000400;

class QFile;

// note that this static method is to filer those control chars out and change some printable funny chars into spaces
class EvaTextFilter 
{
public:
	static const QString filter(const QString &str);
};

class EvaSysEvent : public QCustomEvent
{
public:
	EvaSysEvent() : 
		QCustomEvent(EvaLoadSysEvent){};
};

class EvaScaleEvent : public QCustomEvent
{
public:
	EvaScaleEvent(const QPixmap p) : 
		QCustomEvent(EvaScaleImageEvent), pic(p) {}
	QPixmap pixmap() { return pic; }
private:
	QPixmap pic;
};

class EvaBuddyListEvent : public QCustomEvent
{
public:
	EvaBuddyListEvent() : QCustomEvent(EvaLoadGroupedUsersEvent) {};
	
	void setGroupNames(const std::list<std::string>& names) { groupNames = names; }
	void setDetails(const ContactInfo &info) { myInfo = info; }
	void setFriendList(const FriendList &l) { list = l; }
	void setExtraInfo(const unsigned short info) { mExtraInfo = info; }
	void setSignature(const std::string sig, const unsigned int time) { mSignature = sig; mSigModiTime = time; }
	
	const std::list<std::string> getGroupNames() const { return groupNames; }
	const ContactInfo getMyInfo() const { return myInfo; }
	const FriendList getMyFriendList() const { return list; }
	const unsigned long long getExtraInfo() const { return mExtraInfo; }
	const std::string getSignature() const { return mSignature;}
	const unsigned int getSignatureTime() const { return mSigModiTime;}
private:
	std::list<std::string> groupNames;
	ContactInfo myInfo;
	FriendList list;
	unsigned long long mExtraInfo;
	std::string mSignature;
	unsigned int mSigModiTime;
};

class EvaQunListEvent : public QCustomEvent
{
public:
	EvaQunListEvent() : QCustomEvent(EvaLoadQunUsersEvent) {};
	void setQunList( const QunList &l) { list = l; }
	QunList getQunList() { return list; }
private:
	QunList list;
};

class EvaAskForCustomizedPicEvent : public QCustomEvent
{
public:
	EvaAskForCustomizedPicEvent() : QCustomEvent(EvaRequestCustomizedPicEvent) {};
	void setPicList( const std::list<CustomizedPic> &list) { picList = list; }
	std::list<CustomizedPic> getPicList() { return picList; }
	void setQunID(const unsigned int id) { qunID = id; }
	const unsigned int getQunID() const { return qunID; }
private:
	std::list<CustomizedPic> picList;
	unsigned int qunID;
};

class EvaSendCustomizedPicEvent : public QCustomEvent
{
public:
	EvaSendCustomizedPicEvent() : QCustomEvent(EvaSendPictureReadyEvent) {};
	void setPicList( const std::list<OutCustomizedPic> &list) { picList = list; }
	std::list<OutCustomizedPic> getPicList() { return picList; }
	void setQunID(const unsigned int id) { qunID = id; }
	const unsigned int getQunID() const { return qunID; }
private:
	std::list<OutCustomizedPic> picList;
	unsigned int qunID;
};

class EvaPicReadyEvent : public QCustomEvent
{
public:
	EvaPicReadyEvent() : QCustomEvent(EvaPictureReadyEvent) {};
	void setFileName( const QString &name) { fileName = name; }
	QString getFileName() { return fileName; }
	
	void setTmpFileName( const QString &name) { tmpFileName = name; }
	QString getTmpFileName() { return tmpFileName; }
	
	void setQunID(const unsigned int id) { qunID = id; }
	const unsigned int getQunID() const { return qunID; }
private:
	unsigned int qunID;
	QString fileName;
	QString tmpFileName;
};

class EvaUHReadyEvent : public QCustomEvent
{
public:
	EvaUHReadyEvent() : QCustomEvent(EvaUserHeadReadyEvent) {};
	
	void setQQ( const unsigned int id) { mId = id; }
	void setImages( const QImage on, const QImage off) { imgOn = on; imgOff = off; }
	
	const unsigned int getQQ() { return mId; }
	const QImage getOnImage() const { return imgOn; }
	const QImage getOffImage() const { return imgOff; }
private:
	unsigned int mId;
	QImage imgOn;
	QImage imgOff;
};

typedef unsigned int EPARAM;

class EvaNotifyEvent : public QCustomEvent
{
	public:
		EvaNotifyEvent(int event) 
				: QCustomEvent(EvaEventNotify),
				m_id(event) 
		{};
		int m_id;
		QString m_desc;
		EPARAM m_param;
};

class EvaContactManager;
class EvaHelper : public QThread
{
public:
	enum Type{ LoadGroupedUsers, SaveGroupedUsers, LoadQunUsers, SaveQunUsers, LoadSys, ScaleImage };
	void setCategory(const Type t, QObject *rec) { type = t; receiver = rec; }
	
	// for loading grouped users list
	void setLoadGroupedUsersArgs(QFile *file) ;
	
	// for saving grouped users list
	void setSaveGroupedUsersArgs(QFile *file, std::list<std::string> myGroups, ContactInfo &info, FriendList &myList,
					unsigned short extraInfo, std::string signature, unsigned int sigModiTime);
	
	// for loading Qun list
	void setLoadQunListArgs(QFile *file);
	
	// for saving Qun list
	void setSaveQunListArgs(QFile *file, QunList &list);
	
	// for image scaling
	void setScaleArgs(const QPixmap &p, const QSize &s) { pic = p; size = s; }
	
	// static method for generating customized smiley file into user's customCaches directory, should be the absolute path
	static const QString generateCustomSmiley(const QString &source, const QString &destDir, const bool withThumbnail = false);
	
	// static method for calculating md5 of a file, should be the absolute path. note: char *md5 must be allocated before calling this method
	static const bool getFileMD5(const QString &fileName, char *md5);
	
	// convert MD5 into string expression, all converted string are in upper case
	static const QString md5ToString(const char *md5);
	
	// static method for copying source to dest directory, should be the absolute path
	static const bool copyFile(const QString &source, const QString &dest);
	
	// static method for renaming the source file to dest file name, should be the absolute path, note that: this method will delete source file
	static const bool rename(const QString &source, const QString &dest);

	// starting the thread
	virtual void run();
private:
	static QMutex mutex;
	Type type;
	QObject *receiver;
	
	// for user loading/saving buddy list  use only
	QFile *file;
	
	// for user saving only
	std::list< std::string > groups;
	ContactInfo myInfo;
	FriendList list;
	Q_UINT16 mExtraInfo;
	std::string mSignature;
	int mSigModiTime;
	
	// for qun list saving only
	QunList qunList;
	
	
	// for scale the image only
	QPixmap pic;
	QSize size;

	
	void doGroupedUserLoading();
	void doGroupedUserSaving();
	void doQunUserLoading();
	void doQunUserSaving();
	void doSysLoading();
	void doImageScaling();
	
	friend class EvaContactManager;
};

// return QString
#define GB2Unicode( gb_char ) (QTextCodec::codecForName("GB18030")->toUnicode( gb_char ))
// return char *
#define Unicode2GB( qstring) (QTextCodec::codecForName("GB18030")->fromUnicode( qstring).data())
		
class EvaQtUtils {
public:
	static QRgb toGray(QRgb rgb);
	// Warning: this can only be used in GUI thread
	static QPixmap convertToGrayscale(const QPixmap &pm);
	// This method is thread-safe method
	static void convertToGrayscale (QImage *destImagePtr);
};

#endif
