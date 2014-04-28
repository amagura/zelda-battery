## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zelda-blink zelda-battery
zelda_battery_SOURCES = zelda-battery.scm cppToScheme.c
zelda_battery_CPPFLAGS = -DZELDA_BATTERY_SED=\"$(SED)\" -DZELDA_BATTERY_GREP=\"$(GREP)\"
zelda_blink_SOURCES = zelda-blink.scm cppToScheme.c
zelda_blink_CPPFLAGS = -DZELDA_BATTERY_SED=\"$(SED)\" -DZELDA_BATTERY_GREP=\"$(GREP)\"
AM_CFLAGS = -Wall -g -ggdb -O2 -march=native -mtune=native

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
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_PMSET
zelda_battery_CPPFLAGS += -DZELDA_BATTER_PMSET
endif

if ACPI
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_ACPI
zelda_battery_CPPFLAGS += -DZELDA_BATTERY_ACPI
endif

LINK=$(CSC) $(CSCFLAGS) -o $@ # specify how to link object files

.scm.o:
	$(CSC) $(CSCFLAGS) -c $< -o $@

clean-local: tidy-local

tidy-local:
	@$(RM) *.o
	$(RM) $(bin_PROGRAMS:=.c)

anew: clean all
ready: clean all tidy-local
