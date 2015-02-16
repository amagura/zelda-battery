## Process this file with automake to produce Makefile.in

all-local: local-license

local-license:
	sed -i 'N;$$!P;$$!D;$$d' LICENSE
	sed -i '$$d' LICENSE
	sed -i '/^[0-9]/d' LICENSE
	find . -not -name '*Makefile*' -not -name '*LICENSE*' -not -type d -print | awk '{printf "%d. \"%s\"\n", NR, $$1}' >> LICENSE
	echo >> LICENSE
	echo "--" >> LICENSE
	echo "Alexej Magura" >> LICENSE
