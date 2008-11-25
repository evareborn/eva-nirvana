/****************************************************************************
** Form interface generated from reading ui file 'createsmileyui.ui'
**
** Created: 五  9月 1 00:01:38 2006
**      by: The User Interface Compiler ($Id: createsmileyui.h,v 1.1 2006/08/31 13:10:37 yunfan Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CREATESMILEYUI_H
#define CREATESMILEYUI_H

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;
class QComboBox;

class CreateSmileyUI : public QWidget
{
    Q_OBJECT

public:
    CreateSmileyUI( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~CreateSmileyUI();

    QLabel* lblTop;
    QLabel* lblStep1;
    QLabel* lblSelect;
    QPushButton* btnSelect;
    QLabel* lblStep2;
    QLineEdit* leShortcut;
    QLabel* lblStep2_2;
    QComboBox* cbbGroup;
    QLabel* lblStep3;
    QLineEdit* leTip;
    QLabel* lblCommends;
    QPushButton* btnOK;
    QPushButton* btnCancel;

protected:
    Q3GridLayout* CreateSmileyUILayout;
    Q3VBoxLayout* layout21;
    Q3HBoxLayout* layout16;
    QSpacerItem* spacer6;
    Q3HBoxLayout* layout19;
    QSpacerItem* spacer6_3;
    QSpacerItem* spacer9;
    Q3HBoxLayout* layout17;
    QSpacerItem* spacer6_2;
    Q3HBoxLayout* layout20;
    QSpacerItem* spacer6_2_2;
    Q3HBoxLayout* layout14;
    QSpacerItem* spacer5;

protected slots:
    virtual void languageChange();

};

#endif // CREATESMILEYUI_H
