/****
  /- filename -/
  cppToScheme.c

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
#include <stdbool.h>

const char *
cppToScheme__host_acpi_util(void)
{
#ifndef ZELDA_BATTERY_UTIL
#define ZELDA_BATTERY_UTIL ""
#endif
  return ZELDA_BATTERY_UTIL;
}

bool
cppToScheme__blink_on_ac_pwr(void)
{
#ifndef ZELDA_BATTERY_BLINK_EVEN_ON_AC
#define ZELDA_BATTERY_BLINK_EVEN_ON_AC false
#endif
  return ZELDA_BATTERY_BLINK_EVEN_ON_AC;
}
