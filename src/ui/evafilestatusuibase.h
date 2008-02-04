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

#ifndef EVAFILESTATUSUIBASE_H
#define EVAFILESTATUSUIBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class KProgress;
class KPushButton;

class EvaFileStatusUIBase : public QWidget
{
	Q_OBJECT

public:
	EvaFileStatusUIBase( const unsigned int session, const unsigned int size = 0,
				QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~EvaFileStatusUIBase();
	
	inline const unsigned int getSession() const { return m_Session; }

	void setMimePixmap(const QPixmap &pixmap);
	void setFileName(const QString &fileName);
	void setStartPosition(const unsigned int offset) { m_StartPosition = offset; }
	void updateStatus(const unsigned int size, const unsigned int bytes, const unsigned int time);

	const QString strFileSize(const float size, int precise = 2);
	inline const bool isFinished() const { return m_IsFinished; }
signals:
	void closeRequest(const unsigned int, QWidget *); // send itself to parent
protected:
	QGridLayout* EvaFileStatusUIBaseLayout;
	QVBoxLayout* layout10;
	QHBoxLayout* layout6;
	QHBoxLayout* layout1;
	QVBoxLayout* layout9;

	QLabel* lblIcon;
	QLabel* lblFileName;
	KProgress* kpgFileStatus;
	KPushButton* kpbtnClose;
	QLabel* lblReceived;
	QLabel* lblSpeed;

protected slots:
	virtual void languageChange();
	void slotCloseClicked();

private:
	unsigned int m_StartPosition;
	unsigned int m_Session;
	bool m_IsFinished;
};

#endif // EVAFILESTATUSUIBASE_H
