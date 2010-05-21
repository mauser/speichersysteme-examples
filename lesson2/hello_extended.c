 #define FUSE_USE_VERSION  26
   
  #include <fuse.h>
  #include <stdio.h>
  #include <string.h>
  #include <errno.h>
  #include <fcntl.h>
  #include <unistd.h>

  #include <sys/types.h>
  #include <sys/stat.h>


  
#define _XOPEN_SOURCE 500
 
  static const char *hello_str = "Hello World!\n";
  static const char *hello_path = "/hello";
  static const char *file_path = "/file";
  static const char *file_str = "example\n";
  
  static int hello_getattr(const char *path, struct stat *stbuf)
  {
      int res = 0;
      memset(stbuf, 0, sizeof(struct stat));
      if(strcmp(path, "/") == 0) {
          stbuf->st_mode = S_IFDIR | 0755;
          stbuf->st_nlink = 2;
      }
      else if(strcmp(path, hello_path) == 0) {
          stbuf->st_mode = S_IFREG | 0744;
          stbuf->st_nlink = 1;
          stbuf->st_size = strlen(hello_str);
      } else if(strcmp(path, file_path) == 0) {
          stbuf->st_mode = S_IFREG | 0777;
          stbuf->st_nlink = 1;
	  stbuf->st_uid = 1000;
   	  //stbuf->st_gid =  
	  struct stat s;
	  stat("/tmp/file", &s);
          stbuf->st_size = s.st_size;
      }
      else
          res = -ENOENT;
  
      return res;
  }
 

  static int hello_truncate (const char *c, off_t offset){
	//int ret = truncate(c,offset);
  	
		
	int fd = open("/tmp/file", O_WRONLY);
	int ret = ftruncate( fd, offset );	
	close(fd);
	
	return ret;
  }
 
  static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                           off_t offset, struct fuse_file_info *fi)
  {
      (void) offset;
      (void) fi;
  
      if(strcmp(path, "/") != 0)
          return -ENOENT;
  
      filler(buf, ".", NULL, 0);
      filler(buf, "..", NULL, 0);
      filler(buf, hello_path + 1, NULL, 0);
      filler(buf, file_path + 1, NULL, 0);
  
      return 0;
  }
  
  static int hello_open(const char *path, struct fuse_file_info *fi)
  {
      if(strcmp(path, hello_path) != 0 ){	   
         if(strcmp(path, file_path) != 0 ){
	    return -ENOENT;
	 }
      }
  
      //if((fi->flags & 3) != O_RDONLY)
       //   return -EACCES;
  
      return 0;
  }
  
  static int hello_read(const char *path, char *buf, size_t size, off_t offset,
                        struct fuse_file_info *fi)
  {
      size_t len;
      (void) fi;
      if(strcmp(path, hello_path) != 0 ){	   
         if(strcmp(path, file_path) != 0 ){
	    return -ENOENT;
         }
       }

  
     
    
     if(strcmp(path, hello_path) == 0 ){	   
     len = strlen(hello_str);
     if (offset < len) {
          if (offset + size > len)
              size = len - offset;
          memcpy(buf, hello_str + offset, size);
      } else
          size = 0;
      }

     if(strcmp(path, file_path) == 0 ){	   
     	

	struct stat s;
	stat("/tmp/file", &s);
        len = s.st_size;

	//len = strlen(file_str);
     	if (offset < len) {
          if (offset + size > len){
              size = len - offset;
	  }

	int fd = open("/tmp/file", O_RDONLY);
	pread(fd, buf, size, offset);
	close(fd);
	  
	
          //memcpy(buf, file_str + offset, size);
      	} else
          	size = 0;
      	}

      return size;
  }


static int hello_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	
	int fd = open("/tmp/file", O_WRONLY | O_CREAT);
	int f  =  pwrite(fd, buf, size, offset);
	close(fd);
	if(f<0){
	 printf("ERRNO: %i",errno);
	 return -errno;
	}
			
    	return f;
}
  
  static struct fuse_operations hello_oper = {
      .getattr   = hello_getattr,
      .readdir = hello_readdir,
      .open   = hello_open,
      .read   = hello_read,
      .truncate = hello_truncate,
      .write = hello_write,
  };
  
  int main(int argc, char *argv[])
  {
      return fuse_main(argc, argv, &hello_oper, NULL);
  }

