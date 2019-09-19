#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdint.h>
//#include<io.h>
void right_swap(char *file1, char *file2);
int main(int argc, char* argv[])
{
	right_swap(argv[1], argv[2]);
	return 0;
}
void right_swap(char *file1, char *file2)
{
	int x,y,z,schet=0,const1=1024, value=const1,openf;
	unsigned char *ptr, pixels[1024][1024][3], a,b,c,d;
	struct stat st;
	openf=open(file1, O_RDONLY);
	fstat(openf, &st);
	ptr=(uint8_t*)mmap(NULL, st.st_size, PROT_READ , MAP_PRIVATE, openf, 0);
	for (x=0;x<1024;x++)
	{
		for (y=0; y<1024;y++)
		{
			for (z=0; z<3;z++)
			{
				pixels[x][y][z]=*((uint8_t*)(ptr+56670+schet));
				schet++;
				//printf("1");
			}
			//printf("2");
		}
		printf("%i\n", schet);
	}// заканчивает цикл и программу тошнит ошибкой сегментирования. Просто хз что делать
	//printf("4");//вот этот момент программа уже не выполняет, заканчивая цикл вылетает с ошибкой
	munmap(ptr, st.st_size);
	close(openf);
	//printf("5");
	for(x=0; x<512; x++)
	{
		for(y=0;y<(const1-1);y++)
		{
			for (z=0;z<3;z++)
			{
				a=pixels[x][y][z];
				b=pixels[y][const1][z];
				c=pixels[const1][value][z];
				d=pixels[value][x][z];
				pixels[x][y][z]=b;
				pixels[y][const1][z]=c;
				pixels[const1][value][z]=d;
				pixels[value][x][z]=a;
			}
		}
	}
	schet=0;
	openf=open(file2, O_WRONLY | O_CREAT, S_IWRITE | S_IREAD);
	fstat(openf, &st);
	ptr=(uint8_t*)mmap(NULL, st.st_size, PROT_WRITE, MAP_SHARED, openf,1);
	for(x=0;x<1024;x++)
	{
		for(y=0;y<1024;y++)
		{
			for(z=0;z<3;z++)
			{
				//a=pixels[x][y][z];
				*((uint8_t*)(ptr+schet))=pixels[x][y][z];
				schet++;
			}
		}
	}
}
