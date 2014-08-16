/****
  /- filename -/
  zba.c

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

int
main(void)
{
  struct power_t power;
  power = zbb__init();

  /* print the appropriate number of full hearts */
  for (int idx = 0; idx < power.charge; ++idx)
    printf("%s", "\u2665");

  /* print the appropriate number of empty hearts */
  for (int idx = 10; idx > power.charge; --idx)
    printf("%s", "\u2661");
  return EXIT_SUCCESS;
}
