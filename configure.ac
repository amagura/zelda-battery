AC_PREREQ([2.65])
AC_INIT([ZBatt], [2.0], [agm2819@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_USE_SYSTEM_EXTENSIONS

# Check for compilers
dnl C
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CPP
AC_C_INLINE
AC_FUNC_ALLOCA

# M4 Macros
m4_define([rm_newline], [translit($1, [
])])
m4_define([rm_dots], [translit($1, [.])])
m4_define([PLATFORM], rm_newline(m4_esyscmd([uname -s])))
m4_define([CPUTYPE], rm_newline(m4_esyscmd([uname -m])))
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
#m4_if(PLATFORM, [Linux] [AM_CONDITIONAL([Linux

# Check for libs
IF_NEWER_LINUX(rm_dots(KERNEL),AC_CHECK_FILES([/sys/class/power_supply],,AC_MSG_ERROR([Linux kernel versions >= 2.6.24 without /sys/class/power_supply are unsupported])),AC_CHECK_FILES([/proc/acpi/ac_adapter],,AC_MSG_ERROR([Linux kernel version < 2.6.24 without /proc/acpi/ac_adapter are unsupported])))

#AM_CONDITIONAL([LINUX], IS_LINUX([PLATFORM]))#  [test x]PLATFORM[= x"Linux"])#m4_if(PLATFORM, [Linux], _TRUE))
#AM_CONDITIONAL([BSD], IS_BSD([PLATFORM]))# [test x]PLATFORM[= x"BSD"]) #m4_if(PLATFORM, [BSD], _TRUE))
IF_BSD([PLATFORM], AC_CHECK_LIB([c], [sysctlbyname],,AC_MSG_ERROR([*BSD systems without `sysctlbyname' are unsupported])),)

# Check if peeps want to try to compile to native (compiler may or may not support it)
AC_ARG_ENABLE([native],
              AS_HELP_STRING([--enable-native], [enable processor-model specific compilation]),
              [case "${enableval}" in
               yes) native=true ;;
               no) native=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-native]) ;;
             esac],[native=true])
AM_CONDITIONAL([NATIVE], [test "x$native" = xtrue])

# Check if peeps want to try to compile to generic (compiler may or may not support it)
AC_ARG_ENABLE([generic],
              AS_HELP_STRING([--enable-generic], [enable processor-family specific compilation]),
              [case "${enableval}" in
               yes) generic=true ;;
               no) generic=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-generic]) ;;
             esac],[generic=false])
AM_CONDITIONAL([GENERIC], [test "x$generic" = xtrue])

# Check for debug
AC_ARG_ENABLE([debug],
              [  --enable-debug         turn on debugging],
              [case "${enableval}" in
               yes) debug=true ;;
               no) debug=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
             esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test "x$debug" = xtrue])
AM_CONDITIONAL([CPU_KNOWN], [test ! "x]CPUTYPE[" = "x" && test ! "x]CPUTYPE[" = "xunknown"])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
