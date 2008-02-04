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

#ifndef EVALOGINVERIWINDOW_H
#define EVALOGINVERIWINDOW_H

#include "evaloginveriuibase.h"

class EvaLoginVeriWindow : public EvaLoginVeriUIBase
{
	Q_OBJECT
public:
	EvaLoginVeriWindow(QWidget *p = 0);
	
signals:
	void changeImage();
	void sendVerifyCode( const QString &code );
public slots:
	void slotImageReady();
	void slotVerifyPassed();
private slots:
	void slotBtnChangeImageClicked();
	void slotBtnOKClicked();
};

#endif // EVALOGINVERIWINDOW_H
 
