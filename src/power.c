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
#include <string.h>
#include "main.h"
#include "power.h"

#if ZB_BSD
#include <sys/sysctl.h>
#endif

#if ZB_LINUX
#include "acpi.h"
#endif

struct power
init()
{
  struct power power;
#if ZB_BSD
  size_t size;
  int ac_line;
  size = sizeof(int);
  /* determine if we're running on battery or ac power */
  ZB_DBG("%s\n", "getting hw.acpi.acline"); \
  sysctlbyname("hw.acpi.acline", &ac_line, &size, NULL, false);
  power.source.ac = (bool)ac_line;
  power.source.batt = !power.source.ac;
  /* determine how much battery power is left */
  ZB_DBG("%s\n", "getting hw.acpi.battery.life"); \
  sysctlbyname("hw.acpi.battery.life", &ac_line, &size, NULL, false);
  power.charge.raw = ac_line;
  power.charge.truncated = (int)power.charge.raw / 10;
  //#elif ZB_LINUX
#else
  struct pwr_sup info;
  /* change this value if you need to read from
   * more than one battery.
   */
  int limit = 1;
  info.cap = NULL;
  info.cap = malloc(sizeof(info.cap)*limit);
  info.acline = false;
  pwr_inf(info, limit);
  /* I admit that currently, with the below code
   * and the current implementation
   * (everything outside of the `acpi.c' file, which
   * itself, does in fact support reading from
   * more than one battery), reading the current capacity
   * from more than one battery is unsupported.
   */
  power.charge.raw = info.cap[limit]; /* FIXME, I'm not future-proofed */
  free(info.cap);
  power.charge.truncated = (int)power.charge.raw / 10;
  power.source.ac = info.acline;
  power.source.batt = !power.source.ac;
#endif
  return power;
}
