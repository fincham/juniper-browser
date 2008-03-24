#include "juniper-util.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define SPRINTF_MAX_STRLEN 4096
#define MAX_URL_LENGTH 4000

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
#ifdef DEBUG
        printf("URL needs normalising: %s\n", url);
#endif
        if (strlen(url) + 7 > MAX_URL_LENGTH)
        {
#ifdef DEBUG
            puts("Can't normalise - result would exceed MAX_URL_LENGTH");
#endif
            return NULL;
        }

        if (strstr(url, " ") != NULL)
        {
            /* user is probably wanting to search for something. hardcoded google for now, sorry! */
            return juniper_util_sprintf("http://www.google.co.nz/search?q=%s&ie=utf-8&oe=utf-8&client=juniper", url);
        }

        return juniper_util_sprintf("http://%s", url);
    }
    else
        return g_strdup(url);
}
