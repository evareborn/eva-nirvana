METASOURCES = AUTO

noinst_LTLIBRARIES = libeva.la

libeva_la_SOURCES = evaaddfriend.cpp evafriend.cpp evafriendlist.cpp \
					evagroup.cpp evaimreceive.cpp evaimsend.cpp evalogin.cpp evaonlinestatus.cpp \
					evapacket.cpp evasearchuser.cpp evauserinfo.cpp evautil.cpp md5.c evalogintoken.cpp \
					evacrypt.cpp evaqun.cpp evaqunlist.cpp evalevel.cpp evakeyrequest.cpp \
					evaextrainfo.cpp evamemo.cpp evaweather.cpp evaadvancedsearch.cpp evaaddfriendex.cpp
SUBDIRS = libcustompic libuh libft
libeva_la_LIBADD = $(top_builddir)/src/libeva/libcustompic/libcustompic.la \
	$(top_builddir)/src/libeva/libuh/libuh.la
