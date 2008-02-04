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
 
#ifndef EVACACHEDFILE_H
#define EVACACHEDFILE_H
#include <qstring.h>
#include <qmap.h>

#define MaxMd5CheckLength        10002432
#define MaxBlockLength           2048

class EvaCachedFile{
public:
	// for saving, involving info file, cached file and the dest file
	EvaCachedFile(const QString &srcDir, const QString &srcFilename, const unsigned int size);
	// for loading, this only source file involved
	EvaCachedFile(const QString &srcDir, const QString &srcFilename);

	~EvaCachedFile();

	void setDestFileDir(const QString &dir);
	void setCheckValues(const unsigned char *fileNameMd5, const unsigned char *fileMd5);
	const bool setFileInfo(const QString &fileName, const unsigned int size);

	const bool saveFragment(const unsigned int offset, const unsigned int len, unsigned char *buf);
	const unsigned int getFragment(const unsigned int offset, const unsigned int len, unsigned char *buf);

	const bool isFinished();
	const bool generateDestFile();

	enum ErrorState { ENone, ENotExists, EExists, EWriteFailed, EReadError,
			EFragCheck, EDupFragment, EInfoOpen, EMd5Error, EError};
	inline const int getError() { return m_State; }

	// if loading, return the md5 of source file in your disk,
	// if saving, return the md5 of dest file in your disk
	// if size > 10002432 bytes, return md5 of first 10002432 bytes
	const bool getSourceFileMd5(char *md5);
	const bool getSourceFileNameMd5(char *md5);
	
	// if loading, return the size of source file,
	// if saving, return the received size of dest file, file might not exist
	const unsigned int getFileSize();

	const bool loadInfoFile();
	// this method used in recovery the last downloading, return the next start offset
	const unsigned int getNextOffset();

	inline const QString &getDir() const { return m_DirPath; }
	inline const QString &getFileName() const { return m_FileName; }
private:
	bool m_IsLoading;
	QString m_DirPath;
	QString m_FileName;
	QString m_InfoFileName;
	QString m_CachedFileName;
	char m_FileNameMd5[16]; // we received one
	char m_FileMd5[16]; // we received
	unsigned int m_FileSize;
	ErrorState m_State;
	QMap<unsigned int, unsigned int> m_FragInfo;

	const bool isNewFragment(const unsigned int offset, const unsigned int len);
	const bool updateInfoFile(const unsigned int offset, const unsigned int len);
	const bool isFileCorrect();
	const bool isInfoFinished();

	static const bool calculateFileMd5(const QString& fullpath, char *md5Buf);

	const bool changeFileInfo();
};

#endif //  #ifndef EVACACHEFILE_H

