#include "juniper-util.h"

#include "stdio.h"
#include "stdlib.h"

#define SPRINTF_MAX_STRLEN 4096

gchar * juniper_util_sprintf(gchar * format_str, ...)
{
    gchar * result;
    va_list ap;

    va_start(ap, format_str);

    result = malloc(SPRINTF_MAX_STRLEN);
    vsnprintf(result, SPRINTF_MAX_STRLEN, format_str, ap);

    return result;
}
