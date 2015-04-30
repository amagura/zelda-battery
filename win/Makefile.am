## Process this file with automake to produce Makefile.in

_srcdir_ = $(top_srcdir)/win

bin_PROGRAMS = zbatt zbatc gzbatt

zbatc_SOURCES = zbatc.cxx
zbatt_SOURCES = zbatt.cxx
gzbatt_SOURCES = gzbatt.cxx
noinst_HEADERS = main.hxx

iconsdir = $(pkgdatadir)/icons
scaledir = $(iconsdir)/scalable/apps
pngdir = $(iconsdir)/16x16/apps

AM_CXXFLAGS =
AM_LDFLAGS =
AM_CPPFLAGS =

AM_CXXFLAGS += -Wall -Wextra

if TEST
AM_CPPFLAGS += -DZB_ACPI_ROOT=\"$(top_srcdir)/test\"
endif

if DEBUG
AM_CXXFLAGS += -g -ggdb
AM_CPPFLAGS += -DZB_DEBUG=1
endif

if !DEBUG
if NO_DEBUG
AM_CPPFLAGS += -DZB_DEBUG=0
endif
endif

if NATIVE
if !GENERIC
AM_CXXFLAGS += -march=native -mtune=native
endif
endif

if GENERIC
if CPU_KNOWN
AM_CXXFLAGS += -mtune=generic-$(CPUTYPE)
else
AM_CXXFLAGS += -mtune=generic
endif
endif

zbatt_CXXFLAGS = $(AM_CXXFLAGS)
zbatc_CXXFLAGS = $(AM_CXXFLAGS)

if STATIC
zbatt_CXXFLAGS += --static
zbatc_CXXFLAGS += --static
endif

zbatc_CPPFLAGS = -DZB_PROGNAME=\"zbatc\" $(AM_CPPFLAGS)
zbatt_CPPFLAGS = -DZB_PROGNAME=\"zbatt\" $(AM_CPPFLAGS)
gzbatt_CXXFLAGS = $(AM_CXXFLAGS)
gzbatt_CPPFLAGS = -DZB_PROGNAME=\"gzbatt\" $(AM_CPPFLAGS)
gzbatt_LDFLAGS = $(AM_LDFLAGS)

.PHONY: valgrind lint
all-local:

move-local: $(bin_PROGRAMS)
	for file in $?; do mv "$$file" ../../; done

clean-local: tidy-local

lint:
	for f in $(zbatt_SOURCES) $(zbatc_SOURCES); do cppcheck --enable=all $$f; echo; done

valgrind: clean $(bin_PROGRAMS)
	for f in $(bin_PROGRAMS); do \
		$@ --leak-check=full -v --show-leak-kinds=all --track-origins=yes ./"$$f"; \
		echo; \
		echo ---------; \
		echo; \
	done

mtrace: $(bin_PROGRAMS)
	for f in $^; do MALLOC_TRACE=./mtrace.txt ./$$f; mtrace ./$$f ./mtrace.txt; echo; cat mtrace.txt; echo; echo; done

expand:
	$(CC) $(AM_CXXFLAGS) $(AM_CPPFLAGS) -E $(zbatt_SOURCES)
	$(CC) $(AM_CXXFLAGS) $(AM_CPPFLAGS) -E $(zbatc_SOURCES)

check-local: $(bin_PROGRAMS)
	csi -qn test.scm

asm: $(zbatt_SOURCES:.c=.s) $(zbatc_SOURCES:.c=.s) $(bin_PROGRAMS)
	for f in $(zbatt_SOURCES:.c=.o); do objdump --file-start-context -slS "$$f" > "$${f//.*/.od}"; done
	for f in $(zbatc_SOURCES:.c=.o); do objdump --file-start-context -slS "$$f" > "$${f//.*/.od}"; done

.c.s:
	$(CC) $(AM_CXXFLAGS) $(AM_CPPFLAGS) -S $<

.pyx.c:
	$(CYTHON) --embed -p -2 -Wextra -o $@ $<

tidy-local:
	$(RM) *.o || true
	$(RM) *.od || true
	$(RM) *.py || true
	$(RM) *.pyc || true
	$(RM) test || true
	$(RM) main_gui.c || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null

drone:
	cd /sys/class/power_supply; ls -R; ls -R /sys/class/power_supply

akpi:
	$(CC) $(AM_CXXFLAGS) $(AM_CPPFLAGS) $(AM_LDFLAGS) -o acpi acpi.c main.c

test-clean:
	cd "$(top_srcdir)"; $(MAKE) $@

test-onac:
	cd "$(top_srcdir)"; $(MAKE) $@

test-noac:
	cd "$(top_srcdir)"; $(MAKE) $@

test-lowbat:
	cd "$(top_srcdir)"; $(MAKE) $@

test-empty:
	cd "$(top_srcdir)"; $(MAKE) $@

test-nosup:
	cd "$(top_srcdir)"; $(MAKE) $@
