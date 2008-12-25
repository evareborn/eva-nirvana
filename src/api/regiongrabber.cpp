/*
  Copyright (C) 2003 Nadeem Hasan <nhasan@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or ( at your option ) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#include "regiongrabber.h"

#include <qpainter.h>
#include <qpalette.h>
#include <qstyle.h>
#include <qtimer.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <Q3Painter>
#include <QPixmap>
#include <QMouseEvent>
#include <QLabel>
#include <Q3Frame>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QApplication>
 
#include "defines.h"

//X #include <kglobalsettings.h>
//X #include <klocale.h>

SizeTip::SizeTip( QWidget *parent, const char *name )
    : QLabel( parent, name, Qt::WStyle_Customize | Qt::WX11BypassWM |
      Qt::WStyle_StaysOnTop | Qt::WStyle_NoBorder | Qt::WStyle_Tool )
{
  setMargin( 10 );
  setIndent( 5 );
  setFrameStyle( Q3Frame::Plain | Q3Frame::Box );

  //setPalette( QToolTip::palette() );
  setPalette( QPalette(Qt::yellow, QColor(20,190,255)));
//X   QRect deskR = KGlobalSettings::desktopGeometry( QPoint( 0, 0 ) );
//X   move(deskR.topLeft());
}

void SizeTip::setTip( const QRect &rect )
{
  QString des = "<font color=red>" + QString( i18n("Press mouse left button<br>to grab screenshot.") ) + "</font><p>" + i18n("Size (W x H):");
  QString tip = "<qt>" + des + QString("<b><font color=yellow>%1</font></b> x <b><font color=yellow>%2</font></b>").arg( rect.width() )
      .arg( rect.height() ) + "</qt>";

  setText( tip );
  adjustSize();

  positionTip( rect );
}

void SizeTip::positionTip( const QRect &/*rect*/ )
{
  QRect tipRect = geometry();
  tipRect.moveTopLeft( QPoint( 0, 0 ) );

//X   if ( rect.intersects( tipRect ) )
//X   {
//X     QRect deskR = KGlobalSettings::desktopGeometry( QPoint( 0, 0 ) );
//X 
//X     tipRect.moveCenter( QPoint( deskR.width()/2, deskR.height()/2 ) );
//X     if ( !rect.contains( tipRect, true ) && rect.intersects( tipRect ) )
//X       tipRect.moveBottomRight( geometry().bottomRight() );
//X   }

  move( tipRect.topLeft() );
}

RegionGrabber::RegionGrabber()
  : QWidget( 0, 0 ),
    mouseDown( false ), sizeTip( 0L )
{
  sizeTip = new SizeTip( ( QWidget * )0L );

//   tipTimer = new QTimer( this );
//   connect( tipTimer, SIGNAL( timeout() ), SLOT( updateSizeTip() ) );

  QTimer::singleShot( 200, this, SLOT( initGrabber() ) );
}

RegionGrabber::~RegionGrabber()
{
  delete sizeTip;
}

void RegionGrabber::initGrabber()
{
//X   pixmap = QPixmap::grabWindow( qt_xrootwin() );
  pixmap = QPixmap::grabWindow( QApplication::desktop()->winId() );
  setPaletteBackgroundPixmap( pixmap );

  showFullScreen();

//X   grabMouse( crossCursor );
  sizeTip->setTip(QRect(0,0,0,0));
  sizeTip->show();
}

void RegionGrabber::mousePressEvent( QMouseEvent *e )
{
  if ( e->button() == Qt::LeftButton )
  {
    mouseDown = true;
    grabRect = QRect( e->pos(), e->pos() );
  }
}

void RegionGrabber::mouseMoveEvent( QMouseEvent *e )
{
  if ( mouseDown )
  {    
    drawRubber();
    grabRect.setBottomRight( e->pos() );
    drawRubber();
    updateSizeTip();
  }
}

void RegionGrabber::mouseReleaseEvent( QMouseEvent *e )
{
  mouseDown = false;
  drawRubber();
  sizeTip->hide();

  grabRect.setBottomRight( e->pos() );
  grabRect = grabRect.normalize();

  QPixmap region = QPixmap::grabWindow( winId(), grabRect.x(), grabRect.y(),
      grabRect.width(), grabRect.height() );

  releaseMouse();

  emit regionGrabbed( region );
}

void RegionGrabber::keyPressEvent( QKeyEvent *e )
{
  if ( e->key() == Qt::Key_Escape )
  {
    releaseMouse();
    emit regionGrabbed( QPixmap() );
  }
  else
    e->ignore();
}

void RegionGrabber::updateSizeTip()
{
  QRect rect = grabRect.normalize();

  sizeTip->setTip( rect );
  //sizeTip->show();
}

void RegionGrabber::drawRubber()
{
//X   Q3Painter p;
//X   p.begin( this );
//X //X   p.setRasterO( NotROP );
//X //X   p.setPen( QPen( color0, 1 ) );
//X   p.setBrush( Qt::NoBrush );
//X 
//X   style()->drawPrimitive( QStyle::PE_FrameFocusRect, &p, grabRect, colorGroup(),
//X       QStyle::Style_Default, QStyleOption( colorGroup().base() ) );
//X 
//X   p.end();
}

//X #include "regiongrabber.moc"
