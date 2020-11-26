#include "headers.h"
#include "cmdList.h"
#include "sighandle.h"

#ifndef __REDIRECT_H__
#define __REDIRECT_H__

char *strbrk(char *str, const char *delim);
char * op(char *txt);
char * app(char *part);
lld inp(char *cmd);
lld redirect(char *cmd);

#endif
