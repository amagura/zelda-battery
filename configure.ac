#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ZBatt], [3.1.0], [agm2819@gmail.com],, [https://github.com/amagura/zelda-battery])
#AC_CONFIG_SRCDIR([test.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])

# Check for system extensions
AC_USE_SYSTEM_EXTENSIONS

# Check for targets
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# M4 Code
m4_define([__pi_natsolaris], [AS_CASE([$zb_build_type], [native::x*solaris*], [$1], [])])
m4_define([__pi_natbsd], [AS_CASE([$zb_build_type], [native::x*bsd*], [$1], [])])

m4_define([__pi_natsolaris_blk], [if echo "x$zb_build_type" | $ac_cv_path_GREP -q -- 'xnative::x.*solaris.*'; then])
m4_define([__pi_natsolaris_klb], [fi])

m4_define([__pi_natbsd_blk], [if echo "x$zb_build_type" | $ac_cv_path_GREP -q -- 'xnative::x.*bsd.*'; then])
m4_define([__pi_natbsd_klb], [fi])

m4_define([__pi_natunix_blk], [if echo "x$zb_build_type" | $ac_cv_path_GREP -q -- 'xnative::x.*(bsd|linux|solaris).*'; then])
m4_define([__pi_natunix_klb], [fi])

# Check if we are cross compiling
if test "x$build" = "x$host" = "x$target"; then
  zb_build_type="native::x$host..."
elif test "x$build" = "x$host" \
  && test "x$host" != "x$target"; then
  zb_build_type="cross::x$target..."
elif test "x$build" != "x$host" \
  && test "x$build" != "x$target" \
  && test "x$host" != "x$target"; then
  zb_build_type="canadian::x$build::x$host::x$target..."
elif test "x$build" != "$host" && test "x$host" = "x$target" \
  && test "x$build" != "$target"; then
  zb_build_type="cross-native::x$build::x$host::x$target..."
fi

AS_CASE([$zb_build_type],
          [native::*linux*|native::*bsd*|native::*solaris*],[
           AC_PROG_CC
           PKG_PROG_PKG_CONFIG()
           [break]
          ],
          [native::*cygwin*|native::*mingw*],[
           AC_PROG_CXX
           [break]
          ],)

dnl I only really need to support cross-building for windows

# Checks for programs.
AC_PROG_INSTALL
AC_PATH_PROGS_FEATURE_CHECK([GREP], [grep ggrep], [[echo 'a' | ac_cv_path_GREP -q -- 'a']])

# Checks for libraries.
AC_ARG_WITH([x],
            AS_HELP_STRING([--without-x],
                           [disable x server support (default: enabled for gtk2)])
            AS_HELP_STRING([],
                           [supported options:
                                                gtk2
                                                gtk3], [15], [30]),
            [with_x=gtk2],
            [with_x=no])

if test "x$with_x" != "xno" && test "x$with_x" = "xgtk3"; then
  PKG_CHECK_MODULES([GTK3], [gtk+-3.0 >= 3.0], AC_DEFINE([HAVE_GTK3], [1], [Defined when gtk3 is present.]))
elif test "x$with_x" != "xno" && test "x$with_x" = "xgtk2"; then
    PKG_CHECK_MODULES([GTK2], [gtk+-2.0 >= 2.0], AC_DEFINE([HAVE_GTK2], [1], [Defined when gtk2 is present.]))
fi
#AS_IF([test "x$with_X" != "xno" && test "x$with_X" = "xgtk3"],
      #[PKG_CHECK_MODULES([gtk3], [gtk+-3.0 >= 3.0], AC_DEFINE([HAVE_GTK3], [1], [Define if gtk3 is present.]))])
#AS_IF([test "x$with_X" != "xno" && test "x$with_X" = "xgtk2"],
      #[PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.0], AC_DEFINE([HAVE_GTK2], [1], [Define if gtk2 is present.]))])


# Check for needed items.
#AS_CASE([$zb_build_type],
        #[native::x*solaris*],[AC_CHECK_HEADERS(

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h sys/ioctl.h])
__pi_natsolaris([AC_CHECK_HEADERS([sys/pm.h])])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero memmove mempcpy memset strchr strerror strtol])

__pi_natunix_blk
  AC_SEARCH_LIBS([nearbyint], [m],
                 ,
                 [AC_MSG_ERROR([nearbyint is not defined by libm: use a C99 compliant compiler])])
__pi_natunix_klb

__pi_natbsd_blk
  AC_CHECK_FUNC([sysctlbyname],
                ,
                AC_SEARCH_LIBS([sysctlbyname],
                               [c],
                               ,
                               [AC_MSG_WARN([*BSD systems without 'sysctlbyname' are not supported.])]))
__pi_natbsd_klb

# Check for files.
if test "x$zb_build_type" = "xnative::xlinux;;"; then
  AC_CHECK_FILES([/sys/class/power_supply],
                 ,
                 [AC_MSG_WARN([Linux kernels older than 2.6.24 are not supported.])])
fi

AC_CONFIG_FILES([Makefile
                 img/Makefile
                 src/Makefile
                 src/mswin/Makefile])
#AC_CONFIG_SUBDIRS([old
                   #src/test])
# Automake directives
dnl `foreign' prevents autotools from installing a copy of the GPL
dnl `-Wall' gets rid of unnecessary warnings
AM_INIT_AUTOMAKE([foreign -Wall])

AM_CONDITIONAL([X], [test "x$with_x" != "xno"])
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
