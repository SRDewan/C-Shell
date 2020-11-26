#include "cmdParse.h"

char *in[inf], *out[inf], *add[inf], *fincmd;
struct job *jlist[inf], *curr;
lld proc[inf * 10], max = 0, inv, outv, addv, flag = 1, pin = 0, pout = 0, tempo = 0;
int rpend, wpend, comm[2];
lld stin, stout;

lld jobinit(pid_t child, char *bgcmd)
{
	lld tmp;
	for(tmp = 1; tmp < inf && jlist[tmp]; ++tmp);

	jlist[tmp] = (struct job*)malloc(sizeof(struct job));
	jlist[tmp]->pid = child;
	jlist[tmp]->name = (char *)malloc(strlen(bgcmd) * sizeof(char));
	strcpy(jlist[tmp]->name, bgcmd);

	proc[child] = tmp;
	if(max < tmp)
		max = tmp;

	return 0;
}

lld parse(char *cmd, char *home, lld *exstat)
{
	*exstat = 0;
	int fd = open(".help", O_CREAT | O_TRUNC, 0777);
	stin = dup(STDIN_FILENO);
	if(stin == -1)
	{
		*exstat = -1;
		perror("cmdParse- dup stdin: Error");
		return -1;
	}

	stout = dup(STDOUT_FILENO);
	if(stout == -1)
	{
		*exstat = -1;
		perror("cmdParse- dup stdout: Error");
		return -1;
	}

	lld i, j, k, l, m, pi, pj;
	char *cmds[inf], *token[inf], *bgcmds[inf], *piped[inf]; 
	jlist[0] = NULL;
	proc[0] = 0;

	/*history_up(cmd, home);*/
	cmds[0] = strtok(cmd, ";");
	if(!cmds[0])
		return 0;
	for(i = 1; (cmds[i] = strtok(NULL, ";")); ++i);

	lld semiret = 0;
	for(j = 0; j < i; ++j)
	{
		char last = cmds[j][strlen(cmds[j]) - 1];
		bgcmds[0] = strtok(cmds[j], "&");
		if(!bgcmds[0])
			continue;
		for(k = 1; (bgcmds[k] = strtok(NULL, "&")); ++k);

		lld bgret = 0;
		for(l = 0; l < k; ++l)
		{
			flag = 1;
			lld period = -1, start = 0;
			if(l == k - 1 &&  last != '&')
				flag = 0;

			piped[0] = strtok(bgcmds[l], "|");
			if(!piped[0])
				continue;
			for(pi = 1; (piped[pi] = strtok(NULL, "|")); ++pi);
			int pends[pi][2];

			tempo = 0;
			lld ret = 0, tm = 0;
			for(pj = 0; pj < pi; ++pj)
			{
				ret = 0; tm = 0;
				if(pipe(pends[pj]) == -1)
				{
					perror("cmdParse - pipe create: Error");
					ret = -1;
					return -1;
				}

				char *temp = (char *)malloc(strlen(piped[pj]) * sizeof(char));
				strcpy(temp, piped[pj]);
				redirect(piped[pj]);

				token[0] = strtok(fincmd, " ");
				if(!token[0])
					continue;
				for(m = 1; (token[m] = strtok(NULL, " ")); ++m);

				if(!flag && (!strcmp(token[0], "cd") || !strcmp(token[0], "setenv") || !strcmp(token[0], "unsetenv")))
				{
					ret = callcmd(token, home, &m, start, period);
					continue;
				}

				if(signal(SIGCHLD, parsig) == SIG_ERR)
					perror("cmdParse - parent bg signal: Error");

				if(pipe(comm) == -1)
				{
					ret = -1;
					continue;
				}

				pid_t child = fork();
				if(child == -1)
				{
					perror("cmdParse - child: Error");
					ret = -1;
				}

				else if(!child)
				{
					setpgid(getpid(), 0);

					if(signal(SIGTTIN, SIG_IGN) == SIG_ERR)
						perror("cmdParse - child TTOU restore: Error");

					if(!flag)
					{
						if(signal(SIGINT, SIG_DFL) == SIG_ERR)
							perror("cmdParse - fgchild ctrl-c signal: Error");
						if(signal(SIGTSTP, SIG_DFL) == SIG_ERR)
							perror("cmdParse - fgchild ctrl-z: Error");

						if(signal(SIGTTOU, SIG_IGN) == SIG_ERR)
							perror("cmdParse - child TTOU ignore: Error");
						if(tcsetpgrp(STDIN_FILENO, getpgid(getpid())) == -1)
							perror("cmdParse - set child to fg proc: Error");
						if(signal(SIGTTOU, SIG_DFL) == SIG_ERR)
							perror("cmdParse - child TTOU restore: Error");
					}

					else if(signal(SIGTSTP, SIG_IGN) == SIG_ERR)
						perror("cmdParse - bgchild ctrl-z: Error");

					rpend = stin;
					wpend = stout;
					if(pj)
					{
						rpend = pends[pj - 1][0];
						pin = 1;
					}

					if(pj < pi - 1)
					{
						wpend = pends[pj][1];
						pout = 1;
					}

					if(close(pends[pj][0]))
						perror("cmdParse - close rpipe: Error");

					if(!strcmp(token[0], "nightswatch"))
					{
						start = nightswatch_parse(token, m, &period);
						if(start == -1)
							ret = -1;
					}

					m -= start;
					tm = util(token, home, &m, start, period);
					if(!ret)
						ret = tm;

					char pret[5];
					if(close(comm[0]))
						perror("cmdParse - close child rpipe with parent: Error");
					if(ret)
						strcpy(pret, "-1");
					else
						strcpy(pret, "0");

					write(comm[1], pret, sizeof(pret));
					if(close(comm[1]))
						perror("cmdParse - close child wpipe with parent: Error");
					_exit(0); //to prevent same child from executing consequent iterations
				}

				else if(child > 0)
				{
					setpgid(getpid(), 0);
					curr = (struct job*)malloc(sizeof(struct job));
					curr->pid = child;
					curr->name = (char *)malloc(strlen(temp) * sizeof(char));
					strcpy(curr->name, temp);

					int wstat;
					if(!flag)
					{
						if(waitpid(child, &wstat, WUNTRACED) == -1)
							perror("cmdParse - fg wait: Error");

						ret = tempo;
						if(WIFSTOPPED(wstat))
						{
							ret = -1;
							flag = 1;
							jobinit(curr->pid, curr->name);

							if(signal(SIGCHLD, parsig) == SIG_ERR)
								perror("cmdParse - parent bg signal: Error");
							printf("Process '%s' with pid %lld suspended\n", jlist[proc[child]]->name, (lld)child);
						}

						else 
							close(pends[pj][1]);

						if(signal(SIGTTOU, SIG_IGN) == SIG_ERR)
							perror("cmdParse - par TTOU ignore: Error");
						if(tcsetpgrp(STDIN_FILENO, getpgid(getpid())) == -1)
							perror("cmdParse - set parent to fg proc: Error");
						if(signal(SIGTTOU, SIG_DFL) == SIG_ERR)
							perror("cmdParse - par TTOU restore: Error");
					}

					else
						jobinit(child, temp);
				}

				free(temp);
				free(curr);
				curr = NULL;

				dup2(fd, 1);
				printf("%lld\n", ret);
				dup2(stout, 1);
				if(pj == pi - 1)
					bgret = ret;
			}

			dup2(fd, 1);
			printf("%lld\n", bgret);
			dup2(stout, 1);
			if(l == k - 1)
				semiret = bgret;
		}

		dup2(fd, 1);
		printf("%lld\n", semiret);
		dup2(stout, 1);
		if(j == i - 1)
			*exstat = semiret;
	}

	close(fd);
	return 0;
}
