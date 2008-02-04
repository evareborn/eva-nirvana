METASOURCES = AUTO

noinst_LTLIBRARIES = libevaui.la

libevaui_la_SOURCES = evachatuibase.cpp evachatwindow.cpp evadetailswindow.cpp \
	evafontselecter.cpp evahistoryuibase.cpp evahistoryviewer.cpp evaloginwindow.cpp \
	evamainuibase.cpp evamainwindow.cpp evadcopview.skel evanotifyuibase.cpp \
	evanotifywindow.cpp evasearchuibase.cpp evasearchwindow.cpp evasyshistoryviewer.cpp \
	evasyssettinguibase.cpp evasyssettingwindow.cpp evatextedit.cpp evatipuibase.cpp \
	evatipwindow.cpp evauseruibase.cpp loginuibase.cpp evachatview.cpp evaqunchatuibase.cpp \
	evaqunchatwindow.cpp evaqunlistview.cpp qundetailswindow.cpp evaqunmemberpickerui.cpp \
	evaqunmemberpicker.cpp quncategorypicker.cpp evaquncreatewindow.cpp quncategoryui.cpp \
	quncreateui.cpp qundetailsui.cpp evauserinfowidget.cpp evafilepanel.cpp \
	evafilestatusuibase.cpp mainlistfontsettinguibase.cpp customfaceselector.cpp \
	customfaceuibase.cpp createsmileyui.cpp createsmileywindow.cpp customfacemanagerui.cpp \
	evaloginveriuibase.cpp evaloginveriwindow.cpp evaadduibase.cpp evaaddingwindow.cpp \
	evaaddingnoticeuibase.cpp evaaddingnoticewindow.cpp evasysbroadcastuibase.cpp \
	evasysbroadcastwindow.cpp evaaddqunuibase.cpp evaaddqunwindow.cpp evaqunsysmsguibase.cpp \
	evaqunsysmsgwindow.cpp evatabwidget.cpp evacontactlistview.cpp evalistview.cpp \
	evadisplaywidget.cpp evastatusbar.cpp evascriptmanageruibase.cpp evascriptwidget.cpp
noinst_HEADERS = evaui.h qundetailswindow.h evadcopview.h
INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/api \
	-I$(top_srcdir)/src/libeva $(all_includes)
libevaui_la_LIBADD = $(LIB_KIO) -lkhtml
