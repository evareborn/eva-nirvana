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
 
#include "evaqunchatwindow.h"


#include "customfaceselector.h"
#include "evafontselecter.h"

#include "evaresource.h"
#include "evamain.h"
#include "evauser.h"
#include "evausersetting.h"
#include "evatextedit.h"
#include "evachatview.h"
#include "evaqunlist.h"
#include "evaqunlistview.h"
#include "evahistoryviewer.h"
#include "evafriend.h"
#include "evaqtutil.h"
#include "regiongrabber.h"
#include "createsmileywindow.h"
#include "evamainwindow.h"
#include "evascriptmanager.h"
#include "evacontactmanager.h"
#include "evachatwindowmanager.h"

#include <qtextcodec.h>
#include <qiconset.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qevent.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qtooltip.h>
#include <qregexp.h>
#include <qlistview.h>
#include <qtimer.h>
#include <qstringlist.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <quuid.h>
#include <qapplication.h>
#include <qlayout.h>

#include <klocale.h>
#include <kurl.h>
#include <kfiledialog.h>
#include <kapplication.h>

#ifndef QQ_MSG_IM_MAX
#define QQ_MSG_IM_MAX 15000
#endif

unsigned int EvaQunChatWindow::myQQ=0;
QString EvaQunChatWindow::myName ="";
EvaImageResource *EvaQunChatWindow::images = NULL;
bool EvaQunChatWindow::isSentByEnter = false;
std::list<QString> EvaQunChatWindow::quickList;

EvaQunChatWindow::EvaQunChatWindow( Qun * qun, QWidget * parent, const char * name, WFlags fl )
	: EvaQunChatUIBase(parent, name, fl), smileyPopup(NULL), fontSelecter(NULL), quickMenu(NULL),
	mQun(qun), grabber(NULL), viewer(NULL)
{
	codec = QTextCodec::codecForName("GB18030");
	initObjects();
	initInformation();
	initConnection();
	graphicChanged();
	slotDisplayMembers();
	timer = new QTimer();
	QObject::connect(timer, SIGNAL(timeout()), SLOT(slotTimeout()));
	teInput->setFocus();
	if(mQun){
		if(mQun->isFirstRefresh()){
			mQun->firstRefreshDone();
			QTimer::singleShot(200, this, SLOT(slotRequestQunMembers()));
		}
		else
			QTimer::singleShot(200, this, SLOT(slotRequestQunRealNames()));
	}
}

EvaQunChatWindow::~ EvaQunChatWindow( )
{
	if(timer->isActive())
		timer->stop();
}

void EvaQunChatWindow::setupImages( EvaImageResource * res )
{
	if(!res) return;
	images = res;
}

void EvaQunChatWindow::setQuickReplyMessages( const std::list< QString > & list )
{
	quickList = list;
}

void EvaQunChatWindow::setQuickReplyMenu( )
{
}

const unsigned int EvaQunChatWindow::getQunID( )
{
	if(mQun)
		return mQun->getQunID();
	else
		return 0;
}

void EvaQunChatWindow::graphicChanged( )
{
	if(!images) return;
	QStringList imageDirList;
	imageDirList.append(images->getSmileyPath());
	imageDirList.append(EvaMain::user->getSetting()->getPictureCacheDir());
	teInput->mimeSourceFactory()->setFilePath(imageDirList);

	
	tbSmiley->setIconSet(*(images->getIcon("SMILEY")));
	tbFont->setIconSet(*(images->getIcon("FONT")));
	tbB->setIconSet(*(images->getIcon("FONT_B")));
	tbU->setIconSet(*(images->getIcon("FONT_U")));
	tbI->setIconSet(*(images->getIcon("FONT_I")));
	tbImageFile->setIconSet(*(images->getIcon("CUSTOM_SMILEY")));
	tbScreenShot->setIconSet(*(images->getIcon("SCREEN_SHOT")));
	tbQuickReply->setIconSet(*(images->getIcon("QUICK_REPLY")));
	tbEnableSound->setIconSet(*(images->getIcon("SYSTEM_MSG")));
}

void EvaQunChatWindow::slotReceivedMessage( unsigned int qunID, unsigned int senderQQ, QString message, QDateTime time, const char size, 
					const bool u, const bool i, const bool b, 
					const char blue, const char green, const char red )
{
	if(qunID != mQun->getQunID()) return;
	if(senderQQ == myQQ) return;
	QString nick = getSenderName(senderQQ) + " (" + QString::number(senderQQ) + ")";
	EvaHtmlParser parser;
	QString cachesPath = EvaMain::user->getSetting()->getPictureCacheDir();
	parser.setAbsImagePath(images->getSmileyPath(), cachesPath);
	std::list<CustomizedPic> picList = parser.convertToHtml(message, true, true);
	if(picList.size()){
		EvaAskForCustomizedPicEvent *event = new EvaAskForCustomizedPicEvent();
		event->setPicList(picList);
		event->setQunID(qunID);
		QApplication::postEvent((QObject *)EvaMain::picManager, event);
	}
	chatDisplay->append(nick, time, Qt::blue, true, QColor((Q_UINT8)red, (Q_UINT8)green,(Q_UINT8)blue), size, u, i, b, message);
	if( tbEnableSound->isOn())
		EvaMain::global->getSoundResource()->playNewMessage();
}

void EvaQunChatWindow::showMessages()
{
	chatDisplay->showContents();
}

void EvaQunChatWindow::slotAddMessage(unsigned int sender, QString sNick, unsigned int /*receiver*/, 
					QString /*rNick*/, bool isNormal, QString message, 
					QDateTime time, const char /*size*/, const bool /*u*/, const bool /*i*/, const bool /*b*/, 
					const char /*blue*/, const char /*green*/, const char /*red*/)
{
	if(!teInput->isEnabled()) return;
	EvaHtmlParser parser;
	parser.convertToHtml(message, false, false, true);
	QString msg = QString::number(sender) + "(" +sNick + ")" + (isNormal?(""):i18n("(Auto-Reply)")) + "  " + time.toString("yyyy-MM-dd hh:mm:ss") + "<br />" + message;
	kdDebug()  << msg << endl;
	teInput->append(msg);
}

void EvaQunChatWindow::slotAddInformation(const QString &info)
{
	chatDisplay->showInfomation(info);
}

void EvaQunChatWindow::slotSendResult( bool ok )
{
	if(!ok) {
		QMessageBox::information(this, i18n("Message"), i18n("message sent failed"));
	} else
		teInput->setText("");
	pbSend->setEnabled(true);
	teInput->setEnabled(true);
	teInput->setFocus();
}

void EvaQunChatWindow::slotDisplayMembers()
{
	memberList->clear();
	std::list<FriendItem>::iterator iter;
	std::list<FriendItem> list = mQun->getMembers();

	//kdDebug() << "Qun size: " << list.size() << endl;
	int creator = mQun->getDetails().getCreator();
	int id;
	short faceID;
	QString nick;
	for(iter= list.begin(); iter != list.end(); ++iter){
		id = iter->getQQ();
		faceID = iter->getFace();
		nick = EvaTextFilter::filter(codec->toUnicode(iter->getQunRealName().c_str())); // by henry
 		// by henry first, use isEmpty is always better then length() == 0 ;P
		if(nick.isEmpty()){
			nick = EvaTextFilter::filter(codec->toUnicode(iter->getNick().c_str()));
			if(nick.isEmpty())
				nick = QString::number(id);
		}
		//kdDebug() << "Qun member:"<< id << endl;
		QPixmap *pixOn = images->getFace(images->getFaceFileIndex(faceID), true);
		QPixmap *pixOff = images->getFace(images->getFaceFileIndex(faceID), false);
		if(iter->getQQ() == EvaMain::user->getQQ()){
			if(EvaMain::user->hasUserHead() && EvaMain::uhManager){
				QPixmap *uhPic = images->getUserHeadPixmap(iter->getQQ());
				QPixmap *uhPicOff = images->getUserHeadPixmap(iter->getQQ(), true);
				if(uhPic) pixOn = uhPic;
				if(uhPicOff) pixOff = uhPicOff;
			}
		}
		const QQFriend *frd = EvaMain::user->getFriendList().getFriend(id);
		if(frd && frd->hasUserHead()){
			QPixmap *uhPic = images->getUserHeadPixmap(frd->getQQ());
			QPixmap *uhPicOff = images->getUserHeadPixmap(frd->getQQ(), true);
			if(uhPic) pixOn = uhPic;
			if(uhPicOff) pixOff = uhPicOff;
		}
		EvaQunBuddyItem * item = memberList->addQunBuddy(nick, id, pixOn, pixOff);
		if(id == creator){
			item->setCreator(true);
		}
		if(iter->isAdmin())
			item->setAdmin(true);
		if(iter->isShareHolder())
			item->setShareHolder();
	}
	
	memberList->setColumnWidth(0, 20);
	memberList->setColumnWidth(1, 200);
	
	QTimer::singleShot(1000, this, SLOT(slotRequestQunRealNames()));
	//slotTimeout(); // ask for online member manually
}

void EvaQunChatWindow::slotUpdateOnlineMembers()
{
	std::list<unsigned int> onlineList;
	std::list<FriendItem>::iterator iter;
	std::list<FriendItem> members = mQun->getMembers();
	for(iter=members.begin(); iter!=members.end(); ++iter){
		if(iter->isOnline()){
			if(iter->getQQ() != EvaMain::user->getQQ()){
				onlineList.push_back(iter->getQQ());
			}else{
				if(EvaMain::user->getStatus() == EvaUser::Eva_Online || EvaMain::user->getStatus() == EvaUser::Eva_Leave)
					 onlineList.push_back(iter->getQQ());
			}	
		}	
	}
	
	memberList->updateOnlineMembers(onlineList);
	updateQunCountNumbers();
	QTimer::singleShot(100, this, SLOT(slotRequestQunRealNames()));
}

void EvaQunChatWindow::initObjects( )
{
	fontSelecter = new EvaFontSelecter(this);
	fontSelecter->setColor(Qt::black);
	if(smileyPopup) delete smileyPopup;
	smileyPopup = new CustomFaceSelector(false);	
	
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
	teInput->setEnterSendEnabled(isSentByEnter);
	
	tbtnNotice->setIconSet(QIconSet(*(images->getIcon("SYSTEM_MSG"))));
	
}

void EvaQunChatWindow::initInformation( )
{
	if(!mQun) return;
	//QString nick = codec->toUnicode(buddy->getNick().c_str());
	QString name = codec->toUnicode(mQun->getDetails().getName().c_str());
	QString title = i18n("Qun") + QString(" - %1").arg(name);
	setCaption(title);
	QIconSet face;
	setIcon(*(images->getIcon("QUN")));
	face.setPixmap(*(images->getIcon("QUN")),QIconSet::Large);
	tbQunDetails->setIconSet(face);
	tbQunDetails->setTextLabel(name + " ("+QString::number(mQun->getDetails().getExtID()) +")");
	
	QToolTip::add(tbQunDetails, name + "("+QString::number(mQun->getDetails().getExtID()) +") ");
	
	QString notice = codec->toUnicode(mQun->getDetails().getNotice().c_str());
	teNotice->setText(notice);
	teNotice->adjustSize();
}
void EvaQunChatWindow::updateQunCountNumbers( )
// TODO this should be implemented as a slot.
{
	if(!mQun) return;
	QString num = " (" + QString::number(mQun->getNumOnline()) + "/" + QString::number(mQun->getNumMembers()) + ")";
	lblMembers->setText(i18n("Members") + num);
}

void EvaQunChatWindow::initConnection( )
{
	if(smileyPopup){
		connect(smileyPopup, SIGNAL(selectSysFace(int)), this, SLOT(slotSmileySelected(int)));
		connect(smileyPopup, SIGNAL(selectCustomFace(const QString &)), this, SLOT(slotAddImageToInputEdit(const QString &)));
		connect(smileyPopup, SIGNAL(addSmileyClicked()), this, SLOT(slotManageCustomSmileys()));
	}
	QObject::connect(tbQunDetails, SIGNAL(clicked()), this, SLOT(slotTbQunDetailsClick()));
	QObject::connect(tbSmiley, SIGNAL(clicked()), this, SLOT(slotSmileyClick()));
	QObject::connect(tbFont, SIGNAL(clicked()), this, SLOT(slotFontClick()));
	QObject::connect(tbImageFile, SIGNAL(clicked()), this, SLOT(slotImageFileClick()));
	QObject::connect(tbScreenShot, SIGNAL(clicked()), this, SLOT(slotScreenShotClick()));
	QObject::connect(tbQuickReply, SIGNAL(clicked()), this, SLOT(slotQuickReplyClick()));
	QObject::connect(pbHistory, SIGNAL(clicked()), this, SLOT(slotHistoryClick()));
	QObject::connect(pbSendKey, SIGNAL(clicked()), this, SLOT(slotSendKeyClick()));
	QObject::connect(pbSend, SIGNAL(clicked()), this, SLOT(slotSend()));
	QObject::connect(tbtnNotice, SIGNAL(clicked()), this, SLOT(slotNoticeClick()));
	//QObject::connect(tbShowBuddy, SIGNAL(clicked()), this, SLOT(slotTbShowBuddyClick()));
	//QObject::connect(tbShowMe, SIGNAL(clicked()), this, SLOT(slotTbShowMeClick()));
	QObject::connect(teInput, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotInputKeyPress(QKeyEvent *)));
	
	QObject::connect(fontSelecter, SIGNAL(fontChanged(QColor, int)), this, SLOT(slotFontChanged(QColor, int)));
	QObject::connect(pbClose, SIGNAL(clicked()), this, SLOT(slotPbCloseClick()));
	
	QObject::connect(memberList, SIGNAL(requestBuddyInfo(const unsigned int)), this, SIGNAL(requestDetails(const unsigned int)));
	QObject::connect(memberList, SIGNAL(requestQunCard(const unsigned int)), this, SLOT(slotRequestQunCard(const unsigned int)));
	QObject::connect(memberList, SIGNAL(requestQunMembers()), this, SLOT(slotRequestQunMembers()));
	QObject::connect(memberList, SIGNAL(requestChat(const unsigned int)), SLOT(slotRequestChat(const unsigned int)));

	connect(chatDisplay, SIGNAL(saveAsCustomSmiley(QString )), this, SLOT(slotSaveAsCustomSmiley(QString)) );
}

void EvaQunChatWindow::displaySendingMessage( )
{
	EvaHtmlParser parser;
	parser.setAbsImagePath(images->getSmileyPath());
	QString text = teInput->text();
	QString tmp;
	parser.convertToPlainTxt(text, tmp);
	parser.convertToHtml(text,true, true);
	QString name = getSenderName( myQQ);

	int fontSize = mQun->getChatFontSize();
	QColor fontColor(mQun->getChatFontColor());

	chatDisplay->append( name, sendtime, Qt::darkCyan, true, fontColor, (char)fontSize, 
				tbU->isOn(), tbI->isOn(), tbB->isOn(), text);
	showMessages();
	//teInput->setText("");
}

void EvaQunChatWindow::slotSmileySelected( int fid)
{
	if(!teInput->isEnabled()) return;
	QString smiley = "<img src=\"" + QString::number(fid) + ".gif\">";
	
	int para;
	int index;
	QFont saveFont = teInput->font();
	QColor saveColor = teInput->color();
	// determine the current position of the cursor
	teInput->insert("\255", false, true, true);	
	teInput->getCursorPosition(&para,&index);
	QString txt = teInput->text();
	txt.replace(QRegExp("\255"),smiley);
	teInput->setText(txt);
	teInput->setCursorPosition(para, index);
	teInput->setCurrentFont(saveFont);
	teInput->setColor(saveColor);
}

void EvaQunChatWindow::slotTbQunDetailsClick( )
{
	emit requestQunDetails(mQun->getQunID());
}

void EvaQunChatWindow::slotSmileyClick( )
{
	if(smileyPopup){
		QPoint p = teInput->mapToGlobal(QPoint(0,0));
		smileyPopup->setGeometry(p.x() + tbSmiley->x() , p.y(), smileyPopup->width(), smileyPopup->height());
		smileyPopup->show();
	}
}

void EvaQunChatWindow::slotFontClick( )
{
	if(!fontSelecter) return;
	if(fontSelecter->isVisible()) 
		fontSelecter->hide();
	else{
		QPoint p = teInput->mapToGlobal(QPoint(0,0));
		fontSelecter->setSize( mQun->getChatFontSize());
		fontSelecter->setColor(QColor(mQun->getChatFontColor()) );
		fontSelecter->setGeometry(p.x() + tbFont->x() , p.y(), fontSelecter->width(), fontSelecter->height());	
		fontSelecter->show();
	}
}

void EvaQunChatWindow::slotQuickReplyClick( )
{
}

void EvaQunChatWindow::slotQuickReplyActivated( int id )
{
	if(!teInput->isEnabled()) return;
	std::list<QString>::iterator iter;
	int index = 0;
	for(iter=quickList.begin(); iter!=quickList.end(); ++iter){
		if(index == id) break;	
		index++;
	}
	QString message = *iter;
	EvaHtmlParser parser;
	parser.convertToHtml(message, false);	
	teInput->append(message);
	slotSend();
}

void EvaQunChatWindow::slotHistoryClick( )
{
	//emit requestHistory(mQun->getQunID());
	if ( !viewer )
	{
		QString qName = i18n("Qun");

		if (mQun){
			QunInfo info = mQun->getDetails();
			qName = codec->toUnicode(info.getName().c_str());
		}

		viewer = new EvaHistoryViewer(getQunID(), qName, EvaMain::user->getSetting(), true);

		unsigned short faceId = atoi(EvaMain::user->getDetails().at(ContactInfo::Info_face).c_str());
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

void EvaQunChatWindow::slotHistoryWindowClosed()
{
//	viewer->hide();
	pbHistory->setOn(FALSE);
}

void EvaQunChatWindow::slotSendKeyClick( )
{
}

void EvaQunChatWindow::slotSend( )
{
	EvaHtmlParser parser;
	QString msg = teInput->text();
	sendtime = QDateTime::currentDateTime(Qt::LocalTime);

	int fontSize = mQun->getChatFontSize();
	QColor fontColor(mQun->getChatFontColor());
	std::list<QString> outPicList = parser.getCustomImages(msg);
	if(outPicList.size()){
		sendingImageMsg = msg;
		QString toShow = msg;
		parser.setAbsImagePath(images->getSmileyPath());
		parser.parseToAbsPath(toShow, EvaMain::user->getSetting()->getPictureCacheDir());
		QString name = getSenderName( myQQ);
		chatDisplay->append( name, sendtime, Qt::darkCyan, true, fontColor, (char)fontSize, 
					tbU->isOn(), tbI->isOn(), tbB->isOn(), toShow);
		showMessages();
		//teInput->setText("");
		
		EvaSendCustomizedPicEvent *event = new EvaSendCustomizedPicEvent();
		event->setPicList(getSendFiles(outPicList));
		event->setQunID(mQun->getQunID());
		QApplication::postEvent((QObject *)EvaMain::picManager, event);

		pbSend->setEnabled(false);
		teInput->setEnabled(false);
	}else{
		QString tmp;
		parser.convertToPlainTxt(msg, tmp);
		if(msg == ""){
			QMessageBox::information(0, i18n("Message"), i18n("Cannot send empty message."));
			return;
		}
		if(strlen(msg.ascii()) > QQ_MSG_IM_MAX){
			QMessageBox::information(0, i18n("Message"), i18n("Message is too long, cannot send."));
			return;
		}

		pbSend->setEnabled(false);
		teInput->setEnabled(false);
		displaySendingMessage();

		EvaMainWindow *mainWin = EvaMain::g_mainWin;
		if(mainWin)
			mainWin->addQunToRecentList( mQun->getQunID(), true);
		emit sendQunMessage(mQun->getQunID(), msg, (char)fontSize, tbU->isOn(), tbI->isOn(), tbB->isOn(),
				(char)(fontColor.blue()) , (char)(fontColor.green()), (char)(fontColor.red()));	
	}
}

void EvaQunChatWindow::slotInputKeyPress( QKeyEvent * e )
{
	if(isSentByEnter && ((e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return) ) && 
			(e->state() == Qt::NoButton) ){
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

void EvaQunChatWindow::slotFontChanged( QColor color, int size)
{
	mQun->setChatFontSize(size);
	mQun->setChatFontColor( color.rgb());
	EvaMain::user->saveQunList();
}

void EvaQunChatWindow::setEnterSend( )
{
	isSentByEnter = true;
	sendKey->setItemChecked(1,true);
	sendKey->setItemChecked(2,false);
	teInput->setEnterSendEnabled(isSentByEnter);
}

void EvaQunChatWindow::setCtrlEnterSend( )
{
	isSentByEnter = false;
	sendKey->setItemChecked(1,false);
	sendKey->setItemChecked(2,true);
	teInput->setEnterSendEnabled(isSentByEnter);
}

void EvaQunChatWindow::slotPbCloseClick( )
{
	close();
}

QString EvaQunChatWindow::getSenderName(const unsigned int qqNum)
{
	const FriendItem *item = mQun->getMemberDetails(qqNum);
	QString name;
	if(item){
		name = codec->toUnicode(item->getQunRealName().c_str());
		if(name.isEmpty()){
			if( qqNum != myQQ)
				name = codec->toUnicode(item->getNick().c_str());
			else
				name =myName;
		}
	} else{
		if(qqNum != myQQ)
			name = QString::number(qqNum);
		else
			name = myName;
	}

	return name;
}

void EvaQunChatWindow::slotTimeout()
{
	emit requestQunOnlineList(mQun->getQunID());
}

void EvaQunChatWindow::showEvent( QShowEvent * e )
{
	slotTimeout();
	if(!timer->isActive())
		timer->start(30000, false);
	QWidget::showEvent(e);
}

void EvaQunChatWindow::closeEvent( QCloseEvent * e )
{
	if(timer->isActive())
		timer->stop();
	if (viewer)
		delete viewer;
	QWidget::closeEvent(e);
}

void EvaQunChatWindow::hideEvent( QHideEvent * e )
{
	if(timer->isActive())
		timer->stop();
	QWidget::hideEvent(e);
}

void EvaQunChatWindow::slotRequestQunMembers( )
{	
	//GetContactManager()->fetchQunDetails(mQun->getQunID());
	GetContactManager()->fetchQunMembersInfo(mQun->getQunID());
	//emit requestQunMembers(mQun->getQunID());
}

void EvaQunChatWindow::slotPictureReady( const QString filename , const QString tmpFileName)
{
	if(filename.isEmpty() || tmpFileName.isEmpty()) return;
	chatDisplay->updatePicture(filename, tmpFileName);
}

void EvaQunChatWindow::slotImageFileClick( )
{
	if(!teInput->isEnabled()) return;
	QString destDir = EvaMain::user->getSetting()->getPictureCacheDir();
	QString fileName = KFileDialog::getOpenFileName(destDir,
			"*.png *.bmp *.jpg *.jpeg *.gif |" + i18n(" all images (*.png *.bmp *.jpg *.jpeg *.gif)"), this, 
			i18n("select an image file"));
	if(!fileName.isEmpty()){
		QString destFile = EvaHelper::generateCustomSmiley(fileName, destDir);
		if(destFile.isEmpty()) return;
		slotAddImageToInputEdit(destFile);
	}
}

const std::list<OutCustomizedPic> EvaQunChatWindow::getSendFiles(const std::list<QString> &fileList)
{
	std::list<QString> outPicList = fileList;
	std::list<OutCustomizedPic> picList;
	
	std::list<QString>::iterator iter;
	for(iter=outPicList.begin(); iter!=outPicList.end(); ++iter){
		QString file = EvaMain::user->getSetting()->getPictureCacheDir() + "/" + QString(*iter);
		QFileInfo info(file);
		if(!info.exists()) continue;
		int len = info.size();
		OutCustomizedPic pic;
		pic.fileName = file; 
		pic.imageLength = len;
		EvaHelper::getFileMD5(pic.fileName, (char*)pic.md5);
		picList.push_back(pic);
	}
	return picList;
}

void EvaQunChatWindow::sendImageError( const QString message )
{
	//FIXME we might make it elegent as Tencent dose, displaying message in the display window
	QMessageBox::information(0, i18n("Send Image Error"), message);
	pbSend->setEnabled(true);
	teInput->setEnabled(true);
	teInput->setFocus();
}

void EvaQunChatWindow::slotSendImageDone( const unsigned int agentSessionID, const unsigned int agentIP, const unsigned short agentPort )
{
	sessionID = agentSessionID;
	ip = agentIP;
	port = agentPort;
	EvaHtmlParser parser;
	//QString msg = teInput->text();
	//parser.convertToPlainTxt(msg, sessionID, ip, port);
	parser.convertToPlainTxt(sendingImageMsg, sessionID, ip, port);
	if(sendingImageMsg == ""){
		QMessageBox::information(0, i18n("Message"), i18n("Cannot send empty message."));
		return;
	}
	if(strlen(sendingImageMsg.ascii()) > QQ_MSG_IM_MAX){
		QMessageBox::information(0, i18n("Message"), i18n("Message is too long, cannot send."));
		return;
	}
	sendtime = QDateTime::currentDateTime(Qt::LocalTime);
	//displaySendingMessage();
	int fontSize = mQun->getChatFontSize();
	QColor fontColor(mQun->getChatFontColor());

	EvaMainWindow *mainWin = EvaMain::g_mainWin;
	if(mainWin)
		mainWin->addQunToRecentList( mQun->getQunID(), true);
	emit sendQunMessage(mQun->getQunID(), sendingImageMsg, (char)fontSize, tbU->isOn(), tbI->isOn(), tbB->isOn(),
			(char)(fontColor.blue()) , (char)(fontColor.green()), (char)(fontColor.red()));
}

void EvaQunChatWindow::slotScreenShotClick( )
{
	if(!teInput->isEnabled()) return;
	if(grabber) delete grabber;
	grabber = new RegionGrabber();
	connect( grabber, SIGNAL( regionGrabbed( const QPixmap & ) ),
		SLOT( slotRegionGrabbed( const QPixmap & ) ) );
}

void EvaQunChatWindow::slotRegionGrabbed( const QPixmap & pix)
{
	if ( !pix.isNull() ){
		QString dir = EvaMain::user->getSetting()->getPictureCacheDir();
		
		//QImage img = pix.convertToImage();
		QString file = QUuid::createUuid().toString().upper() + ".JPG";
		pix.save(dir+"/"+file, "JPEG");
		
		QFileInfo info(dir+"/"+file);
		if(info.size() > 50000){
			printf("image too big, sorry\n");
		}else{
			char *md5 = new char[16];
			EvaHelper::getFileMD5(dir+"/"+file, md5);
			QString dest = EvaHelper::md5ToString(md5) + ".JPG";
			EvaHelper::rename(dir+"/"+file, dir+"/"+dest);
			delete md5;
			slotAddImageToInputEdit(dest);
		}
	}
	
	if(grabber) delete grabber;
	grabber = NULL;
	QApplication::restoreOverrideCursor();
}

void EvaQunChatWindow::slotAddImageToInputEdit( const QString & destFile)
{
	if(!teInput->isEnabled()) return;
	if(destFile.isEmpty()) return;
	
	QString smiley = "<img src=\"" + destFile + "\">";
	int para;
	int index;
	QFont saveFont = teInput->font();
	QColor saveColor = teInput->color();
	// determine the current position of the cursor
	teInput->insert("\255", false, true, true);
	teInput->getCursorPosition(&para,&index);
	QString txt = teInput->text();
	txt.replace(QRegExp("\255"),smiley);
	teInput->setText(txt);
	teInput->setCursorPosition(para, index);
	teInput->setCurrentFont(saveFont);
	teInput->setColor(saveColor);
}

void EvaQunChatWindow::slotRequestQunCard( const unsigned int id )
{
	emit requestQunCard(mQun->getQunID(), id);
}

void EvaQunChatWindow::slotNoticeClick( )
{
	//QRect rect = chatDisplay->view()->geometry();
//	if(teNotice->isVisible())
//		teNotice->hide();
//	else
//		teNotice->show();
	//adjustSize();
	//chatDisplay->view()->setGeometry(rect);
	//resize( QSize(470, 422).expandedTo(minimumSizeHint()) );
}

void EvaQunChatWindow::slotRequestQunRealNames( )
{
	if(mQun){
		emit requestQunRealNames(mQun->getQunID());
	}
}

void EvaQunChatWindow::slotManageCustomSmileys()
{
//	CreateSmileyWindow *win = new CreateSmileyWindow();
//	win->show();
	CustomSmileyManager *win = new CustomSmileyManager;
	connect(win, SIGNAL(customSmileyChanged()), this, SLOT(slotCustomSmileyChanged()) );
	win->show();
}

void EvaQunChatWindow::slotSaveAsCustomSmiley( QString url)
{
	CreateSmileyWindow *win = new CreateSmileyWindow(url);
	connect(win, SIGNAL(addCustomSmileyReady( bool )), this, SLOT(slotAddCustomSmileyReady( bool )));
	win->show();
}

void EvaQunChatWindow::slotAddCustomSmileyReady( bool ready)
{
	if(ready){
		slotCustomSmileyChanged();
	}
}

void EvaQunChatWindow::slotCustomSmileyChanged( )
{
	printf("EvaQunChatWindow :: slotCustomSmileyChanged\n");
	if(smileyPopup) {
		disconnect(smileyPopup, 0, 0, 0);
		delete smileyPopup;
	}
	smileyPopup = new CustomFaceSelector(false);
	connect(smileyPopup, SIGNAL(selectSysFace(int)), this, SLOT(slotSmileySelected(int)));
	connect(smileyPopup, SIGNAL(selectCustomFace(const QString &)), this, SLOT(slotAddImageToInputEdit(const QString &)));
	connect(smileyPopup, SIGNAL(addSmileyClicked()), this, SLOT(slotManageCustomSmileys()));
}

void EvaQunChatWindow::appendText( const QString & txt )
{
	if(!teInput->isEnabled()) return;
	teInput->append( txt );
}

void EvaQunChatWindow::addToolButton( QString & scriptName, QString buttonName, QString & pixmap, QString & tip )
{
	if(m_btnMap.find(buttonName) != m_btnMap.end()) return;
	
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
	layout14->addWidget( btn );
	QObject::connect(btn, SIGNAL(clicked()), SLOT(slotCustomBtnClick()));
	m_btnMap[buttonName] = btn;
	m_scriptMap[buttonName] = scriptName;
}

void EvaQunChatWindow::removeToolButton( QString & /*scriptName*/, QString buttonName )
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

void EvaQunChatWindow::slotCustomBtnClick( )
{
	const QObject *obj = QObject::sender();
	QString name = QString::fromLocal8Bit( obj->name());
	printf("Qun sender : %s\n", name.local8Bit().data());
	if(m_scriptMap.find(name) == m_scriptMap.end()) return;
	printf("Qun slotCustomBtnClick : %s\n", name.local8Bit().data());
	GetScriptManager()->customMenuClicked(true, mQun->getExtQunID(), m_scriptMap[name], name); // script name, button name
}

void EvaQunChatWindow::slotRequestChat( const unsigned int id )
{
	QQFriend *frd = EvaMain::user->getFriendList().getFriend(id);
	if(frd)
		EvaMain::g_ChatWindowManager->openChatWindow(frd);
}

