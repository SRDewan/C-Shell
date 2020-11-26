#include "headers.h"
#include "sighandle.h"
#include "cmdParse.h"

extern struct job *jlist[inf], *curr;
extern lld proc[inf * 10], max, flag, exstat, tempo;
extern int comm[2];

void childsig(int sig)
{
	if(kill(0, SIGSTOP) == -1)
		perror("sighandle - suspend bg: Error");
}

void parsig(int signo)
{
	int status;
	char pret[5];
	lld pid = waitpid(-1, &status, WUNTRACED | WNOHANG);

	if(pid == -1 && flag)
		perror("sighandle - bg wait: Error");

	else if(!proc[pid] && pid && !WIFSTOPPED(status))
	{
		if(close(comm[1]))
			perror("sighandle - close parent wpipe with child: Error");
		read(comm[0], pret, sizeof(pret));
		if(!strcmp(pret, "-1"))
			tempo = -1;
		else
			tempo = 0;

		if(close(comm[0]))
			perror("cmdParse - close parent rpipe with child: Error");
	}

	else if(!pid || !proc[pid])
		return;

	else if(WIFEXITED(status) || WIFSIGNALED(status))
	{
		if(WIFEXITED(status))
			printf("Process '%s' with pid %lld exited normally\n", jlist[proc[pid]]->name, pid); 

		else
			printf("Process '%s' with pid %lld exited abnormally\n", jlist[proc[pid]]->name, pid); 

		free(jlist[proc[pid]]);
		jlist[proc[pid]] = NULL;
		if(max == proc[pid])
			for(max = max - 1; max > 0 && !jlist[max]; --max);
		proc[pid] = 0;
	}

	else if(WIFSTOPPED(status))
		printf("Process '%s' with pid %lld suspended with %d\n", jlist[proc[pid]]->name, pid, WSTOPSIG(status));
}

void childz(int sig)
{
	flag = 0;
	if(signal(SIGINT, SIG_IGN) == SIG_ERR)
		perror("sighandle - bgchild ctrl-c: Error");
	if(signal(SIGTSTP, SIG_IGN) == SIG_ERR)
		perror("sighandle - bgchild ctrl-z: Error");
}

void parz(int signo)
{
	if(!curr)
		return;

	flag = 1;
	jobinit(curr->pid, curr->name);

	if(signal(SIGCHLD, parsig) == SIG_ERR)
		perror("sighandle - parent bg signal: Error");

	if(kill(curr->pid, SIGTSTP) == -1)
		perror("sighandle - ctrl-z suspend fg: Error");
}
