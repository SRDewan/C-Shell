#include "headers.h"
#include "opts.h"
#include "ls_l.h"
#include "ls.h"

lld one = 0;
struct op all[3] = {{'~', 0, 0, NULL}, {'a', 0, 0, NULL}, {'l', 0, 0, NULL}};

int cmp(const void *a, const void *b)
{
	struct row *ia = (struct row *)a;
	struct row *ib = (struct row *)b;
	return strcmp(ia->name, ib->name);
}

int len(lld num)
{
	return floor(log10(num)) + 1;
}
	
lld print(struct row entity, lld max[4])
{
	if(all[2].flag)
		printf("%s %*lld %*s %*s %*lld %s ", entity.per, len(max[0]), entity.links, (int)max[1], entity.user, (int)max[2], entity.grp, len(max[3]), entity.sz, entity.timestamp);

	printf("%s\n", entity.name);
	return 0;
}

lld list(char *dirname, struct row entity[])
{
	lld ind = 0, tot = 0, max[4];
	max[1] = max[2] = max[3] = max[0] = -1;

	DIR *dir = opendir(dirname);
	if(!dir)
	{
		perror("ls - dir(open): Error");
		return -1;
	}

	errno = 0;

	if(all[0].flag)
		printf("~:\n");
	else if(!one)
		printf("\n%s:\n", dirname);

	entity = (struct row *)malloc(0);
	for(struct dirent *entry;; ++ind)
	{
		entry = readdir(dir);
		if(!entry) 
		{
			if(errno)
			{
				perror("ls - dir entry: Error");
				return -1;
			}

			break;
		}

		else if(!all[1].flag && entry->d_name[0] == '.')
		{
			--ind;
			continue;
		}

		entity = realloc(entity, sizeof(struct row) * (ind + 1));
		strcpy(entity[ind].name, entry->d_name);
		if(all[2].flag)
		{
			char path[inf];
			strcpy(path, dirname);

			if(path[strlen(path) - 1] != '/')
				strcat(path, "/");

			if(lsl(&entity[ind], strcat(path, (char *)entry->d_name)) == -1)
				return -1;

			tot += entity[ind].block;
			if(max[0] < entity[ind].links)
				max[0] = entity[ind].links;
			if(max[1] < (lld)strlen(entity[ind].user))
				max[1] = (lld)strlen(entity[ind].user);
			if(max[2] < (lld)strlen(entity[ind].grp))
				max[2] = (lld)strlen(entity[ind].grp);
			if(max[3] < entity[ind].sz)
				max[3] = entity[ind].sz;
		}
	}

	if(all[2].flag)
		printf("total %lld\n", tot / 2);

	qsort(entity, ind, sizeof(struct row), cmp);
	for(lld i = 0; i < ind; ++i)
		print(entity[i], max);

	free(entity);
	if(closedir(dir))
		perror("ls - dir(close): Error");

	return 0;
}

lld ls(lld *j, char *token[inf], char *home)
{
	all[0].flag = all[1].flag = all[2].flag = 0;
	lld arr[*j], ctr = opts(arr, j, token, all + 1, 2), ret = 0;       //call to opts()
	struct row *entity;

	if(*j - ctr == 1)
	{
		one = 1;
		if(list(".", entity))
			return -1;
		one = 0;
	}

	else if(*j - ctr == 2)
		one = 1;

	for(lld i = 1; i < *j; ++i)
	{
		if(arr[i])
			continue;

		if(!strcmp(token[i], "~"))
		{
			all[0].flag = 1;
			strcpy(token[i], home);
		}

		if(list(token[i], entity))
			ret = -1;
		all[0].flag = 0;
	}

	one = 0;
	return ret;
}
