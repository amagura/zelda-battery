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
# include "acpi.h"

# if HAVE__SYS_CLASS_POWER_SUPPLY
#  define ZB_ACPI_ROOT "/sys/class/power_supply"
#  define ZB_ACPI_GLOB ZB_ACPI_ROOT "/*/type"
#  define ZB_ACPI_PATH_SIZE (sizeof(ZB_ACPI_ROOT ZB_ACPI_GLOB ZB_ACPI_ROOT)) /* the `""` here adds 1 to the overall length */
#  define ZB_ACPI_BATTYPE "Battery"
#  define ZB_ACPI_ACTYPE "Mains"
#  define ZB_ACPI_TYPE_SIZE (sizeof(ZB_ACPI_BATTYPE ZB_ACPI_ACTYPE ""))
#  define ZB_ACPI_BATCAP_PATH "/capacity"
#  define ZB_ACPI_ACSTAT_PATH "/online"


inline int read_pwr_files(struct pwr_sup *info, char *ac, char *batt, int btlimit)
{
     int result = 0;
     FILE *fp;
     char *tmp = malloc(ZB_ACPI_TYPE_SIZE);
     /* replaces: for (int jdx = 0; jdx < limit; ++jdx) { */
     ZB_DBG("btlimit: %d\n", btlimit);
     if (btlimit == 0)
	  goto ac_adapter;

     if ((fp = fopen(batt, "r")) == NULL) {
	  /* a battery has been removed since the last call to find_pwr_files */
	  result = errno;
	  goto cleanup;
     }
     fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);

     /* get battery percentage levels */
     ZB_DBG("batt cap: %s\n", tmp);
     ZB_STRTONUM(info->cap[btlimit], tmp);
     fclose(fp);
     memset(tmp, '\0', ZB_ACPI_TYPE_SIZE);

ac_adapter:
     if ((fp = fopen(ac, "r")) == NULL) {
	  /* the A/C adapter was removed...
	   * (is this even possible while the machine's running?
	   * Maybe in Multics?) */
	  result = errno;
	  goto cleanup;
     }
     fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);

     /* get AC adapter state */
     ZB_DBG("acline: %s\n", tmp);
     int *kdxtmp = malloc(sizeof(*kdxtmp));
     ZB_STRTONUM(*kdxtmp, tmp);
     info->acline = (bool)*kdxtmp;
     free(kdxtmp);
     fclose(fp);

cleanup:
     free(tmp);

     return result;
}

inline int get_pwr_files(glob_t globuf, char *ac, char *batt, int limit)
{
     int result = 0;
     FILE *fp;
#  if ZB_USE_KCAT
     char *path = malloc(ZB_ACPI_PATH_SIZE);
#  else
     char path[ZB_ACPI_PATH_SIZE];
#  endif
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
     if ((int)globuf.gl_pathc <= 0)
	  goto cleanup;
     for (int idx = 0; idx < (int)globuf.gl_pathc; ++idx) {
#  if !ZB_USE_KCAT /* Disables the following so as to
		    * prevent memory corruption when we `malloc'.
		    */
	  idx && memset(path, '\0', ZB_ACPI_PATH_SIZE);
#  endif
	  fp = fopen(globuf.gl_pathv[idx], "r");

	  if (fp == (NULL)) {
	       fclose(fp);
	       result = errno;
	       goto cleanup;
	  }
	  fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);
	  fclose(fp);

	  /* find batteries */
	  if (strncmp(tmp, ZB_ACPI_BATTYPE, 3) == 0 && limit-- > 0) {
	       if (limit != 0)
		    continue;
#  if ZB_USE_KCAT
	       path = neko(dirname(globuf.gl_pathv[idx]), ZB_ACPI_BATCAP_PATH, NULL);
#  else
	       strcpy(path, dirname(globuf.gl_pathv[idx]));
	       strncat(path, "/capacity", ((ZB_ACPI_PATH_SIZE) - strlen(path) - 1));
#  endif
	       ZB_DBG("path: %s\n", path);
	       memcpy(batt, path, ZB_ACPI_PATH_SIZE);
#  if ZB_USE_KCAT
	       free(path);
#  endif
	       /* else, find AC adapter */
	  } else if (strncmp(tmp, ZB_ACPI_ACTYPE, 4) == 0) {
#  if ZB_USE_KCAT
	       path = neko(dirname(globuf.gl_pathv[idx]), ZB_ACPI_ACSTAT_PATH, NULL);
#  else
	       strcpy(path, dirname(globuf.gl_pathv[idx]));
	       strncat(path, "/online", (ZB_ACPI_PATH_SIZE - strlen(path) - 1));
#  endif
	       ZB_DBG("path: %s\n", path);
	       memcpy(ac, path, ZB_ACPI_PATH_SIZE);
#  if ZB_USE_KCAT
	       free(path);
#  endif
	  }
     }

cleanup:

     free(tmp);
     return result;
}

int pwr_info(struct pwr_sup *info, int btlimit)
{
     ZB_DBG("%s: %d\n", "ZB_LINUX", ZB_LINUX);
     if (btlimit < 0) {
	  return EINVAL;
     }
     int err = 0;
     glob_t globuf;

     glob(ZB_ACPI_GLOB, 0, NULL, &globuf);

     ZB_DBG("%s\n", "lulz, I haven't crashed yet, derp! :P");

     if (globuf.gl_pathc == 0)
	  return ZB_PWR_MISSING;

     if (btlimit > (int)globuf.gl_pathc) {
	  /* way more than ever needed,
	   * since this will also find A/C power supplies
	   * in addition to batteries. */
	  btlimit = (int)globuf.gl_pathc;
     }

     ZB_DBG("btlimit: %d\n", btlimit);
     ZB_DBG("ZB_ACPI_PATH_SIZE: %lu\n", ZB_ACPI_PATH_SIZE);


     char ac[ZB_ACPI_PATH_SIZE+1];
     char batt[ZB_ACPI_PATH_SIZE+1];

     err = get_pwr_files(globuf, ac, batt, btlimit);
     globfree(&globuf);

     if (err != 0)
	  goto cleanup;

     ZB_DBG("limit: %d\nbatts: `%s'\n", btlimit, batt);
     ZB_DBG("ac: `%s'\n", ac);

     err = read_pwr_files(info, ac, batt, btlimit);

cleanup:
     ZB_DBG("info.acline: %d\n", info->acline);

#  if ZB_DEBUG
     for (int mdx = 0; mdx < --btlimit; ++mdx) {
	  ZB_DBG("info.cap[%d]: %d\n", mdx, info->cap[mdx]);
     }
#  endif

     return err;
}

#  if 0
int main()
{
     struct pwr_sup info;
     int limit = 1;
     info.cap = NULL;
     info.cap = malloc(sizeof(info.cap)*limit);
     info.acline = false;
     pwr_inf(&info, limit);
     free(info.cap);
}
#  endif
# else
#  define ZB_ACPI_ROOT "/proc/acpi/"
#  define ZB_ACPI_GLOB ZB_ACPI_ROOT // FIXME glob not known
#  define ZB_ACPI_PATH_SIZE (sizeof(ZB_ACPI_ROOT ZB_ACPI_GLOB ZB_ACPI_ROOT)) /* the `""` here adds 1 to the overall length */
#  define ZB_ACPI_BATCAP_PATH
#  define ZB_ACPI_BATTYPE
#  define ZB_ACPI_ACTYPE
# endif
#endif
