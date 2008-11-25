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
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class Q3TextEdit;

class EvaUserInfoWidget : public QWidget
{
    Q_OBJECT

public:
    EvaUserInfoWidget( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
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
    Q3TextEdit* teSignature;
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
    Q3TextEdit* teAboutMe;
    QLabel* lblAboutMe;

protected:
    Q3GridLayout* EvaUserInfoWidgetLayout;
    Q3GridLayout* layout191;
    QSpacerItem* spacer18;
    QSpacerItem* spacer31;
    Q3VBoxLayout* layout189;
    Q3VBoxLayout* layout188;
    Q3HBoxLayout* layout187;
    Q3HBoxLayout* layout186;
    Q3VBoxLayout* layout39;
    Q3VBoxLayout* layout178;
    Q3HBoxLayout* layout163;
    Q3VBoxLayout* layout64;
    Q3VBoxLayout* layout162;
    Q3HBoxLayout* layout161;
    Q3VBoxLayout* layout86;
    Q3HBoxLayout* layout66;
    Q3HBoxLayout* layout65;
    Q3HBoxLayout* layout103;
    QSpacerItem* spacer17;
    QSpacerItem* spacer24;
    Q3HBoxLayout* layout151;
    QSpacerItem* spacer26;
    Q3HBoxLayout* layout156;
    QSpacerItem* spacer14_2;
    Q3GridLayout* layout99;
    Q3GridLayout* layout144;
    Q3GridLayout* layout143;
    QSpacerItem* spacer13_2;
    Q3GridLayout* layout142;
    QSpacerItem* spacer28;
    QSpacerItem* spacer29;
    Q3GridLayout* layout141;
    Q3HBoxLayout* layout139;
    QSpacerItem* spacer27;
    Q3GridLayout* layout138;
    Q3VBoxLayout* layout30;
    Q3VBoxLayout* layout51;
    Q3VBoxLayout* layout95;
    Q3GridLayout* layout94;
    QSpacerItem* spacer16;
    QSpacerItem* spacer14_2_2;

protected slots:
    virtual void languageChange();

};

#endif // EVAUSERINFOWIDGET_H
