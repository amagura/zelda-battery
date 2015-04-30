## Process this file with automake to produce Makefile.in

dist_doc_DATA = LICENSE AUTHORS INSTALL

if WINDOWS_HOST
SUBDIRS = img win src
else
SUBDIRS = img nix src
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

test-clean: test-setup
	echo 1 > 'test/ACAD/online'
	echo 100 > 'test/BAT0/capacity'
	echo 100 > 'test/BAT1/capactiy'
	echo 'Battery' > 'test/BAT0/type'
	echo 'Battery' > 'test/BAT1/type'
	echo 'Mains' > 'test/ACAD/type'

test-onac: test-setup
	echo 1 > 'test/ACAD/online'
	$(RM) -r 'test/'{BAT0,BAT1} || $(ERM) -r 'test/'{BAT0,BAT1} || true

test-noac: test-setup
	echo 0 > 'test/ACAD/online'

test-lowbat: test-setup
	echo 10 > 'test/BAT0/capacity'
	echo 10 > 'test/BAT1/capacity'

test-empty: test-setup
	echo 0 > 'test/BAT0/capacity'
	echo 0 > 'test/BAT0/capacity'

test-nosup:
	rm -rf 'test/'

test-depth: test-nosup
	mkdir -p "test"; \
	cd "test"; \
	for ((idx = 0; idx < 256; ++idx)); do \
		mkdir -p "BAT$$idx"; \
		cd "BAT$$idx"; \
	done; \
	echo 50 > "capacity"; \
	echo 'Battery' > "type"

test-breadth: test-nosup test-setup
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
