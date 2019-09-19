#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<stdint.h>
struct kat{
	uint8_t short_name[11];
	uint8_t attributes;
	uint8_t reseved;
	uint8_t clock;
	uint16_t time;
	uint16_t data;
	uint16_t data_las;
	uint16_t high_word;
	uint16_t last_op_time;
	uint16_t low_word;
	uint32_t size;
}__attribute__((packed));
int main(int argc, char* argv[])
{
	int sd; struct stat st; uint8_t *ptr;struct kat katalog;
	sd=open(argv[1], O_RDWR);
	fstat(sd, &st);
	ptr=(uint8_t*)mmap(NULL,st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED,sd,0); 
	for(int i=0; i<=320; i+=32){
		katalog=*((struct kat*)(ptr+0x14200+i));
		if(katalog.short_name[0]!=0x0 && katalog.short_name[0]!=0xe5)
		{
			printf("Katalog exists %02x\n",katalog.short_name[0]);
		}
		else {
			printf("Katalog is empty %02x\n",katalog.short_name[0]);
		}
	}
	munmap(ptr, st.st_size);
	close(sd);
	return 0;
}




