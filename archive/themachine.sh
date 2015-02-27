#!/usr/bin/env bash
set -x
find . -mindepth 1 -maxdepth 1 -type d -execdir mkdir -p {}/bin \;
