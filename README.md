zelda-battery [![Build Status](https://travis-ci.org/amagura/zelda-battery.svg?branch=master)](https://travis-ci.org/amagura/zelda-battery)
=============

A Legend of Zelda like health-bar battery meter for the command-line

## Supported Platforms

* Mac OS X (work in progress)
* Linux
* FreeBSD (work in progress)


## Building
1. `autoreconf -i`
2. `./configure`
3. `make`

__Note__ that `./configure` can take an `--enable-ac-blink` option, which is disabled by default.  Enabling this will cause Zelda Battery to blink on low battery _even_ when your computer is plugged into an AC power supply.

## Installing
Just place `zbl` (zelda-blink) and `zba` (zelda-battery) where ever you want them and add the appropriate stuff to your terminal prompt.

### Zsh
```bash
RPROMPT="%{$(./zbl)%}$(./zba)%{\033[0;0m%}"

## run TRAPALRM every $TMOUT seconds
TMOUT=60 # refresh the terminal prompt every 60 seconds

TRAPALRM () {
    zle reset-prompt # refreshs the terminal prompt
}
```
![example showing zelda-battery in a Zsh prompt](/example/zsh.jpg)


### Bash
```bash
PS1='\[$(./zbl)\]$(./zba)\[\033[0;0m\] [\h \W]\$ '
```
![example showing zelda-battery in a Bash prompt](/example/bash.jpg)

### Mksh (Korn Shell)
```ksh
PS1=$'$(./zbl)'$(./zba)$'\033[0;0m'\ ["$(hostname)"\ "$(basename "$(pwd)")"]$(if ((USER_ID)); then print \$; else print \#; fi)\  # trailing space here
```
![example showing zelda-battery in a Mksh prompt](/example/mksh.jpg)

## Dependencies
* C compiler
* `libacpi` (Linux)

## Notes

### Zsh
As long as you use the `TMOUT` and `TRAPALRM`/`zle reset-prompt` stuff, your Zsh prompt should refresh itself every 60 seconds.

### Bash
AFAIK there is _no_ way to periodically refresh a bash prompt without either running `clear`/`Ctrl-L` or pressing enter, which causes the prompt to be redrawn.
