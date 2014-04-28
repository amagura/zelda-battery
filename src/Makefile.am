## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zelda-blink zelda-battery
zelda_battery_SOURCES = zelda-battery.scm cppToScheme.c
zelda_blink_SOURCES = zelda-blink.scm cppToScheme.c
AM_CFLAGS = -Wall -g -ggdb -O2 -march=native -mtune=native
AM_CPPFLAGS =

CSCFLAGS ?= -vnq -O1
CSCFLAGS += $(CHICKEN_FLAGS) -C $(CFLAGS) -C $(CPPFLAGS) $(AM_CPPFLAGS)

if PROFILING
CSCFLAGS += -accumulate-profile -profile-name runtime-profile.out
endif

if DEBUG
CSCFLAGS += -S
endif

if DARWIN
AM_CPPFLAGS += -DZELDA_BATTERY_DARWIN
endif
if LINUX
AM_CPPFLAGS += -DZELDA_BATTERY_LINUX
endif

if PMSET
AM_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"pmset\"
endif
if ACPI
AM_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"acpi\"
endif

LINK=$(CSC) $(CSCFLAGS) -o $@ # specify how to link object files

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
