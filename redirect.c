#include "redirect.h"

extern lld inv, outv, addv;
extern char *in[inf], *out[inf], *add[inf], *fincmd;

char *strbrk(char *str, const char *delim)
{
	static char *next;
	char *tok, *tmp;

	if(!delim) 
		return NULL;

	tok = (str) ? str : next;
	if(!tok)
		return NULL;

	tmp = strstr(tok, delim);
	if(tmp) 
	{
		next = tmp + strlen(delim);
		*tmp = '\0';
	} 

	else 
		next = NULL;

	return tok;
}

char *op(char *txt)
{
	char *comp[inf], *toke[inf], *ret = (char *)malloc(inf * sizeof(char));
	lld i, j, k;

	comp[0] = strtok(txt, ">");
	if(!comp[0])
		return NULL;
	for(i = 1; (comp[i] = strtok(NULL, ">")); ++i);

	for(j = 0; j < i; ++j)
	{
		if(!inv && addv == -1 && outv == -1 && !j)
		{
			fincmd = (char *)malloc(sizeof(char) * strlen(comp[j]));
			strcpy(fincmd, comp[j]);
		}

		toke[0] = strtok(comp[j], " ");
		if(!toke[0])
			continue;
		for(k = 1; (toke[k] = strtok(NULL, " ")); ++k);

		if(!j)
			strcpy(ret, toke[k - 1]);
		else
		{
			out[++outv] = (char *)malloc(inf * sizeof(char));
			strcpy(out[outv], toke[k - 1]);
		}
	}

	return ret;
}

char *app(char *part)
{
	char *ap[inf], *ret = (char *)malloc(inf * sizeof(char)), *temp = (char *)malloc(inf * sizeof(char));
	lld i, j;

	ap[0] = strbrk(part, ">>");
	if(!ap[0])
		return NULL;
	for(i = 1; (ap[i] = strbrk(NULL, ">>")); ++i);

	for(j = 0; j < i; ++j)
	{
		lld tmp = outv + 1;
		if(j)
			++addv;
		strcpy(temp, op(ap[j]));

		if(j)
		{
			add[addv] = (char *)malloc(inf * sizeof(char));
			strcpy(add[addv], temp);
		}
		else
			strcpy(ret, temp);
	}

	return ret;
}

lld inp(char *cmd)
{
	char *rd[inf];
	lld i, j;

	rd[0] = strtok(cmd, "<");
	if(!rd[0])
		return -1;
	for(i = 1; (rd[i] = strtok(NULL, "<")); ++i);

	for(j = 0; j < i; ++j)
	{
		in[++inv] = (char *)malloc(inf * sizeof(char));
		strcpy(in[inv], app(rd[j]));
	}

	return 0;
}

lld redirect(char *cmd)
{
	inv = outv = addv = -1;
	inp(cmd);
	return 0;
}
