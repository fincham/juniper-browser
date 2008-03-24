#include "juniper-prefs.h"

#include "assert.h"
#include "string.h"

#include "juniper-fs.h"
#include "juniper-tabs.h"

#define BOOKMARKS_FILE "bookmarks.xml"

static GBookmarkFile * bookmarks;
static GladeXML * glade;

gboolean juniper_bookmarks_init(GladeXML * xml)
{
    GError * error = NULL;
    GtkMenu * bookmarks_menu;

    glade = xml;

    bookmarks_menu = GTK_MENU(glade_xml_get_widget(xml, "bookmarks_menu"));

    bookmarks = g_bookmark_file_new();
    g_bookmark_file_load_from_file(bookmarks, juniper_fs_build_filename(BOOKMARKS_FILE), &error);

    return true;
}

void juniper_bookmarks_add_bookmark_menu_item_activate(GtkMenuItem * menu_item)
{
    GtkDialog * add_bookmark_dialog;
    GtkVBox * tab;

    add_bookmark_dialog = GTK_DIALOG(glade_xml_get_widget(glade, "add_bookmark_dialog"));
    tab = juniper_tabs_current();

    gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(glade, "name_entry")), juniper_tabs_get_title(tab));
    gtk_entry_set_text(GTK_ENTRY(glade_xml_get_widget(glade, "url_entry")), gtk_entry_get_text(juniper_tabs_address_bar_for_tab(tab)));

    gtk_dialog_run(add_bookmark_dialog);
    gtk_widget_hide(GTK_WIDGET(add_bookmark_dialog));    
}

void juniper_bookmarks_menu_item_activate(GtkMenuItem * menu_item, gchar * url)
{
    juniper_tabs_add_with_location(url);
}

gchar ** juniper_bookmarks_get_uris(gsize *length)
{
    return g_bookmark_file_get_uris(bookmarks, length);
}

gchar * juniper_bookmarks_get_title_for_uri(const gchar * uri)
{
    GError * error;

    return g_bookmark_file_get_title(bookmarks, uri, &error);
}

gchar * juniper_bookmarks_get_description_for_uri(const gchar * uri)
{
    GError * error = NULL;

    return g_bookmark_file_get_description(bookmarks, uri, &error);
}
