AC_PREREQ([2.65])
AC_INIT([ZBatt], [2.1], [agm2819@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_USE_SYSTEM_EXTENSIONS

# Check for compilers
dnl C
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CPP

# Precious variables

# let Automake know if we are using clang

# M4 Macros
m4_define([rm_newline], [translit($1, [
])])
m4_define([rm_dots], [translit($1, [.])])
m4_define([PLATFORM], rm_newline(m4_esyscmd([uname -s])))
m4_define([CPUARCH], rm_newline(m4_esyscmd([uname -m])))
m4_define([IS_LINUX], [m4_if($1, [Linux], TRUE_TEST($1), FALSE_TEST($1))])
m4_define([TRUE_TEST], [test x"$1" = x"$1"])
m4_define([FALSE_TEST], [test x"$1" = x"not $2"])
m4_define([IF_LINUX], [m4_if(
$1, [Linux],[$2],[$3])])
m4_define([IF_BSD], [m4_if(
$1, [NetBSD],[$2],
$1, [FreeBSD],[$2],
$1, [OpenBSD],[$2],
$1, [DragonFly],[$2],[$3])])
m4_define([IS_BSD], [IF_BSD($1, [test x"$1" = x"$1"], [test x"$1" = x"not a BSD"])])
m4_define([CMP], [m4_if(m4_eval(([$1] == [$2])), 1, 0, m4_if(m4_eval(([$1] > [$2])), 1, 1, -1))])
m4_define([KERNEL], rm_newline(m4_bpatsubst(m4_esyscmd([uname -r]), [[^.0-9].*], [])))
m4_define([IF_NEWER_LINUX], [IF_LINUX([PLATFORM], [pushdef([result],CMP([$1], rm_dots([2.6.24])))
m4_if(result,1,[$2],result,0,[$2],[result],[-1],[$3])],,)])

AC_ARG_WITH([gtk2],
	AS_HELP_STRING([--without-gtk2], [disable building the gui]),
	[],
	[with_gtk2=yes])
#m4_define([GTK2_CFLAGS], rm_newline(m4_esyscmd([pkg-config --cflags gtk+-2.0])))
#m4_define([GTK2_LIBS], rm_newline(m4_esyscmd([pkg-config --libs gtk+-2.0])))

#m4_if(PLATFORM, [Linux] [AM_CONDITIONAL([Linux

# Check if we have GTK+2
AM_CONDITIONAL([GTK2], [test "x$with_gtk2" = "xyes"])
AM_COND_IF([GTK2],[PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.0], [HAVE_GTK="yes"], [no])],)

# Travis
AC_MSG_CHECKING([whether build is automated])
AM_CONDITIONAL([TRAVIS], [test "x`echo -n $TRAVIS`" = "xtrue"])
AM_COND_IF([TRAVIS], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])

# Check for libs
AM_COND_IF([TRAVIS], AC_DEFINE([HAVE__SYS_CLASS_POWER_SUPPLY], [1]), IF_NEWER_LINUX(rm_dots(KERNEL),AC_CHECK_FILES([/sys/class/power_supply],,AC_MSG_ERROR([Linux kernel versions >= 2.6.24 without /sys/class/power_supply are unsupported])),AC_CHECK_FILES([/proc/acpi/ac_adapter],,AC_MSG_ERROR([Linux kernel version < 2.6.24 without /proc/acpi/ac_adapter are unsupported]))))

#AM_CONDITIONAL([LINUX], IS_LINUX([PLATFORM]))#  [test x]PLATFORM[= x"Linux"])#m4_if(PLATFORM, [Linux], _TRUE))
#AM_CONDITIONAL([BSD], IS_BSD([PLATFORM]))# [test x]PLATFORM[= x"BSD"]) #m4_if(PLATFORM, [BSD], _TRUE))
IF_BSD([PLATFORM], AC_SEARCH_LIBS([sysctlbyname], [c],AC_DEFINE([HAVE_SYSCTLBYNAME],[1])m4_define([HAVE_SYSCTLBYNAME], [1]),))
IF_BSD([PLATFORM], AC_SEARCH_LIBS([sysctl], [c],AC_DEFINE([HAVE_SYSCTL],[1])m4_define([HAVE_SYSCTL], [1]),))
IF_BSD([PLATFORM],m4_if([HAVE_SYSCTLBYNAME], [1],,[HAVE_SYSCTL], [1],, [AC_MSG_ERROR([*BSD systems without `sysctlbyname` or `sysctl` are unsupported])]))

# Check for progs
AM_COND_IF([GTK2],[AC_CHECK_PROGS([CYTHON], [cython cython2],[no])])
AM_CONDITIONAL([HAVE_CYTHON],[test ! "x$ac_cv_prog_CYTHON" = "xno" && test ! "x$ac_cv_prog_CYTHON" = "x"])
AM_COND_IF([HAVE_CYTHON],[PKG_CHECK_MODULES([py2],[python2],,)],AC_MSG_ERROR([Cython is a make dependency for the GUI]))

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
             esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test "x$debug" = xtrue])
# generic?
m4_define([CLANG], [test "x`($CC -E -w -P -nostdinc - <<EOF
__clang__
EOF
)`" = "x1"])
m4_define([TINYC], [test ! "x`($CC -E -w -P -nostdinc - <<EOF
__TINYC__
EOF
)`" = "x__TINYC__"])
AM_CONDITIONAL([CPU_KNOWN], [test ! "x]CPUARCH[" = "x" && test ! "x]CPUARCH[" = "xunknown" && (]CLANG[ || ]TINYC[)])
AC_SUBST(CPUTYPE, CPUARCH)

AC_CONFIG_FILES([Makefile src/Makefile img/Makefile])
AC_OUTPUT
