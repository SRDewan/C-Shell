#include "cmdList.h"

extern struct job *jlist[inf];
extern lld proc[inf * 10], max, flag, ctr, inv, pin;
extern char *in[inf];
extern int rpend;

lld callcmd(char *token[inf], char *home, lld *m, lld start, lld period)
{
	lld ret = 0;
	if(!strcmp(token[start], "cd"))
		ret = cd(m, token, home);

	else if(!strcmp(token[start], "setenv"))
		ret = senv(m, token + start);

	else if(!strcmp(token[start], "unsetenv"))
		ret = uenv(m, token + start);

	else if(!strcmp(token[start], "ls"))
		ret = ls(m, token + start, home);

	else if(!strcmp(token[start], "echo"))
		ret = echos(m, token + start);

	else if(!strcmp(token[start], "pwd"))
		printf("%s\n", pwd(home, 1));

	else if(!strcmp(token[start], "pinfo"))
		ret = pinfo(m, token + start);

	else if(!strcmp(token[start], "jobs"))
		ret = jobs(jlist, max);

	else if(!strcmp(token[start], "kjob"))
		ret = kjob(m, token + start);

	else if(!strcmp(token[start], "fg"))
		ret = fg(m, token + start);

	else if(!strcmp(token[start], "bg"))
		ret = bg(m, token + start);

	else if(!strcmp(token[start], "overkill"))
		ret = overkill(proc, jlist, &max);

	else if(!strcmp(token[start], "newborn"))
		ret = recproc(m, token + start);

	else if(!strcmp(token[start], "interrupt"))
		ret = inter(ctr);

	else if(!strcmp(token[start], "history"))
		ret = hist(m, token + start, home);

	else if(!strcmp(token[start], "quit"))
	{
		if(overkill(proc, jlist, &max))
			ret = -1;
		if(kill(getppid(), SIGTERM) == -1)
		{
			perror("cmdList - exit parent: Error");
			ret = -1;
		}
		_exit(0);
	}

	else
	{
		for(lld var = 1; *m == 1 && var <= inv; ++var)
		{
			token[*m + var - 1] = (char *)malloc(inf * sizeof(char));
			strcpy(token[*m + var - 1], in[var]);
		}
		token[*m + inv] = NULL;

		if(execvp(token[start], token + start) == -1)
		{
			ret = -1;
			printf("cmdList - exec %s: Error: Command not found\n", token[start]);
			perror("cmdList - exec: Error"); 
		}
	}

	return ret;
}
