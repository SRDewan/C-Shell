#include "headers.h"
#include "opts.h"
#include "number.h"
#include "nightswatch.h"

lld nightswatch_parse(char *token[inf], lld m, lld *period)
{
	lld start = 0;

	if(m == 1)
	{
		printf("nightswatch opts: Error: Insufficient arguments\n"); 
		return -1;
	}

	start = 1;

	if(token[1][0] == '-' && token[1][1] == 'n')
	{
		if(m == 2)
		{
			printf("nightswatch opts: Error: Insufficient arguments\n"); 
			return -1;
		}

		start = 3;
		if(!numcheck(token[2]))
			*period = atol(token[2]);
		else
		{
			printf("nightswatch opts: Error: %s is not valid argument to option '-n'\n", token[2]); 
			return -1;
		}
	}

	else
		*period = 2;

	return start;
}

lld nightswatch(lld period)
{
	fd_set rfds;
	struct timeval tv = {period, 0};
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	int ret = select(1, &rfds, NULL, NULL, &tv); 
	if(ret == -1)
		perror("nightswatch - wait: Error");
	else if(ret && 'q' == (char)getchar())
		return 1;

	return 0;
}
