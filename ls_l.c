#include "headers.h"
#include "ls_l.h"

lld perm(struct row *entry, struct stat entdet)
{
	char str[10];

	if(S_ISDIR(entdet.st_mode))
		strcpy(str, "d"); 
	else if(S_ISLNK(entdet.st_mode))
		strcpy(str, "l"); 
	else
		strcpy(str, "-"); 

	(entdet.st_mode & S_IRUSR) ? strcat(str, "r") : strcat(str, "-"); 
	(entdet.st_mode & S_IWUSR) ? strcat(str, "w") : strcat(str, "-");
	(entdet.st_mode & S_IXUSR) ? strcat(str, "x") : strcat(str, "-");
	(entdet.st_mode & S_IRGRP) ? strcat(str, "r") : strcat(str, "-");
	(entdet.st_mode & S_IWGRP) ? strcat(str, "w") : strcat(str, "-");
	(entdet.st_mode & S_IXGRP) ? strcat(str, "x") : strcat(str, "-");
	(entdet.st_mode & S_IROTH) ? strcat(str, "r") : strcat(str, "-");
	(entdet.st_mode & S_IWOTH) ? strcat(str, "w") : strcat(str, "-");
	(entdet.st_mode & S_IXOTH) ? strcat(str, "x") : strcat(str, "-");

	strcpy(entry->per, str);

	return 0;
}

lld lsl(struct row *entry, char *entname)
{
	struct stat entdet;

	if(stat(entname, &entdet))
	{
		perror("ls_l - entry stat: Error");
		return -1;
	}

	perm(entry, entdet);
	entry->links = (lld)entdet.st_nlink;

	struct passwd *user = getpwuid((lld)entdet.st_uid);
	if(!user)
	{
		perror("ls_l - username: Error");
		return -1;
	}

	struct group *grp = getgrgid((lld)entdet.st_gid);
	if(!grp)
	{
		perror("ls_l - grpname: Error");
		return -1;
	}

	strcpy(entry->user, user->pw_name);
	strcpy(entry->grp, grp->gr_name);
	entry->sz = (lld)entdet.st_size;

	struct tm *tstamp = localtime(&entdet.st_mtime);
	strftime(entry->timestamp, 20, "%b %e %H:%M", tstamp);
	entry->block = (lld)entdet.st_blocks;

	return 0;
}
