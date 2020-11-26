#include "fg.h"

extern struct job *jlist[inf];
extern lld max, proc[inf * 10], flag;

lld fg(lld *k, char *token[inf])
{
	if(flag)
	{
		printf("fg: No job control in this shell\n");
		return -1;
	}

	if(*k != 2)
	{
		printf("fg: Error: Incorrect number of arguments to fg\n");
		return -1;
	}

	else if(numcheck(token[1]))
	{
		printf("fg - numcheck: Error: Invalid argument '%s' to fg\n", token[1]);
		return -1;
	}

	else if(!jlist[atoll(token[1])])
	{
		printf("fg - jobno: Error: '%s' is not a valid job number\n", token[1]);
		return -1;
	}

	else 
	{
		lld jno = atoll(token[1]);
		int st;
		pid_t pid = jlist[jno]->pid;

		/*setpgid(pid, getpgid(getpid()));*/
		if(signal(SIGTTIN, SIG_IGN) == SIG_ERR)
			perror("fg - ignore op signal: Error");
		if(signal(SIGTTOU, SIG_IGN) == SIG_ERR)
			perror("fg - ignore op signal: Error");

		if(tcsetpgrp(STDIN_FILENO, getpgid(pid)) == -1)
		{
			perror("fg - set bgchild to fg: Error");
			return -1;
		}
		
		if(kill(pid, SIGCONT))
		{
			perror("fg - continue: Error");
			return -1;
		}

		if(waitpid(pid, &st, WUNTRACED) == -1)
			perror("fg - wait: Error");

		if(tcsetpgrp(STDIN_FILENO, getpgid(getpid())) == -1)
		{
			perror("fg - set 'fg' to fg: Error");
			return -1;
		}

		if(signal(SIGTTIN, SIG_IGN) == SIG_ERR)
			perror("fg - restore op signal: Error");
		if(signal(SIGTTOU, SIG_DFL) == SIG_ERR)
			perror("fg - restore op signal: Error");

		proc[jlist[jno]->pid] = 0;
		if(max == jno)
			for(max = max - 1; max > 0 && !jlist[max]; --max);
		free(jlist[jno]);
		jlist[jno] = NULL;
	}

	return 0;
}
