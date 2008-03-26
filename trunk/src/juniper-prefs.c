#include "juniper-prefs.h"

#include "assert.h"
#include "string.h"
#include "sqlite3.h"

#include "juniper-db.h"
#include "juniper-fs.h"

static sqlite3 * db_handle;

gboolean juniper_prefs_init()
{
    db_handle = juniper_db_get_handle();
    assert(db_handle != NULL);

    sqlite3_exec(db_handle, "create table preferences (key varchar(255) not null unique, value varchar (255) not null)", NULL, NULL, NULL);
    return TRUE;
}

const gchar * juniper_prefs_get(const gchar * key)
{
    sqlite3_stmt * statement;
    gchar * value;

    sqlite3_prepare_v2(db_handle, "select key, value from preferences where key=?", -1, &statement, NULL);
    sqlite3_bind_text(statement, 1, key, strlen(key), SQLITE_STATIC);

    if (sqlite3_step(statement) != SQLITE_ROW)
        return NULL;

    value = g_strdup((gchar *) sqlite3_column_text(statement, 1));

    sqlite3_finalize(statement);

    return value;
}

gboolean juniper_prefs_set(const gchar * key, const gchar * value)
{
    sqlite3_stmt * statement;

    sqlite3_prepare_v2(db_handle, "insert into preferences (key, value) values(?, ?)", -1, &statement, NULL);
    sqlite3_bind_text(statement, 1, key, strlen(key), SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, value, strlen(value), SQLITE_STATIC);

    if (sqlite3_step(statement) != SQLITE_DONE)
        return FALSE;

    sqlite3_finalize(statement);

    return TRUE;
}
