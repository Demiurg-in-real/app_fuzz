#include <iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<csignal>
#include<cstring>
#include<pthread.h>
namespace fuzzing{
	char iter[2];
	static void *run(void *name1){
		char *name=(char *)name1;
		char *s = (char *)name[0];
		pid_t parpid;
		int wstatus;
//		int counter=0;
		char hs[2];
		unsigned char y=0x0;
//		std::cout <<"1\n";
		for (iter[name[2]]=name[1];iter[name[1]]<name[1]+127;iter[name[2]]++){
			sprintf(hs,"%c\n",y);
			printf("%i\n",iter[name[2]]);
			char *argv[2]={s,hs};
//			for(int i=7; i>=0;i--) printf("%x",(*argv[1]>>i)&0x1);
//			printf(" - %i\n",name);
			parpid=vfork();
			if(parpid == 0){
				execve(s, argv, NULL);
			}
			else {
				waitpid(parpid,&wstatus, WUNTRACED);
				if (WIFSIGNALED(wstatus)){
					//std::cout<<"1";
					std::cout << strsignal(WTERMSIG(wstatus)) << "\n";
				}
			}
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
		unsigned char sen[3];
		char *he="c.out";
		(char*)(sen+0)=(char *)"c.out";
		printf("%s\n",sen[0]);
		//&sen[0]=(char*)"c.out";
		for (int i=0;i<1;i++){
			sen[1]=((char)i) * 128;
			sen[2]=(char)i;
			pthread_create(&proc[i],NULL,run,(void *)sen);
			for(int l=0; l<3;l++) printf("%s --",sen[0]);
//			pthread_join(proc[i], &res);
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
