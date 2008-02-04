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

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;
class QComboBox;

class CreateSmileyUI : public QWidget
{
    Q_OBJECT

public:
    CreateSmileyUI( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
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
    QGridLayout* CreateSmileyUILayout;
    QVBoxLayout* layout21;
    QHBoxLayout* layout16;
    QSpacerItem* spacer6;
    QHBoxLayout* layout19;
    QSpacerItem* spacer6_3;
    QSpacerItem* spacer9;
    QHBoxLayout* layout17;
    QSpacerItem* spacer6_2;
    QHBoxLayout* layout20;
    QSpacerItem* spacer6_2_2;
    QHBoxLayout* layout14;
    QSpacerItem* spacer5;

protected slots:
    virtual void languageChange();

};

#endif // CREATESMILEYUI_H
