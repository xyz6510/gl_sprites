#include "files.h"

int compare_file_size(char *path,int len)
{
	int ret=0;
	int f=open(path,O_RDONLY);
	if (f<0) {
		ret=-1;
		goto end;
	}
	struct stat fs;
	fstat(f,&fs);
	int align=getpagesize();
	int size=((len/align)+1)*align;
	if (fs.st_size!=size) ret=1;
	close(f);
end:
	return ret;
}

int create_file(char *path,int len,int perm)
{
	int ret=0;
	int f=open(path,O_RDWR | O_CREAT | O_TRUNC,perm);chmod(path,perm);
	if (f<0) {
		printf("cannot create file:%s\n",path);
		ret=-1;
		goto end;
	}
	int align=getpagesize();
	int size=((len/align)+1)*align;
	char *m=malloc(size);
	memset(m,0,size);
	write(f,m,size);
	free(m);
	close(f);
end:
	return ret;
}

int map_file(int f,void **m,int len,int rw)
{
	int ret=0;

	int align=getpagesize();
	int size=((len/align)+1)*align;
	posix_memalign(m,align,size);
	int flags=PROT_READ;
	if (rw) flags|=PROT_WRITE;
	*m=mmap(*m,size,flags,MAP_SHARED | MAP_FIXED,f,0);
	if (*m==MAP_FAILED) {
		printf("error mmap file\n");
		ret=-1;
	}
	return ret;
}
