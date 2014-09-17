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
	 -Wall\
	 -g

bin_PROGRAMS = aml

aml_SOURCES =  \
	aml.c

aml_LDADD = \
	libadb/libadb.a \
	libadb/libzipfile/libzipfile.a \
	libadb/libcutils/libcutils.a \
	-lz \
	-lc \
	-lcrypto \
	-lpthread  \
	$(AML_LIBS)


SUBDIRS = \
	libadb
