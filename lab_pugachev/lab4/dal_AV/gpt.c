#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#define POLYNOM (0xEDB88320)

uint32_t crc32 (char* ptr, int len)
{
	uint32_t crc=(uint32_t)(~0); //многочлен сост из всех 1 сумма 
	int i;
	while (len--)
	{
		crc ^=*ptr++;//получили новый байт 
		for(i=0; i<8; i++)//смещаем на x^8
			crc=(crc>>1)^((crc &1)?(POLYNOM):(0));
	}
	return crc^(~0);
}	

struct part_mbr {
	uint8_t boot;
	uint8_t CHS1[3];
	uint8_t type;
	uint8_t CHS2[3];
	uint32_t LBA;
	uint32_t size;
}__attribute__((packed));

struct mbr {
	uint8_t boot[446];
	struct part_mbr part[4];
	uint16_t sig;
}__attribute__((packed));	

struct part_gpt {
    uint8_t type[16]; //вики
    uint8_t guid[16];
    uint64_t first;
    uint64_t last;
    uint64_t attr;
    uint8_t name[72];
}__attribute__((packed));

struct header_gpt {
    uint64_t signature;
    uint32_t revision;
    uint32_t header_size;
    uint32_t crc32_header;
    uint32_t reserved;
    uint64_t current_lba;
    uint64_t backup_lba;
    uint64_t first_lba;
    uint64_t last_lba;
    uint8_t guid[16];
    uint64_t starting_lba;
    uint32_t max_part;
    uint32_t size_part;
    uint32_t crc32_part;
}__attribute__((packed));

int main(int argc, char* argv[]){
	srand(time(NULL));
	int fd;
	struct stat st;
	char* ptr;
	fd = open(argv[1], O_RDWR);
	fstat(fd, &st);
	//size_t file_size = st.st_size;
	ptr = (char*)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	int n = 1;
	struct part_gpt tabl[n];
	
	struct header_gpt header ;
	header.signature    = 0x5452415020494645;
	header.revision     = 0x010000;
	header.header_size  = 0x5c;
//	header.crc32_header = 0;
	header.reserved     = 0;
	header.current_lba  = 1;
	header.backup_lba   = 20479; //последний сектор(размер файла(st.st_size) делить на 512(колличество секторов) минус 1 20480-1)
	header.first_lba    = 34;
	header.last_lba     = 20466; //20480 - 34	
	int i;
	for(i=0; i<16; i++)
		header.guid[i] = rand ();
	header.starting_lba = 2;
	header.max_part     = 128;
	header.size_part    = 128;
	header.crc32_part   = crc32((ptr + 2*512), sizeof(tabl));
	
	header.crc32_header = crc32((ptr + 1*512), sizeof(struct header_gpt)) ;
	
    *((struct header_gpt*)(ptr + 512*1)) = header ;
    struct mbr *mbr1 ;
    mbr1 = (struct mbr*) ptr;
    mbr1->sig=0xaa55;
    mbr1->part[0].size = st.st_size/512-1;
    mbr1->part[0].type = 0xee;
    //mbr1->part[0].CHS1[0]=0x02;
    //mbr1->part[0].CHS1[1]=0x00;
    munmap(ptr, st.st_size) ;
    close(fd) ;
    return 0;
}
