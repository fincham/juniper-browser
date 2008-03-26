#include "juniper-db.h"

#include "juniper-fs.h"

#define DB_FILE "juniper.db"

static sqlite3 * db_handle;

gboolean juniper_db_init()
{
    return (sqlite3_open(juniper_fs_build_filename(DB_FILE), &db_handle) == SQLITE_OK);
}

sqlite3 * juniper_db_get_handle()
{
    return db_handle;
}
