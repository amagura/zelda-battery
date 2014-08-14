## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zbat
zbat_SOURCES = zbat.py

AM_CFLAGS = $$(pkg-config --cflags python2)
AM_LDFLAGS = $$(pkg-config --libs python2)

CYTHON_FLAGS = $$CYTHON_FLAGS --embed -2


if NATIVE
AM_CFLAGS += -march=native -mtune=native
endif

if GENERIC
if CPU_KNOWN
AM_CFLAGS += -mtune=generic-$(CPUTYPE)
else
AM_CFLAGS += -mtune=generic
endif
endif

build-local: zbat.c

link-local: zbat

.py.c:
	$(CYTHON) $(CYTHON_FLAGS) $< -o $@

clean-local: tidy-local

tidy-local:
	@$(RM) *.o || true
	$(RM) *.c || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null
