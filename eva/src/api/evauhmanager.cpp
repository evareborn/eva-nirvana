/***************************************************************************
 *   Copyright (C) 2004-2005 by yunfan                                          *
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

#include "evauhmanager.h"
#include "evauhpacket.h"
#include "evauhprotocols.h"
#include "evamain.h"
#include "evaresource.h"
#include <qfile.h>
#include <qdatastream.h>
#include <qtextstream.h>
#include <qsocketdevice.h>
#include <qdns.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <string.h>

#define MaxBlockSize       800

#define CFACE_SERVER       "cface_tms.qq.com"
#define CFACE_PORT         4000

#define UH_PROFILE_NAME    "evauh.profile"

#define UH_TIME_OUT        30
#define UH_MAX_RETRY_COUNT 4

// UH represents User Head which is used in Tencent QQ, sounds pretty @!#~*!$@# :)
class EvaUHFile {
public:
	EvaUHFile(unsigned int id);
	~EvaUHFile();
	const bool setFileInfo(const char *md5, unsigned int sid);
	const bool setFileBlock(const unsigned int numPackets, const unsigned int packetNum, 
			const unsigned int fileSize, const unsigned int partStart, 
			const unsigned int partSize, const unsigned char *buf);
	
	const QString getMd5String() const;
	const unsigned int getQQ() const { return mId; }
	const int getSessionId() const { return mSessionId; }
	
	// is this file downloaded sucessfully?
	const bool isFinished();
	// return session id, and assign the start and size, return 0 means md5 wrong, 
	// need redownloading this file again, or no session available
	const unsigned int nextBlock(unsigned int *start, unsigned int *size);
	
	const bool isMD5Correct();
	void save(QString &dir);
	void initSettings();
private:
	unsigned int mId;
	char md5[16];
	bool *flags;
	unsigned int mFileSize;  // total size of this file
	unsigned int mNumPackets;   // number of blocks this file needs
	unsigned int mSessionId;   // the session id used in downloading
	unsigned char *mBuffer;  // the file data
};


EvaUHFile::EvaUHFile(unsigned int id)
	: mId(id), flags(NULL), mFileSize(0), mNumPackets(0), mSessionId(0), mBuffer(NULL)
{
}

EvaUHFile::~EvaUHFile()
{
	if(flags) delete flags;
	if(mBuffer) delete mBuffer;
}

const bool EvaUHFile::setFileInfo(const char *_md5, unsigned int sid)
{
	if(mSessionId) return false;
	memcpy(md5, _md5, 16);
	mSessionId = sid;
	return true;
}

const bool EvaUHFile::setFileBlock(const unsigned int numPackets, const unsigned int /*packetNum*/, 
			const unsigned int fileSize, const unsigned int partStart, 
			const unsigned int partSize, const unsigned char *buf)
{
	//if(packetNum >= numPackets) return false; // impossible
	if(mFileSize && fileSize != mFileSize) return false;  // different file size
	//if(mNumPackets && numPackets != mNumPackets) return false; // num of blocks wrong
	//if(flags && flags[packetNum] ) return false; // got this block already
	
	if(!mFileSize) mFileSize = fileSize;
	if(!mNumPackets) mNumPackets = numPackets;
	if(!flags){ 
		flags = new bool[mNumPackets];
		for(unsigned int i=0; i<mNumPackets; i++)
			flags[i] = false;
	}
	
	if(!mBuffer){ 
		mBuffer = new unsigned char[mFileSize];
		memset(mBuffer, 0, mFileSize);
	}
	memcpy(mBuffer+partStart, buf, partSize);
	//flags[packetNum] = true;
	flags[partStart/MaxBlockSize] = true;
	return true;
}

const QString EvaUHFile::getMd5String() const
{
	return EvaHelper::md5ToString(md5);
}

const bool EvaUHFile::isFinished()
{
	if(!flags) return false;
	bool result = true;
	for(unsigned int i=0; i< mNumPackets; i++){
		if(!flags[i]) {
			result = false;
			break;
		}
	}
	return result;
}

// return session id, and assign the start and size, return 0 means md5 wrong, 
// need redownloading this file again, no session available
const unsigned int EvaUHFile::nextBlock(unsigned int *start, unsigned int *size)
{
	if(!flags){
		*start = 0xffffffff;
		*size = 0;
		return mSessionId;
	}
	unsigned int index;
	for(index=0; index<mNumPackets; index++)
		if(!flags[index]) break;
		
	if(index==0 || index== mNumPackets){
		*start = 0xffffffff;
		*size = 0;
	}else{
		*start = (index * MaxBlockSize);
		if(index == mNumPackets-1)
			*size = mFileSize - (*start);
		else
			*size = MaxBlockSize;
	}
	
	return mSessionId;
}

const bool EvaUHFile::isMD5Correct()
{
	if(!mBuffer)
		return false;
	
	char *gMd5 = new char[16];
	memcpy(gMd5, EvaUtil::doMd5((char *)mBuffer, mFileSize), 16);
	if(memcmp(gMd5, md5, 16) != 0){  // which means not equal
		delete [] gMd5;
		return false;
	}
	delete []gMd5;
	return true;
}

// Given a directory, that's enough
void EvaUHFile::save(QString &dir)
{
	if(!mBuffer){
		fprintf(stderr, "EvaUHFile::save -- NULL file buffer, failed\n");
		return;
	}
	
	/* we don't check md5, leave the job to calling function to do, save tiem :)
	// we check md5 first
	if(!isMD5Correct()){
		fprintf(stderr, "EvaUHFile::save -- MD5 checking, failed\n");
		initSettings(); // if md5 wrong, the data is useless, we clear all
		return;
	}
	*/
	QString filePrefix = dir + "/" + getMd5String();
	QFile file(filePrefix + ".bmp");
	if(!file.open(IO_WriteOnly | IO_Raw )){
		fprintf(stderr, "EvaUHFile::save -- file creating, failed\n");
		return;
	}
  
	QDataStream stream(&file);
	stream.writeRawBytes((char *)mBuffer, mFileSize);
	file.flush();
	file.close();
	QImage grayPic;
	grayPic.loadFromData(mBuffer, mFileSize);
	EvaQtUtils::convertToGrayscale(&grayPic);
	grayPic.save(filePrefix +"_off.bmp", "BMP");
}

// we only re-set the details not the basic information like id, session id and md5 value
void EvaUHFile::initSettings()
{
	if(flags) delete []flags;
	flags = NULL;
	if(mBuffer) delete []mBuffer;
	mBuffer = NULL;
	mNumPackets = 0;
	mFileSize = 0;
}


/************************************************************************************************/

 
class EvaUHProfile {
public:
	QString dir;            // note must set the UH directory first
	QMap<unsigned int, UHInfoItem> list;
	bool loadProfile();
	void saveProfile();
	void updateInfo(UHInfoItem item);
	void updateInfo(const unsigned int id, char *md5, unsigned int sid);
	void remove(const unsigned int id);
	void fileFinished(const unsigned int id);
	char *strMd5ToChar(const QString &strMd5);
	char *getMd5(const unsigned int id); // if not exists, NULL return
	// cuz session id only used within one session and will not be saved as well
	// but it does help to download the new file :)
	unsigned int getSession(const unsigned int id); 
	UHInfoItem nextDownload(); // if id is 0, means no file downloading needed
	QString getStrMd5(const unsigned int id);
};

bool EvaUHProfile::loadProfile()
{
	QString filename = dir + "/" + UH_PROFILE_NAME;
	QFile file(filename);
	if(!file.open(IO_ReadOnly)){
		fprintf(stderr, "EvaUHProfile::loadProfile -- file not exists\n");
		return false;
	}
	QTextStream stream(&file);
	while(!stream.atEnd()){
		QString line = stream.readLine();
		QStringList lines = QStringList::split(":", line);
		if(lines.size() != 2) continue;
		bool ok;
		int id = lines[0].stripWhiteSpace().toInt(&ok);
		if(!ok) continue;
		char *md5 = strMd5ToChar(lines[1].stripWhiteSpace());
		if(!md5) continue;
		UHInfoItem item;
		item.id = id;
		item.sessionId = 0;
		memcpy(item.md5, md5, 16);
		delete [] md5; 
		item.isUpdated = false;
		list[id] = item;
	}
	file.close();
	return true;
}

void EvaUHProfile::updateInfo(UHInfoItem item)
{
	QMap<unsigned int, UHInfoItem>::Iterator it = list.find(item.id);
	if(it != list.end()){
		if(memcmp(it.data().md5, item.md5, 16)!=0){
			memcpy(it.data().md5, item.md5, 16);
			it.data().sessionId = item.sessionId;
			it.data().isUpdated = true;
		}
	} else {
		item.isUpdated = true;
		list[item.id] = item;
	}
}

void EvaUHProfile::updateInfo(const unsigned int id, char *md5, unsigned int sid)
{
	QMap<unsigned int, UHInfoItem>::Iterator it = list.find(id);
	if(it != list.end()){
		if(memcmp(it.data().md5, md5, 16)!=0){
			memcpy(it.data().md5, md5, 16);
			it.data().sessionId = sid;
			it.data().isUpdated = true;
		}
	} else {
		UHInfoItem item;
		item.id = id;
		memcpy(item.md5, md5, 16);
		item.sessionId = sid;
		item.isUpdated = true;
		list[id] = item;
	}
}

void EvaUHProfile::remove(const unsigned int id)
{
	list.erase(id);
}

void EvaUHProfile::fileFinished(const unsigned int id)
{
	QMap<unsigned int, UHInfoItem>::Iterator it = list.find(id);
	if(it != list.end())
		it.data().isUpdated = false;
}

void EvaUHProfile::saveProfile()
{
	QString filename = dir + "/" + UH_PROFILE_NAME;
	QFile file(filename);
	if(!file.open(IO_WriteOnly)){
		fprintf(stderr, "EvaUHProfile::saveProfile -- cannot open file to write\n");
		return;
	}
	QTextStream stream(&file);
	QMap<unsigned int, UHInfoItem>::Iterator it;
	for(it = list.begin(); it!=list.end(); ++it){
		if(it.data().isUpdated) continue;     // only save the downloaded ones
		QString strId = QString::number(it.key());
		QString strMd5 = EvaHelper::md5ToString(it.data().md5);
		stream<<strId << ":" << strMd5 << "\n";
	}
	file.flush();
	file.close();
}

// we new a char[], so calling function should delete the memory
char *EvaUHProfile::strMd5ToChar(const QString &strMd5)
{
	QString ch;
	bool ok;
	int tmp;
	if(strMd5.length() != 32) return NULL;
	char *md5 = new char[16];
	
	for(uint i=0; i<strMd5.length(); i+=2){
		ch = strMd5.mid(i, 2);
		tmp = ch.toInt(&ok, 16);
		if(!ok)
			return NULL;
		md5[i/2] = tmp & 0xff;
	}
	return md5;
}

// calling function shouldn't delete the memory
char *EvaUHProfile::getMd5(const unsigned int id)
{
	QMap<unsigned int, UHInfoItem>::Iterator it = list.find(id);
	if(it != list.end())
		return it.data().md5;
	else
		return NULL;
}

// 0 means no session id avalible
unsigned int EvaUHProfile::getSession(const unsigned int id)
{
	QMap<unsigned int, UHInfoItem>::Iterator it = list.find(id);
	if(it != list.end())
		return it.data().sessionId;
	else
		return 0;
}

// return item.id == 0 means all finished, no more downloading needed
UHInfoItem EvaUHProfile::nextDownload()
{
	QMap<unsigned int, UHInfoItem>::Iterator it = list.begin();
	while(it != list.end()){
		if(it.data().isUpdated)
			return list[it.key()];
		++it;
	}
	UHInfoItem item;
	item.id = 0;
	return item;
}

QString EvaUHProfile::getStrMd5(const unsigned int id)
{
	return EvaHelper::md5ToString(getMd5(id));
}

/************************************************************************************************/

EvaUHManager::EvaUHManager(QObject *receiver, const QString &dir)
	: QObject(), QThread(), mReceiver(receiver), m_retryCount(0), AllInfoGotCounter(0), 
	mUHDir(dir), mAskForStop(false), mSocket(NULL), 
	mProfileManager(NULL), mCurrentFile(NULL), mDns(NULL)
{
}

EvaUHManager::~EvaUHManager()
{
	cleanUp();
	if(mProfileManager) delete mProfileManager;
	mProfileManager = NULL;
}

void EvaUHManager::initiate(QSize size)
{
	if(mProfileManager) delete mProfileManager;
	mProfileManager = new EvaUHProfile();
	mProfileManager->dir = mUHDir;
	if(mProfileManager->loadProfile()){
		imageOnList.clear();
		QMap<unsigned int, UHInfoItem>::Iterator it = mProfileManager->list.begin();
		while(it != mProfileManager->list.end()){
			QString filename = getFileName(it.data().id);
			//printf("User Head: id(%d), data.id(%d), file(%s)\n", it.key(), it.data().id, filename.local8Bit().data());
//			imageOnList[it.key()] = QImage(filename).smoothScale(size);
			imageOnList[it.key()] = QImage(filename);
			if(imageOnList[it.key()].isNull()) {
				printf("EvaUHManager::initiate() -- buddy %d has a NULL Image :%s, remove it from evauh.profile\n", it.key(), filename.ascii());
				mProfileManager->remove(it.key());
				mProfileManager->saveProfile();
				imageOnList.clear();
				initiate(); // do it again
				return;
			}
			filename = getFileName(it.data().id, true);
//			imageOffList[it.key()] = QImage(filename).smoothScale(size);
			imageOffList[it.key()] = QImage(filename);
			++it;
		}
	}
}

void EvaUHManager::run()
{
	doDnsRequest();
	int availableBytes = 0;
	bytesRead = 0;
	mSocket = new QSocketDevice(QSocketDevice::Datagram);
	doAllInfoRequest();
	while(1){
		if( (availableBytes = mSocket->bytesAvailable()) ){
			bytesRead = mSocket->readBlock(mBuffer, availableBytes);
			if(bytesRead != availableBytes)
				fprintf(stderr, "EvaUHManager:run -- bytes read might not be accurate\n");
			if(bytesRead)
				processComingData();
		} else{
			checkTimeout();
			msleep( 200 );
		}
		if(mAskForStop) break;
	}
	//printf("EvaUHManager runs finished\n");
	cleanUp();
}

void EvaUHManager::doDnsRequest()
{
	mHostAddresses.clear();
	if(mDns) delete mDns;
	mDns = new QDns(CFACE_SERVER);
	QObject::connect(mDns, SIGNAL(resultsReady()), SLOT(slotDnsReady()));
	
	while(!mHostAddresses.size()){
		if(mAskForStop) break;
		//printf("EvaUHManager::doDnsRequest -- waiting DNS\n");
		sleep(1);
	}
}

void EvaUHManager::slotDnsReady()
{
	//printf("EvaUHManager::slotDnsReady ----   got\n");
	mHostAddresses = mDns->addresses();
	if(!mHostAddresses.size()){
		QHostAddress host;
		host.setAddress("219.133.51.161");
		mHostAddresses.append(host);
	}
}

void EvaUHManager::send(EvaUHPacket *packet)
{
	if(! (mSocket && mSocket->isValid()) ){
		fprintf(stderr, "EvaUHManager::send -- socket error\n");
		delete packet;
		return;
	}
	unsigned char *buffer = new unsigned char[4096];
	int len = 0;
	packet->fill(buffer, &len);
	int results = mSocket->writeBlock((char*)buffer, len, mHostAddresses.first(), CFACE_PORT);
	delete []buffer;
	if(results ==-1 ){
		fprintf(stderr, "EvaUHManager::send -- socket error, writing failed\n");
	}
	if(results != len) {
		fprintf(stderr, "EvaUHManager::send -- writing bytes not equal to sending\n");
	}	
	delete packet;
}

void EvaUHManager::doAllInfoRequest()
{
	std::list<unsigned int> toSend;
	std::list<unsigned int>::iterator it;
	int counter = 0;
	//printf("EvaUHManager::doAllInfoRequest -- AllInfoGotCounter: %d\n", AllInfoGotCounter);
	if(AllInfoGotCounter < mUHList.size()){
		it=mUHList.begin();
		for(unsigned int i=0; i<AllInfoGotCounter ;++it) i++;
		for(; it!=mUHList.end(); ++it){
			toSend.push_back(*it);
			counter++;
			if(counter>=20) break;
		}
	}
	if(!toSend.size()) return;
	//printf("EvaUHManager::doAllInfoRequest -- %d Buddies sent\n", toSend.size());
	EvaUHInfoRequest *packet = new EvaUHInfoRequest();
	packet->setQQList(toSend);
	send(packet);
	cmdSent = All_Info;
	timeSent = QDateTime::currentDateTime();
}

void EvaUHManager::processComingData()
{
	switch(cmdSent){
	case No_CMD:
		break;
	case All_Info:
		processAllInfoReply();
		break;
	case Buddy_Info:
		processBuddyInfoReply();
		break;
	case Buddy_File:
		processBuddyFileReply();
		break;
	default:           //make some compilers happy :)
		break;
	}
}

void EvaUHManager::processAllInfoReply()
{
	//printf("EvaUHManager::processAllInfoReply \n");
	if(!mProfileManager) return;
	EvaUHInfoReply *packet = new EvaUHInfoReply((unsigned char *)mBuffer, bytesRead);
	if(packet->parse()){         //  good packet 
		if(!packet->isSuccessful()){ // no UH picture available
			//mProfileManager->remove();
			//mProfileManager->saveProfile();
			delete packet;
			return;
		}
		std::list<UHInfoItem> list= packet->getInfoItems();
		AllInfoGotCounter += list.size();
		std::list<UHInfoItem>::iterator it = list.begin();
		while(it!=list.end()){
			if(mProfileManager) mProfileManager->updateInfo(*it);
			++it;
		}
		
		if(AllInfoGotCounter < mUHList.size()){
			m_retryCount = 0;
			doAllInfoRequest();
		}else{
			// now we start download User Head :)
			if(!doInfoRequest())    // return false means all done
				mAskForStop = true;
		}
	}
	delete packet;
}

bool EvaUHManager::doInfoRequest()
{
	//printf("EvaUHManager::doInfoRequest\n");
	UHInfoItem item = mProfileManager->nextDownload();
	if(!(item.id))  // if id == 0, no file need download, we finish
		return false;

	m_retryCount++;
	std::list<unsigned int> qqList;
	qqList.push_back(item.id);
	EvaUHInfoRequest *packet = new EvaUHInfoRequest();
	packet->setQQList(qqList);
	send(packet);  // send method will delete sPacket
	cmdSent = Buddy_Info;
	timeSent = QDateTime::currentDateTime();
	return true;
}

void EvaUHManager::doTransferRequest(const unsigned int id, const unsigned int sid,
				const unsigned  int start, const unsigned  int end)
{
	//printf("EvaUHManager::doTransferRequest:\n\tid: %d, sid: %8x, s: %8x, e: %8x\n", id, sid, start, end);
	m_retryCount++;
	EvaUHTransferRequest *packet = new EvaUHTransferRequest();
	packet->setUHInfo(id, sid);
	packet->setFileInfo(start, end);
	send(packet);  // send method will delete sPacket
	cmdSent = Buddy_File;
	timeSent = QDateTime::currentDateTime();
}

void EvaUHManager::processBuddyInfoReply()
{
	if(!mProfileManager) return;
	EvaUHInfoReply *packet = new EvaUHInfoReply((unsigned char *)mBuffer, bytesRead);
	if(packet->parse()){         //  good packet 
		if(!packet->isSuccessful()){ // no UH picture available
			printf("EvaUHManager::processBuddyInfoReply -- no UH picture available\n");
			delete packet;
			return;
		}
		std::list<UHInfoItem> list= packet->getInfoItems();
		std::list<UHInfoItem>::iterator it = list.begin(); // actually this should be at most one element
		//FIXME we should check the new information
		if(mProfileManager) mProfileManager->updateInfo(*it);
		
		
		// now we start download User Head :)
		UHInfoItem item = mProfileManager->nextDownload();
		if(!(item.id)) {  // if id == 0, no file need download, we finish
			mAskForStop = true;
			m_retryCount = 0;
			delete packet;
			return;
		}
		
		if(mCurrentFile) delete mCurrentFile;
		mCurrentFile = new EvaUHFile(item.id);
		mCurrentFile->setFileInfo(item.md5, item.sessionId);
		unsigned int fstart, fend;
		mCurrentFile->nextBlock(&fstart, &fend);
		m_retryCount = 0;
		doTransferRequest(item.id, item.sessionId, fstart, fend);
	}
	delete packet;
}

void EvaUHManager::processBuddyFileReply()
{
	EvaUHTransferReply *packet = new EvaUHTransferReply((unsigned char *)mBuffer, bytesRead);
	if(packet->parse()){
		if(!packet->isDataPacket()){ // this is a notification, transfer will start very soon
			//printf("EvaUHManager::processBuddyFileReply -- Start Soon!!!\n");
			delete packet;
			return;
		}
		if(mCurrentFile){
			//printf("EvaUHManager::processBuddyFileReply -- No Pkts:%d,  Pkt No:%d\n", 
			//			packet->getNumPackets(), packet->getPacketNum());
			m_retryCount = 0;
			mCurrentFile->setFileBlock(packet->getNumPackets(), packet->getPacketNum(),
						packet->getFileSize(), packet->getStart(), 
						packet->getPartSize(), packet->getPartData() );
			if(mCurrentFile->isFinished()){
				if(mCurrentFile->isMD5Correct()){
					mCurrentFile->save(mUHDir);
					mProfileManager->fileFinished(packet->getQQ());
					mProfileManager->saveProfile();
					
					QImage imgOn  = QImage(getFileName(packet->getQQ())).smoothScale(EvaMain::global->getFaceSize());
					QImage imgOff = QImage(getFileName(packet->getQQ(), true)).smoothScale(EvaMain::global->getFaceSize());
					imageOnList[packet->getQQ()]  = imgOn;
					imageOffList[packet->getQQ()] = imgOff;
					EvaUHReadyEvent *event = new EvaUHReadyEvent();
					event->setQQ(packet->getQQ());
					event->setImages(imgOn, imgOff);
					QApplication::postEvent(mReceiver, event);
					
					delete mCurrentFile;
					mCurrentFile = NULL;
					// now we download next User Head
					if(!doInfoRequest()){    // return false means all done
						mAskForStop = true;
						cmdSent = All_Done;
					}
				}else{
					fprintf(stderr, "[EvaUHManager] md5 checking error\n");
					mCurrentFile->initSettings(); // we clear all contents of the file
				}
			}
		}
	}
}

void EvaUHManager::checkTimeout()
{
	if(mAskForStop || cmdSent == All_Done) return;
	
	if(m_retryCount >= UH_MAX_RETRY_COUNT){
		UHInfoItem item = mProfileManager->nextDownload();
		mProfileManager->fileFinished(item.id);
		// we won't save the non-image user head
		//mProfileManager->saveProfile();
		fprintf(stderr, "[EvaUHManager] downloading user(%d) avator error, try next one\n", item.id);
		m_retryCount = 0;
		cmdSent = Buddy_Info;
	}
	
	int last = timeSent.secsTo(QDateTime::currentDateTime());
	if(last >= UH_TIME_OUT){
		//printf("EvaUHManager::checkTimeout : %d\n", cmdSent);
		switch(cmdSent){
		case All_Info:
			doAllInfoRequest();
			break;
		case Buddy_Info:
			if(!doInfoRequest())    // return false means all done
				mAskForStop = true;
			break;
		case Buddy_File:{
			if(!mCurrentFile){
				UHInfoItem item = mProfileManager->nextDownload();
				mCurrentFile = new EvaUHFile(item.id);
				mCurrentFile->setFileInfo(item.md5, item.sessionId);
			}
			unsigned int fstart, fend, sid;
			sid = mCurrentFile->nextBlock(&fstart, &fend);
			if(!sid)  // if no session id, we ask for it
				doInfoRequest();
			else      // otherwise, download it
				doTransferRequest(mCurrentFile->getQQ(), sid, fstart, fend);
			}
			break;
		default:
			break;
		}
	}
}

void EvaUHManager::cleanUp()
{
	AllInfoGotCounter = 0;
	if(mSocket ) delete mSocket;
	mSocket = NULL;
	if(mCurrentFile) delete mCurrentFile;
	mCurrentFile = NULL;
	if(mDns) delete mDns;
	mDns = NULL;
}

void EvaUHManager::stop( )
{
	mAskForStop = true;
}

QString EvaUHManager::getFileName(const unsigned int id, bool isGrayscale)
{
	if(!mProfileManager) return "";
	
	QString strMd5 = mProfileManager->getStrMd5(id);
	if(strMd5.isEmpty()) return strMd5;
	if(isGrayscale)
		strMd5 += "_off.bmp";
	else
		strMd5 += ".bmp";
	QString fileName = mUHDir + "/" + strMd5;
	QFile file(fileName);
	if(!file.exists()) fileName = "";
	return fileName;
}

QImage *EvaUHManager::getUHImage(const unsigned int id, bool isGrayscale)
{
	QImage * result;
	QMap<unsigned int, QImage>::Iterator it;
	if(isGrayscale){
		it = imageOffList.find(id);
		if(it == imageOffList.end())
			result = NULL;
		else
			result = &(it.data());
	}else{
		it = imageOnList.find(id);
		if(it == imageOnList.end())
			result = NULL;
		else
			result = &(it.data());
	}
	return result;
}


