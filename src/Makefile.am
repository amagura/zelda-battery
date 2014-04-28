## Process this file with automake to produce Makefile.in

SUBDIRS = scm c py

bin_PROGRAMS = zelda-blink zelda-battery
zelda_battery_SOURCES = zelda-battery.scm cppToScheme.c
zelda_battery_CPPFLAGS =
zelda_blink_SOURCES = zelda-blink.scm
zelda_blink_CPPFLAGS =
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
zelda_blink_CPPFLAGS += -DADMINS_FRIEND_DARWIN
zelda_battery_CPPFLAGS += -DADMINS_FRIEND_DARWIN
endif
if LINUX
zelda_blink_CPPFLAGS += -DADMINS_FRIEND_LINUX
zelda_battery_CPPFLAGS += -DADMINS_FRIEND_LINUX
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
