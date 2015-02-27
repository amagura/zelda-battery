#!/usr/bin/env bash
set -x

./configure --disable-native --disable-generic

cd src
make clean

for obj in power.o main-text.o main-color.o; do
    make "$obj" CFLAGS=--static
done

for bin in zbatt zbatc; do
    make "$bin" CFLAGS=--static CFLAGS+='obj/*.o'
done
