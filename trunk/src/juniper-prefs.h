#ifndef JUNIPER_PREFS
#define JUNIPER_PREFS

#include "gtk/gtk.h"

gboolean juniper_prefs_init();
gchar * juniper_prefs_get_homepage();
gboolean juniper_prefs_get_open_homepage_on_new_tab();

#endif
