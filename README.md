zelda-battery [![Build Status](https://travis-ci.org/amagura/zelda-battery.svg?branch=master)](https://travis-ci.org/amagura/zelda-battery)
=============

A Legend of Zelda like health-bar battery meter for the command-line

# Supported Platforms

* Mac OS X (I don't have access to a Mac, so no support yet)
* Linux
* FreeBSD


# Building
1. `autoreconf -i`
2. `./configure`
3. `make`

# Installing
Just place `zbatc` (zbatt-color) and `zbatt` (zbatt-text) where ever you want them and add the appropriate stuff to your terminal prompt.

## Zsh
```bash
RPROMPT="%{$(./zbatc)%}$(./zbatt)%{\033[0;0m%}"

## run TRAPALRM every $TMOUT seconds
TMOUT=60 # refresh the terminal prompt every 60 seconds

TRAPALRM () {
    zle reset-prompt # refreshs the terminal prompt
}
```
![example showing zelda-battery in a Zsh prompt](/example/zsh.jpg)


## Bash
```bash
PS1='\[$(./zbatc)\]$(./zbatt)\[\033[0;0m\] [\h \W]\$ '
```
![example showing zelda-battery in a Bash prompt](/example/bash.jpg)

## Mksh (Korn Shell)
```ksh
PS1=$'$(./zbatc)'$(./zbatt)$'\033[0;0m'\ ["$(hostname)"]\
```
![example showing zelda-battery in a Mksh prompt](/example/mksh.jpg)

## Tcsh
```csh
set prompt="%{`./zbatc`%}`./zbatt` %{\033[0;0m%}%m "
```
![example showing zbatt in a tcsh prompt](/example/tcsh.jpg)

# Dependencies
* C compiler
* `libacpi` (Linux)

# Notes

## Zsh
As long as you use the `TMOUT` and `TRAPALRM`/`zle reset-prompt` stuff, your Zsh prompt should refresh itself every 60 seconds.

## Bash
AFAIK there is _no_ way to periodically refresh a bash prompt without either running `clear`/`Ctrl-L` or pressing enter, which causes the prompt to be redrawn.
