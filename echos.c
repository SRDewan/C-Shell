#include "headers.h"
#include "echos.h"

lld echos(lld *k, char *token[inf])
{
	for(int i = 1; i < *k; ++i)
	{
		printf("%s", token[i]);

		if(i != *k - 1)
			printf(" ");
	}

	printf("\n");
	return 0;
}
