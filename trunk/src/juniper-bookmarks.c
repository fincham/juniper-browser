#include "juniper-prefs.h"

#include "assert.h"
#include "string.h"

#include "juniper-fs.h"
#include "juniper-tabs.h"

#define BOOKMARKS_FILE "bookmarks.xml"

static GBookmarkFile * bookmarks;

void juniper_bookmarks_menu_item_activate(GtkMenuItem * menu_item, gchar * url)
{
    juniper_tabs_add_with_location(url);
}

gboolean juniper_bookmarks_init(GtkMenu * bookmarks_menu)
{
    GError * error = NULL;

    bookmarks = g_bookmark_file_new();
    g_bookmark_file_load_from_file(bookmarks, juniper_fs_build_filename(BOOKMARKS_FILE), &error);

    return true;
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
