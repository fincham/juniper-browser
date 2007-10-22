#include "juniper-prefs.h"

#include "assert.h"
#include "string.h"
#include "yaml.h"

#include "juniper-fs.h"
#include "juniper-tabs.h"

#define BOOKMARKS_FILE "bookmarks"

static GHashTable * bookmarks;

void juniper_bookmarks_menu_item_activate(GtkMenuItem * menu_item, gchar * url)
{
    juniper_tabs_add_with_location(url);
}

void juniper_bookmarks_init(GtkMenu * bookmarks_menu)
{
    char * scalar_data, * current_mapping_key = NULL;
    FILE * bookmarks_file;
    yaml_parser_t parser;
    yaml_event_t event;
    GtkMenuItem * menu_item;

    bookmarks = g_hash_table_new(g_str_hash, g_str_equal);

    bookmarks_file = juniper_fs_open(BOOKMARKS_FILE, "a+");

    if (bookmarks_file == NULL)
    {
        puts("Error opening bookmarks file");
        return;
    }

    memset(&parser, 0, sizeof(parser));
    memset(&event, 0, sizeof(event));

    assert(yaml_parser_initialize(&parser));
    yaml_parser_set_input_file(&parser, bookmarks_file);
    
    while (1)
    {
        if (!yaml_parser_parse(&parser, &event))
        {
            puts("Error parsing bookmarks file");
            break;
        }

        if (event.type == YAML_STREAM_END_EVENT)
        {
            break;
        }

        switch (event.type)
        {
            case YAML_SCALAR_EVENT:

                scalar_data = (char *) event.data.scalar.value;

                if (strcmp(scalar_data, "bookmarks") == 0)
                    break;

                if (current_mapping_key == NULL)
                {
                    current_mapping_key = scalar_data;
                }
                else
                {
                    g_hash_table_insert(bookmarks, current_mapping_key, scalar_data);

                    menu_item = GTK_MENU_ITEM(gtk_menu_item_new_with_label(current_mapping_key));
                    g_signal_connect(menu_item, "activate", G_CALLBACK(juniper_bookmarks_menu_item_activate), scalar_data);

                    gtk_menu_shell_append(GTK_MENU_SHELL(bookmarks_menu), GTK_WIDGET(menu_item));
                    gtk_widget_show(GTK_WIDGET(menu_item));

                    current_mapping_key = NULL;
                }

                break;

            default:
                ;
        }

    }

    yaml_event_delete(&event);
    yaml_parser_delete(&parser);
    fclose(bookmarks_file);
}

gchar * juniper_bookmarks_get(const gchar * title)
{
    return (gchar *) g_hash_table_lookup(bookmarks, title);
}

void juniper_bookmarks_add(gchar * title, gchar * url)
{
    g_hash_table_insert(bookmarks, title, url);
}
