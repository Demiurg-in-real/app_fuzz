#include <iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<csignal>
#include<cstring>
#include<pthread.h>
namespace fuzzing{
	static void *run(void *name1){
		for (){
			parpid=vfork();
			if(parpid == 0){
				execve(s, argv, NULL);
			}
			else {
				waitpid(parpid,&wstatus, WUNTRACED);
				if (WIFSIGNALED(wstatus)){
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
}

int main(){
	FILE *point;
	void *point1;
	point = (FILE *)point1;
	fuzzing::threads pthr;
	printf("%c\n",0x48);
	return 0;
}
