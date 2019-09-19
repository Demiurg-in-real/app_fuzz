#include<inttypes.h>
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>
#include<dirent.h>
#include<string.h>
static unsigned char PI[8][16]=
{
	 {1,7,14,13,0,5,8,3,4,15,10,6,9,12,11,2},
	 {8,14,2,5,6,9,1,12,15,4,11,0,13,10,3,7},
	 {5,13,15,6,9,2,12,10,11,7,8,1,4,3,14,0},
	 {7,15,5,10,8,1,6,13,0,9,3,14,11,4,2,12},
	 {12,8,2,1,13,4,15,6,7,0,10,5,3,14,9,11},
	 {11,3,5,8,2,15,10,13,14,1,7,4,12,9,6,0},
	 {6,8,2,3,9,10,5,12,1,14,4,7,11,13,0,15},
	 {12,4,6,2,10,5,11,9,14,8,13,7,0,3,15,1}
};

//static unsigned uint32_t key[8] = 

uint64_t magma(uint64_t block, uint32_t kluch[8]) //Магма считается по алгоритму, вроде верно, как и написано в ГОСТ (34.12-2015)
{
	printf("%16x\n",block);
	uint32_t block2=block2|(block>>32);
	uint32_t block1=block, swap;
	uint8_t byte[4], fbp,sbp;
	int round, schet=0;
	uint64_t izn=0x0;
	for (int i=0;i<4;i++)
	{
		byte[i]=0x0;
	}
	for (round=0; round<32; round++)
	{
		/*if (round>=24)
		{
			if(round==24) schet=7;
			block1=block1+kluch[7-round%8];
			schet--;
		}
		else
		{
			if(schet==8) schet=0;
			block1=block1+kluch[round%8];
			schet++;
		}
		for (int i=0;i<4;i++)
		{
			byte[i]=0x0|(block1>>8*i);
		}

		for (int i=0; i<4;i++)
		{
			fbp=(byte[i]&0xf0)>>4;
			sbp=(byte[i]&0x0f);
			fbp=PI[i*2][fbp];
			sbp=PI[i*2+1][sbp];
			byte[i]=(fbp<<4)|sbp;
		}
		for (int i=3; i>=0; i--)
		{
			block1=(block1<<8)|byte[i];
		}
		block1=(block1<<11)|(block1>>21);
		swap=block1;
		block1=block2;
		block2=swap;
	}
	izn=((izn|block1)<<32)|block2;
	printf("%16x\n",izn);*/
	return izn;
}

uint64_t imitovst(uint64_t fB, uint64_t sB, uint32_t kluch[8], bool EnD) //имитовставка. Надеюсь я верно разобрался в том, как генерируются ключи для ксора последнего блока(ГОСТ 34.13-2015)
{
	fB=magma(fB,kluch);
	fB=fB^sB;
	int i=0;
	uint64_t k1,k2;
	if(EnD)
	{
		sB=magma(sB,kluch);
		if((sB>>63)==0x00)
		{
			k1=(sB<<1);
			fB=fB^k1;
		}
		else
		{
			k1=sB<<1;
			while(((k1<<i)>>63)!=0x01)
			{
				k2=(k1<<i);
				i++;
			}
			fB=fB^k2;
		}
		return fB;
	}
	fB=magma(fB, kluch);
	fB=fB^sB;
	return sB;
}

int main(int args, char* argv[]) //неудачная попытка придумать рекурсивный обход директорий. Падает segfault'ом, когда встречает другую дирректорию. + ошибка где-то в логике окончания папки с файлом.Поищу до пары косяк и исправлю. В работе scandir разобрался и в одной папке получилось посчитать имитовставку для каждого файла, пропуская дирректории. Хотел усложнить рекурсивный обход дирректорий - не вышло.
{
	//struct dirent **namelist;
	uint64_t *openTEXT, *ptr, R=0x0;
	uint64_t retur=0x92def06b3c130a59;
	uint32_t kluch[8]={0xffeeddcc,0xbbaa9988, 0x77665544, 0x33221100, 0xf0f1f2f3,0xf4f5f6f7, 0xf8f9fafb,0xfcfdfeff};
	//char* dirs[100];
	//bool addition=false, enD=true;
	//int n,pap=1,razmer,razmer1,zapas;
	printf("%"PRIu64 "\n", retur);
	retur=magma(retur,kluch);
	printf("%08x\n", retur);
	return 0;
}
	/*for (int i=0; i<8; i++)
	{
		srand(time(NULL)+((i/2+8)*14));
		kluch[i]=rand();
	}
	dirs[0]=argv[1];
	while (pap!=0)
	{
		n=scandir(dirs[pap-1],&namelist, NULL, alphasort);
		if (n == -1)
		{
			printf("The end of dir\n");
		}
		while(n--)
		{
			if(scandir(dirs[pap],&namelist,NULL,alphasort)==-1)
			{
				n--;
				FILE *fil=fopen(namelist[n]->d_name,"rb");
				fseek(fil,0,SEEK_END);
				zapas=ftell(fil);
				fseek(fil, 0, SEEK_SET);
				razmer=zapas/8;
				if(zapas%8!=0)
				{
					razmer1=zapas%8;
					razmer++;
					addition=true;
				}
				openTEXT=(uint64_t*)malloc(sizeof(uint64_t));
				for(int i=0; i<razmer; i++)
				{
					if(addition)
					{
						openTEXT[i]=(openTEXT[i]<<(8*razmer1));
						addition=false;
						break;
					}
					openTEXT[i]=fread(&ptr, sizeof(uint64_t),1, fil);
				}
				for (int i=0; i<razmer;i++)
				{
					if(i==(razmer-1))
					{
						enD=true;
						retur=imitovst(openTEXT[i],R,kluch,enD);
						enD=false;
						printf("./%c\n%08X\n", dirs[pap], retur);
					}
					else
					{
						openTEXT[i+1]=imitovst(openTEXT[i],openTEXT[i+1], kluch, enD);
					}
				}
				fclose(fil);
				free(namelist[n]);
			}
			else
			{
				if(namelist[n]->d_name!="." || namelist[n]->d_name!="..")
				{
					dirs[pap]=strcat(dirs[pap],"/");
					dirs[pap]=strcat(dirs[pap], namelist[n]->d_name);
					free(namelist[n]);
					n--;
					pap++;
				}
				else
				{
					continue;
				}
			}
			free(namelist);
		}
	}
}
*/
