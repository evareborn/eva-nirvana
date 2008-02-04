/****************************************************************************
** Form interface generated from reading ui file 'evauserinfowidget.ui'
**
** Created: 三 11月 23 19:45:10 2005
**      by: The User Interface Compiler ()
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef EVAUSERINFOWIDGET_H
#define EVAUSERINFOWIDGET_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QTextEdit;

class EvaUserInfoWidget : public QWidget
{
    Q_OBJECT

public:
    EvaUserInfoWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EvaUserInfoWidget();

    QLabel* lblBasicInfo;
    QLabel* lblRealName;
    QLabel* lblNickName;
    QLabel* lblAge;
    QLabel* lblArea;
    QLineEdit* leRealName;
    QLineEdit* leNickName;
    QLineEdit* leAge;
    QLineEdit* leArea;
    QLabel* lblNumber;
    QLabel* lblLevel;
    QLabel* lblSex;
    QLabel* lblProvince;
    QLineEdit* leNumber;
    QLabel* lblLevelDisplay;
    QLineEdit* leSex;
    QLabel* lblFace;
    QLineEdit* leProvince;
    QLabel* lblCity;
    QLineEdit* leCity;
    QTextEdit* teSignature;
    QLabel* lblSignature;
    QLineEdit* lePhone;
    QLineEdit* leHomePage;
    QLabel* lblMobile;
    QLineEdit* leMobile;
    QLineEdit* leOccupation;
    QLineEdit* leBlood;
    QLabel* lblBlood;
    QLabel* lblOccupation;
    QLineEdit* leZodiac;
    QLineEdit* leHoroscape;
    QLineEdit* leSchool;
    QLineEdit* leEmail;
    QLineEdit* leAddress;
    QLineEdit* leZipCode;
    QLabel* lblZodiac;
    QLabel* lblHoroscape;
    QLabel* lblSchool;
    QLabel* lblEmail;
    QLabel* lblAddress;
    QLabel* lblZipCode;
    QLabel* lblPhone;
    QLabel* lblHomePage;
    QLabel* lblDetails;
    QLabel* lblDescription;
    QTextEdit* teAboutMe;
    QLabel* lblAboutMe;

protected:
    QGridLayout* EvaUserInfoWidgetLayout;
    QGridLayout* layout191;
    QSpacerItem* spacer18;
    QSpacerItem* spacer31;
    QVBoxLayout* layout189;
    QVBoxLayout* layout188;
    QHBoxLayout* layout187;
    QHBoxLayout* layout186;
    QVBoxLayout* layout39;
    QVBoxLayout* layout178;
    QHBoxLayout* layout163;
    QVBoxLayout* layout64;
    QVBoxLayout* layout162;
    QHBoxLayout* layout161;
    QVBoxLayout* layout86;
    QHBoxLayout* layout66;
    QHBoxLayout* layout65;
    QHBoxLayout* layout103;
    QSpacerItem* spacer17;
    QSpacerItem* spacer24;
    QHBoxLayout* layout151;
    QSpacerItem* spacer26;
    QHBoxLayout* layout156;
    QSpacerItem* spacer14_2;
    QGridLayout* layout99;
    QGridLayout* layout144;
    QGridLayout* layout143;
    QSpacerItem* spacer13_2;
    QGridLayout* layout142;
    QSpacerItem* spacer28;
    QSpacerItem* spacer29;
    QGridLayout* layout141;
    QHBoxLayout* layout139;
    QSpacerItem* spacer27;
    QGridLayout* layout138;
    QVBoxLayout* layout30;
    QVBoxLayout* layout51;
    QVBoxLayout* layout95;
    QGridLayout* layout94;
    QSpacerItem* spacer16;
    QSpacerItem* spacer14_2_2;

protected slots:
    virtual void languageChange();

};

#endif // EVAUSERINFOWIDGET_H
