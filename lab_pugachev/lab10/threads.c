#include<pthread.h>
#include<stdio.h>
//#include<
int check(register int a){
	for(int b=0; b<10000;b++){
	       a=a%11+a*a-(2*a)%3;
	}
	return a;
}
int main(){
	register int a=0;
	int status_addr;
	pthread_t status[4];
	int stat[4];
	for (a=0;a<4;a++){
		stat[a]=pthread_create(&status[a],NULL, check,a);
	}
	for (a=0;a<4;a++){
		stat[a]=pthread_join(&status[a],(void**)&status_addr);
		printf("%d - pthread address, %i - value\n", status_addr, stat[a]);
	}
	return 0;
}

