ZBatt [![Build Status](https://travis-ci.org/amagura/zelda-battery.svg?branch=master)](https://travis-ci.org/amagura/zelda-battery)
=============

A Legend of Zelda inspired health-bar battery meter for Shell prompts and Taskbars

* [Building](https://github.com/amagura/zelda-battery#building)
* [Installing](https://github.com/amagura/zelda-battery#installing)
* [Supported Platforms](https://github.com/amagura/zelda-battery#supported-platforms)
 * [Windows](https://github.com/amagura/zelda-battery#windows)
 * [Linux](https://github.com/amagura/zelda-battery#linux)
 * [*BSD](https://github.com/amagura/zelda-battery#bsd)
* [Depends](https://github.com/amagura/zelda-battery#deendencies)
 * [Make Depends](https://github.com/amagura/zelda-battery#make-dependencies)
* [Examples](https://github.com/amagura/zelda-battery#examples)
 * [Git-esque](https://github.com/amagura/zelda-battery#git-esque)
 * [Zsh](https://github.com/amagura/zelda-battery#zsh)
 * [Bash](https://github.com/amagura/zelda-battery#bash)
 * [Mksh](https://github.com/amagura/zelda-battery#mksh-korn-shell)
 * [Tcsh](https://github.com/amagura/zelda-battery#tcsh)
* [Notes](https://github.com/amagura/zelda-battery#notes)

# Building
1. `autoconf -i` (try `autoreconf -fi` if `autoconf -i` doesn't work)
2. `./configure`
3. `make`



# Installing

1. Build ZBatt
2. `make DESTDIR="<desired install directory>" install` (try running with either `sudo` or as root if you don't have permission to write to `<desired install directory>`.

Alternatively, you can also just place the binaries `zbatc`, `zbatt`, and (if you built the GUI) `gzbatt` where ever you like.

# Supported Platforms

This _probably_ goes without saying, but if you cannot get ZBatt to work or compile on
a supported platform, please send me an email detailing:

1. the misbehaving OS and version thereof
  * if you are on a UNIX-like system, please include the output from `uname -a`
2. the version of ZBatt that you are trying(?) to use
  * if a previous version of ZBatt was working, please include that info.

## Windows

All Windows systems XP and later are supported; support for older Windows versions will come later.

## Linux
Kernels >= 2.6.24 are supported; support for older kernels is not a priority, but may be added <s>if you say pretty please</s> later.

## *BSD
Most BSDs should be supported: as long as you have [`sysctlbyname`](http://www.daemon-systems.org/man/sysctlbyname.3.html), you should be fine.  BSD systems known to work (i.e. systems I've personally tested ZBatt on):

* DragonFlyBSD
* FreeBSD

BSD systems that _should_ work:
* NetBSD
* Midnight BSD
* (Anything based on FreeBSD, lulz)

BSD systems known _not_ to work:
* OpenBSD (soz, peeps)

# Dependencies

Windows:

The Windows version of ZBatt has no extraneous dependencies: it depends on the Windows API.

Linux and *BSD:

* Python 2 (GUI depend)
* GTK+2 (GUI depend)
* PyGTK (GUI depend)

## Make Dependencies

* GNU Autotools (i.e. automake, autoconf, (doesn't need to be GNU) make)

Windows:
* C++98 compliant C++ compiler

Linux:
* GNU Autotools (i.e. automake, autoconf, (doesn't need to be GNU) make)
* C99 compliant C compiler
* Cython (GUI make depend)
* pkg-config (GUI make depend)

\*BSD:
* GNU Autotools (i.e. automake, autoconf, (doesn't need to be GNU) make)
* C99 compliant C compiler
* Cython (GUI make depend)
* pkg-config (GUI make depend)
* [`sysctlbyname`](http://www.daemon-systems.org/man/sysctlbyname.3.html)

# Examples

The command-line is _extremely_ flexible, so while there are defaults
there's nothing stopping you from defining your own experience.

### Git-esque
![example of the git style](/example/git.jpg)
```bash
./zbatc -c 32; ./zbatt -r -f +; ./zbatc -c 31; ./zbatt -x -e -
```

To make it even easier to integrate ZBatt with your current command-line experience,
I've gone ahead and provided examples for some shells.

### Zsh
![example showing zelda-battery in a Zsh prompt](/example/zsh.jpg)
```bash
PROMPT="%{$(./zbatc)%}$(./zbatt)%{[0;0m%} %m%# "

## run TRAPALRM every $TMOUT seconds
TMOUT=60 # refresh the terminal prompt every 60 seconds

TRAPALRM () {
    zle reset-prompt # refreshs the terminal prompt
}
```

### Bash
![example showing zelda-battery in a Bash prompt](/example/bash.jpg)
```bash
PS1='\[$(./zbatc)\]$(./zbatt)\[\033[0;0m\] [\h \W]\$ '
```

### Mksh (Korn Shell)
![example showing zelda-battery in a Mksh prompt](/example/mksh.jpg)
```bash
PS1=$'$(./zbatc)'$(./zbatt)$'\033[0;0m'\ ["$(hostname)"]\
```

### Tcsh
![example showing zbatt in a tcsh prompt](/example/tcsh.jpg)
```bash
set prompt="%{`./zbatc`%}`./zbatt` %{\033[0;0m%}%m "
```

# Notes

## Zsh
As long as you use the `TMOUT` and `TRAPALRM`/`zle reset-prompt` stuff, your Zsh prompt should refresh itself every 60 seconds.

## Bash
AFAIK there is _no_ way to periodically refresh a bash prompt without either running `clear`/`Ctrl-L` or pressing enter, which causes the prompt to be redrawn.

## Submissions
Screenshot, command-line prompt examples, and all other submissions are appreciated as always; thank you.
