#include "juniper-history.h"

#include "assert.h"
#include "stdio.h"
#include "string.h"
#include "sqlite3.h"

#include "juniper-db.h"
#include "juniper-util.h"

static sqlite3 * db_handle;

gboolean juniper_history_init()
{
    db_handle = juniper_db_get_handle();
    assert(db_handle != NULL);

    sqlite3_exec(db_handle, "create table history (uri varchar(255) not null unique, title varchar (255) not null, last_visited timestamp not null)", NULL, NULL, NULL);
    return TRUE;
}

gboolean juniper_history_add(const gchar * uri, const gchar * title)
{
    sqlite3_stmt * statement;
    int result;

    if (uri == NULL || title == NULL)
        return FALSE;

#ifdef DEBUG
    printf("adding history item: uri=%s, title=%s\n", uri, title);
#endif

    sqlite3_prepare_v2(db_handle, "insert or replace into history (uri, title, last_visited) values(?, ?, strftime('%s', 'now'))", -1, &statement, NULL);
    sqlite3_bind_text(statement, 1, uri, strlen(uri), SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, title, strlen(title), SQLITE_STATIC);

    result = sqlite3_step(statement);
    if (result != SQLITE_DONE)
    {
#ifdef DEBUG
        printf("got strange sqlite result: %i (errmsg: %s)\n", result, sqlite3_errmsg(db_handle));
#endif
        return FALSE;
    }

    sqlite3_finalize(statement);

    return TRUE;
}

GSList * juniper_history_matches(const gchar * query)
{
    sqlite3_stmt * statement;
    JuniperHistoryItem * history_item;
    GSList * matches;

    sqlite3_prepare_v2(db_handle, "select uri, title, last_visited from history where uri like ?1 or title like ?1", -1, &statement, NULL);

    sqlite3_bind_text(statement, 1, juniper_util_sprintf("%%%s%%", query), strlen(query) + 2, g_free);

    matches = NULL;

    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        history_item = g_new(JuniperHistoryItem, 1);
        history_item->uri = g_strdup((gchar *) sqlite3_column_text(statement, 0));
        history_item->title = g_strdup((gchar *) sqlite3_column_text(statement, 1));

#ifdef DEBUG
        printf("history match: q=%s, uri=%s, title=%s\n", query, history_item->uri, history_item->title);
#endif

        matches = g_slist_prepend(matches, history_item);
    }

    sqlite3_finalize(statement);

    matches = g_slist_reverse(matches);
    return matches;
}
