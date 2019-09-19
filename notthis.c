#include<stdio.h>
int main()
{
	FILE *p=fopen("text.txt", "rt");
	if(p == NULL){
		//perror("fopen");
		//return -1;
	}
	fclose(p);
	return 0;
}
