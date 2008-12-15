/***************************************************************************
 *   Copyright (C) 2005 by yunfan                                          *
 *   yunfan_zg@163.com                                                     *
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


#include "evaguimain.h"
#include "defines.h"
#include <qapplication.h>
//X #include <kaboutdata.h>
//X #include <kcmdlineargs.h>
//X #include <klocale.h>

#ifdef HAVE_CONFIG_H
 #include "config.h"
#endif

static const char description[] =
	I18N_NOOP("An IM Client Under KDE Using QQ IM Protocol.");

static const char version[] = EVA_VERSION; // update the version number in configure.in.in and re-generate the configure script;

//X static KCmdLineOptions options[] =
//X {
//X 	{ "last", I18N_NOOP("Using the last saved settings to login."), 0},
//X 	{ "u", 0, 0},
//X 	{ "user <qq number>", I18N_NOOP("QQ number to login."), 0},
//X 	{ "p", 0, 0},
//X 	{ "passwd <password>", I18N_NOOP("Password for your QQ number."), 0},
//X 	{ "m", 0, 0},
//X 	{ "mode <mode>", I18N_NOOP("Connection mode (one of UDP, TCP or HTTP_PROXY), defaulting to UDP"), "UDP"},
//X 	{ "pip <IP address>", I18N_NOOP("Proxy server IP address."), 0},
//X 	{ "pport <proxy port>", I18N_NOOP("Proxy server port number"), 0},
//X 	{ "pu <proxy username>", I18N_NOOP("Proxy user name if required."), 0},
//X 	{ "ppw <proxy password>", I18N_NOOP("Proxy password if required."), 0},
//X 	{ "hide", I18N_NOOP("Select login mode to invisible."), 0},
//X 	{ "remember", I18N_NOOP("Save password and proxy settings."), 0},
//X 	KCmdLineLastOption
//X };

int main(int argc, char **argv)
{
//X 	KAboutData about("eva", 
//X 				I18N_NOOP("Eva"), 
//X 				version,
//X 				description,
//X 				 KAboutData::License_GPL, 
//X 				I18N_NOOP("(c) 2004-2008, yunfan"), 
//X 				"yunfan_zg@163.com", 
//X 				"http://www.sourceforge.net/projects/evaq", 
//X 				"eva.bugs@gmail.com");
//X 	about.addAuthor( I18N_NOOP("yunfan"), I18N_NOOP("original developer, maintainer"), 
//X 			"yunfan_zg@163.com","http://www.sourceforge.net/projects/evaq" );
//X 	about.addAuthor( I18N_NOOP("casper"), I18N_NOOP("developer, memo, system setting ui and IP address converting"), "tlmcasper@163.com" );
//X 	//about.setBugAddress( "eva.bugs@gmail.com");
//X 
//X 	about.addCredit( I18N_NOOP("byakko"), I18N_NOOP("UI patch provider"), "byakko.ang@gmail.com");
//X 	about.addCredit( I18N_NOOP("caihua"), I18N_NOOP("icon designer"), "13307864987@gx165.com");
//X 	about.addCredit( I18N_NOOP("Funda Wang"), I18N_NOOP("patch for xdg-user-dir supported"), "fundawang@yeah.net");
//X 	about.addCredit( I18N_NOOP("Levin Du"), 
//X 		I18N_NOOP("patch for system setting \nfixed combo box index massing up after \nclicking \"Default\" several times."),
//X 			"zsdjw@21cn.com");
//X 	about.addCredit( I18N_NOOP("Minmin"), I18N_NOOP("encapsulating encryption class in C++"), "csdengxm@hotmail.com");  
//X 	about.addCredit( I18N_NOOP("nwillis"), I18N_NOOP("idle detecting(auto-away) feature"), "neowillis@gmail.com");  
//X 	about.addCredit( I18N_NOOP("Phytonix"), I18N_NOOP("some original faces & Eva Logo designer."), "yahzee@d3eye.com");
//X 	about.addCredit( I18N_NOOP("Shixin Zeng"), I18N_NOOP("libeva patch for Windows support."), "shixinzeng@gmail.com");
//X 	about.addCredit( I18N_NOOP("Stark Wong"), I18N_NOOP("libeva patch for Visual Studio .NET 2002/2003 compiling support.\n2007II new smiley code"), "starkwong@hotmail.com");
//X 	about.addCredit( I18N_NOOP("zsyddl"), I18N_NOOP("earlier user details window designer"), "morrowren@sina.com" );
//X 	about.addCredit( I18N_NOOP("swear"), I18N_NOOP("forum maintainer"), "inzaghi1230@hotmail.com");
//X 
//X 	KCmdLineArgs::init(argc, argv, &about);
//X 	KCmdLineArgs::addCmdLineOptions( options );
	QApplication app( argc, argv );
        app.setQuitOnLastWindowClosed(false );
	EvaMain *mainWin = 0;

        //KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        /// @todo do something with the command line args here

        mainWin = new EvaMain();

//X 	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
       // args->clear();

        printf( "main loop!\n" );
	return app.exec();
}

