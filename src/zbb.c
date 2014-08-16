/****
  /- filename -/
  zbb.c

  /- copyright -/
  Copyright (c) 2014 Alexej Magura

  This file is part of Zelda Battery.

  Zelda Battery is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Zelda Battery is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Zelda Battery.  If not, see <http://www.gnu.org/licenses/>.
****/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h> /* needs -lm */
#include <libacpi.h> /* needs -lacpi */

#include "zbb.h"

struct power_t
zbb__init(void)
{
  struct power_t power;
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
    power.on_acpower = 1;
  else if (acstate == SUCCESS && ac->ac_state == P_BATT)
    power.on_acpower = 0;
  else
    power.on_acpower = -1; /* error occured */

  if (battstate == SUCCESS) {
    for (int idx = 0; idx < global->batt_count; ++idx) {
      binfo = &batteries[idx];
      /* read current battery values */
      read_acpi_batt(idx);

      /* if battery present, then get charge and
         convert it from a percentage into an integer */
      if (binfo->present) {
        charge = (float)binfo->percentage;
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
