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

#ifndef EVAADDQUNWINDOW_H
#define  EVAADDQUNWINDOW_H

#include "evaaddqunuibase.h"

#include "evaqun.h"

#include <qpixmap.h>

class QCloseEvent;

class EvaAddingQunWindow : public EvaAddQunUIBase
{
	Q_OBJECT
public:
	EvaAddingQunWindow( );

	void setQun(const QunInfo &qun);

	void AddingRejected();
	void AddingNeedAuth();
	void updateGraphic(QPixmap &p);
	void AddingNoAuthReady();

signals:
	void refreshGraphic();
	void requestDetails(const unsigned int id);
	void requestQunDetails(const unsigned int id);
	void requestAdd();
	void requestCancel();

private:
	QunInfo m_Qun;

	void initQun();
	void initWindow();
private slots:
	void slotQunDetailsClicked();
	void slotCreatorDetailsClicked();
	void slotSendClicked();

	virtual void closeEvent(QCloseEvent *e);
};

#endif


