## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zba zbl
zba_SOURCES = zba.c zbb.h
zbl_SOURCES = zbl.c zbb.h
zbl_CPPFLAGS =

AM_CFLAGS = -O2 -Wall
AM_LDFLAGS = -lm -lacpi

if NATIVE
AM_CFLAGS += -march=native -mtune=native
endif

if GENERIC
if CPU_KNOWN
AM_CFLAGS += -mtune=generic-$(CPUTYPE)
else
AM_CFLAGS += -mtune=generic
endif
endif

if ACBLINK
zbl_CPPFLAGS += -D_ZB_ACBLINK
endif

if !BLINK
zbl_CPPFLAGS += -D_ZB_NOBLINK
endif
