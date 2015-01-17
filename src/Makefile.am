## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zbatc zbatt
zbatc_SOURCES = main-color.c power.c main.c acpi.c
zbatt_SOURCES = main-text.c power.c main.c acpi.c

AM_CFLAGS = -O2 -Wall

if HAVE_sys_class_power_supply
AM_CFLAGS += -DZB_SYS_CLASS=1
endif

if DEBUG
AM_CFLAGS += -g -ggdb -DZB_DEBUG=1
endif

if LINUX
AM_CFLAGS += -DZB_LINUX=1
endif

if BSD
AM_CFLAGS += -DZB_BSD=1
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

.PHONY: valgrind lint
all-local:

move-local: $(bin_PROGRAMS)
	for file in $?; do mv "$$file" ../; done

clean-local: tidy-local

lint:
	for f in $(zbatt_SOURCES) $(zbatc_SOURCES); do cppcheck --enable=all $$f; echo; done

valgrind: $(bin_PROGRAMS)
	valgrind --leak-check=full ./zbatc -c '0'; echo ---------
	valgrind --leak-check=full ./zbatt -f '+' -e '-'

mtrace: $(bin_PROGRAMS)
	for f in $^; do MALLOC_TRACE=./mtrace.txt ./$$f; mtrace ./$$f ./mtrace.txt; echo; cat mtrace.txt; echo; echo; done

expand:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -nostdinc -E $(zbatt_SOURCES)
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -nostdinc -E $(zbatc_SOURCES)

check-local:
	csi -qn test.scm

asm: $(zbatt_SOURCES:.c=.s) $(zbatc_SOURCES:.c=.s) $(bin_PROGRAMS)
	for f in $(zbatt_SOURCES); do objdump --file-start-context -slS "$$f" > "$${f//.*/.od}"; done
	for f in $(zbatc_SOURCES); do objdump --file-start-context -slS "$$f" > "$${f//.*/.od}"; done


.c.s:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -S $<

test-local:
	$(CC) $(AM_CFLAGS) -L~/code/libacpi/hg $(AM_CPPFLAGS) $(AM_LDFLAGS) -lacpi -o zbatt $(zbatt_SOURCES)

tidy-local:
	@$(RM) *.o || true
	$(RM) *.s || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null

drone:
	cd /sys/class/power_supply; ls -R; ls -R /sys/class/power_supply
