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
#ifndef ZB_POWER_H_GUARD
#define ZB_POWER_H_GUARD 1
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

struct pwr_src_t {
  bool batt;
  bool ac;
};

struct power_t {
  int charge;
  struct pwr_src_t source;
};

#if _ZB_UNIX_BSD
#define _ZB_INIT() \
  do { \
    size_t size; \
    int ac_line; \
    size = sizeof(int); \
    _ZB_DEBUG("%s\n", "getting hw.acpi.acline"); \
    sysctlbyname("hw.acpi.acline", &ac_line, &size, NULL, false); \
    power.source.ac = (bool)ac_line; \
    power.source.batt = !power.source.ac; \
    _ZB_DEBUG("%s\n", "getting hw.acpi.battery.life"); \
    sysctlbyname("hw.acpi.battery.life", &ac_line, &size, NULL, false); \
    power.charge = (int)(ac_line / 10); \
  } while(0)
#elif _ZB_UNIX_LINUX
#define _ZB_INIT() \
  do { \
    power.charge = 10; \
    bool ac_support = false; \
    bool batt_support = false; \
    \
    global_t *global = malloc(sizeof(global_t)); \
    battery_t *binfo = NULL; \
    adapter_t *ac = &global->adapt; \
    \
    if (check_acpi_support() == -1) { /* if no acpi support */ \
      _ZB_ERROR("%s\n", "no libacpi: acpi support required"); \
      exit(EXIT_FAILURE); \
    } \
    ac_support = init_acpi_acadapt(global); \
    batt_support = init_acpi_batt(global); \
    \
    if (ac_support == SUCCESS) { \
      if (ac->ac_state == P_AC) { \
        power.source.ac = true; \
        power.source.batt = !power.source.ac; \
      } else if (ac->ac_state == P_BATT) { \
        power.source.batt = true; \
        power.source.ac = !power.source.batt; \
      } \
    } \
    \
    if (batt_support == SUCCESS) { \
      int idx = 0; \
      do { \
        binfo = &batteries[idx]; \
        read_acpi_batt(idx); /* read current battery information */ \
        \
        if (binfo->present) { \
          /* XXX (int) here truncates stuff like `9.5' from `95 / 10' to `9' */ \
          power.charge = (int)(binfo->percentage / 10); \
        } else { \
          continue; \
        } \
      } while(++idx < global->batt_count); \
    } \
    free(global); \
  } while(0)
#endif

END_C_DECLS

#endif /* ZB_POWER_H_GUARD */
