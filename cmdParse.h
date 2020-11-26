#include "headers.h"
#include "sighandle.h"
#include "history.h"
#include "prompt.h"
#include "redirect.h"
#include "util.h"
#include "nightswatch.h"
#include "cmdList.h"

#ifndef __CMDPARSE_H__
#define __CMDPARSE_H__

lld jobinit(pid_t child, char *bgcmd);
void childsig(int sig);
void parsig(int signo);
lld parse(char *cmd, char *home, lld *exstat);

#endif
