#include <iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<csignal>
#include<cstring>
#include<pthread.h>
using namespace std;

struct prog_name{
	char *pn[2];//={"b.out","c.out"}
	int counter;
}__attribute__((packed));

class run{
	pid_t forpid;
	pid_t parpid;
	int wstatus;
public:
	void get(prog_name nm);
}__attribute__((packed));
class proceses{
private:
	pthread_t proc[2];
	prog_name nam;
	run like;
public:
	proceses();
	void call_pr();
};
void run::get(prog_name nm){
	parpid=vfork();
	if(parpid == 0){
		execve(nm.pn[nm.counter], NULL, NULL);
	}
	else {
		cout << parpid << "\n";
		waitpid(parpid,&wstatus, WUNTRACED);
		if (WIFSIGNALED(wstatus)){
			cout << strsignal(WTERMSIG(wstatus)) << "\n";
		}
	}
}
proceses::proceses(){
	nam.pn[0]="b.out";
	nam.pn[1]="c.out";
}
void proceses::call_pr(){
	int a;
	for(int i=0; i<2;i++){
		nam.counter=i;
		a=pthread_create(proc[i],NULL,like.get,nam);
		if (a!=0) perror("pthread_create");
		pthread_detach(proc[i]);
	}
};

int main(){
	cout << "Hello, Brothers!\n";
	return 0;
}
