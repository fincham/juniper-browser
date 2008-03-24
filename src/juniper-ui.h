#ifndef JUNIPER_UI
#define JUNIPER_UI

#include "glade/glade.h"
#include "gtk/gtk.h"

gboolean juniper_ui_init(GladeXML *);
GtkWindow * juniper_ui_get_window();
void juniper_ui_set_window_title(const gchar * title);
gboolean juniper_ui_show_message_box(GtkMessageType type, GtkButtonsType buttons, const gchar *title, const gchar *message);
GtkStatusbar * juniper_ui_status_bar();
void juniper_ui_status_bar_clear();
void juniper_ui_status_bar_update(const gchar *text);
GtkDialog * juniper_ui_get_about_dialog();

#endif
