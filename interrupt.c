#include "headers.h"
#include "interrupt.h"

lld inter(lld repeat)
{
	char *line = NULL, *token[inf];
	lld i, num = 0;
	FILE *file = fopen("/proc/interrupts", "r");
	size_t sz = 0;

	if(!file)
	{
		perror("interrupt - file(open): Error");
		return -1;
	}

	for(lld ind = 0; ind < 3; ++ind)
	{
		line = NULL;
		sz = 0;
		getline(&line, &sz, file);

		if(ind != 1)
		{
			token[0] = strtok(line, " ");
			for(i = 1; (token[i] = strtok(NULL, " ")); ++i);

			if(!ind)
				num = i;

			if(!ind && repeat)
				continue;

			for(lld j = 0; j < i; ++j)
			{
				if(ind && (!j || j >= num))
					continue;

				printf("%s\t", token[j]);
			}
			printf("\n");
		}
	}

	if(fclose(file) == -1)
		perror("interrupt - file(close): Error");

	return 0;
}
