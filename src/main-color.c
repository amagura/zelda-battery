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
     bool mc; /* master control */
     bool batt; /* battery control */
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
     // when not on A/C, no blinking if blinking on A/C is enabled
     if ((!pwr.acline && pp.blnk.ctl.ac
	  && !pp.blnk.ctl.batt)
	 && !pp.blnk.ctl.mc)
	  return;
     // W
     if (pwr.acline && !pp.blnk.ctl.ac)
	  return;
     printf("\033[%sm", pp.blnk.ccode);
}

int main(int argc, char **argv)
{
     int c = 0;
     int *optc = 0;

     struct power pwr;
     pwr.charge.nof = -1;
     struct pp_disp_opts pp;
     pp.blnk.ctl.ac = false;
     pp.blnk.ctl.batt = true;
     pp.blnk.ctl.mc = true;
     pp.blnk.ctl.thold = 3; // 30%
     pp.norm.ccode = "31";

     char *sopts =	\
	  "hv"		\
	  "t:a"		\
	  "nk:"		\
	  "K:b:"	\
	  "B:";

     struct option lopts[] = {
	  {"help", no_argument, 0, 'h'},
	  {"version", no_argument, 0, 'v'},
	  {"blink-threshold", required_argument, 0, 't'},
	  {"ac-blink", optional_argument, 0, 'a'},
	  {"no-blink", no_argument, 0, 'n'},
	  {"blink-color", required_argument, 0, 'k'},
	  {"normal-color", required_argument, 0, 'K'},
	  {"battery", required_argument, 0, 'c'},
	  {"radix", required_argument, 0, 'B'},
	  { 0, 0, 0, 0 }
     };

     while ((c = getopt_long(argc, argv, sopts, lopts, optc)) != EOF) {
	  ZB_DBG("optopt %d:", optopt);
	  if (optopt != 0)
	       goto fail;

	  switch(c) {
	  case 'h':
	       zb_help("Usage: %s [OPTION]...\n", "\t\t\t");
	       zb_arg("-t, --blink-threshold=LVL",
		      "set the power-level at which"
			   zb_arg_eol_tabs
			   "  blinking ensues (defaults to 30)",
		      "\t");
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
	       zb_arg("-B, --radix=BASE",
		      "base to use when calculating"
			   zb_arg_eol_tabs
			   "  remaining/expended power (defaults"
			   zb_arg_eol_tabs
			   "   to base 10)",
		      "\t\t");
	       zb_arg("-k, --battery=OFFSET",
		      "offset of desired battery"
			   zb_arg_eol_tabs
			   "  (e.g."
			   zb_arg_eol_tabs
			   "    0 -> no battery,"
			   zb_arg_eol_tabs
			   "    1 -> first battery)",
		      "\t");

	       zb_arg("-c, --fg-color=CCODE",
		      "ansi color code to use as "
			   zb_arg_eol_tabs
			   "  foreground color (defaults to 31)",
			   "\t");
zb_arg("-C, --bg-color=CCODE");
"ansi color code to use as \n\t\t\t\t foreground color (defaults to 31)",

	       zb_arg("-C, --bg-blink-color=CCODE",
		      "ansi color code to use as \n\t\t\t\t  background color on blink (defaults to 31)",
		      "\t");
	       zb_arg("-b, --fg-blink-color=CCODE",
		      "ansi color code to use for \n\t\t\t\t  face-color on blink (defaults to \n\t\t\t\t   (5 + base) or 5;31)",
		      "\t");
	       goto win;
	  case 'b':
	       ZB_STRTONUM(pp.blink.ctl.thold, (const char *)optarg);
	       if (pp.blink.ctl.thold > 99)
		    pp.blink.ctl.thold = 100;
	       break;
	  case 'a':
	       pp.blink.ctl.ac = true;
	       break;
	  case 'n':
	       pp.blink.ctl.mc = false;
	       break;
	  case 'N':
	       ZB_STRTONUM(pwr.charge.nof, (const char *)optarg);
	       if (pwr.charge.nof < 0)
		    pwr.charge.nof *= -1;
	       break;
	  case 'c':
	       pp.color = optarg;
	       break;
	  case 'B':
	       /* FIXME, add support for setting the calculation radix. */
	       ZB_ERROR("%s\n", "FIXME: add support for setting the calculation radix.");
	       goto fail;
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
