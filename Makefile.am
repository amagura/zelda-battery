## Process this file with automake to produce Makefile.in

SUBDIRS = src

AM_CFLAGS = -Wall -g -ggdb -O2

CSCFLAGS = $$CSC_OPTIONS -optimize-leaf-routines

if NATIVE
AM_CFLAGS += -march=native -mtune=native
CSCFLAGS += -C -march=native -C -mtune=native
endif

if GENERIC
AM_CFLAGS += -march=generic -mtune=generic
CSCFLAGS += -C -march=generic -C -mtune=generic
endif

clean-local: tidy-local

tidy-local:
	@$(RM) *.o || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null
