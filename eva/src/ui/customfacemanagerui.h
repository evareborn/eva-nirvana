/****************************************************************************
** Form interface generated from reading ui file 'customfacemanagerui.ui'
**
** Created: 五  9月 1 00:01:38 2006
**      by: The User Interface Compiler ($Id: customfacemanagerui.h,v 1.2 2006/09/05 12:18:48 yunfan Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CUSTOMFACEMANAGERUI_H
#define CUSTOMFACEMANAGERUI_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QToolButton;
class QListView;
class QListViewItem;
class QTable;
class QPushButton;
class QLabel;

class CustomFaceManagerUI : public QWidget
{
    Q_OBJECT

public:
    CustomFaceManagerUI( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~CustomFaceManagerUI();

    QFrame* frame7;
    QToolButton* tbtnImport;
    QToolButton* tbtnExport;
    QToolButton* tbtnAddGroup;
    QToolButton* tbtnEditGroup;
    QToolButton* tbtnRemoveGroup;
    QFrame* frame6;
    QListView* lvGroups;
    QFrame* frmMain;
    QTable* tblFaceList;
    QPushButton* btnAdd;
    QPushButton* btnRemove;
    QPushButton* btnEdit;
    QPushButton* btnUp;
    QPushButton* btnDown;
    QPushButton* btnMoveTo;
    QLabel* lblPreview;
    QPushButton* btnOK;
    QPushButton* btnCancel;

protected:
    QGridLayout* CustomFaceManagerUILayout;
    QVBoxLayout* layout9;
    QGridLayout* frame7Layout;
    QHBoxLayout* layout1;
    QHBoxLayout* layout7;
    QGridLayout* frame6Layout;
    QGridLayout* frmMainLayout;
    QHBoxLayout* layout6;
    QVBoxLayout* layout5;
    QSpacerItem* spacer1;
    QHBoxLayout* layout8;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;

};

#endif // CUSTOMFACEMANAGERUI_H
