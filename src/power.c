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
#include <string.h>
#include "main.h"
#include "power.h"
#include "compat.h"

#if ZB_LINUX
# include "acpi.h"
#elif ZB_BSD
# include <sys/sysctl.h>
#endif
// fprintf(stderr, "%s: %s\n", ZB_PROGNAME, "virtual or nonstandard machine: no power supply or batteries");

void getpwr(struct power *pwr)
{
     int *btnum;
     if ((btnum = &pwr->charge.nof) < 0)
	  *btnum = 1;
     pwr->e = pwr->err;

#if ZB_LINUX
     struct pwr_sup info;
     info.e = pwr->e;
     info.cap = -1;

     /* check if there was an error */
     pwr_info(&info, *btnum);
     if (*info.e != PWR_OK) {
	  ZB_DBG("err: %d\n", *info.e);
	  ZB_ONDBG(perror(ZB_PROGNAME));
     }
#elif ZB_BSD
     size_t size;
     int ac_line;
     int limit = pwr->charge.nof;
     pwr->e = pwr->err;
     size = sizeof(int);
     /* determine if we're running on battery or ac power */
     ZB_DBG("%s\n", "getting hw.acpi.acline");			\
     if (sysctlbyname("hw.acpi.acline", &ac_line, &size, NULL, false) == -1) {
	  ZB_ONDBG(perror(ZB_PROGNAME));
     } else {
	  pwr->acline = (bool)ac_line;
     }

     /* determine how much battery power is left */
     ZB_DBG("%s\n", "getting hw.acpi.battery.life");			\
     if (sysctlbyname("hw.acpi.battery.life", &ac_line, &size, NULL, false) == -1) {
	  ZB_ONDBG(perror(ZB_PROGNAME));
     } else {
     }
     pwr->charge.raw = (limit != 0)
	  ? ac_line
	  : ZB_PWR_NWANTBAT;
     pwr->charge.tr = (limit != 0)
	  ? (int)pwr->charge.raw / (pwr->charge.divsr)
	  : ZB_PWR_NWANTBAT;
#endif
}
