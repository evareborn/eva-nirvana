 /***************************************************************************
 *   Copyright (C) 2006 by yunfan                                          *
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

#ifndef EVACUSTOMFACESELECTOR_H
#define EVACUSTOMFACESELECTOR_H

#include "customfaceuibase.h"
#include "customface.h"
#include <qframe.h>
#include <qmap.h>
#include <qptrlist.h>
#include "evautil.h"

#ifndef NUM_W
#define NUM_W         15
#define NUM_H           8
#define NUM_GRIDS     (NUM_W * NUM_H)
#endif

class CustomFacePanel : public QFrame
{
	Q_OBJECT

public:
	CustomFacePanel(int groupIndex, QString &group, FaceList &members, QWidget* parent = 0);
	virtual ~CustomFacePanel();
	int numPages() { return m_Surfaces.count();}
	int page() { return m_CurrPage; }
	void setPage(int index);
signals:
	void selectSysFace(int);
	void selectCustomFace(QString, QString); // group name, file name
private:
	QString m_groupName;
	int m_groupIndex;
	QPixmap *m_CurrSurface;
	int m_CurrPage;
	QPtrList<QPixmap> m_Surfaces;
	QMap<int, QString> files; // index, original file name
	int m_sysIndex[QQ_SMILEY_AMOUNT];

	void initSysPanel();
	/// return the last added smiley index
	int AddSysPanel(int iStartIndex = 0);
	void initCustomPanel(FaceList &members);
	virtual void paintEvent( QPaintEvent *e);
	virtual void mouseReleaseEvent( QMouseEvent * e);
};

class CustomFaceSelector : public CustomFaceUIBase
{
	Q_OBJECT

public:
    CustomFaceSelector( bool useSysFaceOnly = true, QWidget* parent = 0,
					const char* name = 0,
					WFlags fl = Qt::WStyle_Customize 
							| Qt::WStyle_NoBorder 
							| Qt::WType_Popup);
	~CustomFaceSelector();

signals:
	void selectSysFace(int);
	void selectCustomFace(const QString &); // file name without path. already in the PictureCache dir

	void addSmileyClicked();

private slots:
	void slotSelectSysFace(int id);
	void slotSelectCustomFace(QString, QString);

	void slotAddSmileyClicked();
	void slotUseShortcutClicked();
	void slotPrevClicked();
	void slotNextClicked();
	void slotPanelChanged(QWidget *);
private:
	bool m_UseSysFaceOnly;

	virtual void paintEvent( QPaintEvent *e);
	virtual void mouseReleaseEvent( QMouseEvent * e);
};

#endif // CUSTOMFACESELECTOR_H
