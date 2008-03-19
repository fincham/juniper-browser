#ifndef JUNIPER_EVENTS
#define JUNIPER_EVENTS

#include "gtk/gtk.h"
#include "webkit/webkitwebview.h"

void juniper_events_init();

void juniper_events_address_bar_activate(GtkEntry * address_bar);
void juniper_events_page_title_changed(WebKitWebView * page, WebKitWebFrame * frame, const gchar * page_title, GtkVBox * tab);
void juniper_events_page_load_started(WebKitWebView * page, WebKitWebFrame * frame, GtkVBox * tab);
void juniper_events_page_link_hover(WebKitWebView * page, const gchar * foo, const gchar * url);

gboolean juniper_events_script_alert(WebKitWebView * page, WebKitWebFrame * frame, const gchar * alert_message);
gchar * juniper_events_choose_file(WebKitWebView * page, WebKitWebFrame * frame, const gchar * old_file);

gboolean juniper_events_tab_key_press(GtkWidget * widget, GdkEventKey * event, GtkVBox * tab);
gboolean juniper_events_tab_bar_key_press(GtkWidget * widget, GdkEventKey * event);

#endif
