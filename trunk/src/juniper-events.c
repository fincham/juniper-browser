#include "juniper-events.h"

#include "assert.h"
#include "string.h"

#include "gdk/gdkkeysyms.h"

#include "juniper-prefs.h"
#include "juniper-tabs.h"
#include "juniper-ui.h"

/*
 * It's OK to use current_tab here, as the user was interacting with the
 * current tab when they fired this event.
 */
void juniper_events_address_bar_activate(GtkEntry * address_bar)
{
    WebKitWebView * page;
    const gchar * url;

    url = gtk_entry_get_text(address_bar);

    if (!url || strlen(url) == 0)
        return;

    page = juniper_tabs_page_for_tab(juniper_tabs_current());
    assert(page != NULL);

    webkit_web_view_open(page, url);
}

/**
 * This can't use current_tab, because the page could be loading on a tab that
 * isn't the current one.
 */
void juniper_events_page_title_changed(WebKitWebView * page, WebKitWebFrame * frame, const gchar * page_title, GtkVBox * tab)
{
    GtkLabel * label;

#ifdef DEBUG
    printf("new title: %s\n", page_title);
#endif

    label = GTK_LABEL(gtk_notebook_get_tab_label(juniper_tabs(), GTK_WIDGET(tab)));
    gtk_label_set_text(label, page_title);
}

void juniper_events_page_load_started(WebKitWebView * page, WebKitWebFrame * frame, GtkVBox * tab)
{
    GtkEntry * address_bar;
    gchar * uri;

    address_bar = juniper_tabs_address_bar_for_tab(tab);
    uri = webkit_web_frame_get_uri(frame);

#ifdef DEBUG
    printf("page load started, uri: %s\n", uri);
#endif

    gtk_entry_set_text(address_bar, uri);
}

void juniper_events_about_activate(GtkMenuItem * menu_item)
{
    GtkDialog * dialog;

    dialog = juniper_ui_get_about_dialog();
    gtk_dialog_run(dialog);
    gtk_widget_hide(GTK_WIDGET(dialog));
}

gboolean juniper_events_script_alert(WebKitWebView * page, WebKitWebFrame * frame, const gchar * alert_message)
{
    return juniper_ui_show_message_box(GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Javascript Alert", alert_message);
}

gchar * juniper_events_choose_file(WebKitWebView * page, WebKitWebFrame * frame, const gchar * old_file)
{
    GtkWidget * dialog;
    gchar * filename = NULL;

    puts("file dialog!");

    dialog = gtk_file_chooser_dialog_new("Choose file", juniper_ui_get_window(), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        gchar * filename;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    }

    gtk_widget_destroy(dialog);

    return filename;
}

/**
 * This is for keypresses that apply to the whole browser including the tab bar
 * and therefore do not need a reference to a particular tab.
 */
gboolean juniper_events_tab_bar_key_press(GtkWidget * widget, GdkEventKey * event)
{
    if (event->state & GDK_CONTROL_MASK)
    {
        if (event->keyval == GDK_l)
        {
            gtk_widget_grab_focus(GTK_WIDGET(juniper_tabs_address_bar_for_tab(juniper_tabs_current())));
            return TRUE;
        }
        else if (event->keyval == GDK_Tab)
        {
            /* <Ctrl><Tab> cycles through tabs */
            juniper_tabs_next();
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * This is for keypresses that apply to a particular WebKitPage, and as such
 * require a reference to the tab itself.
 */
gboolean juniper_events_tab_key_press(GtkWidget * widget, GdkEventKey * event, GtkVBox * tab)
{
    WebKitWebView * page;

    page = juniper_tabs_page_for_tab(tab);

    if (event->state & GDK_MOD1_MASK)
    {
        if (event->keyval == GDK_Left)
        {
            /* <Alt><Left> goes back */
            if (webkit_web_view_can_go_back(page))
            {
                juniper_ui_status_bar_clear();
                webkit_web_view_go_back(page);
            }
            else
            {
                juniper_ui_status_bar_update("At start of history.");
            }

            return TRUE;
        }
        else if (event->keyval == GDK_Right)
        {
            /* <Alt><Right> goes forward */
            if (webkit_web_view_can_go_forward(page))
            {
                juniper_ui_status_bar_clear();
                webkit_web_view_go_forward(page);
            }
            else
            {
                juniper_ui_status_bar_update("At end of history.");
            }

            return TRUE;
        }
        else if (event->keyval == GDK_Home)
        {
            /* <Alt><Home> goes to the homepage */
            webkit_web_view_open(page, juniper_prefs_get_homepage());
            return TRUE;
        }
    }
    else if (event->state & GDK_CONTROL_MASK)
    {
        if (event->keyval == GDK_r)
        {
            /* <Ctrl>R reloads the page */
            webkit_web_view_reload(page);
            return TRUE;
        }
    }
    else if (event->state == 0)
    {
        if (event->keyval == GDK_Escape)
        {
            /* <Esc> stops loading the page */
            webkit_web_view_stop_loading(page);
            return TRUE;
        }
        else if (event->keyval == GDK_F5)
        {
            /* <F5> reloads the page */
            webkit_web_view_reload(page);
            return TRUE;
        }
    }
    
    return FALSE;
}

void juniper_events_page_link_hover(WebKitWebView * page, const gchar * title, const gchar * url)
{
    juniper_ui_status_bar_update(url);
}
