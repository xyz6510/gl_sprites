#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int compare_file_size(char *path,int len);
int create_file(char *path,int len,int perm);
int map_file(int f,void **m,int len,int rw);
