AC_PREREQ([2.65])
AC_INIT([Zelda Battery], [0.0.3a], [sickhadas.nix@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall foreign])

# Check for programs.
AC_PROG_CC
AC_PROG_CC_STDC
AC_PATH_PROG([CSC], [csc])
AC_PATH_PROG([CHICKEN], [chicken])
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_SED
AC_PROG_AWK

AC_CHECK_PROG([PMSET], [pmset], [yes], [no])
AC_CHECK_PROG([ACPI], [acpi], [yes], [no])
AM_CONDITIONAL([PMSET], [test x"$ac_cv_prog_PMSET" = x"yes"])
AM_CONDITIONAL([ACPI], [test x"$ac_cv_prog_ACPI" = x"yes"])

# Check if we're on Mac OS (X)
AC_MSG_CHECKING([whether os runs Darwin])
AM_CONDITIONAL([DARWIN], [test x`uname` = xDarwin])
AM_COND_IF([DARWIN], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])

# Check if we're on Linux
AC_MSG_CHECKING([whether os runs Linux])
AM_CONDITIONAL([LINUX], [test x`uname` = xLinux])
AM_COND_IF([LINUX], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])

# Check whether this is an automated build
AC_MSG_CHECKING([whether build is automated])
AM_CONDITIONAL([TRAVIS], [test x`echo $TRAVIS` = xtrue])
AM_COND_IF([TRAVIS], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])])

# Check for profiling
AC_ARG_ENABLE([profiling],
              [  --enable-profiling     Compiled executable will emit profiling information]
              [case "${enableval}" in
               yes) profiling=true ;;
               no) profiling=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-profiling]) ;;
             esac],[profiling=false])
AM_CONDITIONAL([PROFILING], [test x$profiling = xtrue])

# Check for debug
AC_ARG_ENABLE([debug],
              [  --enable-debug         Turn on debugging],
              [case "${enableval}" in
               yes) debug=true ;;
               no) debug=false ;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
             esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_SUBST([CHICKEN_FLAGS], [""])
AC_SUBST([CSCFLAGS], [""])
AC_OUTPUT
