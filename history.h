#include "headers.h"
#ifndef __HISTORY_H__
#define __HISTORY_H__

lld history_rd(char *past[], char *path);
lld history_up(char *cmd, char *home);
lld hist(lld *j, char *token[], char *home);

#endif
