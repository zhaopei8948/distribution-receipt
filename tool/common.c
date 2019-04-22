#include "tool.h"

char *mergePath(char *dest, const char *path, const char *fileName)
{
    strcpy(dest, path);
    strcat(dest, "\\");
    strcat(dest, fileName);
    return dest;
}

char *searchstr(const char **substrs, const char *targetstr)
{
    char *result;
    for (int i = 0; *(substrs + i) != NULL; i++) {
        log_debug("searchstr current %d, %s", i, *(substrs +i));
        if ((result = strstr(targetstr, *(substrs + i)))) {
            return result;
        }
    }

    return NULL;
}
