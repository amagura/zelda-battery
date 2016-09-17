/****
Copyright 2014, 2015, 2016 Alexej Magura

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

void disp(struct pp_disp_opts pp, struct power pwr)
{
     printf("\033[%sm", pp.norm.ccode);
     ZB_DBG("pwr.charge.raw: %d\n", pwr.charge.raw);
     ZB_DBG("(pwr.charge.raw) > pp.blnk.ctl.thold: %d\n", pwr.charge.raw > pp.blnk.ctl.thold);
     if (!pp.blnk.ctl.mc || (pwr.charge.raw) > pp.blnk.ctl.thold)
	  return;
     if (pwr.acline && !pp.blnk.ctl.ac) {
	  ZB_DBG("%s\n", "On A/C power: no need to blink");
	  return;
     }
     // When all the planets are in proper alignment...
     ZB_DBG("pwr.charge.tr: %d\n", pwr.charge.tr);
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
     pp.blnk.ccode = "5";

     char *sopts =	\
	  "hv"		\
	  "t:a::"	\
	  "nc:"		\
	  "C:b:"	\
	  "pd:";

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
	  {"print-info", no_argument, 0, 'p'},
	  { 0, 0, 0, 0 }
     };

     while ((c = getopt_long(argc, argv, sopts, lopts, optc)) != EOF) {
	  ZB_DBG("optopt %d:", optopt);
	  if (optopt != 0)
	       goto fail;

	  switch(c) {
	  case 'h':
	       zb_help("Usage: %s [OPTION]...\n", "\t\t");

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
		      "value used to calculate power sample"
		      zb_arg_eol_tabs
		      "size: e.g. 3 is to the"
		      zb_arg_eol_tabs
		      "nearest 1/3, and 10 is to the"
		      zb_arg_eol_tabs
		      "nearest 1/10 (default: 10)",
		      "\t\t");
	       zb_arg("-t, --blink-threshold=NUM",
		      "battery charge threshold"
		      zb_arg_eol_tabs
		      "where blinking ensues (default: 3)"
		      zb_arg_eol_tabs
		      "the blink threshold affected by"
		      zb_arg_eol_tabs
		      "the divisor, so a threshold of 3"
		      zb_arg_eol_tabs
		      "will trigger on charge levels >= 30"
		      zb_arg_eol_tabs
		      "(threshold * divisor = charge level"
		      zb_arg_eol_tabs
		      "  where blinking starts)",
		      "\t");
	       zb_arg("-c, --normal-color=CCODE",
		      "ansi color code to use when not"
		      zb_arg_eol_tabs
		      "  blinking (default: 31)",
		      "\t");
	       zb_arg("-C, --blink-color=CCODE",
		      "ansi color code to use when "
		      zb_arg_eol_tabs
		      "  blinking (default: 5;31)",
		      "\t");
	       zb_arg("-b, --battery=OFFSET",
		      "desired battery offset"
		      zb_arg_eol_tabs
		      "(e.g. 0 is no-op; 1 is the fisrt battery)",
		      "\t\t");
	       zb_arg("-p,  --print-info",
		      "print statisical info to a temporary file:"
		      zb_arg_eol_tabs
		      "the file will be dumped to stdout on exit",
		      "\t\t");
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

     getpwr(&pwr);
     ZB_XONDBG(perror(ZB_PROGNAME));
     disp(pp, pwr);
win:
     return EXIT_SUCCESS;
fail:
     return EXIT_FAILURE;
}
