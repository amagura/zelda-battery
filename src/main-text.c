/****
Copyright 2014 Alexej Magura

This file is a part of Zbat

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
#include "main.h"
#include "power.h"

#if _ZB_UNIX_BSD
#include <getopt.h>
#elif _ZB_UNIX_LINUX
#include <unistd.h>
#endif

struct txt_disp_options_t {
  char *full_heart;
  char *empty_heart;
};

inline void
disp_pwr_info(struct txt_disp_options_t opts, struct power_t power)
{
  for (int idx = 1; idx <= power.charge.truncated; ++idx) {
    printf("%s", opts.full_heart);
  }
  for (; power.charge.truncated < 10; ++power.charge.truncated) {
    printf("%s", opts.full_heart);
  }
}

inline struct txt_disp_options_t
opt_parse(int argc, char **argv)
{
  int chara = 0; // character storage for getopt
  struct txt_disp_options_t txt_opts;
  const char *shopts = "hvf:e:";

#if _ZB_UNIX_BSD
  txt_opts.full_heart = "+";
  txt_opts.empty_heart = "-";
#else
  txt_opts.full_heart = "\u2665";
  txt_opts.empty_heart = "\u2661";
#endif
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
        _ZB_ARGMSG("-h\t\tprint this message and exit");
        _ZB_ARGMSG("-v\t\tprint program version and exit");
        _ZB_ARGMSG("-f <arg> \tthe string representing remaining battery power");
        _ZB_ARGMSG("-e <arg> \tthe string representing expended battery power");
        exit(EXIT_FAILURE);
      case 'e':
        txt_opts.empty_heart = optarg;
        break;
      case 'f':
        txt_opts.full_heart = optarg;
        break;
    }
  }
}

int
main(int argc, char **argv)
{
  struct power_t power = init();
  struct txt_disp_options_t txt_opts = opt_parse(argc, argv);
  disp_pwr_info(txt_opts, power);
  return EXIT_SUCCESS;
}
