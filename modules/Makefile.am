## Process this file with automake to generate Makefile.in

common = common.h common.c mac.h fun.h compat.h

all-local: $(common)

$(common):
	for e in $@; do \
	cp common/c/"$$e" .; \
	done

clean-local:
	$(RM) $(common) || true
