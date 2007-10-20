#ifndef JUNIPER_EVENTS
#define JUNIPER_EVENTS

#include "gtk/gtk.h"
#include "WebKitGtk/webkitgtkpage.h"

void juniper_events_init();

void juniper_events_address_bar_activate(GtkEntry * address_bar);
void juniper_events_page_title_changed(WebKitPage * page, const gchar * page_title, const gchar * url, GtkVBox * tab);
void juniper_events_page_load_started(WebKitPage * page, WebKitFrame * frame);
void juniper_events_page_load_finished(WebKitPage * page, WebKitFrame * frame);
void juniper_events_page_link_hover(WebKitPage * page, const gchar * foo, const gchar * url);

void juniper_events_javascript_alert(WebKitPage * page, WebKitFrame * frame, const gchar * alert_message);
gchar * juniper_events_choose_file(WebKitPage * page, WebKitFrame * frame, const gchar * old_file);

gboolean juniper_events_tab_key_press(GtkWidget * widget, GdkEventKey * event, GtkVBox * tab);
gboolean juniper_events_tab_bar_key_press(GtkWidget * widget, GdkEventKey * event);

#endif
