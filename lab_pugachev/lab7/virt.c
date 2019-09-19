#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<stdint.h>
int main(int argc, char* argv[])
{
	int sd; struct stat st; uint32_t *ptr,chit;
	sd=open(argv[1], O_RDWR);
	fstat(sd, &st);
	ptr=(uint32_t*)mmap(NULL,st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED,sd,0); //pochitai pro virtualnuu pamat, a to tak ne sovsem ponyatno s hodu... Mdeeee... Ti tupoi, Andrei
	/*printf("%04x\n",*/ chit=*((uint32_t*)((ptr)+0));
	printf("%08X\n", chit);
	munmap(ptr, st.st_size);
	close(sd);
	return 0;
}




