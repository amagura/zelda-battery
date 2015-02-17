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

inline void disp(struct txt_disp_opts opts, struct power pwr)
{
     ZB_DBG("pwr.charge.raw: %d\n", pwr.charge.raw);
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
     pwr.charge.radix = 10;

     struct txt_disp_opts txt;
     txt.remaining = false;
     txt.expended = false;
     txt.full_heart = "\u2665";
     txt.empty_heart = "\u2661";

     char *sopts =	 \
	  "hv"		 \
	  "f:e:"	 \
	  "rx"		 \
	  "b:B:";

     struct option lopts[] = {
	  {"help", no_argument, 0, 'h'},
	  {"version", no_argument, 0, 'v'},
	  {"full", required_argument, 0, 'f'},
	  {"empty", required_argument, 0, 'e'},
	  {"remaining", no_argument, 0, 'r'},
	  {"expended", no_argument, 0, 'x'},
	  {"battery", required_argument, 0, 'b'},
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
	       zb_arg("-f, --full=STRING",
		      "representation of remaining battery\n\t\t\t\t  power",
		      "\t\t");
	       zb_arg("-e, --empty=STRING",
		      "representation of expended battery\n\t\t\t\t  power",
		      "\t\t");
	       zb_arg("-B, --radix=BASE",
		      "base to use when calculating\n\t\t\t\t  remaining/expended power (defaults\n\t\t\t\t   to base 10)",
		      "\t\t");
	       zb_arg("-r, --remaining",
		      "limit output to remaining power only\n\t\t\t\t  (overrides a previous -x option)",
		      "\t\t");
	       zb_arg("-x, --expended",
		      "display expended power only\n\t\t\t\t  (overrides a previous -r option)",
		      "\t\t");
	       zb_arg("-b, --battery=OFFSET",
		      "offset of desired battery\n\t\t\t\t  (e.g. \n\t\t\t\t    0 -> no battery,\n\t\t\t\t    1 -> first battery)",
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
	  case 'B':
	       ZB_STRTONUM(pwr.charge.radix, (const char *)optarg);
	       break;
	  case 'v':
	       printf("%s\n", PACKAGE_VERSION);
	       goto win;
	  }
     }

     int err = 0;
     err = getpwr(&pwr);
     ZB_DBG("err: `%d`\n", err);
     ZB_XONDBG(perror(ZB_PROGNAME));
     disp(txt, pwr);
win:
     return EXIT_SUCCESS;
fail:
     return EXIT_FAILURE;
}
