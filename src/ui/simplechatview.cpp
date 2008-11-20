#include <qpopupmenu.h>

#include "evamain.h"
#include "api/evaresource.h"
#include "ui/simplechatview.h"
 
class EvaImageResource;
#define MIN_FONT_SIZE   8
#define MAX_FONT_SIZE   20

SimpleChatView::SimpleChatView( QWidget * parent, const char * name )
	: QTextEdit(parent, name), m_isScrollAtBottom(true)
{
	//setOnlyLocalReferences(true);
	menu = new QPopupMenu(0, "popup");
//X 	copyAction = KStdAction::copy( this, SLOT(copy()), actionCollection());
//X 	copyAction->setText(i18n("&Copy Text"));
//X 	copyAction->setShortcut( KShortcut("Ctrl+C"));

//X 	setDNDEnabled(true);
//X 	setAutoloadImages(true);
	setHScrollBarMode(QScrollView::AlwaysOff);
	
	//setStandardFont("Helvetica");
	//buffer += "<body style=\"font-size:9pt;font-family:Helvetica\">";
	
//X 	QObject::connect(view(), SIGNAL(finishedLayout()), SLOT(slotScrollToBottom()));
	QObject::connect(this, SIGNAL(selectionChanged()), SLOT(slotSelectionChanged()));
	QObject::connect(this, SIGNAL(popupMenu(const QString &, const QPoint &)), 
			SLOT(slotPopupMenu(const QString &, const QPoint &)));
//X 	QObject::connect(browserExtension(), SIGNAL(openURLRequest(const KURL &, const KParts::URLArgs &)), 
//X 			SLOT(slotLinkClicked( const KURL &, const KParts::URLArgs &)));
}
SimpleChatView::~SimpleChatView()
{
	if(menu) delete menu;
//X 	if(d) delete d;
}

void SimpleChatView::append( QString & /*nick*/, QDateTime /*time*/, QColor /*nameColor*/, bool /*isNormal*/, 
				QColor /*msgColor*/, Q_UINT8 /*size*/, 
				bool /*underline*/, bool /*italic*/, bool /*bold*/, QString contents )
{
//X 	QString msg = wrapNickName(nick, time, nameColor, isNormal) + 
//X 			wrapFontAttributes(msgColor, size, underline, italic, bold, contents);
//X 	updateContents(msg);
    updateContents( contents );
}

void SimpleChatView::updateContents( const QString & contents )
{
	//QString newMsg = contents;
	//Wonder why the following line was added, it does nothing other than extra mess spaces.
	//newMsg.replace("&nbsp;", "&nbsp; ");
	buffer += (contents + "<br>");
}

void SimpleChatView::updatePicture( const QString /*filename*/ , const QString /*tmpFileName*/)
{
//X 	m_isScrollAtBottom =  ( contentsHeight() == (contentsY() +visibleHeight()) );
//X 	buffer.replace(tmpFileName, filename);
//X 	begin();
//X 	write(buffer);
//X 	write("</body>");
//X 	end();
}

void SimpleChatView::showInfomation( const QString & info )
{
//X 	QString picPath = "<img src = \"" + EvaMain::images->getIconFullPath("MSG_INFO") + "\">";
	QString msg = wrapFontAttributes(Qt::gray, 9, false, false, false, info);
	updateContents( msg );
//X 	updateContents("&nbsp;" + picPath + "&nbsp;" + msg );
	showContents();
}

void SimpleChatView::showFileNotification( const QString & /*who*/, const QString & /*filename*/, 
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


QString SimpleChatView::wrapFontAttributes(QColor color, Q_UINT8 size,
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


void SimpleChatView::askResumeMode( const QString /*filename*/, const unsigned int /*session*/ )
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

void SimpleChatView::showContents()
{
	m_isScrollAtBottom =  ( contentsHeight() == (contentsY() + visibleHeight()) );

//X 	begin();
	setText(buffer);
//X 	write("</body>");
//X 	end();
}


// vim: sw=4 sts=4 et tw=100 
