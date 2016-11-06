#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>



int main (int argc, char *argv[]){
	
	char *dirname;
	struct dirent *filest;
	struct stat filestat;
	struct passwd *username;
	struct group *groupname;
	DIR *folder; 
	char type[10];
	static char local_buff[16] = {0};
	char path[255];
	char user[20];
	char modtime[30];
	int i;


        if (argc > 1 ) 
		 dirname=argv[1];
       	else dirname=".";

	if ((folder=opendir(dirname)) == NULL) {

	fprintf (stderr,"Невозможно открыть каталог %s  %s \n",dirname, strerror(errno));
	exit (1);
	}
	
		printf ("\x1b[34m%4s%15s%8s%10s%10s%10s%30s%10s%40s \x1b[0m\n", "type" , "access", "h_links","owner","group","size","modification time","inode","file/dir name");

	while (filest=readdir(folder)){

		sprintf (path,"%s/%s",dirname,filest->d_name);
		if ( lstat(path, &filestat) == -1 ){

			fprintf (stderr,"Невозможно открыть file %s  %s \n", filest->d_name , strerror(errno));
        		exit (1);

		}


		type[0]='\0';
		user[0]='\0';
		switch ( filestat.st_mode & S_IFMT )
    		{
        		case S_IFDIR:  sprintf (type,"d"); break;
        		case S_IFCHR:  sprintf (type,"c"); break;
			case S_IFBLK:  sprintf (type,"b"); break;
			case S_IFREG:  sprintf (type,"-"); break;
			case S_IFLNK:  sprintf (type,"l"); break;
			case S_IFSOCK: sprintf (type,"s"); break;
			default: sprintf (type,"undef");
		}

		sprintf (local_buff, "%s","------------");
		

		if (filestat.st_mode & S_IRUSR) 
			local_buff[0]='r';

		if (filestat.st_mode & S_IWRITE) 
			local_buff[1]='w';

		if (filestat.st_mode & S_IEXEC) 
			local_buff[2]='x';

		if (filestat.st_mode & S_IRGRP) 
			local_buff[4]='r';

		if (filestat.st_mode & S_IWGRP) 
			local_buff[5]='w';

		if (filestat.st_mode & S_IXGRP) 
			local_buff[6]='x';

		if (filestat.st_mode & S_IROTH) 
			local_buff[8]='r';

		if (filestat.st_mode & S_IWOTH) 
			local_buff[9]='w';

		if (filestat.st_mode & S_IXOTH) 
			local_buff[10]='x';


		username = getpwuid (filestat.st_uid);
		groupname = getgrgid (filestat.st_gid);
		sprintf (modtime, "%s",ctime(&filestat.st_ctime));	
		modtime[strlen(modtime)-1]='\0';

	
		printf ("%4s%15s%8d%10s%10s%10d%30s%10d%40s\n", type ,local_buff,filestat.st_nlink, username->pw_name,groupname->gr_name,filestat.st_size,modtime,filestat.st_ino,filest->d_name);
	}	

	closedir (folder);

	
return (0);
}
