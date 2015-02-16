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
#include <stdlib.h>
#include "main.h"
#include "compat.h"
#include "acpi.h"
#include "imgs.h"

GdkPixbuf *new_pixbuf(const gchar *f)
{
     GdkPixbuf *pbuf;
     GError *err = NULL;
     pbuf = gdk_pixbuf_new_from_file(f, &err);
     if (pbuf == NULL) {
	  fprintf(stderr, "%s\n", err->message);
	  g_error_free(err);
     }
     return pbuf;
}

int main(int argc, char **argv)
{
     gtk_init(&argc, &argv);
     GtkStatusIcon *tcon;
     GdkPixbuf *pixbuf = gdk_pixbuf_new_from_inline(-1, full_s1_inline, FALSE, NULL);
     tcon = gtk_status_icon_new_from_pixbuf(pixbuf);
     gtk_status_icon_set_tooltip_text(tcon, "hello");
     gtk_status_icon_set_visible(tcon, TRUE);
//     g_signal_connect_
     gtk_main();
     return EXIT_SUCCESS;
}
