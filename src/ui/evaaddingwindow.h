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
 
#ifndef EVAADDINGWINDOW_H
#define EVAADDINGWINDOW_H

#include "evaadduibase.h"
#include <qpixmap.h>

class QCloseEvent;

class EvaAddingWindow : public EvaAddUIBase
{
	Q_OBJECT
public:
	EvaAddingWindow( );

	void setBuddy(const unsigned int id, const QString &nick, const unsigned short face, const bool selectGroup = false);

	void AddingRejected();
	void AddingNeedAuth();
	void updateGraphic(QPixmap &p);
	void setQuestion(const QString &q);
	void AddingNoAuthReady();

signals:
	void refreshGraphic();
	void requestDetails(const unsigned int id);
	void requestAdd();
	void requestCancel();

	void groupSelected(const int index);
private:
	unsigned int m_ID;
	QString m_Nick;
	unsigned short m_Face;
	bool m_IsSelectGroupOnly;

	void initBuddy();
	void initWindow();
private slots:
	void slotFaceClicked();
	void slotOkClicked();

	virtual void closeEvent(QCloseEvent *e);
};

#endif

