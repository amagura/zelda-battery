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
#include "main.h"
#include "power.h"
#include "compat.h"

struct txt_disp_opts {
  char *full_heart;
  char *empty_heart;
  bool remaining;
  bool expended;
};

inline void disp_pwr_info(struct txt_disp_opts opts, struct power pwr)
{
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
     pwr.charge.nof = -1;

     struct txt_disp_opts txt;
     txt.remaining = false;
     txt.expended = false;
     txt.full_heart = "\u2665";
     txt.empty_heart = "\u2661";

     char *sopts =	 \
	  "hv"		 \
	  "f:e:"	 \
	  "rx"		 \
	  "N:";

     struct option lopts[] = {
	  {"help", no_argument, 0, 'h'},
	  {"version", no_argument, 0, 'v'},
	  {"full", required_argument, 0, 'f'},
	  {"empty", required_argument, 0, 'e'},
	  {"remaining", required_argument, 0, 'r'},
	  {"expended", required_argument, 0, 'x'},
	  {"nth-battery", required_argument, 0, 'N'},
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
		      "representation of remaining battery power",
		      "\t");
	       zb_arg("-e, --empty=STRING",
		      "representation of expended battery power",
		      "\t");
	       zb_arg("-r, --remaining",
		      "limit output to remaining power only",
		      "\t\t");
	       zb_arg("-x, --expended",
		      "display expended power only",
		      "\t\t");
	       zb_arg("-N, --nth-battery=OFFSET",
		      "offset of desired battery (e.g. `0' -> no battery, `1' -> first battery)",
		      "\t\t");
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
	       txt.expended = true;
	       txt.remaining = false;
	       break;
	  case 'N':
	       ZB_STRTONUM(pwr.charge.nof, (const char *)optarg);
	       if (pwr.charge.nof < 0)
		    pwr.charge.nof *= -1;
	       break;
	  case 'v':
	       printf("%s\n", PACKAGE_VERSION);
	       goto win;
	  }
     }

     int err;
     err = init(&pwr);
     ZB_DBG("err: `%d`\n", err);
     ZB_XONDBG(perror(ZB_PROGNAME));
     disp_pwr_info(txt, pwr);
win:
     return EXIT_SUCCESS;
fail:
     return EXIT_FAILURE;
}
