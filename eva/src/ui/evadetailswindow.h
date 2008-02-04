/***************************************************************************
 *   Copyright (C) 2004 by zsyddl                                          *
 *   morrowren@sina.com                                                     *
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
/*
	revised by yunfan  2005
	           tlmcasper 2005
	yunfan_zg@163.com
	tlmcasper@163.com
*/

#ifndef EVADETAILSWINDOW_H
#define EVADETAILSWINDOW_H

#include "evauseruibase.h"
#include "../libeva/evamemo.h"
#include "evafriendlist.h"
#include "evausersetting.h"

#include <qvariant.h>
#include <qstringlist.h>
#include <qtextcodec.h>


class EvaImageResource;
class QCloseEvent;

// the QStringList is used to hold all information about a user and has 37 QStrings only   --- yunfan
class EvaDetailsWindow : public EvaUserUIBase
{
	Q_OBJECT
public:
	EvaDetailsWindow(QStringList &user, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);
	~EvaDetailsWindow(); 
	static void setImageResource( EvaImageResource *res ) { faces = res; }

private:
	static EvaImageResource* faces;
	QStringList details;			//user basic info 
	int selectedFaceIndex;
	bool m_IsModifyMemo;
	bool m_IsMemoPage;
	MemoItem m_Memo;
	QTextCodec *codec;
	const QQFriend *frd;
	unsigned int id;
	

	const bool UpdateData( const bool );
	const QString getClientKeyString();

	void closeEvent(QCloseEvent *e);
signals:
	void requestUpdate( const unsigned int );
	void requestQQShow(const unsigned int);
	
	void requestDownloadMemo( const unsigned int );
	void requestUploadMemo( const unsigned int, const MemoItem &);
	
	void memoChanged(const unsigned int, const MemoItem &);
	
public slots:
	void slotUpdateClick();
	void slotDetailsUpdated( QStringList list );
	void slotUpdateQQShow();
	
	void slotFirstUpdate();
	
	void slotShopClicked();
	void slotAlbumClicked();
	void slotLinkClicked( const QString & url);
	
	void slotBuddyQQShowReady( const unsigned int id );
	
	void slotCurrentChanged( QWidget *wCurrentPage);
	void slotDownloadMemo();
	void slotUpdateMemo(const MemoItem &memo);
	void slotUploadMemoReply(const bool);
	void slotNoMemoFound();

};
#endif 
