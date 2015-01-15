#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>
#include <glob.h>
#include "main.h"
#include "acpi.h"

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

  strncat(tmp, "/capacity", (sizeof tmp - strlen(tmp) - 1));

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
