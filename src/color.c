#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "headers/color.h"

struct pwr_src_t {
  bool batt;
  bool ac;
};

struct power_t {
  int charge;
  struct pwr_src_t source;
} power;

#if _ZB_UNIX_BSD
inline void
init()
{
  size_t size;
  int ac_line;
  size = sizeof(int);
  _ZB_DEBUG("%s\n", "getting hw.acpi.acline");
  sysctlbyname("hw.acpi.acline", &ac_line, &size, NULL, false);
  power.source.ac = (bool)ac_line;
  power.source.batt = !power.source.ac;
  _ZB_DEBUG("%s\n", "getting hw.acpi.battery.life");
  sysctlbyname("hw.acpi.battery.life", &ac_line, &size, NULL, false);
  power.charge = (int)(ac_line / 10);
}

inline void
disp_pwr_info(struct color_disp_options_t opts)
{
  if (!opts.blink) {
    printf("%s", );
  } else {
    if (power.charge <= opts.blink_threshold) {
      if (power.soure.ac)
        printf("%s", opts.acblink ? _ZB_COLOR_RED_BLINK : _ZB_COLOR_RED);
      else
        printf("%s", _ZB_COLOR_RED_BLINK);
    } else {
      printf("%s", _ZB_COLOR_RED);
    }
  }
}
#endif
