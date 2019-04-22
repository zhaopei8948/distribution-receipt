#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "paropt.h"
#include "../log/log.h"

char srcdir[CHAR_BUFF_LENGTH];
char backupdir[CHAR_BUFF_LENGTH];
char inputdir[CHAR_BUFF_LENGTH];
char exportdir[CHAR_BUFF_LENGTH];
char faildir[CHAR_BUFF_LENGTH];
uint32_t delaytime = 0;
int loglevel = 0;

/* private function declarations */
static void help(const char *restrict cmdName);
static void calculationDelayTime(const char *delayTimeStr);
static int checkOption();

static int checkOption()
{
    if (strlen(srcdir) == 0) {
        log_error("srcdir is required, Please add it!");
        return -1;
    }

    if (strlen(inputdir) == 0) {
        log_error("inputdir is required, Please add it!");
        return -1;
    }

    if (strlen(exportdir) == 0) {
        log_error("exportdir is required, Please add it!");
        return -1;
    }

    if (delaytime == 0) {
        log_error("delaytime value is unreasonable. Please try!");
        return -1;
    }

    if (loglevel > 5 || loglevel < 0) {
        log_error("loglevel vaue is incorrect, correct range 0-5, Please try!");
        return -1;
    }

    return 0;
}

static void help(const char *restrict cmdName)
{
    printf("Usage: %s [OPTION]\n\n", cmdName);
    printf("    -s --srcdir <value>                 source dir\n");
    printf("    -b --backupdir <value>              backup dir\n");
    printf("    -i --inputdir <value>               input dir\n");
    printf("    -e --exportdir <value>              export dir\n");
    printf("    -f --faildir <value>                verification fail dir\n");
    printf("    -t --delaytime <time>               time str descrption 1s,1m,1h,1d...\n");
    printf("    -l --loglevel <value>               log level,0:TRACE, 1:DEBUG, 2:INFO, 3:WARN, 4:ERROR, 5:FATAL\n");
    printf("    -h --help                           help\n");
}

static void calculationDelayTime(const char *delayTimeStr)
{
    if (NULL == delayTimeStr)
        return;
    size_t length = strlen(delayTimeStr);
    char timeUnit = delayTimeStr[length - 1];
    char *time = malloc(length * sizeof(char));
    strcpy(time, delayTimeStr);
    time[length - 1] = '\0';

    switch (timeUnit) {
    case 's':
        delaytime = atoi(time) * 1000;
        break;
    case 'm':
        delaytime = atoi(time) * 60 * 1000;
        break;
    case 'h':
        delaytime = atoi(time) * 60 * 60 * 1000;
        break;
    default:
        log_error("delaytime format error");
    }

    free(time);
}

int parseOption(int argc, char **argv)
{
    char *cmd = argv[0];
    int result = 0;
    if (argc == 1) {
        help(cmd);
        return -1;
    }
    while (1) {
        int option_index = 0;
        struct option long_options[] = {
                                        {"srcdir"                 , required_argument, 0,   's'},
                                        {"backupdir"              , required_argument, 0,   'b'},
                                        {"inputdir"               , required_argument, 0,   'i'},
                                        {"exportdir"              , required_argument, 0,   'e'},
                                        {"faildir"                , required_argument, 0,   'f'},
                                        {"delaytime"              , required_argument, 0,   't'},
                                        {"loglevel"               , required_argument, 0,   'l'},
                                        {"help"                   , no_argument,       0,   'h'},
                                        {NULL                     , 0,                 NULL, 0}
        };
        int c;

        /*注意这里的冒号，有冒号就需要加参数值，没有冒号就不用加参数值*/
        c = getopt_long(argc, argv, "s:b:i:e:f:t:l:h",long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 's':
            strcpy(srcdir, optarg);
            break;
        case 'b':
            strcpy(backupdir, optarg);
            break;
        case 'i':
            strcpy(inputdir, optarg);
            break;
        case 'e':
            strcpy(exportdir, optarg);
            break;
        case 'f':
            strcpy(faildir, optarg);
            break;
        case 't':
            calculationDelayTime(optarg);
            break;
        case 'l':
            loglevel = atoi(optarg);
            break;
        case 'h':
        default:
            result = -1;
            help(cmd);
            break;
        }
    }

    return result == -1 ? -1 : checkOption();
}
