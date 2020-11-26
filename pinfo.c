#include "headers.h"
#include "pinfo.h"

lld procdets(pid_t proc)
{
	size_t sz = inf * sizeof(char);
	char *procpath = (char *)malloc(sz), *path= (char *)malloc(sz), *stat = (char *)malloc(sz), *token[inf];

	printf("pid -- %lld\n", (lld)proc);

	sprintf(path, "/proc/%lld/stat", (lld)proc);
	lld mem = open(path, O_RDONLY);
	if(mem == -1)
	{
		perror("pinfo - proc mem(open): Error");
		return -1;
	}

	read(mem, stat, inf * sizeof(char));
	token[0] = strtok(stat, " ");
	for(lld i = 1; (token[i] = strtok(NULL, " ")); ++i);

	if(close(mem) == -1)
		perror("pinfo - proc mem(close): Error");

	printf("Process Status -- %s\n", token[2]);
	printf("memory -- %s\n", token[22]);

	sprintf(path, "/proc/%lld/exe", (lld)proc);
	if(readlink(path, procpath, inf * sizeof(char)) == -1)
	{
		perror("pinfo - path: Error");
		return -1;
	}

	printf("Executable Path -- %s\n", procpath);

	free(path);
	free(procpath);
	free(stat);

	return 0;
}

lld pinfo(lld *k, char *token[inf])
{
	pid_t proc;
	char *path = (char *)malloc(30 * sizeof(char));
	lld ret = 0;

	if(*k == 1)
	{
		proc = getppid();
		if(procdets(proc))
			return -1;
	}

	for(lld i = 1; i < *k; ++i)
	{
		proc = (pid_t)atoi(token[i]);
		sprintf(path, "/proc/%d", proc);

		DIR* dir = opendir(path);
		if(!dir && ENOENT == errno)    //check if given pid is valid
		{
			perror("pinfo: Error");
			ret = -1;
		}

		closedir(dir);
		if(procdets(proc))
			ret = -1;
	}

	free(path);
	return ret;
}
