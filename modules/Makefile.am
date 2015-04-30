## Process this file with automake to generate Makefile.in

common_local = common.h common.c mac.h fun.h compat.h

all-local: $(common_local)

$(common_local):
	for e in $@; do \
	cp common/c/"$$e" .; \
	done

clean-local:
	$(RM) $(common_local) || true
