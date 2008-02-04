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

#ifndef EVATIPWINDOW_H
#define EVATIPWINDOW_H

#include "evatipuibase.h"

class EvaImageResource;
class QTimer;

class EvaTipWindow : public EvaTipUIBase
{
	Q_OBJECT
public:
	EvaTipWindow(EvaImageResource *res, const QString nick, const unsigned int id, const short face, const QString &message);
	~EvaTipWindow();
signals:
	void requestChat(const unsigned int);
protected:
	//virtual void showEvent(QShowEvent *);
	//virtual void paintEvent( QPaintEvent *);
	virtual void mousePressEvent(QMouseEvent *e);
private:
	EvaImageResource *images;
	QPixmap *m_BgPixmap;
	unsigned int qqNum;
	int timelast;
	QTimer *timer;
	QRect scr;
private slots:
	void slotTimeout();
};

#endif

