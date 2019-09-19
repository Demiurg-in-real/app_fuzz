#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<stdint.h>
int main(int argc, char* argv[])
{
	int sd; struct stat st; unsigned short *ptr;
	sd=open(argv[1], O_RDWR);
	fstat(sd, &st);
	ptr=(unsigned short*)mmap(NULL,st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED,sd,0); //pochitai pro virtualnuu pamat, a to tak ne sovsem ponyatno s hodu... Mdeeee... Ti tupoi, Andrei
	/*printf("%04x\n",*/ 
	for(int i=0; i<st.st_size; i++){
		if(*((unsigned short*)((uint16_t*)(ptr)+i))==0x5252){
			printf("%i\n",i);
		}
	}
	munmap(ptr, st.st_size);
	close(sd);
	return 0;
}




