#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <unistd.h>
#include <sysctl.h> /* needs -lc */

#if _ZB_MAKING_ZB_COLOR
#define _ZB_PROGNAME "zbac"
#else
#define _ZB_PROGNAME "zbat"
#endif

inline bool
on_ac_pwr()
{
  int size;
  int ac_line;
  size = sizeof(int);
  sysctlbyname("hw.acpi.acline", &ac_line, &size, NULL, false);
  return (bool)ac_line;
}

int
main()
{
  printf("%d\n", ac_line);
}
