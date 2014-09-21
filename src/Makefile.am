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
	$(AML_CFLAGS)

AM_CFLAGS =\
     -pthread \
	 -Wall \
	 -g

bin_PROGRAMS = aml

aml_SOURCES =  \
	aml.c \
	amlapplicationview.h \
	amlapplicationview.c \
	amlprotocol.h \
	amlprotocol.c \
	amlaboutdialog.h \
	amlaboutdialog.c \
	amlmainwindow.h \
	amlmainwindow.c \
	amlsocket.c \
	amlsocket.h \
	amladb.c \
	amladb.h

aml_LDADD = \
	libadb/libadb.a \
	libadb/libzipfile/libzipfile.a \
	libadb/libcutils/libcutils.a \
	$(AML_LIBS)


SUBDIRS = \
	libadb
