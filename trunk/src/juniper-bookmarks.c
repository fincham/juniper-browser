#include "juniper-bookmarks.h"

#include "assert.h"
#include "string.h"
#include "sqlite3.h"

#include "juniper-db.h"
#include "juniper-tabs.h"

static GladeXML * glade;
static GtkContainer * bookmarks_menu;

static sqlite3 * db_handle;

#define BOOKMARKS_MENU_POS 2
#define NUM_MENUS 4

gboolean juniper_bookmarks_init(GladeXML * xml)
{
    glade = xml;
    bookmarks_menu = GTK_CONTAINER(glade_xml_get_widget(glade, "bookmarks_menu"));

    db_handle = juniper_db_get_handle();
    assert(db_handle != NULL);

    sqlite3_exec(db_handle, "create table bookmarks (uri varchar(255) not null unique, name varchar (255) not null)", NULL, NULL, NULL);

    juniper_bookmarks_reload();

    return true;
}

void juniper_bookmarks_reload()
{
    sqlite3_stmt * statement;
    GList * menu_item;
    GtkWidget * bookmark, * separator;
    const gchar * name;
    gchar * uri;
    gboolean first;

#ifdef DEBUG
    puts("reloading bookmarks");
#endif

    menu_item = gtk_container_get_children(bookmarks_menu);

    while ((menu_item = g_list_next(menu_item)) != NULL)
        gtk_container_remove(bookmarks_menu, GTK_WIDGET(menu_item->data));

    sqlite3_prepare_v2(db_handle, "select name, uri from bookmarks order by name", -1, &statement, NULL);

    first = true;
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        if (first)
        {
            separator = gtk_separator_menu_item_new();
            gtk_menu_shell_append(GTK_MENU_SHELL(bookmarks_menu), separator);
            gtk_widget_show(separator);
            first = false;
        }

        name = g_strdup((gchar *) sqlite3_column_text(statement, 0));
        uri = g_strdup((gchar *) sqlite3_column_text(statement, 1));

        bookmark = gtk_menu_item_new_with_label(name);
        g_signal_connect(bookmark, "activate", G_CALLBACK(juniper_bookmarks_menu_item_activate), uri);

        gtk_menu_shell_append(GTK_MENU_SHELL(bookmarks_menu), bookmark);
        gtk_widget_show(bookmark);

#ifdef DEBUG
        printf("bookmark added: name=%s, uri=%s\n", name, uri);
#endif
    }

    sqlite3_finalize(statement);

#ifdef DEBUG
    puts("done reloading bookmarks");
#endif
}

void juniper_bookmarks_add_bookmark_menu_item_activate(GtkMenuItem * menu_item)
{
    GtkDialog * add_bookmark_dialog;
    GtkVBox * tab;
    GtkEntry * name_entry, * url_entry;
    const gchar * name, * url;

    add_bookmark_dialog = GTK_DIALOG(glade_xml_get_widget(glade, "add_bookmark_dialog"));
    tab = juniper_tabs_current();

    name_entry = GTK_ENTRY(glade_xml_get_widget(glade, "name_entry"));
    url_entry = GTK_ENTRY(glade_xml_get_widget(glade, "url_entry"));

    gtk_entry_set_text(name_entry, juniper_tabs_get_title(tab));
    gtk_entry_set_text(url_entry, gtk_entry_get_text(juniper_tabs_address_bar_for_tab(tab)));

    if (gtk_dialog_run(add_bookmark_dialog) == 1)
    {
        name = gtk_entry_get_text(name_entry);
        url = gtk_entry_get_text(url_entry);

        if (NULL == name || NULL == url || strlen(name) == 0 || strlen(url) == 0)
            return;

        juniper_bookmarks_add(url, name);
    }

    gtk_widget_hide(GTK_WIDGET(add_bookmark_dialog));    
}

void juniper_bookmarks_menu_item_activate(GtkMenuItem * menu_item, const gchar * url)
{
    juniper_tabs_add_with_location(url);
}

void juniper_bookmarks_add(const gchar * uri, const gchar * name)
{
    sqlite3_stmt * statement;
    int result;

#ifdef DEBUG
    printf("adding bookmark: %s (%s)\n", name, uri);
#endif

    sqlite3_prepare_v2(db_handle, "insert into bookmarks (name, uri) values (?, ?)", -1, &statement, NULL);
    sqlite3_bind_text(statement, 1, name, strlen(name), SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, uri, strlen(uri), SQLITE_STATIC);

    result = sqlite3_step(statement);

#ifdef DEBUG
    if (result != SQLITE_DONE)
        printf("got strange sqlite result: %i (errmsg: %s)\n", result, sqlite3_errmsg(db_handle));
#endif

    sqlite3_finalize(statement);

    juniper_bookmarks_reload();
}
