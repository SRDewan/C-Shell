#include "headers.h"
#include "newborn.h"

lld recproc()
{
	lld i = 1;
	char *token[inf], *line = (char *)malloc(sizeof(char) * inf);

	lld file = open("/proc/loadavg", O_RDONLY);
	if(file == -1)
	{
		perror("newborn - file(open): Error");
		return -1;
	}

	read(file, line, inf * sizeof(char));
	token[0] = strtok(line, " ");
	for(i = 1; (token[i] = strtok(NULL, " ")); ++i);

	if(token[i - 1][strlen(token[i - 1]) - 1] == '\n')
		token[i - 1][strlen(token[i - 1]) - 1] = '\0';

	printf("%s\n", token[i - 1]);

	if(close(file) == -1)
		perror("newborn - file(close): Error");

	free(line);
	return 0;
}
