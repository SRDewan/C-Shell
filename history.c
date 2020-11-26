#include "headers.h"
#include "history.h"
#include "number.h"

lld history_rd(char *past[22], char *path)
{
	lld num;
	int inp = open(path, O_RDONLY);
	if(inp == -1)
	{
		if(errno != ENOENT)
		{
			perror("history_rd - open: Error");
			return -1;
		}

		return 0;
	}

	int fsz = lseek(inp, 0, SEEK_END);
	lseek(inp, -fsz, SEEK_END);
	char *buf = (char *)malloc(fsz * sizeof(char)); 

	if(read(inp, buf, (size_t)fsz) == -1)
	{
		perror("history_rd - read: Error");
		return -1;
	}

	if(close(inp) == -1)
		perror("history_rd - close: Error");

	past[0] = strtok(buf, "\n");
	for(num = 1; (past[num] = strtok(NULL, "\n")); ++num);

	return num;
}

lld history_up(char *cmd, char *home)
{
	char path[inf], *latest[22] = {NULL};
	strcpy(path, home);
	strcat(path, "/");
	strcat(path, ".history.txt");

	lld qty = history_rd(latest, path);
	if(qty == -1)
		return -1;

	int op = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0600);
	if(op == -1)
		perror("history_up - open: Error");

	lseek(op, 0, SEEK_END);

	if(qty == 1 && latest[0] == NULL)
	{
		latest[0] = (char *)malloc(strlen(cmd) * sizeof(char));
		strcpy(latest[0], cmd);
	}

	else if(!qty || strcmp(latest[qty - 1], cmd))
	{
		latest[qty] = (char *)malloc(strlen(cmd) * sizeof(char));
		strcpy(latest[qty], cmd);
		++qty;
	}

	lld start = 0;
	if(qty == 21)
		start = 1;

	for(; start < qty; ++start)
	{
		write(op, latest[start], strlen(latest[start]) * sizeof(char));
		write(op, "\n", strlen("\n") * sizeof(char));
	}

	if(close(op) == -1)
		perror("history_up - close: Error");

	return 0;
}

lld hist(lld *j, char *token[], char *home)
{
	char *latest[20] = {NULL}, path[inf];
	strcpy(path, home);
	strcat(path, "/");
	strcat(path, ".history.txt");

	lld i = 10, qty; 
	if(*j == 1)
		qty = history_rd(latest, path);

	else if(!numcheck(token[1]))
	{
		i = atol(token[1]);
		qty = history_rd(latest, path);
	}

	else
	{
		printf("history - args: Error: Invalid argument\n");
		return -1;
	}

	if(qty == -1)
		return -1;

	if(i > qty)
		i = qty;

	for(lld j = qty - i; j < qty; ++j)
		printf("%s\n", latest[j]);

	return 0;
}
