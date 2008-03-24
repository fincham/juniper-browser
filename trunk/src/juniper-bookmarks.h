#ifndef JUNIPER_BOOKMARKS
#define JUNIPER_BOOKMARKS

#include "glade/glade.h"
#include "gtk/gtk.h"

gboolean juniper_bookmarks_init(GladeXML *);
void juniper_bookmarks_add_bookmark_menu_item_activate(GtkMenuItem * menu_item);
gchar * juniper_bookmarks_get(gchar * title);
void juniper_bookmarks_add(gchar * title, gchar * url);

#endif
