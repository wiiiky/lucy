## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPACKAGE_BUILD_DIR=\"$(top_builddir)\" \
	$(LUCY_CFLAGS)

AM_CFLAGS =\
     -pthread \
	 -Wall \
	 -g

bin_PROGRAMS = $(top_builddir)/lucy

__top_builddir__lucy_SOURCES =  \
	lucy.c \
	ui_aboutdialog.h \
	ui_aboutdialog.c \
	ui_applicationview.h \
	ui_applicationview.c \
	ui_applicationrow.h \
	ui_applicationrow.c \
	ui_mainwindow.h \
	ui_mainwindow.c \
	ui_myandroid.h \
	ui_myandroid.c \
	ui_toolstack.h \
	ui_toolstack.c \
	ui_installdialog.h \
	ui_installdialog.c \
	ui_smsbox.h \
	ui_smsbox.c \
	ui_smsrow.h \
	ui_smsrow.c \
	ui_smsview.h \
	ui_smsview.c \
	lcprotocol.h \
	lcprotocol.c \
	lcadb.c \
	lcadb.h \
	lcsocket.h \
	lcsocket.c \
	lcutil.c \
	lcutil.h \
	lccommander.c \
	lccommander.h \
	lcnotify.h \
	lcnotify.c \
	lcresource.h \
	test.c

__top_builddir__lucy_LDADD = \
	libadb/libadb.a \
	libadb/libzipfile/libzipfile.a \
	libadb/libcutils/libcutils.a \
	$(LUCY_LIBS)


SUBDIRS = \
	libadb
