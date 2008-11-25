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
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3Frame>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class Q3Frame;
class QToolButton;
class Q3ListView;
class Q3ListViewItem;
class Q3Table;
class QPushButton;
class QLabel;

class CustomFaceManagerUI : public QWidget
{
    Q_OBJECT

public:
    CustomFaceManagerUI( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~CustomFaceManagerUI();

    Q3Frame* frame7;
    QToolButton* tbtnImport;
    QToolButton* tbtnExport;
    QToolButton* tbtnAddGroup;
    QToolButton* tbtnEditGroup;
    QToolButton* tbtnRemoveGroup;
    Q3Frame* frame6;
    Q3ListView* lvGroups;
    Q3Frame* frmMain;
    Q3Table* tblFaceList;
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
    Q3GridLayout* CustomFaceManagerUILayout;
    Q3VBoxLayout* layout9;
    Q3GridLayout* frame7Layout;
    Q3HBoxLayout* layout1;
    Q3HBoxLayout* layout7;
    Q3GridLayout* frame6Layout;
    Q3GridLayout* frmMainLayout;
    Q3HBoxLayout* layout6;
    Q3VBoxLayout* layout5;
    QSpacerItem* spacer1;
    Q3HBoxLayout* layout8;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;

};

#endif // CUSTOMFACEMANAGERUI_H
