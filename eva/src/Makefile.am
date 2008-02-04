# set the include path for X, qt and KDE
INCLUDES = -I$(top_srcdir)/src/api -I$(top_srcdir)/src/libeva \
	-I$(top_srcdir)/src/ui $(all_includes)

# these are the headers for your project


# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp ui/*.cpp api/*.cpp -o $(podir)/eva.pot

KDE_ICON = eva

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = eva

# the application source, library search path, and link libraries
eva_SOURCES = main.cpp evachatwindowmanager.cpp evamain.cpp evadcopactions.skel \
			evasystemtray.cpp evaidt.cpp evaaddingmanager.cpp evasysmsgmanager.cpp \
			evaloginmanager.cpp evacontactmanager.cpp evascriptmanager.cpp evadcoptools.skel
eva_LDFLAGS = $(KDE_RPATH) $(all_libraries)
eva_LDADD = $(top_builddir)/src/ui/libevaui.la \
	$(top_builddir)/src/libeva/libeva.la $(top_builddir)/src/api/libevaapi.la \
	$(top_builddir)/src/api/filetrans/libapi_ft.la $(LIB_KDEUI) $(LIBXSS)

# this is where the desktop file will go 
shelldesktopdir = $(kde_appsdir)/Utilities

# this is where the shell's XML-GUI resource file goes
shellrcdir = $(kde_datadir)/eva
shellrc_DATA = evaui.rc



SUBDIRS = ui libeva api image sound
applnkApplication_DATA = eva.desktop
applnkApplicationdir = $(xdg_appsdir)
server_DATA = servers qun-category citylist
serverdir = $(kde_datadir)/eva

noinst_HEADERS = evadcoptools.h
