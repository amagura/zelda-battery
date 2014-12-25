#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <unistd.h>
#include <libacpi.h> /* needs -lacpi */

#if _ZB_MAKING_ZB_COLOR
#define _ZB_PROGNAME "zbac"
#else
#define _ZB_PROGNAME "zbat"
#endif


#define _ZB_ERROR(format, ...) \
  do { \
    fprintf(stderr, "%s:error: ", _ZB_PROGNAME); \
    fprintf(stderr, (format), (__VA_ARGS__)); \
  } while(0)

struct pwr_src_t {
  bool batt;
  bool ac;
};

struct power_t {
  int charge;
  struct pwr_src_t source;
} power;

inline void
init()
{
  power.charge = 10;
  bool ac_support = false;
  bool batt_support = false;

  global_t *global = malloc(sizeof(global_t));
  battery_t *binfo = NULL;
  adapter_t *ac = &global->adapt;

  if (check_acpi_support() == -1) { // if true, no acpi support
    _ZB_ERROR("%s\n", "no acpi support");
    exit(EXIT_FAILURE);
  }
  ac_support = init_acpi_acadapt(global);
  batt_support = init_acpi_batt(global);

  /* sets `on_acpwr' to true if we are running on A/C */
  if (ac_support == SUCCESS) {
    if (ac->ac_state == P_AC) {
      power.source.batt = false;
      power.source.ac = true;
    }
    else if (ac->ac_state == P_BATT) {
      power.source.ac = false;
      power.source.batt = true;
    }
  } /* else... what happens if ac_support != SUCCESS... and is that even possible */

  if (batt_support == SUCCESS) {
    int idx = 0;
    do {
      binfo = &batteries[idx];
      read_acpi_batt(idx); // read current battery value

      if (binfo->present) {
        // XXX (int) here truncates stuff like `9.5' from `95 / 10'
        power.charge = (int)(binfo->percentage / 10);
      } else {
        continue;
      }
    } while (++idx < global->batt_count);
  }
  free(global);
}

#if _ZB_MAKING_ZB_COLOR
inline void
disp_pwr_info(int blink_threshold, bool blink)
{
  /* NOTE for the sake of readability, some negligible code duplication occurs */

  init(); // duplication. ;)
  if (power.charge <= blink_threshold && blink) {
    if (power.source.ac)
      printf("%s", (blink ? "\033[5;31m" : "\033[0;31m"));
  } else {
    printf("%s", "\033[0;31m");
  }
}
#else
#define _ZB_FULL_HEART "\u2665"
#define _ZB_EMPTY_HEART "\u2661"

inline void
disp_pwr_info()
{
  init(); // duplication. ;)
  
  for (int hdx = 1; hdx <= power.charge; ++hdx) {
    printf("%s", _ZB_FULL_HEART);
  }
  
  for (; power.charge < 10; ++power.charge) {
    printf("%s", _ZB_EMPTY_HEART);
  }
}
#endif

int
main()
{
#if _ZB_MAKING_ZB_COLOR
  disp_pwr_info(3, true);
#else
  disp_pwr_info();
#endif
  return EXIT_SUCCESS;
}
