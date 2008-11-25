/***************************************************************************
 *   Copyright (C) 2006 by casper                                          *
 *   tlmcasper@163.com                                                     *
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



#ifndef EVASEARCHUIBASE_H
#define EVASEARCHUIBASE_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <QLabel>
#include <Q3VBoxLayout>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class Q3WidgetStack;
class QLabel;
class Q3ButtonGroup;
class QRadioButton;
class Q3GroupBox;
class QLineEdit;
class Q3Table;
class QToolButton;
class QCheckBox;
class QComboBox;
class QPushButton;

class EvaSearchUIBase : public QDialog
{
    Q_OBJECT

public:
    EvaSearchUIBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~EvaSearchUIBase();

    QTabWidget* twSearchMain;
    QWidget* tabBasicSearch;
    Q3WidgetStack* wsBasicSearch;
    QWidget* wsBSPage;
    QLabel* lblSearchOptionTip;
    Q3ButtonGroup* bgBSType;
    QRadioButton* rbCustomSearch;
    QRadioButton* rbSearchFrdCenter;
    QRadioButton* rbSearchOnline;
    Q3WidgetStack* wsBasicCondtion;
    QWidget* wsCustomSearchCondition;
    Q3GroupBox* gbCustomCondition;
    QLabel* lblQQNum;
    QLabel* lblNickName;
    QLineEdit* leQQNum;
    QLineEdit* leNickName;
    QWidget* wsFrdCenterSearchCondition;
    Q3GroupBox* gbFrdCenterCondition;
    QLabel* lblQQFrdTip;
    QLabel* lblOnlineNumbers;
    QLabel* lblOnlineNum;
    QWidget* wsBSResultPage;
    QLabel* lblBSResultTip;
    Q3Table* tbBSResult;
    QLabel* lblBSResultPage;
    QToolButton* tbBSAll;
    QToolButton* tbBSDetails;
    QToolButton* tbBSPrev;
    QToolButton* tbBSNext;
    QWidget* tabAdvancedSearch;
    Q3WidgetStack* wsAdvancedSearch;
    QWidget* wsASPage;
    QLabel* lblASTip;
    QCheckBox* chbOnlineUsers;
    QCheckBox* chbHaveCamera;
    Q3GroupBox* gbBasicCondition;
    QLabel* lblASProvince;
    QLabel* lblASCity;
    QLabel* lblASAge;
    QLabel* lblASGender;
    QComboBox* cbASProvince;
    QComboBox* cbASCity;
    QComboBox* cbASAge;
    QComboBox* cbASSex;
    QWidget* wsASResultPage;
    QLabel* lblASResultTip;
    Q3Table* tbASResult;
    QLabel* lblASResultPage;
    QToolButton* tbASAll;
    QToolButton* tbASDetails;
    QToolButton* tbASPrev;
    QToolButton* tbASNext;
    QWidget* tabQunSearch;
    Q3WidgetStack* wsQunSearch;
    QWidget* wsQSPage;
    QLabel* lblQSTip;
    Q3ButtonGroup* bgQSType;
    QRadioButton* rbSearchAlumni;
    QRadioButton* rbAccuratelySearch;
    QRadioButton* rbSearchByCategory;
    Q3GroupBox* gbQCondition;
    Q3WidgetStack* wsQunCondition;
    QWidget* wsCategoryConditon;
    QLabel* lblCategory;
    QComboBox* cbCategory1;
    QComboBox* cbCategory2;
    QComboBox* cbCategory3;
    QWidget* wsAlumniCondition;
    QLabel* lblAlumniTip;
    QWidget* wsAccurateCondition;
    QLabel* lblQunNum;
    QLineEdit* leQunNum;
    QWidget* wsQSResultPage;
    QLabel* lblQSResultTip;
    Q3Table* tbQSResult;
    QLabel* lblQSResultPage;
    QToolButton* tbQSAll;
    QToolButton* tbQSDetails;
    QToolButton* tbQSPrev;
    QToolButton* tbQSNext;
    QPushButton* pbLastStep;
    QPushButton* pbSearch;
    QPushButton* pbClose;
    QLabel* lblLogo;

protected:
    Q3GridLayout* EvaSearchUIBaseLayout;
    Q3GridLayout* layout127;
    Q3GridLayout* tabBasicSearchLayout;
    Q3GridLayout* wsBSPageLayout;
    Q3VBoxLayout* layout135;
    Q3VBoxLayout* layout105;
    Q3HBoxLayout* layout100;
    QSpacerItem* spacer54;
    Q3GridLayout* bgBSTypeLayout;
    Q3GridLayout* wsCustomSearchConditionLayout;
    Q3GridLayout* gbCustomConditionLayout;
    QSpacerItem* spacer119_2;
    Q3HBoxLayout* layout204_2;
    Q3VBoxLayout* layout188_2;
    Q3VBoxLayout* layout203_2;
    Q3GridLayout* wsFrdCenterSearchConditionLayout;
    Q3GridLayout* gbFrdCenterConditionLayout;
    Q3VBoxLayout* layout46;
    QSpacerItem* spacer27;
    QSpacerItem* spacer28;
    Q3HBoxLayout* layout316_2;
    QSpacerItem* spacer111;
    Q3GridLayout* wsBSResultPageLayout;
    Q3VBoxLayout* layout47;
    Q3HBoxLayout* layout96;
    QSpacerItem* spacer46;
    Q3HBoxLayout* layout46_2;
    QSpacerItem* spacer47;
    Q3GridLayout* tabAdvancedSearchLayout;
    Q3GridLayout* wsASPageLayout;
    Q3VBoxLayout* layout91;
    QSpacerItem* spacer40_2;
    Q3VBoxLayout* layout371;
    Q3HBoxLayout* layout370;
    QSpacerItem* spacer156;
    QSpacerItem* spacer157;
    Q3HBoxLayout* layout369;
    QSpacerItem* spacer155;
    Q3GridLayout* gbBasicConditionLayout;
    QSpacerItem* spacer159;
    Q3HBoxLayout* layout376;
    Q3VBoxLayout* layout372;
    Q3VBoxLayout* layout375;
    Q3HBoxLayout* layout373;
    QSpacerItem* spacer158;
    Q3VBoxLayout* wsASResultPageLayout;
    Q3VBoxLayout* layout47_2;
    Q3HBoxLayout* layout96_2;
    QSpacerItem* spacer48;
    Q3HBoxLayout* layout46_2_2;
    QSpacerItem* spacer50;
    Q3GridLayout* tabQunSearchLayout;
    Q3GridLayout* wsQSPageLayout;
    Q3VBoxLayout* layout203;
    Q3HBoxLayout* layout92;
    QSpacerItem* spacer173_2;
    QSpacerItem* spacer174_2;
    Q3VBoxLayout* layout400_2;
    Q3HBoxLayout* layout399_2;
    QSpacerItem* spacer172_2;
    Q3GridLayout* bgQSTypeLayout;
    Q3GridLayout* gbQConditionLayout;
    Q3GridLayout* wsCategoryConditonLayout;
    QSpacerItem* spacer148;
    QSpacerItem* spacer149;
    Q3VBoxLayout* layout402_2;
    Q3HBoxLayout* layout388_2;
    Q3HBoxLayout* layout394_2;
    QSpacerItem* spacer168_2;
    Q3VBoxLayout* layout390_2;
    Q3GridLayout* wsAlumniConditionLayout;
    Q3GridLayout* wsAccurateConditionLayout;
    QSpacerItem* spacer151;
    QSpacerItem* spacer152;
    Q3HBoxLayout* layout306;
    Q3GridLayout* wsQSResultPageLayout;
    Q3VBoxLayout* layout49;
    Q3HBoxLayout* layout96_3;
    QSpacerItem* spacer46_2;
    Q3HBoxLayout* layout46_3;
    QSpacerItem* spacer51;
    Q3HBoxLayout* layout195;
    QSpacerItem* spacer38;
    QSpacerItem* spacer40;

protected slots:
    virtual void languageChange();

};

#endif // EVASEARCHUIBASE_H
