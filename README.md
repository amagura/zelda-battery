zelda-battery [![Build Status](https://travis-ci.org/dopm/zelda-battery.svg?branch=master)](https://travis-ci.org/dopm/zelda-battery)
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
Just place `zelda-blink` and `zelda-battery` where ever you want them and add the appropriate stuff to your terminal prompt.

### Zsh
```bash
RPROMPT="%{$(src/zelda-blink)%}$(src/zelda-battery)%{\033[0;0m%}"

## run TRAPALRM every $TMOUT seconds
TMOUT=60 # refresh the terminal prompt every 60 seconds

TRAPALRM () {
    zle reset-prompt # refreshs the terminal prompt
}
```
![example showing zelda-battery in a Zsh prompt](/example/zsh.jpg)


### Bash
```bash
PS1='\[$(src/zelda-blink)\]$(src/zelda-battery)\[\033[0;0m\] [\h \W]\$ '
```
![example showing zelda-battery in a Bash prompt](/example/bash.jpg)

## Dependencies
* C compiler
* `libacpi` (Linux)

## Notes

### Zsh
As long as you use the `TMOUT` and `TRAPALRM`/`zle reset-prompt` stuff, your Zsh prompt should refresh itself every 60 seconds.

### Bash
AFAIK there is _no_ way to periodically refresh a bash prompt without either running `clear`/`Ctrl-L` or pressing enter, which causes the prompt to be redrawn.
