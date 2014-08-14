"""
  /- filename -/
  zbl.py

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
"""

def main():
  perc = perc_tInt(powerLvl())
  print("\033[0;31m" if int(powerLvl()) <= 30 else (
    "\033[5;31m" if _acBlink else "\033[0;31m",
    "\033[5;31m"
  )[int(~onAcPower())], end="")

if __name__ == "__main__":
  main()
