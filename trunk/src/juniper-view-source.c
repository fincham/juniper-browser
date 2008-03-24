#include "juniper-view-source.h"

#include "gtksourceview/gtksourcelanguagesmanager.h"
#include "webkit/webkitwebview.h"
#include "webkit/webkitwebframe.h"

#include "juniper-tabs.h"
#include "juniper-util.h"

void juniper_view_source_current_tab()
{
    GtkWindow *view_source_window;
    GtkVBox *vertical_box;
    GtkScrolledWindow *scrolled_window;
    GtkSourceLanguagesManager *languages_manager;
    GtkSourceBuffer *source_buffer;
    GtkSourceView *source_view;
    GtkStatusbar *status_bar;
    WebKitWebView *page;

    page = juniper_tabs_page_for_tab(juniper_tabs_current());

    view_source_window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_title(view_source_window, "View Source");
    gtk_window_set_destroy_with_parent(view_source_window, TRUE);

    vertical_box = GTK_VBOX(gtk_vbox_new(FALSE,0));

    scrolled_window = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(NULL, NULL));
    gtk_scrolled_window_set_policy(scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    status_bar = GTK_STATUSBAR(gtk_statusbar_new());
    gtk_statusbar_set_has_resize_grip(status_bar, TRUE);
    gtk_statusbar_push(GTK_STATUSBAR(status_bar), 0, "0, 0");

    languages_manager = gtk_source_languages_manager_new();
    source_buffer = GTK_SOURCE_BUFFER(gtk_source_buffer_new_with_language(gtk_source_languages_manager_get_language_from_mime_type(languages_manager, "text/html")));
/*  gtk_text_buffer_set_text(GTK_TEXT_BUFFER(source_buffer), webkit_web_frame_get_markup(webkit_web_view_get_main_frame(page)), -1); */

    source_view = GTK_SOURCE_VIEW(gtk_source_view_new_with_buffer(source_buffer));
    g_signal_connect(GTK_TEXT_BUFFER(source_buffer), "mark-set", G_CALLBACK(juniper_view_source_mark_set), status_bar);

    gtk_container_add(GTK_CONTAINER(view_source_window), GTK_WIDGET(vertical_box));
    gtk_box_pack_start(GTK_BOX(vertical_box), GTK_WIDGET(scrolled_window), TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(source_view));
    gtk_box_pack_start(GTK_BOX(vertical_box), GTK_WIDGET(status_bar), FALSE, FALSE, 0);

#ifdef DEBUG
    puts("Showing View Source window...");
#endif

    gtk_widget_show_all(GTK_WIDGET(view_source_window));
}

void juniper_view_source_mark_set(GtkSourceView *source_view, GtkTextIter *location, GtkTextMark *mark, GtkStatusbar *status_bar) /* called when the caret is moved in the source view */
{
    gtk_statusbar_pop(GTK_STATUSBAR(status_bar), 0);
    gtk_statusbar_push(GTK_STATUSBAR(status_bar), 0, juniper_util_sprintf("%i, %i", gtk_text_iter_get_line(location), gtk_text_iter_get_line_offset(location)));
}

