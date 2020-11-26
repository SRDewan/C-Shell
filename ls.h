#include "headers.h"
#include "ls_l.h"
#ifndef __LS_H__
#define __LS_H__

int cmp(const void *a, const void *b);
int len(lld num);
lld print(struct row entity, lld max[4]);
lld list(char *dirname, struct row entity[inf]);
lld ls(lld *j, char *token[], char *home);

#endif
