#if 0
/*
"""""
Copyright 2014, 2015, 2016, 2017 Alexej Magura

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
*/
#endif
#include <config.h>
import os, argparse, ConfigParser
#if WITH_GTK2
import pygtk
import gtk
import gobject
#elif WITH_QT4
import pyqt
#elif WITH_QT5
import pyqt5
#endif

DEF dataDir = DATADIR
DEF progName = ZB_PROGNAME;
DEF ftype = '.png'

## //// ^^ C Decls ^^ ////
cdef extern from "power.h":
    struct py_power:
        int acline
        int tr
        int raw
        int err

cdef extern from "power.h":
    py_power py_getpwr()
## //// $$ C Decls $$ ////

def getPixbuf(charge, imgDir):
    if charge >= 80:
        return gtk.gdk.pixbuf_new_from_file('%s/full%s' % (imgDir, ftype))
    elif charge >= 60:
        return gtk.gdk.pixbuf_new_from_file('%s/qempty%s' % (imgDir, ftype))
    elif charge >= 40:
        return gtk.gdk.pixbuf_new_from_file('%s/half%s' % (imgDir, ftype))
    elif charge >= 20:
        return gtk.gdk.pixbuf_new_from_file('%s/qfull%s' % (imgDir, ftype))
    elif charge >= 0:
        return gtk.gdk.pixbuf_new_from_file('%s/empty%s' % (imgDir, ftype))
    elif charge is -2:
        return gtk.gdk.pixbuf_new_from_file('%s/nobat%s' % (imgDir, ftype))
    else:
        return gtk.gdk.pixbuf_new_from_file('%s/bork%s' % (imgDir, ftype))

def sync_icon(tcon, cfg):
    pwr = py_getpwr()
    tcon.set_from_pixbuf(getPixbuf(pwr.raw if pwr.err is 0 else pwr.err,
                                   '%s/%s' % (cfg.get('gui', 'imgDir'), cfg.get('gui', 'theme'))))
    ttip = 'A/C: %s\n' % ('online' if pwr.acline is 1 else 'offline')
    if pwr.err is 0:
        ttip += 'Battery: %s' % str(pwr.raw)
    elif pwr.err is -2:
        ttip += 'No battery!'
    elif pwr.err is -5:
        ttip = 'No power supply!'
    tcon.set_tooltip(ttip);
    return True

def create_icon(cfg):
    tcon = gtk.StatusIcon()
    tcon.set_tooltip('gzbatt')
    tcon.set_visible(True)
    tcon.connect('popup-menu', clicked)
    sync_icon(tcon, cfg)
    src_id = gobject.timeout_add_seconds(1, sync_icon, tcon, cfg);

def clicked(tcon, button, time):
    menu = gtk.Menu()
    quitm = gtk.ImageMenuItem(gtk.STOCK_QUIT, '_Quit')
    quitm.connect('activate', gtk.main_quit)
    menu.append(quitm)
    menu.show_all()
    menu.popup(None, None, gtk.status_icon_position_menu, button, time, tcon)

def readCfg(file):
    cfg = ConfigParser.ConfigParser()
    if os.path.isfile(file):
        cfg.read(file)
    elif os.path.isfile('%s/example.conf' % dataDir):
        cfg.read('%s/example.conf' % dataDir)
    else:
        cfg.add_section('gui')
        cfg.set('gui', 'theme', 'original')
    return cfg

def main():
    ## Parse args
    parser = argparse.ArgumentParser(prog=progName,
                                     usage='%(prog)s [OPTION]...')
    parser.add_argument('-i',
                        metavar='<DIR>',
                        default=PNGDIR,
                        nargs=1,
                        help='specify an alternate image directory')
    parser.add_argument('-c',
                        metavar='<FILE>',
                        default='%s/.config/%s/%s.conf' % (os.environ['HOME'], progName[1:], progName[1:]),
                        nargs=1,
                        help='specify an alternate configuration file')

    args = parser.parse_args()
    cfg = readCfg(args.c[0])
    imgDir = args.i if os.path.exists(args.i) else '../img'
    cfg.set('gui', 'imgDir', imgDir)
    if (len(cfg.get('gui', 'theme')) < 4):
        cfg.set('gui', 'theme', 'orig')
    elif (len(cfg.get('gui', 'theme')) > 4):
        cfg.set('gui', 'theme', cfg.get('gui', 'theme')[0:4])
    create_icon(cfg)
    gtk.main()

if __name__ == '__main__':
    main()
