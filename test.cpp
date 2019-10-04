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
	static prog_name nm;
	void get(prog_name nam); // container for start a new process without replacing the old one 
public:
	run(); // here we init prog_name struct  with names of programs with faults to run
	static void * inIt(void *coute);// from here we will start new programm with control from thread
}__attribute__((packed)); 

class proces{
	int control;
	pthread_t proc[2];
	run fuck; // need for use in this class
public:
	proces();//init and run pthreads
	~proces();//delete threads
};

run::run(){
	nm.pn[0]="b.out";
	nm.pn[1]="c.out";
};

void run::get(prog_name nam){
	cout << "Process " << nam.counter << " just started";
	parpid=vfork();
	if(parpid == 0){
		execve(nam.pn[nam.counter], NULL, NULL);
	}
	else {
		cout << parpid << "\n";
		waitpid(parpid,&wstatus, WUNTRACED);
		if (WIFSIGNALED(wstatus)){
			cout << strsignal(WTERMSIG(wstatus)) << "\n";
		}
	}
}
static void * run::inIt(void *coute){
	int *couter=(void *) coute;
	nm.counter=*couter;
	get(nm);
}

proces::proces(){
	for (int y=0;y<2;y++){
		void *fucking=(void*)y;
		cout<<"Pthread "<<y<<" is running";
		control=pthread_create(&proc[y],NULL, fuck.inIt,fucking);
		if(control != 0){
			perror("pthread_create");
			exit(0);
		}
		pthread_detach(proc[y]);
	}
}

proces::~proces(){
	for(int j=0;j<2;j++) delete(&proc[j]);
}


int main(){
	proces pr();
	cout << "Hello, Brothers!\n";
	//delete(pr);
	return 0;
}
