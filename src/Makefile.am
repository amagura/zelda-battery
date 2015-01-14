## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zbatc zbatt
zbatc_SOURCES = main-color.c power.c
zbatt_SOURCES = main-text.c power.c

AM_CFLAGS = -O2 -Wall -g -ggdb

if LINUX
AM_CFLAGS += -DZB_LINUX=1
endif

if BSD
AM_CFLAGS += -DZB_BSD=1
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

.PHONY: valgrind lint
all-local:

move-local: $(bin_PROGRAMS)
	for file in $?; do mv $$file ../; done

clean-local: tidy-local

asm: zbatt.s zbatc.s

lint:
	for f in $(zbatt_SOURCES) $(zbatc_SOURCES); do cppcheck --enable=all $$f; echo; done

valgrind: $(bin_PROGRAMS)
	valgrind --leak-check=full ./zbatc -c '0'; echo ---------
	valgrind --leak-check=full ./zbatt -f '+' -e '-'

expand:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -nostdinc -E $(zbatt_SOURCES)
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -nostdinc -E $(zbatc_SOURCES)

check-local:
	csi -qn test.scm
.c.s:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -S $<

tidy-local:
	@$(RM) *.o || true
	$(RM) *.s || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null
