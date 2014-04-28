## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zelda-blink zelda-battery
zelda_battery_SOURCES = zelda-battery.scm cppToScheme.c
zelda_blink_SOURCES = zelda-blink.scm cppToScheme.c
AM_CFLAGS = -Wall -g -ggdb -O2 -march=native -mtune=native
zelda_blink_CPPFLAGS =
zelda_battery_CPPFLAGS =

CSCFLAGS ?= -vnq -O1
CSCFLAGS += $(CHICKEN_FLAGS)

if PROFILING
CSCFLAGS += -accumulate-profile -profile-name runtime-profile.out
endif

if DEBUG
CSCFLAGS += -S
endif

if DARWIN
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_DARWIN
zelda_battery_CPPFLAGS += -DZELDA_BATTERY_DARWIN
endif
if LINUX
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_LINUX
zelda_battery_CPPFLAGS += -DZELDA_BATTERY_LINUX
endif

if PMSET
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"pmset\"
zelda_battery_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"pmset\"
endif
if ACPI
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"acpi\"
zelda_battery_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"acpi\"
endif

LINK=$(CC) $$(csc -cflags) $$(csc -ldflags) $$(csc -libs) -o $@ # specify how to link object files

.scm.o:
	$(CSC) $(CSCFLAGS) -c $< -o $@

clean-local: tidy-local

expand-local:
	for files in $(zelda_battery_SOURCES); do \
	  csc -t $$files; \
	  done
	for files in $(zelda_blink_SOURCES); do \
	  csc -t $$files; \
	  done

tidy-local:
	@$(RM) *.o
	$(RM) $(bin_PROGRAMS:=.c)

anew: clean all
ready: clean all tidy-local
