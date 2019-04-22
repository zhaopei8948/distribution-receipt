#ifndef PAROPT_H
#define PAROPT_H

#include <stdint.h>

#define CHAR_BUFF_LENGTH 1000

extern char srcdir[CHAR_BUFF_LENGTH];
extern char backupdir[CHAR_BUFF_LENGTH];
extern char inputdir[CHAR_BUFF_LENGTH];
extern char exportdir[CHAR_BUFF_LENGTH];
extern char faildir[CHAR_BUFF_LENGTH];
extern uint32_t delaytime;
extern int loglevel;

/* public function declarations*/
int parseOption(int argc, char **argv);

#endif
