"""""
Copyright 2014-2015 Alexej Magura

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
    char *scaledir()
    char *pngdir()
    char *progname()
# //// $$ C Decls $$ ////

IF False:
    imgDir = scaledir()
    ftype = '.svg'
ELSE:
    imgDir = pngdir()
    ftype = '.png'
progName = progname()

def iconDir():
    if os.path.exists(imgDir):
        return imgDir
    else:
        return '../img'

def getPixbuf(charge):
    if charge is 5:
        return gtk.gdk.pixbuf_new_from_file('%s/full%s' % (iconDir(), ftype))
    elif charge is 4:
        return gtk.gdk.pixbuf_new_from_file('%s/qfull%s' % (iconDir(), ftype))
    elif charge is 3:
        return gtk.gdk.pixbuf_new_from_file('%s/half%s' % (iconDir(), ftype))
    elif charge is 2:
        return gtk.gdk.pixbuf_new_from_file('%s/qempty%s' % (iconDir(), ftype))
    elif charge is 1 or charge is 0:
        return gtk.gdk.pixbuf_new_from_file('%s/qempty%s' % (iconDir(), ftype))
    elif charge is -2:
        return gtk.gdk.pixbuf_new_from_file('%s/nobat%s' % (iconDir(), ftype))
    else:
        return gtk.gdk.pixbuf_new_from_file('%s/bork%s' % (iconDir(), ftype))

def sync_icon(tcon):
    pwr = py_getpwr()
    tcon.set_from_pixbuf(getPixbuf(pwr.tr if pwr.err is 0 else pwr.err))
    ttip = 'A/C: %s\n' % 'online' if pwr.acline is 1 else 'offline'
    if pwr.err is 0:
        ttip += 'Battery: %s' % str(pwr.raw)
    elif pwr.err is -2:
        ttip += 'No battery!'
    tcon.set_tooltip(ttip);
    return True

def create_icon():
    tcon = gtk.StatusIcon()
    tcon.set_tooltip('gzbatt')
    tcon.set_visible(True)
    sync_icon(tcon)
    src_id = gobject.timeout_add_seconds(1, sync_icon, tcon);

create_icon()
gtk.main()
