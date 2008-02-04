/***************************************************************************
 *   Copyright (C) 2004 by yunfan                                          *
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


#include "evatipwindow.h"
#include "evaresource.h"
#include "evahtmlparser.h"
#include <qlabel.h>
#include <klocale.h>
#include <kapplication.h>
#include <qtimer.h>
#include <qpixmap.h>
#include <qimage.h>

EvaTipWindow::EvaTipWindow(EvaImageResource *res, const QString nick, const unsigned int id, const short face, const QString &message)
    : EvaTipUIBase(0, 0, WType_TopLevel | WStyle_Customize | WStyle_NoBorder | WStyle_StaysOnTop | WX11BypassWM | WNoAutoErase | WDestructiveClose)
{
	//m_BgPixmap = 0;
	//setBackgroundMode( NoBackground );



	images = res;
	qqNum = id;
	EvaHtmlParser parser;
	parser.setAbsImagePath(images->getSmileyPath());

	QString htmlNick = nick;
	parser.convertToHtml( htmlNick, false, true);
	lblNick->setText("<qt>" +htmlNick + "</qt>");
	
	QString msg = message;

	if(msg.stripWhiteSpace().length() > 30 ){
		msg = message.left(27) + i18n("...");
	}

	parser.convertToHtml(msg, false, true);
	lblMessage->setText("<qt>" +msg+"</qt>");
	
	QPixmap *pic = images->getQQShow(qqNum);
	lblPixmap->setScaledContents(true);
	if(!pic){
		pic = images->getUserHeadPixmap(qqNum);
		if(!pic)
			pic = images->getFace(images->getFaceFileIndex(face));
		lblPixmap->setScaledContents(false);
	}
	
	lblPixmap->setPixmap(*pic);
	scr = KApplication::desktop()->screenGeometry();
	setGeometry(scr.right()- width(), scr.bottom(), width(), height());
		
	timelast=0;
	timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this,SLOT(slotTimeout()));
	timer->start(70, false);
	//show();
}

EvaTipWindow::~EvaTipWindow()
{
	//if(m_BgPixmap) delete m_BgPixmap;
}

void EvaTipWindow::mousePressEvent(QMouseEvent *e)
{
	if(timer->isActive())
		timer->stop();
	if( (e->button() & LeftButton)==LeftButton){
		emit requestChat(qqNum);
	}
	close();
}

void EvaTipWindow::slotTimeout()
{
	if(timelast > 100) {
		timer->stop();
		close();
	}else
		timelast+=2;
	if(y() > (scr.height()- height()-35) ){
		move(scr.right()- width(), scr.height()-(height()/6)*timelast);
	};
}
/*
void EvaTipWindow::showEvent( QShowEvent * )
{
	// set background transparency
	if(m_BgPixmap) delete m_BgPixmap;
	m_BgPixmap = new QPixmap(width(), height());
	QColor bColor(Qt::white);
	QRgb col = bColor.rgb( );

	Q_UINT8 salpha = 192;
	Q_UINT8 dalpha = 255 - salpha;

	int a, r, g, b;
	a = 0;
	r = QMIN( (qRed   (col) * salpha) / 255 + (qRed   (col) * dalpha) / 255, 255 );
	g = QMIN( (qGreen (col) * salpha) / 255 + (qGreen (col) * dalpha) / 255, 255 );
	b = QMIN( (qBlue  (col) * salpha) / 255 + (qBlue  (col) * dalpha) / 255, 255 );

	col = a << 24 | r << 16 | g << 8 | b;
	int pixel = a << 24 | (r * a / 255) << 16 | (g * a / 255) << 8 | (b * a / 255);

	m_BgPixmap->fill(QColor(col, pixel));

	//this->setBackgroundPixmap( *bg);
	//this->setPaletteBackgroundPixmap( *bg);
}

void EvaTipWindow::paintEvent( QPaintEvent * )
{
	//bitBlt(this, 0, 0, m_BgPixmap);
	setPaletteBackgroundPixmap(*m_BgPixmap);
}
*/

