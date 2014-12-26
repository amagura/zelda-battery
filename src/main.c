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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "header/main.h"
#if _ZB_MAKING_COLOR
#include "header/color.h"
#else
#include "header/text.h"
#endif

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

struct pwr_src_t {
  bool batt;
  bool ac;
};

struct power_t {
  int charge;
  struct pwr_src_t source;
} power;


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

  global_t *global = malloc(sizeof(global_t));
  battery_t *binfo = NULL;
  adapter_t *ac = &global->adapt;

  // if true, no acpi support
  if (check_acpi_support() == -1) {
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
      read_acpi_batt(idx); // read current battery information

      if (binfo->present) {
        // XXX (int) here truncates stuff like `9.5' from `95 / 10' to `9'
        power.charge = (int)(binfo->percentage / 10);
      } else {
        continue;
      }
    } while(++idx < global->batt_count);
  }
  free(global);
}
#endif

int
main(int argc, char **argv)
{
  int chara = 0; // character storage

#if _ZB_MAKING_COLOR
  struct color_disp_options_t color_opts;
  color_opts.acblink = false;
  color_opts.blink = true;
  color_opts.blink_threshold = 3;
  const char *shopts = "hvanb:";

#else
  struct txt_disp_options_t txt_opts;
  const char *shopts = "hvf:e:";
#if _ZB_UNIX_BSD
  txt_opts.full_heart = "v";
  txt_opts.empty_heart = "<";
#else
  txt_opts.full_heart = "\u2665";
  txt_opts.empty_heart = "\u2661";
#endif
#endif

#ifndef PACKAGE_VERSION // should be defined by autotools
#define PACKAGE_VERSION "-1"
#endif

  while ((chara = getopt(argc, argv, shopts)) != EOF) {
    if (optopt != 0)
      return EXIT_FAILURE;

    switch(chara) {
      case 'h':
        _ZB_MSG("Usage: %s [OPTION]...\n", _ZB_PROGNAME);
        _ZB_ARGMSG("-h\t\tprint this message and exit");
        _ZB_ARGMSG("-v\t\tprint program version and exit");
        _ZB_DISP_HELP();
        return EXIT_FAILURE;
      case 'v':
        _ZB_MSG("%s", PACKAGE_VERSION);
        return EXIT_FAILURE;
#if _ZB_MAKING_COLOR
      case 'b':
        _ZB_STRTONUM(color_opts.blink_threshold, (const char *)optarg);
        break;
      case 'a':
        color_opts.acblink = true;
        break;
      case 'n':
        color_opts.blink = false;
        break;
#else
      case 'e':
        txt_opts.empty_heart = optarg;
        break;
      case 'f':
        txt_opts.full_heart = optarg;
        break;
#endif
    }
  }

  init();

#if _ZB_MAKING_COLOR
  _ZB_DISP_PWR_INFO(color_opts);
#else
  _ZB_DISP_PWR_INFO(txt_opts);
#endif
  return EXIT_SUCCESS;
}
