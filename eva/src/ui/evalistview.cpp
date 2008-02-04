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

#include "evalistview.h"
#include "evamain.h"
#include "evaresource.h"
#include <qheader.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qtimer.h>
#include <qdragobject.h>
#include <qtextcodec.h>
#include <qpoint.h>
#include <qrect.h>
#include <qtooltip.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qstyle.h>
#include <qsimplerichtext.h>
//#include <qapplication.h>
#include <klocale.h>

#define EVA_LV_LEFT_MARGIN     6


EvaLVToolTip::EvaLVToolTip(EvaListView *lv, QToolTipGroup *group)
	: QToolTip(static_cast<QWidget *>(lv->viewport()), group),
	m_lv(lv)
{
	//setPalette( QPalette(Qt::yellow, QColor(20,190,255)));
}

EvaLVToolTip::~EvaLVToolTip()
{
}

void EvaLVToolTip::maybeTip(const QPoint &p)
{
	EvaListViewItem *item = dynamic_cast<EvaListViewItem *>(m_lv->itemAt(p));
	if(!item) return;

	int section = m_lv->header()->sectionAt(p.x());
	QRect itemRect = m_lv->itemRect(item);
	QRect headerRect = m_lv->header()->sectionRect(section);
	QRect destRect(headerRect.left(), itemRect.top(), headerRect.width(), itemRect.height());
	
        tip(destRect, item->tip());
	
}

/// ==============================================

EvaListViewItem::EvaListViewItem( QListViewItem *parent, QString label, QPixmap *p)
		: QListViewItem( parent, label),
                m_richText(0)
{
	codec = QTextCodec::codecForName("GB18030");
	m_icon = 0;
	if(p)
	m_icon = new QPixmap(*p);
	setDragEnabled(true);
	setText(label);
}

EvaListViewItem::EvaListViewItem( QListView *parent)
		: QListViewItem( parent),
                m_richText(0)
{
	codec = QTextCodec::codecForName("GB18030");
	m_icon = 0;
	setDragEnabled(false);
}

EvaListViewItem::~EvaListViewItem()
{
    if(m_richText) delete m_richText;
    if(m_icon) delete m_icon;
}

void EvaListViewItem::updateIcon(QPixmap *p)
{
    if(!p) return;
    if(m_icon) delete m_icon;
    m_icon = new QPixmap(*p);
}

void EvaListViewItem::setText(const QString &text)
{
    if(!listView()) return;
    if(m_richText) delete m_richText;
    QListViewItem::setText(0, text);
    m_richText = new QSimpleRichText( "<qt><nobr>" + text + "</nobr></qt>",
                                listView()->viewport()->font(), QString::null, 0/*, mimeFactory_*/ );
}

QString EvaListViewItem::tip()
{
    return text(0);
}

void EvaListViewItem::paintBranches( QPainter * p, const QColorGroup & cg, int w, int /*y*/, int h )
{
	QListView *lv = listView();
	lv->style().drawComplexControl( QStyle::CC_ListView,
			p, lv, QRect( 0, 0, w, h ), cg,
			QStyle::Style_Enabled | QStyle::Style_Default, QStyle::SC_ListView, QStyle::SC_None,
			QStyleOption() );
}

void EvaListViewItem::paintCell( QPainter * painter, const QColorGroup & colourGroup, int column, int width, int align )
{
    if( ! isVisible() ) return;
    if ( column == 0){
        QBrush *brush;
        QPalette palette;

        EvaListView *lv = dynamic_cast<EvaListView *> (listView());
        if( !lv ) return;

        palette = lv->viewport()->palette();
        brush   = 0;

        const BackgroundMode bgmode = lv->viewport()->backgroundMode();
        const QColorGroup::ColorRole crole = QPalette::backgroundRoleFromMode( bgmode );
        if ( colourGroup.brush( crole ) != lv->colorGroup().brush( crole ) )
            painter->fillRect( 0, 0, width, height(), colourGroup.brush( crole ) );
        else
            lv->paintEmptyArea( painter, QRect( 0, 0, width, height() ) );

        if ( isSelected() ) {
            brush = new QBrush( palette.color(QPalette::Active, QColorGroup::Highlight) );
            // painter->setPen( palette.color(QPalette::Active, QColorGroup::Text) );
        } else {
            // painter->setPen( palette.color(QPalette::Normal, QColorGroup::Text) );
            }

        int icon_width = 0;
        if(m_icon){
            icon_width = lv->itemMargin() + m_icon->width();
        }
        // TODO: Change the font for highlighted text
        m_richText->draw( painter, lv->itemMargin() + icon_width, 0, 
                                QRect( icon_width, 0, width, height() ), colourGroup, brush );
        //setHeight( m_RichText->height() );
        if(m_icon){
            int xo = lv->itemMargin();
            int yo = ( height() - m_icon->height() ) / 2;
            painter->drawPixmap( xo, yo, *m_icon );
        }
        int height = m_richText->height();
        if(m_icon && m_icon->height() > m_richText->height()){
            height = m_icon->height();
        }
        height += 4;
        setHeight( height );
        widthChanged( 0 );
        delete brush;
    } else {
        QListViewItem::paintCell( painter, colourGroup, column, width, align );
    }
}


/// =================================================




EvaListView::EvaListView(QWidget *parent, const char *name, WFlags f)
	: QListView(parent, name, f)
{
	setItemMargin(EVA_LV_LEFT_MARGIN);
	addColumn("1");
	setColumnWidthMode( 0, Manual);
// 	addColumn("2");
// 	setColumnWidthMode( 1, Manual);
	header()->hide();
	setSorting(0);
	setRootIsDecorated( false );
	setTreeStepSize( 8 );
	setDragAutoScroll(true);
	setHScrollBarMode(QScrollView::AlwaysOff);
	setFrameShape( QFrame::NoFrame );
	
	setAcceptDrops( true );
	setSelectionMode( QListView::Extended );
	setSelectionMode(QListView::Single);
	m_tooltip = new EvaLVToolTip(this);
	
	clearWFlags( WStaticContents );
	setWFlags( WNoAutoErase );
	
	static_cast<EvaListView*>(viewport())->clearWFlags( WStaticContents );
	static_cast<EvaListView*>(viewport())->setWFlags( WNoAutoErase );
	
	codec = QTextCodec::codecForName("GB18030");

}

EvaListView::~EvaListView()
{
	delete m_tooltip;
	m_tooltip = 0;
}
