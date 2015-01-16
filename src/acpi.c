#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>
#include <glob.h>
#include <limits.h>
#include "main.h"
#include "acpi.h"

struct pwr_sup {
  bool state;
  int *cap;
};

#ifndef ZB_SYS_CLASS
#define ZB_SYS_CLASS 1
#endif

#if ZB_SYS_CLASS
#define ZB_ACPI_ROOT "/sys/class/power_supply"
#define ZB_ACPI_GLOB ZB_ACPI_ROOT "/*/type"
#define ZB_ACPI_PATH_SIZE (sizeof(ZB_ACPI_ROOT ZB_ACPI_GLOB ZB_ACPI_ROOT))/* the `""` here adds 1 to the overall length */
#define ZB_ACPI_BATTYPE "Battery"
#define ZB_ACPI_ACTYPE "Mains"

inline int read_pwr_files(struct pwr_sup info, char *ac, char **batt)
{
  int result = 0;

  return result;
}

inline int get_pwr_files(glob_t globuf, char *ac, char **batt, int limit)
{
  int result = 0;
  FILE *fp;
  char path[ZB_ACPI_PATH_SIZE];
  const size_t tmp_size = 256;
  /* the files we'll be reading only consist of
   * a single line of little text,
   * often no more than a word; but
   * definitely no more than 3
   * to 5 words (i.e. a sentence). */
  char *tmp = malloc(tmp_size);

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
    memset(path, '\0', ZB_ACPI_PATH_SIZE);
    fp = fopen(globuf.gl_pathv[idx], "r");

    if (fp == (NULL)) {
      fclose(fp);
      result = errno;
      goto cleanup;
    }
    fgets(tmp, tmp_size, fp);

    /* find batteries */
    if (strncmp(tmp, ZB_ACPI_BATTYPE, 3) == 0 && limit-- > 0) {
      fclose(fp);
      //memcpy(path, dirname(globuf.gl_pathv[idx]), ZB_ACPI_PATH_SIZE);
      //bzero(path, strlen(path));
      strcpy(path, dirname(globuf.gl_pathv[idx]));
      strncat(path, "/capacity", ((ZB_ACPI_PATH_SIZE) - strlen(path) - 1));
      ZB_DBG("path: %s\n", path);
      strcpy(batt[limit], path);
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
  read_pwr_files(info, ac, batt);

  ZB_DBG("batts: `%s'\n", batt[0]);
  ZB_DBG("ac: `%s'\n", ac);

  for (int kdx = 0; kdx < btlimit; ++kdx) {
    free(batt[kdx]);
  }



#if 0
  /* we only want to return as many batteries
   * as we are allowed to find */
  int cap[*batlim];


    fclose(fp);
    fp = fopen(path, "r");

    if (fp == NULL) {
      fclose(fp);
      *info[2] = errno;
      goto cleanup;
    }
    fgets(tmp, tmp_size, fp);




	       fp = fopen(tmp, "r");
	       tmp =
	       dirname(global.gl_pathv[idx])

  }
    }
  }
 cleanup:
  free(tmp);
  globfree(&globuf);
#endif
return 0;
}

int main()
{
  struct pwr_sup info;
  info.cap = NULL;
  info.state = false;
  pwr_inf(info, 1);
}

#else
#define ZB_ACPI_ROOT "/proc/acpi/"
#define ZB_ACPI_GLOB "/proc/acpi/"
#define ZB_ACPI_PATH_SIZE (sizeof(ZB_ACPI_ROOT ZB_ACPI_GLOB "")) /* the `""` here adds 1 to the overall length */
#define ZB_ACPI_BATCAP_PATH
#define ZB_ACPI_BATTYPE
#define ZB_ACPI_ACTYPE
#endif

#if 0
void /* memory leak free :) */
find_battpath(char *path, char *buf, size_t bufsize, int *batnum)
{
  glob_t globuf;
  int r = glob(path, GLOB_MARK, NULL, &globuf);
  /* FIXME, `r' should be checked */
  FILE *fp;
  char *tmp = malloc(sizeof(char)*1024);

  for (int idx = 0; idx < globuf.gl_pathc; ++idx) {
    fp = fopen(globuf.gl_pathv[idx], "r");
    ZB_DBG("path: %s\n", globuf.gl_pathv[idx]);

    if (fp == NULL) {
      perror(ZB_PROGNAME);
      fclose(fp);
      buf = NULL;
      goto end;
    }
    fgets(tmp, sizeof tmp, fp);

    if (strncmp(tmp, "Batt", 3) == 0) {
      fclose(fp); // close fp
      if (batnum && !(*batnum == idx))
        continue;
      if (sizeof dirname(globuf.gl_pathv[idx]) < bufsize)
        strcpy(buf, dirname(globuf.gl_pathv[idx]));
      goto end; // cleanup
    } else {
      fclose(fp); // close fp
      if (batnum && *batnum == idx) {
        /* batnum will be -1 if battery not found */
        *batnum = -1;
        goto end;
      }
      if ((idx + 1 < globuf.gl_pathc))
        memset(tmp, '\0', sizeof(*tmp));
    }
  }
end:
  free(tmp);
  globfree(&globuf);
}

#if 0
/* find_battpath example */

int main()
{
  char tmp[BUFSIZ];
  find_battpath("/sys/class/power_supply/*/type", tmp, sizeof tmp, 0);
  printf("%s\n", tmp);
  return 0;
}
#endif

void /* memory leak free */
bcapcity(char *battpath, int *dst)
{
  FILE *fp;
  char *tmp = malloc(sizeof(char)*1024);

  fp = fopen(battpath, "r");
  if (fp == NULL) {
    perror(ZB_PROGNAME);
    *dst = -1;
    goto end;
  }
  fgets(tmp, sizeof tmp, fp);
  ZB_DBG("capacity: %s\n", tmp);
  ZB_STRTONUM(*dst, tmp);
end:
  fclose(fp);
  free(tmp);
}

#if 0
/* bcapcity example */

int main()
{
  char tmp[BUFSIZ];
  int cap = 0;

  find_battpath("/sys/class/power_supply/*/type", tmp, sizeof tmp, NULL);

#if 0
  if (batnum == -1) {
    ZB_ERROR("%s: '%d'", "no such battery", batnum);
    exit(EXIT_FAILURE);
  }
#endif

  ZB_DBG("tmp: %s\n", tmp); //

  strnact(tmp, "/capacity", (sizeof tmp - strlen(tmp) - 1));

  ZB_DBG("file: %s\n", tmp);
  bcapcity(tmp, &cap);
  printf("cap: %d\n", cap);
  return 0;
}
#endif

void
find_acpath(char *pth, char *buf, size_t bufsize)
{
  glob_t globuf;
  int r = glob(pth, GLOB_MARK, NULL, &globuf);
  /* `r' should be checked FIXME */
  FILE *fp;
  char *tmp = malloc(sizeof(char)*1024);

  for (int kdx = 0; kdx < globuf.gl_pathc; ++kdx) {
    fp = fopen(globuf.gl_pathv[kdx], "r");
    ZB_DBG("path: %s\n", globuf.gl_pathv[kdx]);

    if (fp == NULL) {
      perror(ZB_PROGNAME);
      /* FIXME, we should do something more informative and useful */
      fclose(fp);
      buf = NULL;
      goto end;
    }
    fgets(tmp, sizeof tmp, fp);

    if (strncmp(tmp, "Main", 4) == 0) {
      fclose(fp); // close file pointer
      if (sizeof dirname(globuf.gl_pathv[kdx]) < bufsize)
        strcpy(buf, dirname(globuf.gl_pathv[kdx]));
      goto end; // cleanup
    } else {
      if ((kdx + 1 < globuf.gl_pathc))
        memset(tmp, '\0', sizeof(*tmp));
    }
  }
end:
    free(tmp);
    globfree(&globuf);
}

#if 0
/* find_acpath example */

int main()
{
  char tmp[BUFSIZ];
  find_acpath("/sys/class/power_supply/*/type", tmp, sizeof tmp);
  printf("%s\n", tmp);
  return 0;
}
#endif

/* no leaks */
void /* can be used to determine if system is on AC power */
actat(char *pth, bool *dst)
{
  FILE *fp;
  /* we only need to get a single character */
  char *tmp = malloc(sizeof(char)*64);

  fp = fopen(pth, "r");
  if (fp == NULL) {
    perror(ZB_PROGNAME);
    *dst = -1;
    goto end;
  }
  fgets(tmp, sizeof tmp, fp);
  ZB_DBG("capacity: %s\n", tmp);
  ZB_STRTONUM(*dst, tmp);
end:
  fclose(fp);
  free(tmp);
}

#if 0
int main()
{
  char tmp[BUFSIZ];
  bool cap = false;

  find_acpath("/sys/class/power_supply/*/type", tmp, sizeof tmp);

  strncat(tmp, "/online", (sizeof tmp - strlen(tmp) - 1));

  actat(tmp, &cap);
  printf("acline: %d\n", cap);
}
#endif
#endif
