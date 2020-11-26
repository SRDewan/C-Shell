#include "headers.h"
#include "number.h"

lld numcheck(char *strg)
{
	for(lld a = 0; strg[a] != '\0'; ++a)
	{
		if(!(strg[a] <= '9' && strg[a] >= '0'))
			return -1;
	}

	return 0;
}
