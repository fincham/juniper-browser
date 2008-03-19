#include "juniper-view-source.h"

#include "gtk/gtk.h"
#include "gtksourceview/gtksourceview.h"
#include "gtksourceview/gtksourcelanguagesmanager.h"
#include "webkit/webkitwebview.h"

#include "juniper-tabs.h"

void juniper_view_source_current_tab()
{
    GtkWindow *view_source_window;
    GtkScrolledWindow *scrolled_window;
    GtkSourceLanguagesManager *languages_manager;
    GtkSourceBuffer *source_buffer;
    GtkSourceView *source_view;
    WebKitWebView *page;
    
    page = juniper_tabs_page_for_tab(juniper_tabs_current());

    view_source_window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_title(view_source_window, "View Source");
    gtk_window_set_destroy_with_parent(view_source_window, TRUE);

    scrolled_window = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(NULL, NULL));
    gtk_scrolled_window_set_policy(scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    languages_manager = gtk_source_languages_manager_new();
    source_buffer = GTK_SOURCE_BUFFER(gtk_source_buffer_new_with_language(gtk_source_languages_manager_get_language_from_mime_type(languages_manager, "text/html")));
/*gtk_text_buffer_set_text(GTK_TEXT_BUFFER(source_buffer), webkit_gtk_frame_get_markup(webkit_web_view_get_main_frame(page)), -1);*/

    source_view = GTK_SOURCE_VIEW(gtk_source_view_new_with_buffer(source_buffer));

    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(source_view));
    gtk_container_add(GTK_CONTAINER(view_source_window), GTK_WIDGET(scrolled_window));

    gtk_widget_show_all(GTK_WIDGET(view_source_window));
}
