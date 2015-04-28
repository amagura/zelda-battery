ZBatt [![Build Status](https://travis-ci.org/amagura/zelda-battery.svg?branch=master)](https://travis-ci.org/amagura/zelda-battery)
=============

A Legend of Zelda inspired health-bar battery meter for Shell prompts and Taskbars

# Examples

## GUI
### FreeBSD
![example of gzbatt on FreeBSD](/example/FreeBSD.png)

## Command-line

The command-line is _extremely_ flexible, so while there are defaults
there's nothing stopping you from defining your own experience.
![example of the git style](/example/git.jpg)
```bash
./zbatc -c 32; ./zbatt -r -f +; ./zbatc -c 31; ./zbatt -x -e -
```

To make it even easier to integrate the command-line with your current
environment, I've gone ahead and provided examples for every shell
I could get my hands on.  That said, it is worth noting that some shells
are better equiped for this sort of thing than others.  Under some shells
the CLI will only update every time you run a command: if you're on battery power
and you haven't touched your terminal for the past hour and you think you're still
cool because you just checked and the CLI says you're still at 80% battery--think again!

Zsh, so far, is the only shell I know of that supports updating its prompt
after a given amount of time.  I keep mine set to 60 seconds: not too long, but long
enough so that I rarely have my prompt refresh in the middle of an expansion.

All the same, pick whatever works best for _you_!

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
