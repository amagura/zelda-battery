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

#if ZB_SYS_CLASS
#define ZB_ACPI_ROOT "/sys/class/power_supply"
#define ZB_ACPI_GLOB ZB_ACPI_ROOT "/*/type"
#define ZB_ACPI_PATH_SIZE (sizeof(ZB_ACPI_ROOT ZB_ACPI_GLOB ZB_ACPI_ROOT))/* the `""` here adds 1 to the overall length */
#define ZB_ACPI_BATTYPE "Battery"
#define ZB_ACPI_ACTYPE "Mains"
#define ZB_ACPI_TYPE_SIZE (sizeof(ZB_ACPI_BATTYPE ZB_ACPI_ACTYPE ""))

inline int read_pwr_files(struct pwr_sup *info, char *ac, char **batt, int limit)
{
  int result = 0;
  FILE *fp;
  char *tmp = malloc(ZB_ACPI_TYPE_SIZE);

  for (int jdx = 0; jdx < limit; ++jdx) {
    jdx && memset(tmp, '\0', ZB_ACPI_TYPE_SIZE);
    fp = fopen(batt[jdx], "r");

    if (fp == (NULL)) {
      fclose(fp);
      free(batt[jdx]);
      result = errno;
      goto cleanup;
    }
    fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);

    /* get battery percentage levels */
    ZB_DBG("batt cap: %s\n", tmp);
    ZB_STRTONUM(info->cap[jdx], tmp);
    fclose(fp);
  }
  fp = fopen(ac, "r");

  if (fp == (NULL)) {
    fclose(fp);
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

inline int get_pwr_files(glob_t globuf, char *ac, char **batt, int limit)
{
  int result = 0;
  FILE *fp;
  char path[ZB_ACPI_PATH_SIZE];
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
  for (int idx = 0; idx < (int)globuf.gl_pathc; ++idx) {
    idx && memset(path, '\0', ZB_ACPI_PATH_SIZE);
    fp = fopen(globuf.gl_pathv[idx], "r");

    if (fp == (NULL)) {
      fclose(fp);
      result = errno;
      goto cleanup;
    }
    fgets(tmp, ZB_ACPI_TYPE_SIZE, fp);

    /* find batteries */
    if (strncmp(tmp, ZB_ACPI_BATTYPE, 3) == 0 && limit-- > 0) {
      fclose(fp);
      //memcpy(path, dirname(globuf.gl_pathv[idx]), ZB_ACPI_PATH_SIZE);
      //bzero(path, strlen(path));
      strcpy(path, dirname(globuf.gl_pathv[idx]));
      strncat(path, "/capacity", ((ZB_ACPI_PATH_SIZE) - strlen(path) - 1));
      ZB_DBG("path: %s\n", path);
      memcpy(batt[limit], path, ZB_ACPI_PATH_SIZE);
      //strcpy(batt[limit], path);
      /* else, find AC adapter */
    } else if (strncmp(tmp, ZB_ACPI_ACTYPE, 4) == 0) {
      fclose(fp);
      //memcpy(path, dirname(globuf.gl_pathv[idx]), ZB_ACPI_PATH_SIZE);
      //bzero(path, strlen(path));
      strcpy(path, dirname(globuf.gl_pathv[idx]));
      //strcat(path, "/online");
      strncat(path, "/online", (ZB_ACPI_PATH_SIZE - strlen(path) - 1));
      //bzero(path, strlen(path));
      ZB_DBG("path: %s\n", path);
      memcpy(ac, path, ZB_ACPI_PATH_SIZE);
      //strcpy(ac, path);
    } else {
      fclose(fp);
    }
  }

 cleanup:

  free(tmp);
  return result;
}

int pwr_inf(struct pwr_sup info, int btlimit)
{
  if (btlimit < 0) {
    return EINVAL;
  }
  /* int cap[] */
  /* char *tmp */
  glob_t globuf;
  //FILE *fp;

  glob(ZB_ACPI_GLOB, 0, NULL, &globuf);

  if (btlimit > (int)globuf.gl_pathc) {
    /* way more than ever needed,
     * since this will also find A/C power supplies
     * in addition to batteries. */
    btlimit = (int)globuf.gl_pathc;
  }

  ZB_DBG("btlimit: %d\n", btlimit);
  ZB_DBG("ZB_ACPI_PATH_SIZE: %lu\n", ZB_ACPI_PATH_SIZE);


  char ac[ZB_ACPI_PATH_SIZE+1];
  char *batt[(ZB_ACPI_PATH_SIZE+1)*btlimit];

  for (int hdx = 0; hdx < btlimit; ++hdx) {
    batt[hdx] = malloc(ZB_ACPI_PATH_SIZE+1);
    ZB_DBG("hdx: %d\n", hdx);
  }

  get_pwr_files(globuf, ac, batt, btlimit);
  globfree(&globuf);

  ZB_DBG("batts: `%s'\n", batt[0]);
  ZB_DBG("ac: `%s'\n", ac);


  read_pwr_files(&info, ac, batt, btlimit);

  for (int kdx = 0; kdx < btlimit; ++kdx) {
    free(batt[kdx]);
    ZB_DBG("kdx: %d\n", kdx);
  }

  ZB_DBG("info.acline: %d\n", info.acline);

#if ZB_DEBUG
  for (int mdx = 0; mdx < btlimit; ++mdx) {
    ZB_DBG("info.cap[%d]: %d\n", mdx, info.cap[mdx]);
  }
#endif

  return 0;
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
