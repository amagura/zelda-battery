## Process this file with automake to produce Makefile.in

SUBDIRS = src
AM_CFLAGS = -std=c11 -g -gdb -O2 -Wall

clean-local: tidy-local

tidy-local:
	@$(RM) *.o

#expand-local: expand-madrid-local expand-dog-local

#expand-madrid-local:
#	for file in $(madrid_SOURCES); do \
#	  $(CC) $(CFLAGS) -E $$file; \
#	  done

#expand-dog-local:
#	for file in $(dog_SOURCES); do \
#	  $(CC) $(CFLAGS) -E $$file; \
#	  done

anew: clean all
ready: clean all tidy-local
