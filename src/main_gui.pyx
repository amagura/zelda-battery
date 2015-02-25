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
# //// $$ C Decls $$ ////

def select_icon(tcon, charge):
    pixbuf = gtk.gdk.pixbuf_new_from_file('../img/h1/f-16x16.png')
    tcon.set_from_pixbuf(pixbuf)

def sync_icon(tcon):
    pwr = py_getpwr()
    select_icon(tcon, pwr.tr)
    ttip = 'A/C Adatper: %s\n' % 'online' if pwr.acline is 1 else 'offline'
    if pwr.err is 0:
        ttip += 'Battery: %s' % str(pwr.raw)
    tcon.set_tooltip(ttip);
    return True

def create_icon():
    tcon = gtk.StatusIcon()
    tcon.set_tooltip('gzbatt')
    tcon.set_visible(True)
    src_id = gobject.timeout_add_seconds(1, sync_icon, tcon);


create_icon()
gtk.main()
