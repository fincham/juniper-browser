#include "juniper-fs.h"

#include "errno.h"
#include "string.h"
#include "stdlib.h"
#include "sys/stat.h"

#define ENV_HOME       "HOME"
#define JUNIPER_FOLDER ".juniper"
#define PATH_SEPARATOR "/"

FILE * juniper_fs_open(const gchar * filename, const gchar * mode) {
    char * home, juniper_folder[FILENAME_MAX], full_filename[FILENAME_MAX];
    int mkdir_result;

    home = getenv(ENV_HOME);
    snprintf(juniper_folder, FILENAME_MAX, "%s%s%s", home, PATH_SEPARATOR, JUNIPER_FOLDER);

    mkdir_result = mkdir(juniper_folder, 0500);
    if (!(mkdir_result == 0 || errno == EEXIST))
    {
        printf("Failed to create %s", juniper_folder);
        return NULL;
    }

    snprintf(full_filename, FILENAME_MAX, "%s%s%s", juniper_folder, PATH_SEPARATOR, filename);

    return fopen(full_filename, mode);
}
