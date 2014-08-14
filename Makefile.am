## Process this file with automake to produce Makefile.in

bin_PROGRAMS = zba zbl
zba_SOURCES = zba.py
zbl_SOURCES = zbl.py

AM_CFLAGS = $$(pkg-config --cflags python3) -O3
AM_LDFLAGS = $$(pkg-config --libs python3)

CYTHON_FLAGS = $$CYTHON_FLAGS --embed -3


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

all-local: zba.pyo zbl.pyo build-local link-local

build-local: zba.c zbl.c

link-local: zba zbl	

.py.pyo:
	@echo
	cat zbb.py $< > $@

.pyo.c:
	$(CYTHON) $(CYTHON_FLAGS) $< -o $@

clean-local: tidy-local

tidy-local:
	@$(RM) *.o || true
	$(RM) *.c || true
	$(RM) *.pyo || true
	$(RM) *.html || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null
