#include <iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<csignal>
#include<cstring>

using namespace std;
class run{
	pid_t forpid;
	pid_t parpid;
	int wstatus;
	char erro[80];
	char *check;
public:
	void get();
}__attribute__((packed));
void run::get(){
	parpid=fork();
	if(parpid == 0){
		//forpid=getpid();
		execve("b.out", NULL, NULL);
	}
	else {
		cout << parpid << "\n";
		waitpid(parpid,&wstatus, WUNTRACED);
		if (WIFSIGNALED(wstatus)){

			//check=strerror_r(WTERMSIG(wstatus), erro, 79);// == -1) perror("strerror_r");
			cout << "Sie error   "<< strerror(WTERMSIG(wstatus)) << "\n";
			cout <<  stderr <<"\n";
			//for (int i=0; i<80; i++) printf("%i\n", erro[i]);
		}
	}
}
int main(){
	run like;
	like.get();
	cout << "Hello, Brothers!\n";
	return 0;
}
