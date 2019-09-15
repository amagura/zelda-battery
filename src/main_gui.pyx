#cython: language_level=3
IF 0:
     """""
     Copyright 2014-2019 Alexej Magura

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

import os, argparse, configparser
from gi import require_version
require_version('Gtk', '3.0')
from gi.repository import Gtk, GObject, GdkPixbuf

DEF ftype = '.png'


def getPixbuf(charge, imgDir):
    if charge >= 80:
        return GdkPixbuf.Pixbuf.new_from_file('%s/full%s' % (imgDir, ftype))
    elif charge >= 60:
        return GdkPixbuf.Pixbuf.new_from_file('%s/qempty%s' % (imgDir, ftype))
    elif charge >= 40:
        return GdkPixbuf.Pixbuf.new_from_file('%s/half%s' % (imgDir, ftype))
    elif charge >= 20:
        return GdkPixbuf.Pixbuf.new_from_file('%s/qfull%s' % (imgDir, ftype))
    elif charge >= 0:
        return GdkPixbuf.Pixbuf.new_from_file('%s/empty%s' % (imgDir, ftype))
    elif charge is -2:
        return GdkPixbuf.Pixbuf.new_from_file('%s/nobat%s' % (imgDir, ftype))
    else:
        return GdkPixbuf.Pixbuf.new_from_file('%s/bork%s' % (imgDir, ftype))

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
    tcon.set_tooltip_text(ttip);
    return True

def create_icon(cfg):
    tcon = Gtk.StatusIcon()
    tcon.set_tooltip_text('gzbatt')
    tcon.set_visible(True)
    tcon.connect('popup-menu', clicked)
    sync_icon(tcon, cfg)
    src_id = GObject.timeout_add_seconds(1, sync_icon, tcon, cfg);

def clicked(tcon, button, time):
    menu = Gtk.Menu()
    quitm = Gtk.MenuItem.new_with_mnemonic('_Quit')
    quitm.connect('activate', Gtk.main_quit)
    menu.append(quitm)
    menu.show_all()
    menu.popup_at_pointer(None);
    #menu.popup(None, None, tcon.position_menu, button, time)

def readCfg(file):
    cfg = configparser.ConfigParser()
    if os.path.isfile(file):
        cfg.read(file)
    elif os.path.isfile('%s/example.conf' % DATADIR):
        cfg.read('%s/example.conf' % DATADIR)
    else:
        cfg.add_section('gui')
        cfg.set('gui', 'theme', 'original')
    return cfg

def main():
     ## Parse args
     parser = argparse.ArgumentParser(prog=ZB_PROGNAME,
               usage='%(prog)s [OPTION]...')
     parser.add_argument('-i',
               metavar='<DIR>',
               default=PNGDIR,
               nargs=1,
               help='specify an alternate image directory')
     parser.add_argument('-c',
               metavar='<FILE>',
               default='%s/.config/%s/%s.conf' % (os.environ['HOME'], ZB_PROGNAME[1:], ZB_PROGNAME[1:]),
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
     Gtk.main()

if __name__ == '__main__':
    main()
