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
#include <string.h>
#include "main.h"
#include "compat.h"
#include "power.h"
#include "imgs.h"

#define GZB_MKPXBF(GZB_IMG)					\
     (gdk_pixbuf_new_from_inline(-1, GZB_IMG, FALSE, NULL))

#define GZB_MKLTT(GZB_LTT, GZB_SET, GZB_SIZ)				\
     do {								\
	  (GZB_LTT).full =						\
	       GZB_MKPXBF(GZB_SET##f##_##GZB_SIZ##x##GZB_SIZ);		\
	  (GZB_LTT).qtre =						\
	       GZB_MKPXBF(GZB_SET##qe##_##GZB_SIZ##x##GZB_SIZ);		\
	  (GZB_LTT).half =						\
	       GZB_MKPXBF(GZB_SET##h##_##GZB_SIZ##x##GZB_SIZ);		\
	  (GZB_LTT).qtrf =						\
	       GZB_MKPXBF(GZB_SET##qf##_##GZB_SIZ##x##GZB_SIZ);		\
	  (GZB_LTT).empt =						\
	       GZB_MKPXBF(GZB_SET##e##_##GZB_SIZ##x##GZB_SIZ);		\
	  (GZB_LTT).bork						\
	       = GZB_MKPXBF(GZB_SET##b##_##GZB_SIZ##x##GZB_SIZ);	\
     } while(0)
#define GZB_AC_TIP "A/C: offline"
#define GZB_BAT_TIP "Battery: 100%"
#define GZB_TOOLTIP_SIZE (sizeof(GZB_AC_TIP "\n" GZB_BAT_TIP "\n" GZB_BAT_TIP))
#define GZB_AC_IMG(ZB_PWR) (((ZB_PWR).acline) ? ltt.full : ltt.empt)

struct hearts {
     GdkPixbuf *full; // 80 - 100
     GdkPixbuf *qtre; // 60 - 80
     GdkPixbuf *half; // 40 - 60
     GdkPixbuf *qtrf; // 20 - 40
     GdkPixbuf *empt; // 0 - 20
     GdkPixbuf *bork; // error
};

struct hearts get_imgs(int set, bool small)
{
     struct hearts ltt;
     switch (set) {
     case 1:
	  if (small)
	       GZB_MKLTT(ltt, h1, 10);
	  else
	       GZB_MKLTT(ltt, h1, 16);
	  break;
#if defined(h2f_10x10)
     case 2:
	  if (small)
	       GZB_MKLTT(ltt, h2, 10);
	  else
	       GZB_MKLTT(ltt, h2, 16);
	  break;
#endif
#if defined(h3f_10x10)
     case 3:
	  if (small)
	       GZB_MKLTT(ltt, h3, 10);
	  else
	       GZB_MKLTT(ltt, h3, 16);
	  break;
#endif
     }
     return ltt;
}

GdkPixbuf *select_img(int set, bool small, struct power pwr)
{
     struct hearts ltt = get_imgs(set, small);
     ZB_DBG("pwr.charge.tr: %d\n", pwr.charge.tr);
     ZB_DBG("pwr.charge.err: %d\n", pwr.charge.err);
     ZB_DBG("pwr.acline: %d\n", pwr.acline);
     int test = (pwr.charge.err == ZB_PWR_OK
		 ? pwr.charge.tr
		 : pwr.charge.err);
     switch (test) {
     case 5:
	  return ltt.full;
     case 4:
	  return ltt.qtre;
     case 3:
	  return ltt.half;
     case 2:
	  return ltt.qtrf;
     case 1:
	  return ltt.empt;
     case 0:
	  return ltt.empt;
     case ZB_PWR_NWANTBAT:
	  return GZB_AC_IMG(pwr);
     case ZB_PWR_NBAT:
	  return GZB_AC_IMG(pwr);
     default:
	  return ltt.bork;
     }
}

int sync_icon(GtkStatusIcon *tcon)
{
     struct power pwr;

//     char *tooltip = malloc(GZB_TOOLTIP_SIZE);
     pwr.charge.nof = -1;
     pwr.charge.divsr = 20;
     int err = getpwr(&pwr);

     gtk_status_icon_set_from_pixbuf(tcon, select_img(1, 0, pwr));

     if (pwr.acline)
	  gtk_status_icon_set_tooltip(tcon, "A/C: online");
     else
	  gtk_status_icon_set_tooltip(tcon, "A/C: offline");
/*
     if (pwr.acline)
	  tooltip = nek
     else
	  tooltip = neko("A/C: offline\n", "Battery: ", itoa(pwr.charge.raw), "\n", NULL);
     gtk_status_icon_set_tooltip_text(tooltip);
     free(tooltip);
*/
     return 1;
}

inline void create_icon()
{
     GtkStatusIcon *tcon = gtk_status_icon_new();
     gtk_status_icon_set_tooltip_text(tcon, "gzbatt");
     gtk_status_icon_set_visible(tcon, TRUE);
     sync_icon(tcon);
     g_timeout_add_seconds(1, (GSourceFunc)sync_icon, (gpointer)tcon);
}

int main(int argc, char **argv)
{
     gtk_init(&argc, &argv);
     create_icon();
     gtk_main();
     return EXIT_SUCCESS;
}
