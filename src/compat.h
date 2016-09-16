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
#ifndef ZB_COMPAT_H_GUARD
# define ZB_COMPAT_H_GUARD 1
/* BEGIN_C_DECLS should be used at the beginning of your declarations,
so that C++ compilers don't mangle their names.  Use END_C_DECLS at
the end of C declarations. */
#undef BEGIN_C_DECLS
#undef END_C_DECLS
#ifdef __cplusplus
# define BEGIN_C_DECLS extern "C" {
# define END_C_DECLS }
#else
# define BEGIN_C_DECLS /* empty */
# define END_C_DECLS /* empty */
#endif

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

BEGIN_C_DECLS

# if !defined(ZB_WINDOWS)
#  if defined(__CYGWIN__)			\
     || defined(_WIN16)				\
     || defined(_WIN32)				\
     || defined(_WIN64)				\
     || defined(__WIN32__)			\
     || defined(__TOS_WIN__)			\
     || defined(__WINDOWS__)
#   define ZB_WINDOWS 1
#  else
#   define ZB_WINDOWS 0
#  endif
# endif

# if !defined(ZB_BSD)
#  if defined(__NetBSD__)	   \
     || defined(__FreeBSD__)	   \
     || defined(__FreeBSD__kernel) \
     || defined(__DragonFly__)	   \
     || defined(__bsdi__)	   \
     || defined(__OpenBSD__)	   \
     || defined(_SYSTYPE_BSD)
#   define ZB_BSD 1
#  else
#   define ZB_BSD 0
#  endif
# endif

# if !defined(ZB_LINUX)
#  if defined(__linux__)	\
     || defined(__gnu_linux__)
#   define ZB_LINUX 1
#  else
#   define ZB_LINUX 0
#  endif
# endif

# if !defined(ZB_alpha)
#  if defined(__alpha__) \
     || defined(__alpha) \
     || defined(_M_ALPHA)
#   define ZB_alpha 1
#  else
#   define ZB_alpha 0
#  endif
# endif

# if !defined(ZB_amd64)
#  if defined(__amd64__)     \
     || defined(__amd64)     \
     || defined(__x86_64__)  \
     || defined(__x86_64)    \
     || defined(_M_X64)	     \
     || defined(_M_AMD64)
#   define ZB_amd64 1
#  else
#   define ZB_amd64 0
#  endif
# endif

# if !defined(ZB_x86)
#  if defined(i386)		\
     || defined(__i386)		\
     || defined(__i386__)	\
     || defined(__i486__)	\
     || defined(__i586__)	\
     || defined(__i686__)	\
     || defined(__IA32__)	\
     || defined(_M_I86)		\
     || defined(_M_IX86)	\
     || defined(__X86__)	\
     || defined(_X86_)		\
     || defined(__THW_INTEL__) 	\
     || defined(__I86__)	\
     || defined(__INTEL__)	\
     || defined(__386)
#   define ZB_x86 1
#  else
#   define ZB_x86 0
#  endif
# endif

END_C_DECLS

#endif /* ZB_COMPAT_H_GUARD */
