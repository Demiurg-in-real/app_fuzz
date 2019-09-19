#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include <stdbool.h>
#include<stdlib.h>
#include<time.h>
int main(int argc, char* argv[])
{
	srand(time(NULL));
	int sd, i=0; struct stat st; short int *ptr, *ptr1;
	sd=open(argv[1], O_RDWR);
	unsigned short gamma=rand();
	bool shelk=false;
	fstat(sd, &st);
	ptr=(unsigned short*)mmap(NULL,st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED,sd,0);
	//while(feof(sd))
	for (i=0; i<1048575; i++)
	{
		if (*((unsigned short*)(ptr+i))==0x1234 && shelk==false)
		{
			*((unsigned short*)(ptr+i))=gamma;
		}
		if (*((unsigned short*)(ptr+i))==0xaa55)
		{
			shelk=true;
			continue;
		}
		if (shelk==true)
		{
			*((unsigned short*)(ptr+i))=(*((unsigned short*)(ptr+i)))^gamma;
		}			
	}
	munmap(ptr, st.st_size);
	close(sd);
	return 0;
}

