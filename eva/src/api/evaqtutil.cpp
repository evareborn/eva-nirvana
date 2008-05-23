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

#include "evaqtutil.h"
#include "evautil.h"
#include "evausersetting.h"
#include "evamemo.h"
#include <qtextstream.h>
#include <qimage.h>
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qapplication.h>
#include <cstring>

const QString EvaTextFilter::filter( const QString & source )
{
	QChar ch;
	QString result;
	QTextStream stream(&result, IO_WriteOnly);
	for(uint i=0; i< source.length(); i++){
		ch = source.at(i);
		if(ch.isPrint())
			stream << ch;
	}
	return result;
} 
 
/* ***********************************************************/

QMutex EvaHelper::mutex;
void EvaHelper::run()
{
	mutex.lock();
	switch(type){
	case LoadGroupedUsers:
		doGroupedUserLoading();
		break;
	case SaveGroupedUsers:
		doGroupedUserSaving();
		break;
	case LoadQunUsers:
		doQunUserLoading();
		break;
	case SaveQunUsers:
		doQunUserSaving();
	case LoadSys:
		doSysLoading();
		break;
	case ScaleImage:
		doImageScaling();
	}
	mutex.unlock();
}

void EvaHelper::doSysLoading( )
{
}

void EvaHelper::doImageScaling( )
{
	QImage img(pic.convertToImage().smoothScale(size));
	pic = img;
	EvaScaleEvent *event = new EvaScaleEvent(pic);
	QApplication::postEvent(receiver, event);
}

void EvaHelper::setLoadGroupedUsersArgs( QFile * file )
{
	this->file = file;
}

void EvaHelper::doGroupedUserLoading( )
{
	if(type != LoadGroupedUsers) return;
	if(!file->isOpen()) return;
	std::list<std::string> groupNames;
	ContactInfo myInfo;
	FriendList list;
	
	Q_UINT32 numGroups=0;

	MemoItem memo;

	QDataStream stream(file);
	
	// check version first
	char *flag = new char[3];
	stream.readRawBytes(flag, 3);
	Q_UINT32 version = 0;
	stream>>version;
	if(!(flag[0]=='E' && flag[1]=='V' && flag[2]=='A' && version == profileVersion)){
		file->close();
		file->remove();
		delete flag;
		return;
	}
	delete []flag;
	
	// load my details first
	Q_UINT32 size=0;
	std::string item;
	std::vector<std::string> strlist;
	
	stream>>size;
	char *str = new char[512];
	for(uint i=0; i<size; i++){
		stream>>str;
		item = str;
		strlist.push_back(item);
	}	
	myInfo.setDetails(strlist);

	// read my extra info
	Q_UINT16 myExtraInfo;
	stream>>myExtraInfo;
	
	// read signature & time
	std::string signature;
	stream>>str; 
	signature = str;
	
	Q_UINT32 sigTime;
	stream>>sigTime;
	
	// read in how many groups
	stream>>numGroups;
	std::string name;
	// read all groups in
	for(uint i=0; i<numGroups; i++){
		stream>>str;
		name = str;
		groupNames.push_back(name);
	}
	
	Q_UINT32 id;
	Q_UINT16 face;
	Q_UINT8  age;
	Q_UINT8  gender;
	std::string nick;
	Q_UINT8  extFlag;
	Q_UINT8  commonFlag;
	Q_UINT32 groupIndex;
	Q_UINT16 extraInfo;
	std::string frdSig;
	Q_UINT32 frdSigTime;
	Q_UINT32 fontSize;
	Q_UINT32 fontColor;
	
	// read in all friends
	while(!stream.atEnd()){
		stream>>id;
		stream>>face;
		stream>>age;
		stream>>gender;
		stream>>str;
		nick = str;
		stream>>extFlag;
		stream>>commonFlag;
		stream>>groupIndex;
		stream>>extraInfo;
		stream>>str;
		frdSig = str;
		stream>>frdSigTime;
		
		strlist.clear();
		stream>>size;
		for(uint i=0; i<size; i++){
			stream>>str;
			item = str;
			strlist.push_back(item);
		}
		stream>>str;memo.name = str;
		stream>>str;memo.mobile = str;
		stream>>str;memo.telephone = str;
		stream>>str;memo.address = str;
		stream>>str;memo.email = str;
		stream>>str;memo.zipcode = str;
		stream>>str;memo.note = str;

		stream>>fontSize>>fontColor;
		QQFriend f(id, face);
		
		f.setAge(age);
		f.setGender(gender);
		f.setNick(nick);
		f.setExtFlag(extFlag);
		f.setCommonFlag(commonFlag);
		f.setGroupIndex(groupIndex);
		f.setExtraInfo(extraInfo);
		f.setSignature(frdSig, frdSigTime);
		ContactInfo info;
		info.setDetails(strlist);
		f.setUserInformation(info);
		f.setMemo(memo);
		f.setChatFontSize(fontSize);
		f.setChatFontColor(fontColor);
		
		list.addFriend(f);
	}
	delete str;
	file->close();
	EvaBuddyListEvent *event = new EvaBuddyListEvent();
	event->setGroupNames(groupNames);
	event->setDetails(myInfo);
	event->setFriendList(list);
	event->setExtraInfo(myExtraInfo);
	event->setSignature(signature, sigTime);
	QApplication::postEvent(receiver, event);
}

void EvaHelper::setSaveGroupedUsersArgs( QFile * file, std::list<std::string> myGroups, ContactInfo &info, FriendList &myList,
					unsigned short extraInfo, std::string signature, unsigned int sigModiTime)
{
	this->file = file;
	groups = myGroups;
	myInfo = info;
	list = myList;
	mExtraInfo = extraInfo;
	mSignature = signature;
	mSigModiTime = sigModiTime;
}

void EvaHelper::doGroupedUserSaving()
{
	if(type != SaveGroupedUsers) return;
	if(!file->isOpen()) return;
	QDataStream stream(file);

	
	// save Eva flag and Version
	stream.writeRawBytes("EVA", 3);
	stream<<profileVersion;
	
	std::string detailItem;
	
	// save my info first
	Q_UINT32 myCount = myInfo.count();
	stream<<myCount;
	for(int i=0; i<(int)myCount; i++){
		detailItem = myInfo.at(i);
		stream<<detailItem.c_str();
	}	
	
	// save my extra info
	stream<<mExtraInfo;
	
	// save my signature
	stream<<mSignature.c_str()<< mSigModiTime;
		
	// save group names
	std::list<std::string>::iterator itr;
	// save the number of customized groups (including buddy list)
	Q_UINT32 size = groups.size();
	stream<<size;
	for(itr=groups.begin(); itr!=groups.end(); ++itr){
		stream<<itr->c_str();
	}
	
	std::list<QQFriend> flist = list.getAllFriends();
	std::list<QQFriend>::iterator iter;
	Q_UINT32 id;
	Q_UINT16  face;
	Q_UINT8  age;
	Q_UINT8  gender;
	std::string nick;
	Q_UINT8  extFlag;
	Q_UINT8  commonFlag;
	Q_UINT32 groupIndex;
	Q_UINT16 bExtraInfo;
	std::string signature;
	Q_UINT32 sigModiTime;
	ContactInfo userDetails;
	MemoItem memo;
	Q_UINT32 fontSize;
	Q_UINT32 fontColor;
	
	// save all buddy details
	for(iter = flist.begin(); iter!= flist.end(); ++iter){
		id = iter->getQQ();
		face = iter->getFace();
		age = iter->getAge();
		gender = iter->getGender();
		nick = iter->getNick();
		extFlag = iter->getExtFlag();
		commonFlag = iter->getCommonFlag();
		groupIndex = iter->getGroupIndex();
		bExtraInfo = iter->getExtraInfo();
		signature = iter->getSignature();
		sigModiTime = iter->getSignatureModifyTime();
		userDetails = iter->getUserInformation();
		
		stream<<id<<face<<age<<gender<<nick.c_str()<<extFlag<<commonFlag<<groupIndex<<bExtraInfo<<signature.c_str()<<sigModiTime;
				
		Q_UINT32 count = userDetails.count();
		stream<<count;
		for(int i=0; i<(int)count; i++){
			detailItem = userDetails.at(i);
			stream<<detailItem.c_str();
		}
		memo = iter->getMemo();
		stream<<memo.name.c_str()<<memo.mobile.c_str()<<memo.telephone.c_str();
		stream<<memo.address.c_str()<<memo.email.c_str()<<memo.zipcode.c_str()<<memo.note.c_str();

		fontSize = iter->getChatFontSize();
		fontColor = iter->getChatFontColor();
		stream << fontSize << fontColor;
	}
	file->flush();
	file->close();
}

void EvaHelper::setSaveQunListArgs( QFile * file, QunList & list )
{
	this->file = file;
	qunList = list;
}

void EvaHelper::doQunUserSaving( )
{
	if(type != SaveQunUsers) return;
	if(!file->isOpen()) return;
	QDataStream stream(file);
	
	// save Eva flag and Version
	stream.writeRawBytes("EVA", 3);
	stream<<profileVersion;

	std::list<Qun>::iterator qunIter;
	std::list<Qun>list = qunList.getQunList();
	
	QunInfo info;
	std::list<FriendItem>::iterator memberIter;
	std::list<FriendItem> memberList;
	
	for(qunIter = list.begin(); qunIter!= list.end(); ++qunIter){
		// save qun info
		info = qunIter->getDetails();
		
		Q_UINT32 qunID = qunIter->getQunID();
		Q_UINT32 extID = info.getExtID();
		Q_UINT8 type = info.getType();
		Q_UINT32 creator = info.getCreator();
		Q_UINT8 authType = info.getAuthType();
		Q_UINT16 unknown1 = info.getUnknown1();
		Q_UINT16 category = info.getCategory();
		Q_UINT32 versionID = info.getVersionID();
		std::string name = info.getName();
		Q_UINT16 unknown2 = info.getUnknown2();
		std::string description = info.getDescription();
		std::string notice = info.getNotice();
		
		stream<<qunID<<extID<<type<<creator<<authType<<unknown1<<category<<
			versionID<<name.c_str()<<unknown2<<description.c_str()<<notice.c_str();
		
		Q_UINT32 fontSize = qunIter->getChatFontSize();
		Q_UINT32 fontColor = qunIter->getChatFontColor();
		stream << fontSize << fontColor;

		// save message type
		Q_UINT8 msgType = qunIter->getMessageType();
		stream<<msgType;

		Q_UINT32 nameVersion = qunIter->getRealNamesVersion();
		stream<< nameVersion;

		// save my qun card
		name = qunIter->getCardName();
		Q_UINT8 gender = qunIter->getCardGender();
		std::string phone = qunIter->getCardPhone();
		std::string email = qunIter->getCardEmail();
		std::string memo = qunIter->getCardMemo();
		stream<<name.c_str()<<gender<<phone.c_str()<<email.c_str()<<memo.c_str();
		
		// save all members
		memberList = qunIter->getMembers();
		Q_UINT16 size = memberList.size();
		stream<<size;
		for(memberIter=memberList.begin(); memberIter!=memberList.end(); ++memberIter){
			Q_UINT32 qqNum = memberIter->getQQ();
			Q_UINT16 face = memberIter->getFace();
			Q_UINT8 age = memberIter->getAge();
			Q_UINT8 gender = memberIter->getGender();
			std::string nick = memberIter->getNick();
			Q_UINT8 extFlag = memberIter->getExtFlag();  
			Q_UINT8 commonFlag = memberIter->getCommonFlag();
			Q_UINT16 qunGroupIndex = memberIter->getQunGroupIndex();
			Q_UINT16 qunAdminValue = memberIter->getQunAdminValue();
			// added by henry
			std::string realName = memberIter->getQunRealName();
			stream<<qqNum<<face<<age<<gender<<nick.c_str()<<extFlag<<commonFlag<<qunGroupIndex<<qunAdminValue<<realName.c_str();
			
		}
	}
	file->flush();
	file->close();
}

void EvaHelper::setLoadQunListArgs( QFile * file )
{
	this->file = file;
}

void EvaHelper::doQunUserLoading( )
{
	if(type != LoadQunUsers) return;
	if(!file->isOpen()) return;
	
	QDataStream stream(file);

	///FIXME we should do version checking/saving in a seperate method, which 
	/// could benifit buddy list save/load as well, I am just too lazy :)
	// check version first
	char *flag = new char[3];
	stream.readRawBytes(flag, 3);
	Q_UINT32 version = 0;
	stream>>version;
	if(!(flag[0]=='E' && flag[1]=='V' && flag[2]=='A' && version == profileVersion)){
		file->close();
		file->remove();
		delete flag;
		return;
	}
	delete flag;
	
	QunList list;
	
	Q_UINT32 qunID;
	Q_UINT32 extID;
	Q_UINT8 type;
	Q_UINT32 creator;
	Q_UINT8 authType;
	Q_UINT16 unknown1;
	Q_UINT16 category;
	Q_UINT32 versionID;
	std::string name;
	Q_UINT16 unknown2;
	std::string description;
	std::string notice;
	Q_UINT32 realNamesVersion;

	Q_UINT32 fontSize;
	Q_UINT32 fontColor;
	
	Q_UINT8 cardGender;
	Q_UINT8 msgType;
	char *str = new char[1024];
	memset(str, 0, 1024);
	
	while(!stream.atEnd()){
		// load qun basic info
		stream>>qunID>>extID>>type>>creator>>authType>>unknown1>>category>>versionID>>str;
		name = str;
		stream>>unknown2>>str;
		description = str;
		stream>>str;
		notice = str;
		QunInfo info;
		info.setQunID(qunID);
		info.setExtID(extID);
		info.setType(type);
		info.setCreator(creator);
		info.setAuthType(authType);
		info.setUnknown1(unknown1);
		info.setCategory(category);
		info.setVersionID(versionID);
		info.setName(name);
		info.setUnknown2(unknown2);
		info.setDescription(description);
		info.setNotice(notice);
		
		Qun qun(info.getQunID());
		qun.setDetails(info);

		stream >> fontSize >> fontColor;	
		qun.setChatFontSize(fontSize);
		qun.setChatFontColor(fontColor);

		// load message type
		stream>>msgType;
		qun.setMessageType((Qun::MessageType)msgType);
		
		stream >> realNamesVersion;
		qun.setRealNamesVersion(realNamesVersion);
		// load my card for this qun
		stream>>str;
		qun.setCardName(str);
		stream>>cardGender;
		qun.setCardGender(cardGender);
		stream>>str;
		qun.setCardPhone(str);
		stream>>str;
		qun.setCardEmail(str);
		stream>>str;
		qun.setCardMemo(str);
		
		// load all members details
		std::list<FriendItem> members;
		Q_UINT16 size;
		stream >> size;
		for(int i=0; i< size; i++){
			Q_UINT32 qqNum;
			Q_UINT16 face;
			Q_UINT8 age;
			Q_UINT8 gender;
			std::string nick;
			Q_UINT8 extFlag;  
			Q_UINT8 commonFlag;
			Q_UINT16 qunGroupIndex;
			Q_UINT16 qunAdminValue;
			std::string realName;   // added by henry
			
			stream>>qqNum>>face>>age>>gender>>str;
			nick = str;
			stream>>extFlag>>commonFlag>>qunGroupIndex>>qunAdminValue>>str;
			realName = str;
			FriendItem item;
			item.setQQ(qqNum);
			item.setFace(face);
			item.setAge(age);
			item.setGender(gender);
			item.setNick(nick);
			item.setExtFlag(extFlag);
			item.setCommonFlag(commonFlag);
			item.setQunGroupIndex(qunGroupIndex);
			item.setQunAdminValue(qunAdminValue);
			item.setQunRealName( realName); // henry
			members.push_back(item);
		}
		qun.setMembers(members);
		list.add(qun);
	}
	delete str;
	file->close();
	EvaQunListEvent *event = new EvaQunListEvent();
	event->setQunList(list);
	QApplication::postEvent(receiver, event);
}

const QString EvaHelper::generateCustomSmiley( const QString & source, const QString &destDir, const bool withThumbnail)
{
	QString fileName = source;
	QString imageName = fileName.right(fileName.length() - fileName.findRev("/") -1);
	QString destExt = imageName.right(imageName.length() - (imageName.findRev(".")+1));
	
	if(destExt == "png" || destExt == "PNG"){
		QPixmap srcImage;
		if(!srcImage.load(fileName)){
			printf("EvaHelper: cannot load png image\n");
			return "";
		}
		if(!srcImage.save(destDir + "/" + imageName, "JPEG", 50)){
			printf("EvaHelper: convert to JPG format failed\n");
			return "";
		}
		fileName = destDir + "/" + imageName;
	}
	
	char *md5 = new char[16];
	if(!getFileMD5(fileName, md5)){
		delete md5;
		return "";
	}
	QString strMD5 = md5ToString(md5);
	
	if(destExt == "png" || destExt == "PNG"){
		destExt = "JPG";
		if(!rename(fileName, destDir + "/" + strMD5 +"." + destExt)){
			delete md5;
			return "";
		}
	}else{
		if(!copyFile(fileName, destDir + "/" + strMD5 +"." + destExt)){
			delete md5;
			return "";
		}
	}
	delete md5;
	if(withThumbnail){
		QImage srcpix;

		if(!srcpix.load(fileName)){
			printf("EvaHelper: cannot load image\n");
			return "";
		}
		QImage thumbpix = srcpix.smoothScale(20, 20);
		QString destname = destDir + "/" + strMD5 + "fixed.bmp";
		if(!thumbpix.save(destname, "BMP")){
			printf("EvaHelper: convert to thumbnail BMP format failed\n");
			return "";
		}	
	}
	return strMD5 +"." + destExt;
}

const bool EvaHelper::getFileMD5( const QString & fileName, char *md5)
{
	if(!md5) return false;
	QFileInfo info(fileName);
	if(!info.exists()) return false;
	
	unsigned int len = info.size();
	char *buf = new char[len];
	QFile file(fileName);
	if(!file.open(IO_ReadOnly)){
		printf("EvaHelper:image dose not exists!");
		delete buf;
		return false;
	}
	unsigned int numRead = file.readBlock(buf, len);
	file.close();
	if(numRead!=len){
		delete buf;
		return false;
	}
	memcpy(md5, EvaUtil::doMd5(buf, len), 16);
	delete buf;
	return true;
}

const QString EvaHelper::md5ToString(const char *md5)
{
	if(!md5) return "";
	QString strMd5;
	for(int i=0; i<16; i++){
		QString tmp = QString::number((unsigned char)(md5[i]), 16);
		if(tmp.length()==1)
			tmp = "0" + tmp;
		strMd5+=tmp.upper();
	}
	return strMd5;
}

const bool EvaHelper::copyFile( const QString & source, const QString & dest )
{
	QFileInfo info(source);
	if(!info.exists()) return false;
	unsigned int len = info.size();
	char *buf = new char[len];
	QFile file(source);
	if(!file.open(IO_ReadOnly)){
		printf("EvaHelper:image dose not exists!");
		delete buf;
		return false;
	}
	unsigned int numRead = file.readBlock(buf, len);
	file.close();
	if(numRead!=len){
		delete buf;
		return false;
	}
	
	QFile destFile(dest);
	if(!destFile.exists()){
		if(!destFile.open(IO_WriteOnly | IO_Raw)){
			printf("EvaHelper:cannot copy image file!\n");
			delete buf;
			return false;
		}
		if(destFile.writeBlock(buf, len) != (int)len){
			destFile.close();
			destFile.remove();
			printf("EvaHelper:bytes copied wrong!\n");
			delete buf;
			return false;
			
		}else
			destFile.close();
	}
	return true;
}

const bool EvaHelper::rename( const QString & source, const QString & dest )
{
	QDir destFile(source);
	if(!destFile.exists(source)){
		printf("EvaHelper: rename file failed\n");
		return false;
	}
	destFile.rename(source, dest);
	destFile.remove(source);
	return true;
}


/**=================================================================================================*/

QRgb EvaQtUtils::toGray(QRgb rgb)
{
	int gray = (212671 * qRed (rgb) + 715160 * qGreen (rgb) + 72169 * qBlue (rgb)) / 1000000;
	return qRgba (gray, gray, gray, qAlpha (rgb));
}

QPixmap EvaQtUtils::convertToGrayscale(const QPixmap &pm)
{
	QImage image = QImage();
	if (!pm.isNull ())
		image = pm.convertToImage ();
	convertToGrayscale (&image);
	QPixmap destPixmap;
	destPixmap.convertFromImage(image,
				Qt::ColorOnly/*always display depth*/ |
				Qt::DiffuseDither/*hi quality dither*/ |
				Qt::ThresholdAlphaDither/*no dither alpha*/ |
				Qt::PreferDither/*(dither even if <256 colours)*/);
	return destPixmap;
}

void EvaQtUtils::convertToGrayscale (QImage *destImagePtr)
{
    if (destImagePtr->depth () > 8)
    {
        for (int y = 0; y < destImagePtr->height (); y++)
        {
            for (int x = 0; x < destImagePtr->width (); x++)
            {
                destImagePtr->setPixel (x, y, toGray (destImagePtr->pixel (x, y)));
            }
        }
    }
    else
    {
        // 1- & 8- bit images use a color table
        for (int i = 0; i < destImagePtr->numColors (); i++)
            destImagePtr->setColor (i, toGray (destImagePtr->color (i)));
    }
}
