"""""
Copyright 2014, 2015 Alexej Magura

This file is a part of ZBatt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""""
import os
import pygtk
import gtk
import gobject
import argparse
import ConfigParser, os

# //// ^^ C Decls ^^ ////
cdef extern from "power.h":
    struct py_power:
        int acline
        int tr
        int raw
        int err

cdef extern from "power.h":
    py_power py_getpwr()

cdef extern from "main-gui.h":
    char *pngdir()
    char *progname()
    char *version()
    int debug()
# //// $$ C Decls $$ ////

imgDir = pngdir()
ftype = '.png'
progName = progname()
parser = argparse.ArgumentParser()

def iconDir(mode='origin', theme='origin', ovrride=0):
    if mode is 'original' or mode is 'origin':
        if os.path.exists(imgDir) and not ovrride:
            return '%s/origin/%s' % (imgDir, theme[0:4])
        else:
            return '../img/origin/%s' % (theme[0:4])
    else:
        if os.path.exists(imgDir) and not ovrride:
            return '%s/rev/%s' % (imgDir, theme[0:4])
        else:
            return '../img/rev/%s' % (theme[0:4])

def getPixbuf(charge):
    if charge >= 80:
        return gtk.gdk.pixbuf_new_from_file('%s/full%s' % (iconDir(), ftype))
    elif charge >= 60:
        return gtk.gdk.pixbuf_new_from_file('%s/qempty%s' % (iconDir(), ftype))
    elif charge >= 40:
        return gtk.gdk.pixbuf_new_from_file('%s/half%s' % (iconDir(), ftype))
    elif charge >= 20:
        return gtk.gdk.pixbuf_new_from_file('%s/qfull%s' % (iconDir(), ftype))
    elif charge >= 0:
        return gtk.gdk.pixbuf_new_from_file('%s/empty%s' % (iconDir(), ftype))
    elif charge is -2:
        return gtk.gdk.pixbuf_new_from_file('%s/nobat%s' % (iconDir(), ftype))
    else:
        return gtk.gdk.pixbuf_new_from_file('%s/bork%s' % (iconDir(), ftype))

def sync_icon(tcon):
    pwr = py_getpwr()
    tcon.set_from_pixbuf(getPixbuf(pwr.raw if pwr.err is 0 else pwr.err))
    ttip = 'A/C: %s\n' % ('online' if pwr.acline is 1 else 'offline')
    if pwr.err is 0:
        ttip += 'Battery: %s' % str(pwr.raw)
    elif pwr.err is -2:
        ttip += 'No battery!'
    elif pwr.err is -5:
        ttip = 'No power supply!'
    tcon.set_tooltip(ttip);
    return True

def create_icon():
    tcon = gtk.StatusIcon()
    tcon.set_tooltip('gzbatt')
    tcon.set_visible(True)
    tcon.connect('popup-menu', clicked)
    sync_icon(tcon)
    src_id = gobject.timeout_add_seconds(1, sync_icon, tcon);

def clicked(tcon, button, time):
    menu = gtk.Menu()
    quitm = gtk.ImageMenuItem(gtk.STOCK_QUIT, '_Quit')
    quitm.connect('activate', gtk.main_quit)
    menu.append(quitm)
    menu.show_all()
    menu.popup(None, None, gtk.status_icon_position_menu, button, time, tcon)

create_icon()
gtk.main()
