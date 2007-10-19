#ifndef JUNIPER_TABS
#define JUNIPER_TABS

#include "gtk/gtk.h"
#include "WebKitGtk/webkitgtkpage.h"

void juniper_tabs_init(GtkNotebook *);
void juniper_tabs_cleanup();

GtkNotebook * juniper_tabs();
GtkVBox * juniper_tabs_current();
GtkEntry * juniper_tabs_address_bar_for_tab(GtkVBox *);
WebKitGtkPage * juniper_tabs_page_for_tab(GtkVBox *);

void juniper_tabs_close_current();
void juniper_tabs_add();
void juniper_tabs_add_with_location(gchar *);
void juniper_tabs_next();

#endif
