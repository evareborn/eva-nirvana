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

#include <q3listview.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <QLabel>
#include <Q3GridLayout>
#include <Q3Frame>
#include <Q3VBoxLayout>
//X #include "mainlistfontsettinguibase.h"

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class Q3ListView;
class Q3ListViewItem;
class Q3WidgetStack;
class QWidget;
class QLabel;
class Q3TextEdit;
class QLineEdit;
class QComboBox;
class QPushButton;
class Q3GroupBox;
class QToolButton;
class QPushButton;
class Q3ButtonGroup;
class QRadioButton;
class QCheckBox;
class QSpinBox;
class QSlider;
class Q3Frame;
class Q3IconView;
//class MainListFontSettingUIBase;

class EvaSysSettingUIBase : public QDialog
{
    Q_OBJECT

public:
    EvaSysSettingUIBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~EvaSysSettingUIBase();

    Q3ListView* lvSettingItem;
    Q3WidgetStack* wsSetting;
    QWidget* wspUserInfo;
    QLabel* lblSignature;
    QLabel* lblLevel;
    QLabel* lblLevelDisplay;
    Q3TextEdit* teSignature;
    QLabel* lblAboutMe;
    Q3TextEdit* teAboutMe;
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
    Q3GroupBox* gbQQShow;
    QLabel* lblQQShow;
    QToolButton* tbtnHome;
    QToolButton* tbtnAlbum;
    QPushButton* kpbUpdateShow;
    QToolButton* tbtnShop;
    QWidget* wspContact;
    Q3GroupBox* gbContact;
    Q3ButtonGroup* buttonGroup1;
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

    Q3GroupBox* gbPassword;
    QPushButton* pbChangePassword;
    QPushButton* pbProtectPassword;

    Q3ButtonGroup* bgAuthen;
    QRadioButton* rbAuthNoNeed;
    QRadioButton* rbAuthNeed;
    QRadioButton* rbAuthReject;
    QRadioButton* rbAuthQuest;
    Q3Frame *fraQuestion;
    QLabel *lblQuestion;
    QLabel *lblAnswer;
    QComboBox *cbbQuestion;
    QLineEdit *leAnswer;

    QWidget* wspBasicSetting;
    Q3GroupBox* gbGeneralSetting;
    QCheckBox* chbOnlineTip;
    QCheckBox* chbShowOnlineUsers;
    QCheckBox* chbSound;
    QCheckBox* chbMessage;
    QCheckBox* chbSystemBroadcast;
    QCheckBox* chbSystemNews;

    QCheckBox* chbSendKey;
    QCheckBox* chbShowNickSmiley;
    QCheckBox* chbShowSigSeperate;
    Q3GroupBox* gbOtherSetting;
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
    QPushButton* kkbtnShortKey;
    QWidget* wspResource;
    Q3GroupBox* gbPath;
    QLabel* lblThemePath;
    QLineEdit* leThemePath;
    QPushButton* pbTheme;
    QLabel* lblSoundPath;
    QLineEdit* leSoundPath;
    QPushButton* pbSound;
    QWidget* wspReply;
    Q3GroupBox* gbQuickReply;
    QComboBox* cbbQuickNo;
    Q3TextEdit* teQuickMessage;
    QPushButton* pbQuickNew;
    QPushButton* pbQuickDelete;
    Q3GroupBox* gbAutoReply;
    QComboBox* cbbAutoNo;
    QCheckBox* chbAutoReply;
    Q3TextEdit* teAutoMessage;
    QPushButton* pbAutoNew;
    QPushButton* pbAutoDelete;
    QPushButton* pbFaceDefault;
    QPushButton* pbOk;
    QPushButton* pbApply;
    QPushButton* pbCancel;
    
    Q3IconView* ivFace;

//X     MainListFontSettingUIBase *wspMainFont;
protected:
    Q3GridLayout* EvaSysSettingUIBaseLayout;
    Q3VBoxLayout* layout57;
    Q3HBoxLayout* layout55;
    Q3GridLayout* wspUserInfoLayout;
    QSpacerItem* spacer37;
    Q3GridLayout* layout44;
    Q3GridLayout* layout43;
    Q3VBoxLayout* layout41;
    QSpacerItem* spacer15;
    Q3HBoxLayout* layout70;
    Q3HBoxLayout* layout61;
    QSpacerItem* spacer16;
    Q3HBoxLayout* layout73;
    Q3VBoxLayout* layout72;
    QSpacerItem* spacer17;
    Q3HBoxLayout* layout56;
    Q3HBoxLayout* layout55_2;
    Q3VBoxLayout* layout54;
    Q3VBoxLayout* layout53;
    Q3HBoxLayout* layout52;
    Q3HBoxLayout* layout68;
    Q3VBoxLayout* layout66;
    Q3VBoxLayout* layout67;
    Q3HBoxLayout* layout46;
    Q3VBoxLayout* layout45;
    Q3GridLayout* layout36;
    QSpacerItem* spacer13;
    QSpacerItem* spacer10;
    Q3GridLayout* wspQQShowLayout;
    QSpacerItem* spacer18;
    QSpacerItem* spacer17_2;
    Q3GridLayout* layout13;
    QSpacerItem* spacer6;
    Q3GridLayout* layout12;
    QSpacerItem* spacer5;
    QSpacerItem* spacer7;
    QSpacerItem* spacer8;
    Q3GridLayout* wspContactLayout;
    QSpacerItem* spacer29;
    QSpacerItem* spacer30;
    Q3GridLayout* layout48;
    QSpacerItem* spacer28;
    Q3GridLayout* buttonGroup1Layout;
    Q3HBoxLayout* layout54_2;
    Q3GridLayout* layout56_2;
    Q3GridLayout* layout57_2;
    Q3GridLayout* wspSecurityLayout;
    QSpacerItem* spacer32;
    QSpacerItem* spacer33;
    Q3VBoxLayout* layout53_2;

    Q3GridLayout* authButtonLayout;
    Q3VBoxLayout* layout49;
    Q3GridLayout* wspBasicSettingLayout;
    QSpacerItem* spacer15_2;
    QSpacerItem* spacer16_2;
    Q3VBoxLayout* layout48_2;
    Q3HBoxLayout* layout47;
    Q3VBoxLayout* layout45_2;
    Q3VBoxLayout* layout46_2;
    Q3VBoxLayout* layout52_3;
    Q3HBoxLayout* layout44_2;
    Q3HBoxLayout* layout_idt;
    Q3HBoxLayout* layout45_3;
    Q3HBoxLayout* layout51_2;
    QSpacerItem* spacer19;
    Q3GridLayout* wspResourceLayout;
    QSpacerItem* spacer19_2;
    QSpacerItem* spacer20_2;
    Q3VBoxLayout* layout59;
    Q3GridLayout* layout56_3;
    QSpacerItem* spacer20;
    Q3HBoxLayout* layout55_3;
    Q3GridLayout* layout57_3;
    QSpacerItem* spacer21;
    Q3HBoxLayout* layout101;
    Q3GridLayout* wspReplyLayout;
    QSpacerItem* spacer26;
    QSpacerItem* spacer27;
    Q3GridLayout* layout44_3;
    Q3VBoxLayout* layout43_2;
    Q3HBoxLayout* layout40;
    QSpacerItem* spacer24;
    Q3HBoxLayout* layout42;
    Q3VBoxLayout* layout41_2;
    QSpacerItem* spacer25;
    Q3VBoxLayout* layout39;
    Q3HBoxLayout* layout32;
    QSpacerItem* spacer21_2;
    Q3HBoxLayout* layout38;
    Q3VBoxLayout* layout37;
    QSpacerItem* spacer23;
    Q3HBoxLayout* layout54_3;
    QSpacerItem* spacer34;

    Q3VBoxLayout* layoutQuestLbl;
    Q3VBoxLayout* layoutQuestContents;
    Q3HBoxLayout* layoutHQuest;
    Q3VBoxLayout* layoutQuest;

    Q3VBoxLayout* layoutPwd;
    Q3VBoxLayout* layoutPwButton;
protected slots:
    virtual void languageChange();

};

class EvaSettingListItem : public Q3ListViewItem
{
public:
	EvaSettingListItem(  Q3ListView* parent, Q3ListViewItem* after, int index );
	EvaSettingListItem(  Q3ListViewItem* parent, Q3ListViewItem* after, int index );
	~EvaSettingListItem();
	
	int itemIndex;
};

#endif // EVASYSSETTINGUIBASE_H
