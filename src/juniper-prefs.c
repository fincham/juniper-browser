#include "juniper-prefs.h"

#include "assert.h"
#include "string.h"
#include "yaml.h"

#include "juniper-fs.h"

#define PREFS_FILE "preferences"

#define PREF_HOMEPAGE                 1
#define PREF_OPEN_HOMEPAGE_ON_NEW_TAB 2

#define PREF_HOMEPAGE_KEY                 "homepage"
#define PREF_OPEN_HOMEPAGE_ON_NEW_TAB_KEY "open_homepage_on_new_tab"

#define PREF_HOMEPAGE_MAX_LEN 1024

static gchar * homepage = NULL;
static gboolean open_homepage_on_new_tab;

void juniper_prefs_init()
{
    char * scalar_data;
    FILE * prefs_file;
    yaml_parser_t parser;
    yaml_event_t event;
    int current_mapping_key = 0;

    prefs_file = juniper_fs_open(PREFS_FILE, "a+");

    if (prefs_file == NULL)
    {
        puts("Error opening preferences file");
        return;
    }

    memset(&parser, 0, sizeof(parser));
    memset(&event, 0, sizeof(event));

    assert(yaml_parser_initialize(&parser));
    yaml_parser_set_input_file(&parser, prefs_file);
    
    while (1)
    {
        if (!yaml_parser_parse(&parser, &event))
        {
            puts("Error parsing preferences file");
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

                if (current_mapping_key == 0)
                {
                    if (strcmp(scalar_data, PREF_HOMEPAGE_KEY) == 0)
                    {
                        current_mapping_key = PREF_HOMEPAGE;
                    }
                    else if (strcmp(scalar_data, PREF_OPEN_HOMEPAGE_ON_NEW_TAB_KEY) == 0)
                    {
                        current_mapping_key = PREF_OPEN_HOMEPAGE_ON_NEW_TAB;
                    }
                }
                else if (current_mapping_key == PREF_HOMEPAGE)
                {
                    size_t homepage_len = strlen(scalar_data);
                    if (homepage_len > PREF_HOMEPAGE_MAX_LEN)
                        break;
                    homepage = malloc(homepage_len);
                    strcpy(homepage, scalar_data);
                    current_mapping_key = 0;
                }
                else if (current_mapping_key == PREF_OPEN_HOMEPAGE_ON_NEW_TAB)
                {
                    open_homepage_on_new_tab = (strcasecmp(scalar_data, "true") == 0);
                    current_mapping_key = 0;
                }

                break;

            default:
                ;
        }
    }

    yaml_event_delete(&event);
    yaml_parser_delete(&parser);
    fclose(prefs_file);
}

gchar * juniper_prefs_get_homepage()
{
    return homepage;
}

gboolean juniper_prefs_get_open_homepage_on_new_tab()
{
    return open_homepage_on_new_tab;
}
