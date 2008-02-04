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

#ifndef EVA_TABWIDGET_H
#define EVA_TABWIDGET_H

#include <qwidget.h>
#include <qstring.h>
#include <qpoint.h>
#include <qrect.h>
#include <qmap.h>
#include <qpixmap.h>
#include <qcolor.h>
#include <qwidgetstack.h>

class EvaTabToolTip;
class QMouseEvent;
class QWheelEvent;
class QPaintEvent;
class EvaTabWidget;

class EvaTabBar : public QWidget
{
    Q_OBJECT
public:
    EvaTabBar( QWidget *parent = 0, const char * name = 0, WFlags f = 0 );
    virtual ~EvaTabBar();

    // return the key of the just created tab
    int addTab(QString &name, QPixmap icon, QString &tip);
    bool removeTab(int key);

    void changeTabTo(int key);
    const int numTabs() const { return m_d.size(); }

    void setGridColor(const QColor &c) { m_gridColor = c; }
    void setSelectedColor(const QColor &c) { m_selectedColor = c; }
signals:
    void clicked(int key);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent( QMouseEvent *e);
    void mouseMoveEvent( QMouseEvent *e);
    void wheelEvent ( QWheelEvent * e );
    void paintEvent( QPaintEvent *e);
private:

    class TabData{
    public:
        TabData() : name(""), tip(""), icon(0L){}
        TabData(QString &n, QPixmap i, QString t = QString::null){
            name = n;
            tip = t;
            icon = i;
        }
        ~TabData(){
        }
        void setData(QString &n, QPixmap i, QString t = QString::null){
            name = n;
            tip = t;
            icon = i;
        }
        TabData operator=( const TabData &rhs){
            name = rhs.name;
            tip = rhs.tip;
            icon = rhs.icon;
            return *this;
        }
        QString name;
        QString tip;
        QPixmap icon;
    };
    int m_CurrentTab;
    int m_Counter;
    QMap<int, TabData> m_d;
    QMap<int, int> m_index; // key: seq, data: the key of TabData
    EvaTabToolTip *m_toolTip;

    QColor m_gridColor;
    QColor m_selectedColor;
    QPixmap m_Buffer;
    QPixmap m_BasePixmap;

    void updateBasePixmap();
    // return the vertical sequence based on the tab key. return -1 if key doesn't exist
    int getTabSequence(int key);
    // return the veritical tab sequence of the point(x,y)
    int getTabSequence(int x, int y);
    // return the key of seq-th tab in vertical order
    int getTabKey(int seq);
    int getTabKey(int x, int y);

    void update();
    QRect itemRect(const QPoint &p);
    QString tip(const QPoint &p);

    friend class EvaTabWidget;
    friend class EvaTabToolTip;
};



// class EvaTabWidget : public QWidget
// {
//     Q_OBJECT
// 
// };

class EvaWidgetStack : public QWidgetStack
{
    Q_OBJECT
    friend class EvaTabWidget;
public:
    EvaWidgetStack( QWidget *parent = 0, const char * name = 0, WFlags f = 0 );

protected:
    void paintEvent(QPaintEvent *e);

private:
    int m_CurrentSeq;
    QColor m_gridColor;
    QColor m_tabSelectedColor;

public slots:
    void raiseWidget(int id, int seq);

};


class QHBoxLayout;
class QGridLayout;

class EvaTabWidget : public QWidget
{
    Q_OBJECT

public:
    EvaTabWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EvaTabWidget();

    EvaTabBar* evaTabBar;
    EvaWidgetStack* wsView;
    //QWidget* WStackPage;

    int addTab(QString &name, QPixmap icon, QString &tip, QWidget *w);
		QWidget *getWidget(int id);
    bool removeTab(int key);
    void setGridColor(const QColor &c);
    void setSelectedColor(const QColor &c);
protected:
    QGridLayout* EvaTabWidgetLayout;
    QHBoxLayout* layout;

public slots:
    void changeTabTo(int key);

};

#endif

