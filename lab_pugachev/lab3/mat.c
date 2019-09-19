#include <stdio.h>
#include <stdint.h>
int main(int argc, char* argv[])
{
	FILE *f=fopen(argv[1], "rb+");
	if (f==NULL)
	{
		printf("Smth goes wrong");
		return 0;
	}
	fseek(f, 0 , SEEK_END);
	int razmer=ftell(f);
	fseek(f,0,SEEK_SET);
	printf("%i\n",razmer);
	uint8_t chisl=0x00;
	for (int y=0; y<razmer; y++)
	{
		fwrite(&chisl, sizeof(uint8_t), 1, f);
	}
	fclose(f);
	return 0;
}
