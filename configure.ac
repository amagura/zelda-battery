AC_PREREQ([2.65])
AC_INIT([ZBatt], [3.0.4], [agm2819@gmail.com],,[https://github.com/amagura/zelda-battery])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall])
AC_USE_SYSTEM_EXTENSIONS

# Check if we are running on Windows
AC_LANG(C)
AC_RUN_IFELSE([AC_LANG_PROGRAM([
#include "nix/compat.h"
],[
#if ZB_WINDOWS
return 0;
#else
return 1;
#endif
return 2;
])], [m4_define([IS_WINDOWS], [1])][:], [m4_define([IS_WINDOWS], [0])][:])


#AS_ECHO([[WINDOWS:]][IS_WINDOWS])
m4_define([IF_WINDOWS], [m4_if([IS_WINDOWS], [1], [$1], [$2])])
# Check for compilers
dnl C
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CXX
AM_PROG_CC_C_O

# Check for sed and grep
AC_PROG_SED
AC_PROG_GREP

# Get System Type
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

# Check for pkg-config
PKG_PROG_PKG_CONFIG()

# build os checking
AS_CASE([$build],
  [*linux*], [ZB_BUILD="linux"],
  [*cygwin], [ZB_BUILD="windows"],
  [*mingw32], [ZB_BUILD="windows"],
  [*bsd*], [ZB_BUILD="bsd"],
  [ZB_BUILD=""])

AC_SUBST([ZB_BUILD])

AM_CONDITIONAL([LINUX_BUILD], [test "x$ZB_BUILD" = "xlinux"])
AM_CONDITIONAL([WINDOWS_BUILD], [test "x$ZB_BUILD" = "xwindows"])
AM_CONDITIONAL([BSD_BUILD], [test "x$ZB_BUILD" = "xbsd"])

AC_PATH_PROGS([GPP], [gpp cpp],[no])

# host os checking
AS_CASE([$host],
  [*linux*], [ZB_HOST="linux"],
  [*cygwin], [ZB_HOST="windows"],
  [*mingw32], [ZB_HOST="windows"],
  [*bsd*], [ZB_HOST="bsd"],
  [ZB_HOST=""])
AC_SUBST([ZB_HOST])
AM_CONDITIONAL([LINUX_HOST], [test "x$ZB_HOST" = "xlinux"])
AM_CONDITIONAL([WINDOWS_HOST], [test "x$ZB_HOST" = "xwindows"])
AM_CONDITIONAL([BSD_HOST], [test "x$ZB_HOST" = "xbsd"])

# If OS is `Windows' check for the following headers

AS_IF([test "x$ZB_HOST" = "xwindows"],
[AC_CHECK_HEADER([Windows.h],[AC_DEFINE([HAVE_WINDOWS_H], [1])],
	[AC_CHECK_HEADER([windows.h],[AC_DEFINE([HAVE__WINDOWS_H], [1])])])
AC_CHECK_HEADERS([Winapifamily.h winapifamily.h _mingw_unicode.h specstrings.h])
AC_CHECK_HEADER([Shellapi.h],[AC_DEFINE([HAVE_SHELLAPI_H], [1])],
	[AC_CHECK_HEADER([shellapi.h],[AC_DEFINE([HAVE__SHELLAPI_H], [1])])])])

[CPUARCH="`echo $host | $ac_cv_path_SED 's/^\([a-z|0-9|_]\+\)-.\+$/\1/g'`"]
AC_SUBST([CPUARCH])

# Precious variables
AC_ARG_VAR([CYTHON], [Cython to C compiler (only used when `--with-gtk' is present)])
# Should we use gtk2
AC_ARG_WITH([gtk2],
	AS_HELP_STRING([--without-gtk2], [disable building the gui using gtk2 (default: enabled)]),
	[],
	[with_gtk2=yes])

AC_ARG_WITH([gtk3],
	AS_HELP_STRING([--with-gtk3], [enable building the gui using gtk3 (default: disabled)]),
	[with_gtk3=yes;with_gtk2=no],
	[])
IF_WINDOWS([with_gtk2=no;with_gtk3=no])

IF_WINDOWS(,[AS_IF([test "x$with_gtk3" = "xyes"],
[PKG_CHECK_MODULES([gtk3], [gtk+-3.0 >= 3.0],
	[AC_DEFINE([HAVE_GTK3], [1])])],
	[test "x$with_gtk2" = "xyes"],
[PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.0],
	[AC_DEFINE([HAVE_GTK2], [1])])])])

# Check if we have GTK+2
AM_CONDITIONAL([GTK2], [test "x$with_gtk2" = "xyes"])
AM_CONDITIONAL([GTK3], [test "x$with_gtk3" = "xyes"])
AM_CONDITIONAL([GUI], [test "x$with_gtk2" = "xyes" || test "x$with_gtk3" = "xyes"])

# Travis
AC_MSG_CHECKING([whether build is automated])
AM_CONDITIONAL([TRAVIS], [test "x$TRAVIS" = "xtrue"])
AM_COND_IF([TRAVIS], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])

# Check for libs
AS_IF([! test x"$ZB_HOST" = "xwindows"], AC_SEARCH_LIBS([nearbyint], [m],,[AC_MSG_ERROR([Please use a C99 compliant compiler])]), )

# Check for OS specifics
AS_IF([test x"$ZB_HOST" = x"linux"], [AM_COND_IF([TRAVIS], AC_DEFINE([HAVE__SYS_CLASS_POWER_SUPPLY], [1]),
	    AS_IF([test x"$ZB_HOST" = x"linux"],
	     [AC_CHECK_FILES([/sys/class/power_supply],,
	        AC_MSG_ERROR([Linux systems  without `/sys/class/power_supply' are unsupported]))],))])

AS_IF([test x"$ZB_HOST" = x"bsd"], [AC_SEARCH_LIBS([sysctlbyname], [c],,[AC_MSG_ERROR([*BSD systems without `sysctlbyname` are unsupported])])],)

# Check for progs
IF_WINDOWS(,[AM_COND_IF([GUI],[AC_CHECK_PROGS([CYTHON], [cython cython2],[no])])
AM_COND_IF([GUI],[AS_IF([test "x$ac_cv_prog_CYTHON" = "xcython2"],[PKG_CHECK_MODULES([python2],[python2],,)],[test "x$ac_cv_prog_CYTHON" = "xcython"],[PKG_CHECK_MODULES([python2],[python2],,)],[AC_MSG_ERROR([Cython is a make dependency for the GUI])])])])

# Check if peeps want to try to compile to native (compiler may or may not support it)
AC_ARG_ENABLE([native],
              AS_HELP_STRING([--enable-native], [enable processor-model specific compilation (default: enabled)]),
              [case "${enableval}" in
               yes) native=true ;;
               no) native=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-native]) ;;
             esac],[native=true])
AM_CONDITIONAL([NATIVE], [test "x$native" = "xtrue"])

# Check if peeps want to try to compile to generic (compiler may or may not support it)
AC_ARG_ENABLE([generic],
              AS_HELP_STRING([--enable-generic], [enable processor-family specific compilation (default: disabled)]),
              [case "${enableval}" in
               yes) generic=true ;;
               no) generic=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-generic]) ;;
             esac],[generic=false])
AM_CONDITIONAL([GENERIC], [test "x$generic" = "xtrue"])

# Check for debug
AC_ARG_ENABLE([debug],
              [  --enable-debug         turn on debugging (default: disabled)],
              [case "${enableval}" in
               yes) debug=true ;;
               no) debug=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
             esac],[debug=no])
[test "x$debug" = "xno" && no_debug=false]
[test "x$debug" = "xfalse" && no_debug=true]

# Check for testdir
AC_ARG_ENABLE([testdir],
		[  --enable-testdir	defines ZB_ACPI_ROOT for testing (default: disabled)],
		[case "${enableval}" in
		 yes) testdir=true ;;
		 no) testdir=false ;;
		 *) AC_MSG_ERROR([bad value ${enableval} for --enable-testdir]) ;;
		 esac],[testdir=false])

AM_CONDITIONAL([TEST], [test "x$testdir" = "xtrue"])
AM_CONDITIONAL([DEBUG], [test "x$debug" = "xtrue"])
AM_CONDITIONAL([NO_DEBUG], [test "x$no_debug" = "xtrue"])

# Check for static compilation
AC_ARG_ENABLE([static],
	      AS_HELP_STRING([  --enable-static	turn on static compilation (default: disabled)]),
	      [case "${enableval}" in
	       yes) static=true ;;
	       no) static=false ;;
	       *) AC_MSG_ERROR([bad value ${enableval} for --enable-static]) ;;
	      esac],[static=false])

AM_CONDITIONAL([STATIC], [test "x$static" = "xtrue"])
AM_CONDITIONAL([GENERIC], [test "x$static" = "xtrue" || test "x$generic" = "xtrue"])
AM_CONDITIONAL([NATIVE], [test "x$static" = "xfalse" || test "x$native" = "xtrue"])

# generic?
m4_define([CLANG], [test "x`($CC -E -w -P -nostdinc - <<EOF
__clang__
EOF
)`" = "x1"])
m4_define([TINYC], [test ! "x`($CC -E -w -P -nostdinc - <<EOF
__TINYC__
EOF
)`" = "x__TINYC__"])
AM_CONDITIONAL([CPU_KNOWN], [test ! x"$CPUARCH" = "x" && test ! x"$CPUARCH" =		x"unknown" && (]CLANG[ || ]TINYC[)])

AC_CONFIG_FILES([Makefile src/Makefile img/Makefile win/Makefile])
AC_OUTPUT
