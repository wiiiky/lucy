#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(lucy, 0.2)
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
PKG_CHECK_MODULES(LUCY,
				  [gtk+-3.0]
				  [libssl]
				  [libzip]
				  [libnotify]
				  [json-glib-1.0])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h limits.h netdb.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/ioctl.h sys/mount.h sys/socket.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_FORK
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_HEADER_MAJOR
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit dup2 getcwd gethostbyname gethostname gettimeofday localtime_r memset mkdir pow select setenv socket strchr strdup strerror strpbrk strrchr strtol strtoul tzset])

AC_OUTPUT([
		   Makefile
		   src/Makefile
		   res/Makefile
		   src/libadb/Makefile
		   src/libadb/libzipfile/Makefile
		   src/libadb/libcutils/Makefile
		   ])
