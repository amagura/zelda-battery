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
#include <getopt.h>
#elif _ZB_UNIX_LINUX
#include <unistd.h>
#endif

#define _ZB_COLOR_RED "\033[31m"
#define _ZB_COLOR_BLINK "\033[5m"
#define _ZB_COLOR_BOLD "\033[1m"

struct color_disp_options_t {
  bool blink;
  bool acblink;
  long long blink_threshold;
  //bool beat;
  //char *full_color;
  //char *empty_color;
};

static inline void
disp_pwr_info(struct color_disp_options_t opts, struct power_t power)
{
  printf("%s", _ZB_COLOR_RED);
  if (opts.blink) {
    if (power.charge.raw <= opts.blink_threshold) {
      if (power.source.ac)
        printf("%s", opts.acblink ? _ZB_COLOR_BLINK : "");
      else
        printf("%s", _ZB_COLOR_BLINK);
    }
  }
}

static inline struct color_disp_options_t
opt_parse(int argc, char **argv)
{
  int chara = 0;

  struct color_disp_options_t color_opts;
  color_opts.acblink = false;
  color_opts.blink = true;
  color_opts.blink_threshold = 3;
  const char *shopts = "hvHanb:";

#if _ZB_UNIX_BSD
  int *opt_counter = 0;

  struct option lopts[] = {
    { 0, 0, 0, 0 }
  };
  
  while ((chara = getopt_long(argc, argv, shopts, lopts, opt_counter)) != EOF) {
#else
#if !_ZB_UNIX_LINUX
#include <unistd.h>
#endif
  while ((chara = getopt(argc, argv, shopts)) != EOF) {
#endif
    _ZB_DEBUG("optopt %d:", optopt);
    if (optopt != 0)
      exit(EXIT_FAILURE);

    switch(chara) {
      case 'h':
        _ZB_MSG("Usage: %s [OPTION]...\n", _ZB_PROGNAME);
        _ZB_ARGMSG("-h\tprint this message and exit");
        _ZB_ARGMSG("-v\tprint program version and exit");
        _ZB_ARGMSG("-a\tenable blinking even while on A/C power (overrides previous `-n')");
        _ZB_ARGMSG("-n\tdisable blinking altogether (overrides prevous `-a')");
        _ZB_ARGMSG("-b\tset the power-level at which blinking ensues (defaults to `30')");
        exit(EXIT_FAILURE);
      case 'b':
        _ZB_STRTONUM(color_opts.blink_threshold, (const char *)optarg);
        if (color_opts.blink_threshold > 99)
          color_opts.blink_threshold = 100;
        break;
      case 'a':
        color_opts.acblink = true;
        break;
      case 'n':
        color_opts.blink = false;
        break;
    }
  }
  return color_opts;
}

int
main(int argc, char **argv)
{
  struct power_t power = init();
  struct color_disp_options_t color_opts = opt_parse(argc, argv);
  disp_pwr_info(color_opts, power);
  return EXIT_SUCCESS;
}

