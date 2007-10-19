#include "juniper-tabs.h"

#include "assert.h"
#include "stdio.h"

#include "juniper-events.h"
#include "juniper-prefs.h"

static GtkNotebook * tabs;

void juniper_tabs_init(GtkNotebook * i_tabs)
{
    tabs = i_tabs;
}

GtkNotebook * juniper_tabs()
{
    return tabs;
}

GtkVBox * juniper_tabs_current()
{
    gint page_num;
    GtkVBox * tab;

    page_num = gtk_notebook_get_current_page(tabs);
    assert(page_num > -1);

    tab = GTK_VBOX(gtk_notebook_get_nth_page(tabs, page_num));
    assert(tab != NULL);

    return tab;
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

WebKitGtkPage * juniper_tabs_page_for_tab(GtkVBox * tab)
{
    GtkWidget * scrolled_window, * page;

    scrolled_window = juniper_tabs_nth_widget_for_tab(tab, 1);
    assert(scrolled_window != NULL);

    page = gtk_bin_get_child(GTK_BIN(scrolled_window));
    assert(page != NULL);

    return WEBKIT_GTK_PAGE(page);
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

void juniper_tabs_add_with_location(gchar *location)
{
    GtkEntry * address_bar;
    WebKitGtkPage * page;
    GtkVBox * vbox;
    GtkScrolledWindow * scrolled_window;
    GtkLabel * label;
    WebKitGtkPageClass * page_class;

    /* build the viewport struct */
    address_bar = GTK_ENTRY(gtk_entry_new());
    page = WEBKIT_GTK_PAGE(webkit_gtk_page_new());

    page_class = WEBKIT_GTK_PAGE_GET_CLASS(page);
    page_class->java_script_alert = juniper_events_javascript_alert;
    page_class->choose_file       = juniper_events_choose_file;

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
    g_signal_connect(page, "title-changed", G_CALLBACK(juniper_events_page_title_changed), vbox);
    g_signal_connect(page, "hovering-over-link", G_CALLBACK(juniper_events_page_link_hover), NULL);
    g_signal_connect(page, "key-press-event", G_CALLBACK(juniper_events_tab_key_press), vbox);
    g_signal_connect(address_bar, "activate", G_CALLBACK(juniper_events_address_bar_activate), NULL);
    g_signal_connect(address_bar, "key-press-event", G_CALLBACK(juniper_events_tab_key_press), vbox);

    if (location) {
        gtk_label_set_text(label, "loading...");
        webkit_gtk_page_open(page, location);
    }
    else
    {
        gtk_label_set_text(label, "blank");
    }
}

void juniper_tabs_add()
{
    juniper_tabs_add_with_location(juniper_prefs_get_open_homepage_on_new_tab() ? juniper_prefs_get_homepage() : NULL);
}

void juniper_tabs_cleanup()
{
    while (gtk_notebook_get_n_pages(tabs) > 0)
    {
        juniper_tabs_close_current();
    }
}
