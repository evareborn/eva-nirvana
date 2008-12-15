#include "webkitchatview.h"

#include "evahtmlparser.h"
#include "evaresource.h"
#include "evaguimain.h"
#include "defines.h"
#include <qstringlist.h>
#include <qtimer.h>
#include <qclipboard.h>
#include <qfileinfo.h>
#include <qapplication.h>
#include <QWebFrame>
#include <QMenu>

#define MIN_FONT_SIZE   8
#define MAX_FONT_SIZE   20

WebkitChatView::WebkitChatView( QWidget * parent, const char * /*name*/ )
//X 	: QWebPage(parent, name), menu(NULL), d(0), buffer(""), m_isScrollAtBottom(true)
	: QWebView(parent), menu(NULL), buffer(""), m_isScrollAtBottom(true)
{
	//setOnlyLocalReferences(true);
	menu = new QMenu("popup");
//X 	copyAction = KStdAction::copy( this, SLOT(copy()), actionCollection());
//X 	copyAction->setText(i18n("&Copy Text"));
//X 	copyAction->setShortcut( KShortcut("Ctrl+C"));

//X 	setDNDEnabled(true);
//X 	setAutoloadImages(true);
//X 	view()->setHScrollBarMode(QScrollView::AlwaysOff);
	
        page( )->setLinkDelegationPolicy( QWebPage::DelegateAllLinks );
	//setStandardFont("Helvetica");
	//buffer += "<body style=\"font-size:9pt;font-family:Helvetica\">";
	
//X 	QObject::connect(this, SIGNAL(finishedLayout()), SLOT(slotScrollToBottom()));
	QObject::connect(page(), SIGNAL(loadFinished(bool)), SLOT(slotScrollToBottom()));
	QObject::connect(this, SIGNAL(selectionChanged()), SLOT(slotSelectionChanged()));
//X 	QObject::connect(this, SIGNAL(popupMenu(const QString &, const QPoint &)), 
//X 			SLOT(slotPopupMenu(const QString &, const QPoint &)));
//X 	QObject::connect(browserExtension(), SIGNAL(openURLRequest(const KURL &, const KParts::URLArgs &)), 
//X 			SLOT(slotLinkClicked( const KURL &, const KParts::URLArgs &)));
}

WebkitChatView::~WebkitChatView()
{
	if(menu) delete menu;
//X 	if(d) delete d;
}

QString WebkitChatView::wrapFontAttributes(QColor color, Q_UINT8 size,
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

QString WebkitChatView::wrapNickName(QString &nick, QDateTime time, QColor color, bool isNormal)
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

void WebkitChatView::append( QString & nick, QDateTime time, QColor nameColor, bool isNormal, 
				QColor msgColor, Q_UINT8 size, 
				bool underline, bool italic, bool bold, QString contents )
{
	QString msg = wrapNickName(nick, time, nameColor, isNormal) + 
			wrapFontAttributes(msgColor, size, underline, italic, bold, contents);
	updateContents(msg);
}

void WebkitChatView::updatePicture( const QString filename , const QString tmpFileName)
{
//X 	m_isScrollAtBottom =  ( contentsHeight() == (contentsY() + visibleHeight()) );
	buffer.replace(tmpFileName, filename);
//X 	begin();
	setHtml(buffer);
//X 	write("</body>");
//X 	end();
}

void WebkitChatView::slotScrollToBottom()
{
//X 	if(m_isScrollAtBottom){
//X 		setContentsPos(contentsX(), contentsHeight() - visibleHeight() );
//X 	}
        page()->mainFrame()->setScrollBarValue( Qt::Vertical, page()->mainFrame()->scrollBarMaximum( Qt::Vertical ) );
}

//X void WebkitChatView::slotLinkClicked( const KURL & url, const KParts::URLArgs &/*args*/)
//X {
//X 	QString cmd = url.url();
//X 	QString strSession = cmd.mid(13,cmd.length() - 13);
//X 	bool ok;
//X 	unsigned int session = strSession.toUInt(&ok);
//X 	if(ok){
//X 		if(cmd.startsWith(protocolAccept) ){
//X 			emit fileTransferAcceptRequest(session);
//X 			return;
//X 		}
//X 		if(cmd.startsWith(protocolSaveAs) ){
//X 			emit fileTransferSaveAsRequest(session);
//X 			return;
//X 		}
//X 		if(cmd.startsWith(protocolCancel) ){
//X 			emit fileTransferCancelRequest(session);
//X 			return;
//X 		}
//X 		if(cmd.startsWith(protocolResume) ){
//X 			emit fileTransferResume(session, true);
//X 			return;
//X 		}
//X 		if(cmd.startsWith(protocolNewOne) ){
//X 			emit fileTransferResume(session, false);
//X 			return;
//X 		}
//X 	}
//X 
//X 	QStringList args;
//X 	if(url.isLocalFile()){
//X 		args<<"exec"<< QString::fromLocal8Bit(url.path().ascii());
//X 	}else{
//X 		args<<"exec" <<cmd;
//X 	}
//X 	kapp->kdeinitExec("kfmclient",args);
//X }

/*
   we just simplify the process. if we use KParts::BrowserExtension, we have to do
   lots extra work, adding so much classes. so just hack like following.

   grab useful code from KHTMLPopupGUIClient(khtml_ext.cpp),
   and change a little bit to fit our needs

*/
void WebkitChatView::slotPopupMenu( const QString & /*_url*/, const QPoint & /*point*/ )
{
//X 	menu->clear();
//X 
//X 	bool isImage = false;
//X 	bool hasSelection = QWebPage::hasSelection();
//X 	KURL url = KURL(_url);
//X 
//X 	if(d) delete d;
//X 	d = new MenuPrivateData;
//X 	d->m_url = url;
//X 
//X 
//X 	DOM::Element e = nodeUnderMouse();
//X 	if ( !e.isNull() && (e.elementId() == ID_IMG) ){
//X 		DOM::HTMLImageElement ie = static_cast<DOM::HTMLImageElement>(e);
//X 		QString src = ie.src().string();
//X 		d->m_imageURL = KURL(src);
//X 		d->m_suggestedFilename = src.right(src.length() - src.findRev("/") -1);
//X 		isImage=true;
//X 	}
//X 
//X 	
//X 	KAction *action = 0L;
//X 
//X 	if(hasSelection){
//X 		//action =  new KAction( i18n( "&Copy Text" ), KShortcut("Ctrl+C"), this, SLOT( copy() ),
//X 		//			actionCollection(), "copy" );
//X 		//action = KStdAction::copy( browserExtension(), SLOT(copy()), actionCollection(), "copy");
//X 		//action->setText(i18n("&Copy Text"));
//X 		//action->setEnabled(true);
//X 		copyAction->plug(menu);
//X 		
//X 		// search text
//X 		QString selectedText = QWebPage::selectedText();
//X 		if ( selectedText.length()>18 ) {
//X 			selectedText.truncate(15);
//X 			selectedText+="...";
//X 		}
//X #ifdef HAS_KONQUEROR
//X 		// Fill search provider entries
//X //X 		KConfig config("kuriikwsfilterrc");
//X //X 		config.setGroup("General");
//X //X 		const QString defaultEngine = config.readEntry("DefaultSearchEngine", "google");
//X //X 		const char keywordDelimiter = config.readNumEntry("KeywordDelimiter", ':');
//X 		
//X 
//X 		// default search provider
//X 		KService::Ptr service = KService::serviceByDesktopPath(QString("searchproviders/%1.desktop").arg(defaultEngine));
//X 		
//X 		// search provider icon
//X 		QPixmap icon;
//X 		KURIFilterData data;
//X 		QStringList list;
//X 		const QString defaultSearchProviderPrefix = *(service->property("Keys").toStringList().begin()) + keywordDelimiter;
//X 		data.setData( defaultSearchProviderPrefix + QString("some keyword") );
//X 		list << "kurisearchfilter" << "kuriikwsfilter";
//X 		
//X 		QString name;
//X 		if ( KURIFilter::self()->filterURI(data, list) ){
//X 			QString iconPath = locate("cache", KMimeType::favIconForURL(data.uri()) + ".png");
//X 			if ( iconPath.isEmpty() )
//X 				icon = SmallIcon("find");
//X 			else
//X 				icon = QPixmap( iconPath );
//X 	
//X 			name = service->name();
//X 		} else {
//X 			icon = SmallIcon("google");
//X 			name = "Google";
//X 		}
//X 		
//X 		action = new KAction( i18n( "Search '%1' at %2" ).arg( selectedText ).arg( name ), icon, 0, this,
//X 				SLOT( searchProvider() ), actionCollection(), "searchProvider" );
//X 		action->plug(menu);
//X 
//X 		// favorite search providers
//X 		QStringList favoriteEngines;
//X //X 		favoriteEngines = config.readListEntry("FavoriteSearchEngines"); // for KDE 3.2 API compatibility
//X 		if(favoriteEngines.isEmpty())
//X 			favoriteEngines << "google" << "google_groups" << "google_news" << "webster" << "dmoz" << "wikipedia";
//X 		
//X 		if ( !favoriteEngines.isEmpty()) {
//X 			KActionMenu* providerList = new KActionMenu( i18n( "Search '%1' At" ).arg( selectedText ), actionCollection(), "searchProviderList" );
//X 			bool hasSubMenus = false;
//X 			QStringList::ConstIterator it = favoriteEngines.begin();
//X 			for ( ; it != favoriteEngines.end(); ++it ) {
//X 				if (*it==defaultEngine)
//X 					continue;
//X 				service = KService::serviceByDesktopPath(QString("searchproviders/%1.desktop").arg(*it));
//X 				if (!service)
//X 					continue;
//X 				const QString searchProviderPrefix = *(service->property("Keys").toStringList().begin()) + keywordDelimiter;
//X 				data.setData( searchProviderPrefix + "some keyword" );
//X 			
//X 				if ( KURIFilter::self()->filterURI(data, list) ) {
//X 					QString iconPath = locate("cache", KMimeType::favIconForURL(data.uri()) + ".png");
//X 					if ( iconPath.isEmpty() )
//X 						icon = SmallIcon("find");
//X 					else
//X 						icon = QPixmap( iconPath );
//X 					name = service->name();
//X 			
//X 					providerList->insert( new KAction( name, icon, 0, this,
//X 							SLOT( searchProvider() ), actionCollection(), QString( "searchProvider" + searchProviderPrefix ).latin1() ) );
//X 					hasSubMenus = true;
//X 				}
//X 			}
//X 			if(hasSubMenus) providerList->plug(menu);
//X 		}
//X #endif // HAS_KONQUEROR
//X 		if ( selectedText.contains("://") && KURL(selectedText).isValid() ) {
//X 			action = new KAction( i18n( "Open '%1'" ).arg( selectedText ), "window_new", 0,
//X 							this, SLOT( openSelection() ), actionCollection(), "openSelection" );
//X 			action->plug(menu);
//X 		}
//X 	}
//X 	if ( !url.isEmpty() ) {
//X 		if (url.protocol() == "mailto")	{
//X 			action = new KAction( i18n( "Copy Email Address" ), 0, this, SLOT( slotCopyLinkLocation() ),
//X 					actionCollection(), "copylinklocation" );
//X 			action->plug(menu);
//X 		} else {
//X 			action = new KAction( i18n( "Copy &Link Address" ), 0, this, SLOT( slotCopyLinkLocation() ),
//X 					actionCollection(), "copylinklocation" );
//X 			action->plug(menu);
//X 		}
//X 	}
//X 
//X 	if (isImage)	{
//X #ifndef QT_NO_MIMECLIPBOARD
//X 		action = (new KAction( i18n( "Copy Image" ), 0, this, SLOT( slotCopyImage() ),
//X 				actionCollection(), "copyimage" ));
//X 		action->plug(menu);
//X #endif
//X 		action = new KAction( i18n( "Save Image As..." ), 0, this, SLOT( slotSaveImageAs() ),
//X 				actionCollection(), "saveimageas" );
//X 		action->plug(menu);
//X 
//X 		action = new KAction( i18n( "Save As Custom Smiley"), 0, this, SLOT( slotSaveAsCustomSmiley() ),
//X 				actionCollection(), "saveascustomsmiley" );
//X 		action->plug(menu);
//X 	}
//X 	
//X 	if(menu->count()) menu->popup(point);
}

void WebkitChatView::copy( )
{
//X 	if(hasSelection()){
//X 		QString text = selectedText();
//X 		text.replace(QChar(0xa0), ' ');
//X 		QApplication::clipboard()->setText( text, QClipboard::Clipboard );
//X 		QApplication::clipboard()->setText( text, QClipboard::Selection );
//X 	}
}

void WebkitChatView::searchProvider()
{
//X 	// action name is of form "previewProvider[<searchproviderprefix>:]"
//X 	const QString searchProviderPrefix = QString( sender()->name() ).mid( 14 );
//X 	
//X 	KURIFilterData data;
//X 	QStringList list;
//X 	data.setData( searchProviderPrefix + this->selectedText() );
//X 	list << "kurisearchfilter" << "kuriikwsfilter";
//X 	
//X 	if( !KURIFilter::self()->filterURI(data, list) )
//X 	{
//X 	KDesktopFile file("searchproviders/google.desktop", true, "services");
//X 	data.setData(file.readEntry("Query").replace("\\{@}", this->selectedText()));
//X 	}
//X 	
//X 	KParts::URLArgs args;
//X 	args.frameName = "_blank";
//X 	
//X 	emit slotLinkClicked( data.uri(), args );
}

void WebkitChatView::openSelection()
{
//X 	KParts::URLArgs args;
//X 	args.frameName = "_blank";
//X 	
//X 	emit slotLinkClicked( d->m_url, args );
}

void WebkitChatView::slotCopyLinkLocation()
{
//X 	KURL safeURL(d->m_url);
//X 	safeURL.setPass(QString::null);
//X #ifndef QT_NO_MIMECLIPBOARD
//X 	// Set it in both the mouse selection and in the clipboard
//X 	KURL::List lst;
//X 	lst.append( safeURL );
//X 	QApplication::clipboard()->setData( new KURLDrag( lst ), QClipboard::Clipboard );
//X 	QApplication::clipboard()->setData( new KURLDrag( lst ), QClipboard::Selection );
//X #else
//X 	QApplication::clipboard()->setText( safeURL.url() ); 
//X #endif
}

void WebkitChatView::slotCopyImage()
{
//X #ifndef QT_NO_MIMECLIPBOARD
//X 	KURL safeURL(d->m_imageURL);
//X 	safeURL.setPass(QString::null);
//X 	
//X 	KURL::List lst;
//X 	lst.append( safeURL );
//X 	KMultipleDrag *drag = new KMultipleDrag(view(), "Image");
//X 
//X 	drag->addDragObject( new QImageDrag(d->m_imageURL.path()) );
//X 	drag->addDragObject( new KURLDrag(lst, view(), "Image URL") );
//X 	
//X 	// Set it in both the mouse selection and in the clipboard
//X 	QApplication::clipboard()->setData( drag, QClipboard::Clipboard );
//X 	QApplication::clipboard()->setData( new KURLDrag(lst), QClipboard::Selection );
//X #else
//X 	// do nothing
//X #endif
}

void WebkitChatView::slotSaveImageAs()
{
//X 	QString name = QString::fromLatin1("index.html");;
//X 	if ( !d->m_suggestedFilename.isEmpty() )
//X 		name = d->m_suggestedFilename;
//X 	else if ( !d->m_imageURL.fileName().isEmpty() )
//X 		name = d->m_imageURL.fileName();
//X 
//X 	KURL destURL;
//X 	int query;
//X 	do {
//X 		query = KMessageBox::Yes;
//X 		destURL = KFileDialog::getSaveURL( QDir::homeDirPath() + "/" + name, QString::null, 0, i18n( "Save Image As" ) );
//X 		if( destURL.isLocalFile() ) {
//X 			QFileInfo info( destURL.path() );
//X 			if( info.exists() ) {
//X 				// TODO: use KIO::RenameDlg (shows more information)
//X 				query = KMessageBox::warningContinueCancel( 0, i18n( "A file named \"%1\" already exists. " "Are you sure you want to overwrite it?" ).arg( info.fileName() ), i18n( "Overwrite File?" ), i18n( "Overwrite" ) );
//X 			}
//X 		}
//X 	} while ( query == KMessageBox::Cancel );
//X 	
//X 	if ( destURL.isValid() )
//X 		KIO::file_copy(d->m_imageURL, destURL, -1, true /*overwrite*/);
}

void WebkitChatView::slotSaveAsCustomSmiley()
{
//X 	emit saveAsCustomSmiley(d->m_imageURL.path());
}

// it seems this method doesn't do the job
void WebkitChatView::startDrag( )
{
//X 	QDragObject *d = new QTextDrag(selectedText(), view());
//X 	d->dragCopy();
}

void WebkitChatView::slotSelectionChanged( )
{
//X 	copyAction->setEnabled( hasSelection() );
}

void WebkitChatView::showInfomation( const QString & info )
{
	QString picPath = "<img src = \"" + EvaMain::images->getIconFullPath("MSG_INFO") + "\">";
	QString msg = wrapFontAttributes(Qt::gray, 9, false, false, false, info);
	updateContents("&nbsp;" + picPath + "&nbsp;" + msg );
	showContents();
}

void WebkitChatView::showFileNotification( const QString & /*who*/, const QString & /*filename*/, 
			const int /*size*/, const unsigned int /*session*/, const bool /*isSend*/ )
{
//X 	QString strSession = QString::number(session);
//X 	QString picPath = "<img src = \"" + EvaMain::images->getIconFullPath("MSG_INFO") + "\">";
//X 	QString acceptLink = "<a href=\""+ protocolAccept + "_" + strSession + "\">" + i18n("Accept") + "</a>";
//X 	QString saveAsLink = "<a href=\""+ protocolSaveAs + "_" + strSession + "\">" + i18n("Save As") + "</a>";
//X         QString rejectLink = "<a href=\""+ protocolCancel + "_" + strSession + "\">" + i18n("Reject") + "</a>";
//X 	QString cancelLink = "<a href=\""+ protocolCancel + "_" + strSession + "\">" + i18n("Cancel") + "</a>";
//X 	QString fileSize = (size<0x400)?(QString::number(size) + "B"):
//X 			((size<0x100000)?(QString::number(size/0x400) + "KB") :
//X 			((size<0x40000000)?(QString::number(size/0x100000) + "MB"):
//X 			(QString::number(size/0x40000000) + "GB") ) );
//X 	QString fileInfo = filename + "(" + fileSize + ")";
//X         QString txt;
//X 	if(isSend){
//X 		txt = i18n("Waiting for %1 accepting file \"%2\". Please wait or %3.")
//X 				.arg(who).arg(fileInfo).arg(cancelLink);
//X 	}else{
//X 		txt = i18n("%1 wants to send you a file \"%2\", you can %3, %4 or %5.")
//X 				.arg(who).arg(fileInfo).arg(acceptLink).arg(saveAsLink).arg(rejectLink);
//X 	}
//X 	QString msg = wrapFontAttributes(Qt::gray, 9, false, false, false, txt);
//X 	updateContents("&nbsp;" + picPath + "&nbsp;" + msg );
//X 	showContents();
}

void WebkitChatView::updateContents( const QString & contents )
{
	//QString newMsg = contents;
	//Wonder why the following line was added, it does nothing other than extra mess spaces.
	//newMsg.replace("&nbsp;", "&nbsp; ");
	buffer += (contents + "<br>");
}

void WebkitChatView::showContents()
{
//X 	m_isScrollAtBottom =  ( view()->contentsHeight() == (view()->contentsY() +view()->visibleHeight()) );

//X 	begin();
	setHtml(buffer);
//X 	write("</body>");
//X 	end();
}

void WebkitChatView::askResumeMode( const QString /*filename*/, const unsigned int /*session*/ )
{
//X 	QString strSession = QString::number(session);
//X 	QString picPath = "<img src = \"" + EvaMain::images->getIconFullPath("MSG_INFO") + "\">";
//X 	QString resumeLink = "<a href=\""+ protocolResume + "_" + strSession + "\">" + 
//X 			i18n("resume") + "</a>";
//X 	QString restartLink = "<a href=\""+ protocolNewOne + "_" + strSession + "\">" + 
//X 			i18n("start") + "</a>";
//X 	QString txt = i18n("Cached information of \"%1\" has been found, you can %2 the last download or ignore the last cached download information and %3 a new download.").arg(filename).arg(resumeLink).arg(restartLink);
//X 
//X 	QString msg = wrapFontAttributes(Qt::gray, 9, false, false, false, txt);
//X 	updateContents("&nbsp;" + picPath + "&nbsp;" + msg );
//X 	showContents();
}



// vim: sw=4 sts=4 et tw=100 
