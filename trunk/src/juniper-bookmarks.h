#ifndef JUNIPER_BOOKMARKS
#define JUNIPER_BOOKMARKS

#include "glade/glade.h"
#include "gtk/gtk.h"

gboolean juniper_bookmarks_init(GladeXML *);
void juniper_bookmarks_reload();
void juniper_bookmarks_add_bookmark_menu_item_activate(GtkMenuItem * menu_item);
void juniper_bookmarks_menu_item_activate(GtkMenuItem * menu_item, const gchar * uri);
gchar * juniper_bookmarks_get(gchar * title);
void juniper_bookmarks_add(const gchar * uri, const gchar * name);

#endif
