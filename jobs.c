#include "jobs.h"

char *status(pid_t pid)
{
	size_t sz = inf * sizeof(char);
	char *path= (char *)malloc(sz), *stat = (char *)malloc(sz), *token[inf];

	sprintf(path, "/proc/%lld/stat", (lld)pid);
	lld mem = open(path, O_RDONLY);
	if(mem == -1)
	{
		perror("jobs - proc mem(open): Error");
		return NULL;
	}

	read(mem, stat, inf * sizeof(char));
	token[0] = strtok(stat, " ");
	for(lld i = 1; (token[i] = strtok(NULL, " ")); ++i);

	if(close(mem) == -1)
		perror("jobs - proc mem(close): Error");

	free(path);
	free(stat);

	if(!strcmp(token[2], "S") || !strcmp(token[2], "R"))
		return "Running";

	else if(!strcmp(token[2], "T") || !strcmp(token[2], "D"))
		return "Stopped";

	else
		return "";
}

lld jobs(struct job *list[], lld max)
{
	char *st = (char *)malloc(50 * sizeof(char *));
	lld ret = 0;
	for(lld i = 1; i <= max; ++i)
	{
		if(!list[i])
			continue;

		strcpy(st, status(list[i]->pid));
		if(!st)
			ret = -1;

		printf("[%lld] %s %s [%lld]\n", i, st, list[i]->name, (lld)list[i]->pid);
	}

	free(st);
	return ret;
}
