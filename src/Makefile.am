## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zb-color zb-text
zb_color_SOURCES = zb.c
zb_text_SOURCES = zb.c
zb_color_CPPFLAGS = -D_ZB_MAKING_ZB_COLOR=1

AM_CFLAGS = -O2 -Wall
AM_LDFLAGS = -lacpi

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

all-local: zb-color zb-text
	for file in $?; do mv $$file ../; done

clean-local: tidy-local

asm-local: zb-color.s zb-text.s

check-local:
	./cmp.sh

zb-color.s:
	$(CC) $(CFLAGS) $(AM_CFLAGS) $(zb_color_CPPFLAGS) $(CPPFLAGS) -S $(zb_color_SOURCES) -o $@

zb-text.s:
	$(CC) $(CFLAGS) $(AM_CFLAGS) $(CPPFLAGS) -S $(zb_text_SOURCES) -o $@

tidy-local:
	@$(RM) *.o || true
	$(RM) *.s || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null
