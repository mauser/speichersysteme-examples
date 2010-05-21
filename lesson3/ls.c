#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

struct dirent *readdir(DIR *dir);

int main(){
	
	struct dirent *dp;	
	DIR* dirp = opendir(".");

	while (dirp) {
    		dp = readdir(dirp);
				
    		if (dp != NULL ) {
			if( dp->d_type == 4 ){
					printf("\E[31m %s %i \E[30m\n",dp->d_name, dp->d_type);
			} else {
				
				struct stat s;
	  			stat( dp->d_name , &s);
          			
    				
				printf("%s %i %i %i \n",dp->d_name, dp->d_type , s.st_size, s.st_uid);
	  			
      			}
			
		} else {
        		closedir(dirp);
        		return -1;
    		}
	}
}

