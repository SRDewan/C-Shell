#include "kjob.h"

extern struct job *jlist[inf];
extern lld max, proc[inf * 10];

lld kjob(lld *k, char *token[inf])
{
	if(*k != 3)
	{
		printf("kjob: Error: Incorrect number of arguments to kjob\n");
		return -1;
	}

	else if(numcheck(token[1]))
	{
		printf("kjob - numcheck: Error: Invalid argument '%s' to kjob\n", token[1]);
		return -1;
	}

	else if(numcheck(token[2]))
	{
		printf("kjob - numcheck: Error: Invalid argument '%s' to kjob\n", token[2]);
		return -1;
	}

	else if(!jlist[atoll(token[1])])
	{
		printf("kjob - jobno: Error: '%s' is not a valid job number\n", token[1]);
		return -1;
	}

	else 
	{
		lld jno = atoll(token[1]);
		lld sig = atoll(token[2]);

		if(kill(jlist[jno]->pid, sig))
		{
			perror("kjob - kill: Error");
			return -1;
		}

		else 
		{
			proc[jlist[jno]->pid] = 0;
			if(max == jno)
				for(max = max - 1; max > 0 && !jlist[max]; --max);
			free(jlist[jno]);
			jlist[jno] = NULL;
		}	

	}

	return 0;
}
