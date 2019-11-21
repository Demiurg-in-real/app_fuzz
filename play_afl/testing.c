#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>
int main(int argc, char* argv[]){
	if(argc == 1 || argc >3) return EXIT_FAILURE;
	FILE *f=NULL;
	FILE *f1=NULL;
	if( ((f = fopen(argv[1],"rb+")) == NULL) /*|| ((f1 = fopen(argv[2],"rb") == NULL)*/) perror("fopen");
	uint8_t kelloc[8];
	uint8_t size;
	fread(&size, sizeof(char),1,f);
//	for (int i=0; i<size;i++){
       		fread(kelloc, sizeof(char), size, f);
//	}
	printf("%i\n",size);
	for (int i=7;i>-1;i--){
		printf("%x\n", kelloc[i]);
		if(0xff>kelloc[i] >= 0xaf){
			printf("%i\n",kelloc[i]/0);
		}
	}
	return EXIT_SUCCESS;
}

