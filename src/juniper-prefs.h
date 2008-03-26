#ifndef JUNIPER_PREFS
#define JUNIPER_PREFS

#include "glib.h"

gboolean juniper_prefs_init();
const gchar * juniper_prefs_get(const gchar * key);
gboolean juniper_prefs_set(const gchar * key, const gchar * value);

#endif
