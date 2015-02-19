/****
Copyright 2014-2015 Alexej Magura

This file is a part of ZBatt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****/
#ifndef ZB_MAIN_H_GUARD
# define ZB_MAIN_H_GUARD 1
/* PARAMS is a macro used to wrap function prototypes, so that
  compilers that don't understand ANSI C prototypes still work,
  and ANSI C compilers can issue warnings about type mismatches. */
# undef PARAMS
# if defined (__STDC__) || defined (_AIX) \
       || (defined (__mips) && defined (_SYSTYPE_SVR4)) \
       || defined(WIN32) || defined(__cplusplus)
#  define PARAMS(protos) protos
# else
#  define PARAMS(protos) ()
# endif

# include <errno.h>
# include "compat.h"

/** ^^ Macros ^^ **/
# ifndef ZB_DEBUG
#  define ZB_DEBUG 1 // XXX change this to turn debug messages on/off
# endif

# ifndef ZB_USE_KCAT
#  define ZB_USE_KCAT 1
# endif

/*** ^^ Externally Defined macros ^^ ***/
# ifndef ZB_PROGNAME
#  define ZB_PROGNAME "ZBatt"
# endif

# ifndef PACKAGE_VERSION
#  define PACKAGE_VERSION ""
# endif
/*** $$ Externally Defined macros $$ ***/

# if ZB_DEBUG
#  if ZB_LINUX
#   include <mcheck.h>
#  endif
#  define ZB_DBG(format, ...)						\
     do {								\
	  fprintf(stderr, "## (%s)(%s)%d\n",				\
		  ZB_PROGNAME, __FILE__, __LINE__);			\
	  fprintf(stderr, "#  `%s'\n", __FUNCTION__);			\
	  fprintf(stderr, (format), ##__VA_ARGS__);			\
	  fprintf(stderr, "\n");					\
     } while(0)
#  define ZB_ONDBG(...) (__VA_ARGS__)
#  define ZB_XONDBG(ZB_X) ZB_X
# else
#  define ZB_DBG(format, ...)
#  define ZB_ONDBG(...)
#  define ZB_XONDBG(ZB_X)
# endif

# define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
# undef mempcpy
# define mempcpy(ZB_DST, ZB_SRC, ZB_LEN) (memcpy((ZB_DST), (ZB_SRC), (ZB_LEN)) + ZB_LEN)

# define ZB_ERROR(format, ...)						\
     do {								\
	  fprintf(stderr, "%s:err: ", ZB_PROGNAME);			\
	  fprintf(stderr, (format), __VA_ARGS__);			\
	  fprintf(stderr, "\nin %s:{%d}:%s()\n", __FILE__, __LINE__, __FUNCTION__); \
     } while(0)

# define zb_usage(format) printf((format), (ZB_PROGNAME));
# define zb_arg(opt, desc, tabs)			\
     do {						\
	  printf("  %s%s%s\n", (opt),(tabs),(desc));	\
     } while(0)
# define zb_arg_eol_tabs "\n\t\t\t\t"
# define zb_help(usage, tabs)				\
     do {						\
	  zb_usage((usage));				\
	  zb_arg("-h, --help",				\
		 "print this message and exit",		\
		 (tabs));				\
	  zb_arg("-v, --version",			\
		 "print program version and exit",	\
		 (tabs));				\
     } while(0)

# if HAVE_LIBBSD
#  include <limits.h>
#  include <bsd/stdlib.h>
// FIXME, the following macros shouldn't call `exit' or `perror'.
#  define ZB_STRTONUM(dst_num, const_string)				\
     do {								\
	  errno = 0;							\
	  ((dst_num) = strtonum((const_string),				\
				INT_MIN,				\
				INT_MAX,				\
				NULL));					\
	  if (errno != 0) {						\
	       perror(ZB_PROGNAME);					\
	       exit(EXIT_FAILURE);					\
	  }								\
     } while(0)
# else
#  define ZB_STRTONUM(dst_num, const_string)			\
     do {							\
	  errno = 0;						\
	  ((dst_num) = strtol((const_string), NULL, 10));	\
	  if (errno != 0) {					\
	       perror(ZB_PROGNAME);				\
	       exit(EXIT_FAILURE);				\
	  }							\
     } while(0)
# endif
/** $$ Macros $$ **/

# if ZB_SENTINEL
char *neko PARAMS((const char *s1, ...)) __attribute__ ((__sentinel__));
# else
char *neko PARAMS((const char *s1, ...));
# endif

enum pwrsuply {
     PWR_OK = 0,
     PWR_ERR = -1,
     PWR_NBAT = -2,
     PWR_NAC = -3,
     PWR_NSUPLY = -5,
     PWR_NREAD = -6
};
#endif /* ZB_MAIN_H_GUARD */
