ZBatt [![Build Status](https://travis-ci.org/amagura/zelda-battery.svg?branch=master)](https://travis-ci.org/amagura/zelda-battery)
=============

A Legend of Zelda inspired health-bar battery meter for Shell prompts and Taskbars

# Supported Platforms
## Linux
Kernels >= 2.6.24 are supported; support for older kernels is not a priority, but may be added later.

## *BSD
Most BSDs should be supported: as long as you have [`sysctlbyname`](http://www.daemon-systems.org/man/sysctlbyname.3.html), you should be fine.  BSD systems known to work:
* DragonFly BSD
* FreeBSD

BSD systems that _should_ work:
* NetBSD
* Midnight BSD
* (Anything based on FreeBSD)

BSD systems known _not_ to work:
* OpenBSD

### Dependencies (v3.0+)
* GNU Autotools
* C99 compliant C compiler
* Cython
* Python 2
* GTK+2 (?)
* PyGTK (?)



The GUI (Graphical User Interface) requires __gtk2__ and the `configure` script will need __pkg-config__, but these are optional, since you can opt out of building GUI using the `--without-gtk2` `configure` flag.

### Building
1. `autoreconf -i`
2. `./configure`
3. `make`

### Installing
Just place `zbatc` (zbatt-color) and `zbatt` (zbatt-text) where ever you want them and add the appropriate stuff to your terminal prompt.
