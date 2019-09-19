#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include <stdbool.h>
#include<stdlib.h>
#include<stdint.h>
struct zp 
{
	uint64_t adel;
};
int prit(struct zp *a, char* file1);
int main(int argc, char* argv[])
{
	struct zp vah;
	prit(&vah,argv[1]);
	printf("%04x\n", vah.adel);
	return 0;
}
int prit(struct zp *a, char* file1)
{
	int sd, i=0; struct stat st; uint64_t *ptr;//struct zp *a;
	a->adel=615;
	sd=open(file1, O_RDWR);
	fstat(sd, &st);
	ptr=(uint64_t*)mmap(NULL,st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED,sd,0);
	*((uint64_t*)(ptr+0))=a->adel;
	munmap(ptr, st.st_size);
	close(sd);
	return 0;
}

