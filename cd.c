#include "headers.h"
#include "cd.h"

lld cd(lld *k, char *token[inf], char *home)
{
	char path[inf], *prev = (char *)malloc(inf * sizeof(char));

	if(*k == 1)
		strcpy(path, home);
	else
	{
		strcpy(path, token[1]);

		if(!strcmp(path, "-"))
		{
			prev = getenv("prev");
			if(!prev)
			{
				printf("cd - '-': Error: Could not obtain previous working directory\n");
				free(prev);
				return -1;
			}
			printf("%s\n", prev);
			strcpy(path, prev);
		}

		if(path[0] == '~')
		{
			char *tmp = (char *)malloc(strlen(path) * sizeof(char));
			strcpy(tmp, path + 1);
			strcpy(path, home);
			strcat(path, tmp);
			free(tmp);
		}
	}

	strcpy(prev, pwd(home, 0));
	if(setenv("prev", prev, 1) == -1)
		perror("cd - setenv: Error");

	if(chdir(path))
	{
		perror("cd: Error");
		free(prev);
		return -1;
	}

	if(*k > 2)
		printf("Not more than 1 argument considered\n");

	return 0;
}
