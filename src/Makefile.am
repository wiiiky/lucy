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
	 -I./include \
	 -Wall\
	 -g

bin_PROGRAMS = aml

aml_SOURCES =  \
	utils.h \
	utils.c \
	usb_vendors.h \
	usb_vendors.c \
	usb_linux.c \
	transport_usb.c \
	transport_local.c \
	transport.h \
	transport.c \
	sysdeps.h \
	sockets.c \
	services.c \
	mutex_list.h \
	get_my_path_linux.c \
	file_sync_service.h \
	file_sync_client.c \
	fdevent.h \
	fdevent.c \
	console.c \
	commandline.c \
	adb_client.h \
	adb_client.c \
	adb_auth_host.c \
	adb_auth.h \
	adb.h \
	adb.c \
	aml.c

aml_LDFLAGS =  -lc -lcrypto -pthread 

aml_LDADD = $(AML_LIBS) libzipfile/libzipfile.a libcutils/libcutils.a -lz


SUBDIRS = \
	libzipfile \
	libcutils
