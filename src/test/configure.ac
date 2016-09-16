AC_INIT([])

# Check Zbatt output length
AC_MSG_CHECKING([zbatt's output length])
m4_define([zbatt_output_len], m4_len(m4_chomp(m4_esyscmd([../zbatt -f+ -e-]))))
m4_if(zbatt_output_len, [10],
	[AC_MSG_RESULT([10])],
	[AC_MSG_RESULT(zbatt_output_len)
	AC_MSG_ERROR([zbatt should never print more or less than 10 characters])])
# Should we use gtk2
AC_ARG_WITH([gtk2],
	AS_HELP_STRING([--with-gtk2], [enable building the gui using gtk2 (default: disabled)]),
	[with_gtk2=yes],
	[])

# Check gzbatt's option handling
AS_IF([test "x$with_gtk2" = "xyes"], [AC_MSG_CHECKING([exit status of gzbatt -c is non-zero])
m4_define([gzbatt_ecode], m4_chomp(m4_esyscmd([../gzbatt -c 1> /dev/null 2> /dev/null; echo $?])))
AS_IF([test "x]gzbatt_ecode[" != "x0"],
		    [AC_MSG_RESULT(gzbatt_ecode)],
		    [AC_MSG_RESULT(gzbatt_ecode)
		    AC_MSG_ERROR([gzbatt should fail when -c is passed without argument])])])
