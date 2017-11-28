#!/usr/bin/env bash
export CFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions --param=ssp-buffer-size=4 --static"
export CXXFLAGS="$CFLAGS"
export HOST=x86_64-w64-mingw32
export CC=$HOST-cc
export CXX=$HOST-c++

{ autoconf -i || autoreconf -fi; } \
  && ./configure \
  --prefix=/usr/$HOST \
  --host=$HOST \
  --includedir=/usr/$HOST/include \
  --without-x \
  CFLAGS="$CFLAGS" \
  CXXFLAGS="$CXXFLAGS" \
  "$@"
