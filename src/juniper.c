/* CAN HAS STDIO? (sorry, couldn't resist) */

#include "gtk/gtk.h"
#include "glade/glade.h"

#include "juniper-db.h"
#include "juniper-events.h"
#include "juniper-tabs.h"
#include "juniper-view-source.h"
#include "juniper-prefs.h"
#include "juniper-bookmarks.h"
#include "juniper-ui.h"
#include "juniper-util.h"

void juniper_quit()
{
    juniper_tabs_cleanup();
    gtk_main_quit();
}

int main(int argc, char **argv)
{
    GladeXML * xml;
    const gchar * url;

    gtk_init(&argc, &argv);
    g_set_application_name("Juniper");

    xml = glade_xml_new("/usr/share/juniper/juniper.glade", NULL, NULL);
    glade_xml_signal_autoconnect(xml);

    if (!(juniper_db_init() && juniper_prefs_init() && juniper_bookmarks_init(xml) && juniper_ui_init(xml) && juniper_tabs_init(xml)))
        return 1;

    url = (argc == 2) ? argv[1] : juniper_prefs_get("homepage");

    if (url)
    {
        juniper_tabs_add_with_location(juniper_util_normalise_url(url));
    }
    else
    {
        juniper_tabs_add();
    }

    gtk_main();

    return 0;
}

/* KTHXBYE */
