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

/* clang defines __GNUC__ */
# if !(__GNUC__ || __clang__)
#  ifndef __attribute__
#   define __attribute__(x)
#  endif
# endif

# define inline

# if __GNUC__ && !__clang__
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

// Check if we are on a BSD virtual machine
# if !(defined(ZB_VM_BSD) && defined(ZB_BSD_VM))
#  define ZB_VM__BSD 0
# else
#  define ZB_VM__BSD 1
# endif

// Check if we are on a Linux virtual machine
# if !(defined(ZB_VM_LINUX) && defined(ZB_LINUX_VM))
#  define ZB_VM__LINUX 0
# else
#  define ZB_VM__LINUX 1
# endif

// Check if we are on a UNIX virtual machine
# if !(defined(ZB_VM_UNIX) && defined(ZB_UNIX_VM))
#  define ZB_VM__UNIX 0
# else
#  define ZB_VM__UNIX 1
# endif

// Check if we are on a UNIX-like virtual machine
# define ZB_VM__NIX ((ZB_VM__BSD)		\
		     || (ZB_VM__LINUX)		\
		     || (ZB_VM__UNIX))

// Check if we're on a BSD machine
# if defined(__FreeBSD__)			\
     || defined(__NetBSD__)			\
     || defined(__OpenBSD__)			\
     || defined(__DragonFly__)			\
     || defined(__bsdi__)			\
     || (ZB_VM__BSD)
#  define ZB_BSD 1
# else
#  define ZB_BSD 0
# endif

// Check if we're on a Linux machine
# if defined(__linux__)				\
     || defined(__gnu_linux__)			\
     || (ZB_VM__LINUX)
#  define ZB_LINUX 1
# else
#  define ZB_LINUX 0
# endif

// Check if we're on an Android
# if defined(__ANDROID__)
#  define ZB_DROID 1
# else
#  define ZB_DROID 0
# endif

// Check if we're on a DOS
# if defined(__DOS__)				\
     || defined(__MSDOS__)			\
     || defined(_MSDOS)				\
     || defined(MSDOS)
#  define ZB_DOS 1
# else
#  define ZB_DOS 0
# endif

// Check if we're on an Apple
# if defined(macintosh)				\
     || defined(Macintosh)			\
     || (defined(__APPLE__)			\
	 && defined(__MACH__))
#  define ZB_MAC 1
# else
#  define ZB_MAC 0
# endif

// Check if we're on a Windows machine
# if defined(__WINDOWS__)			\
     || defined(__WIN32__)			\
     || defined(__TOS_WIN__)			\
     || defined(_WIN64)				\
     || defined(_WIN32)				\
     || defined(_WIN16)
#  define ZB_WIN 1
# else
#  define ZB_WIN 0
# endif

// Check if we're on a UNIX machine
# if defined(__unix__)				\
     || defined(__unix)				\
     || defined(unix)				\
     || (ZB_VM__UNIX)
#  define ZB_UNIX 1
# else
#  define ZB_UNIX 0
# endif

// Check if we're on a UNIX-like machine
# define ZB_NIX ((ZB_BSD)			\
		 || (ZB_LINUX)			\
		 || (ZB_UNIX))
/** $$ Macros $$ **/

#endif /* ZB_COMPAT_H_GUARD */
