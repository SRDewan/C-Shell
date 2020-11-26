#include "headers.h"
#include "opts.h"

lld args(lld i, lld *j, char *token[], struct op *elem)
{
	lld num = elem->arg, lp, *arr = (lld *)malloc(num);
	for(lp = 1; lp <= num && lp + i < *j; ++lp)
	{
		/*if(numcheck(token[i + lp]))*/
		/*{*/
			/*printf("%s opts: Error: %s is not valid argument to option '-%c' ", token[0], token[i + lp], elem->letter);*/
			/*elem->flag = 0;*/
			/*return -1;*/
		/*}*/

		/*else */
			/*arr[lp - 1] = atol(token[i + lp]);*/
	}

	if(lp <= num)
	{
		printf("%s opts: Error: Insufficient number of arguments to option '-%c'", token[0], elem->letter);
		elem->flag = 0;
		return -1;
	}

	elem->arglist = arr;
	return 0;
}

lld opts(lld arr[], lld *j, char *token[], struct op all[], lld sz)
{
	lld ctr = 0, *list;

	for(lld i = 1; i < *j; ++i)
	{
		arr[i] = 0;

		if(token[i][0] == '-' && strlen(token[i]) > 1)
		{
			arr[i] = 1;
			++ctr;

			for(lld k = 1; token[i][k] != '\0'; ++k)
			{
				for(lld l = 0; l < sz; ++l)
				{
					if(token[i][k] == all[l].letter && !all[l].flag)
					{
						all[l].flag = 1;

						if(all[l].arg)
							args(i, j, token, &all[l]);
					}
				}
			}
		}
	}

	return ctr;
}
