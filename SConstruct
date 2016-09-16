#!/usr/bin/env python

#  Copyright 2015 Alexej Magura
#
#  Copying and distribution of this file, with or without modification,
#  are permitted in any medium without royalty provided the copyright
#  notice and this notice are preserved.  This file is offered as-is,
#  without any warranty.

import subprocess;

env = Environment()

for t in [['zbatc', 'color.c'], ['zbatt', 'text.c']]:
    env.Program(t[0],
                (
                    'src/%s' % t[1],
                    'src/main.c',
                    'src/acpi.c',
                    'src/power.c',
                    'src/concat.c'
                ))
    t.reverse()
    t.pop()
    t.reverse()

env.Append(CPPDEFINES = ['NO_COMMON_H'])
env.Append(LINKFLAGS = ['-lm'])


def test(n):
    cmd = 'cd "%s"' % Dir('.').path
    if n is 0:
        subprocess.call('%s; mkdir -p test/{ACAD,BAT{0,1}}' % cmd, shell=True)
        for d in [['ACAD', 'type', 'Mains', 'online', '1'],
                  ['BAT0', 'type', 'Battery', 'capacity', '100'],
                  ['BAT1', 'type', 'Battery', 'capacity', '100']]:
            subprocess.call('%s; touch test/%s/%s' % (cmd, d[0], d[1]), shell=True)
            subprocess.call('%s; touch test/%s/%s' % (cmd, d[0], d[3]), shell=True)
            subprocess.call('%s; echo %s > test/%s/%s' % (cmd, d[4], d[0], d[3]), shell=True)
            subprocess.call('%s; echo %s > test/%s/%s' % (cmd, d[2], d[0], d[1]), shell=True)
    elif n is 1:
        subprocess.call('%s; rm -r test' % cmd, shell=True)


env.Action('test-setup', test(0))
