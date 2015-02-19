ZBatt [![Build Status](https://travis-ci.org/amagura/zelda-battery.svg?branch=master)](https://travis-ci.org/amagura/zelda-battery)
=============

A Legend of Zelda like health-bar battery meter for Shell prompts and Taskbars

# Supported Platforms
## Linux
Kernels >= 2.6.24 are supported; support for older kernels is not a priority, but may be added later.

## *BSD
Most BSDs should be supported: as long as you have `sysctlbyname`, you should be fine.  BSD systems known to work: 
* DragonFly BSD
* FreeBSD

### Dependencies (v3.0+)
* GNU Autotools
* C99 compliant C compiler

The GUI (Graphical User Interface) requires __gtk2__ and the `configure` script will need __pkg-config__ to be installed, but that's only if you're building the GUI in addition to the CLI (Command Line Interface).

### Building
1. `autoreconf -i`
2. `./configure`
3. `make`

### Installing
Just place `zbatc` (zbatt-color) and `zbatt` (zbatt-text) where ever you want them and add the appropriate stuff to your terminal prompt.

# Examples

## Styles
### Git
![example of the git style](/example/git.jpg)
```bash
./zbatc -c 32; ./zbatt -p -f +; ./zbatc -c 31; ./zbatt -m -e -
```


## Shells

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
