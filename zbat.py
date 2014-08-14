#!/usr/bin/env python2
"""
  /- filename -/
  zbat.py

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
from commands import getoutput as run;
from kitchen.text.converters import to_bytes;
import re
import sys

_acBlink = False

def onAcPower():
  if sys.platform.startswith('linux'):
    return run('acpi -a').find('on-line') is not -1
  elif sys.platform.startswith('freebsd'):
    return re.search('AC', re.match('.*on-line.*', run('apm')).group()) is not None
  elif sys.platform.startswith('darwin'):
    return re.search('Power', re.match('.*\*.*', re.match('.*AC.*', run('pmset -g ps')))) is not None

def perc_tInt(perc):
  perc = float(perc)
  perc /= 100
  perc *= 10
  perc = int(perc)
  return perc

def powerLvl():
  if sys.platform.startswith('linux'):
    return re.search('([0-9]+)%', run('acpi')).group(1)
  #elif sys.platform.startswith('freebsd'):


def heart(boo, n):
  return [( to_bytes(u'\u2665'), to_bytes(u'\u2661'))[~boo] ]*n;

def main():
  perc = perc_tInt(powerLvl())
  color = "\033[0;31m" if powerLvl() <= 30 else (
    "\033[5;31m" if _acBlink else "\033[0;31m",
    "\033[5;31m"
  )[int(~onAcPower())]
  print '%s%s%s' % (
    color,
    ''.join(heart(1, perc)),
    ''.join(heart(0, 10 - perc)
  ));

if __name__ == "__main__":
  main()
