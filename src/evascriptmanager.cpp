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
#include "evascriptmanager.h"
#include "evascriptmanageruibase.h"
#include "evamain.h"
#include "evaresource.h"
#include "evauser.h"

#include <sys/types.h>
#include <unistd.h>

#include <qdir.h>
#include <qfileinfo.h>
#include <qfont.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qpixmap.h>
#include <qstringlist.h>
#include <qtextcodec.h>

#include <kaboutdialog.h>
#include <kapplication.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kio/netaccess.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kprocio.h>
#include <kprotocolmanager.h>
#include <kpushbutton.h>
#include <krun.h>
#include <kstandarddirs.h>
#include <ktar.h>
#include <ktextedit.h>
#include <kwin.h>
#include <kdebug.h>

class ScriptListViewItem : public KListViewItem
{
public:
	ScriptListViewItem(KListView  *lv, QString &caption) 
		: KListViewItem(lv, caption)
		, m_scriptName("")
	{
	}
	QString  m_scriptName;
};

// void closeOpenFiles(int out, int in, int err) {
// 	for(int i = sysconf(_SC_OPEN_MAX) - 1; i > 2; i--)
// 		if(i!=out && i!=in && i!=err)
// 			close(i);
// }

EvaScriptManager * GetScriptManager()
{
	static EvaScriptManager m_scriptManager;
	return &m_scriptManager;
}

//EvaScriptManager* EvaScriptManager::s_instance = 0;
QString EvaScriptManager::s_evaDcopName;


EvaScriptManager::EvaScriptManager( QWidget *parent, const char *name )
	: DCOPObject("Tools")
	, KDialogBase( parent, name, false, QString::null, Close, Close, true )
	, m_gui( new EvaScriptManagerUIBase( this ) )
{
//	s_instance = this;
	s_evaDcopName = QString("eva-%1").arg(getpid());

	kapp->setTopWidget( this );
	setCaption( kapp->makeStdCaption( i18n( "Script Manager" ) ) );

    // Gives the window a small title bar, and skips a taskbar entry
	KWin::setType( winId(), NET::Utility );
	KWin::setState( winId(), NET::SkipTaskbar );

	setMainWidget( m_gui );
	m_gui->listView->setRootIsDecorated( true );
	m_gui->listView->setFullWidth( true );
	m_gui->listView->setShowSortIndicator( true );


    /// Category items
// 	m_generalCategory    = new KListViewItem( m_gui->listView, i18n( "General" ) );
// 	m_lyricsCategory     = new KListViewItem( m_gui->listView, i18n( "Lyrics" ) );
// 	m_scoreCategory      = new KListViewItem( m_gui->listView, i18n( "Score" ) );
// 	m_transcodeCategory  = new KListViewItem( m_gui->listView, i18n( "Transcoding" ) );
// 
// 	m_generalCategory  ->setSelectable( false );
// 	m_lyricsCategory   ->setSelectable( false );
// 	m_scoreCategory    ->setSelectable( false );
// 	m_transcodeCategory->setSelectable( false );
// 
// 	m_generalCategory  ->setPixmap( 0, SmallIcon( Amarok::icon( "files" ) ) );
// 	m_lyricsCategory   ->setPixmap( 0, SmallIcon( Amarok::icon( "files" ) ) );
// 	m_scoreCategory    ->setPixmap( 0, SmallIcon( Amarok::icon( "files" ) ) );
// 	m_transcodeCategory->setPixmap( 0, SmallIcon( Amarok::icon( "files" ) ) );
// 
//     // Restore the open/closed state of the category items
// 	KConfig* const config = Amarok::config( "ScriptManager" );
// 	m_generalCategory  ->setOpen( config->readBoolEntry( "General category open" ) );
// 	m_lyricsCategory   ->setOpen( config->readBoolEntry( "Lyrics category open" ) );
// 	m_scoreCategory    ->setOpen( config->readBoolEntry( "Score category State" ) );
// 	m_transcodeCategory->setOpen( config->readBoolEntry( "Transcode category open" ) );

	connect( m_gui->listView, SIGNAL( currentChanged( QListViewItem* ) ), SLOT( slotCurrentChanged( QListViewItem* ) ) );
	connect( m_gui->listView, SIGNAL( doubleClicked ( QListViewItem*, const QPoint&, int ) ), SLOT( slotRunScript() ) );
	connect( m_gui->listView, SIGNAL( contextMenuRequested ( QListViewItem*, const QPoint&, int ) ), SLOT( slotShowContextMenu( QListViewItem*, const QPoint& ) ) );

	connect( m_gui->btnInstall,   SIGNAL( clicked() ), SLOT( slotInstallScript() ) );
	//connect( m_gui->retrieveButton,  SIGNAL( clicked() ), SLOT( slotRetrieveScript() ) );
	connect( m_gui->btnUninstall, SIGNAL( clicked() ), SLOT( slotUninstallScript() ) );
	connect( m_gui->btnRun,       SIGNAL( clicked() ), SLOT( slotRunScript() ) );
	connect( m_gui->btnStop,      SIGNAL( clicked() ), SLOT( slotStopScript() ) );
	connect( m_gui->btnConfig,    SIGNAL( clicked() ), SLOT( slotConfigureScript() ) );
	connect( m_gui->btnAbout,     SIGNAL( clicked() ), SLOT( slotAboutScript() ) );

	EvaImageResource * images = EvaMain::images;
	if(images){
		QPixmap *icon = images->getIcon("SCRIPT_INSTALL");
		if(icon)
			m_gui->btnInstall  ->setIconSet( QIconSet( *icon ) );
		//m_gui->retrieveButton ->setIconSet( SmallIconSet( Amarok::icon( "download" ) ) );
		icon = images->getIcon("SCRIPT_UNINSTALL");
		if(icon)
			m_gui->btnUninstall->setIconSet( QIconSet( *icon) );
		
		icon = images->getIcon("SCRIPT_RUN");
		if(icon)
			m_gui->btnRun      ->setIconSet( QIconSet( *icon) );
		
		icon = images->getIcon("SCRIPT_STOP");
		if(icon)
			m_gui->btnStop     ->setIconSet( QIconSet( *icon) );
		
		icon = images->getIcon("SCRIPT_CONFIG");
		if(icon)
			m_gui->btnConfig   ->setIconSet( QIconSet( *icon) );
		
		icon = images->getIcon("SCRIPT_ABOUT");
		if(icon)
			m_gui->btnAbout    ->setIconSet( QIconSet( *icon) );
	}
	QSize sz = sizeHint();
	setMinimumSize( kMax( 350, sz.width() ), kMax( 250, sz.height() ) );
	resize( sizeHint() );

  // Delay this call via eventloop, because it's a bit slow and would block
	//QTimer::singleShot( 1000, this, SLOT( findScripts() ) );
}

EvaScriptManager::~EvaScriptManager()
{
// 	kdDebug() << "EvaScriptManager destructor"<< endl;
// 	QStringList runningScripts;
// 	ScriptMap::Iterator it;
// 	ScriptMap::Iterator end( m_scripts.end() );
// 	for( it = m_scripts.begin(); it != end; ++it ) {
// 		if( it.data().process ) {
// 			terminateProcess( &it.data().process );
// 			releaseScriptResource(it.key());
// 			runningScripts << it.key();
// 			kdDebug() << "Running script: "<< it.key() << endl;
// 		}
// 	}
// 
// 	kdDebug() << "------start writing config " << endl;
//     // Save config
// 	KConfig* const config = EvaMain::user->config( "ScriptManager" );
// 	
// 	kdDebug() << "start writing config " << endl;
// 	config->writeEntry( "Running Scripts", runningScripts );
// 	kdDebug() << "Writing config done" << endl;

// 	Save the open/closed state of the category items
// 	config->writeEntry( "General category open", m_generalCategory->isOpen() );
// 	config->writeEntry( "Lyrics category open", m_lyricsCategory->isOpen() );
// 	config->writeEntry( "Score category open", m_scoreCategory->isOpen() );
// 	config->writeEntry( "Transcode category open", m_transcodeCategory->isOpen() );

	s_evaDcopName = "";
//	s_instance = 0;
}

void EvaScriptManager::releaseScripts( )
{
	kdDebug() << "EvaScriptManager destructor"<< endl;
	m_gui->listView->clear();
	QStringList runningScripts;
	ScriptMap::Iterator it;
	ScriptMap::Iterator end( m_scripts.end() );
	for( it = m_scripts.begin(); it != end; ++it ) {
		if( it.data().process ) {
			terminateProcess( &it.data().process );
			releaseScriptResource(it.key());
			runningScripts << it.key();
			kdDebug() << "Running script: "<< it.key() << endl;
		}
	}
  // Save config
	if(runningScripts.size()) {
		KConfig* const config = EvaMain::user->config( "ScriptManager" );
		config->writeEntry( "Running Scripts", runningScripts );
	}
}

bool EvaScriptManager::runScript( const QString& name, bool silent )
{
	if( !m_scripts.contains( name ) )
		return false;

	m_gui->listView->setCurrentItem( m_scripts[name].li );
	return slotRunScript( silent );
}

bool EvaScriptManager::stopScript( const QString& name )
{
	if( !m_scripts.contains( name ) )
		return false;

	m_gui->listView->setCurrentItem( m_scripts[name].li );
	slotStopScript();

	return true;
}


QStringList EvaScriptManager::listRunningScripts()
{
	QStringList runningScripts;
	ScriptMap::Iterator it = m_scripts.begin();
	for( it = m_scripts.begin(); it != m_scripts.end(); ++it)
		if( it.data().process )
			runningScripts << it.key();

	return runningScripts;
}

QString EvaScriptManager::specForScript( const QString& name )
{
	if( !m_scripts.contains( name ) )
		return QString();
	QFileInfo info( m_scripts[name].url.path() );
	const QString specPath = info.dirPath() + '/' + info.baseName( true ) + ".spec";

	return specPath;
}

void EvaScriptManager::findScripts() //SLOT
{
	releaseScripts();
	QStringList allFiles = kapp->dirs()->findAllResources( "data", "eva/scripts/*", true );

    // Add found scripts to listview:
	{
		for( QStringList::Iterator it = allFiles.begin(); it != allFiles.end(); ++it )
			if( QFileInfo( *it ).isExecutable() )
				loadScript( *it );
	}

    // Handle auto-run:

	KConfig* const config = EvaMain::user->config( "ScriptManager" );
	QStringList runningScripts = config->readListEntry( "Running Scripts" );

	{
		for( QStringList::Iterator it = runningScripts.begin(); it != runningScripts.end(); ++it )
			if( m_scripts.contains( *it ) ) {
				kdDebug() << "Auto-running script: " << *it << endl;
				m_gui->listView->setCurrentItem( m_scripts[*it].li );
				slotRunScript();
			}
	}

	m_gui->listView->setCurrentItem( m_gui->listView->firstChild() );
	slotCurrentChanged( m_gui->listView->currentItem() );
}

void EvaScriptManager::slotCurrentChanged( QListViewItem* item )
{
// 	const bool isCategory = item == m_generalCategory ||
// 			item == m_lyricsCategory ||
// 			item == m_scoreCategory ||
// 			item == m_transcodeCategory;

	if( item ) {
		const QString name = ((ScriptListViewItem *)(item))->m_scriptName;
		m_gui->btnUninstall->setEnabled( true );
		m_gui->btnRun->setEnabled( !m_scripts[name].process );
		m_gui->btnStop->setEnabled( m_scripts[name].process );
		m_gui->btnConfig->setEnabled( m_scripts[name].process );
		m_gui->btnAbout->setEnabled( true );
	}
	else {
		m_gui->btnUninstall->setEnabled( false );
		m_gui->btnRun->setEnabled( false );
		m_gui->btnStop->setEnabled( false );
		m_gui->btnConfig->setEnabled( false );
		m_gui->btnAbout->setEnabled( false );
	}
}

bool EvaScriptManager::slotInstallScript( const QString& path )
{
	QString _path = path;

	if( path.isNull() ) {
		_path = KFileDialog::getOpenFileName( QString::null,
									"*.evascript.tar *.evascript.tar.bz2 *.evascript.tar.gz|"
									+ i18n( "Script Packages (*.evascript.tar, *.evascript.tar.bz2, *.evascript.tar.gz)" )
									, this
									, i18n( "Select Script Package" ) );
		if( _path.isNull() ) return false;
	}

	KTar archive( _path );
	if( !archive.open( IO_ReadOnly ) ) {
		KMessageBox::sorry( 0, i18n( "Could not read this package." ) );
		return false;
	}

	//QString destination = EvaMain::global->getScriptDir() + "/";
	QString destination = KGlobal::dirs()->saveLocation( "data", QString("eva/scripts/"), true);
	const KArchiveDirectory* const archiveDir = archive.directory();

    // Prevent installing a script that's already installed
	const QString scriptFolder = destination + archiveDir->entries().first();
	if( QFile::exists( scriptFolder ) ) {
		KMessageBox::error( 0, i18n( "A script with the name '%1' is already installed. "
				"Please uninstall it first." ).arg( archiveDir->entries().first() ) );
		return false;
	}

	archiveDir->copyTo( destination );
	m_installSuccess = false;
	recurseInstall( archiveDir, destination );

	if( m_installSuccess ) {
		KMessageBox::information( 0, i18n( "Script successfully installed." ) );
		return true;
	}
	else {
		KMessageBox::sorry( 0, i18n( "<p>Script installation failed.</p>"
				"<p>The package did not contain an executable file. "
						"Please inform the package maintainer about this error.</p>" ) );

        // Delete directory recursively
		KIO::NetAccess::del( KURL::fromPathOrURL( scriptFolder ), 0 );
	}

	return false;
}

void EvaScriptManager::recurseInstall( const KArchiveDirectory* archiveDir, const QString& destination )
{
	QStringList entries = archiveDir->entries();

	for( QStringList::Iterator it = entries.begin(); it != entries.end(); ++it ) {
		const QString entry = *it;
		const KArchiveEntry* const archEntry = archiveDir->entry( entry );

		if( archEntry->isDirectory() ) {
			const KArchiveDirectory* const dir = static_cast<const KArchiveDirectory*>( archEntry );
			recurseInstall( dir, destination + entry + '/' );
		}
		else {
				::chmod( QFile::encodeName( destination + entry ), archEntry->permissions() );

				if( QFileInfo( destination + entry ).isExecutable() ) {
					loadScript( destination + entry );
					m_installSuccess = true;
				}
		}
	}
}


void EvaScriptManager::slotUninstallScript()
{
	const QString caption = m_gui->listView->currentItem()->text( 0 );
	const QString name = ((ScriptListViewItem *)(m_gui->listView->currentItem()))->m_scriptName;

	if( KMessageBox::warningContinueCancel( 0, i18n( "Are you sure you want to uninstall the script '%1'?" ).arg( caption ), i18n("Uninstall Script"), i18n("Uninstall") ) == KMessageBox::Cancel )
		return;

	if( m_scripts.find( name ) == m_scripts.end() )
		return;

	const QString directory = m_scripts[name].url.directory();

    // Delete directory recursively
	const KURL url = KURL::fromPathOrURL( directory );
	if( !KIO::NetAccess::del( url, 0 ) ) {
		KMessageBox::sorry( 0, i18n( "<p>Could not uninstall this script.</p><p>The ScriptManager can only uninstall scripts which have been installed as packages.</p>" ) );
		return;
	}

	QStringList keys;

    // Find all scripts that were in the uninstalled folder
	{
		for( ScriptMap::Iterator it = m_scripts.begin(); it != m_scripts.end(); ++it)
			if( it.data().url.directory() == directory )
				keys << it.key();
	}

    // Terminate script processes, remove entries from script list
	{
		for( QStringList::Iterator it = keys.begin(); it != keys.end(); ++it ) {
			delete m_scripts[*it].li;
			terminateProcess( &m_scripts[*it].process );
			releaseScriptResource(*it);
			m_scripts.erase( *it );
		}
	}
}

bool EvaScriptManager::slotRunScript( bool silent )
{
	if( !m_gui->btnRun->isEnabled() ) return false;

	ScriptListViewItem* const li = (ScriptListViewItem *)(m_gui->listView->currentItem());
	//const QString name = li->text( 0 );
	const QString name = li->m_scriptName;

// 	if( m_scripts[name].type == "lyrics" && lyricsScriptRunning() != QString::null ) {
// 		if( !silent )
// 			KMessageBox::sorry( 0, i18n( "Another lyrics script is already running. "
// 					"You may only run one lyrics script at a time." ) );
// 		return false;
// 	}
// 
// 	if( m_scripts[name].type == "transcode" && transcodeScriptRunning() != QString::null ) {
// 		if( !silent )
// 			KMessageBox::sorry( 0, i18n( "Another transcode script is already running. "
// 					"You may only run one transcode script at a time." ) );
// 		return false;
// 	}

    // Don't start a script twice
	if( m_scripts[name].process ) return false;

	//Amarok::ProcIO* script = new Amarok::ProcIO();
	KProcIO * script = new KProcIO(QTextCodec::codecForName( "UTF-8" ));
	script->setComm( static_cast<KProcess::Communication>( KProcess::All ) );
	const KURL url = m_scripts[name].url;
	*script << url.path() << s_evaDcopName;
	script->setWorkingDirectory( KGlobal::dirs()->saveLocation( "data", QString("eva/scripts/%1").arg(name), true));
	//script->setWorkingDirectory( Amarok::saveLocation( "scripts-data/" ) );
	//script->setWorkingDirectory(EvaMain::global->getScriptDir() + "/")
	//script->setWorkingDirectory(url.directory( false ) + "/");

	kdDebug() << name << "||| "<< url << "" << endl;
	connect( script, SIGNAL( receivedStderr( KProcess*, char*, int ) ), SLOT( slotReceivedStderr( KProcess*, char*, int ) ) );
	connect( script, SIGNAL( receivedStdout( KProcess*, char*, int ) ), SLOT( slotReceivedStdout( KProcess*, char*, int ) ) );
	connect( script, SIGNAL( processExited( KProcess* ) ), SLOT( scriptFinished( KProcess* ) ) );

	if( !script->start( KProcess::NotifyOnExit ) )
	{
		if( !silent )
			KMessageBox::sorry( 0, i18n( "<p>Could not start the script <i>%1</i>.</p>"
					"<p>Please make sure that the file has execute (+x) permissions.</p>" ).arg( name ) );
		delete script;
		return false;
	}
	
	EvaImageResource * images = EvaMain::images;
	QPixmap *icon = images->getIcon("SCRIPT_RUN");
	if(icon)
		li->setPixmap( 0,  *icon);
	kdDebug() << "Running script: " << url.path() << endl;

	m_scripts[name].process = script;
	slotCurrentChanged( m_gui->listView->currentItem() );
// 	if( m_scripts[name].type == "lyrics" )
// 		emit lyricsScriptChanged();

	return true;
}

void EvaScriptManager::slotStopScript()
{
	ScriptListViewItem* const li = (ScriptListViewItem *)(m_gui->listView->currentItem());
	//const QString name = li->text( 0 );
	const QString name = li->m_scriptName;

	if( !m_scripts.contains( name ) )
		return;

	terminateProcess( &m_scripts[name].process );
	m_scripts[name].log = QString::null;
	slotCurrentChanged( m_gui->listView->currentItem() );

	li->setPixmap( 0, QPixmap() );

	releaseScriptResource(name);
}

void EvaScriptManager::releaseScriptResource(const QString &scriptName)
{	
	///remove all buttons this script added
	EvaMain::g_mainWin->removeButtons(scriptName);
	///remove all tab pages this script added
	EvaMain::g_mainWin->removeTabs(scriptName);
}

void EvaScriptManager::slotConfigureScript()
{
	kdDebug() << "Configure clicked" << endl;
	const QString name = ((ScriptListViewItem *)(m_gui->listView->currentItem()))->m_scriptName;
	if( !m_scripts[name].process ) return;

	kdDebug() << "Configure clicked --- " << endl;
	const KURL url = m_scripts[name].url;
	QDir::setCurrent( url.directory() );

	m_scripts[name].process->writeStdin( "Configure" );
}

void EvaScriptManager::slotAboutScript()
{
	//const QString name = m_gui->listView->currentItem()->text( 0 );
	const QString name = ((ScriptListViewItem *)(m_gui->listView->currentItem()))->m_scriptName;
	QFile readme( m_scripts[name].url.directory( false ) + "README" );
	QFile license( m_scripts[name].url.directory( false ) + "COPYING" );

	if( !readme.open( IO_ReadOnly ) ) {
		KMessageBox::sorry( 0, i18n( "There is no information available for this script." ) );
		return;
	}

	KAboutDialog* about = new KAboutDialog( KAboutDialog::AbtTabbed|KAboutDialog::AbtProduct,
																					QString::null,
										 KDialogBase::Ok, KDialogBase::Ok, this );
	kapp->setTopWidget( about );
	about->setCaption( kapp->makeStdCaption( i18n( "About %1" ).arg( name ) ) );
	about->setProduct( "", "", "", "" );
    // Get rid of the confusing KDE version text
	QLabel* product = static_cast<QLabel*>( about->mainWidget()->child( "version" ) );
	if( product ) product->setText( i18n( "%1 Eva Script" ).arg( name ) );

	about->addTextPage( i18n( "About" ), QString::fromUtf8(readme.readAll()), true );
	if( license.open( IO_ReadOnly ) )
		about->addLicensePage( i18n( "License" ), license.readAll() );

	about->setInitialSize( QSize( 500, 350 ) );
	about->show();
}

void EvaScriptManager::slotShowContextMenu( QListViewItem* item, const QPoint& pos )
{
// 	const bool isCategory = item == m_generalCategory ||
// 			item == m_lyricsCategory ||
// 			item == m_scoreCategory ||
// 			item == m_transcodeCategory;
// 
// 	if( !item || isCategory ) return;
	if( !item ) return;

    // Look up script entry in our map
	ScriptMap::Iterator it;
	ScriptMap::Iterator end( m_scripts.end() );
	for( it = m_scripts.begin(); it != end; ++it )
		if( it.data().li == item ) break;

	
	EvaImageResource * images = EvaMain::images;
	QPixmap *icon;
	enum { SHOW_LOG, EDIT };
	KPopupMenu menu;
	icon = images->getIcon("NA");
	menu.insertTitle( i18n( "Debugging" ) );
	menu.insertItem( QIconSet(icon?(*icon):QPixmap()), i18n( "Show Output &Log" ), SHOW_LOG );
	icon = images->getIcon("SCRIPT");
	menu.insertItem( QIconSet(icon?(*icon):QPixmap()), i18n( "&Edit" ), EDIT );
	menu.setItemEnabled( SHOW_LOG, it.data().process );
	const int id = menu.exec( pos );

	switch( id )
	{
		case EDIT:
			KRun::runCommand( "kwrite " + it.data().url.path() );
			break;

		case SHOW_LOG:
			QString line;
			while( it.data().process->readln( line ) != -1 )
				it.data().log += (line + "\n");

			KTextEdit* editor = new KTextEdit( it.data().log );
			kapp->setTopWidget( editor );
			editor->setCaption( kapp->makeStdCaption( i18n( "Output Log for %1" ).arg( it.key() ) ) );
			editor->setReadOnly( true );

			QFont font( "fixed" );
			font.setFixedPitch( true );
			font.setStyleHint( QFont::TypeWriter );
			editor->setFont( font );

			editor->setTextFormat( QTextEdit::PlainText );
			editor->resize( 500, 380 );
			editor->show();
			break;
	}
}

/* This is just a workaround, some scripts crash for some people if stdout is not handled. */
void EvaScriptManager::slotReceivedStdout( KProcess*, char* buf, int len )
{
	kdDebug() << QString::fromLatin1( buf, len ) << endl;
}

void EvaScriptManager::slotReceivedStderr( KProcess* process, char* buf, int len )
{
    // Look up script entry in our map
	ScriptMap::Iterator it;
	ScriptMap::Iterator end( m_scripts.end() );
	for( it = m_scripts.begin(); it != end; ++it )
		if( it.data().process == process ) break;

	const QString text = QString::fromLatin1( buf, len );
	kdDebug() << it.key() << ":\n" << text << endl;

	if( it.data().log.length() > 20000 )
		it.data().log = "==== LOG TRUNCATED HERE ====\n";
	it.data().log += text;
}

void EvaScriptManager::scriptFinished( KProcess* process ) //SLOT
{
    // Look up script entry in our map
	ScriptMap::Iterator it;
	ScriptMap::Iterator end( m_scripts.end() );
	for( it = m_scripts.begin(); it != end; ++it )
		if( it.data().process == process ) break;

    // Check if there was an error on exit
	if( process->normalExit() && process->exitStatus() != 0 )
		KMessageBox::detailedError( 0, i18n( "The script '%1' exited with error code: %2" )
				.arg( it.key() ).arg( process->exitStatus() )
						,it.data().log );

    // Destroy script process
	delete it.data().process;
	it.data().process = 0;
	it.data().log = QString::null;
	it.data().li->setPixmap( 0, QPixmap() );
	slotCurrentChanged( m_gui->listView->currentItem() );
	releaseScriptResource(it.key());
}

void EvaScriptManager::terminateProcess( KProcIO** proc )
{
	if( *proc ) {
		(*proc)->kill(); // Sends SIGTERM
		(*proc)->detach();

		delete *proc;
		*proc = 0;
		kdDebug() << "Script process terminated" << endl;
	}
}

void EvaScriptManager::notifyScripts( const QString& message, const unsigned char type )
{
	for( ScriptMap::Iterator it = m_scripts.begin(); it != m_scripts.end(); ++it ) {
		KProcIO* const proc = it.data().process;
		if( proc && (it.data().type & type) )
			proc->writeStdin( message );
	}
}

void EvaScriptManager::notifyScript( const QString& name, const QString message )
{
	for(ScriptMap::Iterator it = m_scripts.begin();
		it != m_scripts.end();
		it++)
	{
		kdDebug() << "Script -- " << it.key() << endl;
	}
	if( m_scripts.find( name ) == m_scripts.end() )
		return;
	
	kdDebug() << "check process" << endl;
	if( !m_scripts[name].process ) return; // not running
	
	const KURL url = m_scripts[name].url;
	QDir::setCurrent( url.directory() );
	
	kdDebug() << message << endl;
	m_scripts[name].process->writeStdin( message );	
}

void EvaScriptManager::loadScript( const QString& path )
{
	if( !path.isEmpty() ) {
		const KURL url = KURL::fromPathOrURL( path );
		QString name = url.fileName();
		QString caption = name;
		//QString type = "generic";
		unsigned char type = MT_Eva;

        	// Read and parse .spec file, if exists
		QFileInfo info( path );
		//KListViewItem* li = 0;
		ScriptListViewItem * li = 0;
		const QString specPath = info.dirPath() + '/' + info.baseName( true ) + ".spec";
		if( QFile::exists( specPath ) ) {
			KConfig spec( specPath, true, false );
			if( spec.hasKey( "name" ) )
				caption = name = spec.readEntry( "name" );
			if( spec.hasKey( "caption" ))
				caption = spec.readEntry( "caption" );
			if( spec.hasKey( "type" ) ){
				QString types = spec.readEntry( "type");
				QStringList typeList = QStringList::split(",", types);
				for(QStringList::Iterator it = typeList.begin(); it != typeList.end(); ++it){
					if((*it).stripWhiteSpace().upper() == QString("MESSAGE"))
						type |= MT_Message;
					if((*it).stripWhiteSpace().upper() == QString("STATUS"))
						type |= MT_Status;
					if((*it).stripWhiteSpace().upper() == QString("BUTTONCLICK"))
						type |= MT_ButtonClick;
				}
			}
		} else // no spec file.
			return;

		if( !li )
			li = new ScriptListViewItem(m_gui->listView, caption);
			//li = new KListViewItem( m_generalCategory, name );

		li->setPixmap( 0, QPixmap() );
		li->m_scriptName = name;

		ScriptItem item;
		item.url = url;
		item.type = type;
		item.process = 0;
		item.li = li;

		m_scripts[name] = item;

		kdDebug() << "Loaded: " << name << "(" << caption << ") "  << url << endl;

		slotCurrentChanged( m_gui->listView->currentItem() );
	}
}

void EvaScriptManager::notifyStatusChange(const unsigned int id)
{
	kdDebug() << "notifyStatusChange" << endl;
	notifyScripts(QString("StatusChanged %1").arg(id), MT_Status);
}

void EvaScriptManager::notifyMessage(const bool isQun, 
																		const unsigned int id, 
																		const QDateTime &time, 
																		const QString &message)
{
	QString revised = message;
	revised.replace('\n', "\n#");
	notifyScripts(QString("Message %1 %2 %3 \n#%4\n@")
					.arg(isQun?"Qun":"Contact")
					.arg(id)
					.arg(time.toString("yyyy-MM-dd hh:mm:ss"))
					.arg(revised),
				MT_Message);
}

void EvaScriptManager::customMenuClicked( const bool isQun, const unsigned int id, QString scriptName, QString name)
{
	kdDebug() << "customMenuClicked :" << scriptName << "(" << name << ")" << endl;
	notifyScript(scriptName, QString("ButtonClicked %1 %2 %3")
							.arg(isQun?"Qun":"Contact")
							.arg(id)
							.arg(name));
}

QString EvaScriptManager::scriptDir( bool isSystem )
{
	QString dir;
	if( isSystem )
		dir = (EvaMain::global->getDirPath() + "/scripts/");
	else
	  dir = (KGlobal::dirs()->saveLocation( "data", QString("eva/scripts/"), true));
	return dir;
}

int EvaScriptManager::numScripts( )
{
	return m_scripts.size();
}

int EvaScriptManager::numRunningScripts( )
{
	int count = 0;
	for(ScriptMap::Iterator it = m_scripts.begin(); it != m_scripts.end(); it++){
		if( it.data().process )
			count++;
	}
	return count;
}

