#include<stdio.h>
int main()
{
	FILE *testi=fopen("flopy.img", "rb"), *result=fopen("result.img", "wb+");
	short int initin;
	for(int i=0; i<(1048575); i++)
	{
		if (feof(testi))
		{
			break;
		}
		if (i<=(512/2-1) || i>=((512/2-1)+143))
		{
			fread(&initin, sizeof(short int), 1, testi);
			fwrite(&initin, sizeof(short int), 1, result);
			//printf("%i", i);
			continue;
		}
		else
		{
			fread(&initin, sizeof(short int),1, testi);
			initin=initin+0x2020;
			fwrite(&initin, sizeof(short int),1,result);
			//printf("%i", i);
		}
	}
	return 0;
}
