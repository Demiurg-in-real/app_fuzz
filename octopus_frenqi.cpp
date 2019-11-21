#include <iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<csignal>
#include<cstring>
#include<pthread.h>
void handle(int sig){
	while(true){
		if((waitpid(-1,NULL,WNOHANG))>0) continue;
		else break;
	}
	printf("By!\n");
}

static void run(){
	pid_t parpid;
	char *arg[3]={NULL, "0", NULL};
	arg[0]="a.out";
	int wstatus;
	parpid = vfork();
//	int (*p)(int)=handle;
		if(parpid == 0){
			if(execve(arg[0],arg, NULL) == -1) perror("execve");
		}
		else {
//			waitpid(parpid,&wstatus,WNOHANG);
			sleep(1);
			signal(SIGCHLD, handle);
			kill(parpid,SIGTERM);
			while(1);
			waitpid(parpid,&wstatus, WUNTRACED);
//			sleep(1);
//			kill(parpid, SIGTERM);
//			while(1);

			if (WIFSIGNALED(wstatus)){
				printf("%s\nNorm\n",strsignal(WTERMSIG(wstatus)));
			}

		}
//	return (void *)parpid;
}

int main(){
	run();
	return 0;
}
