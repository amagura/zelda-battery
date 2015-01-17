ZBatt [![Build Status](https://drone.io/github.com/amagura/zelda-battery/status.png)](https://drone.io/github.com/amagura/zelda-battery/latest)
=============

A Legend of Zelda like health-bar battery meter for Shell prompts.

# Disclaimer
Before you _even_ comment on the code or practices used by this project, keep on _very_ important thing in mind: I wrote this for _me_ to use, but I was nice and made it easy for other people to use in _neat_ ways.

If you see something weird like `goto` statements instead of for-loops remember where this application is being used.  It is for use in shell prompts, where it needs to be _lightning_ fast: because I don't want my Shell prompt to take any noticable amount of time to load, and because (admittedly) it was fun to find ways of making ZBatt unnecesssarily faster.

# Why?
Because I wanted to be able to keep a constant tab on my laptop's power-level without having to look away from the command-line or running a command, thought it would be neat thing to do, and because I _could_.

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
