#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[]){
//	FILE *p = fopen(argv[1],"r");
	char heh[2];
//	fread(heh, sizeof(char), 1,p);
//	fclose(p);
	fgets(heh,2,stdin);
	int i = atoi(heh);
	printf("%i\n", 43/i);
	return 0;
}
