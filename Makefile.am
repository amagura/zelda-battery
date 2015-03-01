## Process this file with automake to produce Makefile.in

dist_doc_DATA = LICENSE AUTHORS

all-local:

test-clean:
	mkdir -p test/{ACAD,BAT0,BAT1}
	echo 1 > 'test/ACAD/online'
	echo 100 > 'test/'{BAT0,BAT1}'/capacity'
	echo 'Battery' > 'test/'{BAT0,BAT1}'/type'
	echo 'Mains' > 'test/ACAD/type'

test-onac:
	echo 1 > 'test/ACAD/online'
	$(RM) -r 'test/'{BAT0,BAT1}

test-noac:
	echo 0 > 'test/ACAD/online'

test-lowbat:
	echo 1 > 'test/'{BAT0,BAT1}'/capacity'

SUBDIRS = src img
