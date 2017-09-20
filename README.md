ZBatt [![Build Status](https://travis-ci.org/amagura/zelda-battery.svg?branch=master)](https://travis-ci.org/amagura/zelda-battery) [![Circle CI](https://circleci.com/gh/amagura/zelda-battery/tree/master.svg?style=svg)](https://circleci.com/gh/amagura/zelda-battery/tree/master)
=============

A Legend of Zelda inspired health-bar battery meter for Shell prompts and Taskbars

<!--
* [Supported Platforms](https://github.com/amagura/zelda-battery#supported-platforms)
 * [Windows](https://github.com/amagura/zelda-battery#windows)
 * [Linux](https://github.com/amagura/zelda-battery#linux)
 * [*BSD](https://github.com/amagura/zelda-battery#bsd)
* [Depends](https://github.com/amagura/zelda-battery#dependencies)
 * [Make Depends](https://github.com/amagura/zelda-battery#make-dependencies)
* [Building](https://github.com/amagura/zelda-battery#building)
* [Installing](https://github.com/amagura/zelda-battery#installing)
* [Examples](https://github.com/amagura/zelda-battery#examples)
 * [Git-esque](https://github.com/amagura/zelda-battery#git-esque)
 * [Zsh](https://github.com/amagura/zelda-battery#zsh)
 * [Bash](https://github.com/amagura/zelda-battery#bash)
 * [Mksh](https://github.com/amagura/zelda-battery#mksh-korn-shell)
 * [Tcsh](https://github.com/amagura/zelda-battery#tcsh)
* [Notes](https://github.com/amagura/zelda-battery#notes)
-->
# Supported Platforms
### Linux
Kernels >= 2.6.24 are supported; support for older kernels is not a priority.
### BSD
Most BSDs should be supported: as long as you have [`sysctlbyname`](http://www.daemon-systems.org/man/sysctlbyname.3.html) you should be fine.

BSD systems known to work:
* DragonFlyBSD
* FreeBSD

BSD systems that _should_ work:
* NetBSD
* Midnight BSD
* (Anything based on FreeBSD)

BSD systems that _don't_ work:
<ul><li>OpenBSD</li></ul>

### Windows
Windows XP and later.

# Dependencies
### Linux and BSD
* Python 2 (GUI)
* GTK+2 (GUI)
* PyGTK (GUI)

### Windows
The Windows version of ZBatt just depends on the stuff that Windows provides, such as the Windows API, lulz.<br />
So you shouldn't need to install anything for ZBatt to work.

## Make Dependencies
* GNU Autotools (i.e. automake, autoconf)
* Make

### Linux and BSD
* C99 compliant C compiler
* Cython (GUI)
* pkg-config (GUI)
* [`sysctlbyname`](http://www.daemon-systems.org/man/sysctlbyname.3.html) (__BSD only__)

### Windows
* C++11 compliant C++ compiler

# Building
1. `autoreconf -fi`
2. `./configure`
3. `make`

# Installing
1. `make DESTDIR="<DIR>" install`

If the install fails because of permissions, try running it with `sudo`.
***
Alternatively, you can also just place the binaries `zbatc`, `zbatt`, and (if you built the GUI) `gzbatt` where ever you like.

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

TRAPALRM ()
{
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
