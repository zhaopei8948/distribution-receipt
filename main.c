#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include "paropt/paropt.h"
#include "log/log.h"
#include "tool/tool.h"

#define ONE_TIME_READ_NUMBER 100

static FILE *LOGFILE;
static char *logFileName = "distribution-recept.log";
static const char *inputFlag[] = {
                           "<CEB312Message",
                           "<CEB412Message",
                           "<CEB512Message",
                           "<CEB514Message",
                           "<CEB622Message",
                           "<CEB624Message",
                           "<CEB626Message",
                           "<CEB712Message",
                           "<CEB816Message",
                           "<CEB818Message",
                           NULL
};
static const char *exportFlag[] = {
                            "<CEB304Message",
                            "<CEB404Message",
                            "<CEB506Message",
                            "<CEB508Message",
                            "<CEB604Message",
                            "<CEB606Message",
                            "<CEB510Message",
                            "<CEB608Message",
                            "<CEB702Message",
                            "<CEB792Message",
                            NULL

};
static char *verificationFail = "<CEB900Message";

/* function declare */
static void stopApplication(int signal);
static void handleFile(const char *path, const char *fileName);
static void Timeproc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);

static void stopApplication(int signal)
{
    log_debug("handle SIGINT signal");
    if (LOGFILE) {
        fclose(LOGFILE);
    }
    exit(0);
}

static void handleFile(const char *path, const char *fileName)
{
    char filePath[DIR_PATH_LENGTH];
    char buff[ONE_TIME_READ_NUMBER];
    FILE *fp;
    mergePath(filePath, path, fileName);

    fp = fopen(filePath, "r");
    if (!fp) {
        log_error("%s file open error.", filePath);
        return;
    }

    size_t ret_size = fread(buff, sizeof(char), ONE_TIME_READ_NUMBER, fp);
    if (ret_size == ONE_TIME_READ_NUMBER) {
        buff[ONE_TIME_READ_NUMBER - 1] = '\0';
        log_debug("buff is %s", buff);
        fclose(fp);

        /* if backupdir exists then backup file */
        if (isExists(backupdir))
            copyMatchFile(backupdir, filePath, fileName);

        /* input */
        char *result = searchstr(inputFlag, buff);
        log_debug("input searchstr result %s", result);
        if (result) {
            moveMatchFile(inputdir, filePath, fileName);
            return;
        }

        /* export */
        result = searchstr(exportFlag, buff);
        log_debug("export searchstr result %s", result);
        if (result) {
            moveMatchFile(exportdir, filePath, fileName);
            return;
        }

        /* fail */
        result = strstr(buff, verificationFail);
        log_debug("verification searchstr result %s", result);
        if (result) {
            if (isExists(faildir))
                moveMatchFile(faildir, filePath, fileName);
            return;
        }

        /* no match */
        log_error("no match.");
        if (DeleteFile(filePath)) {
            log_debug("delete file %s success.", filePath);
        } else {
            log_debug("delete file %s error.", filePath);
        }
    } else {
        fclose(fp);
        log_error("%s file read error.", filePath);
        return;
    }
}

static void Timeproc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
    /* char mbstr[100]; */
    /* time_t now = time(NULL); */

    /* strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", localtime(&now)); */
    log_debug("time reaching ...");
    handleMatchFiles(srcdir, 0, "*.xml", &handleFile);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, stopApplication);
    LOGFILE = fopen(logFileName, "a");
    if (!LOGFILE) {
        LOGFILE = fopen(logFileName, "w");
        if (!LOGFILE) {
            err_sys("File opening failed");
        }
    }
    log_set_fp(LOGFILE);
    if (parseOption(argc, argv) == 0) {
        log_set_level(loglevel);
        log_debug("log...%d", argc);
        BOOL bRet;
        MSG msg;

        SetTimer(NULL, 0, delaytime, (TIMERPROC)Timeproc);

        while ((bRet= GetMessage(&msg, NULL, 0, 0)) != 0) {
            if (bRet == -1) {
            
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

    }
    fclose(LOGFILE);
    return 0;
}
