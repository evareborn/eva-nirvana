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

#include "evatabwidget.h"

#include <qpoint.h>
#include <qrect.h>
#include <qtooltip.h>
#include <qpainter.h>
#include <qsizepolicy.h>
#include <qimage.h>
#include <qmemarray.h>
#include <qevent.h>
#include <qlayout.h>

#define TAB_BAR_SIZE         26
#define TAB_ICON_SIZE        (TAB_BAR_SIZE - 4)
#define TAB_BAR_GRID_WIDTH   1
//#define TAB_GRID_COLOR      (QColor(0xDD, 0xE9, 0xF9))
#define TAB_GRID_COLOR      (QColor(0x2D, 0x7A, 0xFF))
#define TAB_LEFT_MARGIN      4


class EvaTabToolTip : public QToolTip
{
public:
	EvaTabToolTip(EvaTabBar *bar, QToolTipGroup *group = 0);
	virtual ~EvaTabToolTip();
protected:
	void maybeTip(const QPoint &p);
private:
	EvaTabBar *m_bar;
};

EvaTabToolTip::EvaTabToolTip(EvaTabBar *bar, QToolTipGroup *group)
	: QToolTip(bar, group),
	m_bar(bar)
{
	//setPalette( QPalette(Qt::yellow, QColor(20,190,255)));
}

EvaTabToolTip::~EvaTabToolTip()
{
}

void EvaTabToolTip::maybeTip(const QPoint &p)
{
    QRect rect = m_bar->itemRect(p);
    if(rect.width() && rect.height())
        tip(rect, m_bar->tip(p));
}


/// =========================================================


EvaTabBar::EvaTabBar(QWidget *parent, const char * name, WFlags f )
    : QWidget(parent, name, f),
    m_CurrentTab(0),
    m_Counter(0),
    m_gridColor(TAB_GRID_COLOR)
{
	m_selectedColor = colorGroup().light();
	//setBackgroundMode(QWidget::PaletteBase);
	setGeometry(QRect(0, 0, TAB_BAR_SIZE, 31256));
	setMinimumSize(TAB_BAR_SIZE, 0);
	setMaximumSize(TAB_BAR_SIZE, 31256);
	//     setSizePolicy( QSizePolicy( QSizePolicy::Fixed,
	//                                 QSizePolicy::Preferred,
	//                                 0, 0, false ));
	
	m_toolTip = new EvaTabToolTip(this);
}

EvaTabBar::~EvaTabBar()
{
    m_d.clear();
    delete m_toolTip;
    m_toolTip = 0;
}

// return the key of the just created tab
int EvaTabBar::addTab(QString &name, QPixmap icon, QString &tip)
{
    TabData data;
    if(icon.width() > TAB_ICON_SIZE ||
        icon.height() > TAB_ICON_SIZE){
        data.setData(name, icon.convertToImage().smoothScale(TAB_ICON_SIZE, TAB_ICON_SIZE), tip);
    } else
        data.setData(name, icon, tip);

    m_d[m_Counter++] = data;
		updateBasePixmap();
		QWidget::update();
    return (m_Counter - 1);
}

bool EvaTabBar::removeTab(int key)
{
    QMap<int, TabData>::Iterator itr = m_d.find(key);
    if(itr == m_d.end()) return false;

    m_d.erase(itr);
    updateBasePixmap();
		QWidget::update();
    return true;
}

void EvaTabBar::changeTabTo(int key)
{
    int seq = getTabSequence(key);
    if(seq == -1) return;
    bitBlt(&m_Buffer, 0,0, &m_BasePixmap, 0,0, m_BasePixmap.width(), m_BasePixmap.height());


    int x1,y1,x2,y2;
    x1 = TAB_BAR_GRID_WIDTH;
    y1 = (seq * TAB_BAR_SIZE) + TAB_BAR_GRID_WIDTH;
//     x2 = TAB_BAR_SIZE - TAB_BAR_GRID_WIDTH;
//     y2 = seq * TAB_BAR_SIZE + TAB_BAR_SIZE - TAB_BAR_GRID_WIDTH;

    QPainter p(&m_Buffer);
    p.setPen(m_selectedColor);
    p.setBrush(m_selectedColor);
    p.drawRect(x1, y1, TAB_BAR_SIZE, TAB_BAR_SIZE);

    p.setPen(QPen(m_gridColor, TAB_BAR_GRID_WIDTH));
    x1 = 0;
    y1 = (seq * TAB_BAR_SIZE);
    x2 = TAB_BAR_SIZE;
    y2 = y1;
    p.drawLine(x1, y1, x2, y2);

    p.drawLine(x1, y1 + TAB_BAR_SIZE, x2, y2 + TAB_BAR_SIZE);


    x2 = x1;
    y2 = seq * TAB_BAR_SIZE + TAB_BAR_SIZE;
    p.drawLine(x1, y1, x2, y2);

    x1 = (TAB_BAR_SIZE - m_d[key].icon.width()) / 2;
    y1 = y1 + (TAB_BAR_SIZE - m_d[key].icon.height()) / 2;
    p.drawPixmap(x1, y1, m_d[key].icon);

    m_CurrentTab = key;
    //repaint(QRect(itemRect(QPoint( TAB_BAR_SIZE/2, (y1+y2)/2))));
    repaint();
}

int EvaTabBar::getTabSequence(int key)
{
    QMap<int, int>::Iterator it;
    for(it = m_index.begin(); it != m_index.end(); ++it){
        if(it.data() == key)
            return it.key();
    }
    return -1;
}

int EvaTabBar::getTabSequence(int /*x*/, int y)
{
    return  (y / (TAB_BAR_SIZE));
}

int EvaTabBar::getTabKey(int seq)
{
    QMap<int, int>::Iterator it = m_index.find(seq);
    if(it == m_index.end()) return -1;
    return it.data();
}

int EvaTabBar::getTabKey(int x, int y)
{
    return m_index[getTabSequence(x, y)];
}

void EvaTabBar::wheelEvent ( QWheelEvent * e )
{
    e->accept();
    int offset = (e->delta()>0)?(-1):1;
    int seq = getTabSequence(m_CurrentTab);
    if(seq == -1) return;

    int next = getTabKey(seq + offset);
    if(next == -1 ) return;
    changeTabTo(next);
    emit clicked(next);
}

void EvaTabBar::mousePressEvent(QMouseEvent *e)
{
    e->ignore();
}

void EvaTabBar::mouseReleaseEvent( QMouseEvent *e)
{
    e->accept();
    unsigned int seq = getTabSequence(e->x(), e->y());
    if(seq >= m_d.size()) return;

    int key = getTabKey(seq);
    if(key == -1 ) return;
    changeTabTo( key );
    emit clicked(key);
}

void EvaTabBar::mouseMoveEvent( QMouseEvent *e)
{
    e->ignore();
}

void EvaTabBar::paintEvent( QPaintEvent *e)
{
///    QWidget::paintEvent(e);
    
//     int x = e->rect().x();
//     int y = e->rect().y();
//     int w = e->rect().width();
//     int h = e->rect().height();
// 
//     QPainter painter(this);
//     painter.drawPixmap(x, y, m_BasePixmap, x, y, w, h);

    QMemArray<QRect> rects = e->region().rects();
    for ( uint i = 0; i < rects.count(); i++ ) {
        QRect r = rects[(int)i];
        bitBlt( this, r.x(), r.y(), &m_Buffer, r.x(), r.y(), r.width(), r.height() );
    }
}

void EvaTabBar::updateBasePixmap()
{
    m_BasePixmap.resize(width(), height());
    m_BasePixmap.fill(colorGroup().background());
    QPainter p(&m_BasePixmap);
    int x1,y1,x2,y2;
    QMap<int, TabData>::Iterator iter;
    int i = 0;
    m_index.clear();
    for(iter = m_d.begin(); iter != m_d.end(); ++iter){
        m_index[i] = iter.key();

        x1 = TAB_LEFT_MARGIN;
        y1 = i * TAB_BAR_SIZE;
        x2 = TAB_BAR_SIZE - TAB_LEFT_MARGIN;
        y2 = y1;
        p.setPen(QPen(m_gridColor.light(), TAB_BAR_GRID_WIDTH));
        p.drawLine(x1, y1, x2, y2);
        p.setPen(QPen(QColor(255,255,255), TAB_BAR_GRID_WIDTH));
        p.drawLine(x1 , y1+1, x2, y2+1);

        x1 = (TAB_BAR_SIZE - iter.data().icon.width()) / 2;
        y1 = y1 + (TAB_BAR_SIZE - iter.data().icon.height()) / 2;
        p.drawPixmap(x1, y1, iter.data().icon);

        i++;
    }
    m_Buffer.resize(m_BasePixmap.width(), m_BasePixmap.height());
    bitBlt(&m_Buffer, 0,0, &m_BasePixmap, 0,0, m_BasePixmap.width(), m_BasePixmap.height());
}

QRect EvaTabBar::itemRect(const QPoint &p)
{
    uint seq = getTabSequence(p.x(), p.y());
    if(seq >= m_d.size()) return QRect(0,0,0,0);

    return QRect( 0, seq * TAB_BAR_SIZE, TAB_BAR_SIZE, TAB_BAR_SIZE);
}

QString EvaTabBar::tip(const QPoint &p)
{
    uint seq = getTabSequence(p.x(), p.y());
    if(seq >= m_d.size()) return QString::null;

    return m_d[getTabKey(p.x(), p.y())].tip;
}



/// =========================================================


EvaWidgetStack::EvaWidgetStack(QWidget *parent, const char * name, WFlags f )
    : QWidgetStack(parent, name, f),
    m_CurrentSeq(0)
{
    m_gridColor = TAB_GRID_COLOR;
    m_tabSelectedColor = colorGroup().light();

    setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred));
    setBackgroundMode(QWidget::PaletteBase);
    setFrameShape( QFrame::Box );
}

 void EvaWidgetStack::raiseWidget(int id, int seq)
{
    m_CurrentSeq = seq;
    QWidgetStack::raiseWidget(id);
    repaint();
}

void EvaWidgetStack::paintEvent( QPaintEvent */*e*/)
{
    //QWidgetStack::paintEvent(e);
    QPainter p(this);
    p.setPen(QPen(m_gridColor, TAB_BAR_GRID_WIDTH));
    p.drawRect(0,0, width(), height());

    p.setPen(m_tabSelectedColor);

    int y1 = m_CurrentSeq * TAB_BAR_SIZE;
    int y2 = y1 + TAB_BAR_SIZE;
    p.drawLine(0, y1+1, 0, y2-1);
}




/// =========================================================


EvaTabWidget::EvaTabWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "EvaTabWidget" );
    EvaTabWidgetLayout = new QGridLayout( this, 1, 1, 3, 6, "EvaTabWidgetLayout");

    layout = new QHBoxLayout( 0, 0, 0, "layout2"); 

    evaTabBar = new EvaTabBar( this, "EvaTabBar" );
    evaTabBar->m_gridColor = TAB_GRID_COLOR;
    evaTabBar->m_selectedColor = colorGroup().base();
//     evaTabBar->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5,
//                                         (QSizePolicy::SizeType)5, 0, 0, 
//                                         evaTabBar->sizePolicy().hasHeightForWidth() ) );
//     evaTabBar->setFrameShape( QFrame::StyledPanel );
//     evaTabBar->setFrameShadow( QFrame::Raised );
    layout->addWidget( evaTabBar );

    wsView = new EvaWidgetStack( this, "wsView" );
    wsView->m_gridColor = TAB_GRID_COLOR;
    wsView->m_tabSelectedColor  = colorGroup().background();

    //WStackPage = new QWidget( wsView, "WStackPage" );
    //wsView->addWidget( WStackPage, 0 );
    layout->addWidget( wsView );

    EvaTabWidgetLayout->addLayout( layout, 0, 0 );
    resize( QSize(350, 233).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    QObject::connect(evaTabBar, SIGNAL(clicked(int)), SLOT(changeTabTo(int)));
}


EvaTabWidget::~EvaTabWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

int EvaTabWidget::addTab(QString &name, QPixmap icon, QString &tip, QWidget *w)
{
    int key = evaTabBar->addTab(name, icon, tip);
    wsView->addWidget(w, key);
    return key;
}

QWidget *EvaTabWidget::getWidget(int id)
{
	return wsView->widget(id);
}

bool EvaTabWidget::removeTab(int key)
{
    if( !evaTabBar->removeTab(key))
        return false;

    QWidget *w = wsView->widget(key);
    if(!w) return false;

    wsView->removeWidget(w);
    delete w;
    return true;
}

void EvaTabWidget::changeTabTo(int key)
{
    evaTabBar->changeTabTo(key);
    wsView->raiseWidget( key, evaTabBar->getTabSequence(key));
}

void EvaTabWidget::setGridColor(const QColor &c)
{
    evaTabBar->m_gridColor = c;
    evaTabBar->updateBasePixmap();
    wsView->m_gridColor = c;
    repaint();
}

void EvaTabWidget::setSelectedColor(const QColor &c)
{
    evaTabBar->m_selectedColor = c;
    wsView->m_tabSelectedColor = c;
    repaint();
}



