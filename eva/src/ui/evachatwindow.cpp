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
  
#include "evachatwindow.h"
#include "customfaceselector.h"
#include "evafontselecter.h"

#include "evaresource.h"
#include "evamain.h"
#include "evauser.h"
#include "evausersetting.h"
#include "evafriendlist.h"
#include "evatextedit.h"
#include "evahtmlparser.h"
#include "evachatview.h"
#include "evaipseeker.h"
#include "evafilepanel.h"
#include "evafilestatusuibase.h"
#include "regiongrabber.h"
#include "evamainwindow.h"
#include "evahistoryviewer.h"
#include "evascriptmanager.h"

#include <qtextcodec.h>
#include <qrect.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qiconset.h>
#include <qpixmap.h>
#include <qhostaddress.h>
#include <qevent.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlineedit.h>
#include <qpopupmenu.h>
#include <qtooltip.h>
#include <qregexp.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qimage.h>
#include <quuid.h>
#include <qtimer.h>
#include <klocale.h>
#include <kurl.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <kapplication.h>
#include <stdio.h>

#ifndef QQ_MSG_IM_MAX
#define QQ_MSG_IM_MAX 15000
#endif

int EvaChatWindow::myQQ=0;
QString EvaChatWindow::myName ="";
EvaImageResource *EvaChatWindow::images = NULL;
bool EvaChatWindow::isSentByEnter = false;

std::list<QString> EvaChatWindow::quickList;
	
EvaChatWindow::EvaChatWindow(QQFriend * frd, QWidget* parent, const char* name, WFlags fl)
	: EvaChatUIBase(parent, name, fl), smileyPopup(NULL), quickMenu(NULL), fontSelecter(NULL),
	m_NumImages(0), grabber(NULL), viewer(NULL)
{
	//buddy = new QQFriend(*frd);
	buddy = frd;
	codec = QTextCodec::codecForName("GB18030");
	initObjects();
	initInformation();
	initConnection();
	graphicChanged();
	kteInput->setFocus();
}

EvaChatWindow::~EvaChatWindow()
{
	//delete buddy;
}

void EvaChatWindow::setupImages(EvaImageResource *res)
{
	if(!res) return;
	images = res;
}

void EvaChatWindow::setQuickReplyMessages(const std::list<QString> &list) 
{ 
	quickList = list; 
}

const unsigned int EvaChatWindow::getBuddyQQ()
{
	return buddy->getQQ();
}

void EvaChatWindow::graphicChanged()
{
	QStringList imageDirList;
	imageDirList.append(images->getSmileyPath());
	imageDirList.append(EvaMain::user->getSetting()->getPictureCacheDir());
	kteInput->mimeSourceFactory()->setFilePath(imageDirList);

	
	tbSmiley->setIconSet(*(images->getIcon("SMILEY")));
	tbFont->setIconSet(*(images->getIcon("FONT")));
	tbFile->setIconSet(*(images->getIcon("FILE_TRANSFER")));
	tbB->setIconSet(*(images->getIcon("FONT_B")));
	tbU->setIconSet(*(images->getIcon("FONT_U")));
	tbI->setIconSet(*(images->getIcon("FONT_I")));
	tbAddImage->setIconSet(*(images->getIcon("SEND_IMAGE")));
	tbScreenShot->setIconSet(*(images->getIcon("SCREEN_SHOT")));
	tbQuickReply->setIconSet(*(images->getIcon("QUICK_REPLY")));
	tbEnableSound->setIconSet(*(images->getIcon("SYSTEM_MSG")));
	tbHideShows->setIconSet(*(images->getIcon("HIDE_PORTRAIT")));
	slotBuddyQQShowReady(getBuddyQQ());
	slotMyQQShowReady();
}

void EvaChatWindow::initObjects()
{
	if(fontSelecter) delete fontSelecter;
	fontSelecter = new EvaFontSelecter(this);
	if(smileyPopup) delete smileyPopup;
	smileyPopup = new CustomFaceSelector();
	
	quickMenu = new QPopupMenu(tbQuickReply);
	if(quickList.size()){	
		std::list<QString>::iterator iter;
		int index = 0;
		for(iter=quickList.begin(); iter!=quickList.end(); ++iter)
			quickMenu->insertItem(*iter, index++);
		tbQuickReply->setPopup(quickMenu);
		tbQuickReply->setPopupDelay(10);
		QObject::connect(quickMenu, SIGNAL(activated(int)), this,  SLOT(slotQuickReplyActivated(int)));
	}
	
	sendKey = new QPopupMenu();
	sendKey->setCheckable(true);
	sendKey->insertItem(i18n("Press \"Enter\" to Send"),this,SLOT(setEnterSend()),SHIFT+ALT+Key_Enter,1);  
	sendKey->insertItem(i18n("Press \"Ctrl+Enter\" to Send"),this, SLOT(setCtrlEnterSend()),SHIFT+CTRL+ALT+Key_Enter,2);
	if(isSentByEnter)
		sendKey->setItemChecked(1,true);
	else
		sendKey->setItemChecked(2,true);
	pbSendKey->setPopup(sendKey); 
	kteInput->setEnterSendEnabled(isSentByEnter);
}

void EvaChatWindow::initInformation()
{
	if(!buddy) return;
	QString nick = codec->toUnicode(buddy->getNick().c_str());
	QString title = QString(i18n("Chatting with %1")).arg(nick);
	setCaption(title);
	QString status =i18n("(offline)");
	QIconSet face;
	int faceId = buddy->getFace();
	QPixmap *faceOnPic=NULL, *faceOffPic=NULL;
	if(buddy->hasUserHead()){
		faceOnPic = images->getUserHeadPixmap(buddy->getQQ());
		faceOffPic = images->getUserHeadPixmap(buddy->getQQ(), true); // true means grayscale true
	}
	if(!faceOnPic || !faceOffPic){
		faceOnPic = images->getFace(images->getFaceFileIndex(faceId));
		faceOffPic = images->getFace(images->getFaceFileIndex(faceId), false); // false means on-line off
	}
	setIcon(*faceOnPic);
	
	face.setPixmap(*faceOffPic,QIconSet::Large);
	tbBuddy->setIconSet(face);
	switch(buddy->getStatus()){
	case 10:
		status = i18n("(online)");
		face.setPixmap(*faceOnPic,QIconSet::Large);
		tbBuddy->setIconSet(face);
		break;
	case 20:
		//status = i18n("(offline)");
		//face.setPixmap(*(images->getFace(images->getFaceFileIndex(faceId), false)),QIconSet::Large);
		//tbBuddy->setIconSet(face);
		break;
	case 30:{
		status = i18n("(leave)");
		QPixmap pixleave(*faceOnPic);
		QPixmap pixna(*(images->getIcon("NA")));
		copyBlt(&pixleave, pixleave.width()-pixna.width(),pixleave.height()-pixna.height(), 
			&pixna,0,0, pixna.width(), pixna.height());
		face.setPixmap(pixleave, QIconSet::Large);
		tbBuddy->setIconSet(face);
		}
		break;
	case 40:
		status = i18n("(invisible)");
		face.setPixmap(*faceOffPic, QIconSet::Large);
		tbBuddy->setIconSet(face);
		break;
	}
	tbBuddy->setTextLabel(nick);
	QToolTip::add(tbBuddy, nick + "("+QString::number(buddy->getQQ()) +") " + status);
	
	QString ip = QHostAddress(buddy->getIP()).toString();
	EvaIPSeeker ipAddr(EvaGlobal::getDirPath().latin1());
	QString addr = codec->toUnicode(ipAddr.getIPLocation(ip.latin1()).c_str());
	leIP->setText(addr);
	
	QString sig = codec->toUnicode(buddy->getSignature().c_str());
	lblSignature->setText("  " + sig);
	
	//QString port = QString::number(((int)(buddy->getPort()))==-1?0:(unsigned int)(buddy->getPort()));
	QToolTip::add(leIP, ip + " : " + QString::number(buddy->getPort()));
	
}

void EvaChatWindow::initConnection()
{
	if(smileyPopup) {
		connect(smileyPopup, SIGNAL(selectSysFace(int)), this, SLOT(slotSmileySelected(int)));
		// we might need the following later on
		//connect(smileyPopup, SIGNAL(selectCustomFace(const QString &)), this, SLOT(slotAddImageToInputEdit(const QString &)));
	}

	QObject::connect(tbBuddy, SIGNAL(clicked()), this, SLOT(slotTbBuddyClick()));
	QObject::connect(tbSmiley, SIGNAL(clicked()), this, SLOT(slotSmileyClick()));
	QObject::connect(tbFont, SIGNAL(clicked()), this, SLOT(slotFontClick()));
	QObject::connect(tbFile, SIGNAL(clicked()), this, SLOT(slotFileClick()));
	QObject::connect(tbAddImage, SIGNAL(clicked()), this, SLOT(slotAddImageClick()));
	QObject::connect(tbScreenShot, SIGNAL(clicked()), this, SLOT(slotScreenShotClick()));
	QObject::connect(tbQuickReply, SIGNAL(clicked()), this, SLOT(slotQuickReplyClick()));
	QObject::connect(tbHideShows, SIGNAL(clicked()), this, SLOT(slotHideShowsClick()));
	QObject::connect(pbHistory, SIGNAL(clicked()), this, SLOT(slotHistoryClick()));
	QObject::connect(pbSendKey, SIGNAL(clicked()), this, SLOT(slotSendKeyClick()));
	QObject::connect(pbSend, SIGNAL(clicked()), this, SLOT(slotSend()));
	QObject::connect(tbShowBuddy, SIGNAL(clicked()), this, SLOT(slotTbShowBuddyClick()));
	QObject::connect(tbShowMe, SIGNAL(clicked()), this, SLOT(slotTbShowMeClick()));
	QObject::connect(kteInput, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotInputKeyPress(QKeyEvent *)));

	if(fontSelecter)
		QObject::connect(fontSelecter, SIGNAL( fontChanged(QColor, int)), this, SLOT(slotFontChanged( QColor, int )));

	QObject::connect(pbClose, SIGNAL(clicked()), this, SLOT(slotPbCloseClick()));

	QObject::connect(chatDisplay, SIGNAL(fileTransferAcceptRequest(const unsigned int)), 
						SLOT(slotFileTransferAcceptClicked(const unsigned int)));
	QObject::connect(chatDisplay, SIGNAL(fileTransferSaveAsRequest(const unsigned int)), 
						SLOT(slotFileTransferSaveAsClicked(const unsigned int)));
	QObject::connect(chatDisplay, SIGNAL(fileTransferCancelRequest(const unsigned int)), 
						SLOT(slotFileTransferCancelClicked(const unsigned int)));
	QObject::connect(chatDisplay, SIGNAL(fileTransferResume(const unsigned int, const bool)), 
						SLOT(slotFileTransferResume(const unsigned int, const bool)));

	QObject::connect(m_FilePanel, SIGNAL(closeSession(const unsigned int)),
					SLOT(slotFilePanelCloseClicked( const unsigned int )));
}

void EvaChatWindow::displaySendingMessage()
{
	EvaHtmlParser parser;
	parser.setAbsImagePath(images->getSmileyPath());
	QString text = kteInput->text();
	QString tmp;
	parser.convertToPlainTxt(text, tmp);
	parser.convertToHtml(text,true, true);
	
	QColor chatColor(buddy->getChatFontColor());
	int chatSize =  buddy->getChatFontSize();
	chatDisplay->append( myName, sendtime, Qt::darkCyan, true, chatColor, (char)chatSize, 
				tbU->isOn(), tbI->isOn(), tbB->isOn(), text);
	showMessages();
	//kteInput->setText("");
}

void EvaChatWindow::slotReceivedMessage(unsigned int sender, bool isNormal, QString message, QDateTime time, const char size, 
					const bool u, const bool i, const bool b, 
					const char blue, const char green, const char red)
{
	if(sender != buddy->getQQ()) return;
	QString nick = codec->toUnicode(buddy->getNick().c_str());
	EvaHtmlParser parser;
	parser.setAbsImagePath(images->getSmileyPath());
	parser.convertToHtml(message, true, true);
	chatDisplay->append(nick, time, Qt::blue, isNormal,
				QColor((Q_UINT8)red, (Q_UINT8)green,(Q_UINT8)blue),
				size, u, i, b, message);
	if ( tbEnableSound->isOn() )
		EvaMain::global->getSoundResource()->playNewMessage();
}

void EvaChatWindow::showMessages()
{
	chatDisplay->showContents();
}

void EvaChatWindow::slotAddMessage(unsigned int , QString sNick, unsigned int , QString , bool isNormal, 
				QString message, QDateTime time, const char , 
				const bool , const bool , const bool , 
				const char , const char , const char )
{	
	if(!kteInput->isEnabled()) return;
	EvaHtmlParser parser;
	parser.convertToHtml(message, false, false, true);
	QString msg = sNick + (isNormal?(""):i18n("(Auto-Reply)")) + "  " + time.toString("yyyy-MM-dd hh:mm:ss") + "<br />" + message;
	kteInput->append(msg);
}

void EvaChatWindow::slotSendResult(bool ok)
{
	if(!ok) {
		KMessageBox::information(this, i18n("message sent failed"), i18n("Message"));
	} else
		kteInput->setText("");
	pbSend->setEnabled(true);
	kteInput->setEnabled(true);
	//kteInput->setParagraphBackgroundColor(0, Qt::white);
	kteInput->setFocus();
}

void EvaChatWindow::slotBuddyQQShowReady(const unsigned int id)
{
	if(id!= getBuddyQQ()) return;
	QPixmap *pix = images->getQQShow(id);
	if(pix){ 
		QImage img = pix->convertToImage();
		tbShowBuddy->setIconSet(QPixmap(img.smoothScale(tbShowBuddy->width()-8, tbShowBuddy->height() -2)));
	}
}

void EvaChatWindow::slotMyQQShowReady()
{
	QPixmap *pix = images->getQQShow(myQQ);
	if(pix){
		QImage img = pix->convertToImage();
		tbShowMe->setIconSet(QPixmap(img.smoothScale(tbShowMe->width()-8, tbShowMe->height() -2)));
	}
}

void EvaChatWindow::slotSmileySelected(int fid)
{
	if(!kteInput->isEnabled()) return;
	QString smiley = "<img src=\"" + QString::number(fid) + ".gif\"> ";
	
	int para;
	int index;
	QFont saveFont = kteInput->font();
	QColor saveColor = kteInput->color();
	// determine the current position of the cursor
	kteInput->insert("\255", false, true, true);	
	kteInput->getCursorPosition(&para,&index);
	QString txt = kteInput->text();
	txt.replace(QRegExp("\255"),smiley);
	kteInput->setText(txt);
	kteInput->setCursorPosition(para, index);
	kteInput->setCurrentFont(saveFont);
	kteInput->setColor(saveColor);
}

void EvaChatWindow::slotTbBuddyClick()
{
	emit requestDetails(buddy->getQQ());
}

void EvaChatWindow::slotSmileyClick()
{
	if(smileyPopup){
		QPoint p = kteInput->mapToGlobal(QPoint(0,0));
		smileyPopup->setGeometry(p.x() + tbSmiley->x() , p.y(), smileyPopup->width(), smileyPopup->height());
		smileyPopup->show();
	}	
}

void EvaChatWindow::slotFontClick()
{
	if(!fontSelecter) return;
	if(fontSelecter->isVisible()) 
		fontSelecter->hide();
	else{
		QPoint p = kteInput->mapToGlobal(QPoint(0,0));
		fontSelecter->setColor( QColor(buddy->getChatFontColor()));
		fontSelecter->setSize( buddy->getChatFontSize());
		fontSelecter->setGeometry(p.x() + tbFont->x() , p.y(), fontSelecter->width(), fontSelecter->height());
		fontSelecter->show();
 	}
}

void EvaChatWindow::slotQuickReplyClick()
{
	//tbQuickReply->openPopup();
	//quickMenu->exec();
}

void EvaChatWindow::slotQuickReplyActivated(int id)
{
	if(!kteInput->isEnabled()) return;
	std::list<QString>::iterator iter;
	int index = 0;
	for(iter=quickList.begin(); iter!=quickList.end(); ++iter){
		if(index == id) break;	
		index++;
	}
	QString message = *iter;
	EvaHtmlParser parser;
	parser.convertToHtml(message, false);
	kteInput->append(message);	
	slotSend();
}

void EvaChatWindow::slotHideShowsClick()
{
	if(m_ShowFrame->isVisible()){
		//m_FilePanel->hide();
		m_FilePanelFrame->hide();
		m_ShowFrame->hide();
		//lblBuddyShow->hide();
		//tbShowBuddy->hide();
		//lblMeShow->hide();
		//tbShowMe->hide();
		EvaChatUIBaseLayout->removeItem(layout19);
		tbHideShows->setIconSet(*(images->getIcon("SHOW_PORTRAIT")));

		QValueList<int> list;
		list.append(150);
		list.append(100);
		splitter->setSizes(list);
		resize( QSize(320, 367).expandedTo(minimumSizeHint()) );
	}else{
		//m_FilePanel->show();
		m_FilePanelFrame->show();
		m_ShowFrame->show();
		//lblBuddyShow->show();
		//tbShowBuddy->show();
		//lblMeShow->show();
		//tbShowMe->show();
		EvaChatUIBaseLayout->addLayout(layout19);
		tbHideShows->setIconSet(*(images->getIcon("HIDE_PORTRAIT")));

		QValueList<int> list;
		list.append(150);
		list.append(100);
		splitter->setSizes(list);
		resize( QSize(450, 422).expandedTo(minimumSizeHint()) );
	}
}

void EvaChatWindow::slotHistoryClick()
{
//	emit requestHistory(getBuddyQQ());
	if ( !viewer )
	{
		QString nick = codec->toUnicode(buddy->getNick().c_str());

		viewer = new EvaHistoryViewer(getBuddyQQ(), nick, EvaMain::user->getSetting());

		unsigned short faceId = buddy->getFace();
		QPixmap *face = EvaMain::images->getFaceByID(faceId);
		viewer->setIcon(*face);

		connect(viewer, SIGNAL(historyDoubleClicked(unsigned int, QString, unsigned int, QString, bool,
						QString, QDateTime, const char,
						const bool, const bool, const bool,
						const char, const char, const char)),
				this,
				SLOT(slotAddMessage(unsigned int, QString, unsigned int, QString, bool,
						QString, QDateTime, const char,
						const bool, const bool, const bool,
						const char, const char, const char)));
		connect(viewer, SIGNAL(windowClosed()), this, SLOT(slotHistoryWindowClosed()));
	}
	if ( pbHistory->isOn() )
	{
		viewer->move(this->x(), this->y() + this->height() + 25);

		viewer->show();
	}
	else
	{
		viewer->hide();
	}
}

void EvaChatWindow::slotHistoryWindowClosed()
{
//	viewer->hide();
	pbHistory->setOn(FALSE);
}

void EvaChatWindow::slotSendKeyClick()
{
}

void EvaChatWindow::slotSend()
{
	if(!kteInput->isEnabled()) return;
	QString msg = kteInput->text();
	EvaHtmlParser parser;
	parser.setAbsImagePath(images->getSmileyPath(), EvaMain::user->getSetting()->getPictureCacheDir());

	sendtime = QDateTime::currentDateTime(Qt::LocalTime);

	QString toSend = msg;
	QString sendFileNameBase; 
	int numFiles = parser.convertToPlainTxt(toSend, sendFileNameBase);
	
	if(toSend == ""){
		KMessageBox::information(this, i18n("Cannot send empty message."), i18n("Message"));
		return;
	}
	if(strlen(toSend.ascii()) > QQ_MSG_IM_MAX){
		KMessageBox::information(this, i18n("Message is too long, cannot send."), i18n("Message"));
		return;
	}
printf("*********************************** %s\n",toSend.ascii());
	pbSend->setEnabled(false);
	kteInput->setEnabled(false);

	QColor chatColor(buddy->getChatFontColor());
	int chatSize =  buddy->getChatFontSize();
	if(numFiles){
		QString strToShow = msg;
		parser.parseToAbsPath(strToShow, EvaMain::user->getSetting()->getPictureCacheDir());
		chatDisplay->append( myName, sendtime, Qt::darkCyan, true, chatColor, (char)chatSize, 
					tbU->isOn(), tbI->isOn(), tbB->isOn(), strToShow);
		showMessages();
		m_NumImages = numFiles;
		//kteInput->setText("");
	}else	
		displaySendingMessage();
	
	EvaMainWindow *mainWin = EvaMain::g_mainWin;
	if(mainWin)
		mainWin->addBuddyToRecentList( getBuddyQQ(), true);
	emit sendMessage(getBuddyQQ(), true, toSend, (char)chatSize, tbU->isOn(), tbI->isOn(), tbB->isOn(),
			(char)(chatColor.blue()) , (char)(chatColor.green()), (char)(chatColor.red()));

	if(numFiles){		
		QValueList<QString> nameList;
		QValueList<unsigned int> sizeList;
		unsigned int session = buddy->getNextSequence();
		for(int i=0; i<numFiles; i++){
			QString name = sendFileNameBase + QString::number(i) + ".jpg";
			QString fullname = EvaMain::user->getSetting()->getPictureCacheDir() + "/" + name;
					
			QFileInfo info(fullname);
			nameList.append(fullname);
			sizeList.append(info.size());
		}
		
		emit fileTransferSend(buddy->getQQ(), session, nameList, sizeList, QQ_TRANSFER_IMAGE);
	}
	//kteInput->setParagraphBackgroundColor(0, Qt::gray);
}

void EvaChatWindow::slotTbShowBuddyClick()
{
	emit requestBuddyQQShow(getBuddyQQ());
}

void EvaChatWindow::slotTbShowMeClick()
{
	emit requestMyQQShow();
}

void EvaChatWindow::slotInputKeyPress(QKeyEvent *e)
{
	if(isSentByEnter && ((e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return) ) && (e->state() == Qt::NoButton) ){
		e->accept();
		slotSend();
	}else{
		if( !(isSentByEnter) &&
			( (e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return) ) && 
			( (e->state() & Qt::ControlButton)==Qt::ControlButton)    ){
		e->accept();
		slotSend();
		}
	}
}

void EvaChatWindow::slotFontChanged(QColor color, int size)
{
// 	int fontSize = size;
// 	int fontColor = color;
	buddy->setChatFontSize(size);
	buddy->setChatFontColor(color.rgb());
	printf("setting color: %s size: %d\n", color.name().ascii(), size);
	//QColor c=color;
	//QString s=c.name();
	EvaMain::user->saveGroupedBuddyList();
}

void EvaChatWindow::setEnterSend()
{
	isSentByEnter = true;
	sendKey->setItemChecked(1,true);
	sendKey->setItemChecked(2,false);
	kteInput->setEnterSendEnabled(isSentByEnter);
}

void EvaChatWindow::setCtrlEnterSend()
{
	isSentByEnter = false;
	sendKey->setItemChecked(1,false);
	sendKey->setItemChecked(2,true);
	kteInput->setEnterSendEnabled(isSentByEnter);
}

void EvaChatWindow::slotPbCloseClick()
{
	close();
}

void EvaChatWindow::openSendFileDialog()
{
	slotFileClick();
}

void EvaChatWindow::slotFileClick( )
{
	if(!kteInput->isEnabled()) return;
	QString destDir = QDir::homeDirPath();
	QString fullname = KFileDialog::getOpenFileName(destDir,
			"* *.png |" + i18n(" all files (*)"), this, 
			i18n("select a file"));	
	QString name = fullname.right(fullname.length() - fullname.findRev("/") -1);
	QFileInfo info(fullname);
	if(!fullname.isEmpty()){
		unsigned int session = buddy->getNextSequence();
		printf("EvaChatWindow::slotFileClick -- next seq: %d\n", session);
		chatDisplay->showFileNotification(codec->toUnicode(buddy->getNick().c_str()),
				name, info.size(), session, true);
		if(!addToFileList(session, name)) return ; // we've already got this request
		m_FileNoList[session] = session;
		QValueList<QString> nameList;
		nameList.append(fullname);
		QValueList<unsigned int> sizeList;
		sizeList.append(info.size());

		EvaMainWindow *mainWin = EvaMain::g_mainWin;
		if(mainWin)
			mainWin->addBuddyToRecentList( buddy->getQQ(), true);
		emit fileTransferSend(buddy->getQQ(), session, nameList, sizeList, QQ_TRANSFER_FILE);
	}
}

void EvaChatWindow::slotReceivedFileRequest( const unsigned int session, const QString & file,
					const int size, const unsigned char transferType )
{	
	if(transferType == QQ_TRANSFER_IMAGE){
		switch(EvaMain::user->getStatus()){
		case EvaUser::Eva_Online:
		case EvaUser::Eva_Leave:
			m_NumImages++;
			emit fileTransferAccept(buddy->getQQ(), session, "", QQ_TRANSFER_IMAGE);
			break;
		case EvaUser::Eva_Offline: // impossible
		case EvaUser::Eva_Invisible:
			printf("EvaChatWindow::slotReceivedFileRequest --id:%d, seq: %d\n", buddy->getQQ(), buddy->getSequence());
			if(buddy->getSequence()){
				m_NumImages++;
				printf("now I accepted\n");
				emit fileTransferAccept(buddy->getQQ(), session, "", QQ_TRANSFER_IMAGE);
			}
			break;
		default:
			emit fileTransferCancel(buddy->getQQ(), session);
		}
		return;
	}
	if(!addToFileList(session, file)) return ; // we've already got this request
	m_FileNoList[session] = session;
	if(transferType == QQ_TRANSFER_FILE){
		EvaMainWindow *mainWin = EvaMain::g_mainWin;
		if(mainWin)
			mainWin->addBuddyToRecentList( getBuddyQQ(), true);
		chatDisplay->showFileNotification(codec->toUnicode(buddy->getNick().c_str()),
					file, size, session, false);
	}
}

void EvaChatWindow::slotFileTransferAcceptClicked( const unsigned int showSession)
{	
	QString msg = i18n("You have accepted transferring \"%1\", network connecting start, please wait...").arg(getFileName(getSession(showSession)));
	chatDisplay->showInfomation(msg);
	emit fileTransferAccept(buddy->getQQ(), getSession(showSession), "", QQ_TRANSFER_FILE);
}

void EvaChatWindow::slotFileTransferSaveAsClicked( const unsigned int showSession )
{
	unsigned int session = getSession(showSession);
	QString name = getFileName(session);
	QString destDir = QDir::homeDirPath() + "/" + name;
	QString fullname = KFileDialog::getSaveFileName(destDir,
			"* |" + i18n(" all files (*)"), this, 
			i18n("Save As"));
	QString dir = fullname.left(fullname.findRev("/"));
	//QFileInfo info(fullname);
	if(!dir.isEmpty()){
		QString msg = i18n("You have accepted transferring \"%1\", network connecting start, please wait...").arg(getFileName(getSession(showSession)));
		chatDisplay->showInfomation(msg);
		emit fileTransferAccept(buddy->getQQ(), session, dir, QQ_TRANSFER_FILE);
	}
}

void EvaChatWindow::slotFileTransferCancelClicked( const unsigned int showSession)
{
	printf("EvaChatWindow::slotFileTransferCancelClicked -- \n");
	QString msg = i18n("You have cancelled transferring \"%1\".").arg(getFileName(getSession(showSession)));
	chatDisplay->showInfomation(msg);
	emit fileTransferCancel(buddy->getQQ(), getSession(showSession));
	removeFromFileList(getSession(showSession));
	m_FilePanel->closeTab( getSession( showSession));
}

void EvaChatWindow::slotReceivedFileAccepted(const unsigned int session, const bool isAccepted, 
					const unsigned char transferType )
{
	QString filename = getFileName(session);
	if(filename.isEmpty()) return; // we haven't got the request

	if(!isAccepted) removeFromFileList(session);

	if(isAccepted && transferType == QQ_TRANSFER_IMAGE)
		return;
	
	QString name = codec->toUnicode(buddy->getNick().c_str());
	QString action = isAccepted ? i18n("accepted") : i18n("stopped");
	QString msg = i18n("%1 has %2 transferring \"%3\".").arg(name).arg(action).arg(filename);
	chatDisplay->showInfomation(msg);
}

QString EvaChatWindow::getFileName( const unsigned int session )
{
	QMap<unsigned int, QString>::Iterator iter = m_FileList.find(session);
	if(iter == m_FileList.end()) return "";
	return iter.data();
}

const bool EvaChatWindow::addToFileList( const unsigned int session, const QString filename )
{
	QString file = getFileName(session);
	if(!file.isEmpty()) return false; // if exists  return false;
	m_FileList[session] = filename;
	return true;
}

const unsigned int EvaChatWindow::getSession(const unsigned int showSession)
{
	QMap<unsigned int, unsigned int>::Iterator iter;
	for(iter = m_FileNoList.begin(); iter!=m_FileNoList.end(); ++iter){
		if(iter.data() == showSession ) return iter.key();
	}
	return 0;
}

void EvaChatWindow::removeFromFileList( const unsigned int session )
{
	m_FileList.remove(session);
	m_FileNoList.remove(session);
}

void EvaChatWindow::slotSessionChangedTo( const unsigned int oldSession, const unsigned int newSession )
{
	QString file = getFileName(oldSession);
	if(file.isEmpty()) return; // if not existed, return
	removeFromFileList(oldSession);
	addToFileList(newSession, file);
	m_FileNoList[newSession] = oldSession;
}

void EvaChatWindow::slotFileNotifyNormalInfo( const unsigned int session, EvaFileStatus status, 
					const QString dir, const QString filename, const unsigned int /*size */,
					const unsigned char transferType)
{
	switch(status){
	case ESNone:
		break;
	case ESError:{
		QString msg = i18n("network lost connection or your friend has stopped transferring \"%1\".").arg(filename);
		switch(transferType){
		case QQ_TRANSFER_FILE:{
			if(getFileName(session).isEmpty()) break;
			chatDisplay->showInfomation(msg);
			removeFromFileList(session);
			emit fileTransferCancel(buddy->getQQ(), session);
			}
			break;
		case QQ_TRANSFER_IMAGE:{
			m_NumImages--;
			QString num = filename.mid(filename.length() - 5, 1);
			bool ok;
			unsigned int n = num.toUInt(&ok);
			if(!ok) break;
			QString file = getFileName(session * 100 + n);
			if(file.isEmpty()) break;
			chatDisplay->showInfomation(msg);
			emit fileTransferCancel(buddy->getQQ(), session);
			}
			break;
		default:
			break; // impossible, compiler would like this
		}
		}
		m_FilePanel->closeTab( session);
		break;
	case ESResume:
		chatDisplay->askResumeMode(filename, session);
		break;
	case ESSendFinished:{
		QString msg = i18n("sending \"%1\" finished.").arg(filename);
		switch(transferType){
		case QQ_TRANSFER_FILE:
			chatDisplay->showInfomation(msg);
			removeFromFileList(session);
			break;
		case QQ_TRANSFER_IMAGE:{
			m_NumImages--;
			}
			break;
		default:
			removeFromFileList(session);
		}
		}
		break;
	case ESReceiveFinished:{
		QString fullname = dir + "/" + filename;
		QString msg = "";
		switch(transferType){
		case QQ_TRANSFER_FILE:{
			removeFromFileList(session);
			QString fileLink = "<a href = \"file://" + fullname + "\">" + fullname + "</a>";
			msg = i18n("receiving file \"%1\" has been finished and saved in %2").arg(filename).arg(fileLink);
			}
			break;
		case QQ_TRANSFER_IMAGE:{
			m_NumImages--;
			chatDisplay->updatePicture(fullname, "/t_" + filename);
			return;
			}
			break; // make compiler happy
		default:
			return;
		}
		chatDisplay->showInfomation(msg);
		removeFromFileList(session);
		}
		break;
	default:
		break;
	}
}

void EvaChatWindow::slotFileTransferResume( const unsigned int session, const bool isResume)
{
	emit fileTransferResume(buddy->getQQ(), session, isResume);
}

void EvaChatWindow::slotFileStatusNotification( const unsigned int session, const KURL & url, 
						const unsigned int size, const unsigned int bytes, 
						const unsigned int time )
{
	if(!m_ShowFrame->isVisible()) slotHideShowsClick();
	m_FilePanel->updateStatus(session, url, size, bytes, time);
}

void EvaChatWindow::slotFilePanelCloseClicked( const unsigned int session)
{
	printf("EvaChatWindow::slotFilePanelCloseClicked\n");
	removeFromFileList(session);
	emit fileTransferCancel(buddy->getQQ(), session);
}

void EvaChatWindow::closeEvent( QCloseEvent * e )
{
	if(m_FileList.size() && m_NumImages && KMessageBox::questionYesNo(this,
			i18n("File transfer is still in process (%1 file(s) left). Closing this window "
				"will also stop those transfer processes. "
				"Do you want to close this window?").arg(m_FileList.size()),
			i18n("Close Window?")) == KMessageBox::No){
		e->ignore();
	} else{
		QMap<unsigned int, QString>::Iterator iter;
		for(iter = m_FileList.begin(); iter!=m_FileList.end(); ++iter){
			emit fileTransferCancel(buddy->getQQ(), iter.key());
		}
		if (viewer)
			delete viewer;
		e->accept();
	}
}

void EvaChatWindow::slotAddImageClick( )
{
	if(!kteInput->isEnabled()) return;
	QString destDir = QDir::homeDirPath();
	QString srcFullName = KFileDialog::getOpenFileName(destDir,
			"*.png *.bmp *.jpg *.jpeg *.gif |" + i18n(" all images (*.png *.bmp *.jpg *.jpeg *.gif)"), this, 
			i18n("select an image file"));
	if(!srcFullName.isEmpty()){
// 		if(!m_NumImages){
// 			m_ImageFileName = QUuid::createUuid().toString().upper();
// 		}
		//QString destFile = m_ImageFileName + QString::number(m_NumImages) + ".jpg";
		QString destFile = QUuid::createUuid().toString().upper() + ".jpg";
		QString destFullName = EvaMain::user->getSetting()->getPictureCacheDir() + "/" + destFile ;
		QPixmap pix(srcFullName);
		pix.save(destFullName, "JPEG", 100);
		slotAddImageToInputEdit(destFile);
		//m_NumImages++;
	}
}

void EvaChatWindow::slotAddImageToInputEdit( const QString & destFile)
{
	if(!kteInput->isEnabled()) return;
	if(destFile.isEmpty()) return;
	
	QString smiley = "<img src=\"" + destFile + "\">";
	int para;
	int index;
	QFont saveFont = kteInput->font();
	QColor saveColor = kteInput->color();
	// determine the current position of the cursor
	kteInput->insert("\255", false, true, true);
	kteInput->getCursorPosition(&para,&index);
	QString txt = kteInput->text();
	txt.replace(QRegExp("\255"),smiley);
	kteInput->setText(txt);
	kteInput->setCursorPosition(para, index);
	kteInput->setCurrentFont(saveFont);
	kteInput->setColor(saveColor);
}

void EvaChatWindow::slotScreenShotClick( )
{
	if(!kteInput->isEnabled()) return;
	if(grabber) delete grabber;
	grabber = new RegionGrabber();
	connect( grabber, SIGNAL( regionGrabbed( const QPixmap & ) ),
		SLOT( slotRegionGrabbed( const QPixmap & ) ) );
}

void EvaChatWindow::slotRegionGrabbed( const QPixmap & pix )
{
	if ( !pix.isNull() ){
// 		if(!m_NumImages){
// 			m_ImageFileName = QUuid::createUuid().toString().upper();
// 		}
		
		//QString destFile = m_ImageFileName + QString::number(m_NumImages) + ".jpg";
		QString destFile = QUuid::createUuid().toString().upper() + ".jpg";
		QString destFullName = EvaMain::user->getSetting()->getPictureCacheDir() + "/" + destFile ;
		pix.save(destFullName, "JPEG", 100);
		slotAddImageToInputEdit(destFile);
		//m_NumImages++;
	}
	
	if(grabber) delete grabber;
	grabber = NULL;
	QApplication::restoreOverrideCursor();
}

void EvaChatWindow::appendText( const QString & txt )
{
	if(!kteInput->isEnabled()) return;
	kteInput->append(  txt );
}

void EvaChatWindow::addToolButton( QString & scriptName, QString buttonName, QString & pixmap, QString & tip )
{
	printf("EvaChatWindow::addToolButton\n");
	if(m_btnMap.find(buttonName) != m_btnMap.end()) return;
	
	printf("EvaChatWindow::addToolButton ------- \n");
	
	QPixmap p(pixmap);
	if(p.isNull()){
		QPixmap *icon = images->getIcon("SCRIPT");
		if(icon)
			p = *icon;
	}
	QToolButton *btn = new QToolButton( lowerLayoutWidget, buttonName.local8Bit().data() );
	btn->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0,
											btn->sizePolicy().hasHeightForWidth() ) );
	btn->setMinimumSize( QSize( 24, 24 ) );
	btn->setMaximumSize( QSize( 24, 24 ) );
	btn->setAutoRaise( true );
	btn->setIconSet(p);
	QToolTip::add( btn, tip );
	layout3->addWidget( btn );
	QObject::connect(btn, SIGNAL(clicked()), SLOT(slotCustomBtnClick()));
	m_btnMap[buttonName] = btn;
	m_scriptMap[buttonName] = scriptName;
}

void EvaChatWindow::removeToolButton( QString & /*scriptName*/, QString buttonName )
{
	QMap<QString, QToolButton*>::Iterator it = m_btnMap.find(buttonName);
	if( it == m_btnMap.end()) return;
	layout3->remove(it.data());
	delete it.data();
	m_btnMap.erase(it);
	
	QMap<QString, QString>::Iterator itr = m_scriptMap.find(buttonName);
	if( itr == m_scriptMap.end()) return;
	m_scriptMap.erase(itr);
}

void EvaChatWindow::slotCustomBtnClick( )
{
	const QObject *obj = QObject::sender();
	QString name = QString::fromLocal8Bit( obj->name());
	printf("sender : %s\n", name.local8Bit().data());
	if(m_scriptMap.find(name) == m_scriptMap.end()) return;
	printf("slotCustomBtnClick : %s\n", name.local8Bit().data());
	GetScriptManager()->customMenuClicked(false, buddy->getQQ(), m_scriptMap[name], name); // script name, button name
}


