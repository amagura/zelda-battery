## Process this file with automake to produce Makefile.in

dist_doc_DATA = LICENSE ChangeLog

if GTK2
iconsdir = $(pkgdatadir)/icons
scaledir = $(iconsdir)/scalable/apps
pngdir = $(iconsdir)/16x16/apps
dist_scale_DATA = img/full.svg img/empty.svg img/qempty.svg img/qfull.svg img/half.svg img/bork.svg
dist_png_DATA = img/{full,empty,qempty,qfull,half,bork}.png
endif

SUBDIRS = src img
