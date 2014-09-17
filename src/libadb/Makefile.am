## Process this file with automake to produce Makefile.in


noinst_LIBRARIES =  libadb.a

libadb_a_CPPFLAGS = \
	-DADB_HOST=1 \
	-DHAVE_FORKEXEC=1 \
	-DHAVE_SYMLINKS \
	-DHAVE_TERMIO_H \
	$(AML_CFLAGS)

libadb_a_CFLAGS =\
	 -Wall\
	 -g


libadb_a_SOURCES =  \
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
	adb.c 


SUBDIRS = \
	libzipfile \
	libcutils
