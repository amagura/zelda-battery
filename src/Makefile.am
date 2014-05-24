## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zelda-blink zelda-battery
zelda_battery_LIBS = helper.scm regex.scm list.scm cast.scm io.scm power.scm test.scm
zelda_blink_LIBS = helper.scm regex.scm list.scm cast.scm io.scm power.scm test.scm
zelda_battery_SOURCES = zelda-battery.scm cppToScheme.c $(zelda_battery_LIBS)
zelda_blink_SOURCES = zelda-blink.scm cppToScheme.c $(zelda_blink_LIBS)

AM_CFLAGS = -Wall -g -ggdb -O2
zelda_blink_CPPFLAGS =
zelda_battery_CPPFLAGS =

CSCFLAGS = $$CSC_OPTIONS -optimize-leaf-routines $(CHICKEN_FLAGS) -C -O2

ECHO_VARIABLES = $$enable_generic $$enable_GENERIC $$enable_native $$enable_NATIVE $$CPUTYPE

if NATIVE
ECHO_VARIABLES += $(NATIVE)
AM_CFLAGS += -march=native -mtune=native
CSCFLAGS += -C -march=native -C -mtune=native
endif

if GENERIC
ECHO_VARIABLES += $(GENERIC)
if CPU_KNOWN
AM_CFLAGS += -mtune=generic-$(CPUTYPE)
CSCFLAGS += -C -mtune=generic-$(CPUTYPE)
else
AM_CFLAGS += -mtune=generic
CSCFLAGS += -C -mtune=generic
endif
endif

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

# End of Variables

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
	for file in $(zelda_battery_LIBS:.scm=.c) $(zelda_blink_LIBS:.scm=.c); do \
	  $(RM) "$$file" || true; \
	  done
	$(RM) PROFILE-* || true

echo-local:
	for each in $(ECHO_VARIABLES); do \
	  echo $$each; \
	  done

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null
