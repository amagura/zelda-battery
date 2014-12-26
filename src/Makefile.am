## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zbatc zbatt
zbatc_SOURCES = main.c color.c
zbatc_CPPFLAGS = -D_ZB_MAKING_ZB_COLOR=1

AM_CFLAGS = -O2 -Wall
AM_CPPFLAGS = $(zbatc_CPPFLAGS)

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

all-local: zbatc zbatt
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
