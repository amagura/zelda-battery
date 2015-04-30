#!/usr/bin/env bash
export CFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions --param=ssp-buffer-size=4"
export CXXFLAGS="$CFLAGS"
export HOST=i686-w64-mingw32
export CC=$HOST-cc
export CXX=$HOST-c++

(autoconf -i || autoreconf -fi) && \
    ./configure --prefix=/usr/$HOST \
		--host=$HOST \
		--enable-static \
		--includedir=/usr/$HOST/include \
		"$@"
