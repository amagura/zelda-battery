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

#include <errno.h>

/** Macros - BEGIN **/
#if ZB_MAKING_ZB_COLOR
#define ZB_PROGNAME "zbatc"
#else
#define ZB_PROGNAME "zbatt"
#endif

#if 0
#define ZB_DEBUG(format, ...) \
  do { \
    fprintf(stderr, "%s:dbg: ", ZB_PROGNAME); \
    fprintf(stderr, "%s", "`"); \
    fprintf(stderr, (format), (__VA_ARGS__)); \
    fprintf(stderr, "`   %s:%d:\n", __FILE__, __LINE__); \
  } while(0)
#else
#define ZB_DEBUG(format, ...)
#endif

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION ""
#endif

#define ZB_NULL ""

#define ZB_ERROR(format, ...) \
  do { \
    fprintf(stderr, "%s:err: ", ZB_PROGNAME); \
    fprintf(stderr, (format), __VA_ARGS__); \
    fprintf(stderr, "in %s:{%d}:%s()\n", __FILE__, __LINE__, __FUNCTION__); \
  } while(0)

#define ZB_MSG(format, ...) printf((format), (__VA_ARGS__));
#define ZB_ARGMSG(...) printf("  %s\n", (__VA_ARGS__));

#if HAVE_LIBBSD
#include <limits.h>
#include <bsd/stdlib.h>
#define ZB_STRTONUM(dst_num, const_string) \
  do { \
    errno = 0; \
    ((dst_num) = strtonum((const_string), INT_MIN, INT_MAX, NULL)); \
    if (errno != 0) { \
      perror(NULL); \
      exit(EXIT_FAILURE); \
    } \
  } while(0)
#else
#define ZB_STRTONUM(dst_num, const_string) \
  do { \
    errno = 0; \
    ((dst_num) = strtol((const_string), NULL, 10)); \
    if (errno != 0) { \
      perror(ZB_PROGNAME); \
      exit(EXIT_FAILURE); \
    } \
  } while(0)
#endif
/** Macros - END **/

#endif /* ZB_MAIN_H_GUARD */
