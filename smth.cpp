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
	char *erro;
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
		waitpid(parpid,&wstatus, WUNTRACED);
		if (WIFSIGNALED(wstatus)){
			erro=strerror(WTERMSIG(wstatus));
			cout << "Sie error   "<< erro;
		}
	}
}
int main(){
	run like;
	like.get();
	cout << "Hello, Brothers!\n";
	return 0;
}
