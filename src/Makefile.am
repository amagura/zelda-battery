## Process this file with automake to produce Makefile.in

SUBDIRS = scm c py

bin_PROGRAMS = dog madrid
madrid_SOURCES = madrid.scm fdHelper.c lnHelper.c cppToScheme.c file-helper.scm option-helper.scm
madrid_CPPFLAGS = -DADMINS_FRIEND_COMPILING_MADRID
dog_SOURCES = dog.scm
dog_CPPFLAGS = -DADMINS_FRIEND_COMPILING_DOG
AM_CFLAGS = -Wall -g -ggdb -O2 -std=c11 -march=native -mtune=native

CSCFLAGS ?= -vnq -O1
CSCFLAGS += $(CHICKEN_FLAGS)
if PROFILING
CSCFLAGS += -accumulate-profile -profile-name runtime-profile.out
endif
if DEBUG
CSCFLAGS += -S
endif

if DARWIN
dog_CPPFLAGS += -DADMINS_FRIEND_DARWIN
madrid_CPPFLAGS += -DADMINS_FRIEND_DARWIN
endif
if LINUX
dog_CPPFLAGS += -DADMINS_FRIEND_LINUX
madrid_CPPFLAGS += -DADMINS_FRIEND_LINUX
endif

LINK=$(CSC) $(CSCFLAGS) -o $@ # specify how to link object files

.scm.o:
	$(CSC) $(CSCFLAGS) -c $< -o $@

.py..:
	$(PYC)


clean-local: tidy-local

tidy-local:
	@$(RM) *.o
	$(RM) $(bin_PROGRAMS:=.c)

#expand-local: expand-madrid-local expand-dog-local

#expand-madrid-local:
#	for file in $(madrid_SOURCES); do \
#	  $(CC) $(CFLAGS) -E $$file; \
#	  done

#expand-dog-local:
#	for file in $(dog_SOURCES); do \
#	  $(CC) $(CFLAGS) -E $$file; \
#	  done

anew: clean all
ready: clean all tidy-local
