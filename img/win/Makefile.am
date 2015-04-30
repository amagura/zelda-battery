## Process this file with automake to produce Makefile.in

dist_doc_DATA = LICENSE.images
iconsdir = $(pkgdatadir)/icons
#scaledir = $(iconsdir)/scalable/apps
pngdir = $(iconsdir)/16x16/apps
balldir = $(pngdir)/ball
origdir = $(pngdir)/orig
clasdir = $(pngdir)/clas

dist_ball_DATA = ball/full.png ball/empty.png ball/qempty.png \
ball/qfull.png ball/half.png ball/bork.png ball/nobat.png

dist_orig_DATA = orig/full.png orig/empty.png orig/qempty.png \
orig/qfull.png orig/half.png orig/bork.png orig/nobat.png

dist_clas_DATA = clas/full.png clas/empty.png clas/qempty.png \
clas/qfull.png clas/half.png clas/bork.png clas/nobat.png

all-local: #local-license

install-data-local: LICENSE.images
	for d in $(subdirs); do \
	cd "$$d"; \
	$(MAKE); \
	cd ..; \
	done

LICENSE.images:
	cat LICENSE > LICENSE.images

clean-local:
	$(RM) LICENSE.images || true

#local-license:
#	sed -i 'N;$$!P;$$!D;$$d' LICENSE
#	sed -i '$$d' LICENSE
#	sed -i '/^[0-9]/d' LICENSE
#	find . -not -name '*Makefile*' -not -name '*LICENSE*' -not -type d -print | awk '{printf "%d. \"%s\"\n", NR, $$1}' >> LICENSE
#	echo >> LICENSE
#	echo "--" >> LICENSE
#	echo "Alexej Magura" >> LICENSE
