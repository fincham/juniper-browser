#ifndef JUNIPER_UTIL
#define JUNIPER_UTIL

#include "glib.h"

gchar * juniper_util_sprintf(const gchar * format_str, ...);
gchar * juniper_util_normalise_url(const gchar * url);
gchar * juniper_util_truncate(const gchar * string, guint max_length);

#endif
