#include <stdio.h>
#include<string.h>
void main(int argc, char argv[]){
	char buf[512],buf2[512];
	int i;
	snprintf(buf,512,"qwert");
	snprintf(buf2,512,"qwer");
	if(( i=strcmp(buf,buf2)) == 0){
		printf("Equal\n");
	}
	else{
		printf("Not equal %d\n",i);
	}
}
