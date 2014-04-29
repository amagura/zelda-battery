These are _working_ examples.

### zsh
```bash
RPROMPT="%{$(../src/zelda-blink)%}$(../src/zelda-battery)%{[0;0m%}"

## run TRAPALRM every $TMOUT seconds
TMOUT=60 # refresh the terminal prompt every 60 seconds

TRAPALRM () {
  zle reset-prompt # refreshs the terminal prompt
}
```
