/****
Copyright 2014, 2015, 2016, 2017 Alexej Magura

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
#if !ZB_BSD
# ifndef ZB_ACPI_H_GUARD
#  define ZB_ACPI_H_GUARD 1
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

# ifndef _Bool
#  include <stdbool.h>
# endif

# ifndef PWR_ELIMIT
#  include "main.h"
# endif

#  include "power.h"

# if !defined(HAVE__SYS_CLASS_POWER_SUPPLY) && !defined(HAVE__PROC_ACPI)
#  define HAVE__SYS_CLASS_POWER_SUPPLY 1
#  define HAVE__PROC_ACPI 1
# endif

# if defined(HAVE__SYS_CLASS_POWER_SUPPLY)
#  ifndef ZB_ACPI_ROOT
#   define ZB_ACPI_ROOT "/sys/class/power_supply"
#  endif
#  define ZB_ACPI_GLOB ZB_ACPI_ROOT "/*/type"
#  define ZB_ACPI_PATH_SIZE (sizeof(ZB_ACPI_ROOT ZB_ACPI_GLOB)) /* the `""` here adds 1 to the overall length */
#  define ZB_ACPI_BATTYPE "Battery"
#  define ZB_ACPI_ACTYPE "Mains"
#  define ZB_ACPI_TYPE_SIZE (sizeof(ZB_ACPI_BATTYPE ZB_ACPI_ACTYPE ""))
#  define ZB_ACPI_BATCAP_PATH "/capacity"
#  define ZB_ACPI_ACSTAT_PATH "/online"
# endif

# if defined(HAVE__PROC_ACPI) && !defined(HAVE__SYS_CLASS_POWER_SUPPLY)
#  ifndef ZB_ACPI_ROOT
#   define ZB_ACPI_ROOT "/proc/acpi"
#  endif
#  define ZB_ACPI_GLOB ZB_ACPI_ROOT // FIXME glob not known
#  define ZB_ACPI_PATH_SIZE (sizeof(ZB_ACPI_ROOT ZB_ACPI_GLOB))
#  define ZB_ACPI_BATCAP_PATH
#  define ZB_ACPI_BATTYPE
#  define ZB_ACPI_ACTYPE
# endif

struct pwr_sup {
     bool acline;
     int cap;
};

void pwr_info PARAMS((struct pwr_sup *info, struct error *err, int btnum));

# endif /* ZBATT_ACPI_H_GUARD */
#endif
