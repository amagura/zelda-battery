## Process this file with automake to produce Makefile.in

SUBDIRS = src

clean-local: tidy-local

tidy-local:
	@$(RM) *.o || true
	@$(RM) zb-text zb-color || true
	$(RM) *.s || true

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null
