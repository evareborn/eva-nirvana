 
#include "evamainuibase.h"

//#include <qvariant.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <q3header.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
//X #include <klocale.h>
#include "evadisplaywidget.h"
#include "evastatusbar.h"
#include "defines.h"
//#include "evalistview.h"

EvaMainUIBase::EvaMainUIBase( QWidget* parent, const char* name, Qt::WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "EvaMainUIBase" );
    EvaMainUIBaseLayout = new Q3GridLayout( this, 1, 1, 0, 0, "EvaMainUIBaseLayout"); 

    layout1 = new Q3VBoxLayout( 0, 0, 0, "layout1"); 

    layout2 = new Q3HBoxLayout( 0, 0, 0, "layout2"); 

    tbMyFace = new QToolButton( this, "tbMyFace" );
    tbMyFace->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tbMyFace->sizePolicy().hasHeightForWidth() ) );
    tbMyFace->setMinimumSize( QSize( 40, 39 ) );
    tbMyFace->setAutoRaise( TRUE );
    layout2->addWidget( tbMyFace );

    tlNick = new QLabel( this, "tlNick" );
    tlNick->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tlNick->sizePolicy().hasHeightForWidth() ) );
    layout2->addWidget( tlNick );

    tlQQ = new QLabel( this, "tlQQ" );
    tlQQ->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, tlQQ->sizePolicy().hasHeightForWidth() ) );
    layout2->addWidget( tlQQ );
    spacer1 = new QSpacerItem( 50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    spacer1 = new QSpacerItem( 50, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    layout2->addItem( spacer1 );
    layout1->addLayout( layout2 );

    mainDisplay = new EvaDisplayWidget(this, "mainDisplay");
    mainDisplay->showInfoFrame(false);
//X     mainDisplay->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, mainDisplay->sizePolicy().hasHeightForWidth() ) );
//X     mainDisplay->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, mainDisplay->sizePolicy().hasHeightForWidth() ) );
    layout1->addWidget( mainDisplay );

    statusBar = new EvaStatusBar(this, "status bar");
    layout1->addWidget(statusBar);
    
//    layout3 = new QVBoxLayout( 0, 0, 0, "layout3"); 

//     pbSearch = new QPushButton( this, "pbSearch" );
//     layout3->addWidget( pbSearch );
// 
//     pbMessages = new QPushButton( this, "pbMessages" );
//     layout3->addWidget( pbMessages );
//     layout1->addLayout( layout3 );

//     layout4 = new QHBoxLayout( 0, 0, 6, "layout4"); 
// 
//     pbSystem = new QPushButton( this, "pbSystem" );
//     pbSystem->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, pbSystem->sizePolicy().hasHeightForWidth() ) );
//     layout4->addWidget( pbSystem );
// 
//     pbStatus = new QPushButton( this, "pbStatus" );
//     pbStatus->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, pbStatus->sizePolicy().hasHeightForWidth() ) );
//     layout4->addWidget( pbStatus );
//     layout1->addLayout( layout4 );

    EvaMainUIBaseLayout->addLayout( layout1, 0, 0 );
    languageChange();
    resize( QSize(195, 376).expandedTo(minimumSizeHint()) );
//X     clearWState( WState_Polished );
}

EvaMainUIBase::~EvaMainUIBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void EvaMainUIBase::languageChange()
{
    setCaption( i18n( "Eva" ) );
    tbMyFace->setText( "..." );
    tlNick->setText( i18n( "Nick" ) );
    tlQQ->setText( i18n( "QQ" ) );
    //listView->header()->setLabel( 0, i18n( "Column 1" ) );
    //listView->clear();
    //QListViewItem * item = new QListViewItem( listView, 0 );
    //item->setText( 0, i18n( "New Item" ) );

    //pbSearch->setText( i18n( "Search" ) );
    //pbMessages->setText( i18n( "Messages" ) );
    //pbSystem->setText( i18n( "System" ) );
    //pbStatus->setText( i18n( "Online" ) );
}
