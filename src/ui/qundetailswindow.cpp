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


#include "qundetailswindow.h"
#include <qlineedit.h>
#include <qtextedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
//#include <qiconset.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qbuttongroup.h>
#include <qtable.h>
#include <qimage.h>
#include <qtextcodec.h>
#include <qevent.h>
#include <qpoint.h>
#include <qtoolbutton.h>
#include <qmessagebox.h>
#include "evaqunlist.h"
#include "evamain.h"
#include "evaresource.h"
#include "evauser.h"
#include "evaqtutil.h"
#include "evaqunmemberpicker.h"
#include "quncategorypicker.h"
#include "evacontactmanager.h"

#include <kmessagebox.h>
#include <klocale.h>


// QunDetailsWindow::QunDetailsWindow(QWidget* parent, const char* name, WFlags fl)
// 	: QunDetailsUI(parent,name,fl), m_CardId(0)
// {
// 	codec = QTextCodec::codecForName("GB18030");
// 	initTable( );
// 	slotLoadMembers();
// }

QunDetailsWindow::~QunDetailsWindow()
{
	delete mQun;
}

QunDetailsWindow::QunDetailsWindow( Qun * qun, const bool isInList, QWidget * parent, const char * name, WFlags fl )
	: QunDetailsUI(parent,name,fl), mQun(qun), m_IsInList(isInList), m_CardId(0)
{
	mQun = new Qun(*qun);
	codec = QTextCodec::codecForName("GB18030");
	initInformation();
	if(isInList){
		initTable( );
		slotLoadMembers();
	
		picker = new EvaQunMemberPicker(0, "qunpicker", WStyle_Customize | WStyle_NoBorder | 
									WStyle_StaysOnTop |  WStyle_Tool| WX11BypassWM, mQun);
		picker->resize(QSize(240,frameGeometry().height()));
		QObject::connect(picker, SIGNAL(memberClicked(const unsigned int, const bool)), 
					SLOT(slotPickerMemberClicked(const unsigned int, const bool)));
		QObject::connect(this, SIGNAL(removeMember(const unsigned int)), picker, SLOT(slotSetMemberOff(const unsigned int)));

		QObject::connect(pbUpdate, SIGNAL(clicked()), SLOT(slotUpdateClicked()));
	} else {
		picker = 0;
		setWidgetEnabledForInformation(false);
		tabWMain->setTabEnabled( tabMember, false);
		tabWMain->setTabEnabled( tabSetting, false);
		tabWMain->setTabEnabled( TabCard, false);
		pbUpdate->setEnabled(false);
	}

	QObject::connect(pbClose, SIGNAL(clicked()), SLOT(slotClose()));
}

void QunDetailsWindow::initInformation( )
{
	if(!mQun) return;
	
	QunInfo info = mQun->getDetails();
	QString title, name;
	if(info.getExtID() == 0){
		name = QString::number(mQun->getQunID());
	}else
		name = codec->toUnicode(mQun->getDetails().getName().c_str());
	title = i18n("Qun") + QString(" - %1").arg(name);
	setCaption(title);
	//QIconSet face;
	setIcon(*(EvaMain::images->getIcon("QUN")));
	lblLogo->setPixmap(*(EvaMain::images->getIcon("QUN_LOGO")));
	
	lblQunFace->setPixmap(*(EvaMain::images->getIcon("QUN")));
	if(info.getExtID() == 0){
		leQunName->setText( name);
		return;
	}
	
	QString notice = codec->toUnicode(mQun->getDetails().getNotice().c_str());
	QString description = codec->toUnicode(mQun->getDetails().getDescription().c_str());
	
	if(mQun->getDetails().getCreator() == EvaMain::user->getQQ() ||
			(mQun->isAdmin(EvaMain::user->getQQ())) ){
		leQunName->setEnabled(true);
		teNotice->setEnabled(true);
		teDescription->setEnabled(true);
		bgAuthen->setEnabled(true);
		tbCategory->setEnabled(true);
		QObject::connect(tbCategory, SIGNAL(clicked()), SLOT(slotCategoryClicked()));
	}else{
		leQunName->setEnabled(false);
		teNotice->setEnabled(false);
		teDescription->setEnabled(false);
		bgAuthen->setEnabled(false);
		tbCategory->setEnabled(false);
	}
	
	
	leQunID->setText( QString::number(mQun->getDetails().getExtID()));
	leCreator->setText( QString::number(mQun->getDetails().getCreator()));
	leQunName->setText( name);
	
	category = mQun->getDetails().getCategory();
	QunCategory qunCate;
	QString cateDescription = qunCate.getDescription(category);
	tbCategory->setText(cateDescription);
	
	teNotice->setText(notice);
	teDescription->setText(description);
	
	switch(mQun->getDetails().getAuthType()){
	case QQ_QUN_NO_AUTH:
		rbNoAuthenNeed->setChecked(true);
		break;
	case QQ_QUN_NEED_AUTH:
		rbNeedAuthen->setChecked(true);
		break;
	case QQ_QUN_NO_ADD:
		rbRejectAnyone->setChecked(true);
		break;
	}
	switch(mQun->getMessageType()){
	case Qun::Notify:
		rbRecord_Show->setChecked(true);
		break;
	case Qun::Popup:
		rbPopupWindow->setChecked(true);
		break;
	case Qun::Numbers:
		rbDisplayNumbers->setChecked(true);
		break;
	case Qun::RecordOnly:
		rbRecord_only->setChecked(true);
		break;
	case Qun::Reject:
		rbRejectMsgs->setChecked(true);
		break;
	default:
		rbRecord_Show->setChecked(true);
	}
	
	QString realName = codec->toUnicode(mQun->getCardName().c_str());
	QString phone = codec->toUnicode(mQun->getCardPhone().c_str());
	QString email = codec->toUnicode(mQun->getCardEmail().c_str());
	QString memo = codec->toUnicode(mQun->getCardMemo().c_str());
	
	leMyName->setText(realName);
	cbbMyGender->setCurrentItem(mQun->getCardGender());
	leMyPhone->setText(phone);
	leMyEmail->setText(email);
	teMyMemo->setText(memo);
}

void QunDetailsWindow::slotUpdateClicked( )
{
	if(mQun->getDetails().getCreator() != EvaMain::user->getQQ() &&  // not creator
		!(mQun->isAdmin(EvaMain::user->getQQ())) &&              // not admin
		tabWMain->currentPageIndex() != 2 &&                     // not message setting
		tabWMain->currentPageIndex() != 3){                      // not modifying qun card
		pbUpdate->setEnabled(false);
		emit requestQunInfo(mQun->getQunID());
		return;
	}
	switch(tabWMain->currentPageIndex()){
	case 0:
		slotModifyInfo();
		break;
	case 1:
		slotModifyMembers();
		break;
	case 2:
		slotUpdateMessageSetting();
		break;
	case 3:
		slotModifyQunCard();
		break;
	}
}

void QunDetailsWindow::slotModifyInfo( )
{
	if(leQunName->text().stripWhiteSpace().isEmpty()){
		QMessageBox::information(this, i18n("Eva Modify Qun Inforamtion"), 
					i18n("Qun name cannot be empty."));
		leQunName->setFocus();
		return;
	}
	
	int id = mQun->getQunID();
	
	unsigned char auth = 0x02;
	if(rbNoAuthenNeed->isChecked())
		auth = 0x01;
	else if(rbNeedAuthen->isChecked())
		auth = 0x02;
	else if(rbRejectAnyone->isChecked())
		auth = 0x03;
	
	//unsigned short cate = mQun->getDetails().getCategory();
	QString name = leQunName->text();
	QString notice = teNotice->text();
	QString description = teDescription->text();
	setWidgetEnabledForInformation(false);
	pbUpdate->setEnabled(false);
	emit requestModifyQunInfo(id, auth, category, name, notice, description);
}

void QunDetailsWindow::setWidgetEnabledForInformation( bool enabled )
{
	leQunName->setEnabled(enabled);
	teNotice->setEnabled(enabled);
	teDescription->setEnabled(enabled);
	bgAuthen->setEnabled(enabled);
	tbCategory->setEnabled(enabled);
}

void QunDetailsWindow::slotModifyQunInfo(const unsigned int id, bool ok , QString msg)
{
	if(id != mQun->getQunID()) return;
	setWidgetEnabledForInformation(true);
	GetContactManager()->fetchQunDetails( mQun->getQunID());
	QString title = i18n("Eva Modify Qun Inforamtion");
	if(ok){
		QMessageBox::information(this, title, i18n("Modify Qun information sucessfully."));
		return;
	} else {
		QMessageBox::information(this, title, msg);
	}
}

void QunDetailsWindow::slotQunInfomationReady(const unsigned int id, const bool ok, QString msg)
{
	if(!ok){
		QMessageBox::information( this, i18n("Qun Details"), msg);
		return;
	}
	
	if(id != mQun->getQunID()) return;

	mQun = EvaMain::user->getQunList()->getQun(id);
	
	QString name = codec->toUnicode(mQun->getDetails().getName().c_str());
	QString title = i18n("Qun") + QString(" - %1").arg(name);
	setCaption(title);
	
	QString notice = codec->toUnicode(mQun->getDetails().getNotice().c_str());
	QString description = codec->toUnicode(mQun->getDetails().getDescription().c_str());
	
	leQunID->setText( QString::number(mQun->getDetails().getExtID()));
	leCreator->setText( QString::number(mQun->getDetails().getCreator()));
	leQunName->setText( name);
	teNotice->setText(notice);
	teDescription->setText(description);
	
	switch(mQun->getDetails().getAuthType()){
	case QQ_QUN_NO_AUTH:
		rbNoAuthenNeed->setChecked(true);
		break;
	case QQ_QUN_NEED_AUTH:
		rbNeedAuthen->setChecked(true);
		break;
	case QQ_QUN_NO_ADD:
		rbRejectAnyone->setChecked(true);
		break;
	}
	pbUpdate->setEnabled(true);
	
	slotLoadMembers();
}

void QunDetailsWindow::slotModifyQunCard( )
{
	if(m_CardId && m_CardId != EvaMain::user->getQQ()){
		emit requestQunCard(mQun->getQunID(), m_CardId);
		return;
	}
	QString name = leMyName->text();
	unsigned char gender = (unsigned char)(cbbMyGender->currentItem());
	QString phone = leMyPhone->text();
	QString email = leMyEmail->text();
	QString memo = teMyMemo->text();
	
	QString title = i18n("Modify Qun Card");
	QString content = i18n("Your input is too long.");
	if(strlen(name.ascii()) > 255){
		QMessageBox::information(this, title, content);
		leMyName->setFocus();
		return;	
	}
	if(strlen(phone.ascii()) > 255){
		QMessageBox::information(this, title, content);
		leMyPhone->setFocus();
		return;	
	}
	if(strlen(email.ascii()) > 255){
		QMessageBox::information(this, title, content);
		leMyEmail->setFocus();
		return;	
	}
	if(strlen(memo.ascii()) > 255){
		QMessageBox::information(this, title, content);
		teMyMemo->setFocus();
		return;
	}
	 
	pbUpdate->setEnabled(false);
	emit requestModifyQunCard(mQun->getQunID(), EvaMain::user->getQQ(), name, gender, phone, email, memo);
}

void QunDetailsWindow::slotModifyQunCardReply( const unsigned int id, const bool ok, const unsigned int /*qqID*/, QString msg)
{
	if(ok && id != mQun->getQunID()) return;
	pbUpdate->setEnabled(true);
	QString title = i18n("Modify Qun Card");
	if(ok){
		QMessageBox::information(this, title, i18n("Modify Qun Card sucessfully."));
		
		QString name = leMyName->text();
		unsigned char gender = (unsigned char)(cbbMyGender->currentItem());
		QString phone = leMyPhone->text();
		QString email = leMyEmail->text();
		QString memo = teMyMemo->text();
		
		std::string stdName = codec->fromUnicode(name).data();
		std::string stdPhone = codec->fromUnicode(phone).data();
		std::string stdEmail = codec->fromUnicode(email).data();
		std::string stdMemo = codec->fromUnicode(memo).data();
		EvaMain::user->getQunList()->setMyQunCardInfo(id, stdName, gender, stdPhone, stdEmail, stdMemo);
		return;
	} else {
		QMessageBox::information(0, title, msg);
	}	
}

void QunDetailsWindow::slotLoadMembers( )
{
	for(int row=0; row<tblMembers->numRows(); row++){
		for(int col=0; col<tblMembers->numCols(); col++){
			tblMembers->clearCell(row, col);
		}
	}
	
	int i=0;
	std::list<FriendItem>::iterator iter;
	std::list<FriendItem> list = mQun->getMembers();
	unsigned short face;
	unsigned int id;
	QString nick;
	
	tblMembers->setNumRows(list.size());
	for(iter = list.begin(); iter!=list.end(); iter++){
		face = iter->getFace();
		id = iter->getQQ();
		nick = EvaTextFilter::filter(codec->toUnicode(iter->getQunRealName().c_str())); 
		if(nick.isEmpty())
			nick = EvaTextFilter::filter(codec->toUnicode(iter->getNick().c_str()));
		QPixmap *typePic = NULL;
		if(iter->isAdmin())
			typePic = EvaMain::images->getIcon("QUN_ADMIN");
		if(iter->isShareHolder())
			typePic = EvaMain::images->getIcon("QUN_SHAREHOLDER");
		if(mQun->getDetails().getCreator() == id)
			typePic = EvaMain::images->getIcon("QUN_CREATOR");
		
		if(typePic)
			tblMembers->setPixmap(i,0, *typePic);
		QPixmap *bmpFace = EvaMain::images->getFace(EvaMain::images->getFaceFileIndex(face));
		
		if(bmpFace){
			QImage img(bmpFace->convertToImage().smoothScale(16, 16));
			tblMembers->setPixmap(i, 1, QPixmap(img));
		}
		tblMembers->setText(i,1,QString::number(id));
		tblMembers->setText(i,2,nick);
		tblMembers->setText(i,3, iter->isBoy()?i18n("Male"):i18n("Female"));

		i++;
	}
}

void QunDetailsWindow::initTable( )
{
	QHeader *vheader = tblMembers->verticalHeader();
	vheader->hide();
	tblMembers->setLeftMargin(0);
	tblMembers->setNumRows( 0 );
	tblMembers->setNumCols( 4 );
	tblMembers->setColumnWidth(0, 16);
	tblMembers->setColumnWidth(1, 90);
	tblMembers->setColumnWidth(2, 110);
	tblMembers->setColumnWidth(3, 34);
	tblMembers->horizontalHeader()->setLabel( 0, i18n( " " ) );
	tblMembers->horizontalHeader()->setLabel( 1, i18n( "QQ" ) );
	tblMembers->horizontalHeader()->setLabel( 2, i18n( "Nick" ) );
	tblMembers->horizontalHeader()->setLabel( 3, i18n( "Gender" ) );
	tblMembers->setSelectionMode( QTable::SingleRow );
	tblMembers->setFocusStyle( QTable::FollowStyle );
	tblMembers->setReadOnly( true );
	QObject::connect(tblMembers, SIGNAL(clicked(int,int,int,const QPoint&)), SLOT(slotTableClicked(int,int,int,const QPoint&)));

	unsigned int myid = EvaMain::user->getQQ();
	if( mQun->isAdmin( myid) || mQun->getDetails().getCreator() == myid){
		pbSetMembers->setEnabled(true);
		printf("setMember enabled\n");
	} else {
		pbSetMembers->setEnabled(false);
		printf("setMember disabled\n");
	}
	
	pbAddToMe->setEnabled(false);
	pbDelMembers->setEnabled(false);
	pbSetAdmin->setEnabled(false);
	pbUnsetAdmin->setEnabled(false);
	pbTransfer->setEnabled(false);
	
	QObject::connect(pbSetMembers, SIGNAL(clicked()), SLOT(slotSetMembersClicked()));
	QObject::connect(pbAddToMe, SIGNAL(clicked()), SLOT(slotAddToMeClicked()));
	QObject::connect(pbDelMembers, SIGNAL(clicked()), SLOT(slotDelMembersClicked()));
	QObject::connect(pbSetAdmin, SIGNAL(clicked()), SLOT(slotSetAdminClicked()));
	QObject::connect(pbUnsetAdmin, SIGNAL(clicked()), SLOT(slotUnsetAdminClicked()));
	QObject::connect(pbTransfer, SIGNAL(clicked()), SLOT(slotTransferClicked()));
}

void QunDetailsWindow::slotTableClicked( int row, int /*col*/, int /*button*/, const QPoint & /*mousePos*/ )
{
	QString num = tblMembers->text(row, 1);
	bool ok;
	unsigned int id = num.toUInt(&ok);
	
	unsigned int myID = EvaMain::user->getQQ();
	if(ok){
		const FriendItem *item = mQun->getMemberDetails(id);
		if(!item){
			pbSetMembers->setEnabled(true);
			pbAddToMe->setEnabled(false);
			pbDelMembers->setEnabled(true);
			pbSetAdmin->setEnabled(false);
			pbUnsetAdmin->setEnabled(false);
			pbTransfer->setEnabled(false);
			return;
		}
		if(EvaMain::user->getFriendList().hasFriend(id) || myID == item->getQQ()){
			pbAddToMe->setEnabled(false);
		}else{
			pbAddToMe->setEnabled(true);
		}
	 	
		if(myID != mQun->getDetails().getCreator() && !mQun->isAdmin(myID)){
			pbSetMembers->setEnabled(false);
			return;  // neither creator nor admin, we just return
		}else
			pbSetMembers->setEnabled(true);

		if(myID == item->getQQ())
			pbDelMembers->setEnabled(false);
		else
			pbDelMembers->setEnabled(true);	

		pbTransfer->setEnabled(false);
		if( myID == mQun->getDetails().getCreator()){
			if(id != myID){ // selected other member
				if(item->getQunAdminValue() & QUN_TYPE_ADMIN ){
					pbSetAdmin->setEnabled(false);
					pbUnsetAdmin->setEnabled(true);
				} else {
					pbSetAdmin->setEnabled(true);
					pbUnsetAdmin->setEnabled(false);
				}
				pbTransfer->setEnabled(true);
			}
		}
	} else {
		pbAddToMe->setEnabled(false);
		pbDelMembers->setEnabled(false);
		pbSetAdmin->setEnabled(false);
		pbUnsetAdmin->setEnabled(false);
		pbTransfer->setEnabled(false);
	}
}

void QunDetailsWindow::slotSetMembersClicked()
{
	if(picker->isVisible()){
		pbSetMembers->setText(i18n("&Members >>"));
		picker->hide();
	}else{
		pbSetMembers->setText(i18n("&Members <<"));
		picker->move(x() + frameGeometry().width(), y());
		picker->show();
	}
}

void QunDetailsWindow::slotAddToMeClicked()
{
	QString num = tblMembers->text(tblMembers->currentRow(), 1);
	bool ok;
	int id = num.toInt(&ok);
	if(ok){
		const FriendItem *item = mQun->getMemberDetails(id);
		//emit requestAddBuddy(id);
		emit requestAddBuddy(id, codec->toUnicode(item->getNick().c_str()), item->getFace());
		close();
	} 
}

void QunDetailsWindow::slotDelMembersClicked()
{
	int row = tblMembers->currentRow();
	QString txt = tblMembers->text(row, 1);
	bool ok;
	unsigned int id = txt.toUInt(&ok);
	if(!ok) return;
	
	if(id == EvaMain::user->getQQ()) return;
	
	tblMembers->removeRow(row);
	tblMembers->selectRow(0);
	slotTableClicked(0, 1, 0, QPoint(0,0));
	emit removeMember(id);
}

void QunDetailsWindow::slotSetAdminClicked()
{
	QString num = tblMembers->text(tblMembers->currentRow(), 1);
	bool ok;
	int id = num.toInt(&ok);
	if(ok){
		pbSetAdmin->setEnabled(false);
		emit requestQunSetAdmin(mQun->getQunID(), id, true);  // note last arg "true" means to set id as an Administrator
	}
}

void QunDetailsWindow::slotUnsetAdminClicked()
{
	QString num = tblMembers->text(tblMembers->currentRow(), 1);
	bool ok;
	int id = num.toInt(&ok);
	if(ok){
		pbUnsetAdmin->setEnabled(false);
		emit requestQunSetAdmin(mQun->getQunID(), id, false);  // note last arg "false" means to unset  Administrator on id
	}
}

void QunDetailsWindow::slotTransferClicked()
{
	QString num = tblMembers->text(tblMembers->currentRow(), 1);
	bool ok;
	int id = num.toInt(&ok);
	if(ok){
		pbTransfer->setEnabled(false);
		emit requestQunTransfer(mQun->getQunID(), id);
	}	
}

void QunDetailsWindow::slotSetAdmin(const unsigned int id, const bool ok,  const unsigned int qq, const bool isSetAdmin, QString msg)
{
	if(ok && id != mQun->getQunID()) return;
	QString title = i18n("Qun Administrator Setting");
	QString message;
	
	if(isSetAdmin)
		message = QString(i18n("Set \"%1\" as Qun administrator sucessfully.")).arg(qq);
	else
		message = QString(i18n("Unset \"%1\" as Qun administrator sucessfully.")).arg(qq);

	if(ok){
		QMessageBox::information(this, title, message);
		//slotClose( );
	} else {
		QMessageBox::information(this, title, msg);
	}
}

void QunDetailsWindow::slotTransferQun( const unsigned int id, const bool ok, const unsigned int qq, QString msg )
{
	if(ok && id != mQun->getQunID()) return;
	QString title = i18n("Qun Transfer");

	if(ok){
		QMessageBox::information(this, title, QString(i18n("Transfer Qun to \"%1\" sucessfully.")).arg(qq));
		//slotClose( );
	} else {
		QMessageBox::information(this, title, msg);
	}	
}

void QunDetailsWindow::slotClose( )
{
	close();
}

void QunDetailsWindow::closeEvent( QCloseEvent * event)
{
	if(picker)
		delete picker;
	picker=NULL;
	QunDetailsUI::closeEvent(event);
	deleteLater();
}

void QunDetailsWindow::moveEvent( QMoveEvent * event )
{
	if(picker)
		picker->move(x() + frameGeometry().width(), y());
	QunDetailsUI::moveEvent(event);
}

void QunDetailsWindow::slotPickerMemberClicked( const unsigned int id, const bool isChecked )
{	
	int row;
	unsigned int qq;
	bool ok;
	QString txt;
	if(isChecked){
		const QQFriend * frd = (EvaMain::user->getFriendList()).getFriend(id);
		if(!frd) return;
		//if(mQun->hasMember(frd->getQQ())) return;
		for(row=0; row<tblMembers->numRows(); row++){
			txt = tblMembers->text(row, 1);
			qq = txt.toInt(&ok);
			if(!ok) continue;
			if(qq == id) return;
		}
		
		unsigned short face;
		QString nick;
		face = frd->getFace();
		nick = codec->toUnicode(frd->getNick().c_str());
		
		row = tblMembers->numRows(); // note: the index is start from 0
		tblMembers->setNumRows(tblMembers->numRows() + 1);
		
		QPixmap *bmpFace = EvaMain::images->getFace(EvaMain::images->getFaceFileIndex(face));
		
		if(bmpFace){
			QImage img(bmpFace->convertToImage().smoothScale(16, 16));
			tblMembers->setPixmap(row, 1, QPixmap(img));
		}
		tblMembers->setText(row,1,QString::number(id));
		tblMembers->setText(row,2,nick);
		tblMembers->setText(row,3, frd->isBoy()?i18n("Male"):i18n("Female"));
	}else {
		bool ok;
		QString txt;
		unsigned int qq;
		for(row = 0; row < tblMembers->numRows(); row ++){
			txt = tblMembers->text(row, 1);
			qq = txt.toInt(&ok);
			if(!ok) continue;
			if(qq == id){
				tblMembers->removeRow(row);
				return;
			}
		}
	}
}

void QunDetailsWindow::slotModifyMembers( )
{
	std::list<unsigned int> removeList;
	std::list<unsigned int> addList;
	
	std::list<FriendItem>::iterator iter;
	std::list<FriendItem> list = mQun->getMembers();
	for(iter = list.begin(); iter!=list.end(); ++iter){
		removeList.push_back(iter->getQQ());
	}
	
	bool ok;
	QString txt;
	unsigned int qq;
	for(int row = 0; row < tblMembers->numRows(); row ++){
		txt = tblMembers->text(row, 1);
		qq = txt.toInt(&ok);
		if(!ok) return;
		addList.push_back(qq);printf("qq: %d\n",qq);
	}
	printf("addList size: %d\n", addList.size());
	std::list<unsigned int>::iterator itr1, itr1_next;
	std::list<unsigned int>::iterator itr2;
	for(itr1=addList.begin(); itr1!=addList.end(); ){
		itr1_next = itr1;printf("iter1 contents:%d\n", *itr1);
		++itr1_next;printf("iter1_next contents:%d\n", *itr1_next);
		for(itr2=removeList.begin(); itr2!=removeList.end(); ++itr2){
			if( (*itr1) == (*itr2)){
				addList.remove(*itr1);
				removeList.remove(*itr2);
				break;
			}
		}
		itr1 = itr1_next;
	}
	printf("remove size:%d\n", removeList.size());
	printf("add size:%d\n", addList.size());
	numModifications = 0;
	if(removeList.size()){
		numModifications++;
		emit requestModifyQunMembers(mQun->getQunID(), removeList, false);
	}
	if(addList.size()){
		numModifications++;
		emit requestModifyQunMembers(mQun->getQunID(), addList, true);
	}
}

void QunDetailsWindow::slotModifyQunMembers( const unsigned int id, const bool ok, QString msg)
{
	if(ok && id != mQun->getQunID()) return;
	QString title = i18n("Qun Modify Members");
	numModifications--;
	if(ok){
		if(numModifications > 0) return;
		QMessageBox::information(this, title, QString(i18n("Modify Qun members successfully.")));
		//slotClose( );
	} else {
		QMessageBox::information(this, title, msg);
	}
}

void QunDetailsWindow::slotReceivedQunCard( const unsigned int id, const bool ok, const unsigned int qq, QString realName, const unsigned char gender, 
						QString phone, QString email, QString memo, QString msg )
{	
	if(id != mQun->getQunID()) return;
	m_CardId = qq;
	leMyName->setText("");
	cbbMyGender->setCurrentItem(2);
	leMyPhone->setText("");
	leMyEmail->setText("");
	teMyMemo->setText("");
	
	if(ok){
		leMyName->setText(realName);
		leMyName->setReadOnly(true);
		cbbMyGender->setCurrentItem(gender);
		cbbMyGender->setEnabled(false);
		leMyPhone->setText(phone);
		leMyPhone->setReadOnly(true);
		leMyEmail->setText(email);
		leMyEmail->setReadOnly(true);
		teMyMemo->setText(memo);
		teMyMemo->setReadOnly(true);
	}else {
		leMyName->setText(QString::number(qq));
		leMyName->setReadOnly(true);
		teMyMemo->setText(msg);
		teMyMemo->setReadOnly(true);
	}
	
	tabWMain->setCurrentPage(3);
	pbUpdate->setEnabled(true);
}

void QunDetailsWindow::slotCategoryClicked( )
{
	QunCategoryPicker *picker = new QunCategoryPicker(this, "categoryPicker",WStyle_Customize | WStyle_Dialog |
										 WStyle_DialogBorder|WDestructiveClose);
	QObject::connect(picker, SIGNAL(selectCategoryCode(const unsigned short )), SLOT(slotCategorySelected(const unsigned short )));
	picker->show();
}

void QunDetailsWindow::slotCategorySelected( const unsigned short code)
{
	category = code;
	QunCategory qunCate;
	QString cateDescription = qunCate.getDescription(category);
	tbCategory->setText(cateDescription);
}

void QunDetailsWindow::slotMembersUpdated( const unsigned int id )
{
	if(id!= mQun->getQunID()) return;
	slotLoadMembers( );
}

void QunDetailsWindow::slotUpdateMessageSetting( )
{
	unsigned char type = Qun::Notify;
	if(rbRecord_Show->isChecked())
		type = Qun::Notify;
	else if(rbPopupWindow->isChecked())
		type = Qun::Popup;
	else if(rbDisplayNumbers->isChecked())
		type = Qun::Numbers;
	else if(rbRecord_only->isChecked())
		type = Qun::RecordOnly;
	else if(rbRejectMsgs->isChecked())
		type = Qun::Reject;
	
	emit requestUpdateQunMessageSettings(mQun->getQunID(), type);
	QMessageBox::information(this, i18n("Qun Setting"), QString(i18n("Message setting updated successfully.")));
}


#include "qundetailswindow.moc"

