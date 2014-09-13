dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(gadb, 0.1)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_RANLIB



PKG_CHECK_MODULES(GADB, gtk+-3.0
				  libhardware
				  [libssl ])




AC_OUTPUT([
		   Makefile
		   src/Makefile
		   src/libzipfile/Makefile
		   src/libcutils/Makefile
		   ])
