#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include<time.h>
struct form_MBR
{
	uint32_t aktiv_nach_cil;
	uint32_t type_be_end;
	uint32_t absolute_LBA;
	uint32_t size_sectors;
}__attribute__((packed));
//в число секторов в лба в конце защитного мбр пишется (кол-во секторов на самом файле-1)
//(type_be_end - номер головки диска на которой заканчивается раздел и номер цилиндра и сектора, на котором заканчивается раздел - можно записать =0x00 все
struct GPT
{
	uint32_t signature[2];
	uint32_t zagolovok;
	uint32_t size_zagolovok;
	uint32_t All_crc32;
	uint32_t reserved;
	uint32_t Mylba[2];
	uint64_t Alternatelba;
	uint64_t Firstusablelba;
	uint64_t Lastusablelba;
	uint32_t DiskGUID[4];
	uint32_t Partitionlba[2];
	uint32_t numberofpartent;
	uint32_t sizeofpart;
	uint32_t Partitioncrc32[2];
	uint32_t reserved2[105];
} __attribute__((packed));
struct razdel
{
	uint32_t typeGUID[4];
	uint32_t uniqueGUID[4];
	uint64_t startLBA;
	uint64_t endLBA;
	uint32_t attributes[2];
	uint32_t typeofRazd[18];
}__attribute__((packed));

void zapis1(unsigned char *file09, unsigned char *file08, unsigned int smesh1, unsigned int smesh2);
uint32_t **lik_read(uint32_t *gptstr, uint32_t **massive, uint8_t *file1, unsigned int number);
unsigned int format_MBR( struct form_MBR *MbR, unsigned int razmer234);//done
unsigned int  format_GPT (struct GPT *gpt, unsigned int razmer500);//done
uint32_t crc32();//done
void format_razdel();//done
uint32_t  guid(uint32_t generate);//done
int zapis(struct form_MBR *Mbr, struct GPT *Gpt, struct razdel *Raz,uint32_t razmer, unsigned int number,uint8_t *file);//done
unsigned int virovn(unsigned char *file1, unsigned int razmer);//done 
//void right_swap(char *file1, char *file2);
int main(int argc, unsigned char* argv[])
{
	struct form_MBR mbr;
	struct GPT gpt1;
	struct razdel raz;
	unsigned int razmer123=0;
	int i=2;
	uint8_t *wre=0x00;
	if(argv[i]==0)
	{
		printf("he");
	}
	while(argv[i]!=NULL)//считаем общий объём памяти, что займут файлы
	{
		//printf("1");
		FILE *chech=fopen(argv[i], "rb+");
		fseek(chech,0,SEEK_END);
		printf("%i\n",ftell(chech));
		razmer123+=ftell(chech);
		printf("%i\n", razmer123);
		i++;
		fclose(chech);
	}
	printf("%i\n",razmer123);
	if((razmer123%512)!=0)
	{
		razmer123=(razmer123+razmer123%512);
	}
	printf("%i\n",razmer123);
	FILE *kino=fopen(argv[1], "wb");
	for(int y=0; y<(razmer123+(2048+33)*512);y++)
	{
		fwrite(&wre, sizeof(uint8_t),1,kino);
	}
	razmer123=razmer123/512;
	printf("%i\n",razmer123);
	//virovn(argv[1], razmer123);
	//printf("1\n");- нашёл
	format_MBR(&mbr,razmer123);
	printf("1\n");
	format_GPT(&gpt1, razmer123);
	printf("%i 2\n", razmer123);
	printf("%i he\n", gpt1.signature[0]);
	zapis(&mbr,&gpt1,&raz,razmer123,(i),argv[1]);
	for(int dis=2;dis<i+1;dis++)
	{
		unsigned int smes,smes1,smes2;
		smes=(razmer123)/(i);
		smes1=2048+(smes+1)*(dis-2);
		smes2=2048+smes*(dis-1);
		zapis1(argv[dis],argv[1],smes1,smes2);
	}
	return 0;
}
unsigned int virovn(unsigned char *file1, unsigned int razmer)//сделано
{
	FILE *openf=fopen(file1, "rb+"); uint8_t *wr=0x00;//int razmer;
	if (openf==NULL)
	{
		printf("Ya rodilsa, a ti idi nahui");
		return 0;
	}
	fseek(openf, 0, SEEK_END);
	unsigned int med=ftell(openf);
	razmer=med/512;
	printf("%i, %i tut\n",razmer, med%512);
	if((med%512)!=0)
	{
		med=(512-(med-razmer*512));
		printf("%i\n",med);
		for(int i=0; i<(med); i++)
		{
			fwrite(&wr, sizeof(uint8_t),1,openf);
		}
		razmer++;
	}
	fclose(openf);
	printf("%i 1\n",(razmer));//после этого вылетает долбаный сегфолт - откуда? всё верно передаётся же.
	return razmer;//тут разве не указатель должен возвращаться?
}
unsigned int format_MBR(struct form_MBR *mbR, unsigned int razmer234)
{
	//struct form_MBR *MbR;//=mbR;- на этом выдаёт ошибку. Почему?
	printf("%i\n", razmer234);
	mbR->aktiv_nach_cil=0x20000;
	printf("fuck\n");
	mbR->type_be_end=0xffffffee;
	mbR->absolute_LBA=0x1;
	printf("ha\n");
	mbR->size_sectors=(razmer234-1);
	printf("2\n");	//вроде верно
	return 0;
}
unsigned int  format_GPT (struct GPT *gpt, unsigned int razmer500)
{
	//struct GPT *gpt;
	gpt->signature[1]=0x54524150;
	gpt->signature[0]=0x20494645;
	gpt->zagolovok=0x00010000;
	gpt->size_zagolovok=0x0000005c;
	gpt->reserved=0x00000000;
	gpt->Mylba[0]=0x00000001;
	gpt->Mylba[1]=0x00000000;
	gpt->Alternatelba=(razmer500/-(0x1));
	gpt->Firstusablelba=0x800;
	gpt->Lastusablelba=(razmer500-(0x21));
	gpt->Partitionlba[0]=0x00000002;
	gpt->Partitionlba[1]=0x00000000;
	gpt->numberofpartent=0x00000080;
	gpt->sizeofpart=0x00000080;
	for(int i=0; i<105;i++)
	{
		gpt->reserved2[i]=0x00000000;
	}
	for(int i=0;i<4; i++)
	{
		uint32_t nas=guid(nas);
		printf("%i\n", nas);
		gpt->DiskGUID[i]=nas;
	}
	//printf("3\n");
	return 0;
}//сойдёт
void format_razdel(struct razdel *raz,int nachalo,int konec)
{
	raz->typeGUID[0]=0xc12a7328;
	raz->typeGUID[1]=0x11d2f81f;
	raz->typeGUID[2]=0xa0004bba;
	raz->typeGUID[3]=0x3bc93ec9;
	uint32_t smesh;
	for(int i=0; i<4; i++)
	{
		uint32_t smesh=guid(smesh);
		raz->uniqueGUID[i]=smesh;
	}
	raz->startLBA=nachalo; //!!!
	raz->endLBA=konec;
	raz->attributes[0]=0x00;
	raz->attributes[1]=0x00;
	uint32_t array1[18]={0x0069004c,0x00790073,0x00200078,0x00690066,0x0065006c,0x00790073,0x00740073,0x006d0065,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	for (int i=0;i<18;i++)
	{
		raz->typeofRazd[i]=array1[i];
	}
	//printf("4\n");

}//сойдёт
uint32_t guid(uint32_t generate)
{
	//uint32_t stime1=time(NULL);
	srand(time(NULL));
	/*uint32_t*/ generate=rand();
	//printf("5\n");
	return generate;
}

unsigned int crc32(uint32_t *message)//(unsigned char *message) 
{
	int i, j;
	uint32_t byte, crc, mask;
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
		i= i + 1;
	}
	printf("8\n");
	return ~crc;
}//сойдёт
int zapis(struct form_MBR *Mbr, struct GPT *Gpt, struct razdel *Raz,uint32_t razmer, unsigned int number,uint8_t *file)//so schetom dovedi do uma
{
	//struct form_MBR Mbr;struct GPT Gpt; struct razdel *Raz;struct form_MBR mBr;
	uint32_t *ptr1; uint64_t *ptr2; int openf,schet=0x1be; struct stat st; unsigned int Razmer, razmer1,razmer2;
	openf=open(file,O_RDWR);
	fstat(openf,&st);
	printf("hew\n");
	ptr1=(uint32_t*)mmap(NULL, st.st_size,PROT_READ | PROT_WRITE, MAP_SHARED, openf,0);
	ptr2=(uint64_t*)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED,openf,0);
	printf("hew3 %08X \n",Gpt->signature[1]);
	*((uint32_t*)(ptr1+schet))=Mbr->aktiv_nach_cil;
	*((uint32_t*)(ptr1+schet+4))=Mbr->type_be_end;
	*((uint32_t*)(ptr1+schet+8))=Mbr->absolute_LBA;
	*((uint32_t*)(ptr1+schet+12))=Mbr->size_sectors;
	schet=512;
	printf("hew2\n");
	*((uint32_t*)(ptr1+schet-2))=0xaa55;
	*((uint32_t*)(ptr1+schet))=Gpt->signature[0];
	*((uint32_t*)(ptr1+schet+4))=Gpt->signature[1];
	*((uint32_t*)(ptr1+schet+8))=Gpt->zagolovok;
	*((uint32_t*)(ptr1+schet+12))=Gpt->size_zagolovok;
	*((uint32_t*)(ptr1+schet+20))=Gpt->reserved;
	*((uint32_t*)(ptr1+schet+24))=Gpt->Mylba[0];
	*((uint32_t*)(ptr1+schet+28))=Gpt->Mylba[1];
	*((uint64_t*)(ptr2+schet+32))=Gpt->Alternatelba;
	*((uint64_t*)(ptr2+schet+40))=Gpt->Firstusablelba;
	*((uint64_t*)(ptr2+schet+48))=Gpt->Lastusablelba;
	//*((uint32_t*)(ptr1+schet))=Gpt->
	schet+=56;
	printf("hew1\n");
	for(int i=0;i<4;i++)
	{
		*((uint32_t*)(ptr1+schet))=Gpt->DiskGUID[i];
		schet+=4;
	}
	*((uint32_t*)(ptr1+schet))=Gpt->Partitionlba[0];
	*((uint32_t*)(ptr1+schet+4))=Gpt->Partitionlba[1];
	*((uint32_t*)(ptr1+schet+8))=Gpt->numberofpartent;
	*((uint32_t*)(ptr1+schet+12))=Gpt->sizeofpart;
	schet+=16;
	for (int i=0; i<105;i++)
	{
		*((uint32_t*)(ptr1+schet))=Gpt->reserved2[i];
		schet+=4;
	}
	for (int i=0;i<(number); i++)
	{
		Razmer=(razmer/(0x200))/(number);
		razmer1=2048+(Razmer+1)*i;
		razmer2=2048+Razmer*(i+1);
		format_razdel(Raz,razmer1,razmer2);	
		for(int y=0;y<4;y++)
		{
			*((uint32_t*)(ptr1+schet))=Raz->typeGUID[i];
			*((uint32_t*)(ptr1+schet+16))=Raz->uniqueGUID[i];
			schet+=4;
		}
		*((uint64_t*)(ptr2+schet))=Raz->startLBA;
		schet+=8;
		*((uint64_t*)(ptr2+schet))=Raz->endLBA;
		schet+=8;
		*((uint32_t*)(ptr1+schet))=Raz->attributes[0];
		schet+=4;
		*((uint32_t*)(ptr1+schet))=Raz->attributes[1];
		schet+=4;
		for(int y=0;y<18;y++)
		{
			*((uint32_t*)(ptr1+schet))=Raz->typeofRazd[i];
			schet+=4;
		}
		printf("%i\n",i);
	}
	printf("doshol\n");
	munmap(ptr1,st.st_size);
	munmap(ptr2,st.st_size);
	close(openf);
	//del(openf);
	int number12=number;
	uint32_t massive[number12][32], gptmas[128];//должно работать, я хз
	printf("hop\n");
	lik_read(gptmas, massive, file,number);
	printf("mb tut?\n");
	FILE *openf1=fopen(file, "rb+");
	if(openf1==NULL)
	{
		printf("Daleko zashol, no poshol nahui");
		return 0;
	}
	uint32_t *wr,op;
	fseek(openf1,512,SEEK_SET);
	printf("doshik\n");
	for (int y=0; y<512/4; y++)
	{
		op=gptmas[y];
		*wr=op;
		fwrite(&wr, sizeof(uint32_t),1,openf1);
	}
	for (int i=0; i<number;i++)
	{
		for (int y=0; y<128/4; y++)
		{
			op=massive[i][y];
			*wr=op;
			fwrite(&wr, sizeof(uint32_t),1,openf1);
		}
	}
	fseek(openf1, -33*512, SEEK_END);
	for(int i=0; i<number;i++)
	{
		for(int y=0; y<128/4; y++)
		{
			op=massive[i][y];
			*wr=op;
			fwrite(&wr, sizeof(uint32_t),1,openf1);
		}
	}
	fseek(openf1, -512,SEEK_END);
	for(int i=0; i<512;i++)
	{
		*wr=gptmas[i];
		fwrite(&gptmas,sizeof(uint32_t),1,openf1);
	}
	printf("6\n");
	return 0;
}
uint32_t **lik_read(uint32_t *gptstr, uint32_t **massive, uint8_t *file1, unsigned int number1)
{
	unsigned int number=number1;
	FILE *openf=fopen(file1, "rb+");uint32_t *chit, massive1[number*32],gptstr1[128];int schet=0;//uint8_t gptstr[512],massive[number][128];
	bool shelk=true;
	if(openf==NULL)
	{
		printf("Pshol nah");
		return 0;
	}
	fseek(openf,512, SEEK_SET);
	for (int i=0;i<number;i++)
	{
		if(shelk)
		{
			for (int y=0;y<512/4;y++)
			{
				gptstr[y]=fread(&chit,sizeof(uint32_t), 1, openf);
				gptstr1[y]=gptstr[y];
			}
			shelk=false;
		}
		for (int y=0; y<128/4;y++)
		{
			massive[i][y]=fread(&chit, sizeof(uint32_t),1,openf);
			massive1[schet]=massive[i][y];
			schet++;
		}
	}
//ne hvataet рассчёта crc32 и дозаписи в конце (ну это уже через fopen сделаю, хер ли мудачиться просто такё

	gptstr[22]=crc32(massive1);
	gptstr1[22]=gptstr[22];
	gptstr[4]=crc32(gptstr1);
	printf("7\n");
	return gptstr, massive;
}
void zapis1(unsigned char *file09, unsigned char *file08, unsigned int smesh1, unsigned int smesh2)
{
	FILE *perv=fopen(file09,"rb"), *vtor=fopen(file08,"rb+");
	uint8_t *che;
	fseek(vtor,smesh1,SEEK_SET);
	for(int pzdc=smesh1*512;pzdc<=smesh2*512;pzdc++)
	{
		fread(&che,sizeof(uint8_t),1,perv);
		fwrite(&che,sizeof(uint8_t),1,vtor);
	}
	fclose(perv);
	fclose(vtor);
}

