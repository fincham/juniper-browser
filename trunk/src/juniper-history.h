#ifndef JUNIPER_HISTORY
#define JUNIPER_HISTORY

#include "glib.h"

typedef struct _JuniperHistoryItem JuniperHistoryItem;

struct _JuniperHistoryItem
{
    const gchar * uri;
    const gchar * title;
};

gboolean juniper_history_init();
gboolean juniper_history_add(const gchar * uri, const gchar * title);
GSList * juniper_history_matches(const gchar * uri);

#endif
