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
#include <errno.h>
#include "main.h"
#include "power.h"

#if ZB_BSD
#include <getopt.h>
#elif ZB_LINUX
#include <unistd.h>
#endif

struct pp_disp_opts {
     bool blink;
     bool acblink;
     int blink_thold;
     char *color;
};

enum colors {
     BLNK = '5'
};

inline void disp_pwr_info(struct pp_disp_opts opts, struct power pwr)
{
     printf("\033[%sm", opts.color);
     if (!opts.blink || (pwr.charge.raw) > opts.blink_thold)
	  return;
     if (pwr.acline)
	  printf("%s", opts.acblink ? (char *)BLNK : "");
     else
	  printf("%s", (char *)BLNK);
}

int main(int argc, char **argv)
{
     int c = 0;
     struct power pwr;
     pwr.charge.nof = -1;
     struct pp_disp_opts pp;
     pp.acblink = false;
     pp.blink = true;
     pp.blink_thold = 3; // 30%
     pp.color = "31";
     const char *sopts = "hvHanb:c:N:";
#if ZB_BSD
     int *optc = 0;

     struct option lopts[] = {
	  { 0, 0, 0, 0 }
     };

# define ZB_LOOP_ARGS getopt_long(argc, argv, sopts, lopts, optc)
#else
# if !ZB_LINUX
#  include <unistd.h>
# endif
# define ZB_LOOP_ARGS getopt(argc, argv, sopts)
#endif
     while ((c = ZB_LOOP_ARGS) != EOF) {
	  ZB_DBG("optopt %d:", optopt);
	  if (optopt != 0)
	       exit(EXIT_FAILURE);

	  switch(c) {
	  case 'h':
	       ZB_MSG("Usage: %s [OPTION]...\n", ZB_PROGNAME);
	       ZB_ARGMSG("-h\t\tprint this message and exit");
	       ZB_ARGMSG("-v\t\tprint program version and exit");
	       ZB_ARGMSG("-a\t\tenable blinking even while on A/C power (overrides previous `-n')");
	       ZB_ARGMSG("-n\t\tdisable blinking altogether (overrides prevous `-a')");
	       ZB_ARGMSG("-N\t\toffset of desired battery (e.g. `0' -> no battery, `1' -> first battery)");
	       ZB_ARGMSG("-b\t\tset the power-level at which blinking ensues (defaults to `30')");
	       ZB_ARGMSG("-c <arg>\tansi color code to use (defaults to `31')");
	       exit(EXIT_FAILURE);
	  case 'b':
	       ZB_STRTONUM(pp.blink_thold, (const char *)optarg);
	       if (pp.blink_thold > 99)
		    pp.blink_thold = 100;
	       break;
	  case 'a':
	       pp.acblink = true;
	       break;
	  case 'n':
	       pp.blink = false;
	       break;
	  case 'N':
	       ZB_STRTONUM(pwr.charge.nof, (const char *)optarg);
	       if (pwr.charge.nof < 0)
		    pwr.charge.nof *= -1;
	       break;
	  case 'c':
	       pp.color = optarg;
	       break;
	  case 'v':
	       printf("%s\n", PACKAGE_VERSION);
	       break;
	  }
     }

     int err;
     err = init(&pwr);
     ZB_XONDBG(perror(ZB_PROGNAME));
     disp_pwr_info(pp, pwr);
     return EXIT_SUCCESS;
}
