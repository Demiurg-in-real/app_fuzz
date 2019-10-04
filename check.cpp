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
		char *names[2];
		int counter;
	}__attribute__((packed));
	class execing{
		pid_t check_pid;
		static inf gett;
		//static void *run();
		friend class threads;
	public:
		execing();
		static void *run(void);
		~execing();
	};
	class threads{
		pthread_t proc[2];
		void go();
		execing here;
	public:
		threads();
		~threads();
	};
	void execing::*run(void){
		std::cout << "Process " << gett.counter << " just started";
		pid_t parpid;
		int wstatus;
		parpid=vfork();
		if(parpid == 0){
			execve(gett.names[gett.counter], NULL, NULL);
		}
		else {
			std::cout << parpid << "\n";
			waitpid(parpid,&wstatus, WUNTRACED);
			if (WIFSIGNALED(wstatus)){
				std::cout << strsignal(WTERMSIG(wstatus)) << "\n";
			}
		}
	}
	execing::execing(){
		gett.names[0]="b.out";
		gett.names[1]="c.out";
		for (int i=0; i<2;i++){
			printf("%s are initializated\n",gett.names[i]);
		}
		//check="Kill them!";
	}
	execing::~execing(){
		std::cout<<"I am dead\n";
	}
	threads::threads(){
		go();
	}
	threads::~threads(){
		//execing here();
		for (int i=0; i<2; i++){
			pthread_cancel(proc[i]);
		}
		//std::cout<<here.check;
	}
	void threads::go(){
		//execing here();
		void *p;
		//p=&here.run;
		for(int i=0;i<2;i++){
			here.gett.counter=i;
			pthread_create(&proc[i],NULL,here.run,NULL);
		}
	}
//}


//using namespace fuzzing;

int main(){
	threads pthr();
//	pthr.go();
	std::cout << "Hello, Brothers!\n";
	//delete(pr);
	return 0;
}
