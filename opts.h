#include "headers.h"
#ifndef __OPTS_H__
#define __OPTS_H__

struct op
{
	char letter;
	lld arg, flag, *arglist;
};

lld args(lld i, lld *j, char *token[], struct op *elem);
lld opts(lld arr[], lld *j, char *token[], struct op all[], lld sz);

#endif
