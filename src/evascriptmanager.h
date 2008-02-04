/***************************************************************************
 *   This class is grabbed from Amarok's ScriptManager                     *
 *   Copyright (C) 2004-2006 by Mark Kretschmann <markey@web.de>           *
 *                      2005 by Seb Ruiz <me@sebruiz.net>                  *
 *   Copyright (C) 2007 revised for Eva use                                *
 *                      by yunfan <yunfan_zg@163.com>                      *
 *                                                                         *
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02111-1307, USA.          *
 ***************************************************************************/

#ifndef EVA_SCRIPT_MANAGER_H
#define EVA_SCRIPT_MANAGER_H

/**
 * @class EvaScriptManager
 * @short Script management widget and backend
 * @author yunfan <yunfan_zg@163.com>
 *
 * Script notifications, sent to stdin:
 *   configure:       Configure
 *   status change:   StatusChanged [0-9]+
 *   message:         Message 1970-06-23 11:22:33 {Qun|Contact} [0-9]+ message
 *   button clicking: ButtonClicked {Qun|Contact} [0-9]+ buttonname
 *   stop script:     Stop
 *
 * when stoping a script, this manager doesn't terminate it immediately, it
 * will send the script a message "Stop". Once script got this notification
 * it can do some finishing job( like remove tabs if it adds some), then the
 * script should end itself so that the manager will know it exits.
 *
 * @note To implement this feature, the class ScriptManager in Amarok is used
 *       as an initial implementation template. Many of this class are directly from
 *       Amarok's source code. Many thanks to Amarok team, especially the author
 *       of ScriptManager, Mark Kretschmann.
 * @see http://www.myswear.net/myswear/eva/Script-Writing_HowTo
 */

#include <qmap.h>

#include <kdialogbase.h>      //baseclass
#include <kurl.h>
#include "evamain.h"
#include "evamainwindow.h"
#include "evadcoptools.h"

class EvaMainWindow;
class EvaScriptManagerUIBase;
class ScriptManagerBase;
class QListViewItem;
class KArchiveDirectory;
class KProcess;
class KProcIO;
class EvaScriptWidget;

//Message types
const unsigned char MT_Eva          = 1;      // every script has this type
const unsigned char MT_Message      = 1 << 1; // if you want receive chatting messages
const unsigned char MT_Status       = 1 << 2; // if you want receive any stauts changing event
const unsigned char MT_ButtonClick  = 1 << 3; // if you want capture any custom button clicking event

class EvaScriptManager : public KDialogBase, virtual public EvaDCOPToolsInterface
{
	Q_OBJECT

	public:
		EvaScriptManager( QWidget *parent = 0, const char *name = 0 );
		virtual ~EvaScriptManager();

// 		static EvaScriptManager* instance() 
// 		{ return s_instance ? s_instance : new EvaScriptManager( EvaMain::g_mainWin ); }


		/** Returns a list of all currently running scripts. Used by the DCOP handler. */
		QStringList listRunningScripts();

		/** Custom Menu Click */
		void customMenuClicked(const bool isQun, const unsigned int id, QString scriptName, QString name);

		/** Returns the path of the spec file of the given script */
		QString specForScript( const QString& name );

		/** Send a notification to all scripts with the contact's ID number*/
		void notifyStatusChange(const unsigned int id);
		
		/**
		 * Send a chat message from Qun or contact to all scripts
		 * @param isQun Qun message or contact message
		 * @param id Qun external id or contact QQ number depending on \c isQun
		 * @param message the message
		 */
		void notifyMessage(const bool isQun, const unsigned int id, const QDateTime &time, const QString &message);
		
		///DCOP methods

    /**
		 * Runs the script with the given name. Used by the DCOP handler.
		 * @param name The name of the script.
		 * @return True if successful.
		 */
		bool runScript( const QString& name, bool silent = false );

    /**
		 * Stops the script with the given name. Used by the DCOP handler.
		 * @param name The name of the script.
		 * @return True if successful.
		 */
		bool stopScript( const QString& name );
		QString scriptDir( bool isSystem );
		int numScripts();
		int numRunningScripts();
		void releaseScripts();
		
		/** Finds all installed scripts and adds them to the listview */
		void findScripts();
	private slots:

		/** Enables/disables the buttons */
		void slotCurrentChanged( QListViewItem* );

		bool slotInstallScript( const QString& path = QString::null );
		//void slotRetrieveScript();
		void slotUninstallScript();
		bool slotRunScript( bool silent = false );
		void slotStopScript();
		void slotConfigureScript();
		void slotAboutScript();
		void slotShowContextMenu( QListViewItem*, const QPoint& );

		void slotReceivedStdout( KProcess*, char*, int );
		void slotReceivedStderr( KProcess*, char*, int );
		void scriptFinished( KProcess* process );

	private:
		
		/** Terminates a process with SIGTERM and deletes the KProcIO object */
		void terminateProcess( KProcIO** proc );

		/** Sends a string message to all running scripts */
		void notifyScripts( const QString& message, const unsigned char type );
		/** Sends a string message to a given named script */
		void notifyScript( const QString& name, const QString message );

		/** Adds a script to the listview */
		void loadScript( const QString& path );

		/** Copies the file permissions from the tarball and loads the script */
		void recurseInstall( const KArchiveDirectory* archiveDir, const QString& destination );
		
		/** remove any script-added buttons/tabs **/
		void releaseScriptResource(const QString &scriptName);

		static QString        s_evaDcopName; //!< used as a parameter passed to script.
//		static EvaScriptManager* s_instance;
		EvaScriptManagerUIBase*    m_gui;

// 		QListViewItem*        m_generalCategory;
// 		QListViewItem*        m_lyricsCategory;
// 		QListViewItem*        m_scoreCategory;
// 		QListViewItem*        m_transcodeCategory;

		bool                  m_installSuccess;

		struct ScriptItem {
			KURL           url;
			unsigned char  type;
			KProcIO*       process;
			QListViewItem* li;
			QString        log;
			ScriptItem() : process( 0 ), li( 0 ) {}
		};

		typedef QMap<QString, ScriptItem> ScriptMap;

		ScriptMap m_scripts;
		
		friend class EvaScriptWidget;
};

extern EvaScriptManager *GetScriptManager();

#endif //EVA_SCRIPT_MANAGER_H
