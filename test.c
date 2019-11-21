#include<stdio.h>
#include<unistd.h>
int main(){
	int eup[2];
	pipe(eup);
//	fflush(stdout);
	dup2(eup[0],0);
1	dup2(eup[1],1);
	fflush(stdout);
	printf("Hi!\n");
	char buf[16];
	fread(buf,sizeof(char),16,stdout);
//	read(eup[1],buf,16);
//	close(eup[0]);
//	close(eup[1]);
	fprintf(stderr,"%s\n", buf);
	return 0;
}
