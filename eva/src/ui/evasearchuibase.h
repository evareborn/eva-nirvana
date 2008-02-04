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

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QWidgetStack;
class QLabel;
class QButtonGroup;
class QRadioButton;
class QGroupBox;
class QLineEdit;
class QTable;
class QToolButton;
class QCheckBox;
class QComboBox;
class QPushButton;

class EvaSearchUIBase : public QDialog
{
    Q_OBJECT

public:
    EvaSearchUIBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~EvaSearchUIBase();

    QTabWidget* twSearchMain;
    QWidget* tabBasicSearch;
    QWidgetStack* wsBasicSearch;
    QWidget* wsBSPage;
    QLabel* lblSearchOptionTip;
    QButtonGroup* bgBSType;
    QRadioButton* rbCustomSearch;
    QRadioButton* rbSearchFrdCenter;
    QRadioButton* rbSearchOnline;
    QWidgetStack* wsBasicCondtion;
    QWidget* wsCustomSearchCondition;
    QGroupBox* gbCustomCondition;
    QLabel* lblQQNum;
    QLabel* lblNickName;
    QLineEdit* leQQNum;
    QLineEdit* leNickName;
    QWidget* wsFrdCenterSearchCondition;
    QGroupBox* gbFrdCenterCondition;
    QLabel* lblQQFrdTip;
    QLabel* lblOnlineNumbers;
    QLabel* lblOnlineNum;
    QWidget* wsBSResultPage;
    QLabel* lblBSResultTip;
    QTable* tbBSResult;
    QLabel* lblBSResultPage;
    QToolButton* tbBSAll;
    QToolButton* tbBSDetails;
    QToolButton* tbBSPrev;
    QToolButton* tbBSNext;
    QWidget* tabAdvancedSearch;
    QWidgetStack* wsAdvancedSearch;
    QWidget* wsASPage;
    QLabel* lblASTip;
    QCheckBox* chbOnlineUsers;
    QCheckBox* chbHaveCamera;
    QGroupBox* gbBasicCondition;
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
    QTable* tbASResult;
    QLabel* lblASResultPage;
    QToolButton* tbASAll;
    QToolButton* tbASDetails;
    QToolButton* tbASPrev;
    QToolButton* tbASNext;
    QWidget* tabQunSearch;
    QWidgetStack* wsQunSearch;
    QWidget* wsQSPage;
    QLabel* lblQSTip;
    QButtonGroup* bgQSType;
    QRadioButton* rbSearchAlumni;
    QRadioButton* rbAccuratelySearch;
    QRadioButton* rbSearchByCategory;
    QGroupBox* gbQCondition;
    QWidgetStack* wsQunCondition;
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
    QTable* tbQSResult;
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
    QGridLayout* EvaSearchUIBaseLayout;
    QGridLayout* layout127;
    QGridLayout* tabBasicSearchLayout;
    QGridLayout* wsBSPageLayout;
    QVBoxLayout* layout135;
    QVBoxLayout* layout105;
    QHBoxLayout* layout100;
    QSpacerItem* spacer54;
    QGridLayout* bgBSTypeLayout;
    QGridLayout* wsCustomSearchConditionLayout;
    QGridLayout* gbCustomConditionLayout;
    QSpacerItem* spacer119_2;
    QHBoxLayout* layout204_2;
    QVBoxLayout* layout188_2;
    QVBoxLayout* layout203_2;
    QGridLayout* wsFrdCenterSearchConditionLayout;
    QGridLayout* gbFrdCenterConditionLayout;
    QVBoxLayout* layout46;
    QSpacerItem* spacer27;
    QSpacerItem* spacer28;
    QHBoxLayout* layout316_2;
    QSpacerItem* spacer111;
    QGridLayout* wsBSResultPageLayout;
    QVBoxLayout* layout47;
    QHBoxLayout* layout96;
    QSpacerItem* spacer46;
    QHBoxLayout* layout46_2;
    QSpacerItem* spacer47;
    QGridLayout* tabAdvancedSearchLayout;
    QGridLayout* wsASPageLayout;
    QVBoxLayout* layout91;
    QSpacerItem* spacer40_2;
    QVBoxLayout* layout371;
    QHBoxLayout* layout370;
    QSpacerItem* spacer156;
    QSpacerItem* spacer157;
    QHBoxLayout* layout369;
    QSpacerItem* spacer155;
    QGridLayout* gbBasicConditionLayout;
    QSpacerItem* spacer159;
    QHBoxLayout* layout376;
    QVBoxLayout* layout372;
    QVBoxLayout* layout375;
    QHBoxLayout* layout373;
    QSpacerItem* spacer158;
    QVBoxLayout* wsASResultPageLayout;
    QVBoxLayout* layout47_2;
    QHBoxLayout* layout96_2;
    QSpacerItem* spacer48;
    QHBoxLayout* layout46_2_2;
    QSpacerItem* spacer50;
    QGridLayout* tabQunSearchLayout;
    QGridLayout* wsQSPageLayout;
    QVBoxLayout* layout203;
    QHBoxLayout* layout92;
    QSpacerItem* spacer173_2;
    QSpacerItem* spacer174_2;
    QVBoxLayout* layout400_2;
    QHBoxLayout* layout399_2;
    QSpacerItem* spacer172_2;
    QGridLayout* bgQSTypeLayout;
    QGridLayout* gbQConditionLayout;
    QGridLayout* wsCategoryConditonLayout;
    QSpacerItem* spacer148;
    QSpacerItem* spacer149;
    QVBoxLayout* layout402_2;
    QHBoxLayout* layout388_2;
    QHBoxLayout* layout394_2;
    QSpacerItem* spacer168_2;
    QVBoxLayout* layout390_2;
    QGridLayout* wsAlumniConditionLayout;
    QGridLayout* wsAccurateConditionLayout;
    QSpacerItem* spacer151;
    QSpacerItem* spacer152;
    QHBoxLayout* layout306;
    QGridLayout* wsQSResultPageLayout;
    QVBoxLayout* layout49;
    QHBoxLayout* layout96_3;
    QSpacerItem* spacer46_2;
    QHBoxLayout* layout46_3;
    QSpacerItem* spacer51;
    QHBoxLayout* layout195;
    QSpacerItem* spacer38;
    QSpacerItem* spacer40;

protected slots:
    virtual void languageChange();

};

#endif // EVASEARCHUIBASE_H
