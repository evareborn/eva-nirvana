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

#include "evaaddingwindow.h"

#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qiconset.h>
//#include <qtextedit.h>
#include <qtextcodec.h>
#include <qevent.h>

#include <kmessagebox.h>
#include <klocale.h>

#include <list>

#include "../evamain.h"
#include "evaresource.h"
#include "evauser.h"


EvaAddingWindow::EvaAddingWindow()
	: m_ID(0),
	m_Nick(""),
	m_Face(0),
	m_IsSelectGroupOnly(false)
{
	initWindow();
	QObject::connect(tbtnFace, SIGNAL(clicked()), SLOT(slotFaceClicked( )));
	QObject::connect(btnRefreshCode, SIGNAL(clicked()), SIGNAL(refreshGraphic()));
	QObject::connect(btnOk, SIGNAL(clicked()), SLOT(slotOkClicked()));
	QObject::connect(btnCancel, SIGNAL(clicked()), SLOT(close()));

}

void EvaAddingWindow::initBuddy( )
{
	if(m_IsSelectGroupOnly)
		lblTip->setText("<b>" + QString(i18n("Select a group for %1")).arg(m_ID) + "</b>");
	else
		lblTip->setText("<b>" + i18n("Retrieving information from server...") + "</b>");

	lblTopic->setText(QString(i18n("You are about to add ")) + m_Nick + QString("(%1)").arg(m_ID) );
	QPixmap *facePic = EvaMain::images->getFaceByID( m_Face);
	QIconSet faceIcon;
	faceIcon.setPixmap(*facePic,QIconSet::Large);
	tbtnFace->setIconSet(faceIcon);

	// load users groups
	std::list<std::string> names = EvaMain::user->getGroupNames();
	std::list<std::string>::iterator iter;
	QTextCodec *codec = QTextCodec::codecForName( "GB18030");
	cbbGroups->clear();
	for(iter = names.begin(); iter!= names.end(); ++iter){
		QString g = codec->toUnicode(iter->c_str());
		cbbGroups->insertItem(g);
	}
	cbbGroups->setCurrentItem( 0);

	fraVeriGraphic->setHidden(true);
	fraQuestion->setHidden( true);
	leMessage->setHidden( true);
	adjustSize();

	if(!m_IsSelectGroupOnly)
		btnOk->setEnabled(false);
}

void EvaAddingWindow::setBuddy( const unsigned int id, const QString & nick, const unsigned short face, const bool selectGroup )
{	
	m_ID = id;
	m_Nick = nick;
	m_Face = face;
	m_IsSelectGroupOnly = selectGroup;
	initBuddy( );
}

void EvaAddingWindow::AddingRejected( )
{
	lblTip->setText("<b>" + m_Nick + i18n(" refuses to be added.") + "</b>");
	fraVeriGraphic->setHidden(true);
	fraQuestion->setHidden( true);
	leMessage->setHidden( true);

	btnOk->setEnabled(true);
	btnCancel->setEnabled( true);
}

void EvaAddingWindow::AddingNeedAuth( )
{
	lblTip->setText("");
	btnOk->setEnabled(true);
	btnCancel->setEnabled( true);
	leMessage->setHidden( false);
	//adjustSize();
}

void EvaAddingWindow::updateGraphic( QPixmap & p )
{
	lblCodeGraphic->setPixmap( p);
	fraVeriGraphic->setHidden(false);
}

void EvaAddingWindow::setQuestion( const QString & q )
{
	lblTip->setText("");
	lblQuesContents->setText(q);
	fraQuestion->setHidden( false );
	leMessage->setHidden(true);
	btnOk->setEnabled(true);
	//adjustSize();
}

void EvaAddingWindow::slotFaceClicked( )
{
	emit requestDetails(m_ID);
}

void EvaAddingWindow::slotOkClicked( )
{
	if(m_IsSelectGroupOnly){
		emit groupSelected(cbbGroups->currentItem());
		return;
	}
		
	if( !fraVeriGraphic->isHidden() ){
		if(leCode->text().isEmpty()){
			KMessageBox::information(0, i18n("Enter the right verification code please."),
				i18n("Eva Search/Add Friend"));
			leCode->setFocus();
			return;
		}
	}

	//fraQuestion->setHidden( true);
	if( !fraQuestion->isHidden() ){
		if(leAnswer->text().isEmpty()){
			KMessageBox::information(0, i18n("Enter the answer please."),
				i18n("Eva Search/Add Friend"));
			leAnswer->setFocus();
			return;
		}
	}
	//cbbGroups->currentItem(), leMessage->text(), leCode->text(), leAnswer->text(), chbReverse->isChecked()
	emit requestAdd();
}

void EvaAddingWindow::AddingNoAuthReady( )
{
	//fraVeriGraphic->setHidden(true);
	fraQuestion->setHidden( true);
	leMessage->setHidden( true);
	btnOk->setEnabled(true);
}

void EvaAddingWindow::initWindow( )
{
	lblTip->setText("");
	lblTopic->setText("");
	cbbGroups->setCurrentItem( 0 );
	lblCodeGraphic->setPixmap( QPixmap());
	leCode->setText("");
	lblQuesContents->setText("");
	leAnswer->setText( "");
	leMessage->setText("");
	//QWidget::close();
}

void EvaAddingWindow::closeEvent( QCloseEvent * e )
{
	emit requestCancel();
	initWindow();
	e->accept();
}
