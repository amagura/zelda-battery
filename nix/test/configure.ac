AC_INIT([])

# Check Zbatt output length
AC_MSG_CHECKING([zbatt's output length])
m4_define([zbatt_output_len], m4_len(m4_chomp(m4_esyscmd([../zbatt -f+ -e-]))))
m4_if(zbatt_output_len, [10],
	[AC_MSG_RESULT([10])],
	[AC_MSG_RESULT(zbatt_output_len)
	AC_MSG_ERROR([zbatt should never print more or less than 10 characters])])
