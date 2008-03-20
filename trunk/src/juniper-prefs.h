#ifndef JUNIPER_PREFS
#define JUNIPER_PREFS

#include "gtk/gtk.h"

gboolean juniper_prefs_init();

gpointer juniper_prefs_get(const gchar * key);
void juniper_prefs_set(const gchar * key, const gpointer value);

#endif
