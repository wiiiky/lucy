## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DADB_HOST=1 \
	-DHAVE_FORKEXEC=1 \
	-DHAVE_SYMLINKS \
	-DHAVE_TERMIO_H \
	$(LUCY_CFLAGS)

AM_CFLAGS =\
     -pthread \
	 -Wall \
	 -g

bin_PROGRAMS = lucy

lucy_SOURCES =  \
	lucy.c \
	lcapplicationview.h \
	lcapplicationview.c \
	lcprotocol.h \
	lcprotocol.c \
	lcaboutdialog.h \
	lcaboutdialog.c \
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
	lctoolstack.c

lucy_LDADD = \
	libadb/libadb.a \
	libadb/libzipfile/libzipfile.a \
	libadb/libcutils/libcutils.a \
	$(LUCY_LIBS)


SUBDIRS = \
	libadb
