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
#ifndef ZB_COLOR_H_GUARD
#define ZB_COLOR_H_GUARD 1
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

#include <stdbool.h>
#include "header/main.h"

struct color_disp_options_t {
  bool blink;
  bool acblink;
  long long blink_threshold;
  /* yet to implement */
  //char *full_color;
  //char *empty_color;
};

#define _ZB_COLOR_RED "\033[0;31m"
#define _ZB_COLOR_RED_BLINK "\033[5;31m"

// will eventually rewrite as an inline function
#define _ZB_DISP_PWR_INFO(opts) \
  do { \
    if (!(opts).blink) { \
      printf("%s", _ZB_COLOR_RED); \
    } else { \
      if (power.charge <= (opts).blink_threshold) { \
        if (power.source.ac) { \
          printf("%s", (opts).acblink ? _ZB_COLOR_RED_BLINK : _ZB_COLOR_RED); \
        } else { \
          printf("%s", _ZB_COLOR_RED_BLINK); \
        } \
      } else { \
        printf("%s", _ZB_COLOR_RED); \
      } \
    } \
  } while(0)

#define _ZB_DISP_HELP() \
  do { \
    _ZB_ARGMSG("-a\tenable blinking even while on A/C power"); \
    _ZB_ARGMSG("-n\tdisable blinking altogether (overrides previous `-a')"); \
    _ZB_ARGMSG("-b\tset the power-level at which blinking should begin (defaults to `3' as in `30')"); \
  } while(0)

END_C_DECLS

#endif /* ZB_COLOR_H_GUARD */
