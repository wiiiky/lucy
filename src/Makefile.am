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
	lcapplicationview.h \
	lcapplicationview.c \
	lcprotocol.h \
	lcprotocol.c \
	lcaboutdialog.h \
	lcaboutdialog.c \
	lcapplicationrow.h \
	lcapplicationrow.c \
	lcmainwindow.h \
	lcmainwindow.c \
	lcadb.c \
	lcadb.h \
	lcsocket.h \
	lcsocket.c \
	lcutil.c \
	lcutil.h \
	lccommander.c \
	lccommander.h \
	lctoolstack.h \
	lctoolstack.c \
	lcmyandroid.h \
	lcmyandroid.c \
	lcinstalldialog.h \
	lcinstalldialog.c \
	lcsmsbox.h \
	lcsmsbox.c

__top_builddir__lucy_LDADD = \
	libadb/libadb.a \
	libadb/libzipfile/libzipfile.a \
	libadb/libcutils/libcutils.a \
	$(LUCY_LIBS)


SUBDIRS = \
	libadb
