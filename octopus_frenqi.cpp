#include <iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<csignal>
#include<cstring>
#include<pthread.h>
namespace fuzzing{

	static pthread_mutex_t mtx1 = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t mtx2 = PTHREAD_MUTEX_INITIALIZER;

	struct info{
		char *name;
		char **args;
		int how_many;
		int start;
		int finish;
	}__attribute__((packed));

	static void *run(void *argss){
//		std::cout<<"Who I am?\n";
		pthread_mutex_lock(&mtx1);
		info *turtle = (info *)argss;
		int start = turtle->start;
		int finish = turtle->finish;
		char **ar;
//		std::cout<<"Her\n"<<finish<<"-ways\n";
		ar=turtle->args;
//		std::cout<<"Vot omo blin!\n";
		pthread_mutex_unlock(&mtx1);
		pid_t parpid;
		int wstatus;
		for (int i = start; i < finish; i++){
//			std::cout<<"kak okazalos - net\n";
			ar[1]=(char*)22;
//			std::cout<<"mb tut...\n";
			parpid=vfork();
			if(parpid == 0){
				execve(ar[0],ar, NULL);
			}
			else {
				waitpid(parpid,&wstatus, WUNTRACED);
				printf("%i - byte\n",start);
				pthread_mutex_lock(&mtx2);
				if (WIFSIGNALED(wstatus)){
					std::cout << strsignal(WTERMSIG(wstatus)) << "\n";
				}
				pthread_mutex_unlock(&mtx2);
			}
		}
		return (void *)parpid;
	}
	class threads{
		pthread_t proc[4];
		info *here = new info;
		void go();
	public:
		threads();
		~threads();
	};
	threads::threads(){
//		here = new info;
		std::cout<<"here\n";
//		sprintf(here->name,"c.out");
		here->name="c.out";
//		std::cout<<"vde\n";
		here->how_many=2;
		try{
			std::cout<<"Hi!\n";
			here->args =(char **) malloc(sizeof(char*)*here->how_many); //new char[here->how_many];
			printf("%x\n",here->args);
			printf("%s\n",here->name);
			here->args[0]=here->name;
			std::cout<<here->args[0]<<" - init complit\n";
			
		} catch(std::bad_alloc xa){
			std::cout<<"Blin\n";
		}
		go();
	};
	void threads::go(){
		std::cout<<"entered in prog\n";
		for(int i=0;i<4;i++){
			here->start=(i*256/4);
			here->finish=((i+1)*256/4);
			std::cout<<"why?\n";
			pthread_create(&proc[i],NULL,run,(void *)here);
			usleep(5);
		}
	};
	threads::~threads(){
		for(int l=0;l<4;l++) {
			pthread_cancel(proc[l]);
			std::cout <<"Process - "<< l << " says goodbay\n";
		}
		delete here;
		std::cout<<"Structure info says goodbay too!\n";
	}
}

int main(){
	std::cout<<"Bl\n";
	fuzzing::threads *pth;
	pth = new fuzzing::threads;
	printf("%c\n",0x48);
	delete pth;
	return 0;
}
