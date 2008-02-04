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

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QRadioButton;
class QLabel;
class QComboBox;

class RmSmileyGroupDialog : public QDialog
{
    Q_OBJECT

public:
	RmSmileyGroupDialog( int indexOfRm, CustomFaceConfig *config, QWidget* parent = 0, const char* name = 0, bool modal = true, WFlags fl = 0 );
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
	QGridLayout* RmSmileyGroupUILayout;
	QHBoxLayout* layout3;
	QSpacerItem* spacer3;
	QVBoxLayout* layout7;
	QVBoxLayout* layout6;
	QHBoxLayout* layout4;
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

class QPopupMenu;

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
	QPopupMenu *m_MoveToMenu;

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

	void slotGroupRenamed(QListViewItem*, int, const QString &);
	void slotGroupSelectionChanged( QListViewItem *);

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
 
