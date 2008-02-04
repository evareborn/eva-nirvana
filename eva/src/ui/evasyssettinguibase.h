/***************************************************************************
 *   Copyright (C) 2005 by tlmcasper                                          *
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

#ifndef EVASYSSETTINGUIBASE_H
#define EVASYSSETTINGUIBASE_H

#include <qvariant.h>
#include <qdialog.h>

#include <qlistview.h>
#include "mainlistfontsettinguibase.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListView;
class QListViewItem;
class QWidgetStack;
class QWidget;
class QLabel;
class QTextEdit;
class QLineEdit;
class QComboBox;
class QPushButton;
class QGroupBox;
class QToolButton;
class KPushButton;
class QButtonGroup;
class QRadioButton;
class QCheckBox;
class QSpinBox;
class QSlider;
class KKeyButton;
class QFrame;
class QIconView;
//class MainListFontSettingUIBase;

class EvaSysSettingUIBase : public QDialog
{
    Q_OBJECT

public:
    EvaSysSettingUIBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~EvaSysSettingUIBase();

    QListView* lvSettingItem;
    QWidgetStack* wsSetting;
    QWidget* wspUserInfo;
    QLabel* lblSignature;
    QLabel* lblLevel;
    QLabel* lblLevelDisplay;
    QTextEdit* teSignature;
    QLabel* lblAboutMe;
    QTextEdit* teAboutMe;
    QLabel* lblRealName;
    QLabel* lblSchool;
    QLabel* lblOccupation;
    QLabel* lblHomePage;
    QLineEdit* leRealName;
    QLabel* lblSex;
    QComboBox* cbSex;
    QLineEdit* leSchool;
    QComboBox* cbOccupation;
    QLineEdit* leHomePage;
    QLabel* lblAge;
    QLabel* lblZodiac;
    QLabel* lblHoroscape;
    QLabel* lblBlood;
    QLineEdit* leAge;
    QComboBox* cbZodiac;
    QComboBox* cbHoroscape;
    QComboBox* cbBlood;
    QLabel* lblNumber;
    QLabel* lblNickName;
    QLineEdit* leNickName;
    QLineEdit* leNumber;
    QLabel* lblFace;
    QPushButton* pbChangeFace;
    QWidget* wspQQShow;
    QGroupBox* gbQQShow;
    QLabel* lblQQShow;
    QToolButton* tbtnHome;
    QToolButton* tbtnAlbum;
    KPushButton* kpbUpdateShow;
    QToolButton* tbtnShop;
    QWidget* wspContact;
    QGroupBox* gbContact;
    QButtonGroup* buttonGroup1;
    QLabel* lblPrompt;
    QRadioButton* rbPublic;
    QRadioButton* rbProtect;
    QRadioButton* rbPrivate;
    QLineEdit* leMobile;
    QLineEdit* leEmail;
    QLabel* lblPhone;
    QLabel* lblAddress;
    QLabel* lblEmail;
    QLineEdit* leAddress;
    QLabel* lblMobile;
    QLineEdit* lePhone;
    QLabel* lblCity;
    QComboBox* cbArea;
    QComboBox* cbCity;
    QLabel* lblProvince;
    QComboBox* cbProvince;
    QLabel* lblZipCode;
    QLineEdit* leZipCode;
    QLabel* lblArea;
    QWidget* wspSecurity;

    QGroupBox* gbPassword;
    QPushButton* pbChangePassword;
    QPushButton* pbProtectPassword;

    QButtonGroup* bgAuthen;
    QRadioButton* rbAuthNoNeed;
    QRadioButton* rbAuthNeed;
    QRadioButton* rbAuthReject;
    QRadioButton* rbAuthQuest;
    QFrame *fraQuestion;
    QLabel *lblQuestion;
    QLabel *lblAnswer;
    QComboBox *cbbQuestion;
    QLineEdit *leAnswer;

    QWidget* wspBasicSetting;
    QGroupBox* gbGeneralSetting;
    QCheckBox* chbOnlineTip;
    QCheckBox* chbShowOnlineUsers;
    QCheckBox* chbSound;
    QCheckBox* chbMessage;
    QCheckBox* chbSystemBroadcast;
    QCheckBox* chbSystemNews;

    QCheckBox* chbSendKey;
    QCheckBox* chbShowNickSmiley;
    QCheckBox* chbShowSigSeperate;
    QGroupBox* gbOtherSetting;
    QLabel* lblPageSize;
    QSpinBox* sbPageSize;
    QLabel* lblPageSizeEnd;
    QLabel* lblIdleTime;
    QSpinBox* sbIdleTime;
    QLabel* lblIdleTimeEnd;
    QLabel* lblFaceSize;
    QLabel* lblFaceSmall;
    QSlider* sliderFaceSize;
    QLabel* lblFaceBig;
    QLabel* lblShortKey;
    KKeyButton* kkbtnShortKey;
    QWidget* wspResource;
    QGroupBox* gbPath;
    QLabel* lblThemePath;
    QLineEdit* leThemePath;
    QPushButton* pbTheme;
    QLabel* lblSoundPath;
    QLineEdit* leSoundPath;
    QPushButton* pbSound;
    QWidget* wspReply;
    QGroupBox* gbQuickReply;
    QComboBox* cbbQuickNo;
    QTextEdit* teQuickMessage;
    QPushButton* pbQuickNew;
    QPushButton* pbQuickDelete;
    QGroupBox* gbAutoReply;
    QComboBox* cbbAutoNo;
    QCheckBox* chbAutoReply;
    QTextEdit* teAutoMessage;
    QPushButton* pbAutoNew;
    QPushButton* pbAutoDelete;
    QPushButton* pbFaceDefault;
    QPushButton* pbOk;
    QPushButton* pbApply;
    QPushButton* pbCancel;
    
    QIconView* ivFace;

    MainListFontSettingUIBase *wspMainFont;
protected:
    QGridLayout* EvaSysSettingUIBaseLayout;
    QVBoxLayout* layout57;
    QHBoxLayout* layout55;
    QGridLayout* wspUserInfoLayout;
    QSpacerItem* spacer37;
    QGridLayout* layout44;
    QGridLayout* layout43;
    QVBoxLayout* layout41;
    QSpacerItem* spacer15;
    QHBoxLayout* layout70;
    QHBoxLayout* layout61;
    QSpacerItem* spacer16;
    QHBoxLayout* layout73;
    QVBoxLayout* layout72;
    QSpacerItem* spacer17;
    QHBoxLayout* layout56;
    QHBoxLayout* layout55_2;
    QVBoxLayout* layout54;
    QVBoxLayout* layout53;
    QHBoxLayout* layout52;
    QHBoxLayout* layout68;
    QVBoxLayout* layout66;
    QVBoxLayout* layout67;
    QHBoxLayout* layout46;
    QVBoxLayout* layout45;
    QGridLayout* layout36;
    QSpacerItem* spacer13;
    QSpacerItem* spacer10;
    QGridLayout* wspQQShowLayout;
    QSpacerItem* spacer18;
    QSpacerItem* spacer17_2;
    QGridLayout* layout13;
    QSpacerItem* spacer6;
    QGridLayout* layout12;
    QSpacerItem* spacer5;
    QSpacerItem* spacer7;
    QSpacerItem* spacer8;
    QGridLayout* wspContactLayout;
    QSpacerItem* spacer29;
    QSpacerItem* spacer30;
    QGridLayout* layout48;
    QSpacerItem* spacer28;
    QGridLayout* buttonGroup1Layout;
    QHBoxLayout* layout54_2;
    QGridLayout* layout56_2;
    QGridLayout* layout57_2;
    QGridLayout* wspSecurityLayout;
    QSpacerItem* spacer32;
    QSpacerItem* spacer33;
    QVBoxLayout* layout53_2;

    QGridLayout* authButtonLayout;
    QVBoxLayout* layout49;
    QGridLayout* wspBasicSettingLayout;
    QSpacerItem* spacer15_2;
    QSpacerItem* spacer16_2;
    QVBoxLayout* layout48_2;
    QHBoxLayout* layout47;
    QVBoxLayout* layout45_2;
    QVBoxLayout* layout46_2;
    QVBoxLayout* layout52_3;
    QHBoxLayout* layout44_2;
    QHBoxLayout* layout_idt;
    QHBoxLayout* layout45_3;
    QHBoxLayout* layout51_2;
    QSpacerItem* spacer19;
    QGridLayout* wspResourceLayout;
    QSpacerItem* spacer19_2;
    QSpacerItem* spacer20_2;
    QVBoxLayout* layout59;
    QGridLayout* layout56_3;
    QSpacerItem* spacer20;
    QHBoxLayout* layout55_3;
    QGridLayout* layout57_3;
    QSpacerItem* spacer21;
    QHBoxLayout* layout101;
    QGridLayout* wspReplyLayout;
    QSpacerItem* spacer26;
    QSpacerItem* spacer27;
    QGridLayout* layout44_3;
    QVBoxLayout* layout43_2;
    QHBoxLayout* layout40;
    QSpacerItem* spacer24;
    QHBoxLayout* layout42;
    QVBoxLayout* layout41_2;
    QSpacerItem* spacer25;
    QVBoxLayout* layout39;
    QHBoxLayout* layout32;
    QSpacerItem* spacer21_2;
    QHBoxLayout* layout38;
    QVBoxLayout* layout37;
    QSpacerItem* spacer23;
    QHBoxLayout* layout54_3;
    QSpacerItem* spacer34;

    QVBoxLayout* layoutQuestLbl;
    QVBoxLayout* layoutQuestContents;
    QHBoxLayout* layoutHQuest;
    QVBoxLayout* layoutQuest;

    QVBoxLayout* layoutPwd;
    QVBoxLayout* layoutPwButton;
protected slots:
    virtual void languageChange();

};

class EvaSettingListItem : public QListViewItem
{
public:
	EvaSettingListItem(  QListView* parent, QListViewItem* after, int index );
	EvaSettingListItem(  QListViewItem* parent, QListViewItem* after, int index );
	~EvaSettingListItem();
	
	int itemIndex;
};

#endif // EVASYSSETTINGUIBASE_H
