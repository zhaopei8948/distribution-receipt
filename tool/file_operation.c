#include <windows.h>
#include "tool.h"

void handleMatchFiles(const char *dir, int searchSubdirectories, const char *pattern,
                      void (*handleCallBack)(const char *path, const char *fileName))
{
    HANDLE hFind;
    WIN32_FIND_DATA findData;
    char dirNew[DIR_PATH_LENGTH];
    log_debug("dir is: %s", dir);

    strcpy(dirNew, dir);
    strcat(dirNew, "\\");
    strcat(dirNew, pattern);
    hFind = FindFirstFile(dirNew, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        log_error("Failed to find first file!");
        return;
    }

    do {
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
            continue;
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            log_debug("%s \t<dir>", findData.cFileName);
            if (searchSubdirectories) {
                strcpy(dirNew, dir);
                strcat(dirNew, "\\");
                strcat(dirNew, findData.cFileName);
                handleMatchFiles(dirNew, 1, pattern, handleCallBack);
            }
        } else {
            log_debug("find file is: %s", findData.cFileName);
            handleCallBack(dir, findData.cFileName);
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
}

void moveMatchFile(const char *destDir, const char *srcFilePath, const char *fileName)
{
    char destFilePath[DIR_PATH_LENGTH];
    mergePath(destFilePath, destDir, fileName);
    if (MoveFile(srcFilePath, destFilePath)) {
        log_debug("move file %s --> %s success.", srcFilePath, destFilePath);
    } else {
        log_error("move file %s --> %s fail.", srcFilePath, destFilePath);
    }
}

void copyMatchFile(const char *destDir, const char *srcFilePath, const char *fileName)
{
    char destFilePath[DIR_PATH_LENGTH];
    mergePath(destFilePath, destDir, fileName);
    if (CopyFile(srcFilePath, destFilePath, FALSE)) {
        log_debug("copy file %s --> %s success.", srcFilePath, destFilePath);
    } else {
        log_error("copy file %s --> %s fail.", srcFilePath, destFilePath);
    }
}

int isExists(const char *dir)
{
    if (strlen(dir) == 0)
        return 0;
    HANDLE hFind;
    WIN32_FIND_DATA findData;
    hFind = FindFirstFile(dir, &findData);
    if (hFind != INVALID_HANDLE_VALUE && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        return 1;
    } else {
        return 0;
    }
    FindClose(hFind);
}
