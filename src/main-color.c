/****
Copyright 2014-2015 Alexej Magura

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
#include <getopt.h>
#include "main.h"
#include "power.h"
#include "compat.h"

struct onoff {
     bool mc;
     bool ac; /* A/C control */
     int thold; /* threshold */
};

struct colors {
     char *ccode;
     struct onoff ctl;
};

struct pp_disp_opts {
     struct colors norm;
     struct colors blnk;
};

inline void disp(struct pp_disp_opts pp, struct power pwr)
{
     printf("\033[%sm", pp.norm.ccode);
     if (!pp.blnk.ctl.mc || (pwr.charge.raw) > pp.blnk.ctl.thold)
	  return;
     if (pwr.acline && !pp.blnk.ctl.ac)
	  return;
     // When all the planets are in proper alignment...
     printf("\033[%sm", pp.blnk.ccode);
}

int main(int argc, char **argv)
{
     int c = 0;
     int *optc = 0;

     struct power pwr;
     pwr.charge.nof = -1;
     pwr.charge.divsr = 10;
     struct pp_disp_opts pp;
     pp.blnk.ctl.ac = false;
     pp.blnk.ctl.mc = true;
     pp.blnk.ctl.thold = 3; // 30%
     pp.norm.ccode = "31";

     char *sopts =	\
	  "hv"		\
	  "t:a::"	\
	  "nc:"		\
	  "C:b:"	\
	  "d:";

     struct option lopts[] = {
	  {"help", no_argument, 0, 'h'},
	  {"version", no_argument, 0, 'v'},
	  {"blink-threshold", required_argument, 0, 't'},
	  {"ac-blink", required_argument, 0, 'a'},
	  {"no-blink", no_argument, 0, 'n'},
	  {"blink-color", required_argument, 0, 'C'},
	  {"normal-color", required_argument, 0, 'c'},
	  {"battery", required_argument, 0, 'b'},
	  {"divisor", required_argument, 0, 'd'},
	  { 0, 0, 0, 0 }
     };

     while ((c = getopt_long(argc, argv, sopts, lopts, optc)) != EOF) {
	  ZB_DBG("optopt %d:", optopt);
	  if (optopt != 0)
	       goto fail;

	  switch(c) {
	  case 'h':
	       zb_help("Usage: %s [OPTION]...\n", "\t\t\t");

	       zb_arg("-a, --ac-blink",
		      "enable blinking while on A/C power"
			   zb_arg_eol_tabs
			   "  (overrides a previous -n option)",
		      "\t\t");
	       zb_arg("-n, --no-blink",
		      "disable blinking altogether"
			   zb_arg_eol_tabs
			   "  (overrides a previous -a option)",
		      "\t\t");
	       zb_arg("-d, --divisor=NUM",
		      "The value used to calculate power"
		      zb_arg_eol_tabs
		      "sample size (e.g."
		      zb_arg_eol_tabs
		      "    3 ->"
		      zb_arg_eol_tabs
		      "      current power level to the nearest 1/3,"
		      "   10 ->"
		      zb_arg_eol_tabs
		      "      current power level to the nearest 1/10)",
		      "\t\t");
	       zb_arg("-t, --blink-threshold=LVL",
		      "set the power-level at which"
		      zb_arg_eol_tabs
		      "  blinking ensues (defaults to 30)",
		      "\t");
	       zb_arg("-c, --normal-color=CCODE",
		      "ansi color code to use for "
		      zb_arg_eol_tabs
		      "  color when not blinking"
		      zb_arg_eol_tabs
		      "  (defaults to 31)",
		      "\t");
	       zb_arg("-C, --blink-color=CCODE",
		      "ansi color code to use for "
		      zb_arg_eol_tabs
		      "  color when blinking (defaults to 5;31)",
		      "\t");
	       zb_arg("-b, --battery=OFFSET",
		      "offset of desired battery"
		      zb_arg_eol_tabs
		      "  (e.g."
		      zb_arg_eol_tabs
		      "    0 -> no battery,"
		      zb_arg_eol_tabs
		      "    1 -> first battery)",
		      "\t");
	       goto win;
	  case 't':
	       ZB_STRTONUM(pp.blnk.ctl.thold, (const char *)optarg);
	       if (pp.blnk.ctl.thold > 99)
		    pp.blnk.ctl.thold = 100;
	       break;
	  case 'a':
	       pp.blnk.ctl.ac = true;
	       break;
	  case 'n':
	       pp.blnk.ctl.mc = false;
	       break;
	  case 'b':
	       ZB_STRTONUM(pwr.charge.nof, (const char *)optarg);
	       if (pwr.charge.nof < 0)
		    pwr.charge.nof *= -1;
	       break;
	  case 'c':
	       pp.norm.ccode = optarg;
	       break;
	  case 'C':
	       pp.blnk.ccode = optarg;
	       break;
	  case 'd':
	       ZB_STRTONUM(pwr.charge.divsr, (const char *)optarg);
	       break;
	  case 'v':
	       printf("%s\n", PACKAGE_VERSION);
	       goto win;
	  }
     }

     int err;
     err = getpwr(&pwr);
     ZB_DBG("err: `%d`\n", err);
     ZB_XONDBG(perror(ZB_PROGNAME));
     disp(pp, pwr);
win:
     return EXIT_SUCCESS;
fail:
     return EXIT_FAILURE;
}
