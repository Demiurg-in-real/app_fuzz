#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct part {
	uint8_t	flag;
	uint8_t	st_chs[3];
	uint8_t	tip;
	uint8_t	fin_chs[3];
	uint32_t st_lba;
	uint32_t size;
};
struct mbr {
	unsigned char	code[446];
	struct part		p[4];
	unsigned short	sign;
};__attribute__((packed));

unsigned int crc32b(unsigned char *message) {
   int i, j;
   unsigned int byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   while (message[i] != 0)
    {
      byte = message[i];           
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--)
      {   
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}
int main (int argc, char* argv[])
{
	
	int		fd;
	char	*ptr;
	struct stat	st;
	struct mbr *mbr;
	int i;
	if ((fd=open(argv[1],O_RDWR))==-1)
	{
		printf("File not found\n");
		return -1;
	}
	fstat(fd, &st);
	ptr = (char *)mmap(NULL, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	memset(ptr,0,st.st_size);
	mbr = (struct mbr*)ptr;

	mbr->sign = 0xaa55;
	mbr->p[0].tip =0xee;
	mbr->p[0].st_lba = 1;
	mbr->p[0].size = st.st_size/512-1;
	
	
	return 0;	
}
