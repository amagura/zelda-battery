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

#if ZB_BSD
#include <getopt.h>
#elif ZB_LINUX
#include <unistd.h>
#endif

#define ZB_COLOR_BLINK "\033[5m"

struct color_disp_options {
  bool blink;
  bool acblink;
  int blink_threshold;
  char *color;
};

static inline void
disp_pwr_info(struct color_disp_options opts, struct power power)
{
  printf("\033[%sm", opts.color);
  if (opts.blink) {
    if (power.charge.raw <= opts.blink_threshold) {
      if (power.source.ac)
        printf("%s", opts.acblink ? ZB_COLOR_BLINK : "");
      else
        printf("%s", ZB_COLOR_BLINK);
    }
  }
}

static inline struct color_disp_options
opt_parse(int argc, char **argv)
{
  int c = 0;

  struct color_disp_options color_opts;
  color_opts.acblink = false;
  color_opts.blink = true;
  color_opts.blink_threshold = 3;
  color_opts.color = "31";
  const char *shopts = "hvHanb:c:";

#if ZB_BSD
  int *opt_counter = 0;

  struct option lopts[] = {
    { 0, 0, 0, 0 }
  };

  while ((c = getopt_long(argc, argv, shopts, lopts, opt_counter)) != EOF) {
#else
#if !ZB_LINUX
#include <unistd.h>
#endif
  while ((c = getopt(argc, argv, shopts)) != EOF) {
#endif
    ZB_DBG("optopt %d:", optopt);
    if (optopt != 0)
      exit(EXIT_FAILURE);

    switch(c) {
      case 'h':
        ZB_MSG("Usage: %s [OPTION]...\n", ZB_PROGNAME);
        ZB_ARGMSG("-h\tprint this message and exit");
        ZB_ARGMSG("-v\tprint program version and exit");
        ZB_ARGMSG("-a\tenable blinking even while on A/C power (overrides previous `-n')");
        ZB_ARGMSG("-n\tdisable blinking altogether (overrides prevous `-a')");
        ZB_ARGMSG("-b\tset the power-level at which blinking ensues (defaults to `30')");
        ZB_ARGMSG("-c <arg> \tansi color code to use (defaults to `31')");
        exit(EXIT_FAILURE);
      case 'b':
        ZB_STRTONUM(color_opts.blink_threshold, (const char *)optarg);
        if (color_opts.blink_threshold > 99)
          color_opts.blink_threshold = 100;
        break;
      case 'a':
        color_opts.acblink = true;
        break;
      case 'n':
        color_opts.blink = false;
        break;
      case 'c':
        color_opts.color = optarg;
        break;
      case 'v':
        printf("%s\n", PACKAGE_VERSION);
        break;
    }
  }
  return color_opts;
}

int
main(int argc, char **argv)
{
  struct color_disp_options color_opts = opt_parse(argc, argv);
  struct power power = init();
  disp_pwr_info(color_opts, power);
  return EXIT_SUCCESS;
}

