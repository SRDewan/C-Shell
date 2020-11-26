#include "bg.h"

extern struct job *jlist[inf];
extern lld max, proc[inf * 10], flag;

lld bg(lld *k, char *token[inf])
{
	if(flag)
	{
		printf("bg: No job control in this shell\n");
		return 0;
	}

	if(*k != 2)
	{
		printf("bg: Error: Incorrect number of arguments to bg\n");
		return -1;
	}

	else if(numcheck(token[1]))
	{
		printf("bg - numcheck: Error: Invalid argument '%s' to bg\n", token[1]);
		return -1;
	}

	else if(!jlist[atoll(token[1])])
	{
		printf("bg - jobno: Error: '%s' is not a valid job number\n", token[1]);
		return -1;
	}

	else 
	{
		lld jno = atoll(token[1]);
		if(kill(jlist[jno]->pid, SIGCONT))
		{
			perror("bg - continue: Error");
			return -1;
		}
	}

	return 0;
}
