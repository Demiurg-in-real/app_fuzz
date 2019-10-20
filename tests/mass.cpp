#include<iostream>
#include<cstdio>
int main(){
	char **buf;
	buf=(char**)malloc(sizeof(char*)*2);
	sprintf(&buf[0],"My nam is programm. And now we will speek about it.");
	*buf[1]=(char)63;
	std::cout<<"here\n";
	printf("%s\n%i\n",buf[0],*buf[1]);
	return 0;
}
