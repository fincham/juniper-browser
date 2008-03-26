#ifndef JUNIPER_DB
#define JUNIPER_DB

#include "glib.h"
#include "sqlite3.h"

gboolean juniper_db_init();
sqlite3 * juniper_db_get_handle();

#endif
