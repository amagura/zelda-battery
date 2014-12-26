/****
Copyright 2014 Alexej Magura

This file is a part of Zelda Battery

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
#define ZB_MAIN_H_GUARD 1
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
#undef PARAMS
#if defined (__STDC__) || defined (_AIX) \
       || (defined (__mips) && defined (_SYSTYPE_SVR4)) \
       || defined(WIN32) || defined(__cplusplus)
# define PARAMS(protos) protos
#else
# define PARAMS(protos) ()
#endif

BEGIN_C_DECLS

#if _ZB_MAKING_ZB_COLOR
#define _ZB_PROGNAME "zbatc"
#else
#define _ZB_PROGNAME "zbatt"
#endif

#define _ZB_ERRMSG(format, ...) fprintf(stderr, (format), (__VA_ARGS__))

#define _ZB_DEBUG(format, ...) \
  do { \
    _ZB_ERRMSG("%s:dbg: ", _ZB_PROGNAME); \
    _ZB_ERRMSG("%s", "`"); \
    _ZB_ERRMSG((format), (__VA_ARGS__)); \
    _ZB_ERRMSG("`   %s:%d:\n", __FILE__, __LINE__); \
  } while(0)

#define _ZB_ERROR(format, ...) \
  do { \
    _ZB_ERRMSG("%s:err: ", _ZB_PROGNAME); \
    _ZB_ERRMSG("%s", "`"); \
    _ZB_ERRMSG((format), (__VA_ARGS__)); \
    _ZB_ERRMSG("`   %s:%d:\n", __FILE__, __LINE__); \
  } while(0)

#define _ZB_MSG(format, ...) printf((format), (__VA_ARGS__));

#if HAVE_LIBBSD
#include <limits.h>
#include <bsd/stdlib.h>
#define _ZB_STRTONUM(dst_num, const_string) \
  do { \
    if (((dst_num) = strtonum((const_string), INT_MAX, INT_MIN, NULL)) == 0) { \
      perror(NULL); \
      exit(EXIT_FAILURE); \
    } \
  } while(0)
#else
#define _ZB_STRTONUM(dst_num, const_string) \
  do { \
    if (((dst_num) = strtoll((const_string), NULL, 10)) == 0) { \
      perror(NULL); \
      exit(EXIT_FAILURE); \
    } \
  } while(0)
#endif

END_C_DECLS

#endif /* ZB_MAIN_H_GUARD */
