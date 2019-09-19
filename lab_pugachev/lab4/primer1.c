#include <stdint>

//vstavil

#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include <stdbool.h>
#include<stdlib.h>

//vstavil
int razmer(int chislo, char argum)
{
	FILE *magad=fopen(argum, "rb");
	if (magad==NULL)
	{
		printf("Something went wrong, please check it");
		return (32);
	}
	fseek (magad, 0, SEEK_END);
	chislo=ftell(magad);
	fclose(magad);
	return chislo;
}

struct part {
	uint8_t boot; //unsigned int - more universal
	uint8_t chs[3];
	uint8_t type;
	uint8_t chs2[3];
	uint8_t LBA;
	uint8_t size;//System 5-  рассказывается, как компилятор должен собирать сишный код. Под винду есть заморочка с упаковкой структур (для того, чтобы память корректно занимало. __attribute__((packed)); - атрибут для того, чтобы эту структуру компилятор "упаковал" корректно. Пишется после скобки структуры
}
struct mbr {
	uint8_t code[446];
	struct part P[4];
	uint16_t sig;
}__attribute__((packed));
struct mbr *mdr;
mbr=(struct mbr*) ptr;
mbr->sig=0xaa55;
mbr->P[0].type=0xee
mbr->P[0].lba=1;
mbr->P[0].size=st.st_size/512-1;





int main()
{
	int sd, i=0; struct stat st; short int *ptr;
	sd=open(argv[1], O_RDWR);
	fstat(sd,&st);
	ptr=(unsigned short*)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, sd, 0);
	munmap(ptr, st.st_size);
	close(sd);
	return 0;
