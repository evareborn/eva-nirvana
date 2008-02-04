/***************************************************************************
 *   Copyright (C) 2007 by yunfan                                          *
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
#ifndef EVA_SCRIPT_WIDGET_H
#define EVA_SCRIPT_WIDGET_H

#include <khtmlview.h>
#include <khtml_part.h>
#include <kurl.h>
#include <qdatetime.h>
#include <qcolor.h>

class KPopupMenu;
class MenuPrivateData;
//class KAction;

class EvaScriptWidget : public KHTMLPart
{
	Q_OBJECT
	public:
		EvaScriptWidget(QWidget* parent = 0, const char* name = 0);
		virtual ~EvaScriptWidget();
// 	signals:
// 		void saveAsCustomSmiley(QString ); // full name with absolute path
	private:
	
		KPopupMenu *m_menu;
		MenuPrivateData *m_d;
		KAction *m_copyAction;

		QString getNodeText(DOM::Node &node);
	private slots:

		void slotPopupMenu(const QString &url, const QPoint &point);

		void slotSelectionChanged();
		void slotOpenExternalBrower();
		void slotLinkClicked( const KURL & urls, const KParts::URLArgs &);
		void slotCopy();
		void slotCopyLinkLocation();
		void slotCopyImage();
		void slotSaveImageAs();
		void slotSaveAsCustomSmiley();
		void slotInstallRemoteScript();
		void slotDownloadScriptDone (KIO::Job *job, const KURL &from, const KURL &to, bool directory, bool renamed);

};

#endif // EVA_SCRIPT_WIDGET_H
