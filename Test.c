#include<pthread.h>
#include<stdio.h>
static void *ThreadFunc(void *args){
	printf("1\n");
	int *s=(int *)args;
	printf("%i\n",s);
	pid_t parpid=vfork();
	if(parpid == 0){
		execve("b.out",NULL,NULL);
	}
	else{
		printf("Hi Jack\n");
		return (void *) 14;//strlen(s);
	}
}
int main(){
	printf("pth\n");
	pthread_t pth;
	void *res;
	int s;
	printf("2\n");
	s = pthread_create(&pth, NULL, ThreadFunc, (void *)45);
	if(s != 0){
		perror("pthread_create");
		//errExitEN();
	}
	printf("Hi,bitch\n");
	s=pthread_join(pth,&res); 
	if(s != 0) perror("pthread_join");
	printf("Pthread returned %ld\n",(long)res);
	return 0;
}

