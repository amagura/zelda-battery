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
#include "compat.h"
#if ZB_LINUX
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <libgen.h>
# include <glob.h>
# include <limits.h>
# include "main.h"
# include "power.h"
# include "acpi.h"

extern size_t concatl(char *dst, size_t sz, const char *s1, ...) __attribute__((sentinel));

# if defined(HAVE__SYS_CLASS_POWER_SUPPLY)
/* optionally gets information regarding the battery specified */
int get_batt_info(int *cap, char *batt, int btnum)
{
     ZB_DBG("btnum: %d\n", btnum);

     /* if no battery is desired, return `ZB_PWR_OK' */
     if (btnum == 0)
	  return PWR_ENOWANT;

     FILE *fp;

     if ((fp = fopen(batt, "r")) == NULL)
	  return PWR_ENOBAT;

     char tmp[ZB_ACPI_TYPE_SIZE];

     fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);

     /* get battery level */
     ZB_DBG("batt cap: %s\n", tmp);
     ZB_STRTONUM(*cap, tmp);
     fclose(fp);

     return PWR_OK;
}

int get_ac_info(bool *acline, char *acfile)
{
     FILE *fp;

     if ((fp = fopen(acfile, "r")) == NULL)
	  return PWR_ENOAC;

     char tmp[ZB_ACPI_TYPE_SIZE];
     fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);

     /* get A/C adapter state */
     ZB_DBG("acline: %s\n", tmp);
     int kdx;
     ZB_STRTONUM(kdx, tmp);
     *acline = (bool)kdx;
     fclose(fp);

     return PWR_OK;
}

void read_pwr_files(struct pwr_sup *info, struct error *err, char *ac, char *batt, int btnum)
{

     ZB_DBG("info->cap before: %d\n", info->cap);
     zb_ping;
     zb_eset((err), get_ac_info(&(info->acline), ac));
     zb_eset((err), get_batt_info(&(info->cap), batt, btnum));
     if (info->acline == 0 && info->cap == 0) {
	  /* power supply is most likely broken... or this is a test....  Did we pass? :P */
	  zb_eset((err), PWR_EBRK);
     }
     zb_ping;
     ZB_DBG("info->cap after: %d\n", info->cap);
     zb_eset((err), get_ac_info(&(info->acline), ac));
     zb_pong;
     ZB_DBG("*err->vp: %d\n", *err->vp);
}

# if ZB_USE_KCAT
void get_pwr_files(glob_t globuf, char *ac, char *batt, int limit)
{
     FILE *fp;
#  if ZB_DEBUG
     size_t bytes;
#  endif

     /* the files we'll be reading only consist of
      * a single line of little text,
      * often no more than a word; but
      * definitely no more than 3
      * to 5 words (i.e. a sentence). */
     char tmp[ZB_ACPI_TYPE_SIZE];

     /*   Note the use of `globuf.gl_pathc` instead of `batlim`:
      * `batlim' is only used to limit the _number of batteries_;
      * what about A/C power supplies?
      *
      *   Note the use of `(int)' in the `for' loop:
      * IIRC, int has an upper range limit somewhere
      * in the 30 thousands: I very much doubt that
      * you're running a machine that has
      * +30,000 batteries; so no need to use the same
      * type as `globuf.gl_pathc'; plus, `int', IIRC, is
      * the standard convention for _generic_ loop counter types. */

     for (int idx = 0; idx < (int)globuf.gl_pathc; ++idx) {
	  if ((fp = fopen(globuf.gl_pathv[idx], "r")) == NULL)
	       continue;

	  fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);
	  fclose(fp);

	  /* find batteries */
	  if (strncmp(tmp, ZB_ACPI_BATTYPE, 3) == 0 && limit-- > 0) {
	       ZB_DBG("globuf.gl_pathv[idx]: `%s`\n", globuf.gl_pathv[idx]);
	       if (limit != 0)
		    continue;
#  if ZB_DEBUG
	       bytes = concatl(batt, ZB_ACPI_PATH_SIZE, dirname(globuf.gl_pathv[idx]), ZB_ACPI_BATCAP_PATH, (void *)NULL);
#  else
	       concatl(batt, ZB_ACPI_PATH_SIZE, dirname(globuf.gl_pathv[idx]), ZB_ACPI_BATCAP_PATH, (void *)NULL);
#  endif

	       ZB_DBG("bytes: `%lu`\n", bytes - 0);
	       ZB_DBG("ZB_ACPI_PATH_SIZE: `%lu'\n", ZB_ACPI_PATH_SIZE);
	       ZB_DBG("batt: `%s`\n", batt);
	       /* else, find AC adapter */
	  } else if (strncmp(tmp, ZB_ACPI_ACTYPE, 4) == 0) {
#  if ZB_DEBUG
	       bytes = concatl(ac, ZB_ACPI_PATH_SIZE, dirname(globuf.gl_pathv[idx]), ZB_ACPI_ACSTAT_PATH, (void *)NULL);
#  else
	       concatl(ac, ZB_ACPI_PATH_SIZE, dirname(globuf.gl_pathv[idx]), ZB_ACPI_ACSTAT_PATH, (void *)NULL);
#  endif
	       ZB_DBG("bytes: `%lu`\n", bytes - 0);
	       ZB_DBG("ZB_ACPI_PATH_SIZE: `%lu'\n", ZB_ACPI_PATH_SIZE);
	       ZB_DBG("ac: `%s'\n", ac);
	  }
     }
}
# else
void get_pwr_files(glob_t globuf, char *ac, char *batt, int limit)
{
     FILE *fp;
     char path[ZB_ACPI_PATH_SIZE];
     char tmp[ZB_ACPI_TYPE_SIZE];

     for (int idx = 0; idx < (int)globuf.gl_pathc; ++idx) {
	  if (idx > 0)
	       bzero(path, ZB_ACPI_PATH_SIZE);

	  if ((fp = fopen(globuf.gl_pathv[idx], "r")) == NULL)
	       continue;

	  fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);
	  fclose(fp);

	  /* find batteries */
	  if (strncmp(tmp, ZB_ACPI_BATTYPE, 3) == 0 && limit-- > 0) {
	       if (limit != 0)
		    continue;

	       strcpy(path, dirname(globuf.gl_pathv[idx]));
	       strncat(path, ZB_ACPI_BATCAP_PATH, ((ZB_ACPI_PATH_SIZE) - strlen(path) - 1));
	       ZB_DBG("path: `%s`\n", path);
	       memcpy(batt, path, ZB_ACPI_PATH_SIZE);
	       /* else, find AC adapter */
	  } else if (strncmp(tmp, ZB_ACPI_ACTYPE, 4) == 0) {
	       strcpy(path, dirname(globuf.gl_pathv[idx]));
	       strncat(path, ZB_ACPI_ACSTAT_PATH, (ZB_ACPI_PATH_SIZE - strlen(path) - 1));
	       ZB_DBG("path: `%s`\n", path);
	       memcpy(ac, path, ZB_ACPI_PATH_SIZE);
	  }
     }
}
# endif

void pwr_info(struct pwr_sup *info, struct error *err, int btnum)
{
     ZB_DBG("%s: %d\n", "ZB_LINUX", ZB_LINUX);

     glob_t globuf;
     int globr;

     globr = glob(ZB_ACPI_GLOB, 0, NULL, &globuf);
     ZB_DBG("GLOB_NOSPACE: %d\n", GLOB_NOSPACE);
     ZB_DBG("GLOB_ABORTED: %d\n", GLOB_ABORTED);
     ZB_DBG("GlOB_NOMATCH: %d\n", GLOB_NOMATCH);
     ZB_DBG("globr: %d\n", globr);
     if (globr != 0) {
	  switch (globr) {
	  case GLOB_NOSPACE:
	       break;
	  case GLOB_ABORTED:
	       break;
	  case GLOB_NOMATCH:
	       zb_eset(err, PWR_ENOSUPLY);
	       return;
	  default:
	       break;
	  }
     }
     ZB_DBG("%s\n", "lulz, I haven't crashed yet, derp! :P");

     if (globuf.gl_pathc == 0) {
	  zb_eset(err, PWR_ENOSUPLY);
	  return;
     }

     ZB_DBG("globuf.gl_pathc: %lu\n", globuf.gl_pathc);
# if ZB_DEBUG > 1
     for (int idx = 0; idx < globuf.gl_pathc; ++idx) {
	  ZB_DBG("globuf.gl_pathv[%d]: `%s`\n", idx, globuf.gl_pathv[idx]);
     }
# endif
     if (btnum > (int)globuf.gl_pathc)
	  btnum = 1;

     ZB_DBG("btnum: %d\n", btnum);
     ZB_DBG("ZB_ACPI_PATH_SIZE: %lu\n", ZB_ACPI_PATH_SIZE);
     ZB_DBG("ZB_ACPI_TYPE_SIZE: %lu\n", ZB_ACPI_TYPE_SIZE);

     char ac[ZB_ACPI_PATH_SIZE];
     char batt[ZB_ACPI_PATH_SIZE];


     get_pwr_files(globuf, ac, batt, btnum);
     globfree(&globuf);

     ZB_DBG("limit: %d\nbatts: `%s'\n", btnum, batt);
     ZB_DBG("ac: `%s'\n", ac);

     read_pwr_files(info, err, ac, batt, btnum);

     ZB_DBG("info.acline: %d\n", info->acline);

#  if ZB_DEBUG
     ZB_DBG("info.cap: %d\n", info->cap);
#  endif
}
# elif defined(HAVE__PROC_ACPI)
# endif
#endif
