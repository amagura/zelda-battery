#!/usr/bin/env bash
export CFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions --param=ssp-buffer-size=4"
export CXXFLAGS="$CFLAGS"
./configure --host=i686-w64-mingw32 --prefix=/usr/i686-w64-mingw32 --enable-static "$@"
