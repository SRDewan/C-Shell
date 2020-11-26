#include "headers.h"
#include "prompt.h"
#include "sighandle.h"

int main()
{
	char *home = (char *)malloc(inf * sizeof(char));

	if(!getcwd(home, inf * sizeof(char)))
		perror("shell - home: Error");
	home = (char *)realloc(home, strlen(home) * sizeof(char));

	if(setenv("home", home, 1) == -1)
	{
		perror("shell - setenv home: Error");
		free(home);
		return 0;
	}

	else if(setenv("prev", "~", 1) == -1)
		perror("shell - setenv prev: Error");

	if(signal(SIGTSTP, parz) == SIG_ERR)
		perror("shell - parent ctrl-z signal: Error");
	if(signal(SIGINT, SIG_IGN) == SIG_ERR)
		perror("shell - parent ctrl-c signal: Error");

	prompt();
	free(home);

	if(unsetenv("home") == -1)
		perror("shell - unsetenv home: Error");

	if(unsetenv("prev") == -1)
		perror("shell - unsetenv prev: Error");

	return 0;
}
