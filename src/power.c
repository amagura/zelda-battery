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
#include <string.h>
#include <math.h>
#include "main.h"
#include "power.h"
#include "compat.h"

#if ZB_LINUX
# include "acpi.h"
#elif ZB_BSD
// XXX this fixes a compiler error on FreeBSD 10.1-RELEASE-p6
typedef unsigned int u_int;
# include <sys/sysctl.h>
#endif

// fprintf(stderr, "%s: %s\n", ZB_PROGNAME, "virtual or nonstandard machine: no power supply or batteries");

void getpwr(struct power *pwr)
{
     if (pwr->charge.nof < 0)
	  pwr->charge.nof = 1;

     memset(&pwr->err.vec, PWR_OK, sizeof(int)*PWR_ELIMIT);
     pwr->err.vp = pwr->err.vec;
     pwr->err.last = pwr->err.vec;

     ZB_DBG("err: %d\n", *pwr->err.vp);

#if ZB_LINUX
     struct pwr_sup info;
     info.cap = -1;

     /* check if there was an error */
     pwr_info(&info, &(pwr->err), pwr->charge.nof);
     ZB_DBG("pwr->err.vec[pwr->err.last]: %d\n", *pwr->err.last);
     ZB_DBG("pwr->err.vec[pwr->err.pos]: %d\n", *pwr->err.vp);
     if (*pwr->err.last != PWR_OK) {
	  ZB_DBG("err: %d\n", *pwr->err.last);
	  ZB_DBG("strerr: %s\n", strerror(*pwr->err.last));
	  info.cap = *pwr->err.last;
	  /*

	  pwr->err.vec[pwr->err.last--] = PWR_OK;
	  for (; pwr->err.pos > 0; --pwr->err.pos) {
	       if (pwr->err.vec[pwr->err.pos] != PWR_OK)
		    break;
	  }
	  pwr->err.last = pwr->err.pos == 0 ? 0 : pwr->err.pos - 1;
	  */
     }
     ZB_DBG("info.cap: %d\n", info.cap);
     pwr->charge.raw = info.cap;
     pwr->charge.tr = (int)pwr->charge.raw / (pwr->charge.divsr);
     pwr->charge.rnd = nearbyint((double)pwr->charge.raw / (pwr->charge.divsr));
     pwr->acline = info.acline;
#elif ZB_BSD
     size_t size;
     int ac_line;
     int limit = pwr->charge.nof;

     size = sizeof(int);
     /* determine if we're running on battery or ac power */
     ZB_DBG("%s\n", "getting hw.acpi.acline");

     if (sysctlbyname("hw.acpi.acline", &ac_line, &size, NULL, false) == -1) {
	  ZB_ONDBG(perror(ZB_PROGNAME));
     } else {
	  pwr->acline = (bool)ac_line;
     }

     /* determine how much battery power is left */
     ZB_DBG("%s\n", "getting hw.acpi.battery.life");
     if (sysctlbyname("hw.acpi.battery.life", &ac_line, &size, NULL, false) == -1) {
	  ZB_ONDBG(perror(ZB_PROGNAME));
     } else {
     }
     pwr->charge.raw = (limit != 0)
	  ? ac_line
	  : PWR_ENOWANT;
     pwr->charge.tr = (limit != 0)
	  ? (int)pwr->charge.raw / (pwr->charge.divsr)
	  : PWR_ENOWANT;
     pwr->charge.rnd = (limit != 0)
	  ? nearbyint((double)(pwr->charge.raw / (pwr->charge.divsr)))
	  : PWR_ENOWANT;
#endif
# if 0
     for (int jdx = 0; jdx < pwr->err.last; ++jdx) {
	  pwr->err.sum += pwr->err.vec[jdx];
     }
#endif
}

struct py_power py_getpwr()
{
     struct power pwr;
     struct py_power pyp;
     memset(&pwr.charge, 0, sizeof(pwr.charge));
     pwr.charge.nof = -1;
     pwr.charge.divsr = 5;
     getpwr(&pwr);
     ZB_DBG("lulz, I haven't segfault'd yet, derp.");
     pyp.acline = pwr.acline;
     ZB_DBG("*pwr.err.vp: %d\n", *pwr.err.vp);
     ZB_DBG("pwr.charge.raw: %d\n", pwr.charge.raw);
     zb_ping;
     ZB_DBG("*pwr.err.last: %d\n", *pwr.err.last);
     pyp.err = *pwr.err.last;
     pyp.tr = pwr.charge.rnd;
     pyp.raw = pwr.charge.raw;
     zb_pong;
     return pyp;
}
