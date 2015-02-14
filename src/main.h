/****
Copyright 2014 Alexej Magura

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

/** ^^ Macros ^^ **/
# ifndef ZB_DEBUG
#  define ZB_DEBUG 1 // XXX change this to turn debug messages on/off
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
#  include <mcheck.h>
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

# define ZB_ERROR(format, ...)						\
     do {								\
	  fprintf(stderr, "%s:err: ", ZB_PROGNAME);			\
	  fprintf(stderr, (format), __VA_ARGS__);			\
	  fprintf(stderr, "\nin %s:{%d}:%s()\n", __FILE__, __LINE__, __FUNCTION__); \
     } while(0)

# define ZB_MSG(format, ...) printf((format), (__VA_ARGS__));
# define ZB_ARGMSG(...) printf("  %s\n", (__VA_ARGS__));

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

/* Alloca crap */
# ifdef STDC_HEADERS
#  include <stdlib.h>
#  include <stddef.h>
# else
#  ifdef HAVE_STDLIB_H
#   include <stdlib.h>
#  endif
# endif
# ifdef HAVE_ALLOCA_H
#  include <alloca.h>
# elif !defined alloca
#  ifdef __GNUC__
#   define alloca __builtin_alloca
#  elif defined _AIX
#   define alloca __alloca
#  elif defined _MSC_VER
#   include <malloc.h>
#   define alloca _alloca
#  elif !defined HAVE_ALLOCA
#   ifdef  __cplusplus
extern "C"
#   endif
void *alloca (size_t);
#  endif
# endif

/*** ^^ Compiler Compat macros ^^ ***/
# if !__clang__
#  ifndef __has_attribute
#   define __has_attribute(x) 0
#  endif
# endif

# if !__GNUC__
#  ifndef __attribute__
#   define __attribute__(x)
#  endif
# endif

# if __GNUC__
#  define ZB_SENTINEL __sentinel__
# elif defined(__sentinel__)
#  define ZB_SENTINEL __sentinel__
# elif defined(sentinel)
#  define ZB_SENTINEL sentinel
# else
#  if __has_attribute(__sentinel__)
#   define ZB_SENTINEL __sentinel__
#  elif __has_attribute(sentinel)
#   define ZB_SENTINEL sentinel
#  endif
# endif
/*** $$ Compiler Compat macros $$ ***/

/* OS Idenfitication macros */
# define ZB_LINUX (defined(__linux__) || defined(__gnu_linux__))
# define ZB_BSD (defined(__FreeBSD__)					\
		|| defined(__NetBSD__)					\
		|| defined(__OpenBSD__)					\
		|| defined(__DragonFly__))
# define ZB_UNIX (defined(__unix__) && !(ZB_BSD) && !(ZB_LINUX))

# if ZB_BSD
#  define ZB_IF_BSD(x) (x)
# else
#  define ZB_IF_BSD(x)
# endif

# if ZB_LINUX
#  define ZB_IF_LINUX(x) (x)
# else
#  define ZB_IF_LINUX(x)
# endif

# if ZB_UNIX
#  define ZB_IF_UNIX(x) (x)
# else
#  define ZB_IF_UNIX(x)
# endif
/** $$ Macros $$ **/

# if ZB_SENTINEL
char *concat PARAMS((const char *s1, ...)) __attribute__ ((__sentinel__));
# else
char *concat PARAMS((const char *s1, ...));
# endif

enum pwrsuply {
     ZB_PWR_OK = 0,
     ZB_PWR_NWANTBAT = -1,
     ZB_PWR_UNRDABL = -2,
     ZB_PWR_MISSING = -3,
     ZB_PWR_VIRTMACH = -4
};

#endif /* ZB_MAIN_H_GUARD */
