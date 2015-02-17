/****
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
****/
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "compat.h"
#include "power.h"
#include "imgs.h"

GdkPixbuf *mkicon(struct power pwr)
{
     GdkPixbuf *pixbuf = NULL;
     switch ((int)(pwr.charge.raw / 25)) {
     case '4':
	  pixbuf = gdk_pixbuf_new_from_inline(-1, full_s1_16x16_inline, FALSE, NULL);
	  break;
     case '3':
	  pixbuf = gdk_pixbuf_new_from_inline(-1, full_s1_16x16_inline, FALSE, NULL);
	  break;
     case '2':
	  pixbuf = gdk_pixbuf_new_from_inline(-1, full_s1_10x10_inline, FALSE, NULL);
	  break;
     case '1':
	  pixbuf = gdk_pixbuf_new_from_inline(-1, full_s1_10x10_inline, FALSE, NULL);
	  break;
     case '0':
	  pixbuf = gdk_pixbuf_new_from_inline(-1, full_s1_10x10_inline, FALSE, NULL);
	  break;
	  }
     return pixbuf;
}

inline void create_icon()
{
     GtkStatusIcon *tcon = gtk_status_icon_new();
     gtk_status_icon_set_tooltip_text(tcon, "gzbatt");
     gtk_status_icon_set_visible(tcon, TRUE);
     sync_icon(tcon);
}

int main(int argc, char **argv)
{
     struct power pwr;
     pwr.charge.nof = -1;
     pwr.charge.radix = 10;

     gtk_init(&argc, &argv);
     GtkStatusIcon *tcon = NULL;
     GdkPixbuf *pbuf;
     getpwr(&pwr);
     pbuf = mkicon(pwr);
     gtk_status_icon_new_from_pixbuf(pbuf);
     gtk_status_icon_set_tooltip_text(tcon, "hello");
     gtk_status_icon_set_visible(tcon, TRUE);
     gtk_main();
     return EXIT_SUCCESS;
}
