METASOURCES = AUTO

noinst_LTLIBRARIES = libevaapi.la

libevaapi_la_SOURCES = evaconnecter.cpp evahtmlparser.cpp evanetwork.cpp \
	evapacketmanager.cpp evaresource.cpp evasetting.cpp evasocket.cpp evauser.cpp evadcopcontacts.skel\
	evausersetting.cpp qmdcodec.cpp evaservers.cpp evaipaddress.cpp evaipseeker.cpp \
	evaqtutil.cpp evapicmanager.cpp regiongrabber.cpp evauhmanager.cpp customface.cpp
libevaapi_la_LIBADD = $(top_builddir)/src/libeva/libeva.la \
	$(top_builddir)/src/api/filetrans/libapi_ft.la -lqt-mt
INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/libeva \
	-I$(top_srcdir)/src/libeva/libcustompic -I$(top_srcdir)/src/libeva/libuh $(all_includes)
noinst_HEADERS = evaservers.h
SUBDIRS = filetrans
