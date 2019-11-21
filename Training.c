#include<stdio.h>
#include<signal.h>
#include<stdbool.h>
#include<stdlib.h>
void handle(int ir){
	printf("Signal %i said by!\n",ir);
	exit(EXIT_SUCCESS);
}
int main(){
	void (*p)(int)=&handle;
	pid_t r;
	r=getpid();
	signal(SIGTERM,p);
	while(true){
		printf("%i\n",r);
//		signal(SIGINT, p);
	}
	return 0;
}
