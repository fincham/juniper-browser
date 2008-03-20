#include "juniper-util.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define SPRINTF_MAX_STRLEN 4096
#define MAX_URL_LENGTH 1024

gchar * juniper_util_sprintf(const gchar * format_str, ...)
{
    gchar * result;
    va_list ap;

    va_start(ap, format_str);

    result = malloc(SPRINTF_MAX_STRLEN);
    vsnprintf(result, SPRINTF_MAX_STRLEN, format_str, ap);

    return result;
}

gchar * juniper_util_normalise_url(const gchar * url)
{
    if (strstr(url, "://") == NULL)
    {
        gchar * normalised_url;

        if (strlen(url) + 7 > MAX_URL_LENGTH)
            return NULL;

        normalised_url = malloc(strlen(url)+7);
        sprintf(normalised_url, "%s%s", "http://", url);

        return normalised_url;
    }
    else
        return g_strdup(url);
}
