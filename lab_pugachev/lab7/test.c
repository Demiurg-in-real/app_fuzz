#include<stdio.h>
#include<stdint.h>
#include<fcntl.h>
int main()
{
	int a[2]={1,2};
	if (a[2]==NULL)
	{
		printf("Good\n");
		return 0;
	}
	/*int fd=open("flopy.img", O_RDONLY);
	if(fd==(-1))
	{
		printf("Error\n");
		return 0;
	}
	uint8_t buf[2];
	uint16_t buf1;
	buf1=read(fd, buf,2);
	printf("%04X\n",buf1);
	uint8_t a=0x0,b=0x0;
	uint16_t c=0xaafe;
	a=a|c;
	b=b|(c>>4);
	printf("a=%04X; b=%04X; c=%04X;\n",a,b,c);*/
	return 0;
}
