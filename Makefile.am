## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zba zbl
zba_SOURCES = zba.c
zbl_SOURCES = zbl.c

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

if !BLINKING
zbl_CPPFLAGS += -D_ZB_NOBLINK
endif

all-local: zba.co zbl.co

.c.co:
	@echo
	cat src/zbb.c src/$< > $@
	cat $@ > $<
	$(RM) $@

clean-local: tidy-local

tidy-local:
	@$(RM) *.o || true
	$(RM) *.co || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null
