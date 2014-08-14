#!/usr/bin/env python2
from commands import getoutput as run;
import re
import sys

def onAcPower():
  if sys.platform.startswith('linux'):
    return run('acpi -a').find('on-line') is not -1
  elif sys.platform.startswith('freebsd'):
    return re.search('AC', re.match('.*on-line.*', run('apm')).group()) is not None
  elif sys.platform.startswith('darwin'):
    return re.search('Power', re.match('.*\*.*', re.match('.*AC.*', run('pmset -g ps')))) is not None


