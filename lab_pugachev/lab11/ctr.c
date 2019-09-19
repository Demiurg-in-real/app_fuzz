#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
	
int isPowerOfTwo (uint8_t x)
{
	return ((x != 0) && ((x & (~x + 1)) == x));
}


int main(){
	uint8_t *buf;
	int r;
	dup2(r,fileno(stdout));
	fread(buf,sizeof(char),1,stdout);
	isPowerOfTwo(*buf);
	return 0;
}
