AC_PREREQ([2.65])
AC_INIT([Zelda Battery], [1.1], [agm2819@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall])

# Check for compilers
dnl C
AC_PROG_CC
AC_PROG_CC_STDC
AC_C_INLINE
AC_PROG_SED

# Check for TR
AC_CHECK_PROG([TR], [tr], [yes], [no])

# Declare some precious variables
m4_define([PLATFORM], m4_esyscmd([uname -s | tr '\n' ' ' | sed 's/ $//']))
m4_define([CPUTYPE], m4_esyscmd([uname -m | tr '\n' ' ' | sed 's/ $//']))
m4_define([_TEST], [test x"$1" = x"$2"])
m4_define([FATAL_TEST], [test x"$1" = x"$2" && exit 1])
#m4_if(PLATFORM, [Linux] [AM_CONDITIONAL([Linux

# Check for libs
AC_CHECK_LIB([acpi], [check_acpi_support],,FATAL_TEST(PLATFORM, [Linux]))
AC_CHECK_LIB([c], [sysctlbyname],,FATAL_TEST(PLATFORM, [BSD]))
AM_CONDITIONAL([LINUX], _TEST(PLATFORM, [Linux]))#  [test x]PLATFORM[= x"Linux"])#m4_if(PLATFORM, [Linux], _TRUE))
AM_CONDITIONAL([BSD], _TEST(PLATFORM, [BSD]))# [test x]PLATFORM[= x"BSD"]) #m4_if(PLATFORM, [BSD], _TRUE))

# Check for programs
#AC_CHECK_PROG([PMSET], [pmset], [yes], [no])
#AC_CHECK_PROG([ACPI], [acpi], [yes], [no])
#AC_CHECK_PROG([APM], [apm], [yes], [no]) # *BSD support
#AM_CONDITIONAL([PMSET], [test x"$ac_cv_prog_PMSET" = x"yes"])
#AM_CONDITIONAL([ACPI], [test x"$ac_cv_prog_ACPI" = x"yes"])
#AM_CONDITIONAL([APM], [test x"$ac_cv_prog_APM" = x"yes"])

# Check whether this is an automated build
AC_MSG_CHECKING([whether build is automated])
AM_CONDITIONAL([TRAVIS], [test x`echo $TRAVIS` = xtrue])
AM_COND_IF([TRAVIS], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])


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

# Enable or disable blinking altogether
#AC_ARG_ENABLE([blink],
#              [  --enable-blinking      includes support for battery indicator blinking (default)],
#              [case "${enableval}" in
#               yes) blink=true ;;
#               no) blink=false ;;
#               *) AC_MSG_ERROR([bad value ${enableval} for --enable-blinking]) ;;
#             esac],[blink=true])
#AM_CONDITIONAL([BLINK], [test x$blink = xtrue])

## Enable blinking, even when on AC (Alternating Current; not AutoConf) Power
#AC_ARG_ENABLE([acblink],
#              [  --enable-acblink       blink on low battery, even when on AC power],
#              [case "${enableval}" in
#               yes) acblink=true ;;
#               no) acblink=false ;;
#               *) AC_MSG_ERROR([bad value ${enableval} for --enable-acblink]) ;;
#             esac],[acblink=false])
#AM_CONDITIONAL([ACBLINK], [test x$acblink = xtrue])

# Check for debug
#AC_ARG_ENABLE([debug],
              #[  --enable-debug         turn on debugging],
              #[case "${enableval}" in
               #yes) debug=true ;;
               #no) debug=false ;;
               #*) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
             #esac],[debug=false])
#AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

#AC_ARG_ENABLE([heartbeat],
              #[  --enable-heartbeat     right-most heart changes from bold to regular font repeatedly],
              #[case "${enableval}" in
               #yes) heartbeat=true ;;
               #no) heartbeat=false ;;
               #*) AC_MSG_ERROR([bad value ${enableval} for --enable-heartbeat]) ;;
             #esac],[heartbeat=false])
#AM_CONDITIONAL([HEARTBEAT], [test "x$heartbeat" = xtrue])

AM_CONDITIONAL([CPU_KNOWN], [test ! "x]CPUTYPE[" = x && test ! "x]CPUTYPE[" = xunknown])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
