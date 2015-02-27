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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "power.h"

#if _ZB_UNIX_BSD
#include <sys/sysctl.h>
#elif _ZB_UNIX_LINUX
#include <malloc.h>
#include <libacpi.h>
#endif

struct power_t
init()
{
  struct power_t power;
#if _ZB_UNIX_BSD
  size_t size;
  int ac_line;
  size = sizeof(int);
  /* determine if we're running on battery or ac power */
  _ZB_DEBUG("%s\n", "getting hw.acpi.acline"); \
  sysctlbyname("hw.acpi.acline", &ac_line, &size, NULL, false);
  power.source.ac = (bool)ac_line;
  power.source.batt = !power.source.ac;
  /* determine how much battery power is left */
  _ZB_DEBUG("%s\n", "getting hw.acpi.battery.life"); \
  sysctlbyname("hw.acpi.battery.life", &ac_line, &size, NULL, false);
  power.charge.raw = ac_line;
  power.charge.truncated = (int)power.charge.raw / 10;
#elif _ZB_UNIX_LINUX
  bool ac_support = false;
  bool batt_support = false;

  global_t *global = malloc(sizeof(global_t));
  battery_t *binfo = NULL;
  adapter_t *ac = &global->adapt;

  if (check_acpi_support() == -1) { /* if no acpi support */
    _ZB_ERROR("%s\n", "no libacpi: acpi support required");
    exit(EXIT_FAILURE);
  }
  ac_support = init_acpi_acadapt(global);
  batt_support = init_acpi_batt(global);

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
      binfo = &batteries[idx];
      read_acpi_batt(idx); /* read current battery information */

      if (binfo->present) {
        power.charge.raw = binfo->percentage;
        power.charge.truncated = (int)power.charge.raw / 10;
      } else {
        continue;
      }
    } while(++idx < global->batt_count);
  }
  free(global);
#endif
  return power;
}
