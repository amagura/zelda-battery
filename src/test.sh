#!/usr/bin/env bash
set -x

echo "zbatt outputs 10 chars"
./zbatt -f + -e - | wc -L | grep 10 -q || exit
