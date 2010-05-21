#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

struct dirent *readdir(DIR *dir);

int main( unsigned int argc, unsigned char *argv[]  ){
	FILE *src,*dst;
  	long lSize;
  	
	char * buffer;
  	
	size_t result;

  	src = fopen ( argv[1] , "rb" );
  	if (src==NULL) {fputs ("File error",stderr); exit (1);}

  	// obtain file size:
  	fseek (src , 0 , SEEK_END);
  	lSize = ftell ( src );
  	rewind ( src );

  	// allocate memory to contain the whole file:
  	buffer = (char*) malloc (sizeof(char)*lSize);
  	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  	// copy the file into the buffer:
  	result = fread (buffer,1,lSize,src);
  	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

  	// terminate
  	fclose (src);
  	
	
  	dst = fopen ( argv[2] , "w" );
  	if (dst==NULL) {fputs ("File error",stderr); exit (1);}
	fwrite( buffer, sizeof(char)*lSize,lSize,dst);
	fclose(dst);
	

	free (buffer);
}

