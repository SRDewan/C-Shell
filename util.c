#include "util.h"

extern struct job *jlist[inf];
extern lld inv, outv, addv, proc[inf * 10], flag, pin, pout;
extern char *in[inf], *out[inf], *add[inf];
extern int rpend, wpend;
lld ctr, stin, stout;

lld output(char *token[inf], char *home, lld *m, lld start, lld period)
{
	lld ret = 0, tmp = 0;
	for(lld j = 0; j <= outv; ++j)
	{
		lld wr = open(out[j], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if(wr == -1)
		{
			perror("util - write open: Error");
			ret = -1;
			continue;
		}

		if(dup2(wr, STDOUT_FILENO) == -1)
		{
			perror("util - write dup: Error");
			ret = -1;
			continue;
		}

		tmp = callcmd(token, home, m, start, period);
		if(!ret)
			ret = tmp;

		if(close(wr) == -1)
			perror("util - write close: Error");

		if(dup2(stout, STDOUT_FILENO) == -1)
			perror("util - stdout dup: Error");
	}

	return ret;
}

lld append(char *token[inf], char *home, lld *m, lld start, lld period)
{
	lld ret = 0, tmp = 0;
	for(lld j = 0; j <= addv; ++j)
	{
		lld ap = open(add[j], O_WRONLY | O_APPEND | O_CREAT, 0644);
		if(ap == -1)
		{
			perror("util - app open: Error");
			ret = -1;
			continue;
		}

		lld h = dup2(ap, STDOUT_FILENO);
		if(h == -1)
		{
			perror("util - app dup: Error");
			ret = -1;
			continue;
		}

		tmp = callcmd(token, home, m, start, period);
		if(!ret)
			ret = tmp;

		if(close(ap) == -1)
			perror("util - app close: Error");

		if(dup2(stout, STDOUT_FILENO) == -1)
			perror("util - stdout dup: Error");
	}

	return ret;
}

lld input(char *token[inf], char *home, lld *m, lld start, lld period)
{
	lld prd = 0, ret = 0, tmp = 0;
	if(pin)
		prd = 1;

	for(lld i = 1; prd || i <= inv; ++i)    
	{
		lld rd;
		if(prd)
		{
			rd = rpend;
			prd = 0;
		}

		else
		{
			rd = open(in[i], O_RDONLY);
			if(rd == -1)
			{
				perror("util - inp open: Error");
				ret = -1;
				continue;
			}
		}

		if(dup2(rd, STDIN_FILENO) == -1)
		{
			perror("util - inp dup: Error");
			ret = -1;
			continue;
		}

		if(addv >= 0)
		{
			tmp = append(token, home, m, start, period);
			if(!ret)
				ret = tmp;
		}

		if(outv >= 0)
		{
			tmp = output(token, home, m, start, period);
			if(!ret)
				ret = tmp;
		}

		if(pout && dup2(wpend, STDOUT_FILENO) == -1)
		{
			perror("util - pipe op dup: Error");
			ret = -1;
		}

		else if(pout)
		{
			tmp = callcmd(token, home, m, start, period);
			if(!ret)
				ret = tmp;
			if(dup2(stout, STDOUT_FILENO) == -1)
				perror("util - stdout dup: Error");
		}

		else if(addv < 0 && outv < 0)
		{
			tmp = callcmd(token, home, m, start, period);
			if(!ret)
				ret = tmp;
		}

		if(close(rd) == -1)
			perror("util - read close: Error");

		if(dup2(stin, STDIN_FILENO) == -1)
			perror("util - stdin dup: Error");
	}

	return ret;
}

lld util(char *token[inf], char *home, lld *m, lld start, lld period)
{
	lld ret = 0, tmp = 0;
	for(ctr = 0;; ++ctr)
	{
		if(inv >= 1)
		{
			tmp = input(token, home, m, start, period);
			if(!ret)
				ret = tmp;
		}

		else
		{
			if(pin && dup2(rpend, STDIN_FILENO) == -1)
			{
				perror("util - inp dup pipe: Error");
				ret = -1;
				continue;
			}

			if(addv >= 0)
			{
				tmp = append(token, home, m, start, period);
				if(!ret)
					ret = tmp;
			}
			if(outv >= 0)
			{
				tmp = output(token, home, m, start, period);
				if(!ret)
					ret = tmp;
			}

			if(pout && dup2(wpend, STDOUT_FILENO) == -1)
			{
				perror("util - pipe op dup: Error");
				ret = -1;
			}

			if(pout)
			{
				tmp = callcmd(token, home, m, start, period);
				if(!ret)
					ret = tmp;
				if(dup2(stout, STDOUT_FILENO) == -1)
					perror("util - stdout dup: Error");
			}

			else if(addv < 0 && outv < 0)
			{
				tmp = callcmd(token, home, m, start, period);
				if(!ret)
					ret = tmp;
			}

			if(dup2(stin, STDIN_FILENO) == -1)
				perror("util - stdin dup: Error");
		}

		if(!strcmp(token[0], "nightswatch"))
		{
			if(nightswatch(period))
				break;
		}

		else
			break;
	}

	return ret;
}
