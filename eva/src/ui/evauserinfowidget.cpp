/****************************************************************************
** Form implementation generated from reading ui file 'evauserinfowidget.ui'
**
** Created: 三 11月 23 19:45:12 2005
**      by: The User Interface Compiler ()
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "evauserinfowidget.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a EvaUserInfoWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
EvaUserInfoWidget::EvaUserInfoWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "EvaUserInfoWidget" );
    setEnabled( TRUE );
    setMinimumSize( QSize( 483, 610 ) );
    setMaximumSize( QSize( 483, 610 ) );
    setBackgroundOrigin( QWidget::WidgetOrigin );
    EvaUserInfoWidgetLayout = new QGridLayout( this, 1, 1, 11, 6, "EvaUserInfoWidgetLayout"); 

    layout191 = new QGridLayout( 0, 1, 1, 0, 6, "layout191"); 
    spacer18 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout191->addItem( spacer18, 0, 1 );

    layout189 = new QVBoxLayout( 0, 0, 6, "layout189"); 

    layout188 = new QVBoxLayout( 0, 0, 15, "layout188"); 

    lblBasicInfo = new QLabel( this, "lblBasicInfo" );
    layout188->addWidget( lblBasicInfo );

    layout187 = new QHBoxLayout( 0, 0, 6, "layout187"); 

    layout186 = new QHBoxLayout( 0, 0, 6, "layout186"); 

    layout39 = new QVBoxLayout( 0, 0, 6, "layout39"); 

    lblRealName = new QLabel( this, "lblRealName" );
    layout39->addWidget( lblRealName );

    lblNickName = new QLabel( this, "lblNickName" );
    layout39->addWidget( lblNickName );

    lblAge = new QLabel( this, "lblAge" );
    layout39->addWidget( lblAge );

    lblArea = new QLabel( this, "lblArea" );
    layout39->addWidget( lblArea );
    layout186->addLayout( layout39 );

    layout178 = new QVBoxLayout( 0, 0, 6, "layout178"); 

    leRealName = new QLineEdit( this, "leRealName" );
    leRealName->setEnabled( TRUE );
    leRealName->setMinimumSize( QSize( 100, 0 ) );
    leRealName->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leRealName->setFrameShape( QLineEdit::Box );
    leRealName->setFrameShadow( QLineEdit::Plain );
    leRealName->setLineWidth( 1 );
    leRealName->setReadOnly( TRUE );
    layout178->addWidget( leRealName );

    leNickName = new QLineEdit( this, "leNickName" );
    leNickName->setEnabled( TRUE );
    leNickName->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leNickName->setFrameShape( QLineEdit::Box );
    leNickName->setFrameShadow( QLineEdit::Plain );
    leNickName->setLineWidth( 1 );
    leNickName->setReadOnly( TRUE );
    layout178->addWidget( leNickName );

    leAge = new QLineEdit( this, "leAge" );
    leAge->setEnabled( TRUE );
    leAge->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leAge->setFrameShape( QLineEdit::Box );
    leAge->setFrameShadow( QLineEdit::Plain );
    leAge->setLineWidth( 1 );
    leAge->setReadOnly( TRUE );
    layout178->addWidget( leAge );

    leArea = new QLineEdit( this, "leArea" );
    leArea->setEnabled( TRUE );
    leArea->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leArea->setFrameShape( QLineEdit::Box );
    leArea->setFrameShadow( QLineEdit::Plain );
    leArea->setLineWidth( 1 );
    leArea->setReadOnly( TRUE );
    layout178->addWidget( leArea );
    layout186->addLayout( layout178 );
    layout187->addLayout( layout186 );

    layout163 = new QHBoxLayout( 0, 0, 6, "layout163"); 

    layout64 = new QVBoxLayout( 0, 0, 6, "layout64"); 

    lblNumber = new QLabel( this, "lblNumber" );
    layout64->addWidget( lblNumber );

    lblLevel = new QLabel( this, "lblLevel" );
    layout64->addWidget( lblLevel );

    lblSex = new QLabel( this, "lblSex" );
    layout64->addWidget( lblSex );

    lblProvince = new QLabel( this, "lblProvince" );
    layout64->addWidget( lblProvince );
    layout163->addLayout( layout64 );

    layout162 = new QVBoxLayout( 0, 0, 6, "layout162"); 

    layout161 = new QHBoxLayout( 0, 0, 6, "layout161"); 

    layout86 = new QVBoxLayout( 0, 0, 6, "layout86"); 

    layout66 = new QHBoxLayout( 0, 0, 6, "layout66"); 

    leNumber = new QLineEdit( this, "leNumber" );
    leNumber->setEnabled( TRUE );
    leNumber->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leNumber->setFrameShape( QLineEdit::Box );
    leNumber->setFrameShadow( QLineEdit::Plain );
    leNumber->setLineWidth( 1 );
    leNumber->setReadOnly( TRUE );
    layout66->addWidget( leNumber );
    layout86->addLayout( layout66 );

    lblLevelDisplay = new QLabel( this, "lblLevelDisplay" );
    lblLevelDisplay->setEnabled( TRUE );
    layout86->addWidget( lblLevelDisplay );

    layout65 = new QHBoxLayout( 0, 0, 6, "layout65"); 

    leSex = new QLineEdit( this, "leSex" );
    leSex->setEnabled( TRUE );
    leSex->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leSex->setFrameShape( QLineEdit::Box );
    leSex->setFrameShadow( QLineEdit::Plain );
    leSex->setLineWidth( 1 );
    leSex->setReadOnly( TRUE );
    layout65->addWidget( leSex );
    layout86->addLayout( layout65 );
    layout161->addLayout( layout86 );

    layout103 = new QHBoxLayout( 0, 0, 6, "layout103"); 
    spacer17 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout103->addItem( spacer17 );

    lblFace = new QLabel( this, "lblFace" );
    lblFace->setMinimumSize( QSize( 46, 46 ) );
    lblFace->setMaximumSize( QSize( 46, 46 ) );
    lblFace->setFrameShape( QLabel::Box );
    lblFace->setAlignment( int( QLabel::AlignCenter ) );
    layout103->addWidget( lblFace );
    spacer24 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout103->addItem( spacer24 );
    layout161->addLayout( layout103 );
    layout162->addLayout( layout161 );

    layout151 = new QHBoxLayout( 0, 0, 6, "layout151"); 

    leProvince = new QLineEdit( this, "leProvince" );
    leProvince->setEnabled( TRUE );
    leProvince->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leProvince->setFrameShape( QLineEdit::Box );
    leProvince->setFrameShadow( QLineEdit::Plain );
    leProvince->setLineWidth( 1 );
    leProvince->setReadOnly( TRUE );
    layout151->addWidget( leProvince );

    lblCity = new QLabel( this, "lblCity" );
    layout151->addWidget( lblCity );

    leCity = new QLineEdit( this, "leCity" );
    leCity->setEnabled( TRUE );
    leCity->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leCity->setFrameShape( QLineEdit::Box );
    leCity->setFrameShadow( QLineEdit::Plain );
    leCity->setLineWidth( 1 );
    leCity->setReadOnly( TRUE );
    layout151->addWidget( leCity );
    spacer26 = new QSpacerItem( 50, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout151->addItem( spacer26 );
    layout162->addLayout( layout151 );
    layout163->addLayout( layout162 );
    layout187->addLayout( layout163 );
    layout188->addLayout( layout187 );

    layout156 = new QHBoxLayout( 0, 0, 6, "layout156"); 

    layout99 = new QGridLayout( 0, 1, 1, 0, 6, "layout99"); 

    teSignature = new QTextEdit( this, "teSignature" );
    teSignature->setEnabled( TRUE );
    teSignature->setMinimumSize( QSize( 0, 50 ) );
    teSignature->setMaximumSize( QSize( 32767, 50 ) );
    teSignature->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    teSignature->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    teSignature->setFocusPolicy( QTextEdit::NoFocus );
    teSignature->setFrameShape( QTextEdit::Box );
    teSignature->setFrameShadow( QTextEdit::Plain );
    teSignature->setLineWidth( 1 );
    teSignature->setHScrollBarMode( QTextEdit::AlwaysOff );
    teSignature->setWordWrap( QTextEdit::WidgetWidth );
    teSignature->setReadOnly( TRUE );

    layout99->addMultiCellWidget( teSignature, 0, 1, 1, 1 );

    lblSignature = new QLabel( this, "lblSignature" );

    layout99->addWidget( lblSignature, 0, 0 );
    layout156->addLayout( layout99 );
    spacer14_2 = new QSpacerItem( 71, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout156->addItem( spacer14_2 );
    layout188->addLayout( layout156 );
    layout189->addLayout( layout188 );

    layout144 = new QGridLayout( 0, 1, 1, 0, 15, "layout144"); 

    layout143 = new QGridLayout( 0, 1, 1, 0, 6, "layout143"); 

    layout142 = new QGridLayout( 0, 1, 1, 0, 6, "layout142"); 

    lePhone = new QLineEdit( this, "lePhone" );
    lePhone->setEnabled( TRUE );
    lePhone->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    lePhone->setFrameShape( QLineEdit::Box );
    lePhone->setFrameShadow( QLineEdit::Plain );
    lePhone->setLineWidth( 1 );
    lePhone->setReadOnly( TRUE );

    layout142->addWidget( lePhone, 0, 0 );

    leHomePage = new QLineEdit( this, "leHomePage" );
    leHomePage->setEnabled( TRUE );
    leHomePage->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leHomePage->setFrameShape( QLineEdit::Box );
    leHomePage->setFrameShadow( QLineEdit::Plain );
    leHomePage->setLineWidth( 1 );
    leHomePage->setReadOnly( TRUE );

    layout142->addMultiCellWidget( leHomePage, 1, 1, 0, 2 );

    lblMobile = new QLabel( this, "lblMobile" );

    layout142->addWidget( lblMobile, 0, 1 );

    leMobile = new QLineEdit( this, "leMobile" );
    leMobile->setEnabled( TRUE );
    leMobile->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leMobile->setFrameShape( QLineEdit::Box );
    leMobile->setFrameShadow( QLineEdit::Plain );
    leMobile->setLineWidth( 1 );
    leMobile->setReadOnly( TRUE );

    layout142->addWidget( leMobile, 0, 2 );
    spacer28 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout142->addItem( spacer28, 0, 3 );
    spacer29 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout142->addItem( spacer29, 1, 3 );

    layout143->addMultiCellLayout( layout142, 2, 2, 0, 1 );
    spacer13_2 = new QSpacerItem( 180, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout143->addItem( spacer13_2, 1, 1 );

    layout141 = new QGridLayout( 0, 1, 1, 0, 6, "layout141"); 

    layout139 = new QHBoxLayout( 0, 0, 6, "layout139"); 

    layout138 = new QGridLayout( 0, 1, 1, 0, 6, "layout138"); 

    leOccupation = new QLineEdit( this, "leOccupation" );
    leOccupation->setEnabled( TRUE );
    leOccupation->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leOccupation->setFrameShape( QLineEdit::Box );
    leOccupation->setFrameShadow( QLineEdit::Plain );
    leOccupation->setLineWidth( 1 );
    leOccupation->setReadOnly( TRUE );

    layout138->addWidget( leOccupation, 0, 1 );

    leBlood = new QLineEdit( this, "leBlood" );
    leBlood->setEnabled( TRUE );
    leBlood->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leBlood->setFrameShape( QLineEdit::Box );
    leBlood->setFrameShadow( QLineEdit::Plain );
    leBlood->setLineWidth( 1 );
    leBlood->setReadOnly( TRUE );

    layout138->addWidget( leBlood, 1, 1 );

    lblBlood = new QLabel( this, "lblBlood" );

    layout138->addWidget( lblBlood, 1, 0 );

    lblOccupation = new QLabel( this, "lblOccupation" );

    layout138->addWidget( lblOccupation, 0, 0 );
    layout139->addLayout( layout138 );
    spacer27 = new QSpacerItem( 30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout139->addItem( spacer27 );

    layout141->addMultiCellLayout( layout139, 0, 1, 1, 1 );

    leZodiac = new QLineEdit( this, "leZodiac" );
    leZodiac->setEnabled( TRUE );
    leZodiac->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leZodiac->setFrameShape( QLineEdit::Panel );
    leZodiac->setFrameShadow( QLineEdit::Plain );
    leZodiac->setLineWidth( 1 );
    leZodiac->setReadOnly( TRUE );

    layout141->addWidget( leZodiac, 0, 0 );

    leHoroscape = new QLineEdit( this, "leHoroscape" );
    leHoroscape->setEnabled( TRUE );
    leHoroscape->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leHoroscape->setFrameShape( QLineEdit::Box );
    leHoroscape->setFrameShadow( QLineEdit::Plain );
    leHoroscape->setLineWidth( 1 );
    leHoroscape->setReadOnly( TRUE );

    layout141->addWidget( leHoroscape, 1, 0 );

    layout143->addMultiCellLayout( layout141, 0, 0, 0, 1 );

    layout30 = new QVBoxLayout( 0, 0, 6, "layout30"); 

    leSchool = new QLineEdit( this, "leSchool" );
    leSchool->setEnabled( TRUE );
    leSchool->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leSchool->setFrameShape( QLineEdit::Box );
    leSchool->setFrameShadow( QLineEdit::Plain );
    leSchool->setLineWidth( 1 );
    leSchool->setReadOnly( TRUE );
    layout30->addWidget( leSchool );

    leEmail = new QLineEdit( this, "leEmail" );
    leEmail->setEnabled( TRUE );
    leEmail->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leEmail->setFrameShape( QLineEdit::Box );
    leEmail->setFrameShadow( QLineEdit::Plain );
    leEmail->setLineWidth( 1 );
    leEmail->setReadOnly( TRUE );
    layout30->addWidget( leEmail );

    leAddress = new QLineEdit( this, "leAddress" );
    leAddress->setEnabled( TRUE );
    leAddress->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leAddress->setFrameShape( QLineEdit::Box );
    leAddress->setFrameShadow( QLineEdit::Plain );
    leAddress->setLineWidth( 1 );
    leAddress->setReadOnly( TRUE );
    layout30->addWidget( leAddress );

    leZipCode = new QLineEdit( this, "leZipCode" );
    leZipCode->setEnabled( TRUE );
    leZipCode->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    leZipCode->setFrameShape( QLineEdit::Box );
    leZipCode->setFrameShadow( QLineEdit::Plain );
    leZipCode->setLineWidth( 1 );
    leZipCode->setReadOnly( TRUE );
    layout30->addWidget( leZipCode );

    layout143->addLayout( layout30, 1, 0 );

    layout144->addLayout( layout143, 1, 1 );

    layout51 = new QVBoxLayout( 0, 0, 6, "layout51"); 

    lblZodiac = new QLabel( this, "lblZodiac" );
    layout51->addWidget( lblZodiac );

    lblHoroscape = new QLabel( this, "lblHoroscape" );
    layout51->addWidget( lblHoroscape );

    lblSchool = new QLabel( this, "lblSchool" );
    layout51->addWidget( lblSchool );

    lblEmail = new QLabel( this, "lblEmail" );
    layout51->addWidget( lblEmail );

    lblAddress = new QLabel( this, "lblAddress" );
    layout51->addWidget( lblAddress );

    lblZipCode = new QLabel( this, "lblZipCode" );
    layout51->addWidget( lblZipCode );

    lblPhone = new QLabel( this, "lblPhone" );
    layout51->addWidget( lblPhone );

    lblHomePage = new QLabel( this, "lblHomePage" );
    layout51->addWidget( lblHomePage );

    layout144->addLayout( layout51, 1, 0 );

    lblDetails = new QLabel( this, "lblDetails" );

    layout144->addMultiCellWidget( lblDetails, 0, 0, 0, 1 );
    layout189->addLayout( layout144 );

    layout95 = new QVBoxLayout( 0, 0, 15, "layout95"); 

    lblDescription = new QLabel( this, "lblDescription" );
    layout95->addWidget( lblDescription );

    layout94 = new QGridLayout( 0, 1, 1, 0, 6, "layout94"); 
    spacer16 = new QSpacerItem( 20, 77, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout94->addItem( spacer16, 1, 0 );
    spacer14_2_2 = new QSpacerItem( 71, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout94->addItem( spacer14_2_2, 1, 2 );

    teAboutMe = new QTextEdit( this, "teAboutMe" );
    teAboutMe->setEnabled( TRUE );
    teAboutMe->setMinimumSize( QSize( 0, 0 ) );
    teAboutMe->setMaximumSize( QSize( 32767, 100 ) );
    teAboutMe->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    teAboutMe->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    teAboutMe->setFocusPolicy( QTextEdit::NoFocus );
    teAboutMe->setFrameShape( QTextEdit::Box );
    teAboutMe->setFrameShadow( QTextEdit::Plain );
    teAboutMe->setLineWidth( 1 );
    teAboutMe->setHScrollBarMode( QTextEdit::AlwaysOff );
    teAboutMe->setWordWrap( QTextEdit::WidgetWidth );
    teAboutMe->setReadOnly( TRUE );

    layout94->addMultiCellWidget( teAboutMe, 0, 1, 1, 1 );

    lblAboutMe = new QLabel( this, "lblAboutMe" );

    layout94->addWidget( lblAboutMe, 0, 0 );
    layout95->addLayout( layout94 );
    layout189->addLayout( layout95 );

    layout191->addLayout( layout189, 0, 0 );
    spacer31 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout191->addItem( spacer31, 1, 0 );

    EvaUserInfoWidgetLayout->addLayout( layout191, 0, 0 );
    languageChange();
    resize( QSize(483, 610).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // tab order
    setTabOrder( leRealName, leNickName );
    setTabOrder( leNickName, leAge );
    setTabOrder( leAge, leArea );
    setTabOrder( leArea, leNumber );
    setTabOrder( leNumber, leSex );
    setTabOrder( leSex, leProvince );
    setTabOrder( leProvince, leCity );

    // buddies
    lblRealName->setBuddy( leRealName );
    lblArea->setBuddy( leArea );
    lblNumber->setBuddy( leNumber );
    lblSex->setBuddy( leSex );
    lblProvince->setBuddy( leProvince );
    lblCity->setBuddy( leCity );
    lblMobile->setBuddy( leRealName );
    lblBlood->setBuddy( leRealName );
    lblOccupation->setBuddy( leRealName );
    lblZodiac->setBuddy( leRealName );
    lblHoroscape->setBuddy( leRealName );
    lblSchool->setBuddy( leRealName );
    lblEmail->setBuddy( leRealName );
    lblAddress->setBuddy( leRealName );
    lblZipCode->setBuddy( leRealName );
    lblPhone->setBuddy( leRealName );
    lblHomePage->setBuddy( leRealName );
}

/*
 *  Destroys the object and frees any allocated resources
 */
EvaUserInfoWidget::~EvaUserInfoWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaUserInfoWidget::languageChange()
{
    setCaption( tr( "UserInfoWidget" ) );
    lblBasicInfo->setText( tr( "Basic Info" ) );
    lblRealName->setText( tr( "Real Name:" ) );
    lblNickName->setText( tr( "Nick Name:" ) );
    lblAge->setText( tr( "Age:" ) );
    lblArea->setText( tr( "Region:" ) );
    leRealName->setText( QString::null );
    leNickName->setText( QString::null );
    leAge->setText( QString::null );
    leArea->setText( QString::null );
    lblNumber->setText( tr( "QQ Num:" ) );
    lblLevel->setText( tr( "QQ Level:" ) );
    lblSex->setText( tr( "Gender:" ) );
    lblProvince->setText( tr( "Province:" ) );
    leNumber->setText( QString::null );
    lblLevelDisplay->setText( QString::null );
    leSex->setText( QString::null );
    lblFace->setText( QString::null );
    leProvince->setText( QString::null );
    lblCity->setText( tr( "City:" ) );
    leCity->setText( QString::null );
    teSignature->setText( QString::null );
    lblSignature->setText( tr( "Signature:" ) );
    lePhone->setText( QString::null );
    leHomePage->setText( QString::null );
    lblMobile->setText( tr( "Mobile:" ) );
    leMobile->setText( QString::null );
    leOccupation->setText( QString::null );
    leBlood->setText( QString::null );
    lblBlood->setText( tr( "Blood Type:" ) );
    lblOccupation->setText( tr( "Occupation:" ) );
    leZodiac->setText( QString::null );
    leHoroscape->setText( QString::null );
    leSchool->setText( QString::null );
    leEmail->setText( QString::null );
    leAddress->setText( QString::null );
    leZipCode->setText( QString::null );
    lblZodiac->setText( tr( "Chinese Zodiac:" ) );
    lblHoroscape->setText( tr( "Horoscope:" ) );
    lblSchool->setText( tr( "School:" ) );
    lblEmail->setText( tr( "Email:" ) );
    lblAddress->setText( tr( "Address:" ) );
    lblZipCode->setText( tr( "Postal Code:" ) );
    lblPhone->setText( tr( "Phone:" ) );
    lblHomePage->setText( tr( "Home Page:" ) );
    lblDetails->setText( tr( "Details" ) );
    lblDescription->setText( tr( "Description" ) );
    teAboutMe->setText( QString::null );
    lblAboutMe->setText( tr( "About Me:" ) );
}

