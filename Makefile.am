## Process this file with automake to produce Makefile.in

dist_doc_DATA = LICENSE AUTHORS INSTALL

dist_pkgdata_DATA = example.conf

UNIX_DIR = src
MSDOS_DIR = win

if WINDOWS_HOST
SUBDIRS = img win
else
SUBDIRS = img $(UNIX_DIR)
endif

DIST_SUBDIRS = $(SUBDIRS)

ZB_TARBALL = $$(uname)-$$(uname -m).tar.gz

all-local:

anew: clean all

test-setup:
	mkdir -p test/{ACAD,BAT0,BAT1}
	touch test/ACAD/online
	touch test/BAT0/capacity
	touch test/BAT1/capacity
	touch test/BAT0/type
	touch test/BAT1/type
	touch test/ACAD/type
	echo 1 > 'test/ACAD/online'
	echo 100 > 'test/BAT0/capacity'
	echo 100 > 'test/BAT1/capactiy'
	echo 'Battery' > 'test/BAT0/type'
	echo 'Battery' > 'test/BAT1/type'
	echo 'Mains' > 'test/ACAD/type'

test-clean:
	$(RM) -r test

test-onac: test-clean test-setup
	echo 1 > 'test/ACAD/online'
	$(RM) -r 'test/'{BAT0,BAT1} || $(ERM) -r 'test/'{BAT0,BAT1} || true

test-noac: test-clean test-setup
	echo 0 > 'test/ACAD/online'

test-lowbat: test-clean test-setup
	echo 10 > 'test/BAT0/capacity'
	echo 10 > 'test/BAT1/capacity'

test-empty: test-setup
	echo 0 > 'test/BAT0/capacity'
	echo 0 > 'test/BAT0/capacity'

test-nosup:
	$(RM) -r test

test-depth: test-clean
	mkdir -p "test"; \
	cd "test"; \
	for ((idx = 0; idx < 256; ++idx)); do \
		mkdir -p "BAT$$idx"; \
		cd "BAT$$idx"; \
	done; \
	echo 50 > "capacity"; \
	echo 'Battery' > "type"

test-breadth: test-clean test-setup
	cd "test"; \
	for ((idx = 0; idx < 256; ++idx)); do \
		mkdir -p "BAT$$idx"; \
		echo 50 > "BAT$$idx/capacity"; \
		echo 'Battery' > "BAT$$idx/type"; \
	done

clean-local:
	$(RM) -r inst/usr

release: clean
	$(MAKE) DESTDIR="$$PWD/inst" install
	cd inst; \
	$(RM) $(ZB_TARBALL) ;\
	tar czf $(ZB_TARBALL) usr

refresh-check:
	cd $(UNIX_DIR); \
	$(MAKE) $@

check-all:
	cd $(UNIX_DIR); \
	$(MAKE) $@

install-blank:
	printf '#include <stdlib.h>\nint main(int argc, char **argv) { return EXIT_SUCCESS; }' > blank.c
	$(CC) -o src/zbatt blank.c
	$(CC) -o src/zbatc blank.c
	cd $(UNIX_DIR); \
	  $(MAKE) install
