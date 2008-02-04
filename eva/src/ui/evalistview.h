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

#ifndef EVALISTVIEW_H
#define EVALISTVIEW_H

#include <qlistview.h>
#include <qvaluestack.h>
#include <qtooltip.h>
#include <qcolor.h>
#include <qsize.h>
#include <string>
#include <list>
#include <map>

class QTimer;
class QPixmap;
class EvaListView;
class QCustomEvent;
class QPainter;
class QColorGroup;
class QSimpleRichText;
class QTextCodec;


class EvaLVToolTip : public QToolTip
{
public:
    EvaLVToolTip(EvaListView *lv, QToolTipGroup *group = 0);
    virtual ~EvaLVToolTip();
protected:
    void maybeTip(const QPoint &p);
private:
    EvaListView *m_lv;
};

// list view item types
enum LVIType { E_LVIBuddy, E_LVIGroup, E_LVIQun};

class EvaListViewItem :  public QListViewItem
{
public:
	EvaListViewItem( QListViewItem *parent, QString label, QPixmap *p = 0);
	EvaListViewItem( QListView *parent);
	~EvaListViewItem();
	
	virtual void updateIcon(QPixmap *p);
	// can be rich text
	void setText(const QString &text);
	
	virtual QString tip();
	
	virtual void paintCell( QPainter * painter, const QColorGroup & colourGroup, int column, int width, int align );
	virtual void paintBranches(QPainter * p, const QColorGroup & cg,int w, int y, int h);
	const LVIType type() const { return m_type; }

protected:
	LVIType m_type;
	QTextCodec *codec;
	QPixmap *m_icon;
private:
	QSimpleRichText *m_richText;
	
	friend class EvaListView;
};

class EvaListView : public QListView 
{
    Q_OBJECT
public:
    EvaListView( QWidget *parent=0, const char *name =0, WFlags f= 0);
    ~EvaListView();

protected:	
    QTextCodec *codec;

private:
    EvaLVToolTip *m_tooltip;

    friend class EvaListViewItem;
};

#endif

