#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/sysctl.h> /* needs -lc */

#if _ZB_MAKING_ZB_COLOR
#define _ZB_PROGNAME "zbac"
#else
#define _ZB_PROGNAME "zbat"
#endif

inline bool
on_ac_pwr()
{
  size_t size;
  int ac_line;
  size = sizeof(int);
  sysctlbyname("hw.acpi.acline", &ac_line, &size, NULL, false);
  return (bool)ac_line;
}

inline int
charge()
{
  size_t size;
  int batt_charge;
  size = sizeof(int);
  sysctlbyname("hw.acpi.battery.life", &batt_charge, &size, NULL, false);
  return (int)(batt_charge / 10);
}

int
main()
{
#
  struct power_t {
    int charge;
    bool ac_line;
  } power;
  power.charge = charge();
  power.ac_line = on_ac_pwr();

  for (int hdx = 1; hdx <= 
}
