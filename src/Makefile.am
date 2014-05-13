## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zelda-blink zelda-battery
zelda_battery_SOURCES = zelda-battery.scm cppToScheme.c helper.scm regex.scm
zelda_blink_SOURCES = zelda-blink.scm cppToScheme.c helper.scm regex.scm
AM_CFLAGS = -Wall -g -ggdb -O2 -march=native -mtune=native
zelda_blink_CPPFLAGS =
zelda_battery_CPPFLAGS =

CSCFLAGS ?= -optimize-leaf-routines
CSCFLAGS += -optimize-leaf-routines $(CHICKEN_FLAGS) -C -O2 -C -march=native -C -mtune=native -C -Wall

if PROFILING
CSCFLAGS += -accumulate-profile -profile-name PROFILE.out
endif

if DEBUG
CSCFLAGS += -S
endif

if PMSET
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"pmset\"
zelda_battery_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"pmset\"
endif

if ACPI
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"acpi\"
zelda_battery_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"acpi\"
endif

if YACPI
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"yacpi\"
zelda_battery_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"yacpi\"
endif

if APM
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"apm\"
zelda_battery_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"apm\"
endif

if ACPICONF
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"acpiconf\"
zelda_battery_CPPFLAGS += -DZELDA_BATTERY_UTIL=\"acpiconf\"
endif

if ACBLINK
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_BLINK_EVEN_ON_AC
endif

if !BLINKING
zelda_blink_CPPFLAGS += -DZELDA_BATTERY_NO_BLINK
endif

LINK=$(CSC) $(CSCFLAGS) -o $@ # specify how to link object files

.scm.o:
	$(CSC) $(CSCFLAGS) -c $< -o $@

clean-local: tidy-local

all-local:

profile-local:
	$(CSC) $(CSCFLAGS) $(CPPFLAGS) -accumulate-profile -profile-name PROFILE-BLINK.001 -S $(zelda_blink_SOURCES)
	$(CSC) $(CSCFLAGS) $(CPPFLAGS) -accumulate-profile -profile-name PROFILE-BATTERY.002 -S $(zelda_battery_SOURCES)
	for i in $$(bash -c 'echo {0..1000}'); do\
	  for prog in $(bin_PROGRAMS); do\
	  ./$$prog > /dev/null; \
	  done; \
	  done
	chicken-profile PROFILE-BLINK.001
	chicken-profile PROFILE-BATTERY.002

expand-local:
	for files in $(zelda_battery_SOURCES); do \
	  csc -t $$files; \
	  done
	for files in $(zelda_blink_SOURCES); do \
	  csc -t $$files; \
	  done

tidy-local:
	@$(RM) *.o || true
	$(RM) $(bin_PROGRAMS:=.c) || true
	$(RM) helper.c || true
	$(RM) PROFILE-* || true

anew: clean all
ready: clean all tidy-local
