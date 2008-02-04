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

#include "evacachedfile.h"
#include "../../libeva/evautil.h"
#include <stdio.h>
#include <string.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextcodec.h>

#define InfoFileName_Ext       ".info"
#define CacheFileName_Ext      ".cache"

/*!
 * This class is to implement the file related functionalities of file transfering.
 * \a EvaCachedFile provides loading fragment from a file and save a fragment to a 
 * temporary file and method to generate the final file from the temporary cached 
 * file.
 *
 */



EvaCachedFile::EvaCachedFile(const QString &srcDir, const QString &srcFilename, const unsigned int size)
	: m_IsLoading(false), m_DirPath(srcDir), m_FileName(srcFilename), 
	m_FileSize(size), m_State(ENone)
{
	changeFileInfo();
}

// we know that we don't need the info file for loading
EvaCachedFile::EvaCachedFile(const QString &srcDir, const QString &srcFilename)
	: m_IsLoading(true), m_DirPath(srcDir), m_FileName(srcFilename), m_CachedFileName(""),
	m_State(ENone)
{
	QString filePath = m_DirPath + "/" + m_FileName;
	QFileInfo info(filePath);
	if(!info.exists()){
		fprintf(stderr, "EvaCachedFile::constructor -- \"%s\" dosen't exist!\n", filePath.ascii());
		m_State = ENotExists;
		return;
	}
}

EvaCachedFile::~EvaCachedFile()
{
}

const bool EvaCachedFile::setFileInfo(const QString &fileName, const unsigned int size)
{
	m_FileName = fileName;
	m_FileSize = size;
	changeFileInfo();
	return true;
	//return changeFileInfo();
}

const bool EvaCachedFile::changeFileInfo()
{
	m_CachedFileName = m_FileName + CacheFileName_Ext;
	m_InfoFileName = m_FileName + InfoFileName_Ext;

	QString filePath = m_DirPath + "/" + m_CachedFileName;
	QFileInfo info(filePath);
	if(info.exists()){
		fprintf(stderr, "EvaCachedFile::constructor -- \"%s\" already exist!\n", filePath.ascii());
		m_State = EExists;
		return false;
	}
	//m_InfoFileName = m_FileName + InfoFileName_Ext;
	filePath = m_DirPath + "/" + m_InfoFileName;
	info.setFile(filePath);
	if(info.exists()){	
		fprintf(stderr, "EvaCachedFile::constructor -- \"%s\" already exists! delete it!\n", filePath.ascii());
		QFile file(filePath);
		if(!file.remove()){
			fprintf(stderr, "EvaCachedFile::constructor -- cannot remove \"%s\"!\n", filePath.ascii());
			m_State = EError;
			return false;
		}
	}
	return true;
}

/*!
 *  save one fragment onto disk
 * 
 * \param offset the abusolute offset address, the start position
 * \param len the length of wanted
 * \param buf the contents to save
 * \return true if saving success, otherwise false
 *
 * \note this method is to save the data into a temporary file which 
 *         each fragment contains 8 extra header bytes \e
 * 
 */


const bool EvaCachedFile::saveFragment(const unsigned int offset, 
					const unsigned int len, 
					unsigned char *buf)
{
	if(m_IsLoading) return false;
	if( ! isNewFragment(offset, len)){
		printf("EvaCachedFile::saveFragment -- already got this fragment!\n");
		return true; // if we got it already, always return true
	}

	QString fullpath = m_DirPath + "/" + m_CachedFileName;
	QFile file(fullpath);
	if(!file.open(IO_Raw | IO_WriteOnly | IO_Append)){
		printf("EvaCachedFile::saveFragment -- cannot open \"%s\"!\n", fullpath.ascii());
		return false;
	}
	char strHeader[8];
	memcpy(strHeader, &offset, 4);
	memcpy(strHeader+4, &len, 4);
	file.writeBlock(strHeader, 8);
	file.writeBlock((char *)buf, len);
	file.close();
	if( ! updateInfoFile(offset, len)) return false;

	// let user control this
// 	if( isFinished() )
// 		return generateDestFile();

	return true;
}

/*!
 *  read one fragment from the source file in the disk
 *
 * \param offset the abusolute start position of this read operation
 * \param len the length of read
 * \param buf the pre-allocated buffer all the read data written into with length of len
 * \return the exact bytes of read
 */

const unsigned int EvaCachedFile::getFragment(const unsigned int offset, 
						const unsigned int len, 
						unsigned char *buf)
{
	if(!m_IsLoading) return false;

	unsigned int bytesRead = 0;
	QFile file(m_DirPath + "/" + m_FileName);
	if(!file.open(IO_Raw | IO_ReadOnly)){
		printf("EvaCachedFile::getFragment -- \"%s\" dosen't exist!\n", m_FileName.ascii());
		return bytesRead;
	}
	if(file.at(offset))
		bytesRead = file.readBlock((char *)buf, len);
	file.close();
	if( !bytesRead){
		m_State = EReadError;
		return 0;
	}
	return bytesRead;
}

void EvaCachedFile::setCheckValues(const unsigned char *fileNameMd5, const unsigned char *fileMd5)
{
	memcpy(m_FileNameMd5, fileNameMd5, 16);
	memcpy(m_FileMd5, fileMd5, 16);	
}

const bool EvaCachedFile::isFinished()
{
	return isInfoFinished();
}

const bool EvaCachedFile::isNewFragment(const unsigned int offset, const unsigned int /*len*/)
{
	if(m_IsLoading) return false;
	QMap<unsigned int, unsigned int>::Iterator iter;
	iter = m_FragInfo.find(offset);
	// we might need to check the length of this fragment
	if(iter != m_FragInfo.end()){
		m_State = EDupFragment;
		return false;
	}
	return true;
}

const bool EvaCachedFile::updateInfoFile(const unsigned int offset, const unsigned int len)
{
	if(m_IsLoading) return false;
	QFile file(m_DirPath + "/" + m_InfoFileName);
	if(!file.open(IO_WriteOnly | IO_Truncate)){
		fprintf(stderr, "EvaCachedFile::updateInfoFile -- cannot update info file!\n");
		m_State = EInfoOpen;
		return false;
	}
	m_FragInfo[offset] = len;
	QDataStream stream(&file);

	Q_UINT32 qsize = m_FileSize;
	// we save the basic info first
	stream<< m_FileName;
	stream<<qsize;
	stream.writeRawBytes(m_FileNameMd5, 16);
	stream.writeRawBytes(m_FileMd5, 16);

	QMap<unsigned int, unsigned int>::Iterator iter;
	Q_UINT32 qoffset, qlen;
	for(iter=m_FragInfo.begin(); iter!=m_FragInfo.end(); ++iter){
		qoffset = iter.key();
		qlen = iter.data();
		stream<<qoffset<<qlen;
	}
	file.close();
	return true;
}

const bool EvaCachedFile::loadInfoFile()
{
	if(m_IsLoading) return false;
	QFile file(m_DirPath + "/" + m_InfoFileName);
	if(!file.open(IO_ReadOnly)){
		fprintf(stderr, "EvaCachedFile::loadInfoFile -- no info file available.\n");
		m_State = EInfoOpen;
		return false;
	}
	
	QDataStream stream(&file);
	QString fileName;
	stream>> fileName;
	if(fileName != m_FileName)
		return false;
	
	Q_UINT32 tmp;
	stream>>tmp;
	if(tmp != m_FileSize) {
		file.close();
		return false;
	}

	char *fnmd5 = new char[16];
	stream.readRawBytes(fnmd5, 16);
	if(memcmp(fnmd5, m_FileNameMd5, 16)){
		m_State = EMd5Error;
		file.close();
		delete [] fnmd5;
		return false;
	}
	delete [] fnmd5;

	char *fmd5 = new char[16];
	stream.readRawBytes(fmd5, 16);
	if(memcmp(fmd5, m_FileMd5, 16)){
		m_State = EMd5Error;
		file.close();
		delete [] fmd5;
		return false;
	}
	delete [] fmd5;

	Q_UINT32 qoffset, qlen;
	while(!stream.atEnd()){
		stream>>qoffset>>qlen;
		m_FragInfo[qoffset]=qlen;
	}
	file.close();
	return true;
}

// we only test the size of the file
const bool EvaCachedFile::isInfoFinished()
{
	if(m_IsLoading) return false;

	Q_UINT32 tmp = 0;
	QMap<unsigned int, unsigned int>::Iterator iter;
	//Q_UINT32 qoffset, qlen;
	for(iter=m_FragInfo.begin(); iter!=m_FragInfo.end(); ++iter){
		tmp += iter.data();
	}
	printf("EvaCachedFile::isInfoFinished -- tmp: %d, m_FileSize: %d\n", tmp, m_FileSize);
	if(tmp == m_FileSize) return true;
	return false;
}

const unsigned int EvaCachedFile::getNextOffset()
{
	if(m_IsLoading) return 0;

	Q_UINT32 offset = 0;
	QMap<unsigned int, unsigned int>::Iterator iter;
	for(iter=m_FragInfo.begin(); iter!=m_FragInfo.end(); ++iter){
		offset += iter.data();
	}
	return offset;
}

const bool EvaCachedFile::isFileCorrect()
{
	if(m_IsLoading) return true; // if we are loading file, this would be always true

	// check dest-file size
	QFileInfo info(m_DirPath + "/" + m_FileName);
	if(!info.exists())
		return false;
	
	if(info.size() != m_FileSize) return false;

	// check dest-file md5 value
	char *md5 = new char[16];
	if(!getSourceFileMd5(md5)){
		m_State = EMd5Error;
		return false;
	}

	if(memcmp(m_FileMd5, md5, 16)){
		m_State = EMd5Error;
		return false;
	}

	// check dest-file name md5
	getSourceFileNameMd5(md5);
	if(memcmp(m_FileNameMd5, md5, 16)){
		fprintf(stderr, "EvaCachedFile::isFileCorrect -- \"%s\" file name saved might be wrong but file contents should be all right.\n", m_FileName.ascii());
	}
	delete []md5;
	return true;
}

const bool EvaCachedFile::generateDestFile()
{
	if(m_IsLoading) return false;
	if(m_DirPath.isEmpty()) return false;
	QString cachedFileName = m_DirPath + "/" + m_FileName + CacheFileName_Ext;
	QString destFileName = m_DirPath + "/" + m_FileName;
	QFile cached(cachedFileName);
	QFile dest(destFileName);
	if(!cached.open(IO_ReadOnly)){
		fprintf(stderr, "EvaCachedFile::generateDestFile -- cannot open cached file \"%s\"!\n", cachedFileName.ascii());
		return false;
	}
	if(!dest.open(IO_WriteOnly)){
		fprintf(stderr, "EvaCachedFile::generateDestFile -- cannot create destination file \"%s\"!\n", destFileName.ascii());
		return false;
	}

	char *buf; // we might create a fixed-size buffer to save time
	char strHeader[8];
	unsigned int offset, len;
	while(!cached.atEnd()){
		cached.readBlock(strHeader, 8);
		memcpy(&offset, strHeader, 4);
		memcpy(&len, strHeader+4, 4);
		buf = new char [len];
		cached.readBlock(buf, len);

		dest.at(offset);
		dest.writeBlock(buf, len);
		delete []buf;
	}
	cached.close();
	dest.close();
	if(!isFileCorrect()){
		//dest.remove();
		fprintf(stderr, "EvaCachedFile::generateDestFile -- md5 checking wrong\n");
		return false;
	}
	// actually we got all we want, we don't care about the result of removing following files
	cached.remove();
	QFile info(m_DirPath + "/" + m_InfoFileName);
	info.remove();
	m_FragInfo.clear();
	return true;
}

const bool EvaCachedFile::calculateFileMd5(const QString& fullpath, char *md5Buf)
{
	QFileInfo info(fullpath);
	if(!info.exists()){
		fprintf(stderr, "EvaCachedFile::calculateFileMd5 -- \"%s\" dosen't exist!\n", fullpath.ascii());
		return false;
	}
	unsigned int len = info.size();
	if(len > MaxMd5CheckLength)
		len = MaxMd5CheckLength;
	char *buf = new char[len];

	QFile file(fullpath);
	if(!file.open(IO_ReadOnly)){
		printf("EvaCachedFile::calculateFileMd5 -- \"%s\" dosen't exist!\n", fullpath.ascii());
		delete []buf;
		return false;
	}
	unsigned int numRead = file.readBlock(buf, len);
	file.close();
	if(numRead!=len){
		delete []buf;
		return false;
	}
	memcpy(md5Buf, EvaUtil::doMd5(buf, len), 16);
	delete []buf;
	return true;
}

const bool EvaCachedFile::getSourceFileMd5(char *md5)
{
	if(!md5) return false;
	if(m_DirPath.isEmpty() || m_FileName.isEmpty()) return false;
	return calculateFileMd5(m_DirPath + "/" + m_FileName, md5);
}

const bool EvaCachedFile::getSourceFileNameMd5(char *md5)
{
	if(!md5) return false;
	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	QCString tmp = codec->fromUnicode(m_FileName);
	memcpy(md5, EvaUtil::doMd5(tmp.data(), tmp.length()), 16);
	return true;
}

const unsigned int EvaCachedFile::getFileSize()
{
	if(!m_IsLoading) return m_FileSize;
	QFileInfo info(m_DirPath + "/" + m_FileName);
	if(!info.exists())
		return 0;
	return info.size();
}

void EvaCachedFile::setDestFileDir( const QString & dir )
{
	if(m_IsLoading) return;
	m_DirPath = dir;
	changeFileInfo();
}
