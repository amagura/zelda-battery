/****
Copyright 2014 Alexej Magura

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
#if !ZB_BSD

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>
#include <glob.h>
#include <limits.h>
#include "main.h"
#include "acpi.h"

/* for Emacs; so that it will catch errors in the code I'm writing */
#if !defined(HAVE__SYS_CLASS_POWR_SUPPLY) && !defined(HAVE__PROC_ACPI_AC_ADAPTER)
#define HAVE__SYS_CLASS_POWR_SUPPLY 1
#endif

#if HAVE__SYS_CLASS_POWER_SUPPLY
#define ZB_ACPI_ROOT "/sys/class/power_supply"
#define ZB_ACPI_GLOB ZB_ACPI_ROOT "/*/type"
#define ZB_ACPI_PATH_SIZE (((sizeof ZB_ACPI_ROOT ZB_ACPI_GLOB ZB_ACPI_ROOT)) + 1)/* the `""` here adds 1 to the overall length */
#define ZB_ACPI_BATTYPE "Battery"
#define ZB_ACPI_ACTYPE "Mains"
#define ZB_ACPI_TYPE_SIZE ((sizeof ZB_ACPI_BATTYPE ZB_ACPI_ACTYPE "") + 1)
#define ZB_CHECK_ERROR(ZB_ERR, ZB_LABEL)	\
  if ((ZB_ERR) != 0)				\
    goto ZB_LABEL;

inline int read_ac_file(struct pwr_sup *info, char *ac)
{
     int result = 0;
     FILE *fp;
     char tmp[ZB_ACPI_TYPE_SIZE];

     if ((fp = fopen(ac, "r")) == NULL) {
	  result = errno;
	  goto end;
     }
     fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);
     fclose(fp);

     /* get AC Adapter state */
     ZB_DBG("acline: %s\n", tmp);
     int *kdxtmp = malloc(sizeof(*kdxtmp));
     ZB_STRTONUM(*kdxtmp, tmp);
     info->acline = (bool)*kdxtmp;
     free(kdxtmp);

end:
     return result;
}

inline int read_batt_files(struct pwr_sup *info, char **batt, signed int limit)
{
     int result = 0;
     FILE *fp;
     char tmp[ZB_ACPI_TYPE_SIZE];

     int jdx = -1;
     while (++jdx < limit) {
	  ZB_DBG("jdx: %d\n", jdx);
	  if (jdx)
	       memset(tmp, '\0', ZB_ACPI_TYPE_SIZE);

	  if (batt == NULL) {
	       // no battery
	       *info->cap = ZB_PWR_MISSING;
	       ++info->cap;
	       continue;
	  }
	  ZB_DBG("batt[%d]: %s\n", jdx, *batt);

	  if (limit == -1)
	       fp = fopen(batt[0], "r");
	  else
	       fp = fopen(batt[limit * -1], "r");

	  if (fp == NULL) {
	       result = errno;
	       continue;
	  }
	  fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);

	  /* get battery percentage levels */
	  ZB_DBG("batt cap: %s\n", tmp);
	  ZB_STRTONUM(info->cap[jdx], tmp);
	  fclose(fp);
     }

     return result;
}

inline int find_batt_files(glob_t globuf, char **batt, signed int limit)
{
     ZB_DBG("limit: %d\n", limit);
     int result = ZB_PWR_OK;
     FILE *fp;

     /* the files we'll be reading only consist of
      * a single line of little text,
      * often no more than a word; but
      * definitely no more than 3
      * to 5 words (i.e. a sentence). */
     char *tmp = malloc(ZB_ACPI_TYPE_SIZE);

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
     if ((int)globuf.gl_pathc <= 0) {
	  // no batt files whatsoever
	  result = ZB_PWR_MISSING;
	  goto cleanup;
     }
     for (int idx = 0; idx < (int)globuf.gl_pathc; ++idx) {
	  if ((fp = fopen(globuf.gl_pathv[idx], "r")) == NULL) {
	       result = errno;
	       break;
	  }
	  fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);

	  /* find batteries */
	  if (strncmp(tmp, ZB_ACPI_BATTYPE, 3) == 0 && (limit != 0)) {
	       ZB_DBG("limit: %d\n", limit);
	       fclose(fp);
	       free(tmp);
	       tmp = concat(dirname(globuf.gl_pathv[idx]), "/capacity", NULL);
	       if (limit == -1)
		    memcpy(batt[0], tmp, ZB_ACPI_PATH_SIZE);
	       else
		    memcpy(batt[limit * -1], tmp, ZB_ACPI_PATH_SIZE);
	       ++limit;
	  } else {
	       fclose(fp);
	       break;
	  }
     }

cleanup:
     free(tmp);
     return result;
}


inline int find_ac_file(glob_t globuf, char *ac)
{
     int result = ZB_PWR_OK;
     FILE *fp;
     char *path = "";

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
     if ((int)globuf.gl_pathc <= 0)
	  return ZB_PWR_MISSING;
     int idx = 0;

     for (; idx < (int)globuf.gl_pathc; ++idx) {
	  if ((fp = fopen(globuf.gl_pathv[idx], "r")) == NULL) {
	       result = errno;
	       break;
	  }

	  fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);
	  if (strncmp(tmp, ZB_ACPI_ACTYPE, 4) == 0) {
	       fclose(fp);
	       path = concat(dirname(globuf.gl_pathv[idx]), "/online", NULL);
	       memcpy(ac, path, ZB_ACPI_PATH_SIZE);
	       free(path);
	  } else {
	       fclose(fp);
	       break;
	  }
     }
     return result;
}

int pwr_info(struct pwr_sup *info, int btlimit)
{
     if (btlimit < 0)
	  return EINVAL;
     int err = 0;
     glob_t globuf;
     mtrace();
     glob(ZB_ACPI_GLOB, 0, NULL, &globuf);

     ZB_DBG("%s\n", "lulz, I haven't crashed yet, derp! :P");

     if (globuf.gl_pathc == 0)
	  return ZB_PWR_VIRTMACH;

     if (btlimit > (int)globuf.gl_pathc) {
	  /* way more than ever needed,
	   * since this will also find A/C power supplies
	   * in addition to batteries. */
	  btlimit = (int)globuf.gl_pathc;
     }

     ZB_DBG("btlimit: %d\n", btlimit);
     ZB_DBG("ZB_ACPI_PATH_SIZE: %lu\n", ZB_ACPI_PATH_SIZE);

     char ac[ZB_ACPI_PATH_SIZE];
     char **batt = malloc(ZB_ACPI_PATH_SIZE*btlimit);

     if (btlimit != 0) {
	  int hdx = 0;
	  while (hdx < btlimit)
	       batt[hdx++] = malloc((ZB_ACPI_PATH_SIZE));
	  err = find_batt_files(globuf, batt, -(btlimit));
	  ZB_CHECK_ERROR(err, cleanup);

#if ZB_DEBUG
	  if (btlimit > 1) {
	       ZB_DBG("hello\n");
	       int jdx = 0;
	       for (; jdx < btlimit; ++jdx) {
		    ZB_DBG("batts: `%s'\n", (batt[jdx]));
	       }

	  } else {
	       ZB_DBG("batts: `%s'\n", *batt);
	  }
#endif

	  err = read_batt_files(info, batt, -(btlimit));
	  ZB_CHECK_ERROR(err, cleanup);

#if ZB_DEBUG
	  for (int mdx = 0; mdx < btlimit; ++mdx)
	       ZB_DBG("info.cap[%d]: %d\n", mdx, info->cap[mdx]);
#endif
     }

     err = find_ac_file(globuf, ac);
     ZB_CHECK_ERROR(err, cleanup);
     ZB_DBG("ac: `%s'\n", ac);
     err = read_ac_file(info, ac);

cleanup:
     globfree(&globuf);
     for (int kdx = 0; kdx < btlimit; ++kdx)
	  free(batt[kdx]);
     ZB_DBG("err: `%d'\n", err);
     ZB_DBG("info.acline: %d\n", info->acline);

     muntrace();
     return err;
}

#if 0
int main()
{
     struct pwr_sup info;
     int limit = 1;
     info.cap = NULL;
     info.cap = malloc(sizeof(info.cap)*limit);
     info.acline = false;
     pwr_inf(info, limit);
     free(info.cap);
}
#endif
#else
#define ZB_ACPI_ROOT "/proc/acpi/"
#define ZB_ACPI_GLOB "/proc/acpi/"
#define ZB_ACPI_PATH_SIZE (sizeof(ZB_ACPI_ROOT ZB_ACPI_GLOB "")) /* the `""` here adds 1 to the overall length */
#define ZB_ACPI_BATCAP_PATH
#define ZB_ACPI_BATTYPE
#define ZB_ACPI_ACTYPE
#endif
#endif
