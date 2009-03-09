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

#include "evachatview.h"
#include "evahtmlparser.h"
#include "evaresource.h"
#include "evamain.h"
#include <qstringlist.h>
#include <qtimer.h>
#include <qclipboard.h>
#include <qfileinfo.h>
#include <qdragobject.h>
#include <qapplication.h>

#include <kaction.h>
#include <kconfig.h>
#include <kservice.h>
#include <kurifilter.h>
#include <kmimetype.h>
#include <kstandarddirs.h> // function "locate"
#include <kiconloader.h>   // function "SmallIcon"
#include <kdesktopfile.h>
#include <kurldrag.h>
#include <kmultipledrag.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kio/job.h>
#include <kpopupmenu.h>
#include <kapplication.h>
#include <klocale.h>

/// defines come from khtml/misc/htmltags.h
//#define ATTR_HREF 54
//#define ATTR_TARGET 133
#define ID_IMG    48
#include <dom/html_element.h>
#include <dom/html_image.h>

#define SCROLL_DELAY    200
#define MIN_FONT_SIZE   8
#define MAX_FONT_SIZE   20

class MenuPrivateData
{
public:
  KURL m_url;
  KURL m_imageURL;
  QPixmap m_pixmap;
  QString m_suggestedFilename;
};

const QString EvaChatView::protocolAccept = "cmd://accept";
const QString EvaChatView::protocolSaveAs = "cmd://saveas";
const QString EvaChatView::protocolCancel = "cmd://cancel";
const QString EvaChatView::protocolResume = "cmd://resume";
const QString EvaChatView::protocolNewOne = "cmd://newone";

EvaChatView::EvaChatView( QWidget * parent, const char * name )
	: KHTMLPart(parent, name), menu(NULL), d(0), buffer(""), m_isScrollAtBottom(true)
{
	//setOnlyLocalReferences(true);
	menu = new KPopupMenu(0, "popup");
	copyAction = KStdAction::copy( this, SLOT(copy()), actionCollection());
	copyAction->setText(i18n("&Copy Text"));
	copyAction->setShortcut( KShortcut("Ctrl+C"));

	setDNDEnabled(true);
	setAutoloadImages(true);
	view()->setHScrollBarMode(QScrollView::AlwaysOff);
	
	//setStandardFont("Helvetica");
	//buffer += "<body style=\"font-size:9pt;font-family:Helvetica\">";
	
	QObject::connect(view(), SIGNAL(finishedLayout()), SLOT(slotScrollToBottom()));
	QObject::connect(this, SIGNAL(selectionChanged()), SLOT(slotSelectionChanged()));
	QObject::connect(this, SIGNAL(popupMenu(const QString &, const QPoint &)), 
			SLOT(slotPopupMenu(const QString &, const QPoint &)));
	QObject::connect(browserExtension(), SIGNAL(openURLRequest(const KURL &, const KParts::URLArgs &)), 
			SLOT(slotLinkClicked( const KURL &, const KParts::URLArgs &)));
}

EvaChatView::~EvaChatView()
{
	if(menu) delete menu;
	if(d) delete d;
}

QString EvaChatView::wrapFontAttributes(QColor color, Q_UINT8 size,
                                            bool underline, bool italic, bool bold,
                                            QString contents)
{
	QString fontHead = "<span style=\"";
	QString fontSize = "font-size: "+QString::number((size<=MIN_FONT_SIZE)?MIN_FONT_SIZE:((size>MAX_FONT_SIZE)?MAX_FONT_SIZE:size))+"pt; ";

	QString fontColor = QString("color: ") + color.name() + QString("\" >");
	
	QString fontStye = "", fontStyeEnd="";
	if(bold){
		fontStye+="<b>";
		fontStyeEnd+="</b>";
	}
	if(italic){
		fontStye+="<i>";
		fontStyeEnd="</i>" + fontStyeEnd;
	}
	if(underline){
		fontStye+="<u>";
		fontStyeEnd="</u>" + fontStyeEnd;
	}
	QString fontEnd = "</span>";
	QString ret = fontHead + fontSize + fontColor + fontStye + contents + fontStyeEnd + fontEnd;
	return ret;
}

QString EvaChatView::wrapNickName(QString &nick, QDateTime time, QColor color, bool isNormal)
{	
	QString htmlName = nick;
	EvaHtmlParser parser;
	parser.setAbsImagePath(EvaMain::images->getSmileyPath());
	parser.convertToHtml(htmlName, false, true);
	QString msg = "<span style=\"font-size: 9pt; color: " + color.name() +"\">" + htmlName + " ";
	if(!isNormal)
		msg+=i18n("(Auto-Reply)");

	QDateTime current = QDateTime::currentDateTime();
	QString dateFormat = "hh:mm:ss";
	if(current.date() != time.date())
		dateFormat = "yyyy-MM-dd hh:mm:ss";
	msg+="  "+time.toString(dateFormat) + "</span><br>";
	return msg;
}

void EvaChatView::append( QString & nick, QDateTime time, QColor nameColor, bool isNormal, 
				QColor msgColor, Q_UINT8 size, 
				bool underline, bool italic, bool bold, QString contents )
{
	QString msg = wrapNickName(nick, time, nameColor, isNormal) + 
			wrapFontAttributes(msgColor, size, underline, italic, bold, contents);
	updateContents(msg);
}

void EvaChatView::updatePicture( const QString filename , const QString tmpFileName)
{
	m_isScrollAtBottom =  ( view()->contentsHeight() == (view()->contentsY() +view()->visibleHeight()) );
	buffer.replace(tmpFileName, filename);
	begin();
	write(buffer);
	write("</body>");
	end();
}

void EvaChatView::slotScrollToBottom()
{
	if(m_isScrollAtBottom){
		view()->setContentsPos(view()->contentsX(), view()->contentsHeight() - view()->visibleHeight() );
	}
}

void EvaChatView::slotLinkClicked( const KURL & url, const KParts::URLArgs &/*args*/)
{
	QString cmd = url.url();
	QString strSession = cmd.mid(13,cmd.length() - 13);
	bool ok;
	unsigned int session = strSession.toUInt(&ok);
	if(ok){
		if(cmd.startsWith(protocolAccept) ){
			emit fileTransferAcceptRequest(session);
			return;
		}
		if(cmd.startsWith(protocolSaveAs) ){
			emit fileTransferSaveAsRequest(session);
			return;
		}
		if(cmd.startsWith(protocolCancel) ){
			emit fileTransferCancelRequest(session);
			return;
		}
		if(cmd.startsWith(protocolResume) ){
			emit fileTransferResume(session, true);
			return;
		}
		if(cmd.startsWith(protocolNewOne) ){
			emit fileTransferResume(session, false);
			return;
		}
	}

	QStringList args;
	if(url.isLocalFile()){
		args<<"exec"<< QString::fromLocal8Bit(url.path().ascii());
	}else{
		args<<"exec" <<cmd;
	}
	kapp->kdeinitExec("kfmclient",args);
}

/*
   we just simplify the process. if we use KParts::BrowserExtension, we have to do
   lots extra work, adding so much classes. so just hack like following.

   grab useful code from KHTMLPopupGUIClient(khtml_ext.cpp),
   and change a little bit to fit our needs

*/
void EvaChatView::slotPopupMenu( const QString & _url, const QPoint & point )
{
	menu->clear();

	bool isImage = false;
	bool hasSelection = KHTMLPart::hasSelection();
	KURL url = KURL(_url);

	if(d) delete d;
	d = new MenuPrivateData;
	d->m_url = url;


	DOM::Element e = nodeUnderMouse();
	if ( !e.isNull() && (e.elementId() == ID_IMG) ){
		DOM::HTMLImageElement ie = static_cast<DOM::HTMLImageElement>(e);
		QString src = ie.src().string();
		d->m_imageURL = KURL(src);
		d->m_suggestedFilename = src.right(src.length() - src.findRev("/") -1);
		isImage=true;
	}

	
	KAction *action = 0L;

	if(hasSelection){
		//action =  new KAction( i18n( "&Copy Text" ), KShortcut("Ctrl+C"), this, SLOT( copy() ),
		//			actionCollection(), "copy" );
		//action = KStdAction::copy( browserExtension(), SLOT(copy()), actionCollection(), "copy");
		//action->setText(i18n("&Copy Text"));
		//action->setEnabled(true);
		copyAction->plug(menu);
		
		// search text
		QString selectedText = KHTMLPart::selectedText();
		if ( selectedText.length()>18 ) {
			selectedText.truncate(15);
			selectedText+="...";
		}
#ifdef HAS_KONQUEROR
		// Fill search provider entries
		KConfig config("kuriikwsfilterrc");
		config.setGroup("General");
		const QString defaultEngine = config.readEntry("DefaultSearchEngine", "google");
		const char keywordDelimiter = config.readNumEntry("KeywordDelimiter", ':');
		

		// default search provider
		KService::Ptr service = KService::serviceByDesktopPath(QString("searchproviders/%1.desktop").arg(defaultEngine));
		
		// search provider icon
		QPixmap icon;
		KURIFilterData data;
		QStringList list;
		const QString defaultSearchProviderPrefix = *(service->property("Keys").toStringList().begin()) + keywordDelimiter;
		data.setData( defaultSearchProviderPrefix + QString("some keyword") );
		list << "kurisearchfilter" << "kuriikwsfilter";
		
		QString name;
		if ( KURIFilter::self()->filterURI(data, list) ){
			QString iconPath = locate("cache", KMimeType::favIconForURL(data.uri()) + ".png");
			if ( iconPath.isEmpty() )
				icon = SmallIcon("find");
			else
				icon = QPixmap( iconPath );
	
			name = service->name();
		} else {
			icon = SmallIcon("google");
			name = "Google";
		}
		
		action = new KAction( i18n( "Search '%1' at %2" ).arg( selectedText ).arg( name ), icon, 0, this,
				SLOT( searchProvider() ), actionCollection(), "searchProvider" );
		action->plug(menu);

		// favorite search providers
		QStringList favoriteEngines;
		favoriteEngines = config.readListEntry("FavoriteSearchEngines"); // for KDE 3.2 API compatibility
		if(favoriteEngines.isEmpty())
			favoriteEngines << "google" << "google_groups" << "google_news" << "webster" << "dmoz" << "wikipedia";
		
		if ( !favoriteEngines.isEmpty()) {
			KActionMenu* providerList = new KActionMenu( i18n( "Search '%1' At" ).arg( selectedText ), actionCollection(), "searchProviderList" );
			bool hasSubMenus = false;
			QStringList::ConstIterator it = favoriteEngines.begin();
			for ( ; it != favoriteEngines.end(); ++it ) {
				if (*it==defaultEngine)
					continue;
				service = KService::serviceByDesktopPath(QString("searchproviders/%1.desktop").arg(*it));
				if (!service)
					continue;
				const QString searchProviderPrefix = *(service->property("Keys").toStringList().begin()) + keywordDelimiter;
				data.setData( searchProviderPrefix + "some keyword" );
			
				if ( KURIFilter::self()->filterURI(data, list) ) {
					QString iconPath = locate("cache", KMimeType::favIconForURL(data.uri()) + ".png");
					if ( iconPath.isEmpty() )
						icon = SmallIcon("find");
					else
						icon = QPixmap( iconPath );
					name = service->name();
			
					providerList->insert( new KAction( name, icon, 0, this,
							SLOT( searchProvider() ), actionCollection(), QString( "searchProvider" + searchProviderPrefix ).latin1() ) );
					hasSubMenus = true;
				}
			}
			if(hasSubMenus) providerList->plug(menu);
		}
#endif // HAS_KONQUEROR
		if ( selectedText.contains("://") && KURL(selectedText).isValid() ) {
			action = new KAction( i18n( "Open '%1'" ).arg( selectedText ), "window_new", 0,
							this, SLOT( openSelection() ), actionCollection(), "openSelection" );
			action->plug(menu);
		}
	}
	if ( !url.isEmpty() ) {
		if (url.protocol() == "mailto")	{
			action = new KAction( i18n( "Copy Email Address" ), 0, this, SLOT( slotCopyLinkLocation() ),
					actionCollection(), "copylinklocation" );
			action->plug(menu);
		} else {
			action = new KAction( i18n( "Copy &Link Address" ), 0, this, SLOT( slotCopyLinkLocation() ),
					actionCollection(), "copylinklocation" );
			action->plug(menu);
		}
	}

	if (isImage)	{
#ifndef QT_NO_MIMECLIPBOARD
		action = (new KAction( i18n( "Copy Image" ), 0, this, SLOT( slotCopyImage() ),
				actionCollection(), "copyimage" ));
		action->plug(menu);
#endif
		action = new KAction( i18n( "Save Image As..." ), 0, this, SLOT( slotSaveImageAs() ),
				actionCollection(), "saveimageas" );
		action->plug(menu);

		action = new KAction( i18n( "Save As Custom Smiley"), 0, this, SLOT( slotSaveAsCustomSmiley() ),
				actionCollection(), "saveascustomsmiley" );
		action->plug(menu);
	}
	
	if(menu->count()) menu->popup(point);
}

void EvaChatView::copy( )
{
	if(hasSelection()){
		QString text = selectedText();
		text.replace(QChar(0xa0), ' ');
		QApplication::clipboard()->setText( text, QClipboard::Clipboard );
		QApplication::clipboard()->setText( text, QClipboard::Selection );
	}
}

void EvaChatView::searchProvider()
{
	// action name is of form "previewProvider[<searchproviderprefix>:]"
	const QString searchProviderPrefix = QString( sender()->name() ).mid( 14 );
	
	KURIFilterData data;
	QStringList list;
	data.setData( searchProviderPrefix + this->selectedText() );
	list << "kurisearchfilter" << "kuriikwsfilter";
	
	if( !KURIFilter::self()->filterURI(data, list) )
	{
	KDesktopFile file("searchproviders/google.desktop", true, "services");
	data.setData(file.readEntry("Query").replace("\\{@}", this->selectedText()));
	}
	
	KParts::URLArgs args;
	args.frameName = "_blank";
	
	emit slotLinkClicked( data.uri(), args );
}

void EvaChatView::openSelection()
{
	KParts::URLArgs args;
	args.frameName = "_blank";
	
	emit slotLinkClicked( d->m_url, args );
}

void EvaChatView::slotCopyLinkLocation()
{
	KURL safeURL(d->m_url);
	safeURL.setPass(QString::null);
#ifndef QT_NO_MIMECLIPBOARD
	// Set it in both the mouse selection and in the clipboard
	KURL::List lst;
	lst.append( safeURL );
	QApplication::clipboard()->setData( new KURLDrag( lst ), QClipboard::Clipboard );
	QApplication::clipboard()->setData( new KURLDrag( lst ), QClipboard::Selection );
#else
	QApplication::clipboard()->setText( safeURL.url() ); 
#endif
}

void EvaChatView::slotCopyImage()
{
#ifndef QT_NO_MIMECLIPBOARD
	KURL safeURL(d->m_imageURL);
	safeURL.setPass(QString::null);
	
	KURL::List lst;
	lst.append( safeURL );
	KMultipleDrag *drag = new KMultipleDrag(view(), "Image");

	drag->addDragObject( new QImageDrag(d->m_imageURL.path()) );
	drag->addDragObject( new KURLDrag(lst, view(), "Image URL") );
	
	// Set it in both the mouse selection and in the clipboard
	QApplication::clipboard()->setData( drag, QClipboard::Clipboard );
	QApplication::clipboard()->setData( new KURLDrag(lst), QClipboard::Selection );
#else
	// do nothing
#endif
}

void EvaChatView::slotSaveImageAs()
{
	QString name = QString::fromLatin1("index.html");;
	if ( !d->m_suggestedFilename.isEmpty() )
		name = d->m_suggestedFilename;
	else if ( !d->m_imageURL.fileName().isEmpty() )
		name = d->m_imageURL.fileName();

	KURL destURL;
	int query;
	do {
		query = KMessageBox::Yes;
		destURL = KFileDialog::getSaveURL( QDir::homeDirPath() + "/" + name, QString::null, 0, i18n( "Save Image As" ) );
		if( destURL.isLocalFile() ) {
			QFileInfo info( destURL.path() );
			if( info.exists() ) {
				// TODO: use KIO::RenameDlg (shows more information)
				query = KMessageBox::warningContinueCancel( 0, i18n( "A file named \"%1\" already exists. " "Are you sure you want to overwrite it?" ).arg( info.fileName() ), i18n( "Overwrite File?" ), i18n( "Overwrite" ) );
			}
		}
	} while ( query == KMessageBox::Cancel );
	
	if ( destURL.isValid() )
		KIO::file_copy(d->m_imageURL, destURL, -1, true /*overwrite*/);
}

void EvaChatView::slotSaveAsCustomSmiley()
{
	emit saveAsCustomSmiley(d->m_imageURL.path());
}

// it seems this method doesn't do the job
void EvaChatView::startDrag( )
{
	QDragObject *d = new QTextDrag(selectedText(), view());
	d->dragCopy();
}

void EvaChatView::slotSelectionChanged( )
{
	copyAction->setEnabled( hasSelection() );
}

void EvaChatView::showInfomation( const QString & info )
{
	QString picPath = "<img src = \"" + EvaMain::images->getIconFullPath("MSG_INFO") + "\">";
	QString msg = wrapFontAttributes(Qt::gray, 9, false, false, false, info);
	updateContents("&nbsp;" + picPath + "&nbsp;" + msg );
	showContents();
}

void EvaChatView::showFileNotification( const QString & who, const QString & filename, 
			const int size, const unsigned int session, const bool isSend )
{
	QString strSession = QString::number(session);
	QString picPath = "<img src = \"" + EvaMain::images->getIconFullPath("MSG_INFO") + "\">";
	QString acceptLink = "<a href=\""+ protocolAccept + "_" + strSession + "\">" + i18n("Accept") + "</a>";
	QString saveAsLink = "<a href=\""+ protocolSaveAs + "_" + strSession + "\">" + i18n("Save As") + "</a>";
        QString rejectLink = "<a href=\""+ protocolCancel + "_" + strSession + "\">" + i18n("Reject") + "</a>";
	QString cancelLink = "<a href=\""+ protocolCancel + "_" + strSession + "\">" + i18n("Cancel") + "</a>";
	QString fileSize = (size<0x400)?(QString::number(size) + "B"):
			((size<0x100000)?(QString::number(size/0x400) + "KB") :
			((size<0x40000000)?(QString::number(size/0x100000) + "MB"):
			(QString::number(size/0x40000000) + "GB") ) );
	QString fileInfo = filename + "(" + fileSize + ")";
        QString txt;
	if(isSend){
		txt = i18n("Waiting for %1 accepting file \"%2\". Please wait or %3.")
				.arg(who).arg(fileInfo).arg(cancelLink);
	}else{
		txt = i18n("%1 wants to send you a file \"%2\", you can %3, %4 or %5.")
				.arg(who).arg(fileInfo).arg(acceptLink).arg(saveAsLink).arg(rejectLink);
	}
	QString msg = wrapFontAttributes(Qt::gray, 9, false, false, false, txt);
	updateContents("&nbsp;" + picPath + "&nbsp;" + msg );
	showContents();
}

void EvaChatView::updateContents( const QString & contents )
{
	//QString newMsg = contents;
	//Wonder why the following line was added, it does nothing other than extra mess spaces.
	//newMsg.replace("&nbsp;", "&nbsp; ");
	buffer += (contents + "<br>");
}

void EvaChatView::showContents()
{
	m_isScrollAtBottom =  ( view()->contentsHeight() == (view()->contentsY() +view()->visibleHeight()) );

	begin();
	write(buffer);
	write("</body>");
	end();
}

void EvaChatView::askResumeMode( const QString filename, const unsigned int session )
{
	QString strSession = QString::number(session);
	QString picPath = "<img src = \"" + EvaMain::images->getIconFullPath("MSG_INFO") + "\">";
	QString resumeLink = "<a href=\""+ protocolResume + "_" + strSession + "\">" + 
			i18n("resume") + "</a>";
	QString restartLink = "<a href=\""+ protocolNewOne + "_" + strSession + "\">" + 
			i18n("start") + "</a>";
	QString txt = i18n("Cached information of \"%1\" has been found, you can %2 the last download or ignore the last cached download information and %3 a new download.").arg(filename).arg(resumeLink).arg(restartLink);

	QString msg = wrapFontAttributes(Qt::gray, 9, false, false, false, txt);
	updateContents("&nbsp;" + picPath + "&nbsp;" + msg );
	showContents();
}



