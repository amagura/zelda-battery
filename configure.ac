#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ZBatt], [3.1.0], [agm2819@gmail.com],, [https://github.com/amagura/zelda-battery])
#AC_CONFIG_SRCDIR([test.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])

# Check for system extensions
AC_USE_SYSTEM_EXTENSIONS

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
AC_ARG_WITH([X],
            AS_HELP_STRING([--without-x],
                           [disable X server support (default: enabled for gtk2)])
            AS_HELP_STRING([],
                           [supported options:
                                                gtk2
                                                gtk3], [15], [30]),
            [with_X=gtk2],
            [with_X=no])

AS_IF([test "x$with_X" != "xno"],
      [AS_IF([test "x$with_X" = "xgtk3"],
             [PKG_CHECK_MODULES([gtk3], [gtk+-3.0 >= 3.0],
                                AC_DEFINE([HAVE_GTK3],
                                          [1],
                                          [Define if gtk3 is present]))],
             [PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.0],
                                AC_DEFINE([HAVE_GTK2],
                                          [1],
                                          [Define if gtk2 is present]))]
             )])


# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h sys/ioctl.h])
AC_CHECK_HEADERS([sys/pm.h],,,)

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero memmove mempcpy memset strchr strerror strtol])
AC_CHECK_FUNC([sysctlbyname],
              ,
              AC_SEARCH_LIBS([sysctlbyname],
                             [c],
                             ,
                             [AC_MSG_WARN([*BSD systems without 'sysctlbyname' are not supported.])]))


AC_CONFIG_FILES([Makefile
                 img/Makefile
                 src/Makefile
                 src/mswin/Makefile])
#AC_CONFIG_SUBDIRS([old
                   #src/test])
# Automake directives
dnl `foreign' prevents autotools from installing a copy of the GPL
AM_INIT_AUTOMAKE([foreign])
AM_CONDITIONAL([X], [test "x$with_X" != "xno"])
AM_CONDITIONAL([NATIVE], [test "x$native" = "xtrue"])
AM_CONDITIONAL([GENERIC], [test "x$generic" = "xtrue"])
AM_CONDITIONAL([STATIC], [test "x$static" = "xtrue"])
AM_CONDITIONAL([DEBUG], [test "x$debug" = "xtrue"])
AM_CONDITIONAL([NO_DEBUG], [test "x$debug" != "xtrue"])
AM_CONDITIONAL([MSWIN], [test "x$mswin" = "xtrue"])
AM_CONDITIONAL([TEST], [test "x$testdir" = "xtrue"])
m4_define([CLANG], [test "x`($CC -E -w -P -nostdinc - <<EOF
__clang__
EOF
)`" = "x1"])
m4_define([TINYC], [test ! "x`($CC -E -w -P -nostdinc - <<EOF
__TINYC__
EOF
)`" = "x__TINYC__"])
AM_CONDITIONAL([CPU_KNOWN], [test ! "x$CPUARCH" = "x" && test ! "x$CPUARCH" = "xunknown" && (]CLANG[ || ]TINYC[)])


AC_OUTPUT
