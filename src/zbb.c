#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <math.h> /* needs -lm */
#include <libacpi.h> /* needs -lacpi */

#include "zbb.h"

struct power_t
zbb__init(void)
{
  struct power_t power;
  power.on_acpower = false;
  float charge = 0.0;

  int acstate, battstate;

  /* global structure is _the_ acpi structure here */
  global_t *global = malloc (sizeof (global_t));
  battery_t *binfo;
  adapter_t *ac = &global->adapt;

  /* check for acpi support */
  if (check_acpi_support() == -1) // if true, then no acpi support
    exit(24);

  /* initialize battery and acstate */
  battstate = init_acpi_batt(global);
  acstate = init_acpi_acadapt(global);

  /* set on_acpower to true, if we are, in fact, running on AC */
  if (acstate == SUCCESS && ac->ac_state == P_AC)
    power.on_acpower = true;

  if (battstate == SUCCESS) {
    for (int idx = 0; idx < global->batt_count; ++idx) {
      binfo = &batteries[idx];
      /* read current battery values */
      read_acpi_batt(idx);

      /* if battery present, then get charge and
         convert it from a percentage into an integer */
      if (binfo->present) {
        charge = (float)45;//binfo->percentage;
        charge /= 100;
        charge *= 10;

        /* the math doesn't work quite right...
           for some reason if the charge is >= to 10, but < 20, then
           charge ends up being 1, which is wrong.  It should be zero. */
        if ((int)charge <= 1)
          --charge;

        /* if charge isn't a whole number, make it one */
        charge = floor(charge);
        power.charge = (int)charge;
      }
    }
  } else {
    /* no battery present, or there was an error
       reading from said battery. */
    exit(11);
  }
  return power;
}
