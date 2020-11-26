#include "headers.h"
#include "env.h"

lld senv(lld *m, char *token[])
{
	char val[inf];
	strcpy(val, "");

	if(!(*m == 2 || *m == 3))
	{
		printf("env - set: Error: Incorrect number of arguments to setenv\n");
		return -1;
	}

	else if(*m == 3)
		strcpy(val, token[2]);

	if(setenv(token[1], val, 1) == -1)
	{
		perror("env - setenv: Error");
		return -1;
	}
	
	return 0;
}

lld uenv(lld *m, char *token[])
{
	if(!(*m == 2))
	{
		printf("env - unset: Error: Incorrect number of arguments to unsetenv\n");
		return -1;
	}

	if(unsetenv(token[1]) == -1)
	{
		perror("env - unsetenv: Error");
		return -1;
	}

	return 0;
}
