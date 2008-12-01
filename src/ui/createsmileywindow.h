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
#ifndef EVACREATESMILEYWINDOW_H
#define EVACREATESMILEYWINDOW_H

#include "createsmileyui.h"
#include "customfacemanagerui.h"
#include "customface.h"
#include <qdialog.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QCloseEvent>

class CustomFaceConfig;
class QCloseEvent;

class CreateSmileyWindow : public CreateSmileyUI
{
	Q_OBJECT

public:
	CreateSmileyWindow(const QString &url = QString::null, const int groupIndex = 0 );
	virtual ~CreateSmileyWindow();

signals:
	void addCustomSmileyReady( bool );  // adding ok or not

private:
	QString m_FileName;
	bool m_MultiFiles;
	QStringList m_FileNames;
	CustomFaceConfig *m_Config;
	int m_DefaultGroup;

	void init();

	virtual void closeEvent ( QCloseEvent *e);
private slots:

	void slotSelectImagesClicked();
	void slotOKClicked();
};

class GroupActionPrivate;

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QPushButton;
class QRadioButton;
class QLabel;
class QComboBox;

class RmSmileyGroupDialog : public QDialog
{
    Q_OBJECT

public:
	RmSmileyGroupDialog( int indexOfRm, CustomFaceConfig *config, QWidget* parent = 0, const char* name = 0, bool modal = true, Qt::WFlags fl = 0 );
	virtual ~RmSmileyGroupDialog();
	
	QPushButton* btnOK;
	QPushButton* btnCancel;
	QRadioButton* rbtnRmAll;
	QRadioButton* rbtnRmMv;
	QLabel* lblGroupTip;
	QComboBox* cbbGroups;

	// return     -2: above this operation, -1: remove all,  otherwise the group index all sub smiley will be moved into
	int m_Result;
	static int QueryDialog(int indexOfRm, CustomFaceConfig *config, QWidget* parent = 0);

private:
	Q3GridLayout* RmSmileyGroupUILayout;
	Q3HBoxLayout* layout3;
	QSpacerItem* spacer3;
	Q3VBoxLayout* layout7;
	Q3VBoxLayout* layout6;
	Q3HBoxLayout* layout4;
	QSpacerItem* spacer4;

	int m_IndexOfRm;
	CustomFaceConfig *m_Config;
protected slots:
	virtual void languageChange();
private slots:
	void slotRmAllClicked();
	void slotRmMvClicked();

	void slotOKClicked();
	void slotCancelClicked();
};

class QMenu;

class CustomSmileyManager : public CustomFaceManagerUI
{
	Q_OBJECT

public:
	CustomSmileyManager();
	~CustomSmileyManager();

signals:
	void customSmileyChanged();

private:
	CustomFaceConfig *m_Config;
	bool m_IsChanged;
	GroupActionPrivate *d;
	QMenu *m_MoveToMenu;

	virtual void closeEvent ( QCloseEvent *e);

	void init();

	void loadGroups();

	void initFaces();
	void loadSmileys(int groupIndex);
	bool removeSmileyFile(const CustomFace &face);
private slots:
	
	void slotOKClicked();

	void slotAddGroupClicked();
	void slotEditGroupClicked();
	void slotRemoveGroupClicked();

	void slotGroupRenamed(Q3ListViewItem*, int, const QString &);
	void slotGroupSelectionChanged( Q3ListViewItem *);

	void slotFaceSelectionChanged( );
	void slotFaceValueChanged(int row, int col);

	void slotAddSmiley();
	void slotAddCustomSmileyReady( bool );
	void slotRemoveSmiley();
	void slotEditSmiley();
	void slotUpSmiley();
	void slotDownSmiley();
	void slotMoveToSmiley();
	void slotMoveSmileyTo(int id);
};
#endif
 
