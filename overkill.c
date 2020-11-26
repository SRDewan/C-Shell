#include "overkill.h"

lld overkill(lld proc[], struct job *list[], lld *max)
{
	lld ret = 0;
	for(lld i = 1; i <= *max; ++i)
	{
		if(!list[i])
			continue;

		else if(kill(list[i]->pid, SIGKILL))
		{
			perror("overkill - kill: Error");
			ret = -1;
		}

		else 
		{
			proc[list[i]->pid] = 0;
			if(*max == i)
				for(*max = *max - 1; *max > 0 && !list[*max]; --*max);
			free(list[i]);
			list[i] = NULL;
		}	
	}

	return ret;
}
