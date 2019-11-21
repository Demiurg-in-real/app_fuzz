#include<iostream>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<string>
using namespace std;
class fuz{
	public:
		fuz(int arc, char**arv);
		void compile();
		char *afl[3]={"../afl/afl-clang",NULL,NULL};//Here and dawn is the ways two binaries of fuzzers
		char *hong[3]={"./honggfuzz/hfuzz_cc/hfuzz-gcc",NULL,NULL};
		char *afl_fuzzer[3];
		//char *j[6]={"HOME=/","PATH=/bin:/usr/bin","TZ=UTC0","USER=beelzebub","LOGNAME=tarzan",0};
		char *j[2]={"PATH=/bin",NULL};
		uint8_t *buf;
		int wstatus;
		uint8_t order;
};
fuz::fuz(int arc,char**arv){
	if(arc > 3){
		std::cout<<"You can't run with more than two args.\nTry again.\n"<<std::endl;
		exit(-1);
	}
	hong[1]=arv[1];
	afl[1]=arv[1];
	order=0x1;
	compile();
}
void fuz::compile(){
	int pip[2];
	pid_t chil;
	if(pipe(pip) == -1){
		perror("pipe");
		exit(0);
	}
	switch (chil=vfork()){
		case -1:
			perror("fork");
			exit(-1);
		case 0:
			close(pip[0]);
			dup2(pip[1],2);
			if(order == 1){
				printf("hm\n");
				execve(hong[0],hong,j);//,NULL);
			}
			if(order == 2){
				printf("bl\n");
				execve(afl[0],afl,j);
			}
			perror("execve");
			exit(-1);
		default:
			buf = new uint8_t [10000];
			close(pip[1]);
			sleep(1);
			read(pip[0], buf,10000);
			close(pip[0]);
			waitpid(chil,&wstatus, WNOHANG);
			if(WIFEXITED(wstatus)){
				for(int i=0; i<10000-10;i++)if(buf[i] == 101 && buf[i+1]==114 && buf[i+2] == 114 && buf[i+3] == 111 && buf[i+4] == 114) printf("Hey!\n");
			}
			close(pip[1]);
			delete [] buf;
	}
}
void fuz::fuzzing(){
	int pip1[2];
	int pip2[2];
	pid_t exp;
	if(pipe(pip1) == -1 && pipe(pip2) == -1){
		perror("pipe");
		exit(-1);
	}
	switch(exp=vfork()){
		case -1:
			perror("vfork");
			exit(-1);
		case 0:
			close(pip1[0]);
			close(pip2[0]);
			dup2(pip1[1],2);//stderr
			dup2(pip2[1],1);//stdout
			if(order == 1) execve(hong_fuzzer[0], hong_fuzzer,j);
			if(order == 2) execve(afl_fuzzer[0],afl_fuzzer,j);
			perror("execve");
			exit(-1);
		default:
			//...
	}
}


int main(int argc, char* argv[]){
	fuz go(argc, argv);
	return 0;
}

