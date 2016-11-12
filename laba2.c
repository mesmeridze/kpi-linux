#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>


long recurssivedirsize (char * recfolder, long totalsize); 

int main (int argc, char *argv[]){
	
	struct stat filestat;
	DIR *folder; 
	int argum,err;
	long totalsize=0; 


        if (argc <= 1 ) {

       		argv[0]=".";
		argum = 0;
	}
	else argum=1;

	printf ("%30s%20s", "folder", "size");

	for (argum; argum < argc; argum++){
		err = lstat(argv[argum], &filestat);
		if(-1 == err) {
			if(ENOENT == errno) {
				fprintf (stderr,"%s  %s \n",argv[argum], strerror(errno));
				continue;
			} 
			else {
				printf("\nerror");
				exit(1);
			}
		}	

    		if(!S_ISDIR(filestat.st_mode)) {
			printf ("%s Is not a directory\n",argv[argum]);
			continue;
		}
		totalsize=0;

		totalsize = recurssivedirsize (argv[argum], totalsize) + filestat.st_size;


		printf ("\n%30s%20lu\n", argv[argum], totalsize);

	}

return (0);
}




long recurssivedirsize (char * recfolder, long totalsize){


	struct dirent *filest;
	char path[PATH_MAX];
	DIR *folder;
	struct stat filestat;

        if ((folder=opendir(recfolder)) == NULL) return (totalsize); 


        while (filest=readdir(folder)){

                if (strcmp (filest->d_name, ".") == 0 || strcmp (filest->d_name, "..") == 0) 
                        continue;

                sprintf (path,"%s/%s",recfolder,filest->d_name);
		
		if (strlen(path) > 1024){
			printf ("\n File / Folder path of %s is longer than 1024 symbols. Skipping...",filest->d_name);
		}


		if (lstat (path, &filestat) == -1){ 
			printf ("Could not open file/folder %s",filest->d_name);
			continue;
		}

		if(S_ISDIR(filestat.st_mode)) {
			totalsize = totalsize+filestat.st_size;
			totalsize = recurssivedirsize (path, totalsize);
			continue;
                }
		if(!S_ISREG(filestat.st_mode)) {
			continue;
		}
		totalsize = totalsize+filestat.st_size;

	}
	closedir (folder);
	return (totalsize);
} 
