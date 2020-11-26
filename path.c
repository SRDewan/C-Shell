#include "headers.h"
#include "path.h"

char *pwd(char *home, lld cmd)
{
	char *path = (char *)malloc(inf * sizeof(char)), *cwd = (char *)malloc(inf * sizeof(char)); 

	if(!getcwd(cwd, inf * sizeof(char)))
	{
		perror("pwd: Error");
		return NULL;
	}
	cwd = (char *)realloc(cwd, strlen(cwd) * sizeof(char));

	if(cmd)
		return cwd;

	if(!strcmp(home, cwd))
	{
		free(cwd);
		return "~";
	}

	else if(!strncmp(home, cwd, sizeof(char) * strlen(home)))
	{
		strcpy(path, "~");
		strcat(path, cwd + strlen(home) * sizeof(char));
		path = (char *)realloc(path, strlen(path) * sizeof(char));
		free(cwd);
		return path;
	}

	else
	{
		strcpy(path, cwd);
		free(cwd);
		return path;
	}
}
