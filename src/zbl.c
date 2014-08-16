/****
  /- filename -/
  zbl.c

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

#include "zbb.h"
#ifndef _ZB_ACBLINK
#define _ZB_ACBLINK 0 /* disabled by default */
#endif
#ifndef _ZB_NOBLINK
#define _ZB_NOBLINK 0 /* blinking is enabled by default */
#endif

int
main(void)
{
  struct power_t power;
  power = zbb__init();

  if (power.charge <= 3 && !_ZB_NOBLINK) {
    if (power.on_acpower)
      printf("%s", _ZB_ACBLINK ? "\033[5;31m" : "\033[0;31m");
  } else {
    printf("%s", "\033[0;31m");
  }
  return EXIT_SUCCESS;
}
