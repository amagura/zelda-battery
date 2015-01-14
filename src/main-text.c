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
#include "main.h"
#include "power.h"

#if ZB_BSD
#include <getopt.h>
#elif ZB_LINUX
#include <unistd.h>
#endif

struct txt_disp_options {
  char *full_heart;
  char *empty_heart;
  bool remaining;
  bool expended;
};

static inline void
disp_pwr_info(struct txt_disp_options opts, struct power power)
{
  if (opts.remaining || !opts.expended) {
    for (int idx = 10; idx <= power.charge.raw; idx += 10)
      printf("%s", opts.full_heart);
  }
  if (opts.expended || !opts.remaining) {
    for (; power.charge.raw < 100; power.charge.raw += 10)
      printf("%s", opts.empty_heart);
  }
}

static inline struct txt_disp_options
opt_parse(int argc, char **argv)
{
  int c = 0; // ccter storage for getopt
  struct txt_disp_options txt_opts;
  txt_opts.remaining = false;
  txt_opts.expended = false;
  const char *shopts = "hvf:e:pm";

#if ZB_BSD
  txt_opts.full_heart = "+";
  txt_opts.empty_heart = "-";
#else
  txt_opts.full_heart = "\u2665";
  txt_opts.empty_heart = "\u2661";
#endif

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
        ZB_ARGMSG("-h\t\tprint this message and exit");
        ZB_ARGMSG("-v\t\tprint program version and exit");
        ZB_ARGMSG("-f <arg> \tthe string representing remaining battery power");
        ZB_ARGMSG("-e <arg> \tthe string representing expended battery power");
        ZB_ARGMSG("-p\t\tonly print remaining power");
        ZB_ARGMSG("-m\t\tonly print expended power");
        exit(EXIT_FAILURE);
      case 'e':
        txt_opts.empty_heart = optarg;
        break;
      case 'f':
        txt_opts.full_heart = optarg;
        break;
      case 'p':
        txt_opts.remaining = true;
        txt_opts.expended = false;
        break;
      case 'm':
        txt_opts.expended = true;
        txt_opts.remaining = false;
        break;
      case 'v':
        printf("%s\n", PACKAGE_VERSION);
        exit(EXIT_SUCCESS);
        break;
    }
  }
  return txt_opts;
}

int
main(int argc, char **argv)
{
  struct txt_disp_options txt_opts = opt_parse(argc, argv);
  struct power power = init();
  disp_pwr_info(txt_opts, power);
  return EXIT_SUCCESS;
}
