#[macro_use] extern crate honggfuzz;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stribog.h"

#define USER_COUNT (6)
unsigned char *users[USER_COUNT][3] = {
{"alex","nyHEICiuUs", "d6a2ce4283bdfea071d2af27ad6de360615ea454d478d123774150e8d5649b34"},
{"bob","PnwkfGpAYq", "c6fdbb54acf6e2b81a18bbbf0d141b036dc5debd6fdfa5d6e38adc7a6f535dff"},
{"alice","J9I5qqc06z", "45d614a4deecdaf2abe525615d1227752f179ce772a62af3e4052b9b4a0b366c"},
{"root","dctpH7M22w", "28dd1c6b2df1bdc74210b6fd792ab70a60615b70c8948795107a834be5e8a3ca"},
{"daemon","qlOHSpotdt", "627d40cc3f089faa8c1669f280ba6e74369e46113f59400653f2335267839770"},
{"user","SreBBDM0IT", "a62174a553f3bfa59b6f8685c4e78f546f3c4cc3ef2e1f0de61834b17e21bdc0"},
};

#define HASH_COUNT (10)

int check_passwd(unsigned char* passwd, unsigned char* salt, unsigned char* hash)
{
	struct stribog_ctx_t ctx;
	int i, j;
	char str_for_hash[1024];
	strcpy(str_for_hash,passwd);
	for ( i = 0 ; i < HASH_COUNT; i++ ) {		
		init(&ctx, HASH256);
		strcat(str_for_hash,salt);
		stribog(&ctx, (u8*)str_for_hash, strlen(str_for_hash));		
		for ( j = 0 ; j < OUTPUT_SIZE_256 ; j++)
			sprintf(str_for_hash+(j*2),"%02x",ctx.h[(OUTPUT_SIZE_256-1) - j]);
	}
	return (strcmp(str_for_hash,hash) == 0);
}

int main(int argc, char *argv[])
{
	int i,j;
	char *passwd = NULL;
	size_t len, size;
	FILE* fd;
	
	fd = fopen(argv[1],"r");

	while ((size = getline(&passwd,&len,fd)) != -1){
		if (passwd[--size] == '\n' ) passwd[size] = 0;
	   
		printf("Try: '%s'\n",passwd);
		for ( i = 0 ; i < USER_COUNT ; i++ ) {
			if(check_passwd(passwd,users[i][1],users[i][2])){
				printf("\r%s: %s\n",users[i][0],passwd);
				fclose(fd);
				return 0;
			}
		}
		free(passwd);
		passwd=NULL;
	}
	fclose(fd);
	printf("Password not found\n");
	return -1;
}

