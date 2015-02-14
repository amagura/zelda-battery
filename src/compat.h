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
#ifndef ZB_COMPAT_H_GUARD
# define ZB_COMPAT_H_GUARD 1
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

/** ^^ Macros ^^ **/

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
# define ZB_BSD ((__FreeBSD__)			\
		 || (__NetBSD__)		\
		 || (__OpenBSD__)		\
		 || (__DragonFly__))
# define ZB_LINUX ((__linux__)			\
		   || (__gnu_linux__))
# define ZB_UNIX ((__unix__)			\
		  && !(ZB_BSD)			\
		  && !(ZB_LINUX))
# define ZB_NIX ((ZB_BSD)			\
		 || (ZB_LINUX)			\
		 || (ZB_UNIX))
/** $$ Macros $$ **/

#endif /* ZB_COMPAT_H_GUARD */
