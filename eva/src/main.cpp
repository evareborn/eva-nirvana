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


#include "evamain.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

#ifdef HAVE_CONFIG_H
 #include "config.h"
#endif

static const char description[] =
	I18N_NOOP("An IM Client Under KDE Using QQ IM Protocol.");

static const char version[] = VERSION; // update the version number in configure.in.in and re-generate the configure script;

static KCmdLineOptions options[] =
{
	{ "last", I18N_NOOP("Using the last saved settings to login."), 0},
	{ "u", 0, 0},
	{ "user <qq number>", I18N_NOOP("QQ number to login."), 0},
	{ "p", 0, 0},
	{ "passwd <password>", I18N_NOOP("Password for your QQ number."), 0},
	{ "m", 0, 0},
	{ "mode <mode>", I18N_NOOP("Connection mode (one of UDP, TCP or HTTP_PROXY), defaulting to UDP"), "UDP"},
	{ "pip <IP address>", I18N_NOOP("Proxy server IP address."), 0},
	{ "pport <proxy port>", I18N_NOOP("Proxy server port number"), 0},
	{ "pu <proxy username>", I18N_NOOP("Proxy user name if required."), 0},
	{ "ppw <proxy password>", I18N_NOOP("Proxy password if required."), 0},
	{ "hide", I18N_NOOP("Select login mode to invisible."), 0},
	{ "remember", I18N_NOOP("Save password and proxy settings."), 0},
	KCmdLineLastOption
};

int main(int argc, char **argv)
{
	KAboutData about("eva", 
				I18N_NOOP("Eva"), 
				version,
				description,
				 KAboutData::License_GPL, 
				I18N_NOOP("(c) 2004-2008, yunfan"), 
				"yunfan_zg@163.com", 
				"http://www.sourceforge.net/projects/evaq", 
				"eva.bugs@gmail.com");
	about.addAuthor( I18N_NOOP("yunfan"), I18N_NOOP("original developer, maintainer"), 
			"yunfan_zg@163.com","http://www.sourceforge.net/projects/evaq" );
	about.addAuthor( I18N_NOOP("casper"), I18N_NOOP("developer, memo, system setting ui and IP address converting"), "tlmcasper@163.com" );
	//about.setBugAddress( "eva.bugs@gmail.com");

	about.addCredit( I18N_NOOP("byakko"), I18N_NOOP("UI patch provider"), "byakko.ang@gmail.com");
	about.addCredit( I18N_NOOP("caihua"), I18N_NOOP("icon designer"), "13307864987@gx165.com");
	about.addCredit( I18N_NOOP("Funda Wang"), I18N_NOOP("patch for xdg-user-dir supported"), "fundawang@yeah.net");
	about.addCredit( I18N_NOOP("Levin Du"), 
		I18N_NOOP("patch for system setting \nfixed combo box index massing up after \nclicking \"Default\" several times."),
			"zsdjw@21cn.com");
	about.addCredit( I18N_NOOP("Minmin"), I18N_NOOP("encapsulating encryption class in C++"), "csdengxm@hotmail.com");  
	about.addCredit( I18N_NOOP("nwillis"), I18N_NOOP("idle detecting(auto-away) feature"), "neowillis@gmail.com");  
	about.addCredit( I18N_NOOP("Phytonix"), I18N_NOOP("some original faces & Eva Logo designer."), "yahzee@d3eye.com");
	about.addCredit( I18N_NOOP("Shixin Zeng"), I18N_NOOP("libeva patch for Windows support."), "shixinzeng@gmail.com");
	about.addCredit( I18N_NOOP("Stark Wong"), I18N_NOOP("libeva patch for Visual Studio .NET 2002/2003 compiling support.\n2007II new smiley code"), "starkwong@hotmail.com");
	about.addCredit( I18N_NOOP("zsyddl"), I18N_NOOP("earlier user details window designer"), "morrowren@sina.com" );
	about.addCredit( I18N_NOOP("swear"), I18N_NOOP("forum maintainer"), "inzaghi1230@hotmail.com");

	KCmdLineArgs::init(argc, argv, &about);
	KCmdLineArgs::addCmdLineOptions( options );
	KApplication app;
	EvaMain *mainWin = 0;

        //KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        /// @todo do something with the command line args here

        mainWin = new EvaMain();

	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
       // args->clear();

    	// mainWin has WDestructiveClose flag by default, so it will delete itself.
	return app.exec();
}

