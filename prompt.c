#include "headers.h"
#include "path.h"
#include "prompt.h"
#include "cmdParse.h"

lld next = 0, exflag = 0;

char *promptDets(char *home, lld exstat)
{
	char *user = getenv("USER"), st[inf] = "";
	if(!user)
		printf("prompt - user: Error: Unable to obtain username");

	if(exflag)
	{
		if(exstat)
			strcpy(st, ":'(");
		else
			strcpy(st, ":')");
	}

	char host[inf];
	if(gethostname(host, inf))
		perror("prompt - host: Error");

	char *det = (char *)malloc(strlen(user) + strlen(host) + inf);
	sprintf(det, "%s<%s@%s:%s>", st, user, host, pwd(home, 0));

	return det;
}

void prompt()
{
	size_t sz = inf;
	char *cmd = (char *)malloc(sz), *home= getenv("home");
	lld exstat;

	if(!home)
	{
		printf("prompt - home: Error: Failed to determine home directory\n");
		return;
	}

	for(;;)
	{
		printf("%s", promptDets(home, exstat));
		if(getline(&cmd, &sz, stdin) == -1)
			kill(getpid(), SIGKILL);

		cmd[strlen(cmd) - 1] = '\0';  //to prevent parse() from including '\n' at end of last token
		if(!cmd)
			continue;

		parse(cmd, home, &exstat);
		exflag = 1;
	}

	free(cmd);
}
