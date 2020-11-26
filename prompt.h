#include "headers.h"
#ifndef __PROMPT_H__
#define __PROMPT_H__

struct job
{
	pid_t pid;
	char *name;
};

char *promptDets(char *home, lld exstat);
void prompt();

#endif
