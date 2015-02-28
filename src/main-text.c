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
#include <getopt.h>
#include <string.h>
#include "main.h"
#include "power.h"
#include "compat.h"

struct txt_disp_opts {
  char *full_heart;
  char *empty_heart;
  bool remaining;
  bool expended;
};

void disp(struct txt_disp_opts opts, struct power pwr)
{
     ZB_DBG("pwr.charge.raw: %d\n", pwr.charge.raw);
     if (pwr.charge.raw == PWR_ENOBAT) {
	  for (int idx = 0; idx < 10; ++idx) {
	       if (idx % 2 == 0) {
		    printf("%s", opts.empty_heart);
	       } else {
		    printf("%s", opts.full_heart);
	       }
	  }
	  return;
     }

     if (pwr.charge.raw == 0) {
	  pwr.charge.raw = pwr.acline * 100;
     }
     if (opts.remaining || !opts.expended) {
	  for (int idx = 10; idx <= pwr.charge.raw; idx += 10)
	       printf("%s", opts.full_heart);
     }
     if (opts.expended || !opts.remaining) {
	  for (; pwr.charge.raw < 100; pwr.charge.raw += 10)
	       printf("%s", opts.empty_heart);
     }
}

int main(int argc, char **argv)
{
     int c = 0;
     int *optc = 0;

     struct power pwr;
     memset(&pwr.charge, 0, sizeof(pwr.charge));
     pwr.charge.nof = -1;
     pwr.charge.divsr = 10;

     struct txt_disp_opts txt;
     txt.remaining = false;
     txt.expended = false;
     txt.full_heart = "\u2665";
     txt.empty_heart = "\u2661";

     char *sopts =	 \
	  "hv"		 \
	  "f:e:"	 \
	  "rx"		 \
	  "b:d:";

     struct option lopts[] = {
	  {"help", no_argument, 0, 'h'},
	  {"version", no_argument, 0, 'v'},
	  {"full", required_argument, 0, 'f'},
	  {"empty", required_argument, 0, 'e'},
	  {"remaining", no_argument, 0, 'r'},
	  {"expended", no_argument, 0, 'x'},
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
	       zb_arg("-f, --full=STRING",
		      "representation of remaining battery"
		      zb_arg_eol_tabs
		      "  power",
		      "\t\t");
	       zb_arg("-e, --empty=STRING",
		      "representation of expended battery"
		      zb_arg_eol_tabs
		      "  power",
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
	       zb_arg("-r, --remaining",
		      "limit output to remaining power only"
		      zb_arg_eol_tabs
		      "  (overrides a previous -x option)",
		      "\t\t");
	       zb_arg("-x, --expended",
		      "display expended power only"
		      zb_arg_eol_tabs
		      "  (overrides a previous -r option)",
		      "\t\t");
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
	  case 'e':
	       txt.empty_heart = optarg;
	       break;
	  case 'f':
	       txt.full_heart = optarg;
	       break;
	  case 'r':
	       txt.remaining = true;
	       txt.expended = false;
	       break;
	  case 'x':
	       txt.remaining = false;
	       txt.expended = true;
	       break;
	  case 'b':
	       ZB_STRTONUM(pwr.charge.nof, (const char *)optarg);
	       if (pwr.charge.nof < 0)
		    pwr.charge.nof *= -1;
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
     disp(txt, pwr);
win:
     return EXIT_SUCCESS;
fail:
     return EXIT_FAILURE;
}
