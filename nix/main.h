/****
Copyright 2014, 2015 Alexej Magura

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
# include <stdlib.h>

# ifndef ZB_DEBUG
#  define ZB_DEBUG 0 // XXX change this to turn debug messages on/off
# endif

# if ZB_DEBUG
#  ifndef ZB_DLEVEL
#   define ZB_DLEVEL 2 // XXX change this to increase/decrease debug verbosity
#  endif
# endif

# ifndef ZB_USE_KCAT
#  define ZB_USE_KCAT 1 // XXX change this to turn kcat on/off
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
#  define ZB_SDBG(format, exp)				\
     do {						\
	  fprintf(stderr, "## (%s)(%s)%d\n",		\
		  ZB_PROGNAME, __FILE__, __LINE__);	\
	  fprintf(stderr, "#  `%s`\n", __FUNCTION__);	\
	  fprintf(stderr, (format), (exp));		\
	  fprintf(stderr, "\n");			\
     } while(0)
#  define ZB_ONDBG(...) (__VA_ARGS__)
#  define ZB_XONDBG(ZB_X) ZB_X
#  define zb_ping ZB_DBG("\n^^^^ %s ^^^^\n", "MARCO!")
#  define zb_pong ZB_DBG("\n$$$$ %s $$$$\n", "POLO!")
#  define zb_neko(ZB_F, ...)				\
     do {						\
	  fprintf(stderr,				\
		  "\n%s{neko-chan}%s(%s)(%s)(%d)\n",	\
		  "\033[91m❤\033[0m",			\
		  "\033[91m❤\033[0m",			\
		  __FILE__, __FUNCTION__, __LINE__);	\
	  fprintf(stderr, "%s%s%s, %s%s%s~\n",		\
		  "\033[32mn",				\
		  "\033[35my",				\
		  "\033[31ma\033[0m",			\
		  "\033[32mn",				\
		  "\033[35my",				\
		  "\033[31ma\033[33ma\033[0m");		\
	  fprintf(stderr, (ZB_F), ##__VA_ARGS__);	\
	  fprintf(stderr, "\n");			\
     } while(0)
# else
#  define ZB_DBG(format, ...)
#  define ZB_SDBG(format, exp)
#  define ZB_ONDBG(...)
#  define ZB_XONDBG(ZB_X)
#  define zb_ping
#  define zb_pong
#  define zb_neko(ZB_F, ...)
# endif


# define ZB_FATAL(...)			\
     do {				\
	  fprintf(stderr,		\
		  "%s: %s\n",		\
		  (ZB_PROGNAME),	\
		  ##__VA_ARGS__);	\
	  exit(EXIT_FAILURE);		\
     } while (0)


# undef bzero
# define bzero(COM_B, COM_LEN)			\
     (memset((void *)(COM_B),			\
	     '\0',				\
	     (size_t)(COM_LEN)),		\
      (void)0)

# undef mempcpy
# define mempcpy(COM_D, COM_S, COM_L)		\
     (memcpy((void *)(COM_D),			\
	     (const void *)(COM_S),		\
	     (size_t)(COM_L))			\
      + (size_t)(COM_L))

#  define mempmove(COM_D, COM_S, COM_L)		\
     (memmove((void *)(COM_D),			\
	      (const void *)(COM_S),		\
	      (size_t)(COM_L))			\
      + (size_t)(COM_L))

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

# define zb_eset(ZB_EPTR, ZB_ENO)				\
     do {							\
	  ZB_DBG("(ZB_ENO): %d\n", (ZB_ENO));			\
	  zb_ping;						\
	  if ((ZB_ENO) != PWR_OK) {				\
	       zb_ping;						\
	       *(ZB_EPTR)->vp = (ZB_ENO);			\
	       (ZB_EPTR)->last = (ZB_EPTR)->vp++;		\
	       zb_ping;						\
	  }							\
	  ZB_DBG("*(ZB_EPTR)->last: %d\n", *((ZB_EPTR)->last));	\
	  ZB_DBG("*(ZB_EPTR)->vp: %d\n", *((ZB_EPTR)->vp));	\
	  zb_pong;						\
     } while (0)

# define zb_esync(ZB_EPTR)			\
     do {					\
	  ZB_EPTR->last = ZB_EPTR->pos++;	\
     } while(0)

/*
# define zb_efree(ZB_EPTR)			\
     do {					\
*/
/** $$ Macros $$ **/

/** concat: catenate several strings together **/
size_t concatl PARAMS((char *dest, size_t destsiz, const char *s1, ...));
size_t concatm PARAMS((char *dest, size_t destsiz, const char *s1, ...));
# undef catl
# define catl(...) (concatl(##__VA_ARGS__, (void *)NULL))
# undef catm
# define catm(...) (concatm(##__VA_ARGS__, (void *)NULL))

/** intlen: find how many digits a given integral contains. **/
size_t intlenm PARAMS((int n));

/** rev: reverse an array of characters **/
void rev PARAMS((char *s));

/** itoa: convert a number to an atom (i.e. string) **/
void itoa PARAMS((char *dst, int src));

enum pwrsuply {
     PWR_OK = 0,
     PWR_DEFAULT = -1,
     PWR_ENOBAT = -2,
     PWR_ENOAC = -3,
     PWR_ENOSUPLY = -5,
     PWR_ENOREAD = -6,
     PWR_ENOWANT = -7,
     PWR_ELIMIT = 16,
     PWR_EBRK = -8
};

#endif /* ZB_MAIN_H_GUARD */
