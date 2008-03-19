/* CAN HAS STDIO? (sorry, couldn't resist) */

#include "gtk/gtk.h"
#include "glade/glade.h"

#include "juniper-events.h"
#include "juniper-tabs.h"
#include "juniper-view-source.h"
#include "juniper-prefs.h"
#include "juniper-bookmarks.h"
#include "juniper-ui.h"

void juniper_quit()
{
    juniper_tabs_cleanup();
    gtk_main_quit();
}

int main(int argc, char **argv)
{
    GladeXML * xml;
    GtkWindow * window;
    GtkNotebook * tabs;
    gchar * url;

    gtk_init(&argc, &argv);
    xml = glade_xml_new("juniper.glade", NULL, NULL);
    glade_xml_signal_autoconnect(xml);

    window = GTK_WINDOW(glade_xml_get_widget(xml, "main_window"));
    tabs = GTK_NOTEBOOK(glade_xml_get_widget(xml, "tabs"));

    if (!juniper_prefs_init())
        return 1;

    if (!juniper_bookmarks_init(GTK_MENU(glade_xml_get_widget(xml, "bookmarks_menu"))))
        return 1;

    juniper_ui_init(window, GTK_STATUSBAR(glade_xml_get_widget(xml, "status_bar")), GTK_DIALOG(glade_xml_get_widget(xml, "about_dialog")));
    juniper_tabs_init(tabs);

    url = (argc == 2) ? argv[1] : juniper_prefs_get_homepage();

    if (url)
    {
        juniper_tabs_add_with_location(url);
    }
    else
    {
        juniper_tabs_add();
    }

    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    return 0;
}

/* KTHXBYE */
