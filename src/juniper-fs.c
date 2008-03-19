#include "juniper-fs.h"

#include "errno.h"
#include "string.h"
#include "stdlib.h"
#include "sys/stat.h"

#define ENV_HOME            "HOME"
#define JUNIPER_FOLDER      ".juniper"
#define JUNIPER_FOLDER_MODE 0700
#define PATH_SEPARATOR      "/"

gchar * juniper_fs_build_filename(const gchar * filename)
{
    char * home, juniper_folder[FILENAME_MAX], full_filename[FILENAME_MAX];
    int mkdir_result;

    home = getenv(ENV_HOME);
    snprintf(juniper_folder, FILENAME_MAX, "%s%s%s", home, PATH_SEPARATOR, JUNIPER_FOLDER);

    mkdir_result = mkdir(juniper_folder, JUNIPER_FOLDER_MODE);
    if (!(mkdir_result == 0 || errno == EEXIST))
    {
        printf("Failed to create %s", juniper_folder);
        return NULL;
    }

    snprintf(full_filename, FILENAME_MAX, "%s%s%s", juniper_folder, PATH_SEPARATOR, filename);

    return full_filename;
}
