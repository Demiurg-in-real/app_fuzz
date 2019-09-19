#include <stdio.h>
#include <stdint.h>

int main(){
	FILE *w=fopen("/dev/urandom","rb");
	FILE *r=fopen(stdout, "rb");
	uint8_t *check;
	check=fread(&check,sizeof(char), 1,w);
	printf("%02x\n", check);
	fclose(w);
	fclose(r);
	return 0;
}

