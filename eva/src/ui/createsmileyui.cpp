/****************************************************************************
** Form implementation generated from reading ui file 'createsmileyui.ui'
**
** Created: 五  9月 1 00:01:38 2006
**      by: The User Interface Compiler ($Id: createsmileyui.cpp,v 1.3 2006/09/05 12:18:48 yunfan Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "createsmileyui.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qrect.h>
#include <qwhatsthis.h>
#include <klocale.h>
#include <kapplication.h>

/*
 *  Constructs a CreateSmileyUI as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
CreateSmileyUI::CreateSmileyUI( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "CreateSmileyUI" );
    CreateSmileyUILayout = new QGridLayout( this, 1, 1, 11, 6, "CreateSmileyUILayout"); 

    layout21 = new QVBoxLayout( 0, 0, 6, "layout21"); 

    lblTop = new QLabel( this, "lblTop" );
    lblTop->setPaletteForegroundColor( QColor( 0, 51, 153 ) );
    layout21->addWidget( lblTop );

    lblStep1 = new QLabel( this, "lblStep1" );
    lblStep1->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
    layout21->addWidget( lblStep1 );

    layout16 = new QHBoxLayout( 0, 0, 6, "layout16"); 
    spacer6 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout16->addItem( spacer6 );

    lblSelect = new QLabel( this, "lblSelect" );
    lblSelect->setPaletteForegroundColor( QColor( 102, 0, 153 ) );
    lblSelect->setFrameShape( QLabel::Box );
    layout16->addWidget( lblSelect );

    btnSelect = new QPushButton( this, "btnSelect" );
    layout16->addWidget( btnSelect );
    layout21->addLayout( layout16 );

    lblStep2 = new QLabel( this, "lblStep2" );
    layout21->addWidget( lblStep2 );

    layout19 = new QHBoxLayout( 0, 0, 6, "layout19"); 
    spacer6_3 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout19->addItem( spacer6_3 );

    leShortcut = new QLineEdit( this, "leShortcut" );
    leShortcut->setMaxLength( 6 );
    layout19->addWidget( leShortcut );
    spacer9 = new QSpacerItem( 31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout19->addItem( spacer9 );

    lblStep2_2 = new QLabel( this, "lblStep2_2" );
    layout19->addWidget( lblStep2_2 );

    cbbGroup = new QComboBox( FALSE, this, "cbbGroup" );
    layout19->addWidget( cbbGroup );
    layout21->addLayout( layout19 );

    lblStep3 = new QLabel( this, "lblStep3" );
    layout21->addWidget( lblStep3 );

    layout17 = new QHBoxLayout( 0, 0, 6, "layout17"); 
    spacer6_2 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout17->addItem( spacer6_2 );

    leTip = new QLineEdit( this, "leTip" );
    layout17->addWidget( leTip );
    layout21->addLayout( layout17 );

    layout20 = new QHBoxLayout( 0, 0, 6, "layout20"); 
    spacer6_2_2 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout20->addItem( spacer6_2_2 );

    lblCommends = new QLabel( this, "lblCommends" );
    lblCommends->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
    layout20->addWidget( lblCommends );
    layout21->addLayout( layout20 );

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 
    spacer5 = new QSpacerItem( 141, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout14->addItem( spacer5 );

    btnOK = new QPushButton( this, "btnOK" );
    layout14->addWidget( btnOK );

    btnCancel = new QPushButton( this, "btnCancel" );
    layout14->addWidget( btnCancel );
    layout21->addLayout( layout14 );

    CreateSmileyUILayout->addLayout( layout21, 0, 0 );
    languageChange();
    resize( QSize(445, 316).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    QRect scr = KApplication::desktop()->screenGeometry();
    move(scr.center()-rect().center());
}

/*
 *  Destroys the object and frees any allocated resources
 */
CreateSmileyUI::~CreateSmileyUI()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CreateSmileyUI::languageChange()
{
    setCaption( i18n("Add custom smiley" ) );
    lblTop->setText( i18n("<b>Please follow the steps below to create your smiley.</b><br>\n"
"<b>You could select one or more files at a time.</b>" ) );
    lblStep1->setText( i18n("1. Select files from your computer.<br>\n"
"&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"#0066cc\">( 150 pixels x 150 pixels or smaller size for better quality. )</font>" ) );
    lblSelect->setText( QString::null );
    btnSelect->setText( i18n("Select Images ..." ) );
    lblStep2->setText( i18n("2. Input the shortcut key for the smiley. Max length is 6 chars.<br>\n"
"&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"#0066cc\">For example: >3Q. You could leave it empty.</font>" ) );
    lblStep2_2->setText( i18n("Select Group:" ) );
    lblStep3->setText( i18n("3. Give your smiley a name." ) );
    lblCommends->setText( i18n("<font color=\"#0099cc\">Note: When creating more than one smileys, the file names would be used as smiley names and shortcuts.</font>" ) );
    btnOK->setText( i18n("&OK" ) );
    btnOK->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    btnCancel->setText( i18n("&Cancel" ) );
    btnCancel->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

