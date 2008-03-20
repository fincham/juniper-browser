#include "juniper-tabs.h"

#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

#include "juniper-events.h"
#include "juniper-prefs.h"
#include "juniper-ui.h"

#define MAX_TAB_COUNT  100

static GtkNotebook * tabs;
static gboolean first_tab_added = TRUE;

void juniper_tabs_init(GtkNotebook * i_tabs)
{
    tabs = i_tabs;
}

GtkNotebook * juniper_tabs()
{
    return tabs;
}

gint juniper_tabs_count()
{
    return gtk_notebook_get_n_pages(tabs);
}

GtkVBox * juniper_tabs_current()
{
    gint page_num;
    GtkVBox * tab;

    page_num = gtk_notebook_get_current_page(tabs);
    assert(page_num > -1);

    tab = juniper_tabs_nth(page_num);
    assert(tab != NULL);

    return tab;
}

GtkVBox * juniper_tabs_nth(guint index)
{
    return GTK_VBOX(gtk_notebook_get_nth_page(tabs, index));
}

const gchar * juniper_tabs_get_title(GtkVBox * tab)
{
    return gtk_label_get_text(GTK_LABEL(gtk_notebook_get_tab_label(tabs, GTK_WIDGET(tab))));
}

void juniper_tabs_set_title(GtkVBox * tab, const gchar * title)
{
    GtkLabel * label;

    label = GTK_LABEL(gtk_notebook_get_tab_label(tabs, GTK_WIDGET(tab)));
    gtk_label_set_text(label, title);

    if (tab == juniper_tabs_current())
    {
        juniper_ui_set_window_title(title);
    }
}

GtkWidget * juniper_tabs_nth_widget_for_tab(GtkVBox * tab, guint index)
{
    GtkBox * box;
    GtkBoxChild * child;

    assert(0 <= index && index <= 1);

    box = GTK_BOX(tab);
    assert(g_list_length(box->children) == 2);

    child = (GtkBoxChild *) g_list_nth_data(box->children, index);
    assert(child->widget != NULL);

    return GTK_WIDGET(child->widget);
}

GtkEntry * juniper_tabs_address_bar_for_tab(GtkVBox * tab)
{
    GtkWidget * address_bar;

    address_bar = juniper_tabs_nth_widget_for_tab(tab, 0);
    assert(address_bar != NULL);

    return GTK_ENTRY(address_bar);
}

WebKitWebView * juniper_tabs_page_for_tab(GtkVBox * tab)
{
    GtkWidget * scrolled_window, * page;

    scrolled_window = juniper_tabs_nth_widget_for_tab(tab, 1);
    assert(scrolled_window != NULL);

    page = gtk_bin_get_child(GTK_BIN(scrolled_window));
    assert(page != NULL);

    return WEBKIT_WEB_VIEW(page);
}

void juniper_tabs_close_current()
{
    GtkVBox *tab;

    tab = juniper_tabs_current();

    /* remove the tab from the notebook */
    gtk_notebook_remove_page(tabs, gtk_notebook_get_current_page(tabs));
}

void juniper_tabs_next()
{
    gint current_page, n_pages;

    current_page = gtk_notebook_get_current_page(tabs);
    n_pages = gtk_notebook_get_n_pages(tabs);

    if (current_page < n_pages - 1)
    {
        gtk_notebook_next_page(tabs);
    }
    else
    {
        gtk_notebook_set_current_page(tabs, 0);
    }
}

void juniper_tabs_navigate_to(GtkVBox * tab, const gchar * location)
{
    assert(tab != NULL);

    if (location == NULL)
        return;

    gtk_label_set_text(GTK_LABEL(gtk_notebook_get_tab_label(tabs, GTK_WIDGET(tab))), "loading...");
    webkit_web_view_open(juniper_tabs_page_for_tab(tab), location);
}

void juniper_tabs_add_with_location(const gchar * location)
{
    GtkEntry * address_bar;
    WebKitWebView * page;
    GtkVBox * vbox;
    GtkScrolledWindow * scrolled_window;
    GtkLabel * label;
    WebKitWebViewClass * page_class;

    if (juniper_tabs_count() + 1 > MAX_TAB_COUNT)
        return;

    /* build the viewport struct */
    address_bar = GTK_ENTRY(gtk_entry_new());
    page = WEBKIT_WEB_VIEW(webkit_web_view_new());

    page_class = WEBKIT_WEB_VIEW_GET_CLASS(page);
    page_class->script_alert = juniper_events_script_alert;
    page_class->choose_file  = juniper_events_choose_file;

    /* lay out the tab UI */
    scrolled_window = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(NULL, NULL));
    gtk_scrolled_window_set_policy(scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(page));

    vbox = GTK_VBOX(gtk_vbox_new(FALSE, 0));
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(address_bar), FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(scrolled_window), TRUE, TRUE, 0);
    gtk_widget_show_all(GTK_WIDGET(vbox));

    /* append the tab with a [blank] label and focus it */
    label = GTK_LABEL(gtk_label_new(""));
    gtk_notebook_set_current_page(tabs, gtk_notebook_append_page(tabs, GTK_WIDGET(vbox), GTK_WIDGET(label)));

    /* connect signal handlers */
    g_signal_connect(page, "navigation-requested", G_CALLBACK(juniper_events_navigation_requested), vbox);
    g_signal_connect(page, "load-committed", G_CALLBACK(juniper_events_page_load_started), vbox);
    g_signal_connect(page, "title-changed", G_CALLBACK(juniper_events_page_title_changed), vbox);
    g_signal_connect(page, "hovering-over-link", G_CALLBACK(juniper_events_page_link_hover), NULL);
    g_signal_connect(page, "key-press-event", G_CALLBACK(juniper_events_tab_key_press), vbox);
    g_signal_connect(address_bar, "activate", G_CALLBACK(juniper_events_address_bar_activate), NULL);
    g_signal_connect(address_bar, "key-press-event", G_CALLBACK(juniper_events_tab_key_press), vbox);

    if (first_tab_added)
    {
        g_signal_connect(tabs, "switch-page", G_CALLBACK(juniper_events_current_tab_changed), NULL);
        first_tab_added = FALSE;
    }

    if (location)
    {
        juniper_tabs_navigate_to(vbox, location);
    }
    else
    {
        gtk_label_set_text(label, "blank");
    }
}

void juniper_tabs_add()
{
    juniper_tabs_add_with_location(juniper_prefs_get("open_homepage_on_new_tab") ? juniper_prefs_get("homepage") : NULL);
}

void juniper_tabs_cleanup()
{
    while (gtk_notebook_get_n_pages(tabs) > 0)
    {
        juniper_tabs_close_current();
    }
}
