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

#include "header/main.h"
#include "header/power.h"

// I may make the linux and bsd sources (more or less) compatible
// since `sysctl' is also present in Linux
// and the `sysctl' code is smaller (code-wise, need to look at asm) than
// the raw `acpi' lib code

#if _ZB_UNIX_BSD
#include <sys/sysctl.h>
#elif _ZB_UNIX_LINUX
#include <malloc.h>
#include <libacpi.h>
#endif


#if _ZB_UNIX_BSD
inline void
init()
{
  size_t size;
  int ac_line;
  size = sizeof(int);
  _ZB_DEBUG("%s\n", "getting hw.acpi.acline");
  sysctlbyname("hw.acpi.acline", &ac_line, &size, NULL, false);
  power.source.ac = (bool)ac_line;
  power.source.batt = !power.source.ac;
  _ZB_DEBUG("%s\n", "getting hw.acpi.battery.life");
  sysctlbyname("hw.acpi.battery.life", &ac_line, &size, NULL, false);
  power.charge = (int)(ac_line / 10);
}
#elif _ZB_UNIX_LINUX
inline void
init()
{
  power.charge = 10;
  bool ac_support = false;
  bool batt_support = false;

  global_t *global_ptr = malloc(sizeof(global_t));
  battery_t *binfo_ptr = NULL;
  adapter_t *ac_ptr = &global->adapt;

  // if true, no acpi support
  if (check_acpi_support() == -1) {
    _ZB_ERROR("%s\n", "no libacpi: acpi support required");
    exit(EXIT_FAILURE);
  }
  ac_support = init_acpi_acadapt(global_ptr);
  batt_support = init_acpi_batt(global_ptr);

  if (ac_support == SUCCESS) {
    if (ac->ac_state == P_AC) {
      power.source.ac = true;
      power.source.batt = !power.source.ac;
    } else if (ac->ac_state == P_BATT) {
      power.source.batt = true;
      power.source.ac = !power.source.batt;
    }
  }

  if (batt_support == SUCCESS) {
    int idx = 0;
    do {
      binfo_ptr = &batteries[idx];
      read_acpi_batt(idx); // read current battery information

      if (binfo_ptr->present) {
        // XXX (int) here truncates stuff like `9.5' from `95 / 10' to `9'
        power.charge = (int)(binfo_ptr->percentage / 10);
      } else {
        continue;
      }
    } while(++idx < global_ptr->batt_count);
  }
  free(global_ptr);
}
#endif
