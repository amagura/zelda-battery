## Process this file with automake to produce Makefile.in

iconsdir = $(pkgdatadir)/icons
#scaledir = $(iconsdir)/scalable/apps
pngdir = $(iconsdir)/16x16/apps

if GTK2
#dist_scale_DATA = full.svg empty.svg qempty.svg qfull.svg half.svg bork.svg
dist_png_DATA = full.png empty.png qempty.png qfull.png half.png bork.png
endif

#all-local: local-license

local-license:
	sed -i 'N;$$!P;$$!D;$$d' LICENSE
	sed -i '$$d' LICENSE
	sed -i '/^[0-9]/d' LICENSE
	find . -not -name '*Makefile*' -not -name '*LICENSE*' -not -type d -print | awk '{printf "%d. \"%s\"\n", NR, $$1}' >> LICENSE
	echo >> LICENSE
	echo "--" >> LICENSE
	echo "Alexej Magura" >> LICENSE
