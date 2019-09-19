#include <stdint.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include <stdbool.h>
#include<stdlib.h>
#include <time.h>
//unsigned int - more universal
//System 5-  рассказывается, как компилятор должен собирать сишный код. Под винду есть заморочка с упаковкой структур (для того, чтобы память корректно занимало. __attribute__((packed)); - атрибут для того, чтобы эту структуру компилятор "упаковал" корректно. Пишется после скобки структуры
struct GPT_MADE {
	uint8_t signature[8];
	uint8_t revision[4];
	uint8_t headersize[4];
	uint32_t headercrc32;
	uint8_t reserved[4];
	uint8_t mylba[8];
	uint8_t alternallba[8];
	uint8_t firstuslba[8];
	uint8_t lastuslba[8];
	uint8_t uniqGUID[16];
	uint8_t partentr[8];
	uint8_t numbpart[4];
	uint8_t sizepart[4];
	uint32_t partentrCRC32;
	uint8_t reserved2; //Может удалить? Оно нафиг не нужно, по сути. Места под этот резерв я не оставляю же
} __attribute__((packed));

struct zagolovok {
	uint8_t parttype[16];
	uint8_t uniqGUID[16];
	uint8_t slba[8];
	uint8_t elba[8];
	uint8_t attributes[8];
	uint8_t name[72];
}__attribute__((packed));
/*uint_least32_t*/ void Crc32(unsigned char *buf, size_t len, struct GPT_MADE *suda, int chislo);
int razmer(int chislo, char *argum);
void generate_GUID(struct GPT_MADE *vnes);
void zabiv_structuri(struct GPT_MADE *standartn, int RAZMER); 
void zabiv_zagolovka(struct zagolovok *vbiv, int razme, uint8_t chislo, int nomer);
void sborka(struct zagolovok *vb, struct GPT_MADE *st, int razmer, char *nechto);
void generate_GUID1(struct zagolovok *vnis); 
	
const uint8_t for_first_mbr[12]={0x02,0x00,0xee, 0x46, 0x05, 0x01, 0x01, 0x00,0x00,0x00, 0xff, 0x4f};

int main(int argc, char* argv[])
{
		struct GPT_MADE *GPT;
	struct zagolovok *MADEN;
	int razm, ig=1;
	char *figt=argv[1]; uint8_t *figt1=argv[2];
	razm=razmer(razm, figt);
	zabiv_structuri(GPT, razm);
	zabiv_zagolovka(MADEN, razm, figt1, ig); // i - менялось бы при каждой итерации при записи, но цикл записи написать не успел
	int sd, i=0; struct stat st; short int *ptr;
	sd=open(argv[1], O_RDWR);
	fstat(sd,&st);
	ptr=(unsigned short*)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, sd, 0);
	//*((unsigned short*)((ptr+500))=(*((... // zapis
	munmap(ptr, st.st_size);
	close(sd);
	return 0;
}
int razmer(int chislo, char *argum)
{
	FILE *magad=fopen(argum, "rb");
	if (magad==NULL)
	{
		printf("Something went wrong, check the code, pls");
		return 24;
	}
	fseek(magad, 0, SEEK_END);
	chislo=ftell(magad);
	fclose(magad);
	return chislo;
}
/*uint_least32_t*/void Crc32(unsigned char *buf, size_t len, struct GPT_MADE *suda, int chislo) //Подсчёт контрольной суммы
{
	uint_least32_t crc_table[256];
	uint_least32_t crc; int i, j;
	for (i = 0; i < 256; i++)
	{
		crc = i;
	        for (j = 0; j < 8; j++)
		{
			crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
		}
	        crc_table[i] = crc;
	}
	crc = 0xFFFFFFFFUL;
	while (len--)
	{
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
	}
	if (chislo==1)
	{
		suda->partentrCRC32=(crc ^ 0xFFFFFFFFUL);
	}
	else
	{
		suda->headercrc32=(crc ^ 0xFFFFFFFFUL);
	}
	//return crc ^ 0xFFFFFFFFUL;
}
void generate_GUID(struct GPT_MADE *vnes) //Генерирую и сразу заполняю GUID
{
	srand((uint32_t) time(NULL));
	unsigned char *stream=malloc(16);
	size_t i;
	for (i=0; i<16; i++)
	{
		stream[i]=rand();
	}
	for (i=0; i<16;i++)
	{
		vnes->uniqGUID[i]=stream[i];
	}
}
void generate_GUID1(struct zagolovok *vnis) //Генерирую и сразу заполняю GUID
{
	srand((uint32_t) time(NULL));
	unsigned char *stream=malloc(16);
	size_t i;
	for (i=0; i<16; i++)
	{
		stream[i]=rand();
	}
	for (i=0; i<16;i++)
	{
		vnis->uniqGUID[i]=stream[i];
	}
}

void zabiv_structuri(struct GPT_MADE *standartn, int RAZMER) //Заполняю основные данные GPT, кроме CRC32, GUID и таблицы разделов диска
{
	char sign[8]={0x45, 0x46, 0x49, 0x20, 0x50, 0x41, 0x52, 0x54};
	char revis[4]={0x00,0x00,0x01,0x00};
	char hesi[4]={0x5c, 0x00,0x00,0x00};
	char mlba[8]={0x01, 0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	char alLBA[8]={}; int razd1=(RAZMER/512-1);
	char FLBA[8]={0x22, 0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	char LLBA[8]={}; int razd2=(RAZMER/512-34);
	char pentr[8]={0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	char npart[4]={0x80,0x00,0x00,0x00};
	char spart[4]={0x80,0x00,0x00,0x00};
	for (int i=0; i<8;i++)
	{
		for (int y=i*8;y<i*8+8; y++)
		{
			alLBA[i]=(alLBA[i]<<1)|((razd1>>y)&0x1);
			LLBA[i]=(LLBA[i]<<1)|((razd2>>y)&0x1);
		}
	}
	for(int i=0;i<4;i++)
	{
		standartn->revision[i]=revis[i];
		standartn->headersize[i]=hesi[i];
		standartn->reserved[i]=0x00;
		standartn->numbpart[i]=npart[i];
		standartn->sizepart[i]=spart[i];
	}
	for(int i=0;i<8;i++)
	{
		standartn->signature[i]=sign[i];
		standartn->mylba[i]=mlba[i];
		standartn->alternallba[i]=alLBA[i];
		standartn->firstuslba[i]=FLBA[i];
		standartn->lastuslba[i]=LLBA[i];
		standartn->partentr[i]=pentr[i];
	}
	generate_GUID1(standartn);
}
void zabiv_zagolovka(struct zagolovok *vbiv, int razme, uint8_t chislo, int nomer)
{
	char ptype[16]={0x28, 0x73, 0x2a, 0xc1, 0x1f, 0xf8, 0xd2, 0x11, 0xba, 0x4b, 0x00, 0xa0, 0xc9, 0x3e, 0xc9, 0x3b};
	generate_GUID1(vbiv);
	char firlba[8]={};int raz1=((razme/chislo)*nomer);
	char enlba[8]={}; int raz2=((razme/chislo+1)*nomer);
	for(int i=0;i<16;i++)
	{
		vbiv->parttype[i]=ptype[i];
	}
	for (int i=0;i<8;i++)
	{
		for (int y=i*8;i<i*8+8;y++)
		{
			firlba[i]=(firlba[i]<<1)|((raz1>>y)&0x1);
			enlba[i]=(enlba[i]<<1)|((raz2>>y)&0x1);
		}
		vbiv->slba[i]=firlba[i];
		vbiv->elba[i]=enlba[i];
		vbiv->attributes[i]=0x00;
	}
	for (int i=0; i<72; i++)
	{
		vbiv->name[i]=0x00;
	}
}	
void sborka(struct zagolovok *vb, struct GPT_MADE *st, int razmer, char *nechto)
{
	int sd, i=0,y=0; struct stat st1; uint8_t *ptr;
	bool MbR=true, mainrazd=false,razd1=false,razd2=false,razd3=false, razd4=false, razd5=false,razd6=false,razd7=false,razd8=false;
	sd=open(nechto, O_RDWR);
	fstat(sd, &st1);
	ptr=(uint8_t*)mmap(NULL, st1.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, sd,0);
	for (i=0; i<razmer; i++)
	{
		if (i==448 && MbR)// Записываю стандартную запись защитного мбр раздела
		{
			*((uint8_t*)(ptr+i))=for_first_mbr[y];
			y++;
			if (y==16)
			{
				y=0;
				MbR=false;
			}
		}
		if (i==510)//Закончил с защитным мбр разделом
		{
			*((uint8_t*)(ptr+i))=0x55;
			i++;
			*((uint8_t*)(ptr+i))=0xaa;
			mainrazd=true;
		}
		if (mainrazd)
		{
			if (razd1)
			{
				*((uint8_t*)(ptr+i))=st->signature[y];
				y++;
				if (y==8)
				{
					razd1=false;
					y=0;
					razd2=true;
				}
			}
			if(razd2)
			{
				*((uint8_t*)(ptr+i))=st->revision[y];
				y++;
				if (y==4)
				{
					razd2=false;
					y=0;
					razd3=true;
				}
			}
			if (razd3)
			{
				*((uint8_t*)(ptr+i))=st->headersize[y];
				y++;
				if(y==4)
				{
					razd3=false;
					y=0;
					razd4=true;
					i+=4;
				}
			}
			if (razd4)
			{
				*((uint8_t*)(ptr+i))=st->reserved[y];
				y++;
				if (y==4)
				{
					razd4=false;
					y=0;
					razd5=true;
				}
			}
			if (razd5)
			{
				*((uint8_t*)(ptr+i))=st->mylba[y];
				*((uint8_t*)(ptr+i+8))=st->alternallba[y];
				*((uint8_t*)(ptr+i+16))=st->firstuslba[y];
				*((uint8_t*)(ptr+i+24))=st->lastuslba[y];
				y++;
				if (y==8)
				{
					razd5=false;
					y=0;
					i+=24;
					razd6=true;
				}
			}
			if(razd6)
			{
				*((uint8_t*)(ptr+i))=st->uniqGUID[y];
				y++;
				if(y==16)
				{
					razd6=false;
					y=0;
					razd7=true;
				}
			}
			if(razd7)
			{
				*((uint8_t*)(ptr+i))=st->partentr[y];
				y++;
				if (y==8)
				{
					razd7=false;
					y=0;
					razd8=true;
				}
			}
			if (razd8)
			{
				*((uint8_t*)(ptr+i))=st->numbpart[y];
				*((uint8_t*)(ptr+i+4))=st->sizepart[y];
				y++;
				if(y==4)
				{
					razd8=false;
					y=0;
					i+=8;
				}
			}
		}
	}
}
