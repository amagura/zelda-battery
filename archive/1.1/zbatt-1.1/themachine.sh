#!/usr/bin/env bash
set -x
cpp -nostdinc -P -o torture torture.sh
bash -x torture
exit
