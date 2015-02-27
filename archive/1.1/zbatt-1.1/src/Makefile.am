## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zbatc zbatt
zbatc_SOURCES = main-color.c power.c
zbatt_SOURCES = main-text.c power.c

noinst_HEADERS = power.h main.h

AM_CFLAGS = -O2 -Wall

if LINUX
AM_CFLAGS += -D_ZB_UNIX_LINUX=1
endif

if BSD
AM_CFLAGS += -D_ZB_UNIX_BSD=1
endif

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

all-local:

move-local: $(bin_PROGRAMS)
	for file in $?; do mv $$file ../; done

clean-local: tidy-local

asm-local: zbatt.s zbatc.s

check-local:
	./cmp.sh
.c.s:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -S $<

tidy-local:
	@$(RM) *.o || true
	$(RM) *.s || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null
