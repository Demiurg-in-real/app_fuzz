#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<inttypes.h>
// flags
// 0x01 - only for reading
// 0x02 - hidden file
// 0x04 - system file
// 0x08 - tom mark
// 0x0f - long_name
// 0x10 - katalog
// 0x20 - archive file
//
// free klust - 0
// defektive kluster: 
// 0x0ff7 -fat12 ; 0x0fff7 - fat16 ; 0x0ffffff7 - fat32 ;
// first-last kluster:
// 0x0ff8 - 0x0fff - fat12 ; 0xfff8 - 0xffff - fat16 ; 0x0ffffff8 - 0x0fffffff - fat32 ;
// interpretation of first byte of name:
// 0xe5 - element of katalof is free (deleted) ; 0x00 - element of katalog is free and clear place of katalog
// 0x05 - hz...
//enum flags {free=0xe5, fullfree=0x00};- достаточно бесполезная фигня с точки зрения программирования.
struct fat1216{
	uint8_t instruction[3];
	uint64_t OEM;
	uint16_t size_sector;//0x0b
	uint8_t num_sec_in_klust;//0x0d
	uint16_t res_num_sec_in_klust;//0x0e
	uint8_t num_tables;//0x10
	uint16_t num_files_in_korn;//0x11 - naumber of files in korn. katalog. For fat32 - zero
	uint16_t all_num_sec;//0x13
	uint8_t type_nos;//0x15
	uint16_t num_sect;//0x16 need to copy
	uint16_t coount_of_sec_on_way; //0x18
	uint16_t size_of_heds; //0x1a
	uint32_t size_of_hidden_sec; //0x1c
	uint32_t obsh_num_of_sec;//0x20
	/////for fat12/16
	uint8_t for_disk13h;//0x24
	uint8_t reserved_for_windows; // always has 0, 0x25
	uint8_t sign_more; //0x26
	uint32_t num_log; //0x27
	uint8_t metka[11];// 0x2b
	uint64_t abbrev;//0x36
}__attribute__((packed));
//структура элемента каталога с коротким именем
struct katalog {
	uint8_t short_name[11];
	uint8_t attributes_of_file;
	uint8_t reserved_for_windows_nt;
	uint8_t time_milliseconds;
	uint16_t time_make;
	uint16_t data_make;
	uint16_t last_access;
	uint16_t high_word_first_klust;
	uint16_t time_last_write;
	uint16_t data_last_write;
	uint16_t low_word_first_klust;
	uint32_t size;
}__attribute__((packed));
struct katalog_long_name{
	uint8_t number_of_fragment;
	char first_name[10];
	uint8_t file_attribute;
	uint8_t byte_flag;
	uint8_t kontrol_sum;
	char second_name[12];
	uint16_t numb_first_klust; //must be zero 0
	char third_name[4]; //0x1c
}__attribute__((packed));
struct FSInfo {
	uint32_t signature;
	uint8_t reserved[480];
	uint32_t signature2;
	uint32_t free_klusters;
	uint32_t begin_kluster;
	uint32_t reserved2[3];
	uint32_t signature3;
}__attribute__((packed));
struct table_fat12 {
	uint16_t line;
}__attribute__((packed));
struct table_fat16 {
	uint16_t line;
}__attribute__((packed));
struct table_fat32 {
	uint64_t line;
}__attribute__((packed));


char* get_file(char* name, uint32_t from, uint32_t razmer, int fi);

int main(int argc, char* argv[])
{
	uint8_t *ptr; 
	int file_read;
	uint32_t go_to;
	struct stat statistic;
	struct fat1216 boot_sector;
	file_read=open(argv[1], O_RDONLY);
	if (file_read == -1){
		printf("Some troubles with file. It can be:\n1)Program doesn't have the required permissions.\n2)You didn't specify the first parametr.\n3)You are bitch!\n");
		return 0;
	}
	fstat(file_read, &statistic);
	ptr=mmap(NULL, statistic.st_size, PROT_READ, MAP_PRIVATE,file_read, 0);
	boot_sector=*((struct fat1216*)(ptr+0));
	//printf("%04x\n", boot_sector.OEM);
	go_to=boot_sector.size_sector*(/*boot_sector.num_sec_in_klust*/boot_sector.res_num_sec_in_klust+boot_sector.num_tables*boot_sector.num_sect);//+boot_sector.num_files_in_korn*32;
	//printf("%04x\n",go_to);
	int y=0, z=0;
	struct katalog katal[boot_sector.num_files_in_korn];
	struct katalog_long_name kata[boot_sector.num_files_in_korn];
	struct katalog add_kat[16][16];
	struct katalog_long_name add_kata[16][16];
	for (int i=0; i<=boot_sector.num_files_in_korn*32;i+=32){
		katal[i/32]=*((struct katalog*)(ptr+go_to+i));
		if (katal[i/32].attributes_of_file == 0x0f){
			kata[i/32]=*((struct katalog_long_name*)(ptr+go_to+i));
		}
		if (katal[i/32].attributes_of_file == 0x10){
			do{
			add_kat[z][y]=*((struct katalog*)(ptr+go_to+boot_sector.num_files_in_korn*32+(katal[i/32].low_word_first_klust-2)*boot_sector.size_sector));
			if(add_kat[z][y].attributes_of_file==0x0f){
				add_kata[z][y]=*((struct katalog_long_name*)(ptr+go_to+boot_sector.num_files_in_korn*32+(katal[i/32].low_word_first_klust-2)*boot_sector.size_sector));
			}
			y++;
			//go_to=go_to+boot_sector.num_files_in_korn*32+(katal[i/32].low_word_first_klust-2)*boot_sector.size_sector;
			//printf("Yep %08x\n", go_to);//add_kat[z][y].low_word_first_klust);
			}
			while(add_kat[z][y].low_word_first_klust!=0x0);
			z++;
		}
		//printf("%08x\n", katal[i/32].low_word_first_klust);

	}
	//printf("3\n");
	// adding attributes for reading file
	char* Name="zerp";
	uint16_t offset, size=boot_sector.size_sector*boot_sector.num_sec_in_klust;
	go_to=go_to+boot_sector.num_files_in_korn*32;
	uint32_t where;
	char *simv;
	//writing files
	for(int i=0; i<boot_sector.num_files_in_korn; i++)
	{
		fIle:
		if (katal[i].attributes_of_file == 0x10){
			//
		}
		else
		{
			//printf("4\n");
			if(katal[i].attributes_of_file == 0x0f)
			{
				for(int d=1; d<11; d++)
				{
					printf("hey\n");
					*simv=kata[i].first_name[d];
					strcat(Name,simv);//kata[i].first_name[d]);
					printf("hi!\n");
				}
				for(int d=0; d<3*4;d++)
				{
					strcat(Name, kata[i].second_name[d]);
				}
				for(int d=0; d<4; d++){
					strcat(Name,kata[i].third_name);
				}
			}
			if(katal[i].attributes_of_file!=0x0f && katal[i].attributes_of_file!=0x10 && katal[i].short_name[1]!=0x00){
					offset=katal[i].low_word_first_klust-2;
					where=offset*size+go_to;
					printf("%i\n", i);
					printf("%08x\n", offset);
					Name=get_file(Name,where, katal[i].size, file_read);
				}
			
		}
	}
	return 0;	
}

char* get_file(char* name, uint32_t from, uint32_t razmer, int fi)
{
	uint8_t *simv, *ptr;
	struct stat get;
	fstat(fi, &get);
	ptr=(uint8_t*)mmap(NULL, get.st_size, PROT_READ, MAP_PRIVATE,fi,0);
	FILE *writ=fopen(name, "wb");
	if(writ==NULL) return 0;
	printf("%08x\n", from);
	for(int i=0; i<razmer;i++)
	{
		simv=*((uint8_t*)(ptr+i+from));
		fwrite(&simv, sizeof(uint8_t),1,writ);
	}
	printf("1\n");
	fclose(writ);
	name="";
	return name;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
			Рабочие пометки
не получается сделать 
struct fat1216 *boot_sector;
&boot_sector=*((struct fat1216*)(ptr+0); - выдаёт ошибку объявления левого операнда. Пробовал без & и с *, и просто - всё равно не работает
*/
