#ifndef JUNIPER_BOOKMARKS
#define JUNIPER_BOOKMARKS

#include "gtk/gtk.h"

gboolean juniper_bookmarks_init(GtkMenu * bookmarks_menu);
gchar * juniper_bookmarks_get(gchar * title);
void juniper_bookmarks_add(gchar * title, gchar * url);

#endif
