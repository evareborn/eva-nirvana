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
#include "createsmileywindow.h"
#include "evausersetting.h"
#include "evauser.h"
#include "../evamain.h"
#include "evaqtutil.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qdir.h>
#include <qlistview.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qtable.h>
#include <qpopupmenu.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qmovie.h>

#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klocale.h>


CreateSmileyWindow::CreateSmileyWindow( const QString & url, const int groupIndex )
	: m_FileName(url),
	m_MultiFiles(false),
	m_Config(0),
	m_DefaultGroup(groupIndex)
{
	init();
}

CreateSmileyWindow::~ CreateSmileyWindow( )
{
	if(m_Config) delete m_Config;
}

void CreateSmileyWindow::init( )
{
	if(!m_FileName.isEmpty()){
		QString file = m_FileName.right( m_FileName.length() - m_FileName.findRev("/") - 1);
		lblSelect->setText(file);
		btnSelect->setEnabled(false);

		leShortcut->setText(file.left(6));
		
		leTip->setText(file.left( file.findRev(".") ) );
	}

	QString dir = EvaMain::user->getSetting()->getCustomSmileyDir() + "/";
	m_Config = new CustomFaceConfig(dir);

	if(m_Config->loadXML()){
		QStringList groups = m_Config->groupNames();
		cbbGroup->insertStringList(m_Config->groupNames());
	}
	cbbGroup->setCurrentItem(m_DefaultGroup);

	connect(btnOK, SIGNAL(clicked()), this, SLOT(slotOKClicked() ));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));

	connect(btnSelect, SIGNAL(clicked()), this, SLOT(slotSelectImagesClicked() ));
}

void CreateSmileyWindow::closeEvent( QCloseEvent * e )
{
	e->accept();
	deleteLater();
}

void CreateSmileyWindow::slotSelectImagesClicked( )
{
	QString startDir =  QDir::homeDirPath();
	QStringList fileNames = KFileDialog::getOpenFileNames(startDir,
			"*.png *.bmp *.jpg *.jpeg *.gif |" + i18n(" all images (*.png *.bmp *.jpg *.jpeg *.gif)"), this, 
			i18n("select image file(s)"));
	if(fileNames.count() == 1){
		m_FileName = fileNames[0];
		QString file = m_FileName.right( m_FileName.length() - m_FileName.findRev("/") - 1);

		lblSelect->setText(file);

		leShortcut->setEnabled( true);
		leShortcut->setText(file.left(6));

		leTip->setEnabled( true );
		leTip->setText(file.left( file.findRev(".") ) );
	}else{
		m_MultiFiles = true;
		lblSelect->setText(i18n("Multi-Files Selected."));
		m_FileNames = fileNames;
		leShortcut->setText("");
		leTip->setText("");
		leShortcut->setEnabled( false);
		leTip->setEnabled( false);
	}
}

void CreateSmileyWindow::slotOKClicked( )
{
	bool ok = false;
	QString destDir = EvaMain::user->getSetting()->getCustomSmileyDir();
	if(cbbGroup->currentItem()>0)
		destDir += ( "/" + cbbGroup->currentText() );
	if(!m_FileName.isEmpty()){

 		QString destFile = EvaHelper::generateCustomSmiley(m_FileName, destDir, true);
 		if(!destFile.isEmpty()) {
			QString name = destFile.right( destFile.length() - destFile.findRev("/") - 1);
			CustomFace face(name, leShortcut->text(), leTip->text(), 0, cbbGroup->currentItem());
			ok = m_Config->addFace(face);
		}
	} else {
		///TODO: add multiple files
		for(QStringList::Iterator it = m_FileNames.begin(); it != m_FileNames.end(); ++it){
			QString destFile = EvaHelper::generateCustomSmiley((*it), destDir, true);
			if(!destFile.isEmpty()) {
				QString name = destFile.right( destFile.length() - destFile.findRev("/") - 1);
				CustomFace face( name, name.left(6), name.left(name.findRev(".")), 0, cbbGroup->currentItem());
				ok = m_Config->addFace(face);
			}
		}
	}

	if(ok) m_Config->saveXML();

	emit addCustomSmileyReady(ok);
	close();
}


/**======================================================*/


RmSmileyGroupDialog::RmSmileyGroupDialog(  int indexOfRm, CustomFaceConfig *config, QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ), m_IndexOfRm(indexOfRm), m_Config(config)
{
	if ( !name )
		setName( "RmSmileyGroupUI" );
	RmSmileyGroupUILayout = new QGridLayout( this, 1, 1, 16, 7, "RmSmileyGroupUILayout"); 
	
	layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
	spacer3 = new QSpacerItem( 81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout3->addItem( spacer3 );
	
	btnOK = new QPushButton( this, "btnOK" );
	layout3->addWidget( btnOK );
	
	btnCancel = new QPushButton( this, "btnCancel" );
	layout3->addWidget( btnCancel );
	
	RmSmileyGroupUILayout->addLayout( layout3, 1, 0 );
	
	layout7 = new QVBoxLayout( 0, 6, 15, "layout7"); 
	
	rbtnRmAll = new QRadioButton( this, "rbtnRmAll" );
	layout7->addWidget( rbtnRmAll );
	
	layout6 = new QVBoxLayout( 0, 0, 6, "layout6"); 
	
	rbtnRmMv = new QRadioButton( this, "rbtnRmMv" );
	layout6->addWidget( rbtnRmMv );
	
	layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 
	spacer4 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
	layout4->addItem( spacer4 );
	
	lblGroupTip = new QLabel( this, "lblGroupTip" );
	layout4->addWidget( lblGroupTip );
	
	cbbGroups = new QComboBox( FALSE, this, "cbbGroups" );
	layout4->addWidget( cbbGroups );
	layout6->addLayout( layout4 );
	layout7->addLayout( layout6 );
	
	RmSmileyGroupUILayout->addLayout( layout7, 0, 0 );
	languageChange();
	resize( QSize(309, 171).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

	// make remove all smileys in this group as the default
	rbtnRmAll->setChecked( true);
	rbtnRmMv->setChecked( false );

	// setup combo box
	QString dir = EvaMain::user->getSetting()->getCustomSmileyDir() + "/";
	m_Config = new CustomFaceConfig(dir);

	if(m_Config->loadXML()){
		QStringList groups = m_Config->groupNames();
		for( int i=0; i < (int)(groups.count()); i++){
			if(i != m_IndexOfRm )  // ignore the group we are operating on
				cbbGroups->insertItem(groups[i]);
		}
	}
	cbbGroups->setCurrentItem(0);
	m_Result = -2;
	
	connect(rbtnRmAll, SIGNAL(clicked()), this, SLOT(slotRmAllClicked()));
	connect(rbtnRmMv, SIGNAL(clicked()), this, SLOT(slotRmMvClicked()));

	connect(btnOK, SIGNAL(clicked()), this, SLOT(slotOKClicked() ));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
}

RmSmileyGroupDialog::~RmSmileyGroupDialog()
{
}

void RmSmileyGroupDialog::languageChange()
{
	setCaption( i18n( "Remove Smiley Group" ) );
	btnOK->setText( i18n( "&OK" ) );
	btnOK->setAccel( QKeySequence( "Alt+O" ) );
	btnCancel->setText( i18n( "&Cancel" ) );
	btnCancel->setAccel( QKeySequence( "Alt+C" ) );
	rbtnRmAll->setText( i18n( "Remove all smileys in this group" ) );
	rbtnRmMv->setText( i18n( "Before removing this group," ) );
	lblGroupTip->setText( i18n( "move all its smileys to:" ) );
}

void RmSmileyGroupDialog::slotRmAllClicked()
{
	rbtnRmAll->setChecked( true);
	rbtnRmMv->setChecked( false );
}

void RmSmileyGroupDialog::slotRmMvClicked()
{
	rbtnRmAll->setChecked( false);
	rbtnRmMv->setChecked( true );
}

void RmSmileyGroupDialog::slotOKClicked()
{
	if(rbtnRmAll->isChecked())
		m_Result = -1;
	else{
		m_Result = cbbGroups->currentItem();
		// as we ignored the group itself
		if(m_Result >= m_IndexOfRm)
			m_Result++;
	}
	close();
}

void RmSmileyGroupDialog::slotCancelClicked()
{
	m_Result = -2;
}

int RmSmileyGroupDialog::QueryDialog(int indexOfRm, CustomFaceConfig *config, QWidget* parent)
{
	RmSmileyGroupDialog *dialog = new RmSmileyGroupDialog( indexOfRm, config, parent);
	dialog->exec();
	int result = dialog->m_Result;
	delete dialog;
	return result;
}

/**======================================================*/

class CustomItem : public QTableItem
{
public:
	CustomItem(QTable *table, const QString &text)
		: QTableItem(table, QTableItem::OnTyping, text )
	{
	}	
	CustomItem(QTable *table, const QString &text, const QPixmap &p )
		: QTableItem(table, QTableItem::OnTyping, text, p)
	{
	}
	
	virtual int alignment() const { return Qt::AlignLeft | Qt::AlignVCenter; }
};

class GroupActionPrivate
{
public:
	enum GroupAction {GNone, GSelect, GAdd, GEdit, GRemove};
	
	GroupAction m_Action;
	QListViewItem *m_Item;
	QString m_OldName;
	friend class CustomSmileyManager;
};

CustomSmileyManager::CustomSmileyManager()
	: m_Config(0), m_IsChanged(false), m_MoveToMenu(0)
{
	init();

	connect(btnOK, SIGNAL(clicked()), this, SLOT(slotOKClicked() ));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
}

CustomSmileyManager::~CustomSmileyManager()
{
	if(m_Config) delete m_Config;
}

void CustomSmileyManager::init( )
{
	// doesn't support import/export to eip package
	tbtnImport->hide();
	tbtnExport->hide();

	// cancel does same job as ok, so hide it for now
	btnCancel->hide();

	lblPreview->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

	d = new GroupActionPrivate;
	d->m_Action = GroupActionPrivate::GNone;
	d->m_Item = 0;
	d->m_OldName = "";

	if(m_Config) delete m_Config;
	QString dir = EvaMain::user->getSetting()->getCustomSmileyDir() + "/";
	m_Config = new CustomFaceConfig(dir);
	m_Config->loadXML();
	// group list first
	loadGroups();

	// init table
	initFaces();

	lvGroups->setSelected( lvGroups->firstChild(), true);
}

void CustomSmileyManager::loadGroups( )
{
	if(!m_Config) return;

	lvGroups->addColumn(i18n("Smiley Groups"));
	lvGroups->setColumnWidth(0, lvGroups->width());
	lvGroups->setItemMargin( 2 );

	QStringList groups = m_Config->groupNames();
	// as  QLIstView alway insert the current item on the top,
	// so, just add items reversely
	for( int i = (int)(groups.count() -1); i >= 0; i--){
		(void) new QListViewItem(lvGroups, groups[i]);
	}
	connect(tbtnAddGroup, SIGNAL(clicked()), this, SLOT(slotAddGroupClicked()));
	connect(tbtnEditGroup, SIGNAL(clicked()), this, SLOT(slotEditGroupClicked()));
	connect(tbtnRemoveGroup, SIGNAL(clicked()), this, SLOT(slotRemoveGroupClicked()));

	connect(lvGroups, SIGNAL(itemRenamed(QListViewItem*, int, const QString &)),
			this, SLOT(slotGroupRenamed(QListViewItem*, int, const QString &)));
	connect(lvGroups, SIGNAL(selectionChanged( QListViewItem *) ),
			this, SLOT(slotGroupSelectionChanged( QListViewItem *) ) );
}

void CustomSmileyManager::initFaces( )
{
	tblFaceList->verticalHeader()->hide();
	tblFaceList->horizontalHeader()->setLabel( 0, i18n("No." ) );
	tblFaceList->horizontalHeader()->setLabel( 1, i18n("Smiley" ) );
	tblFaceList->horizontalHeader()->setLabel( 2, i18n("Shortcut" ) );
	tblFaceList->setColumnWidth(0, 15);
	tblFaceList->setColumnWidth(1, tblFaceList->visibleWidth() - 15 - 60 );
	tblFaceList->setLeftMargin(0);

	connect(tblFaceList, SIGNAL(selectionChanged( )), this, SLOT(slotFaceSelectionChanged( )));
	connect(tblFaceList, SIGNAL(valueChanged(int, int)), this, SLOT(slotFaceValueChanged(int, int)));

	connect(btnAdd, SIGNAL(clicked()), this, SLOT(slotAddSmiley()));
	connect(btnRemove, SIGNAL(clicked()), this, SLOT(slotRemoveSmiley()));
	connect(btnEdit, SIGNAL(clicked()), this, SLOT(slotEditSmiley()));
	connect(btnUp, SIGNAL(clicked()), this, SLOT(slotUpSmiley()));
	connect(btnDown, SIGNAL(clicked()), this, SLOT(slotDownSmiley()));
	connect(btnMoveTo, SIGNAL(clicked()), this, SLOT(slotMoveToSmiley()));
}

void CustomSmileyManager::loadSmileys( int groupIndex )
{
	// clear the table first
	for(int row = 0; row < tblFaceList->numRows(); row++){
			tblFaceList->removeRow(row);
	}
	tblFaceList->setNumRows(0);
	QString dir = EvaMain::user->getSetting()->getCustomSmileyDir() + "/";

	if(groupIndex>0)
		dir += (m_Config->groupName( groupIndex) + "/");

	FaceList list = m_Config->groupMembers( groupIndex );
	int row = 0;
	if(list.size()){
		tblFaceList->setNumRows(list.size());
		tblFaceList->horizontalHeader()->setLabel( 0, i18n("No." ) );
		tblFaceList->horizontalHeader()->setLabel( 1, i18n("Smiley" ) );
		tblFaceList->horizontalHeader()->setLabel( 2, i18n("Shortcut" ) );
		for( FaceList::Iterator it = list.begin(); it != list.end(); ++it){
			//tblFaceList->setText(row, 0, QString::number( row + 1));
			tblFaceList->setItem(row, 0, 
						new QTableItem(tblFaceList, QTableItem::Never,
									QString::number(row + 1)));

			QPixmap *pixmap = new QPixmap(dir + (*it).fixed());
			CustomItem *item = new CustomItem(tblFaceList, (*it).tip(), *pixmap);
			tblFaceList->setItem(row, 1, item);

			CustomItem *itemSC = new CustomItem(tblFaceList, (*it).shortcut() );
			tblFaceList->setItem(row, 2, itemSC);

			tblFaceList->setRowHeight(row, pixmap->width() + 2);
			delete pixmap;
			row++;
		}
		tblFaceList->setColumnWidth(0, 20);
		tblFaceList->setColumnWidth(1, tblFaceList->visibleWidth() - 20 - 50 );
	}
}

void CustomSmileyManager::closeEvent( QCloseEvent * e )
{
	e->accept();
	if(m_IsChanged){
		m_Config->saveXML();
		emit customSmileyChanged();
	}
	deleteLater();
}

void CustomSmileyManager::slotOKClicked( )
{
	if(m_IsChanged){
		m_Config->saveXML();
		emit customSmileyChanged();
	}
	close();
}

// add a group
void CustomSmileyManager::slotAddGroupClicked( )
{
	QString name = QString(i18n("Group%1")).arg(m_Config->numGroups());

	QString path = EvaMain::user->getSetting()->getCustomSmileyDir() + "/";
	QDir dir(path + name);
	if(!dir.exists()){ // if the dest dir doesn't exist,
		if(dir.mkdir( path + name )) // create the dir
			m_IsChanged = true;
		else{  // create failed
			KMessageBox::error( this, i18n( "Cannot create the directory \"%1\". " ).arg( dir.path() ), i18n( "Add Group" ) );
			return;
		}
	} else { // the directory exists, the group name should be used already
		KMessageBox::error( this, i18n( "A directory named \"%1\" already exists. " ).arg( dir.path() ), i18n( "Directory Exists!" ) );
		return;
	}
	QListViewItem *item = new QListViewItem(lvGroups, lvGroups->lastItem(), name);
	m_Config->addGroup( name );
	m_IsChanged = true;
	m_Config->saveXML();

	lvGroups->setSelected( item, true);

	d->m_Action = GroupActionPrivate::GEdit;
	d->m_Item = item;
	d->m_OldName = name;
	item->setRenameEnabled( 0, true);
	item->startRename( 0 );
}

// rename the group
void CustomSmileyManager::slotEditGroupClicked( )
{
	QListViewItem *item = lvGroups->selectedItem();
	if(item){
		if(item == lvGroups->firstChild()) return;

		d->m_Action = GroupActionPrivate::GEdit;
		d->m_Item = item;
		d->m_OldName = item->text( 0 );

		item->setRenameEnabled( 0, true );
		item->startRename( 0 );
	}
}

/** remove one group, 2 options:
      1: remove all smileys in this group, then remove the group itself
      2: move all sub items into another group, then remove the empty group

     NOTE: this operation is un-recoverable.
*/
void CustomSmileyManager::slotRemoveGroupClicked( )
{
	if(!m_Config) return;

	QListViewItem *item = lvGroups->selectedItem();
	if(item){
		if(item == lvGroups->firstChild()) return; // never remove default group

		int index = m_Config->groupIndex( item->text( 0 ) );
		if(index <= -1) return;

		QString path = EvaMain::user->getSetting()->getCustomSmileyDir() + "/";
		QDir dir( path + item->text(0) );
		FaceList list = m_Config->groupMembers( index);
		int result = -2;
		if(list.size() > 0 ){
			result = RmSmileyGroupDialog::QueryDialog( index, m_Config, this);
			switch(result){
				case -2: // abort
					return;
					break;
				case -1:  // remove all
					break;
				default: // otherwise, move to result
				{
					if(dir.exists()){
						dir.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
						const QFileInfoList *list = dir.entryInfoList();
						QFileInfoListIterator it( *list);
						QFileInfo *fi;
						QString destDir = path;
						if(result>0)
							destDir += ( m_Config->groupName( result ) + "/") ;
						while( (fi = it.current() ) != 0 ){
							EvaHelper::copyFile(fi->absFilePath(), (destDir + fi->fileName()) );
							++it;
						}
					}
					m_Config->moveChildrenTo(index, result);
					m_IsChanged = true;
				}
			}
		}

		if(dir.exists()){
			// update the in-memory config
			if(m_Config->removeGroup( index )){
				lvGroups->takeItem( item);
				delete item;
				m_IsChanged = true;
			} else return;

			dir.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
			const QFileInfoList *list = dir.entryInfoList();
			QFileInfoListIterator it( *list);
			QFileInfo *fi;
			while( (fi = it.current() ) != 0 ){
				dir.remove( fi->absFilePath() );
				++it;
			}
			// as all files have been deleted,  we need keep the config file up to date.
			m_Config->saveXML();
			if( ! dir.rmdir( dir.path(), true)){
				KMessageBox::error( this, i18n( "Remove directory \"%1\" failed. " ).arg( dir.path() ),
							i18n( "Remove Directory!" ) );
			}
		}
	}
}

void CustomSmileyManager::slotGroupRenamed( QListViewItem *item, int /*col*/, const QString & text)
{
	item->setRenameEnabled( 0, false ); // make it un-editable
	if(!m_Config) return;
	if(d->m_Action != GroupActionPrivate::GEdit) return;
	if(d->m_Item != item ) return;
	if(d->m_OldName == text) return;
	
	if( ! m_Config->renameGroup(d->m_OldName, text)){
		KMessageBox::information( this, i18n( "Cannot rename the group name \"%1\" to \"%2\". " )
									.arg( d->m_OldName).arg( text),
								i18n( "Rename Group" ) );
	} else{
		QString path = EvaMain::user->getSetting()->getCustomSmileyDir() + "/";
		QDir dir(path + d->m_OldName);
		if(dir.exists()){
			if(!dir.rename(path + d->m_OldName, path + text, true)){
				KMessageBox::error( this, i18n( "Can not change the directory name: %1. " )
									.arg( dir.path()), i18n("Rename Group") );
			} else{
				m_IsChanged = true;
			}
		} else {
			if(dir.mkdir( path + text )){
				m_IsChanged = true;
			}else
				KMessageBox::error( this, i18n( "Cannot create the directory: \"%1\". " ).arg( dir.path() ),
							i18n( "Rename Group" ) );
		}
	}
}

void CustomSmileyManager::slotGroupSelectionChanged( QListViewItem * group)
{
	if(!group) return;
	group->setSelected( true );
	loadSmileys(m_Config->groupIndex( group->text(0) ) );
}

void CustomSmileyManager::slotFaceSelectionChanged( )
{
	// QTable::numSelections() doesn't return the correct value
	// if use select some rows by pressing shift key

	int numSelections = 0;
	int first = -1;
	for(int r = 0; r < tblFaceList->numRows(); r++){
		numSelections += ((tblFaceList->isRowSelected(r))?1:0);
		if( (numSelections == 1) && (first == -1) ){
			first = r;
		}
	}
	bool isSingle = (numSelections == 1);
	btnEdit->setEnabled(isSingle);
	btnUp->setEnabled(isSingle);
	btnDown->setEnabled(isSingle);
	if(numSelections){
		QListViewItem *item = lvGroups->selectedItem();
		if(item){
			int gId = m_Config->groupIndex( item->text( 0 ) );
			if(gId < 0) return;
			CustomFace face = m_Config->getFace( gId, first);
			QString path = EvaMain::user->getSetting()->getCustomSmileyDir() + "/";
			if(gId > 0 )
				path += (item->text(0) + "/");
			QString ext = face.org().right(3);
			lblPreview->clear();
			if(ext.lower() == "gif"){
				lblPreview->setMovie( QMovie(path + face.org()));
			} else {
				QImage img(path + face.org() );
				
				lblPreview->setPixmap( QPixmap(img.smoothScale( lblPreview->size(), QImage::ScaleMin)));
			}
		}
	}
}

void CustomSmileyManager::slotFaceValueChanged( int row, int col )
{
	if(!m_Config) return;

	QListViewItem *item = lvGroups->selectedItem();
	if(item){
		int gId = m_Config->groupIndex( item->text( 0 ) );
		if(gId < 0) return;
		QString text = tblFaceList->text(row, col);
		switch(col){
		case 1:  // tip
			m_Config->updateFaceTip(gId, row, tblFaceList->text(row, col));
			m_Config->saveXML();
			m_IsChanged = true;
			break;
		case 2:  // shortcut
			m_Config->updateFaceShortcut(gId, row, tblFaceList->text(row, col));
			m_Config->saveXML();
			m_IsChanged = true;
			break;
		default: // anything else, ignore
			break;
		}
	}
}

void CustomSmileyManager::slotAddSmiley( )
{
	if(!m_Config) return;

	QListViewItem *item = lvGroups->selectedItem();
	if(! item) return;

	int gId = m_Config->groupIndex( item->text( 0 ) );
	if(gId < 0) return;
	
	CreateSmileyWindow *win = new CreateSmileyWindow(QString::null, gId);
	connect(win, SIGNAL(addCustomSmileyReady( bool )), this, SLOT(slotAddCustomSmileyReady( bool )));
	win->show();
}

void CustomSmileyManager::slotAddCustomSmileyReady( bool ok)
{
	if(!ok) return;
	if(!m_Config) return;

	QListViewItem *item = lvGroups->selectedItem();
	if(! item) return;

	int gId = m_Config->groupIndex( item->text( 0 ) );
	if(gId < 0) return;

	m_Config->loadXML();
	loadSmileys(gId);
	// notify smiley popup window to update
	emit customSmileyChanged();
}

bool CustomSmileyManager::removeSmileyFile( const CustomFace & face )
{
	QString path = EvaMain::user->getSetting()->getCustomSmileyDir() + "/";
	if(face.group() > 0){
		path += ( m_Config->groupName( face.group()) + "/");
	}
	QDir dir( path);
	if(dir.exists()){
		if(dir.remove( path + face.fixed())) // remove thumbnail
			return dir.remove(path + face.org()); // remove smiley
	}
	return false;
}

void CustomSmileyManager::slotRemoveSmiley( )
{
	if(!m_Config) return;

	QListViewItem *item = lvGroups->selectedItem();
	if(item){
		int gId = m_Config->groupIndex( item->text( 0 ) );
		if(gId < 0) return;

		// as removing row also clear the selection( or change to the last row),
		// so we have to record the selection first
		bool *flags = new bool[tblFaceList->numRows()];
		int row = 0;
		for(row = 0; row < tblFaceList->numRows(); row++){
			if (tblFaceList->isRowSelected(row))
				flags[row] = true;
			else
				flags[row] = false;
		}

		for( row = tblFaceList->numRows() -1; row >= 0; row--){
			if (flags[row]){
				CustomFace face = m_Config->getFace( gId, row);
				if(removeSmileyFile(face)){
					// update config first
					m_Config->removeFace( gId, row);
					m_Config->saveXML(); // update config file in case
			
					// update display
					tblFaceList->removeRow(row);
		
					m_IsChanged = true;
				}
			}
		}
		delete []flags;
	}
}

void CustomSmileyManager::slotEditSmiley( )
{
	int row = 0;
	for(row = 0; row < tblFaceList->numRows(); row++){
		if (tblFaceList->isRowSelected(row)) break;
	}
	// start editing tip. (should we set editing shortcut as default?)
	tblFaceList->editCell(row, 1);
}

void CustomSmileyManager::slotUpSmiley( )
{
	if(!m_Config) return;

	QListViewItem *item = lvGroups->selectedItem();
	if(item){
		int gId = m_Config->groupIndex( item->text( 0 ) );
		if(gId < 0) return;
		int row = 1; // the first row cannot be moved up
		for(row = 1; row < tblFaceList->numRows(); row++){
			if (tblFaceList->isRowSelected(row)){
				m_Config->moveFaceUp(gId, row);
				tblFaceList->swapRows(row, row -1);
				//tblFaceList->updateContents();
				tblFaceList->clearSelection();
				tblFaceList->selectRow(row - 1);
				m_IsChanged = true;
				break;
			}
		}
	}
}

void CustomSmileyManager::slotDownSmiley( )
{
	if(!m_Config) return;

	QListViewItem *item = lvGroups->selectedItem();
	if(item){
		int gId = m_Config->groupIndex( item->text( 0 ) );
		if(gId < 0) return;
		int row = 0;
		for(row = 0; row < tblFaceList->numRows() - 1; row++){  // ignore the last row
			if (tblFaceList->isRowSelected(row)) {
				m_Config->moveFaceDown( gId, row);
				tblFaceList->swapRows(row, row + 1);
				//tblFaceList->updateContents();
				tblFaceList->clearSelection();
				tblFaceList->selectRow(row + 1);
				m_IsChanged = true;
				break;
			}
		}
	}
}

void CustomSmileyManager::slotMoveToSmiley( )
{
	if(!m_Config) return;

	QListViewItem *item = lvGroups->selectedItem();
	if(item){
		if(m_MoveToMenu){
			disconnect(m_MoveToMenu, 0, 0, 0);
			delete m_MoveToMenu;
		}
		m_MoveToMenu = new QPopupMenu(0,"moveto");

		int currIndex = m_Config->groupIndex( item->text( 0 ) );
		QStringList groups = m_Config->groupNames();
		for( int i=0; i < (int)(groups.count()); i++){
			if(i != currIndex )  // ignore the group we are operating on
				m_MoveToMenu->insertItem(groups[i], i);
		}
		connect(m_MoveToMenu, SIGNAL(activated(int)), this, SLOT(slotMoveSmileyTo(int)));
		QPoint p = btnMoveTo->mapToGlobal(QPoint(btnMoveTo->width()+1,0));
		m_MoveToMenu->popup( p );
	}
}

void CustomSmileyManager::slotMoveSmileyTo( int Id )
{
	if(!m_Config) return;

	QListViewItem *item = lvGroups->selectedItem();
	if(item){
		QString srcGName = item->text(0); // selected group name
		int gId = m_Config->groupIndex( srcGName );  // selected group index
		QString destGName = m_Config->groupName( Id); // dest group name
		if( (gId < 0) || (Id < 0) || (gId == Id) ) return;

		QString dir = EvaMain::user->getSetting()->getCustomSmileyDir() + "/";
		QString src = dir, dest = dir;
		if(gId != 0) src += (srcGName + "/");
		if(Id != 0 ) dest += (destGName + "/");

		int row = 0;
		bool moved = false;

		// as removing row also clear the selection( or change to the last row),
		// so we have to record the selection first
		bool *flags = new bool[tblFaceList->numRows()];
		for(row = 0; row < tblFaceList->numRows(); row++){
			if (tblFaceList->isRowSelected(row))
				flags[row] = true;
			else
				flags[row] = false;
		}

		for( row = tblFaceList->numRows() -1; row >= 0; row--){
			if (flags[row]){
				CustomFace face = m_Config->getFace( gId, row);
				if(m_Config->moveFaceTo( gId, row, Id)){  // if update config true

					EvaHelper::copyFile( src + face.fixed(), dest + face.fixed());
					EvaHelper::copyFile( src + face.org(), dest + face.org());
			
					QDir smileyDir(src);
					smileyDir.remove( src + face.fixed());
					smileyDir.remove( src +  + face.org());

					// update display
					tblFaceList->removeRow(row);

					m_IsChanged = true;
					moved = true;
				}
			}
		}
		delete []flags;
		if(moved)
			m_Config->saveXML(); // save config in case of some unexpected situations
	}
}
