#include "juniper-events.h"

#include "assert.h"
#include "string.h"

#include "gdk/gdkkeysyms.h"
#include "webkit/webkitwebframe.h"

#ifndef NO_WEBKIT_NAVIGATION_ACTION
#include "webkit/webkitnavigationaction.h"
#endif

#include "juniper-history.h"
#include "juniper-prefs.h"
#include "juniper-tabs.h"
#include "juniper-ui.h"
#include "juniper-util.h"

/*
 * It's OK to use current_tab here, as the user was interacting with the
 * current tab when they fired this event.
 */
void juniper_events_address_bar_activate(GtkEntry * address_bar)
{
    const gchar * uri;

    uri = gtk_entry_get_text(address_bar);

    if (!uri || strlen(uri) == 0)
        return;

    juniper_tabs_navigate_to(juniper_tabs_current(), juniper_util_normalise_url(uri));
}

void juniper_events_address_bar_changed(GtkEntry * address_bar)
{
    GSList * match;
    const gchar * text;
    gint entered_length;

    text = gtk_entry_get_text(address_bar);
    entered_length = strlen(text);

    if (text == NULL || entered_length == 0)
        return;

    match = juniper_history_matches(text);

    if (match == NULL)
        return;
}

/**
 * This can't use current_tab, because the page could be loading on a tab that
 * isn't the current one.
 */
void juniper_events_page_title_changed(WebKitWebView * page, WebKitWebFrame * frame, const gchar * page_title, GtkVBox * tab)
{
#ifdef DEBUG
    printf("new title: %s\n", page_title);
#endif

    juniper_tabs_set_title(tab, page_title);
}

void juniper_events_current_tab_changed(GtkNotebook * tabs, GtkNotebookPage * notebook_page, guint page_num)
{
    juniper_ui_set_window_title(juniper_tabs_get_title(juniper_tabs_nth(page_num)));
}

#ifndef NO_WEBKIT_NAVIGATION_ACTION
WebKitNavigationResponse juniper_events_navigation_requested(WebKitWebView * page, WebKitNavigationAction * action, WebKitWebFrame * frame, WebKitNetworkRequest * request, GtkVBox * tab)
{
    gint button, modifier_flags, navigation_type;
    const gchar * url;

    button = webkit_navigation_action_get_button(action);
    navigation_type = webkit_navigation_action_get_navigation_type(action);
    modifier_flags = webkit_navigation_action_get_modifier_flags(action);
    url = webkit_navigation_action_get_original_url(action);

#ifdef DEBUG
    printf("navigation requested: button=%i, navigation_type=%i, modifier_flags=%i, url=%s\n", button, navigation_type, modifier_flags, url);
#endif

    if (button == 1 || (button == 0 && (modifier_flags & GDK_CONTROL_MASK))) /* middle click or ctrl-click */
    {
        juniper_tabs_add_with_location(url);
        return WEBKIT_NAVIGATION_RESPONSE_IGNORE;
    }

    juniper_ui_status_bar_update(juniper_util_sprintf("Navigating to %s", url));

    return WEBKIT_NAVIGATION_RESPONSE_ACCEPT;
}
#endif

void juniper_events_page_load_started(WebKitWebView * page, WebKitWebFrame * frame, GtkVBox * tab)
{
    GtkEntry * address_bar;
    const gchar * uri;

    address_bar = juniper_tabs_address_bar_for_tab(tab);
    uri = webkit_web_frame_get_uri(frame);

#ifdef DEBUG
    printf("page load started, uri: %s\n", uri);
#endif

    gtk_entry_set_text(address_bar, uri);
    juniper_history_add(uri, juniper_tabs_get_title(tab));
}

void juniper_events_page_load_finished(WebKitWebView * page, WebKitWebFrame * frame, GtkVBox * tab)
{
    GtkEntry * address_bar;

    address_bar = juniper_tabs_address_bar_for_tab(tab);
    juniper_history_add(gtk_entry_get_text(address_bar), juniper_tabs_get_title(tab));
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
            juniper_tabs_navigate_to(tab, juniper_prefs_get("homepage"));
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
