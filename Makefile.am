## Process this file with automake to produce Makefile.in

SUBDIRS = src
AM_CFLAGS = -Wall -g -ggdb -O2 -march=native -mtune=native

clean-local: tidy-local

tidy-local:
	@$(RM) *.o

anew: clean all
ready: clean all tidy-local
