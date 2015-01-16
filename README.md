zelda-battery [![Build Status](https://drone.io/github.com/amagura/zelda-battery/status.png)](https://drone.io/github.com/amagura/zelda-battery/latest)
=============

A Legend of Zelda like health-bar battery meter for the command-line

# Why?
Because I wanted to be able to keep a constant tab on my laptop's power-level without having to look away from the command-line, thought it would be neat thing to do, and because I _could_.

# Supported Platforms

* Mac OS X (I don't have a Mac, sorry. No support yet)
* Linux
* FreeBSD

### Dependencies
* GNU Autotools
* C Compiler
* `libacpi` (Linux)

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
