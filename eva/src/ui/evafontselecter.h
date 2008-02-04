/***************************************************************************
 *   Copyright (C) 2005 by yunfan                                          *
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
  
#ifndef EVAFONTSELECTER_H
#define EVAFONTSELECTER_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class KColorCombo;
class KIntSpinBox;
class QPushButton;
class QFrame;

class EvaFontSelecter : public QWidget
{
	Q_OBJECT
public:
	EvaFontSelecter( QWidget* parent = 0, const char* name = 0, WFlags fl = Qt::WStyle_Customize 
			| Qt::WStyle_NoBorder | Qt::WStyle_StaysOnTop | Qt::WType_TopLevel | WType_Popup);
	~EvaFontSelecter();
	void setColor(const QColor &col);
	void setSize( const int size);
signals:
	void fontChanged(QColor, int);
protected slots:
	virtual void languageChange();    
private:
	QLabel* lblColor;
	KColorCombo* color;
	QLabel* lblSize;
	KIntSpinBox* kIntSpinBox;
	QPushButton* pbOK;
	QFrame* frame;
	
	QGridLayout* EvaFontSelecterLayout;
	QGridLayout* EvaFontWindowLayout;
	QHBoxLayout* layout3;
	
private slots:
	void slotClicked();

};

#endif // EVAFONTSELECTER_H
