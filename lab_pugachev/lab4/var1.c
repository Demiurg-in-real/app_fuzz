#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include <stdbool.h>
#include<stdlib.h>
//#include<time.h>
int main(int argc, char* argv[])
{
	int sd, i=0; struct stat st; short int *ptr;
	sd=open(argv[1], O_RDWR);
	fstat(sd, &st);
	ptr=(unsigned short*)mmap(NULL,st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED,sd,0);
	munmap(ptr, st.st_size);
	close(sd);
	return 0;
}

