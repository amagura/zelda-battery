AC_PREREQ([2.65])
AC_INIT([Zelda Battery], [0.0.6a], [sickhadas.nix@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall foreign])

# Check for compilers
dnl C
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Declare some precious variables
AC_ARG_VAR([CSC], [driver program for the CHICKEN Scheme compiler])
AC_ARG_VAR([CHICKEN], [CHICKEN Scheme-to-C compiler])
AC_ARG_VAR([POWER_UTIL], [utility with which to take battery/power readings])

# Check to see if we need to look for `csc' and/or `chicken'.
AM_CONDITIONAL([NO_CHECK_FOR_CSC], [test ! x"$CSC" = x""])
AM_CONDITIONAL([NO_CHECK_FOR_CHICKEN], [test ! x"$CHICKEN" = x""])
AM_COND_IF([NO_CHECK_FOR_CSC], [AC_CHECK_PROG([CSC], [csc], [no])], [AC_PATH_PROG([CSC], [csc], [no])])
AM_COND_IF([NO_CHECK_FOR_CHICKEN], [AC_CHECK_PROG([CHICKEN], [chicken], [no])], [AC_PATH_PROG([CHICKEN], [chicken], [no])])

# Check for programs
AC_CHECK_PROG([PMSET], [pmset], [yes], [no])
AC_CHECK_PROG([ACPI], [acpi], [yes], [no])
AC_CHECK_PROG([YACPI], [yacpi], [yes], [no])
AC_CHECK_PROG([APM], [apm], [yes], [no]) # *BSD support
AC_CHECK_PROG([ACPICONF], [acpiconf], [yes], [no])
AM_CONDITIONAL([PMSET], [test x"$ac_cv_prog_PMSET" = x"yes"])
AM_CONDITIONAL([ACPI], [test x"$ac_cv_prog_ACPI" = x"yes"])
AM_CONDITIONAL([YACPI], [test x"$ac_cv_prog_YACPI" = x"yes" && test x"$ac_cv_prog_ACPI" = x"no"])
AM_CONDITIONAL([APM], [test x"$ac_cv_prog_APM" = x"yes"])
AM_CONDITIONAL([ACPICONF], [test x"$ac_cv_prog_ACPICONF" = x"yes" && test x"$ac_cv_prog_APM" = x"no"])

# Check whether this is an automated build
AC_MSG_CHECKING([whether build is automated])
AM_CONDITIONAL([TRAVIS], [test x`echo $TRAVIS` = xtrue])
AM_COND_IF([TRAVIS], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])


# Check if peeps want to try to compile to native (compiler may or may not support it)
AC_ARG_ENABLE([native],
              [  --enable-native        sets '-march=native' and '-mtune=native']
              [case "${enableval}" in
               yes) native=true ;;
               no) native=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-native]) ;;
             esac],[native=false])
AM_CONDITIONAL([NATIVE], [test x`echo $native` = xtrue])

# Check if peeps want to try to compile to generic (compiler may or may not support it)
AC_ARG_ENABLE([generic],
              [  --enable-generic       sets '-march=generic' and '-mtune=generic']
              [case "${enableval}" in
               yes) native=true ;;
               no) native=false ;;
               *) AC_MSG_ERROR([bad valie ${enableval} for --enable-generic]) ;;
             esac],[generic=false])
AM_CONDITIONAL([GENERIC], [test x`echo $generic` = xtrue])

# Check for profiling
AC_ARG_ENABLE([profiling],
              [  --enable-profiling     compiled executable will emit profiling information]
              [case "${enableval}" in
               yes) profiling=true ;;
               no) profiling=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-profiling]) ;;
             esac],[profiling=false])
AM_CONDITIONAL([PROFILING], [test x$profiling = xtrue])

# Enable or disable blinking altogether
AC_ARG_ENABLE([blinking],
              [  --enable-blinking      includes support for battery indicator blinking]
              [case "${enableval}" in
               yes) blinking=true ;;
               no) blinking=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-blinking]) ;;
             esac],[blinking=true])
AM_CONDITIONAL([BLINKING], [test x$blinking = xtrue])

# Enable blinking, even when on AC (Alternating Current; not AutoConf) Power
AC_ARG_ENABLE([acblink],
              [  --enable-ac-blink      blink on low battery, even when on AC power]
              [case "${enableval}" in
               yes) acblink=true;;
               no) acblink=false;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-ac-blink]) ;;
             esac],[acblink=false])
AM_CONDITIONAL([ACBLINK], [test x$acblink = xtrue])

# Check for debug
AC_ARG_ENABLE([debug],
              [  --enable-debug         turn on debugging],
              [case "${enableval}" in
               yes) debug=true ;;
               no) debug=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
             esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_SUBST([CHICKEN_FLAGS], ["$CHICKEN_OPTIONS"])
AC_SUBST([CSCFLAGS], ["$CSC_OPTIONS"])
#AC_MSG_CHECKING([for destdir])
#AM_CONDITIONAL([NO_DESTDIR], [test ! x"$DESTDIR" = x""])
#AM_COND_IF([NO_DESTDIR], [AC_SUBST([DESTDIR], ["$DESTDIR"])], [])
#AM_COND_IF([NO_DESTDIR], [AC_MSG_RESULT([$DESTDIR])], [AC_MSG_RESULT([no])])
AC_OUTPUT
