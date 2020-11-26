#include "headers.h"
#include "cd.h"
#include "ls.h"
#include "echos.h"
#include "path.h"
#include "pinfo.h"
#include "history.h"
#include "newborn.h"
#include "interrupt.h"
#include "jobs.h"
#include "kjob.h"
#include "fg.h"
#include "bg.h"
#include "overkill.h"
#include "env.h"
#include "nightswatch.h"

#ifndef __CMDLIST_H__
#define __CMDLIST_H__

lld callcmd(char *token[inf], char *home, lld *m, lld start, lld period);

#endif
