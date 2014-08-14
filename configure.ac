AC_PREREQ([2.65])
AC_INIT([Zelda Battery], [0.1.0], [sickhadas.nix@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall foreign])

# Check for compilers
dnl C
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_INSTALL

# Declare some precious variables
AC_ARG_VAR([CYTHON], [Python to C compiler])
AC_ARG_VAR([CPUTYPE], [machine hardware name (should be the output of 'uname -m')])

# Check to see if we need to look for `csc' and/or `chicken'.
AM_CONDITIONAL([NO_CHECK_FOR_CYTHON], [test ! x"$CYTHON" = x""])
AM_COND_IF([NO_CHECK_FOR_CYTHON], [AC_CHECK_PROG([CYTHON], [cython2 cython], [no])], [AC_PATH_PROG([CYTHON], [cython2 cython], [no])])

# Check for programs
AC_CHECK_PROG([PRINTF], [printf gprintf], [yes], [no]) dnl Needed by `make trial'
AC_CHECK_PROG([PMSET], [pmset], [yes], [no])
AC_CHECK_PROG([ACPI], [acpi], [yes], [no])
AC_CHECK_PROG([APM], [apm], [yes], [no]) # *BSD support
AM_CONDITIONAL([PMSET], [test x"$ac_cv_prog_PMSET" = x"yes"])
AM_CONDITIONAL([ACPI], [test x"$ac_cv_prog_ACPI" = x"yes"])
AM_CONDITIONAL([APM], [test x"$ac_cv_prog_APM" = x"yes"])

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
             esac],[native=false])
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

## Enable or disable blinking altogether
#AC_ARG_ENABLE([blinking],
              #[  --enable-blinking      includes support for battery indicator blinking (default)],
              #[case "${enableval}" in
               #yes) blinking=true ;;
               #no) blinking=false ;;
               #*) AC_MSG_ERROR([bad value ${enableval} for --enable-blinking]) ;;
             #esac],[blinking=true])
#AM_CONDITIONAL([BLINKING], [test x$blinking = xtrue])

## Enable blinking, even when on AC (Alternating Current; not AutoConf) Power
#AC_ARG_ENABLE([acblink],
              #[  --enable-acblink       blink on low battery, even when on AC power],
              #[case "${enableval}" in
               #yes) acblink=true ;;
               #no) acblink=false ;;
               #*) AC_MSG_ERROR([bad value ${enableval} for --enable-acblink]) ;;
             #esac],[acblink=false])
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

AS_IF([test ! "x$CPUTYPE" = x], ,
      [CPUTYPE=`uname -m`])

AM_CONDITIONAL([CPU_KNOWN], [test ! "x$CPUTYPE" = x && test ! "x$CPUTYPE" = xunknown])

AC_CONFIG_FILES([Makefile])
AC_SUBST([CYTHON_FLAGS], ["$CYTHON_FLAGS"])
AC_SUBST([CPUTYPE], ["$CPUTYPE"])
AC_OUTPUT
