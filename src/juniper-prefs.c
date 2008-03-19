#include "juniper-prefs.h"

#include "assert.h"
#include "string.h"
#include "libxml/parser.h"
#include "libxml/tree.h"

#include "juniper-fs.h"

#define PREFS_FILE "preferences.xml"

#define PREF_HOMEPAGE_KEY                 "homepage"
#define PREF_OPEN_HOMEPAGE_ON_NEW_TAB_KEY "open_homepage_on_new_tab"

#define PREF_HOMEPAGE_MAX_LEN 1024

static GHashTable * prefs_table = NULL;

gboolean juniper_prefs_init()
{
    xmlDoc * doc = NULL;
    xmlNode * root_element = NULL;
    xmlNode * prefs_element = NULL;

    LIBXML_TEST_VERSION

    doc = xmlReadFile(juniper_fs_build_filename(PREFS_FILE), NULL, 0);

    if (doc == NULL)
    {
        puts("Error reading preferences file");
        return FALSE;
    }

    root_element = xmlDocGetRootElement(doc);

    prefs_table = g_hash_table_new(g_str_hash, g_str_equal);

    for (prefs_element = root_element->children; prefs_element; prefs_element = prefs_element->next)
    {
        if (prefs_element->type == XML_ELEMENT_NODE)
        {
            g_hash_table_insert(prefs_table, (gchar *) prefs_element->name, prefs_element->content);
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return TRUE;
}

gchar * juniper_prefs_get_homepage()
{
    return g_hash_table_lookup(prefs_table, PREF_HOMEPAGE_KEY);
}

gboolean juniper_prefs_get_open_homepage_on_new_tab()
{
    g_hash_table_lookup(prefs_table, PREF_OPEN_HOMEPAGE_ON_NEW_TAB_KEY);
    return FALSE; // TODO
}
