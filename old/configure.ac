AC_PREREQ([2.65])
AC_INIT([ZBatt], [3.1.0], [agm2819@gmail.com],,[https://github.com/amagura/zelda-battery])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-mac])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([foreign -Wall])

AC_PROG_CC([cc gcc])
AC_PROG_CC_STDC
AC_PROG_CPP
AC_LANG([C])



AC_USE_SYSTEM_EXTENSIONS
# Check for sed and grep
AC_PROG_SED
AC_PROG_GREP

AS_CASE([$host],
        [*linux*|*bsd*|*solaris*], [
         AC_MSG_NOTICE([Linux/BSD/Solaris])
         AC_LANG_ASSERT([C])
         AC_CONFIG_FILES([src/Makefile img/Makefile])
         PKG_PROG_PKGCONFIG()
        ],
        [*cygwin*|mingw32*], [
         AC_MSG_NOTICE([Windows])
         AC_CONFIG_FILES([src/mswin/Makefile])
        ],
        [AC_MSG_ERROR([System is not supported])])


# Check for pkg-config

# Check build_os and set ZB_BOS accordingly
AS_CASE([$host],
        [*linux*], [
         AC_CHECK_FILES([/sys/class/power_supply],
                        ,
                        AC_MSG_ERROR([Linux kernels older than 2.6.24 are not supported.]))
         ],
        [*bsd*], [
         AC_CHECK_FUNC([sysctlbyname],
                       ,
                       AC_SEARCH_LIBS([sysctlbyname],
                                      [c],
                                      ,
                                      [AC_MSG_ERROR([*BSD systems without 'sysctlbyname' are not supported.])]))
         ],
        [*cygwin*|*mingw32*], [
         AC_CHECK_HEADERS([Windows.h windows.h \
                           Winapifamily.h winapifamily.h \
                           _mingw_unicode.h specstrings.h \
                           Shellapi.h shellapi.h],,,)
         AM_CONDITIONAL([MSWIN], [test "xalways_true" = "xalways_true"])
         ],
         [*solaris*], [
          AC_CHECK_HEADERS([sys/pm.h],,,)
          ],
          [] # default
         )

[CPUARCH="`echo $host | $ac_cv_path_SED 's/^\([a-z|0-9|_]\+\)-.\+$/\1/g'`"]
AC_SUBST([CPUARCH])

# Precious variables
AC_ARG_VAR([CYTHON], [Cython to C compiler (only used when '--with-X' is present)])

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

AM_CONDITIONAL([X], [test "x$with_X" != "xno"])

#IF_WINDOWS(,[AS_IF([test "x$with_gtk3" = "xyes"],
#[PKG_CHECK_MODULES([gtk3], [gtk+-3.0 >= 3.0],
        #[AC_DEFINE([HAVE_GTK3], [1])])],
        #[test "x$with_gtk2" = "xyes"],
#[PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.0],
        #[AC_DEFINE([HAVE_GTK2], [1])])])])

# Check if we have GTK+2
# Travis
AC_MSG_CHECKING([whether build is automated])
AM_CONDITIONAL([TRAVIS], [test "x$TRAVIS" = "xtrue"])
AM_COND_IF([TRAVIS], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])

# Check for libs
#AS_IF([! test x"$ZB_HOST" = "xwindows"], AC_SEARCH_LIBS([nearbyint], [m],,[AC_MSG_ERROR([Please use a C99 compliant compiler])]), )

## Check for OS specifics
#AS_IF([test x"$ZB_HOST" = x"linux"], [AM_COND_IF([TRAVIS], AC_DEFINE([HAVE__SYS_CLASS_POWER_SUPPLY], [1]),
	    #AS_IF([test x"$ZB_HOST" = x"linux"],
	     #[AC_CHECK_FILES([/sys/class/power_supply],,
		#AC_MSG_ERROR([Linux systems  without `/sys/class/power_supply' are not supported]))],))])

#AS_IF([test x"$ZB_HOST" = "xbsd"],
      #AC_SEARCH_LIBS([sysctlbyname],
                     #[c],
                     #,
                     #[AC_MSG_ERROR([*BSD systems without `sysctlbyname' are not supported])]))

#AC_CHECK_TYPES([INT_MAX],, [AC_MSG_WARN([INT_MAX is not defined])], [#include <limits.h>])



#IF_WINDOWS(,[AM_COND_IF([X],[AC_CHECK_PROGS([CYTHON], [cython cython2],[no])])
#AM_COND_IF([X],[AS_IF([test "x$ac_cv_prog_CYTHON" = "xcython2"],[PKG_CHECK_MODULES([python2],[python2],,)],[test "x$ac_cv_prog_CYTHON" = "xcython"],[PKG_CHECK_MODULES([python2],[python2],,)],[AC_MSG_ERROR([Cython is a make dependency for X server support])])])])

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

AC_OUTPUT
