#include "headers.h"
#ifndef __LS_L_H__
#define __LS_L_H__ 

struct row
{
	char name[inf], per[10]; 
	lld links;
	char user[inf], grp[inf];
	lld sz;
	char timestamp[20];
	lld block;
};

lld perm(struct row *entry, struct stat entname);
lld lsl(struct row *entry, char *entname);

#endif

