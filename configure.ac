dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(aml, 0.1)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_RANLIB



PKG_CHECK_MODULES(AML,
				  [gtk+-3.0]
				  [libssl]
				  [libcrypto]
				  [libzip])




AC_OUTPUT([
		   Makefile
		   src/Makefile
		   res/Makefile
		   src/libadb/Makefile
		   src/libadb/libzipfile/Makefile
		   src/libadb/libcutils/Makefile
		   ])
