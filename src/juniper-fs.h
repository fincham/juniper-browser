#ifndef JUNIPER_FS
#define JUNIPER_FS

#include "gtk/gtk.h"
#include "stdio.h"

FILE * juniper_fs_open(const gchar * filename, const gchar * mode);

#endif
