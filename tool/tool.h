#ifndef FILE_TOOL_H
#define FILE_TOOL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../log/log.h"

#define DIR_PATH_LENGTH 1000  /* dir path length */

#define MAXLINE 4096  /* max line length */

/* error related function */
void err_ret(const char *fmt, ...);
void err_sys(const char *fmt, ...);
void err_cont(int error, const char *fmt, ...);
void err_exit(int error, const char *fmt, ...);
void err_dump(const char *fmt, ...);
void err_msg(const char *fmt, ...);
void err_quit(const char *fmt, ...);

/* file tool related function */
void handleMatchFiles(const char *dir, int searchSubdirectories, const char *pattern,
                      void (*handleCallBack)(const char *path, const char *fileName));
void moveMatchFile(const char *destDir, const char *srcDir, const char *fileName);
void copyMatchFile(const char *destDir, const char *srcDir, const char *fileName);
int isExists(const char *dir);

/* common function */
char *mergePath(char *dest, const char *filePath, const char *fileName);
char *searchstr(const char **substrs, const char *targetstr);


#endif
