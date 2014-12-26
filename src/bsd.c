#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

#if _ZB_MAKING_ZB_COLOR
inline void
disp_pwr_info(int blink_thershold, bool blink)
{
}
#endif


int
main()
{
  struct power_t {
    int charge;
    bool ac_line;
  } power;
  power.charge = charge();
  power.ac_line = on_ac_pwr();
#if _ZB_MAKING_ZB_COLOR
  if (power.charge <= 3) {
    if (power.ac_line)
      printf("%s", "\033[0;31m");
    else
      printf("%s", "\033[5;31m");
  } else {
    printf("%s", "\033[0;31m");
  }
#else
#define _ZB_FULL_HEART "\u2665"
#define _ZB_EMPTY_HEART "\u2661"

  for (int hdx = 1; hdx <= power.charge; ++hdx) {
    printf("%s", _ZB_FULL_HEART);
  }
  
  for (; power.charge < 10; ++power.charge) {
    printf("%s", _ZB_EMPTY_HEART);
  }
#endif
}
