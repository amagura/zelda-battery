## Process this file with automake to produce Makefile.in

SUBDIRS = src
AM_CFLAGS = -std=c11 -g -gdb -O2 -Wall

clean-local: tidy-local

tidy-local:
	@$(RM) *.o

anew: clean all
ready: clean all tidy-local
