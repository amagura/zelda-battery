## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zbatc zbatt
zbatc_SOURCES = main-color.c main.c acpi.c power.c
zbatt_SOURCES = main-text.c main.c acpi.c power.c
gzbatt_SOURCES = gzbatt.pyx main.c acpi.c power.c
noinst_HEADERS = main.h acpi.h power.h compat.h imgs.h

AM_CFLAGS =
gzbatt_CFLAGS =
gzbatt_LDFLAGS =

if GTK2
gzbatt_CFLAGS += @gtk2_CFLAGS@ @py2_CFLAGS@
gzbatt_LDFLAGS += @gtk2_LIBS@ @py2_LIBS@
bin_PROGRAMS += gzbatt
endif

if DEBUG
AM_CFLAGS += -g -ggdb -DZB_DEBUG=1
endif

if NATIVE
if !GENERIC
AM_CFLAGS += -march=native -mtune=native
endif
endif

if GENERIC
if CPU_KNOWN
AM_CFLAGS += -mtune=generic-$(CPUTYPE)
else
AM_CFLAGS += -mtune=generic
endif
endif

zbatc_CFLAGS = $(AM_CFLAGS) -DZB_PROGNAME=\"zbatc\"
zbatt_CFLAGS = $(AM_CFLAGS) -DZB_PROGNAME=\"zbatt\"
gzbatt_CFLAGS += $(AM_CFLAGS) -DZB_PROGNAME=\"gzbatt\"

.PHONY: valgrind lint
all-local:

move-local: $(bin_PROGRAMS)
	for file in $?; do mv "$$file" ../../; done

clean-local: tidy-local

lint:
	for f in $(zbatt_SOURCES) $(zbatc_SOURCES); do cppcheck --enable=all $$f; echo; done

valgrind: $(bin_PROGRAMS)
	valgrind --leak-check=full -v --show-leak-kinds=all --track-origins=yes ./zbatc -c '0';
	valgrind --leak-check=full -v --show-leak-kinds=all --track-origins=yes ./zbatt -f '+' -e '-'; echo ---------

mtrace: $(bin_PROGRAMS)
	for f in $^; do MALLOC_TRACE=./mtrace.txt ./$$f; mtrace ./$$f ./mtrace.txt; echo; cat mtrace.txt; echo; echo; done

expand:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -E $(zbatt_SOURCES)
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -E $(zbatc_SOURCES)

check-local: $(bin_PROGRAMS)
	csi -qn test.scm

asm: $(zbatt_SOURCES:.c=.s) $(zbatc_SOURCES:.c=.s) $(bin_PROGRAMS)
	for f in $(zbatt_SOURCES:.c=.o); do objdump --file-start-context -slS "$$f" > "$${f//.*/.od}"; done
	for f in $(zbatc_SOURCES:.c=.o); do objdump --file-start-context -slS "$$f" > "$${f//.*/.od}"; done

.c.s:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -S $<

.pyx.c:
	$(CYTHON) --embed -p -2 -Wextra -o $@ $<

tidy-local:
	$(RM) *.o || true
	$(RM) *.s || true
	$(RM) *.od || true
	$(RM) test || true
	$(RM) gzbatt || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null

drone:
	cd /sys/class/power_supply; ls -R; ls -R /sys/class/power_supply

akpi:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) $(AM_LDFLAGS) -o acpi acpi.c main.c
