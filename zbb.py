"""
  /- filename -/
  zblib.py

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
from ctypes import cdll
from subprocess import getoutput as run
from re import search#, match;
from sys import platform

_acBlink = False

def onAcPower():
  if platform.startswith('linux'):
    return run('acpi -a').find('on-line') is not -1
  #elif platform.startswith('freebsd'):
    #return search('AC', match('.*on-line.*', run('apm')).group()) is not None
  #elif platform.startswith('darwin'):
    #return search('Power', match('.*\*.*', match('.*AC.*', run('pmset -g ps')))) is not None

def perc_tInt(perc):
  perc = float(perc)
  perc /= 100
  perc *= 10
  return int(perc)

def powerLvl():
  if platform.startswith('linux'):
    return search('([0-9]+)%', run('acpi')).group(1)
  #elif sys.platform.startswith('freebsd'):

def heart(boo, n):
  return [[ bytes('\u2665', 'utf-8').decode('utf-8', 'ignore'), bytes('\u2661', 'utf-8').decode('utf-8', 'ignore') ][~boo]]*n;
