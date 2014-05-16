#!/usr/bin/env bash

(src/zelda-blink; src/zelda-battery) || echo 'be sure to run `make` before running the demo'
echo
