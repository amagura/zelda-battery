#!/usr/bin/env bash

str="$(echo $(./zbatt))"
string="$(echo "$str" | sed -r 's/(.)/\1 /g; s/\s$//')"

full=0
empty=0
for char in $string; do
  [[ "$char" =~ "♥" ]] && ((++full))
  [[ "$char" =~ "♡" ]] && ((++empty))
done
total=$((full + empty))

echo "full: $full"
echo "empty: $empty"
echo "total: $total"

if [[ $total > 10 ]]; then
  printf "\e[0;31m%s\e[0;0m\n" "not ok"
  exit 1
else
  printf "\e[0;32m%s\e[0;0m\n" "ok"
fi
