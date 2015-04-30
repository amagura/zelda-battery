AC_PREREQ([2.65])
AC_INIT([ZBatt], [3.0.3], [agm2819@gmail.com],,[https://github.com/amagura/zelda-battery])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall])
AC_USE_SYSTEM_EXTENSIONS

m4_define([rm_newline], [translit($1, [
])])
m4_define([PLATFORM], rm_newline(m4_esyscmd([uname -o])))
m4_define([IF_WINDOWS], [m4_if(PLATFORM, [Cygwin], [$1], [$2])])

# Check for compilers
dnl C
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CPP
AC_PROG_SED
AC_PROG_GREP

# Get System Type
AC_CANONICAL_HOST

# Check for pkg-config
PKG_PROG_PKG_CONFIG()

# host checking
AS_CASE([$host],
  [*linux*], [ZB_HOST="linux"]
  	     [m4_define([ZB__HOST], [linux])],
  [*pc-cygwin], [ZB_HOST="windows"]
  		[m4_define([ZB__HOST], [windows])],
  [*bsd*], [ZB_HOST="bsd"]
  	   [m4_define([ZB__HOST], [bsd])],
  [ZB_HOST=""][m4_define([ZB__HOST], [])])
AC_SUBST([ZB_HOST])

[CPUARCH="`echo $host | $ac_cv_path_SED 's/^\([a-z|0-9|_]\+\)-.\+$/\1/g'`"]
AC_SUBST([CPUARCH])

# Precious variables
AC_ARG_VAR([CYTHON], [Cython to C compiler (only used when `--with-gtk' is present)])
# Should we use gtk2
AC_ARG_WITH([gtk2],
	AS_HELP_STRING([--without-gtk2], [disable building the gui (enabled by default)]),
	IF_WINDOWS([[with_gtk2=no]], [[with_gtk2=yes]]),
	IF_WINDOWS([[with_gtk2=no]], [[with_gtk2=yes]]))

IF_WINDOWS(,[PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.0], [HAVE_GTK="yes"], [HAVE_GTK="no"])])

# Check if we have GTK+2
AM_CONDITIONAL([GTK2], [test "x$with_gtk2" = "xyes"])

# Travis
AC_MSG_CHECKING([whether build is automated])
AM_CONDITIONAL([TRAVIS], [test "x$TRAVIS" = "xtrue"])
AM_COND_IF([TRAVIS], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])

# Check for libs
AS_IF([! test x"$ZB_HOST" = "xwindows"], AC_SEARCH_LIBS([nearbyint], [m],,[AC_MSG_ERROR([Please use a C99 compliant compiler])]), )

# Check for OS specifics
AS_IF([test x"$ZB_HOST" = x"linux"], [AM_COND_IF([TRAVIS], AC_DEFINE([HAVE__SYS_CLASS_POWER_SUPPLY], [1]))])
AS_IF([test x"$ZB_HOST" = x"linux"],
 [AC_CHECK_FILES([/sys/class/power_supply],,
   AC_MSG_ERROR([Linux systems  without `/sys/class/power_supply' are unsupported]))],)

AS_IF([test x"$ZB_HOST" = x"bsd"], [AC_SEARCH_LIBS([sysctlbyname], [c],,[AC_MSG_ERROR([*BSD systems without `sysctlbyname` are unsupported])])],)

# Check for progs
AM_COND_IF([GTK2],[AC_CHECK_PROGS([CYTHON], [cython cython2],[no])])
AM_COND_IF([GTK2],[AS_IF([test "x$ac_cv_prog_CYTHON" = "xcython2"],[PKG_CHECK_MODULES([python2],[python2],,)],[test "x$ac_cv_prog_CYTHON" = "xcython"],[PKG_CHECK_MODULES([python2],[python2],,)],[AC_MSG_ERROR([Cython is a make dependency for the GUI])])])

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

AC_CONFIG_FILES([Makefile src/Makefile img/Makefile])
AC_OUTPUT
