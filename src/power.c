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


int getpwr(struct power *pwr)
{
     int *retval = &pwr->charge.err;
     *retval = 0;
#if ZB_LINUX
     int limit;
     struct pwr_sup info;

     if ((limit = pwr->charge.nof) < 0)
	  limit = 1;

     info.cap = malloc(sizeof(*info.cap) * limit);
     info.acline = false;

     if ((*retval = pwr_info(&info, limit)) != ZB_PWR_OK) {
	  ZB_DBG("err: %d\n", *retval);
	  ZB_ONDBG(perror(ZB_PROGNAME));
     }

     pwr->charge.raw = (limit != 0)
	  ? info.cap[limit]
	  : ZB_PWR_NWANTBAT;
     pwr->charge.tr = (limit != 0)
	  ? (int)pwr->charge.raw / (pwr->charge.divsr)
	  : ZB_PWR_NWANTBAT;

# if ZB_DEBUG
     for (int idx = 0; idx < limit; ++idx) {
	  ZB_DBG("limit: `%d`\n", limit);
	  ZB_DBG("info.cap[%d]: `%d`\n", idx, info.cap[idx]);
     }
# endif
     ZB_DBG("pwr->charge.raw: %d\n", pwr->charge.raw);
     free(info.cap);
     ZB_DBG("info.acline: %d\n", info.acline);
     pwr->acline = info.acline;
#elif ZB_BSD
     size_t size;
     int ac_line;
     size = sizeof(int);

     /* determine if we're running on battery or ac power */
     ZB_DBG("%s\n", "getting hw.acpi.acline");			\
     sysctlbyname("hw.acpi.acline", &ac_line, &size, NULL, false);
     pwr->acline = (bool)ac_line;

     /* determine how much battery power is left */
     ZB_DBG("%s\n", "getting hw.acpi.battery.life");			\
     sysctlbyname("hw.acpi.battery.life", &ac_line, &size, NULL, false);
     pwr->charge.raw = (limit != 0)
	  ? ac_line
	  : ZB_NWANTBAT;
     pwr->charge.tr = (limit != 0)
	  ? (int)pwr->charge.raw / (pwr->charge.divsr)
	  : ZB_NWANTBAT;
#endif
     return *retval;
}
