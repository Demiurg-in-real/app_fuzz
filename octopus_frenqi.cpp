#include <iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<csignal>
#include<cstring>
#include<pthread.h>
namespace fuzzing{
	int y=0;
	static void *run(void *name){
		char *s = (char *)name;
		pid_t parpid;
		int wstatus;
		int counter=0;
		char hs[2];
		unsigned char y=0x0;
		std::cout <<"1\n";
		while(counter < 256){
			sprintf(hs,"%c\n",y);
//			printf("%i\n",counter);
			char *argv[2]={s,hs};
//			for(int i=7; i>=0;i--) printf("%x",(*argv[1]>>i)&0x1);
			//printf(" - %i\n",y);
			parpid=vfork();
			if(parpid == 0){
				execve(s, argv, NULL);
			}
			else {
				waitpid(parpid,&wstatus, WUNTRACED);
				if (WIFSIGNALED(wstatus)){
					//std::cout<<"1";
//					std::cout << strsignal(WTERMSIG(wstatus)) << "\n";
				}
			}
			counter++;
			y++;
		}
		return (void *)parpid;
	}
	class threads{
		pthread_t proc[2];
		int c;
		void *res;
		void go();
	public:
		threads();
		~threads();
	};
	threads::threads(){
		go();
	}
	threads::~threads(){
		for (int i=0;i<2;i++){
//			std::cout<<i<<"\n";
			pthread_cancel(proc[i]);
		}
	}
	void threads::go(){
		for (int i=0;i<1;i++){	
			y++;
			pthread_create(&proc[i],NULL,run,(void *)"c.out");
			pthread_join(proc[i], &res);
		}
	};
}

int main(){
	FILE *point;
	void *point1;
	point = (FILE *)point1;
	fuzzing::threads pthr;
	printf("%c\n",0x48);
	return 0;
}
