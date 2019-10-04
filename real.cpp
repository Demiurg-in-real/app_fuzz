#include <iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<csignal>
#include<cstring>
#include<pthread.h>
//namespace fuzzing{
	struct inf{
		char names[2];
		int counter;
	}__attribute__((packed));
	int y=0;
	static void * run(void *name){
		char *s = (char *)name;
		//int *id=(int *)name[1];
		std::cout << "Process " << y << " just started";
		pid_t parpid;
		int wstatus;
		parpid=vfork();
		if(parpid == 0){
			execve(s, NULL, NULL);
		}
		else {
			std::cout << parpid <<" - it returned by parpid | "<<getpid() << " - returned by getpid\n";
			waitpid(parpid,&wstatus, WUNTRACED);
			if (WIFSIGNALED(wstatus)){
				std::cout << strsignal(WTERMSIG(wstatus)) << "\n";
			}
		}
		return (void *)parpid;
	}
	class threads{
		pthread_t proc[2];
		int c;
		inf here;
		void *res;
		void go();
	public:
		threads();
		~threads();
	};
	threads::threads(){
		//*(here.names[0])=(char *)"b.out";
		//*(here.names[1])=(char *)"c.out";
		go();
	}
	threads::~threads(){
		for (int i=0;i<2;i++){
			pthread_cancel(proc[i]);
			std::cout<<i<<"\n";
		}
	}
	void threads::go(){
		for (int i=0;i<2;i++){	
			y++;
			pthread_create(&proc[i],NULL,run,(void *)"c.out");
			pthread_join(proc[i], &res);
			std::cout<<(pid_t *)res<<" - proc return | proc id - "<<proc[i]<<"\n";
		}
//		for (int i=0; i<2;i++){
//			pthread_join(proc[i], &res);
//			std::cout<<(pid_t *)res<<" - proc return | proc id - "<<proc[i]<<"\n";
//		}
	};
//}
//using namespace fuzzing;

int main(){
	threads pthr;
	std::cout << "Hello, Brothers!\n";
	return 0;
}
