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
#include "evaguimain.h"
#include "defines.h"
#include "evaresource.h"
#include <q3header.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qtimer.h>
#include <q3dragobject.h>
#include <qtextcodec.h>
#include <qpoint.h>
#include <qrect.h>
#include <qtooltip.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qstyle.h>
#include <q3simplerichtext.h>
//Added by qt3to4:
#include <QStyleOption>
#include <Q3Frame>
//#include <qapplication.h>
//X #include <klocale.h>

#define EVA_LV_LEFT_MARGIN     6

//X 
//X EvaLVToolTip::EvaLVToolTip(EvaListView *lv, QToolTipGroup *group)
//X //X 	: QToolTip(static_cast<QWidget *>(lv->viewport()), group),
//X 	: m_lv(lv)
//X {
//X 	//setPalette( QPalette(Qt::yellow, QColor(20,190,255)));
//X }
//X 
//X EvaLVToolTip::~EvaLVToolTip()
//X {
//X }
//X 
//X void EvaLVToolTip::maybeTip(const QPoint &p)
//X {
//X 	EvaListViewItem *item = dynamic_cast<EvaListViewItem *>(m_lv->itemAt(p));
//X 	if(!item) return;
//X 
//X 	int section = m_lv->header()->sectionAt(p.x());
//X 	QRect itemRect = m_lv->itemRect(item);
//X 	QRect headerRect = m_lv->header()->sectionRect(section);
//X 	QRect destRect(headerRect.left(), itemRect.top(), headerRect.width(), itemRect.height());
//X 	
//X         tip(destRect, item->tip());
//X 	
//X }
//X 
/// ==============================================

EvaListViewItem::EvaListViewItem( Q3ListViewItem *parent, QString label, QPixmap *p)
		: Q3ListViewItem( parent, label),
                m_richText(0)
{
	codec = QTextCodec::codecForName("GB18030");
	m_icon = 0;
	if(p)
	m_icon = new QPixmap(*p);
	setDragEnabled(true);
	setText(label);
}

EvaListViewItem::EvaListViewItem( Q3ListView *parent)
		: Q3ListViewItem( parent),
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
    Q3ListViewItem::setText(0, text);
    m_richText = new Q3SimpleRichText( "<qt><nobr>" + text + "</nobr></qt>",
                                listView()->viewport()->font(), QString::null, 0/*, mimeFactory_*/ );
}

QString EvaListViewItem::tip()
{
    return text(0);
}

//X void EvaListViewItem::paintBranches( QPainter * /*p*/, const QColorGroup & /*cg*/, int /*w*/, int /*y*/, int /*h*/ )
//X {
//X //X 	Q3ListView *lv = listView();
//X //X 	lv->style()->drawComplexControl( QStyle::CC_Q3ListView,
//X //X 			p, lv, QRect( 0, 0, w, h ), cg,
//X //X 			QStyle::State_Enabled | QStyle::State_Default, QStyle::SC_Q3ListView, QStyle::SC_None,
//X //X 			QStyleOption() );
//X }
//X 
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

//X         const Qt::BackgroundMode bgmode = lv->viewport()->backgroundMode();
//X         const QColorGroup::ColorRole crole = QPalette::backgroundRoleFromMode( bgmode );
        const QColorGroup::ColorRole crole = QPalette::Window;
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
        Q3ListViewItem::paintCell( painter, colourGroup, column, width, align );
    }
}


/// =================================================




EvaListView::EvaListView(QWidget *parent, const char *name, Qt::WFlags f)
	: Q3ListView(parent, name, f)
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
	setHScrollBarMode(Q3ScrollView::AlwaysOff);
	setFrameShape( Q3Frame::NoFrame );
	
	setAcceptDrops( true );
	setSelectionMode( Q3ListView::Extended );
	setSelectionMode(Q3ListView::Single);
//X 	m_tooltip = new EvaLVToolTip(this);
	
//X 	clearWFlags( WStaticContents );
//X 	setWFlags( WNoAutoErase );
	
//X 	static_cast<EvaListView*>(viewport())->clearWFlags( WStaticContents );
//X 	static_cast<EvaListView*>(viewport())->setWFlags( WNoAutoErase );
	
	codec = QTextCodec::codecForName("GB18030");

}

EvaListView::~EvaListView()
{
//X 	delete m_tooltip;
//X 	m_tooltip = 0;
}
