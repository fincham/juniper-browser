#ifndef JUNIPER_TABS
#define JUNIPER_TABS

#include "glade/glade.h"
#include "gtk/gtk.h"
#include "webkit/webkitwebview.h"

gboolean juniper_tabs_init(GladeXML *);
void juniper_tabs_cleanup();

GtkNotebook * juniper_tabs();
GtkVBox * juniper_tabs_current();
GtkVBox * juniper_tabs_nth(guint index);

void juniper_tabs_navigate_to(GtkVBox * tab, const gchar * location);

const gchar * juniper_tabs_get_title(GtkVBox * tab);
void juniper_tabs_set_title(GtkVBox * tab, const gchar * title);

GtkEntry * juniper_tabs_address_bar_for_tab(GtkVBox *);
WebKitWebView * juniper_tabs_page_for_tab(GtkVBox *);

void juniper_tabs_close_current();
void juniper_tabs_add();
void juniper_tabs_add_with_location(const gchar *);
void juniper_tabs_next();

#endif
