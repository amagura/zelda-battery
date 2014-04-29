zelda-battery [![Build Status](https://travis-ci.org/dopm/zelda-battery.svg?branch=master)](https://travis-ci.org/dopm/zelda-battery)
=============

A Legend of Zelda like health-bar battery meter for the command-line


## Building
1. `autoreconf -fi`
2. `./configure`
3. `make`

__Note__ that `./configure` can take an `--enable-ac-blink` option, which is disabled by default.  Enabling this will cause Zelda Battery to blink on low battery _even_ when your computer is plugged into an AC power supply.

## Installing
Just place `zelda-blink` and `zelda-battery` where ever you want them and add the appropriate stuff to your terminal prompt.

### Zsh

